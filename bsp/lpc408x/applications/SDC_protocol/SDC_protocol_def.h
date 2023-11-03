/**
 * @file SDC_protocol_def.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 分站下行通信协议
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

#ifdef __cplusplus
}
#endif
#endif
