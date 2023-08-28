/**
 * @file varDef.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "varDef.h"

const var_init_t g_varInit = {
    .product_name = PRODUCT_NAME,
    .product_soft_ver = PRODUCT_SOFT_VER,
    .product_hard_ver = PRODUCT_HARD_VER,
    .build_time = __TIME__,
    .m_sensor_info = {
        [C2H2] = {2, 1, "乙炔", "ppm", 0},
        [CH4] = {1, 2, "甲烷", "%", 2},
        [FLOW] = {3, 4, "流量", "L/min", 0},
        [O2] = {4, 6, "氧气", "%", 1},
        [CO2] = {5, 11, "二氧化碳", "%", 2},
        [CO2_2] = {6, 11, "二氧化碳_2", "%", 2},
        [H2S_2] = {7, 15, "硫化氢_2", "%", 1},
        [CO] = {8, 13, "一氧化碳", "ppm", 0},
        [C2H4] = {9, 13, "乙烯", "ppm", 0},
        [C2H6] = {10, 13, "乙烷", "%", 0},
        [H2S_1] = {11, 13, "硫化氢", "%", 1},
        [TEMP] = {12, 13, "温度", "℃", 1},
        [PRESS] = {13, 13, "压力", "kPa", 1},
    },
};

static const var_param_t l_varParamInit = {
    .ver = FLASH_PARAMETER_VER,
    .id = FLASH_PARAMETER_ID,
    .addr = DEVICE_ID,
    .m_sensor_config = {
        [C2H2] = {1},
        [CH4] = {1},
        [FLOW] = {1},
        [O2] = {1},
        [CO2] = {1},
        [CO2_2] = {1},
        [H2S_2] = {1},
        [CO] = {1},
        [C2H4] = {1},
        [C2H6] = {1},
        [H2S_1] = {1},
        [TEMP] = {1},
        [PRESS] = {1},
    },
};
var_param_t g_varParam = {
    .ver = FLASH_PARAMETER_VER,
    .id = FLASH_PARAMETER_ID,
    .addr = DEVICE_ID,
    .m_sensor_config = {
        [C2H2] = {1},
        [CH4] = {1},
        [FLOW] = {1},
        [O2] = {1},
        [CO2] = {1},
        [CO2_2] = {0},
        [H2S_2] = {0},
        [CO] = {1},
        [C2H4] = {1},
        [C2H6] = {1},
        [H2S_1] = {0},
        [TEMP] = {1},
        [PRESS] = {1},
    },
};
var_work_t g_varWork = {
    .m_sensor_value = {
        {1, 1, 0,0, 123456},
        {1, 1, 0,0, 12345.6},
        {1, 1, 0,0, 1234.56},
        {1, 1, 0,0, 123.456},
        {1, 1, 0,0, 12.3456},
        {1, 1, 0,0, 123},
        {1, 1, 0,0, 1234},
        {1, 1, 0,0, 12345},
        {1, 1, 0,0, 0.123},
        {1, 1, 0,0, 0.1},
    },
};
