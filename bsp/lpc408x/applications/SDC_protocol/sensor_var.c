/**
 * @file sensor_var.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 传感器名称信息
 * @version 0.1
 * @date 2023-11-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "sensor_var.h"

const rom_sensor_var_t rom_sensor_var[SEN_NUM] = {
    [SEN_ALL] = {.fullname = "自定义", .shortname = "用户", .symbolname = "user", .unit = ""},
    [SEN_ENVI_TEMP] = {.fullname = "环境温度", .shortname = "温度", .symbolname = "TEMP", .unit = ""},
    [SEN_CO] = {.fullname = "一氧化碳", .shortname = "一氧", .symbolname = "CO", .unit = ""},
    [SEN_SMOG] = {.fullname = "烟雾", .shortname = "烟雾", .symbolname = "SMOG", .unit = ""},
    [SOUND_LIGHT_ALARM] = {.fullname = "声光报警器", .shortname = "声光", .symbolname = "SOUND_LI", .unit = ""},
};
