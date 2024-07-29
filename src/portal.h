#pragma once

const char* portal_html_template = R"--(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <style>
            body { display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; font-family: Arial, sans-serif; }
            form { display: flex; flex-direction: column; align-items: center; max-width: 400px; width: 100%; }
            input, select { margin: 5px; padding: 10px; font-size: 16px; width: 100%; box-sizing: border-box; }
            input[type='submit'] { margin-top: 20px; padding: 10px 20px; width: auto; }
            .hour-label { display: inline-block; width: 60px; margin: 2px; }
            .hour-grid { display: grid; grid-template-columns: repeat(6, 1fr); gap: 5px; }
            .inline-group { display: flex; justify-content: space-between; width: 100%; }
            .inline-group select { width: 48%; }
        </style>
        <script>
        </script>
    </head>
    <body>
        <form action="/submit" method="POST">
            SSID: <select name="ssid" id="ssidSelect"></select>
            Password: <input type="password" name="password" value="{{ passwordValue }}">
            <div class="inline-group">
                Province: <select id="province" name="province" onchange="updateCities()"></select>
                City: <select id="city" name="city"></select>
            </div>
            <div class="inline-group">
                Birthday Month: <select name="birthday_month" id="birthdayMonth"></select>
                Birthday Day: <select name="birthday_day" id="birthdayDay"></select>
            </div>
            Sync Hours:
            <div class="hour-grid" id="syncHour">
                <label class="hour-label"><input type="checkbox" name="sync_hours" value="0" checked disabled>0:00</label>
            </div>
            <input type="submit" value="Submit">
        </form>
        <script>
            const ssids = {{ ssids }};
            const provinceCities = {{ province_cities }};
            const syncHours = {{ sync_hours }};
            function updateCities() {
                var province = document.getElementById('province').value;
                var nodeCity = document.getElementById('city');
                nodeCity.innerHTML = '';
                const cities = provinceCities[province];
                if (cities) {
                    if (cities.length === 1) {
                        var option = document.createElement('option');
                        option.value = cities[0].code;
                        option.text = cities[0].name;
                        nodeCity.appendChild(option);
                    } else {
                        for (var i = 1; i < cities.length; i++) {
                            var option = document.createElement('option');
                            option.value = cities[i].code;
                            option.text = cities[i].name;
                            nodeCity.appendChild(option);
                        }
                    }
                }
            }

            const nodeSsid = document.getElementById('ssidSelect');
            for (const ssid of ssids) {
                const option = document.createElement('option');
                option.value = ssid.name;
                option.text = `${ssid.ssid} (${ssid.rssi} dm)`;
                nodeSsid.appendChild(option);
            }

            nodeSsid.value = '{{ selected_ssid }}';

            const nodeProvince = document.getElementById('province');
            for (let i = 0; i < provinceCities.length; i++) {
                const cities = provinceCities[i]
                const option = document.createElement('option');
                option.value = i;
                option.text = cities[0].name;
                nodeProvince.appendChild(option);

                if (cities.find(e => e.code == '{{ selected_city }}')) {
                    nodeProvince.value = i;
                }
            }
            updateCities();

            const nodeMonth = document.getElementById('birthdayMonth');
            for (let i = 1; i <= 12; i++) {
                const option = document.createElement('option');
                option.value = i;
                option.text = i;
                nodeMonth.appendChild(option);
            }
            nodeMonth.value = '{{ birthday_month }}';

            const nodeDay = document.getElementById('birthdayDay');
            for (let i = 1; i <= 31; i++) {
                const option = document.createElement('option');
                option.value = i;
                option.text = i;
                nodeDay.appendChild(option);
            }
            nodeDay.value = '{{ birthday_day }}';

            const nodeHour = document.getElementById('syncHour');
            for (let i = 1; i < 24; i++) {
                const label = document.createElement('label');
                label.className = 'hour-label';
                label.for = `hour_${i}`;

                const input = document.createElement('input');
                input.type = 'checkbox';
                input.name = 'sync_hours';
                input.value = i;
                input.checked = syncHours.includes(i);
                input.id = `hour_${i}`;

                label.appendChild(input);
                label.append(`${i}:00`);
                nodeHour.appendChild(label);
            }
        </script>
    </body>
</html>
)--";
