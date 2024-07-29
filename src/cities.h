#pragma once

const char* city_json = R"(
[
  [
    {
      "code": "BeiJing",
      "name": "北京"
    }
  ],
  [
    {
      "code": "TianJin",
      "name": "天津"
    }
  ],
  [
    {
      "code": "ShangHai",
      "name": "上海"
    }
  ],
  [
    {
      "code": "ChongQing",
      "name": "重庆"
    }
  ],
  [
    {
      "code": "HeBei",
      "name": "河北"
    },
    {
      "code": "ShiJiaZhuang",
      "name": "石家庄"
    },
    {
      "code": "TangShan",
      "name": "唐山"
    },
    {
      "code": "QinHuangDao",
      "name": "秦皇岛"
    },
    {
      "code": "HanDan",
      "name": "邯郸"
    },
    {
      "code": "XingTai",
      "name": "邢台"
    },
    {
      "code": "BaoDing",
      "name": "保定"
    },
    {
      "code": "ZhangJiaKou",
      "name": "张家口"
    },
    {
      "code": "ChengDe",
      "name": "承德"
    },
    {
      "code": "CangZhou",
      "name": "沧州"
    },
    {
      "code": "LangFang",
      "name": "廊坊"
    },
    {
      "code": "HengShui",
      "name": "衡水"
    }
  ],
  [
    {
      "code": "ShanXi",
      "name": "山西"
    },
    {
      "code": "TaiYuan",
      "name": "太原"
    },
    {
      "code": "DaTong",
      "name": "大同"
    },
    {
      "code": "YangQuan",
      "name": "阳泉"
    },
    {
      "code": "ChangZhi",
      "name": "长治"
    },
    {
      "code": "JinCheng",
      "name": "晋城"
    },
    {
      "code": "ShuoZhou",
      "name": "朔州"
    },
    {
      "code": "JinZhong",
      "name": "晋中"
    },
    {
      "code": "YunCheng",
      "name": "运城"
    },
    {
      "code": "XinZhou",
      "name": "忻州"
    },
    {
      "code": "LinFen",
      "name": "临汾"
    },
    {
      "code": "LvLiang",
      "name": "吕梁"
    }
  ],
  [
    {
      "code": "NeiMengGu",
      "name": "内蒙古"
    },
    {
      "code": "HuHeHaoTe",
      "name": "呼和浩特"
    },
    {
      "code": "BaoTou",
      "name": "包头"
    },
    {
      "code": "WuHai",
      "name": "乌海"
    },
    {
      "code": "ChiFeng",
      "name": "赤峰"
    },
    {
      "code": "TongLiao",
      "name": "通辽"
    },
    {
      "code": "EErDuoSi",
      "name": "鄂尔多斯"
    },
    {
      "code": "HuLunBeiEr",
      "name": "呼伦贝尔"
    },
    {
      "code": "BaYanNaoEr",
      "name": "巴彦淖尔"
    },
    {
      "code": "WuLanChaBu",
      "name": "乌兰察布"
    }
  ],
  [
    {
      "code": "LiaoNing",
      "name": "辽宁"
    },
    {
      "code": "ShenYang",
      "name": "沈阳"
    },
    {
      "code": "DaLian",
      "name": "大连"
    },
    {
      "code": "AnShan",
      "name": "鞍山"
    },
    {
      "code": "FuShun",
      "name": "抚顺"
    },
    {
      "code": "BenXi",
      "name": "本溪"
    },
    {
      "code": "DanDong",
      "name": "丹东"
    },
    {
      "code": "JinZhou",
      "name": "锦州"
    },
    {
      "code": "YingKou",
      "name": "营口"
    },
    {
      "code": "FuXin",
      "name": "阜新"
    },
    {
      "code": "LiaoYang",
      "name": "辽阳"
    },
    {
      "code": "PanJin",
      "name": "盘锦"
    },
    {
      "code": "TieLing",
      "name": "铁岭"
    },
    {
      "code": "ChaoYang",
      "name": "朝阳"
    },
    {
      "code": "HuLuDao",
      "name": "葫芦岛"
    }
  ],
  [
    {
      "code": "JiLin",
      "name": "吉林"
    },
    {
      "code": "ChangChun",
      "name": "长春"
    },
    {
      "code": "JiLin",
      "name": "吉林"
    },
    {
      "code": "SiPing",
      "name": "四平"
    },
    {
      "code": "LiaoYuan",
      "name": "辽源"
    },
    {
      "code": "TongHua",
      "name": "通化"
    },
    {
      "code": "BaiShan",
      "name": "白山"
    },
    {
      "code": "SongYuan",
      "name": "松原"
    },
    {
      "code": "BaiCheng",
      "name": "白城"
    },
    {
      "code": "YanBian",
      "name": "延边"
    }
  ],
  [
    {
      "code": "HeiLongJiang",
      "name": "黑龙江"
    },
    {
      "code": "HaErBin",
      "name": "哈尔滨"
    },
    {
      "code": "QiQiHaEr",
      "name": "齐齐哈尔"
    },
    {
      "code": "JiXi",
      "name": "鸡西"
    },
    {
      "code": "HeGang",
      "name": "鹤岗"
    },
    {
      "code": "ShuangYaShan",
      "name": "双鸭山"
    },
    {
      "code": "DaQing",
      "name": "大庆"
    },
    {
      "code": "YiChun",
      "name": "伊春"
    },
    {
      "code": "JiaMuSi",
      "name": "佳木斯"
    },
    {
      "code": "QiTaiHe",
      "name": "七台河"
    },
    {
      "code": "MuDanJiang",
      "name": "牡丹江"
    },
    {
      "code": "HeiHe",
      "name": "黑河"
    },
    {
      "code": "SuiHua",
      "name": "绥化"
    },
    {
      "code": "DaXingAnLing",
      "name": "大兴安岭"
    }
  ],
  [
    {
      "code": "JiangSu",
      "name": "江苏"
    },
    {
      "code": "NanJing",
      "name": "南京"
    },
    {
      "code": "WuXi",
      "name": "无锡"
    },
    {
      "code": "XuZhou",
      "name": "徐州"
    },
    {
      "code": "ChangZhou",
      "name": "常州"
    },
    {
      "code": "SuZhou",
      "name": "苏州"
    },
    {
      "code": "NanTong",
      "name": "南通"
    },
    {
      "code": "LianYunGang",
      "name": "连云港"
    },
    {
      "code": "HuaiAn",
      "name": "淮安"
    },
    {
      "code": "YanCheng",
      "name": "盐城"
    },
    {
      "code": "YangZhou",
      "name": "扬州"
    },
    {
      "code": "ZhenJiang",
      "name": "镇江"
    },
    {
      "code": "TaiZhou",
      "name": "泰州"
    },
    {
      "code": "SuQian",
      "name": "宿迁"
    }
  ],
  [
    {
      "code": "ZheJiang",
      "name": "浙江"
    },
    {
      "code": "HangZhou",
      "name": "杭州"
    },
    {
      "code": "NingBo",
      "name": "宁波"
    },
    {
      "code": "WenZhou",
      "name": "温州"
    },
    {
      "code": "JiaXing",
      "name": "嘉兴"
    },
    {
      "code": "HuZhou",
      "name": "湖州"
    },
    {
      "code": "ShaoXing",
      "name": "绍兴"
    },
    {
      "code": "JinHua",
      "name": "金华"
    },
    {
      "code": "QuZhou",
      "name": "衢州"
    },
    {
      "code": "ZhouShan",
      "name": "舟山"
    },
    {
      "code": "TaiZhou",
      "name": "台州"
    },
    {
      "code": "LiShui",
      "name": "丽水"
    }
  ],
  [
    {
      "code": "AnHui",
      "name": "安徽"
    },
    {
      "code": "HeFei",
      "name": "合肥"
    },
    {
      "code": "WuHu",
      "name": "芜湖"
    },
    {
      "code": "BengBu",
      "name": "蚌埠"
    },
    {
      "code": "HuaiNan",
      "name": "淮南"
    },
    {
      "code": "MaAnShan",
      "name": "马鞍山"
    },
    {
      "code": "HuaiBei",
      "name": "淮北"
    },
    {
      "code": "TongLing",
      "name": "铜陵"
    },
    {
      "code": "AnQing",
      "name": "安庆"
    },
    {
      "code": "HuangShan",
      "name": "黄山"
    },
    {
      "code": "ChuZhou",
      "name": "滁州"
    },
    {
      "code": "FuYang",
      "name": "阜阳"
    },
    {
      "code": "SuZhou",
      "name": "宿州"
    },
    {
      "code": "LuAn",
      "name": "六安"
    },
    {
      "code": "BoZhou",
      "name": "亳州"
    },
    {
      "code": "ChiZhou",
      "name": "池州"
    },
    {
      "code": "XuanCheng",
      "name": "宣城"
    }
  ],
  [
    {
      "code": "FuJian",
      "name": "福建"
    },
    {
      "code": "FuZhou",
      "name": "福州"
    },
    {
      "code": "XiaMen",
      "name": "厦门"
    },
    {
      "code": "PuTian",
      "name": "莆田"
    },
    {
      "code": "SanMing",
      "name": "三明"
    },
    {
      "code": "QuanZhou",
      "name": "泉州"
    },
    {
      "code": "ZhangZhou",
      "name": "漳州"
    },
    {
      "code": "NanPing",
      "name": "南平"
    },
    {
      "code": "LongYan",
      "name": "龙岩"
    },
    {
      "code": "NingDe",
      "name": "宁德"
    }
  ],
  [
    {
      "code": "JiangXi",
      "name": "江西"
    },
    {
      "code": "NanChang",
      "name": "南昌"
    },
    {
      "code": "JingDeZhen",
      "name": "景德镇"
    },
    {
      "code": "PingXiang",
      "name": "萍乡"
    },
    {
      "code": "JiuJiang",
      "name": "九江"
    },
    {
      "code": "XinYu",
      "name": "新余"
    },
    {
      "code": "YingTan",
      "name": "鹰潭"
    },
    {
      "code": "GanZhou",
      "name": "赣州"
    },
    {
      "code": "JiAn",
      "name": "吉安"
    },
    {
      "code": "YiChun",
      "name": "宜春"
    },
    {
      "code": "FuZhou",
      "name": "抚州"
    },
    {
      "code": "ShangRao",
      "name": "上饶"
    }
  ],
  [
    {
      "code": "ShanDong",
      "name": "山东"
    },
    {
      "code": "JiNan",
      "name": "济南"
    },
    {
      "code": "QingDao",
      "name": "青岛"
    },
    {
      "code": "ZiBo",
      "name": "淄博"
    },
    {
      "code": "ZaoZhuang",
      "name": "枣庄"
    },
    {
      "code": "DongYing",
      "name": "东营"
    },
    {
      "code": "YanTai",
      "name": "烟台"
    },
    {
      "code": "WeiFang",
      "name": "潍坊"
    },
    {
      "code": "JiNing",
      "name": "济宁"
    },
    {
      "code": "TaiAn",
      "name": "泰安"
    },
    {
      "code": "WeiHai",
      "name": "威海"
    },
    {
      "code": "RiZhao",
      "name": "日照"
    },
    {
      "code": "LinYi",
      "name": "临沂"
    },
    {
      "code": "DeZhou",
      "name": "德州"
    },
    {
      "code": "LiaoCheng",
      "name": "聊城"
    },
    {
      "code": "BinZhou",
      "name": "滨州"
    },
    {
      "code": "HeZe",
      "name": "菏泽"
    }
  ],
  [
    {
      "code": "HeNan",
      "name": "河南"
    },
    {
      "code": "ZhengZhou",
      "name": "郑州"
    },
    {
      "code": "KaiFeng",
      "name": "开封"
    },
    {
      "code": "LuoYang",
      "name": "洛阳"
    },
    {
      "code": "PingDingShan",
      "name": "平顶山"
    },
    {
      "code": "AnYang",
      "name": "安阳"
    },
    {
      "code": "HeBi",
      "name": "鹤壁"
    },
    {
      "code": "XinXiang",
      "name": "新乡"
    },
    {
      "code": "JiaoZuo",
      "name": "焦作"
    },
    {
      "code": "PuYang",
      "name": "濮阳"
    },
    {
      "code": "XuChang",
      "name": "许昌"
    },
    {
      "code": "LuoHe",
      "name": "漯河"
    },
    {
      "code": "SanMenXia",
      "name": "三门峡"
    },
    {
      "code": "NanYang",
      "name": "南阳"
    },
    {
      "code": "ShangQiu",
      "name": "商丘"
    },
    {
      "code": "XinYang",
      "name": "信阳"
    },
    {
      "code": "ZhouKou",
      "name": "周口"
    },
    {
      "code": "ZhuMaDian",
      "name": "驻马店"
    }
  ],
  [
    {
      "code": "HuBei",
      "name": "湖北"
    },
    {
      "code": "WuHan",
      "name": "武汉"
    },
    {
      "code": "HuangShi",
      "name": "黄石"
    },
    {
      "code": "ShiYan",
      "name": "十堰"
    },
    {
      "code": "YiChang",
      "name": "宜昌"
    },
    {
      "code": "XiangYang",
      "name": "襄阳"
    },
    {
      "code": "EZhou",
      "name": "鄂州"
    },
    {
      "code": "JingMen",
      "name": "荆门"
    },
    {
      "code": "XiaoGan",
      "name": "孝感"
    },
    {
      "code": "JingZhou",
      "name": "荆州"
    },
    {
      "code": "HuangGang",
      "name": "黄冈"
    },
    {
      "code": "XianNing",
      "name": "咸宁"
    },
    {
      "code": "SuiZhou",
      "name": "随州"
    },
    {
      "code": "EnShi",
      "name": "恩施"
    },
    {
      "code": "XianTao",
      "name": "仙桃"
    }
  ],
  [
    {
      "code": "HuNan",
      "name": "湖南"
    },
    {
      "code": "ChangSha",
      "name": "长沙"
    },
    {
      "code": "ZhuZhou",
      "name": "株洲"
    },
    {
      "code": "XiangTan",
      "name": "湘潭"
    },
    {
      "code": "HengYang",
      "name": "衡阳"
    },
    {
      "code": "ShaoYang",
      "name": "邵阳"
    },
    {
      "code": "YueYang",
      "name": "岳阳"
    },
    {
      "code": "ChangDe",
      "name": "常德"
    },
    {
      "code": "ZhangJiaJie",
      "name": "张家界"
    },
    {
      "code": "YiYang",
      "name": "益阳"
    },
    {
      "code": "ChenZhou",
      "name": "郴州"
    },
    {
      "code": "YongZhou",
      "name": "永州"
    },
    {
      "code": "HuaiHua",
      "name": "怀化"
    },
    {
      "code": "LouDi",
      "name": "娄底"
    }
  ],
  [
    {
      "code": "GuangDong",
      "name": "广东"
    },
    {
      "code": "GuangZhou",
      "name": "广州"
    },
    {
      "code": "ShenZhen",
      "name": "深圳"
    },
    {
      "code": "ZhuHai",
      "name": "珠海"
    },
    {
      "code": "ShaoGuan",
      "name": "韶关"
    },
    {
      "code": "ShanTou",
      "name": "汕头"
    },
    {
      "code": "FoShan",
      "name": "佛山"
    },
    {
      "code": "JiangMen",
      "name": "江门"
    },
    {
      "code": "ZhanJiang",
      "name": "湛江"
    },
    {
      "code": "MaoMing",
      "name": "茂名"
    },
    {
      "code": "ZhaoQing",
      "name": "肇庆"
    },
    {
      "code": "HuiZhou",
      "name": "惠州"
    },
    {
      "code": "MeiZhou",
      "name": "梅州"
    },
    {
      "code": "ShanWei",
      "name": "汕尾"
    },
    {
      "code": "HeYuan",
      "name": "河源"
    },
    {
      "code": "YangJiang",
      "name": "阳江"
    },
    {
      "code": "QingYuan",
      "name": "清远"
    },
    {
      "code": "DongGuan",
      "name": "东莞"
    },
    {
      "code": "ZhongShan",
      "name": "中山"
    },
    {
      "code": "ChaoZhou",
      "name": "潮州"
    },
    {
      "code": "JieYang",
      "name": "揭阳"
    },
    {
      "code": "YunFu",
      "name": "云浮"
    }
  ],
  [
    {
      "code": "GuangXi",
      "name": "广西"
    },
    {
      "code": "NanNing",
      "name": "南宁"
    },
    {
      "code": "LiuZhou",
      "name": "柳州"
    },
    {
      "code": "GuiLin",
      "name": "桂林"
    },
    {
      "code": "WuZhou",
      "name": "梧州"
    },
    {
      "code": "BeiHai",
      "name": "北海"
    },
    {
      "code": "FangChengGang",
      "name": "防城港"
    },
    {
      "code": "QinZhou",
      "name": "钦州"
    },
    {
      "code": "GuiGang",
      "name": "贵港"
    },
    {
      "code": "YuLin",
      "name": "玉林"
    },
    {
      "code": "BaiSe",
      "name": "百色"
    },
    {
      "code": "HeZhou",
      "name": "贺州"
    },
    {
      "code": "HeChi",
      "name": "河池"
    },
    {
      "code": "LaiBin",
      "name": "来宾"
    },
    {
      "code": "ChongZuo",
      "name": "崇左"
    }
  ],
  [
    {
      "code": "HaiNan",
      "name": "海南"
    },
    {
      "code": "HaiKou",
      "name": "海口"
    },
    {
      "code": "SanYa",
      "name": "三亚"
    },
    {
      "code": "SanSha",
      "name": "三沙"
    },
    {
      "code": "DanZhou",
      "name": "儋州"
    }
  ],
  [
    {
      "code": "SiChuan",
      "name": "四川"
    },
    {
      "code": "ChengDu",
      "name": "成都"
    },
    {
      "code": "ZiGong",
      "name": "自贡"
    },
    {
      "code": "PanZhiHua",
      "name": "攀枝花"
    },
    {
      "code": "LuZhou",
      "name": "泸州"
    },
    {
      "code": "DeYang",
      "name": "德阳"
    },
    {
      "code": "MianYang",
      "name": "绵阳"
    },
    {
      "code": "GuangYuan",
      "name": "广元"
    },
    {
      "code": "SuiNing",
      "name": "遂宁"
    },
    {
      "code": "NeiNiang",
      "name": "内江"
    },
    {
      "code": "LeShan",
      "name": "乐山"
    },
    {
      "code": "NanChong",
      "name": "南充"
    },
    {
      "code": "MeiShan",
      "name": "眉山"
    },
    {
      "code": "YiBin",
      "name": "宜宾"
    },
    {
      "code": "GuangAn",
      "name": "广安"
    },
    {
      "code": "DaZhou",
      "name": "达州"
    },
    {
      "code": "YaAn",
      "name": "雅安"
    },
    {
      "code": "BaZhong",
      "name": "巴中"
    },
    {
      "code": "ZiYang",
      "name": "资阳"
    }
  ],
  [
    {
      "code": "GuiZhou",
      "name": "贵州"
    },
    {
      "code": "GuiYang",
      "name": "贵阳"
    },
    {
      "code": "LiuPanShui",
      "name": "六盘水"
    },
    {
      "code": "ZunYi",
      "name": "遵义"
    },
    {
      "code": "AnShun",
      "name": "安顺"
    },
    {
      "code": "BiJie",
      "name": "毕节"
    },
    {
      "code": "TongRen",
      "name": "铜仁"
    },
    {
      "code": "QianXiNan",
      "name": "黔西南"
    },
    {
      "code": "QianDongNan",
      "name": "黔东南"
    },
    {
      "code": "QianNan",
      "name": "黔南"
    }
  ],
  [
    {
      "code": "YunNan",
      "name": "云南"
    },
    {
      "code": "KunMing",
      "name": "昆明"
    },
    {
      "code": "QuJing",
      "name": "曲靖"
    },
    {
      "code": "YuXi",
      "name": "玉溪"
    },
    {
      "code": "BaoShan",
      "name": "保山"
    },
    {
      "code": "ZhaoTong",
      "name": "昭通"
    },
    {
      "code": "LiJiang",
      "name": "丽江"
    },
    {
      "code": "PuEr",
      "name": "普洱"
    },
    {
      "code": "LinCang",
      "name": "临沧"
    }
  ],
  [
    {
      "code": "XiZang",
      "name": "西藏"
    },
    {
      "code": "LaSa",
      "name": "拉萨"
    },
    {
      "code": "RiKaZe",
      "name": "日喀则"
    },
    {
      "code": "CangDu",
      "name": "昌都"
    },
    {
      "code": "LinZhi",
      "name": "林芝"
    },
    {
      "code": "ShanNan",
      "name": "山南"
    },
    {
      "code": "NaQu",
      "name": "那曲"
    }
  ],
  [
    {
      "code": "ShanXi",
      "name": "陕西"
    },
    {
      "code": "XiAn",
      "name": "西安"
    },
    {
      "code": "TongChuan",
      "name": "铜川"
    },
    {
      "code": "BaoJi",
      "name": "宝鸡"
    },
    {
      "code": "XianYang",
      "name": "咸阳"
    },
    {
      "code": "WeiNan",
      "name": "渭南"
    },
    {
      "code": "YanAn",
      "name": "延安"
    },
    {
      "code": "HanZhong",
      "name": "汉中"
    },
    {
      "code": "YuLin",
      "name": "榆林"
    },
    {
      "code": "AnKang",
      "name": "安康"
    },
    {
      "code": "ShangLuo",
      "name": "商洛"
    }
  ],
  [
    {
      "code": "GanSu",
      "name": "甘肃"
    },
    {
      "code": "LanZhou",
      "name": "兰州"
    },
    {
      "code": "JiaYuGuan",
      "name": "嘉峪关"
    },
    {
      "code": "JinChang",
      "name": "金昌"
    },
    {
      "code": "BaiYin",
      "name": "白银"
    },
    {
      "code": "TianShui",
      "name": "天水"
    },
    {
      "code": "WuWei",
      "name": "武威"
    },
    {
      "code": "ZhangYe",
      "name": "张掖"
    },
    {
      "code": "PingLiang",
      "name": "平凉"
    },
    {
      "code": "JiuQuan",
      "name": "酒泉"
    },
    {
      "code": "QingYang",
      "name": "庆阳"
    },
    {
      "code": "DingXi",
      "name": "定西"
    },
    {
      "code": "LongNan",
      "name": "陇南"
    }
  ],
  [
    {
      "code": "QingHai",
      "name": "青海"
    },
    {
      "code": "XiNing",
      "name": "西宁"
    },
    {
      "code": "HaiDong",
      "name": "海东"
    }
  ],
  [
    {
      "code": "NingXia",
      "name": "宁夏"
    },
    {
      "code": "YinChuan",
      "name": "银川"
    },
    {
      "code": "ShiZuiShan",
      "name": "石嘴山"
    },
    {
      "code": "WuZhong",
      "name": "吴忠"
    },
    {
      "code": "GuYuan",
      "name": "固原"
    },
    {
      "code": "ZhongWei",
      "name": "中卫"
    }
  ],
  [
    {
      "code": "XinJiang",
      "name": "新疆"
    },
    {
      "code": "WuLuMuQi",
      "name": "乌鲁木齐"
    },
    {
      "code": "KeLaMaYi",
      "name": "克拉玛依"
    },
    {
      "code": "TuLuFan",
      "name": "吐鲁番"
    },
    {
      "code": "HaMi",
      "name": "哈密"
    }
  ],
  [
    {
      "code": "TaiwanSheng",
      "name": "台湾省"
    },
    {
      "code": "TaiBei",
      "name": "台北"
    },
    {
      "code": "GaoXiong",
      "name": "高雄"
    },
    {
      "code": "TaiZhong",
      "name": "台中"
    },
    {
      "code": "TaiNan",
      "name": "台南"
    },
    {
      "code": "XinBei",
      "name": "新北"
    },
    {
      "code": "TaoYuan",
      "name": "桃园"
    }
  ],
  [
    {
      "code": "XiangGang",
      "name": "香港"
    }
  ],
  [
    {
      "code": "AoMen",
      "name": "澳门"
    }
  ]
]
)";
