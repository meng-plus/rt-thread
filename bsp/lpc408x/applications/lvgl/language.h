/**
 * @file language.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief  lvgl 语言编码切换
 * 注意 语言编码为UTF8
 * 使用英文方便快速开发，中文方便客户展示
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __LANGUAGE_H_
#define __LANGUAGE_H_

#ifndef LANGUAGE_CN
#define LANGUAGE_CN 1
#endif

#define LANGUAGE(en, ch) (LANGUAGE_CN ? ch : en)

#define SYMBOL_GLOBAL LANGUAGE("", "")

#define SYMBOL_TEMP LANGUAGE("C", "℃")
#define TEXT_SENSOR ("自定义用户环境温度一氧化碳氧气烟雾声光报警")
#define TEXT_DASHBOARD LANGUAGE("dashboard", "首页")
#define TEXT_CONFIG LANGUAGE("config", "配置")
#define TEXT_DEBUG LANGUAGE("debug", "调试")
#define TEXT_SET LANGUAGE("set", "设置")
#define TEXT_ABOUT LANGUAGE("about", "关于")
#define TEXT_SOFT_VER LANGUAGE("SF_VER", "软件版本")
#define TEXT_HARDWARE_VER LANGUAGE("HD_VER", "硬件版本")
#define TEXT_INPUT LANGUAGE("INPUT", "输入")
#define TEXT_OUTPUT LANGUAGE("OUPUT", "输出")
#define TEXT_OFF LANGUAGE("OFF", "关")
#define TEXT_ON LANGUAGE("ON", "开")
#define TEXT_NAME LANGUAGE("name", "名称")
#define TEXT_CHN LANGUAGE("chn", "通道")
#define TEXT_ADDR LANGUAGE("addr", "地址")
#define TEXT_UART_ADDR LANGUAGE("uart_addr", "串口地址")
#define TEXT_SLAVE_ADDR LANGUAGE("slave addr", "从机地址")
#define TEXT_DEL LANGUAGE("del", "删除")
#define TEXT_NEW LANGUAGE("new", "新增")
#define TEXT_RESTORE LANGUAGE("restore", "恢复")
#define TEXT_SAVE LANGUAGE("save", "保存")
#define TEXT_WIFI_SET LANGUAGE("WIFI Set", "WIFI设置")
#define TEXT_BLUETOOTH_SET LANGUAGE("BlueTooth Set", "蓝牙设置")
#define TEXT_LORA_SET LANGUAGE("lora Set", "lora设置")
#define TEXT_EXIT LANGUAGE("exit", "退出")
#define TEXT_OFFLINE LANGUAGE("offline", "掉线")
#define TEXT_ERROR LANGUAGE("error", "错误")
#define TEXT_SEL_CHN LANGUAGE("select chn", "选择通道")
#define TEXT_RESTORE_OK LANGUAGE("Successfully restore", "恢复成功")
#define TEXT_SAVE_OK LANGUAGE("Successfully saved", "保存成功")
#define TEXT_ERROR_CODE LANGUAGE("Error code", "错误码")
#define TEXT_ERROR_CREATE LANGUAGE("create error", "创建错误")
#define TEXT_OPEN LANGUAGE("open", "打开")
#define TEXT_CLOSE LANGUAGE("close", "关闭")
#define TEXT_SEND LANGUAGE("send", "发送")
#define TEXT_REC LANGUAGE("receive", "接收")
#define TEXT_SEND_MSG LANGUAGE("sendMsg", "发送数据")
#define TEXT_GET_NAME LANGUAGE("getName", "获取名称")
#define TEXT_GET_IP LANGUAGE("getIp", "获取ip")
#define TEXT_GET_ADDR LANGUAGE("getIp", "获取地址")
#define TEXT_WORK_MODE LANGUAGE("", "工作状态")
#define TEXT_CAL_PUMP LANGUAGE("", "测量泵")
#define TEXT_GAS_TANK_PRESSURE LANGUAGE("", "气罐压力")
#define TEXT_TIME LANGUAGE("time", "时间")
#define TEXT_ERR_CNT LANGUAGE("err cnt", "错误次数")
#define TEXT_FLOW LANGUAGE("", "流量")
#define TEXT_FOREPUMP LANGUAGE("", "预抽泵")
#define TEXT_IDLE LANGUAGE("IDLE", "待机")
#define TEXT_MEAS LANGUAGE("MEAS", "测量")
#define TEXT_RUN LANGUAGE("run", "运行")
#define TEXT_WARING LANGUAGE("waring", "警告")
#define TEXT_STATUS_MSG LANGUAGE("status messages", "状态消息")
#define TEXT_INDEX LANGUAGE("index", "序号")
#define TEXT_ERR_MSG LANGUAGE(" description", "描述")

#endif
