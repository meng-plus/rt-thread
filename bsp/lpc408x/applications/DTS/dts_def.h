/**
 * @file dts_def.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief   测温主机数据结构定义
 * @ref A0003-通过2-RS485接口按标准的modbusRTU-485点表读取数据.xlsx
 * @version 0.1
 * @date 2023-10-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __DTS_DEF_H_
#define __DTS_DEF_H_

#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C"
{
#endif

#define DTS_ADDRESS 34
#define DTS_CHANNEL_NUM 16
#define DTS_PARTITION_NUM 125
    typedef struct dts_sys
    {
        uint16_t status;  /*!< 设备状态 0：offline 1:ok */
        uint16_t chn_num; /*!< 0002  总通道数 */
    } dts_sys_t;
    typedef struct dts_channel
    {
        uint16_t signal_qua;   /*!< 0021  Signal Quality 信号质量 */
        uint16_t partition;    /*!< 0022 有效分区数量 */
        uint16_t length;       /*!< 0023 通道总长度 */
        uint16_t bfire_alarm;  /*!< 0024 火灾报警 */
        uint16_t bFault_alarm; /*!< 0025 故障断线报警 */
        uint16_t reserved[3];  /*!< 保留寄存器 */
    } dts_channel_t;
    typedef struct dts_chn_Data
    {
        uint16_t status;          /*!< 0200 报警状态(0-无报警1-定温2-差温-3预报警) */
        uint16_t threshold_alarm; /*!< 0201 报警阈值(对应报警状态) */
        uint16_t location_alarm;  /*!< 0202 报警位置（对应米标） */
        uint16_t temp_max_real;   /*!< 0203 实时最高温度 */
        uint16_t loc_max_real;    /*!< 0204 实时最高温度位置 */
        uint16_t temp_average;    /*!< 0205 实时分区平均值 */
        uint16_t reserved[2];     /*!< 0206 预留 */

    } dts_chn_Data_t;

    typedef struct dts_data
    {
        dts_sys_t system;                                             /*!< 设备信息 */
        dts_channel_t channel[DTS_CHANNEL_NUM];                       /*!< 通道信息 */
        dts_chn_Data_t partition[DTS_CHANNEL_NUM][DTS_PARTITION_NUM]; /*!< 通道数据 */
    } dts_data_t;

#ifdef __cplusplus
}
#endif
#endif
