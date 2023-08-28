/**
 * @file vardef.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023
 *
 */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __VARDEF_H_
#define __VARDEF_H_
/* Private includes ----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C"
{
#endif
/* Exported macro ------------------------------------------------------------*/
#define PRODUCT_NAME "KJ428"
#define PRODUCT_SOFT_VER ("V1.00 20230804") /* 软件版本号                 	*/
#define PRODUCT_HARD_VER ("H1.02 20140911") /* 硬件版本			        	*/

#define DEVICE_ID 0x00
    typedef enum _SENSOR_TYPE
    {
        C2H2,
        CH4,
        FLOW,
        O2,
        CO2,
        CO2_2,
        H2S_2,
        CO,
        C2H4,
        C2H6,
        H2S_1,
        TEMP,
        PRESS,
        GAS_TYPE_NUM,
    } SENSOR_TYPE_E;
    /* Exported types ------------------------------------------------------------*/

    typedef struct _SENSOR_INGO
    {
        uint8_t id;
        uint8_t addr; /*!< 传感器地址 */
        char *name;   /*!< 传感器名称 */
        char *unit;   /*!< 单位名称 */
        uint8_t dot;  /*!< 精度 */
    } sensor_info_t;
    typedef struct _SENSOR_CONFIG
    {
        uint8_t en : 1; /*!< 使能标识 */

    } sensor_config_t;
    typedef struct _SENSOR_VALUE
    {
        uint8_t state : 2; /*!< 状态标识 */
        uint8_t valid : 1; /*!< 数据有效性标识 */
        uint16_t adc;      /*!< 原始值 */
        float msr_raw;     /*!< 原始测量值 */
        float msr_view;    /*!< 展示测量值 */
    } sensor_value_t;
    /* Exported constants --------------------------------------------------------*/
    /*!<运行中参数  仅在内存*/
    typedef struct _RAM_VAR_WORKING
    {
        uint8_t workmode;
        sensor_value_t m_sensor_value[GAS_TYPE_NUM];
    } var_work_t;
    /*!< 设备的配置参数 flash中存储 */
#define FLASH_PARAMETER_VER 0x100 /*!< 顺序增加 */
/**
 * @brief  Do not modify it unless you want to reset the parameters
 **/
#define FLASH_PARAMETER_ADDR 0x0000
#define FLASH_PARAMETER_ID 0x1234
    typedef struct __FLASH_PRODUCT_PARAMETER
    {
        uint16_t ver;  /*!<版本标识*/
        uint16_t id;   /*!< 识别号*/
        uint16_t addr; /*!< 设备地址 */
        sensor_config_t m_sensor_config[GAS_TYPE_NUM];
    } var_param_t;

    /**
     * @brief 全局只读配置数据
     *
     */
    typedef struct _ROM_VAR_INIT
    {
        char* product_name;
        char* product_soft_ver;
        char* product_hard_ver;
        char* build_time;
        sensor_info_t m_sensor_info[GAS_TYPE_NUM];
    } var_init_t;

    /* Exported functions prototypes ---------------------------------------------*/
    extern const var_init_t g_varInit;
    extern var_param_t g_varParam;
    extern var_work_t g_varWork;
     
#ifdef __cplusplus
}
#endif
#endif
