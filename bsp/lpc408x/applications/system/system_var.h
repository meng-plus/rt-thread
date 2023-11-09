/**
 * @file systeminfo.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 全局系统参数管理
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __SYSTEM_VAR_H_
#define __SYSTEM_VAR_H_

#include "system_var_def.h"

#ifdef __cplusplus
extern "C"
{
#endif
    extern varWork_t g_var_work;

    extern screen_param_t g_screen_param;
    extern product_param_t g_prod_param;
    extern sensor_param_t g_sensor_param;

    extern const var_init_t g_var_init;

    uint8_t var_save(void *param);
    uint8_t var_reload(void *param);
    uint8_t var_resetting(void *param);
    /**
     * @ref sensor config
     */
    uint8_t sensor_new(sensor_param_t *param);
    uint8_t sensor_del(sensor_param_t *param, uint8_t id);

#ifdef __cplusplus
}
#endif
#endif
