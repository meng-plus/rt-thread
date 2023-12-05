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
#include "thread_DTS.h"
#include "SDC_protocol_def.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define SOFT_VER ("V1.00 20231128") /* 软件版本号                 	*/
#define HARD_VER ("H1.00 20230828") /* 硬件版本			        	*/

    /**
     * @brief   data in ram  only
     */
#define SENSOR_NUM_MAX 10
    typedef struct _RAM_VAR_WORKING
    {
        uint32_t tick;
        thread_dts_t *dts; /*!< 测温主机数据 */

        /*!< Sensor value */
        uint32_t Sensor_tick_last[SENSOR_NUM_MAX]; // Sensor[SEN_NUM];
        sdc_0x90_t Sensor[SENSOR_NUM_MAX];         // Sensor[SEN_NUM];
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
        uint16_t dts_delayms;  /*!< 测温主机数据扫描周期 range[0,10000] */
    } product_param_t;

/**
 * @brief  Do not modify it unless you want to reset the parameters
 **/
#define FLASH_SCREEN_PARAMETER_VER 0x100 /*!< 顺序增加 */

    typedef struct _FLASH_SCREEN_PARAMETER
    {
        /*!< touch  */
        header_param_t header; /*!<版本标识*/
        uint16_t touch : 1;    /*!< 0:需要校准 1校准过 */

        uint16_t min_raw_x;
        uint16_t min_raw_y;
        uint16_t max_raw_x;
        uint16_t max_raw_y;
        uint16_t range_x; /*!< 屏幕尺寸 */
        uint16_t range_y; /*!< 屏幕尺寸 */
    } screen_param_t;

/**
 * @brief  Do not modify it unless you want to reset the parameters
 **/
#define FLASH_SENSOR_PARAMETER_VER 0x100 /*!< 顺序增加 */

    /*!< 透传设备配置 HK32支持两路 */
    typedef struct device_config
    {
        uint8_t type;   /*!< 设备类型 */
        uint8_t en : 1; /*!< 启用 */
        uint8_t addr;   /*!< 设备地址 */
        uint32_t baud;  /*!< 波特率0:4800, 1:9600, 2:19200, 3:38400, 4:115200 */

    } device_config_t;

    typedef struct _SENSOR_CONFIG
    {
        uint8_t type;     /*!< 传感器类型@ref enum SENSOR_DEF */
        uint8_t chn;      /*!< 传感器通道 */
        uint8_t addr;     /*!< 传感器所在通道地址 */
        uint8_t dev_addr; /*!< 传感器所在设备地址 @ref device_config_t::addr */
    } sensor_config_t;

    typedef struct _FLASH_SENSOR_PARAMETER
    {
        header_param_t header; /*!<版本标识*/
        uint8_t device_num;
        uint8_t sensor_num;

        device_config_t dev_config[2];
        sensor_config_t sen_config[10];

    } sensor_param_t;

#ifdef __cplusplus
}
#endif
#endif
