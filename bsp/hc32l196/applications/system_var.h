/**
 * @file system_var.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 系统配置数据
 *  tips:请注意 要4字节对齐
 * @version 0.1
 * @date 2024-05-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __SYSTEM_INFO_H_
#define __SYSTEM_INFO_H_

#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
/* APP version information */
#define APP_VERSION_MAJOR 1 /**< Major version number (X.x.x) */
#define APP_VERSION_MINOR 0 /**< Minor version number (x.X.x) */
#define APP_VERSION_PATCH 0 /**< Patch version number (x.x.X) */
/* hardware version information */
#define HF_VERSION_MAJOR 1 /**< Major version number (X.x.x) */
#define HF_VERSION_MINOR 0 /**< Minor version number (x.X.x) */
#define HF_VERSION_PATCH 0 /**< Patch version number (x.x.X) */

/* e.g. #if RTTHREAD_VERSION >= RT_VERSION_CHECK(4, 1, 0) */
#define APP_VERSION_CHECK(major, minor, revise) ((major * 10000) + (minor * 100) + revise)
#define HEX_VERSION_CHECK(year, month, version) ((year * 0x100) + (month * 0x10) + version)

/* version */
/* 软件版本号                 	*/
#define APP_VERSION      APP_VERSION_CHECK(APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH)
#define HF_VERSION       HEX_VERSION_CHECK(HF_VERSION_MAJOR, HF_VERSION_MINOR, HF_VERSION_PATCH) /* 硬件版本			        	*/
#define PROTOCOL_VERSION HEX_VERSION_CHECK(24, 02, 01)                                           /* 协议版本			        	*/

/* str  */
#define NUM2STR(num)           #num
#define VERSTR4NUM(ma, mi, pa) NUM2STR(ma) "." NUM2STR(mi) "." NUM2STR(pa)

#define APP_VERSION_STR      VERSTR4NUM(APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH)
#define HF_VERSION_STR       VERSTR4NUM(HF_VERSION_MAJOR, HF_VERSION_MINOR, HF_VERSION_PATCH) /* 硬件版本			        	*/
#define PROTOCOL_VERSION_STR VERSTR4NUM(24, 05, 17)                                           /* 协议版本			        	*/


typedef struct _RAM_VAR_WORKING
{
    uint32_t tick;
    uint16_t addr; /*!< 当前设备的地址 */

} varWork_t;

/**
     * @brief  data in rom  only
     *
     */
typedef struct _ROM_VAR_INIT
{
    const char *product_name;

} var_init_t;

/*!< 设备的配置参数 flash中存储 */
#define PARAMETER_VER 0x10
typedef struct _FLASH_HEADER_PARAMETER
{
    uint16_t ver;
    uint16_t reserve[3];
} header_param_t;
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
    header_param_t header; /*!<版本标识*/
    uint8_t ad5161_value;  /*!< 配置数据 */
    uint16_t dac_value;    /*!< 配置数据 */
} product_param_t;

extern varWork_t g_var_work;

extern product_param_t g_prod_param;

extern const var_init_t g_var_init;

uint8_t var_save(void *param);
uint8_t var_reload(void *param);
uint8_t var_resetting(void *param);

#ifdef __cplusplus
}
#endif
#endif
