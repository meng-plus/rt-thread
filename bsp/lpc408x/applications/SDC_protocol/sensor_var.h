/**
 * @file sensor_var.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 传感器参数 名称
 * @version 0.1
 * @date 2023-11-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __SENSOR_VAR_H_
#define __SENSOR_VAR_H_

#include "sensor_def.h"
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _ROM_SENSOR_VAR
    {
        char *fullname;   /*!< 中文全称 */
        char *shortname;  /*!< 中文简称 */
        char *symbolname; /*!< 符号 */
        char *unit;       /*!< 单位 */

    } rom_sensor_var_t;
    extern rom_sensor_var_t rom_sensor_var[SEN_NUM];
#ifdef __cplusplus
}
#endif
#endif
