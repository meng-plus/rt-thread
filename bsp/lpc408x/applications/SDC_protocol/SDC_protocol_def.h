/**
 * @file SDC_protocol_def.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 分站下行通信协议
 *  分站下行通讯协议分析指南_Ver1.69_20220311.pdf
 * @version 0.1
 * @date 2023-11-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __SDC_PROTOCOL_DEF_H_
#define __SDC_PROTOCOL_DEF_H_

#include <stdint.h>
#include <stdbool.h>
#include "sensor_def.h"
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct SDC_PROTOCOL_DEF
    {
        uint8_t lead1; /*!< 前导0xFB */
        uint8_t lead2; /*!< 前导0xFB */
        uint8_t lead3; /*!< 前导0xFB */

        /*!< 校验部分 */
        uint8_t dstIP;   /*!< 目标地址 */
        uint8_t srcIP;   /*!< 发送地址 */
        uint8_t length;  /*!< 帧长度  */
        uint8_t cmd;     /*!< 命令 */
        uint8_t data[0]; /*!< 柔性数组,  crc16(0x1021)校验高位在前，低位在后 */
    } sdc_data_t;

    enum SDC_STATUS
    {
        SDC_OFFLINE = 0,     /*!<断线  */
        SDC_COM_ARMBNOALITY, /*!< 探头异常 */
        SDC_REV,             /*!< 未配置此功能 */
        SDC_WARING_I = 7,
        SDC_WARING_II,
        SDC_WARING_III,
        SDC_WARING_IV,
        SDC_PROBE_ABNORMALITY,    /*!< 探头异常 */
        SDC_WARING_DATA_MUTATION, /*!< 数据突变 */
        SDC_WARING_LIMIT_LOWER,   /*!< 下限报警 */
        SDC_WARING_LIMIT_UP,      /*!< 上限报警 */
        SDC_NORMAL,               /*!< NORMAL */
    };
    /**
     * @brief 模拟量传感器响应分站查询（0x90）
     * 4byte 对齐
     */
    typedef struct SDC_protocol_0x90
    {
        uint8_t SenName; /*!< 传感器名 */
        uint8_t ChNum;   /*!< 通道 */
        uint16_t SenSat; /*!< 传感器状态 */

        uint8_t SenMode;   /*!< 工作模式 */
        uint8_t CfgUpdata; /*!< 参数更新 */
        uint8_t SenFlag;   /*!< 标志位 */
        uint8_t Res;

        float SenData[4]; /*!< 数据 一般为单数据 */

    } sdc_0x90_t;

#ifdef __cplusplus
}
#endif
#endif
