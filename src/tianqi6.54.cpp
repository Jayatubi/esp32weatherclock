#include "EPD.h"
#include "GUI_Paint.h"
#include "weather_icons.h"
#include "images.h"
//#include "ProvincesCities.h"
#include "api_key.h"
#include "cities.h"
#include "portal.h"
#include <cstdlib>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <esp_sleep.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <mutex>
#include <vector>
#include <map>

#define DEFAULT_VREF    1100        // 默认参考电压
#define CUSTOM_CONVERSION_FACTOR 2.989  // 调整后的校正转换因子
#define FILTER_SIZE 10

#define FULL_BATTERY_VOLTAGE 4.2  // 满电电压
#define LOW_BATTERY_VOLTAGE 3.3   // 低电压阈值

// 使用ADC1通道0 (GPIO36)
static const adc1_channel_t channel = ADC1_CHANNEL_0;
static const adc_atten_t atten = ADC_ATTEN_DB_11;        // 使用11dB衰减，以扩展测量范围到3.3V
static esp_adc_cal_characteristics_t *adc_chars = NULL;
float voltage_filter[FILTER_SIZE] = {0};
int filter_index = 0;

const char *paramFile = "/params.txt";
const char *rebootCounterFile = "/rebootCounter.txt"; // 文件名记录重启计数
const char *timeServer = "http://worldtimeapi.org/api/timezone/Asia/Shanghai";
const char *weatherServer = "https://api.seniverse.com/v3/weather/daily.json";
const char *weatherApiKey = WEATHER_API_KEY; // 请前往“心知天气”免费申请密钥

PAINT_TIME sPaint_time;
UBYTE *BlackImage = NULL;
char currentDate[11];
char currentDayOfWeek[4];
char weatherText[20];
char weatherCodeDay[4];
char highTemp[4];
char lowTemp[4];
char windDirection[10];
char humidity[4];
char futureDates[3][6];
char futureWeatherCodes[3][4];
char futureHighTemps[3][4];
char futureLowTemps[3][4];

int tuX = 170;
int tuY = 45;
int rebootCounter = 0; // 记录重启计数

char ssid[32] = "";
char password[32] = "";
char city[32] = "";
int birthdayMonth = 1; // 默认生日月份为1月
int birthdayDay = 1;   // 默认生日日期为1日
int syncHours[] = {0, 3, 6, 9, 12, 15, 18, 21}; // 默认同步时间节点
int syncHoursCount = 8; // 默认同步时间节点数量

bool connectToWiFi();

bool getTimeFromServer();

bool getWeatherFromServer();

void updateDisplay(PAINT_TIME *sPaint_time, bool fullRefresh);

void displayDateTime(const char *dateStr, const char *dayOfWeekStr, PAINT_TIME *sPaint_time);

void displayWeatherInfo();

void drawBoldChar(int x, int y, char ch, sFONT *font, int color, int bgcolor);

bool validateTime(bool timeStatus);

bool validateWeather(bool weatherStatus);

bool retryFunction(bool (*func)(), int maxRetries, const char *errMsg, bool resetOnFailure);

void readParamsFromSPIFFS();

void writeParamsToSPIFFS(const char *ssid,
                         const char *password,
                         const char *city,
                         int birthdayMonth,
                         int birthdayDay,
                         const int *syncHours,
                         int syncHoursCount);

void clearParams();

void displayLogo();

void displayTextBelowLogo(const char *text, int ipX, int espY);

void clearScreen();

bool isAnyParamEmpty();

void startConfigPortal();

void capitalizeFirstLetter(char *str);

void checkAndDisplayBirthday();

void displayFutureWeatherInfo(int startX);

void displayIcon(const unsigned char *icon, int x, int y, int width, int height);

void readRebootCounter();

void writeRebootCounter(int counter);

void resetRebootCounter();

void showWIFIIcon(int x, int y);

void hideWIFIIcon(int x, int y);

void IRAM_ATTR handleButtonWIFI();  // 函数声明
void resetToFactorySettings(); // 新增：重置为出厂设置

const char *logFile = "/log.txt";
const size_t maxLogSize = 1024 * 50; // 假设50KB为日志文件最大大小，您可以根据需求调整
const unsigned long logDuration = 2 * 24 * 60 * 60 * 1000; // 2天的毫秒数

void writeLog(const char *message);

void checkLogSize();

String readLogFile();

std::mutex epdMutex;

WebServer server(80);

unsigned long apStartTime = 0;
bool apActive = false;
volatile bool buttonWIFIed = false; // 用于跟踪按钮是否被按下
volatile unsigned long lastButtonPressTime = 0; // 用于记录上一次按键按下的时间
volatile int buttonPressCount = 0; // 用于记录按键按下的次数
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED; // 互斥量

// 全局变量来存储上一次的电池电压和百分比
float lastBatteryVoltage = 0.0;
float lastBatteryPercentage = 0.0;

void writeLog(const char *message) {
  checkLogSize(); // 检查并管理日志文件大小
  File file = SPIFFS.open(logFile, FILE_APPEND);
  if (file) {
    file.println(message);
    file.close();
  } else {
    Serial.println("Unable to open log file for writing");
  }
}

void checkLogSize() {
  File file = SPIFFS.open(logFile, FILE_READ);
  if (file) {
    size_t fileSize = file.size();
    file.close();
    if (fileSize > maxLogSize) {
      // 如果文件超过最大大小限制，则清除文件内容
      file = SPIFFS.open(logFile, FILE_WRITE);
      if (file) {
        file.close();
        Serial.println("Log file cleared");
      }
    }
  }
}

String readLogFile() {
  File file = SPIFFS.open(logFile, FILE_READ);
  if (!file) {
    return "Unable to open log file for reading";
  }

  String content = file.readString();
  file.close();
  return content;
}

#define LOG_SERIAL_OUTPUT

#ifdef LOG_SERIAL_OUTPUT
#define SerialPrint(...) do { Serial.print(__VA_ARGS__); logSerial(__VA_ARGS__); } while (0)
#define SerialPrintln(...) do { Serial.println(__VA_ARGS__); logSerialln(__VA_ARGS__); } while (0)
#else
#define SerialPrint(...) Serial.print(__VA_ARGS__)
#define SerialPrintln(...) Serial.println(__VA_ARGS__)
#endif

void logSerial(const char *message) {
  writeLog(message);
}

void logSerialln(const char *message) {
  writeLog(message);
  writeLog("\n");
}

void logSerial(int value) {
  writeLog(String(value).c_str());
}

void logSerialln(int value) {
  writeLog(String(value).c_str());
  writeLog("\n");
}

void logSerial(uint8_t value) {
  writeLog(String(value).c_str());
}

void logSerialln(uint8_t value) {
  writeLog(String(value).c_str());
  writeLog("\n");
}

void logSerial(IPAddress ip) {
  writeLog(ip.toString().c_str());
}

void logSerialln(IPAddress ip) {
  writeLog(ip.toString().c_str());
  writeLog("\n");
}

void init_adc() {
  // 配置ADC
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(channel, atten);

  // 使用默认参考电压1100mV进行特性校准
  adc_chars = (esp_adc_cal_characteristics_t *) calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_characterize(ADC_UNIT_1, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
}

float filterVoltage(float new_voltage) {
  voltage_filter[filter_index] = new_voltage;
  filter_index = (filter_index + 1) % FILTER_SIZE;

  float sum = 0;
  for (int i = 0; i < FILTER_SIZE; i++) {
    sum += voltage_filter[i];
  }
  return sum / FILTER_SIZE;
}

float measureVBAT() {
  uint32_t adc_reading = adc1_get_raw(channel);

  // 使用库进行校准
  uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
  float actual_voltage = (float) voltage / 1000.0; // 将mV转换为V
  actual_voltage *= CUSTOM_CONVERSION_FACTOR; // 应用校正转换因子

  // 应用滤波
  actual_voltage = filterVoltage(actual_voltage);

  // 打印原始值和校准电压
  String logMessage = "Raw value: 0x" + String(adc_reading, HEX) + ", voltage: " + String(actual_voltage) + " V";
  SerialPrintln(logMessage.c_str());
  return actual_voltage;
}

float calculateBatteryPercentage(float voltage) {
  // 计算电量百分比
  if (voltage >= FULL_BATTERY_VOLTAGE) {
    return 100.0;
  } else if (voltage <= LOW_BATTERY_VOLTAGE) {
    return 0.0;
  } else {
    return (voltage - LOW_BATTERY_VOLTAGE) / (FULL_BATTERY_VOLTAGE - LOW_BATTERY_VOLTAGE) * 100.0;
  }
}

void displayBatteryStatus(int X, int Y, bool useLastValues = false) {
  std::lock_guard<std::mutex> lock(epdMutex);  // 锁定

  float batteryVoltage;
  float batteryPercentage;

  if (useLastValues) {
    batteryVoltage = lastBatteryVoltage;
    batteryPercentage = lastBatteryPercentage;
  } else {
    batteryVoltage = measureVBAT();
    batteryPercentage = calculateBatteryPercentage(batteryVoltage);
    lastBatteryVoltage = batteryVoltage;
    lastBatteryPercentage = batteryPercentage;
  }

  // 清除之前的内容，确保不会超出边界
  Paint_ClearWindows(X, Y, X + 16, Y + 16, WHITE);

  // 选择相应的电池图标
  const uint8_t *batteryIcon;
  if (batteryPercentage <= 5) {
    batteryIcon = battery0_5_Icon;
  } else if (batteryPercentage <= 25) {
    batteryIcon = battery5_25_Icon;
  } else if (batteryPercentage <= 50) {
    batteryIcon = battery25_50_Icon;
  } else if (batteryPercentage <= 75) {
    batteryIcon = battery50_75_Icon;
  } else {
    batteryIcon = battery75_100_Icon;
  }

  // 显示电池图标
  displayIcon(batteryIcon, X, Y, 16, 16);

  // 更新显示内容
  EPD_2in13_V4_Display_Partial(BlackImage);
}

void displayIcon(const unsigned char *icon, int x, int y, int width, int height) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (icon[i * (width / 8) + j / 8] & (0x80 >> (j % 8))) {
        Paint_DrawPoint(x + j, y + i, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
      } else {
        Paint_DrawPoint(x + j, y + i, WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
      }
    }
  }
}

void displayLogo() {
  Paint_Clear(WHITE);
  displayIcon(my_Logo_bits, 93, 27, my_Logo_width, my_Logo_height);
  EPD_2in13_V4_Display_Base(BlackImage);
}

void displayTextBelowLogo(const char *text, int ipX, int espY) {
  SerialPrint("Displaying text: ");
  SerialPrintln(text);
  if (ipX < 0) ipX = 0;

  int textStrLen = strlen(text) * (Font16.Width - 2);
  Paint_ClearWindows(ipX, espY, ipX + textStrLen, espY + Font16.Height, WHITE);

  int x = ipX;
  for (int i = 0; i < strlen(text); i++) {
    Paint_DrawChar(x, espY, text[i], &Font16, BLACK, WHITE);
    x += Font16.Width - 2;
  }

  EPD_2in13_V4_Display_Partial(BlackImage);
}

void clearScreen() {
  Paint_Clear(WHITE);
  EPD_2in13_V4_Display_Base(BlackImage);
}

bool isAnyParamEmpty() {
  return (strlen(ssid) == 0 || strlen(password) == 0 || strlen(city) == 0 || syncHoursCount == 0);
}

void startConfigPortal() {
  SerialPrintln("Starting configuration portal...");
  WiFi.softAP("ESP32_Config");
  apStartTime = millis();
  apActive = true;
  IPAddress myIP = WiFi.softAPIP();
  SerialPrint("AP IP address: ");
  SerialPrintln(myIP);

  server.on("/", []() {
    String html(portal_html_template);

    // 读取已有参数
    readParamsFromSPIFFS();

    // 扫描WiFi网络
    int n = WiFi.scanNetworks();
    JsonDocument ssidDoc;
    for (int i = 0; i < n; ++i) {
      auto entry = ssidDoc.add<JsonObject>();
      entry["ssid"] = WiFi.SSID(i);
      entry["rssi"] = WiFi.RSSI(i);
    }
    String ssidJson;
    serializeJson(ssidDoc, ssidJson);
    html.replace("{{ ssids }}", ssidJson);
    html.replace("{{ selected_ssid }}", ssid);

    JsonDocument cityDoc;
    deserializeJson(cityDoc, city_json);
    String province_cities;
    serializeJson(cityDoc, province_cities);
    html.replace("{{ province_cities }}", province_cities);
    html.replace("{{ selected_city }}", city);

    JsonDocument syncHourDoc;
    for (const auto hour : syncHours) {
      syncHourDoc.add(hour);
    }
    String syncHourJson;
    serializeJson(syncHourDoc, syncHourJson);
    html.replace("{{ sync_hours }}", syncHourJson);


    html.replace("{{ birthday_month }}", String(birthdayMonth));
    html.replace("{{ birthday_day }}", String(birthdayDay));

    server.send(200, "text/html", html);
  });

  server.on("/submit", []() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    String city = server.arg("city");
    int birthday_month = server.arg("birthday_month").toInt();
    int birthday_day = server.arg("birthday_day").toInt();
    std::vector<int> syncHoursList = {0}; // 初始化包含0
    for (int i = 0; i < server.args(); i++) {
      if (server.argName(i) == "sync_hours" && server.arg(i).toInt() != 0) {
        syncHoursList.push_back(server.arg(i).toInt());
      }
    }

    // 格式化生日
    birthdayMonth = birthday_month;
    birthdayDay = birthday_day;

    // 处理同步时间点
    syncHoursCount = syncHoursList.size();
    for (int i = 0; i < syncHoursCount; i++) {
      syncHours[i] = syncHoursList[i];
    }

    writeParamsToSPIFFS(ssid.c_str(),
                        password.c_str(),
                        city.c_str(),
                        birthday_month,
                        birthday_day,
                        syncHours,
                        syncHoursCount);

    server.send(200, "text/html", "Parameters saved. Rebooting...");
    delay(1000);
    ESP.restart();
  });

  server.on("/logs", []() {
    String logContent = readLogFile();
    server.send(200, "text/plain", logContent);
  });

  server.begin();
  SerialPrintln("HTTP server started for configuration");
}

void setup() {
  Serial.begin(115200);
  SerialPrintln("EPD_2in13_V4_test Demo");
  DEV_Module_Init();

  SerialPrintln("e-Paper Init and Clear...");
  EPD_2in13_V4_Init();
  EPD_2in13_V4_Clear();

  UWORD Imagesize = ((EPD_2in13_V4_WIDTH % 8 == 0) ? (EPD_2in13_V4_WIDTH / 8) : (EPD_2in13_V4_WIDTH / 8 + 1)) * EPD_2in13_V4_HEIGHT;
  BlackImage = (UBYTE *) malloc(Imagesize);
  if (BlackImage == NULL) {
    SerialPrintln("Failed to allocate memory for black and white image...");
    while (1);
  }
  SerialPrintln("Paint_NewImage");
  Paint_NewImage(BlackImage, EPD_2in13_V4_WIDTH, EPD_2in13_V4_HEIGHT, 90, WHITE);
  Paint_Clear(WHITE);

  displayLogo();

  if (!SPIFFS.begin(true)) {
    SerialPrintln("SPIFFS initialization failed");
    return;
  }
  writeLog("System startup");

  readRebootCounter();

  readParamsFromSPIFFS();

  if (isAnyParamEmpty()) {
    int ipX = 12;
    int espY = 90;
    displayTextBelowLogo("ESP32-Config--192.168.4.1", ipX, espY);
    startConfigPortal();
  } else {
    if (!retryFunction(connectToWiFi, 10, "WiFi connection failed", true)) {
      ESP.restart();
    }

    int ipX = 40;
    int espY = 90;
    displayTextBelowLogo("Designed by Chaeng", ipX, espY);

    if (!retryFunction(getTimeFromServer, 5, "Failed to get time", false)) {
      rebootCounter++;
      writeRebootCounter(rebootCounter);
      if (rebootCounter >= 3) {
        clearParams();
        resetRebootCounter();
      }
      ESP.restart();
    }
    SerialPrint("Time obtained: ");
    SerialPrint(sPaint_time.Hour);
    SerialPrint(":");
    SerialPrintln(sPaint_time.Min);
    SerialPrint("Date obtained: ");
    SerialPrintln(currentDate);
    SerialPrint("Day of the week obtained: ");
    SerialPrintln(currentDayOfWeek);

    if (!retryFunction(getWeatherFromServer, 5, "Failed to get weather information", false)) {
      rebootCounter++;
      writeRebootCounter(rebootCounter);
      if (rebootCounter >= 3) {
        clearParams();
        resetRebootCounter();
      }
      ESP.restart();
    }
    SerialPrintln("Successfully obtained weather information");

    clearScreen();

    displayDateTime(currentDate, currentDayOfWeek, &sPaint_time);
    displayWeatherInfo();

    checkAndDisplayBirthday();

    EPD_2in13_V4_Display_Base(BlackImage);
    DEV_Delay_ms(2000);

    SerialPrintln("Partial refresh");
    Paint_SelectImage(BlackImage);

    startConfigPortal();
  }

  // 设置IO12为输入并启用上拉电阻
  pinMode(12, INPUT_PULLUP);

  // 设置外部中断
  attachInterrupt(digitalPinToInterrupt(12), handleButtonWIFI, FALLING);

  // 设置轻度休眠模式下的GPIO唤醒
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 0); // 设置为低电平唤醒

  // 初始化ADC
  init_adc();
}

void IRAM_ATTR handleButtonWIFI() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 50) { // 50毫秒的去抖动时间
    portENTER_CRITICAL_ISR(&mux);
    lastButtonPressTime = interruptTime;
    buttonPressCount++;
    portEXIT_CRITICAL_ISR(&mux);
  }
  lastInterruptTime = interruptTime;
}

bool connectToWiFi() {
  WiFi.begin(ssid, password);
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    retryCount++;
    SerialPrintln("Attempting to connect to WiFi...");
    if (WiFi.status() != WL_CONNECTED && retryCount > 10) {
      SerialPrintln("WiFi connection failed");
      writeLog("WiFi connection failed");
      return false;
    }
  }
  SerialPrintln("WiFi connected successfully");
  SerialPrint("IP Address: ");
  SerialPrintln(WiFi.localIP());
  writeLog("WiFi connected successfully");
  return true;
}

bool getTimeFromServer() {
  if (WiFi.status() != WL_CONNECTED) {
    SerialPrintln("WiFi not connected");
    writeLog("WiFi not connected");
    return false;
  }

  HTTPClient http;
  http.begin(timeServer);
  http.setTimeout(5000);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    SerialPrint("Time Payload: ");
    SerialPrintln(payload.c_str());

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      SerialPrint("JSON parsing failed: ");
      SerialPrintln(error.c_str());
      http.end();
      writeLog("JSON parsing failed");
      return false;
    }

    String datetime = doc["datetime"];
    String date = datetime.substring(0, 10);
    String time = datetime.substring(11, 16);

    date.replace('-', '.');
    strcpy(currentDate, date.c_str());

    sPaint_time.Hour = time.substring(0, 2).toInt();
    sPaint_time.Min = time.substring(3, 5).toInt();

    int dayOfWeek = doc["day_of_week"];
    const char *daysOfWeekShort[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    strcpy(currentDayOfWeek, daysOfWeekShort[dayOfWeek]);

    http.end();
    writeLog("Time obtained successfully");
    return true;
  } else {
    SerialPrint("Failed to get time, HTTP code: ");
    SerialPrintln(httpCode);
    http.end();
    writeLog("Failed to get time");
    return false;
  }
}

bool getWeatherFromServer() {
  if (WiFi.status() != WL_CONNECTED) {
    SerialPrintln("WiFi not connected");
    writeLog("WiFi not connected");
    return false;
  }

  HTTPClient http;
  String weatherUrl = String(weatherServer) + "?key=" + weatherApiKey + "&location=" + city + "&language=en&unit=c";
  http.begin(weatherUrl);
  http.setTimeout(5000);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    SerialPrint("Weather Payload: ");
    SerialPrintln(payload.c_str());

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      SerialPrint("JSON parsing failed: ");
      SerialPrintln(error.c_str());
      http.end();
      writeLog("JSON parsing failed");
      return false;
    }

    if (doc["results"] && doc["results"][0]["daily"] && doc["results"][0]["daily"][0]) {
      JsonArray daily = doc["results"][0]["daily"];

      for (int i = 0; i < 3; i++) {
        JsonObject day = daily[i];
        strncpy(futureDates[i], day["date"].as<const char *>() + 5, 5);
        futureDates[i][5] = '\0';
        strncpy(futureWeatherCodes[i], day["code_day"], sizeof(futureWeatherCodes[i]) - 1);
        futureWeatherCodes[i][sizeof(futureWeatherCodes[i]) - 1] = '\0';
        strncpy(futureHighTemps[i], day["high"], sizeof(futureHighTemps[i]) - 1);
        futureHighTemps[i][sizeof(futureHighTemps[i]) - 1] = '\0';
        strncpy(futureLowTemps[i], day["low"], sizeof(futureLowTemps[i]) - 1);
        futureLowTemps[i][sizeof(futureLowTemps[i]) - 1] = '\0';
      }

      JsonObject today = daily[0];
      const char *text_day = today["text_day"];
      const char *code_day = today["code_day"];
      const char *high = today["high"];
      const char *low = today["low"];
      const char *wind_direction = today["wind_direction"];
      const char *humidity_value = today["humidity"];

      strncpy(weatherText, text_day, sizeof(weatherText) - 1);
      strncpy(weatherCodeDay, code_day, sizeof(weatherCodeDay) - 1);
      strncpy(highTemp, high, sizeof(highTemp) - 1);
      strncpy(lowTemp, low, sizeof(lowTemp) - 1);
      strncpy(windDirection, wind_direction, sizeof(windDirection) - 1);
      strncpy(humidity, humidity_value, sizeof(humidity) - 1);

      weatherText[sizeof(weatherText) - 1] = '\0';
      weatherCodeDay[sizeof(weatherCodeDay) - 1] = '\0';
      highTemp[sizeof(highTemp) - 1] = '\0';
      lowTemp[sizeof(lowTemp) - 1] = '\0';
      windDirection[sizeof(windDirection) - 1] = '\0';
      humidity[sizeof(humidity) - 1] = '\0';

      http.end();
      writeLog("Weather information obtained successfully");
      return true;
    } else {
      SerialPrintln("JSON data parsing failed");
      http.end();
      writeLog("JSON data parsing failed");
      return false;
    }
  } else {
    SerialPrint("Failed to get weather information, HTTP code: ");
    SerialPrintln(httpCode);
    http.end();
    writeLog("Failed to get weather information");
    return false;
  }
}


void displayDateTime(const char *dateStr, const char *dayOfWeekStr, PAINT_TIME *sPaint_time) {
  std::lock_guard<std::mutex> lock(epdMutex);  // 锁定
  Paint_ClearWindows(0, 0, EPD_2in13_V4_WIDTH, 0 + Font16.Height, WHITE);

  int dateX = 0;
  int dayOfWeekX = 132;
  int timeX = 200;
  int y = 0;

  for (int i = 0; dateStr[i] != '\0'; i++) {
    drawBoldChar(dateX + (i * (Font16.Width - 2)), y, dateStr[i], &Font16, BLACK, WHITE);
  }

  for (int i = 0; dayOfWeekStr[i] != '\0'; i++) {
    drawBoldChar(dayOfWeekX + (i * (Font16.Width - 0)), y, dayOfWeekStr[i], &Font16, BLACK, WHITE);
  }

  char timeString[6];
  sprintf(timeString, "%02d:%02d", sPaint_time->Hour, sPaint_time->Min);
  for (int i = 0; timeString[i] != '\0'; i++) {
    drawBoldChar(timeX + (i * (Font16.Width - 2)), y, timeString[i], &Font16, BLACK, WHITE);
  }
}

void displayWeatherInfo() {
  int wxY = Font16.Height + 15;
  int lineSpacing = Font16.Height;

  const char *labels[] = {"City:", "WX:", "Temp:", "Wind:", "Humidity:"};
  const char *values[] = {city, weatherText, nullptr, windDirection, nullptr};

  char tempStr[10];
  sprintf(tempStr, "%s/%sC", highTemp, lowTemp);
  values[2] = tempStr;

  char humidityStr[10];
  sprintf(humidityStr, "%s%%", humidity);
  values[4] = humidityStr;

  for (int i = 0; i < 5; i++) {
    int x = 0;
    for (int j = 0; labels[i][j] != '\0'; j++) {
      Paint_DrawChar(x + (j * (Font16.Width - 2)), wxY, labels[i][j], &Font16, BLACK, WHITE);
    }
    x += strlen(labels[i]) * (Font16.Width - 2);
    for (int j = 0; values[i][j] != '\0'; j++) {
      Paint_DrawChar(x + (j * (Font16.Width - 2)), wxY, values[i][j], &Font16, BLACK, WHITE);
    }
    wxY += lineSpacing;
  }

  const unsigned char *icon = getWeatherIcon(weatherCodeDay);
  displayIcon(icon, tuX, tuY, 48, 48);
}

void displayFutureWeatherInfo(int startX) {
  int startY = 15;
  int columnSpacing = 80;
  int lineSpacing = Font16.Height + 5;

  for (int i = 0; i < 3; i++) {
    int x = startX + i * columnSpacing;
    int y = startY;

    for (int j = 0; futureDates[i][j] != '\0'; j++) {
      Paint_DrawChar(x + (j * (Font16.Width - 2)), y, futureDates[i][j], &Font16, BLACK, WHITE);
    }
    y += lineSpacing;

    const unsigned char *icon = getWeatherIcon(futureWeatherCodes[i]);
    displayIcon(icon, x, y, 48, 48);
    y += 48 + 5;

    char tempStr[10];
    sprintf(tempStr, "%s/%sC", futureHighTemps[i], futureLowTemps[i]);
    for (int j = 0; tempStr[j] != '\0'; j++) {
      Paint_DrawChar(x + (j * (Font16.Width - 2)), y, tempStr[j], &Font16, BLACK, WHITE);
    }
  }

  EPD_2in13_V4_Display_Base(BlackImage);
}

void updateDisplay(PAINT_TIME *sPaint_time, bool fullRefresh) {
  char timeString[6];
  sprintf(timeString, "%02d:%02d", sPaint_time->Hour, sPaint_time->Min);

  int timeX = 200;
  Paint_ClearWindows(timeX, 0, timeX + (5 * (Font16.Width - 2)), 0 + Font16.Height, WHITE);

  int y = 0;
  for (int i = 0; timeString[i] != '\0'; i++) {
    drawBoldChar(timeX + (i * (Font16.Width - 2)), y, timeString[i], &Font16, BLACK, WHITE);
  }

  if (sPaint_time->Hour == 0 && sPaint_time->Min == 0) {
    // 去掉incrementDate逻辑
    Paint_ClearWindows(0, 0, EPD_2in13_V4_WIDTH, 0 + Font16.Height, WHITE); // Clear entire header area
    displayDateTime(currentDate, currentDayOfWeek, sPaint_time);
  }

  if (fullRefresh) {
    EPD_2in13_V4_Display_Base(BlackImage);
  } else {
    EPD_2in13_V4_Display_Partial(BlackImage);
  }

  SerialPrint("Display updated: ");
  SerialPrint(sPaint_time->Hour);
  SerialPrint(":");
  SerialPrintln(sPaint_time->Min);

  // 如果AP不活动，则进入轻度休眠59秒
  if (!apActive) {
    esp_sleep_enable_timer_wakeup(59 * 1000000);
    esp_light_sleep_start();
  }
}

void drawBoldChar(int x, int y, char ch, sFONT *font, int color, int bgcolor) {
  Paint_DrawChar(x, y, ch, font, color, bgcolor);
  Paint_DrawChar(x - 1, y, ch, font, color, bgcolor);
  Paint_DrawChar(x, y + 1, ch, font, color, bgcolor);
}

bool validateTime(bool timeStatus) {
  if (timeStatus && sPaint_time.Hour >= 0 && sPaint_time.Hour < 24 &&
    sPaint_time.Min >= 0 && sPaint_time.Min < 60) {
    return true;
  }
  return false;
}

bool validateWeather(bool weatherStatus) {
  if (weatherStatus && strlen(weatherText) > 0 && strlen(weatherCodeDay) > 0 &&
    strlen(highTemp) > 0 && strlen(lowTemp) > 0 &&
    strlen(windDirection) > 0 && strlen(humidity) > 0) {
    return true;
  }
  return false;
}

bool retryFunction(bool (*func)(), int maxRetries, const char *errMsg, bool resetOnFailure) {
  int retries = 0;
  while (retries < maxRetries) {
    if (func()) {
      return true;
    } else {
      SerialPrintln(errMsg);
      retries++;
      delay(2000);
    }
  }
  if (resetOnFailure) {
    clearParams();
  }
  return false;
}

void clearParams() {
  strcpy(ssid, "");
  strcpy(password, "");
  strcpy(city, "");
  birthdayMonth = 1;
  birthdayDay = 1;
  syncHoursCount = 8;
  int defaultSyncHours[] = {0, 3, 6, 9, 12, 15, 18, 21};
  memcpy(syncHours, defaultSyncHours, sizeof(defaultSyncHours));
  writeParamsToSPIFFS(ssid, password, city, birthdayMonth, birthdayDay, syncHours, syncHoursCount);
}

void readParamsFromSPIFFS() {
  File file = SPIFFS.open(paramFile, "r");
  if (!file) {
    SerialPrintln("Parameter file not found, using default values");
    clearParams(); // 设置默认值
    return;
  }

  String content = file.readString();
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, content);
  if (error) {
    SerialPrintln("Failed to parse parameter file, using default values");
    clearParams(); // 设置默认值
  } else {
    strlcpy(ssid, doc["ssid"] | "", sizeof(ssid));
    strlcpy(password, doc["password"] | "", sizeof(password));
    strlcpy(city, doc["city"] | "", sizeof(city));
    birthdayMonth = doc["birthday_month"] | 1;
    birthdayDay = doc["birthday_day"] | 1;
    syncHoursCount = doc["syncHoursCount"];
    for (int i = 0; i < syncHoursCount; i++) {
      syncHours[i] = doc["syncHours"][i];
    }
  }

  file.close();
}

void writeParamsToSPIFFS(const char *ssid,
                         const char *password,
                         const char *city,
                         int birthdayMonth,
                         int birthdayDay,
                         const int *syncHours,
                         int syncHoursCount) {
  JsonDocument doc;
  doc["ssid"] = ssid;
  doc["password"] = password;
  doc["city"] = city;
  doc["birthday_month"] = birthdayMonth;
  doc["birthday_day"] = birthdayDay;
  doc["syncHoursCount"] = syncHoursCount;
  for (int i = 0; i < syncHoursCount; i++) {
    doc["syncHours"][i] = syncHours[i];
  }

  File file = SPIFFS.open(paramFile, "w");
  if (!file) {
    SerialPrintln("Unable to open file for writing");
    return;
  }

  serializeJson(doc, file);
  file.close();
}

void capitalizeFirstLetter(char *str) {
  if (str != NULL && str[0] != '\0') {
    str[0] = toupper(str[0]);
    for (int i = 1; str[i] != '\0'; i++) {
      str[i] = tolower(str[i]);
    }
  }
}

void checkAndDisplayBirthday() {
  char currentMonthDay[6];
  strncpy(currentMonthDay, currentDate + 5, 5);
  currentMonthDay[5] = '\0';

  char birthdayFormatted[6];
  snprintf(birthdayFormatted, sizeof(birthdayFormatted), "%02d.%02d", birthdayMonth, birthdayDay);

  if (strcmp(currentMonthDay, birthdayFormatted) == 0) {
    SerialPrintln("Displaying birthday wishes");
    displayTextBelowLogo("Happy Birthday!", 60, 15);
  }
}

void readRebootCounter() {
  File file = SPIFFS.open(rebootCounterFile, "r");
  if (file) {
    rebootCounter = file.parseInt();
    file.close();
  } else {
    rebootCounter = 0;
  }
}

void writeRebootCounter(int counter) {
  File file = SPIFFS.open(rebootCounterFile, "w");
  if (file) {
    file.print(counter);
    file.close();
  }
}

void resetRebootCounter() {
  writeRebootCounter(0);
}

void showWIFIIcon(int x, int y) {
  displayIcon(WIFI_Icon, x, y, WIFI_Icon_width, WIFI_Icon_height);
  EPD_2in13_V4_Display_Partial(BlackImage);
  SerialPrintln("WIFI icon displayed");
}

void hideWIFIIcon(int x, int y) {
  Paint_ClearWindows(x, y, x + WIFI_Icon_width, y + WIFI_Icon_height, WHITE);
  EPD_2in13_V4_Display_Partial(BlackImage);
  SerialPrintln("WIFI icon hidden");
}

void resetToFactorySettings() {
  SerialPrintln("Resetting to factory settings...");
  clearParams();
  if (SPIFFS.remove(logFile)) {
    SerialPrintln("Log file cleared");
  } else {
    SerialPrintln("Failed to clear log file");
  }
  ESP.restart();
}

void loop() {
  static unsigned long previousMillis = 0;
  static unsigned long batteryPreviousMillis = 0;
  static int batteryCheckCount = 0;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 60000) { // 60000 毫秒即 1 分钟
    previousMillis = currentMillis;

    sPaint_time.Min = sPaint_time.Min + 1;
    if (sPaint_time.Min == 60) {
      sPaint_time.Hour = sPaint_time.Hour + 1;
      sPaint_time.Min = 0;
      if (sPaint_time.Hour == 24) {
        sPaint_time.Hour = 0;
        // 去掉incrementDate逻辑
      }
      EPD_2in13_V4_Display_Base(BlackImage);
    }

    updateDisplay(&sPaint_time, false);

    bool shouldSync = false;
    for (int i = 0; i < syncHoursCount; i++) {
      if (sPaint_time.Hour == syncHours[i] && sPaint_time.Min == 0) {
        shouldSync = true;
        break;
      }
    }

    if (shouldSync) {
      if (sPaint_time.Hour == 0) {
        // 在0点时，只进行时间同步，不更新天气
        SerialPrintln("Starting time synchronization at 0:00...");
        bool timeSuccess = retryFunction(connectToWiFi, 10, "WiFi connection failed", true) && retryFunction(
          getTimeFromServer,
          5,
          "Failed to get time",
          false);
        if (timeSuccess) {
          SerialPrintln("Time synchronization successful.");
          SerialPrint("Time updated: ");
          SerialPrint(sPaint_time.Hour);
          SerialPrint(":");
          SerialPrintln(sPaint_time.Min);
          SerialPrint("Date updated: ");
          SerialPrintln(currentDate);
          SerialPrint("Day of the week updated: ");
          SerialPrintln(currentDayOfWeek);
          clearScreen();
          displayDateTime(currentDate, currentDayOfWeek, &sPaint_time);
          displayWeatherInfo(); // 保留天气信息
          checkAndDisplayBirthday();
          displayBatteryStatus(234, 106, true);
          EPD_2in13_V4_Display_Base(BlackImage);
        } else {
          SerialPrintln("Time synchronization failed. Rebooting...");
          ESP.restart();
        }
      } else {
        // 在其他同步时间点，同步时间和天气
        SerialPrintln("Starting synchronization...");
        bool timeSuccess = retryFunction(connectToWiFi, 10, "WiFi connection failed", true) && retryFunction(
          getTimeFromServer,
          5,
          "Failed to get time",
          false);
        bool weatherSuccess = retryFunction(getWeatherFromServer, 5, "Failed to get weather information", false);

        if (timeSuccess) {
          SerialPrintln("Time synchronization successful.");
          SerialPrint("Time updated: ");
          SerialPrint(sPaint_time.Hour);
          SerialPrint(":");
          SerialPrintln(sPaint_time.Min);
          SerialPrint("Date updated: ");
          SerialPrintln(currentDate);
          SerialPrint("Day of the week updated: ");
          SerialPrintln(currentDayOfWeek);
          clearScreen();
          displayDateTime(currentDate, currentDayOfWeek, &sPaint_time);
        } else {
          SerialPrintln("Time synchronization failed. Rebooting...");
          ESP.restart();
        }

        if (weatherSuccess) {
          SerialPrintln("Weather synchronization successful.");
          displayWeatherInfo();
        } else {
          SerialPrintln("Weather synchronization failed.");
        }
        displayBatteryStatus(234, 106, true);
        checkAndDisplayBirthday();
        EPD_2in13_V4_Display_Base(BlackImage);
      }
    }
  }

  if (apActive && (currentMillis - apStartTime >= 300000)) {
    WiFi.disconnect(true);
    WiFi.softAPdisconnect(true);
    apActive = false;
    SerialPrintln("AP closed to save power");
  }

  // 控制displayBatteryStatus的调用频率
  if ((batteryCheckCount < 15 && currentMillis - batteryPreviousMillis >= 1000) ||
    (batteryCheckCount >= 15 && currentMillis - batteryPreviousMillis >= 300000)) {
    batteryPreviousMillis = currentMillis;
    displayBatteryStatus(234, 106); // 在指定坐标显示电池状态
    batteryCheckCount++;
  }

  // 判断AP状态，显示或隐藏WiFi图标
  if (apActive) {
    showWIFIIcon(218, 106); // 显示WiFi图标
  } else {
    hideWIFIIcon(218, 106); // 隐藏WiFi图标
  }

  // 检查按钮是否被按下
  unsigned long currentTime = millis();
  if (buttonPressCount > 0 && currentTime - lastButtonPressTime > 500) { // 500毫秒的双击检测时间窗口
    if (buttonPressCount == 2) {
      SerialPrintln("Double click detected, displaying weather information for the next three days");
      clearScreen();
      displayFutureWeatherInfo(20);
      delay(5000);
      SerialPrintln("Resuming normal weather clock display");
      clearScreen();
      displayDateTime(currentDate, currentDayOfWeek, &sPaint_time);
      displayWeatherInfo();
      checkAndDisplayBirthday();
      displayBatteryStatus(234, 106, true);
      EPD_2in13_V4_Display_Base(BlackImage);
    }
    buttonPressCount = 0;
  }

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "factory_reset") {
      resetToFactorySettings();
    } else if (command == "set_2359") {
      SerialPrintln("Setting time to 23:59 for testing midnight transition.");
      sPaint_time.Hour = 23;
      sPaint_time.Min = 59;
      // 调整日期和星期
      strcpy(currentDate, "2024.07.26");
      strcpy(currentDayOfWeek, "FRI");
      updateDisplay(&sPaint_time, false);
    }
  }
  server.handleClient();
}
