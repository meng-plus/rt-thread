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
#define HF_VERSION       HEX_VERSION_CHECK(23, 12, 2)  /* 硬件版本			        	*/
#define PROTOCOL_VERSION HEX_VERSION_CHECK(24, 02, 01) /* 协议版本			        	*/

/* str  */
#define NUM2STR(num)           #num
#define VERSTR4NUM(ma, mi, pa) NUM2STR(ma) "." NUM2STR(mi) "." NUM2STR(pa)

#define APP_VERSION_STR      VERSTR4NUM(APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH)
#define HF_VERSION_STR       VERSTR4NUM(23, 12, 2)  /* 硬件版本			        	*/
#define PROTOCOL_VERSION_STR VERSTR4NUM(24, 02, 01) /* 协议版本			        	*/

enum DEVICE_LIST
{
    DEVICE_COLOR_0 = 0x01 << 0,
    DEVICE_COLOR_1 = 0x01 << 1,
    DEVICE_RESERVE = 0x01 << 31,
};
/**
     * @brief   data in ram  only
     */
typedef struct _MOTOR_CTL
{
    uint16_t cmd; // @ref MOTOR_COMMAND_E
} motor_ctl_t;
typedef struct _STEPMOTOR_CTL
{
    uint16_t cmd; // @ref MOTOR_COMMAND_E
} stepmotor_ctl_t;
typedef struct _RAM_VAR_WORKING
{
    uint32_t tick;
    uint16_t addr;          /*!< 当前设备的地址 */
    uint32_t device_status; /*!< 外设状态,置1 则为可用状态 @ref enum DEVICE_LIST */
    uint16_t carton_sel;    /*!<地址  */
    uint16_t paper_boxs;
    uint16_t paper_rolls;
    motor_ctl_t motor;
    stepmotor_ctl_t stepmotor;
    /**
         * @brief 颜色传感器
         *
         */
    struct IRGB
    {
        uint32_t ir;
        uint32_t green;
        uint32_t red;
        uint32_t blue;
    } nIRGB[2];
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
typedef struct __STEP_MOTOR_CONFIG
{
    uint8_t lv_num;    /*!< 速度分级数量 */
    uint8_t rev;
    uint16_t speed[2]; /*!< 速度分级数量 */
    uint16_t time;     /*!< 步进电机ms 加速度时间 */
    float rpm_mm;      /*!< 步进电机mm mm/rpm */

} step_motor_config_t;

typedef struct __FLASH_PRODUCT_PARAMETER
{
    header_param_t header;   /*!<版本标识*/
    uint16_t debug      : 1; /*!< 启动日志调试 */
    uint16_t protection : 1; /*!< 保护模式，仅通信 */
    // 当硬件配置0x00地址时看它
    uint16_t addr;            /*!< 本机地址 0x01为主机，0x02~0x04 为从机地址，顺序增加当前最多规划3个 */
    uint32_t motor_period;    /*!< 裁纸刀电机频率 */
    uint16_t motor_ratio_max; /*!< 裁纸刀最大功率 (0,100]% */
    uint16_t color_period_ms; /*!< 颜色模块的周期颜色采集速度 */
    step_motor_config_t step_motor_config[2];
    uint32_t per_circle;      /*!< 步进电机细分800 1600 3200 */
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
