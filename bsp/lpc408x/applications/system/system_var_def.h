/**
 * @file systeminfo.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 系统配置数据
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __SYSTEM_INFO_H_
#define __SYSTEM_INFO_H_

#include <stdint.h>
#include <stdbool.h>
#include "Session_DTS.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define SOFT_VER ("V1.00 20231023") /* 软件版本号                 	*/
#define HARD_VER ("H1.00 20230828") /* 硬件版本			        	*/

    /**
     * @brief   data in ram  only
     */

    typedef struct _RAM_VAR_WORKING
    {
        uint32_t tick;
        thread_dts_t *dts; /*!< 测温主机数据 */
    } varWork_t;

    /**
     * @brief  data in rom  only
     *
     */
    typedef struct _ROM_VAR_INIT
    {
        const uint32_t resv;
        const char *product_name;
        uint16_t dts_delayms_max;
    } var_init_t;

/*!< 设备的配置参数 flash中存储 */
/**
 * @brief  Do not modify it unless you want to reset the parameters
 **/
#define FLASH_PRO_PARAMETER_VER 0x100 /*!< 顺序增加 */

    /**
     * @brief data in  flash
     *
     */
    typedef struct __FLASH_PRODUCT_PARAMETER
    {
        uint32_t ver;         /*!<版本标识*/
        uint16_t dts_delayms; /*!< 测温主机数据扫描周期 range[0,10000] */

    } product_param_t;

/**
 * @brief  Do not modify it unless you want to reset the parameters
 **/
#define FLASH_SCREEN_PARAMETER_VER 0x100 /*!< 顺序增加 */

    typedef struct _FLASH_SCREEN_PARAMETER
    {
        /*!< touch  */
        uint16_t ver;       /*!<版本标识*/
        uint16_t touch : 1; /*!< 0:需要校准 1校准过 */

        uint16_t min_raw_x;
        uint16_t min_raw_y;
        uint16_t max_raw_x;
        uint16_t max_raw_y;
        uint16_t range_x; /*!< 屏幕尺寸 */
        uint16_t range_y; /*!< 屏幕尺寸 */
    } screen_param_t;

#ifdef __cplusplus
}
#endif
#endif
