/**
 * @file error_code_init.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 错误码框架的初始化程序
 * 工厂模式结构，避免注册过程分散在工程中不方便修改
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "error_code.h"
#include "rtdevice.h"
#include "rt_err_code.h"
#include "language.h"
void error_code_register_sensor();
int rt_error_code_init()
{

    error_code_init();
    error_code_register_sensor();
    return 0;
}

INIT_COMPONENT_EXPORT(rt_error_code_init);

error_code_t *er_code_find_system_var()
{
    // 静态变量，用于缓存最近一次使用的错误码对象
    static error_code_t cached_error_code = {.code = 0}; // 初始化为一个无效的错误码

    // 检查缓存中是否已有相应的错误码对象
    if (cached_error_code.code != 0)
    {
        return &cached_error_code;
    }

    // 如果缓存中没有相应的错误码对象，则进行搜索和注册
    error_code_t er_code_obj = {.code = 305, .cnt_max = 1, .description = "EEPRom 异常"};
    error_code_t *found_obj = error_code_find(er_code_obj.code);
    cached_error_code = er_code_obj;
    if (found_obj != NULL)
    { // 更新缓存
        error_code_unregister(found_obj);
    }
    error_code_register(&cached_error_code);
    return &cached_error_code;
}

error_code_t *er_code_find_DTS()
{
    // 静态变量，用于缓存最近一次使用的错误码对象
    static error_code_t cached_error_code = {.code = 0}; // 初始化为一个无效的错误码

    // 检查缓存中是否已有相应的错误码对象
    if (cached_error_code.code != 0)
    {
        return &cached_error_code;
    }

    // 如果缓存中没有相应的错误码对象，则进行搜索和注册
    error_code_t er_code_obj = {.code = 306, .cnt_max = 10, .description = TEXT_DTS_ERROR};
    error_code_t *found_obj = error_code_find(er_code_obj.code);
    cached_error_code = er_code_obj;
    if (found_obj != NULL)
    { // 更新缓存
        error_code_unregister(found_obj);
    }
    error_code_register(&cached_error_code);
    return &cached_error_code;
}

error_code_t *rt_err_code_find(uint32_t code)
{
    error_code_t *found_obj = NULL;
    switch (code)
    {
    case 305:
        found_obj = er_code_find_system_var();
        break;
    case 306:
        found_obj = er_code_find_DTS();
        break;
    default:
        found_obj = error_code_find(code);
        break;
    }
    return found_obj;
}
#include "sensor_var.h"
static error_code_t er_code_obj[6];

void error_code_register_sensor()
{
    error_code_t er_code_obj_tmp[] = {
        {.code = SEN_ALL, .cnt_max = 10, .description = rom_sensor_var[SEN_ALL].fullname},
        {.code = SEN_ENVI_TEMP, .cnt_max = 10, .description = rom_sensor_var[SEN_ENVI_TEMP].fullname},
        {.code = SEN_CO, .cnt_max = 10, .description = rom_sensor_var[SEN_CO].fullname},
        {.code = SEN_O2, .cnt_max = 10, .description = rom_sensor_var[SEN_O2].fullname},
        {.code = SEN_SMOG, .cnt_max = 10, .description = rom_sensor_var[SEN_SMOG].fullname},
        {.code = SOUND_LIGHT_ALARM, .cnt_max = 10, .description = rom_sensor_var[SOUND_LIGHT_ALARM].fullname},
    };
    for (size_t i = 0; i < sizeof(er_code_obj) / sizeof(er_code_obj[0]); i++)
    {
        er_code_obj[i] = er_code_obj_tmp[i];
        error_code_register(&er_code_obj[i]);
    }
}
