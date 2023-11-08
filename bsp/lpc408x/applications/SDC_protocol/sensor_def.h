/**
 * @file sen_def.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-11-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __SENSOR_DEF_H_
#define __SENSOR_DEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

    enum SENSOR_DEF
    {
        SEN_ALL = 0x00,               //	自定义传感器	——
        SEN_PIPE_CH4 = 0x01,          //	管道甲烷	模拟量传感器
        SEN_PIPE_FLUX = 0x02,         //	管道流量	模拟量传感器
        SEN_PIPE_PRESS = 0x03,        //	管道压力	模拟量传感器
        SEN_PIPE_TEMP = 0x04,         //	管道温度	模拟量传感器
        SEN_ENVI_CH4 = 0x05,          //	环境甲烷	模拟量传感器
        SEN_ENVI_TEMP = 0x06,         //	环境温度	模拟量传感器
        SEN_ENVI_PRESS = 0x07,        //	环境压力	模拟量传感器
        SEN_OPEN_CLOSE = 0x08,        //	开停	开关量传感器
        SEN_CO = 0x09,                //	一氧化碳	模拟量传感器
        SEN_O2 = 0x0A,                //	氧气	模拟量传感器
        SEN_CO2 = 0x0B,               //	二氧化碳	模拟量传感器
        SEN_HUMIDITY = 0x0C,          //	湿度	模拟量传感器
        SEN_FEED = 0x0D,              //	馈电	开关量传感器
        SEN_AIR_DUCT = 0x0E,          //	风筒	开关量传感器
        SEN_AIR_DOOR = 0x0F,          //	风门	开关量传感器
        SEN_DUST = 0x10,              //	粉尘浓度	模拟量传感器
        SEN_SMOG = 0x11,              //	烟雾	开关量传感器
        SEN_WIND_SPEED = 0x12,        //	风速	模拟量传感器
        SEN_WIND_PRESS = 0x13,        //	风压	模拟量传感器
        SEN_WIND_MEA = 0x14,          //	风量	模拟量传感器
        SEN_WIND_DIR = 0x15,          //	风向	开关量传感器
        SEN_AXLE_TEMP = 0x16,         //	轴温	模拟量传感器
        SEN_WATER_TEMP = 0x17,        //	水温	模拟量传感器
        SEN_WATER_HIGHT = 0x18,       //	水位	模拟量传感器
        SEN_LIQUID_FLUX = 0x19,       //	液态流量	模拟量传感器
        SEN_COAL_PRESS = 0x1A,        //	煤层压力	模拟量传感器
        SEN_ULTRA_CH4 = 0x1B,         //	超声甲烷	模拟量传感器
        SEN_NEG_PRESS = 0x1C,         //	负压	模拟量传感器
        SEN_PIPE_INFRARED_CH4 = 0x1D, //	管道红外甲烷	模拟量传感器
        SEN_HL_CH4 = 0x1E,            //	高低浓甲烷	模拟量传感器
        SEN_LIQUID_LEVEL = 0x1F,      //	液位	模拟量传感器
        PWR_12V = 0x20,               //	12V 电源箱	KDY660/12
        PWR_IB_18V = 0x21,            //	18V ib  电源箱	KDW660/18B
        PWR_IB_21V = 0x22,            //	21V ib  电源箱	KDW660/21B
        PWR_IB_24V = 0x23,            //	24V ib  电源箱	KDW660/24B
        PWR_IA_18V = 0x24,            //	18V ia  电源箱	KDY660/18B(A)
        PWR_IA_21V = 0x25,            //	21V ia  电源箱	KDY660/21B
        PWR_IA_24V = 0x26,            //	24V ia  电源箱	KDY660/24B
        PWR_IA_14V8_BAT = 0x27,       //	14.8V ia  电池电源	DXHL5/14.8
        // 0x28~0x2F	预留	预留
        SEN_ENVI_LASER_CH4_T = 0x30,    //	激光甲烷（分布式）	ZJJ10-T
        SEN_ENVI_LASER_CH4 = 0x31,      //	激光甲烷	GJG10J
        SEN_ENVI_LASER_CH4_FULL = 0x32, //	激光甲烷	GJG100J（环境）
        SEN_PIPE_LASER_CH4_FULL = 0x33, //	管道激光甲烷	GJG100J（管道）
        SEN_ENVI_LASER_CH4_WH = 0x34,   //	无线激光甲烷（主）	GJG100JW-H
        SEN_ENVI_LASER_CH4_WL = 0x35,   //	无线激光甲烷（从）	GJG100JW-L
        // 0x36,~0x3F	预留	预留
        PWR_BREAKER = 0x40,       //	断电器
        SOUND_LIGHT_ALARM = 0x41, //	声光报警器	控制器
        // 0x42,//~0x4F	预留	预留
        KJJ660_SWITCHER = 0x50, //	KJJ660 环网交换机	网络设备
        KJJ12_SWITCHER = 0x51,  //	KJJ12 环网交换机	网络设备
        KXY18_BASE = 0x52,      //	KXY18 基站	"KXY18 矿用本安型基站（定位、广播）"
        // 0x52,~0x5F	预留
        COAL_LEVEL = 0x60,      //	煤位	模拟量传感器
        SEN_PRESS = 0x61,       //	压力	模拟量传感器
        SEN_H2S = 0x62,         //	硫化氢	模拟量传感器
        SEN_SO2 = 0x63,         //	二氧化硫	模拟量传感器
        SEN_H2 = 0x64,          //	氢气	模拟量传感器
        SEN_DOOR_ALARM = 0x65,  //	双风门报警器	开关量传感器	"S1-开S2-关"
        SEN_VOICE_ALARM = 0x66, //	语音报警器	开关量传感器	"S1-语音播报停S2-语音播报中"
        SEN_NO2 = 0x67,         //	二氧化氮	模拟量传感器
        SEN_WIND_TEMP = 0x68,   //	压风温度	模拟量传感器
        SEN_TEMP = 0x69,        //		温度	模拟量传感器
        LOCAL_PRESS = 0x70,     //	本地大气压	模拟量传感器
        SEN_N2 = 0x71,          //	氮气浓度	模拟量传感器
                                // 0x72~0xFF	预留
        SEN_NUM,                /*!< 传感器总数 */
    };
#ifdef __cplusplus
}
#endif
#endif
