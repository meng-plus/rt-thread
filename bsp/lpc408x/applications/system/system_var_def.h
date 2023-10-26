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
    } varWork_t;

    /**
     * @brief  data in rom  only
     *
     */
    typedef struct _ROM_VAR_INIT
    {
        const uint32_t resv;
        const char *product_name;
    } var_init_t;

/*!< 设备的配置参数 flash中存储 */
#define FLASH_PARAMETER_VER 0x100 /*!< 顺序增加 */

    /**
     * @brief  Do not modify it unless you want to reset the parameters
     **/
    /**
     * @brief data in  flash
     *
     */
    typedef struct __FLASH_PRODUCT_PARAMETER
    {
        uint32_t ver; /*!<版本标识*/

    } product_param_t;

#ifdef __cplusplus
}
#endif
#endif
