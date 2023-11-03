/**
 * @file DOIC_protocol_def.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 分布式在线仪表通信协议 协议定义部分
 * @version 0.1
 * @date 2023-11-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __DOIC_PROTOCOL_DEF_H_
#define __DOIC_PROTOCOL_DEF_H_

#include <stdio.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif
    enum DOIC_CTRL_ID
    {
        DOIC_CTRL_FAST = 0x04,    /*!< 紧急响应 */
        DOIC_CTRL_RES = 0x02,     /*!< 需要设备应答 */
        DOIC_CTRL_RES_ERR = 0x01, /*!<错误应答 */

    };
    typedef struct DOIC_PROTOCOL_DEF
    {
        uint8_t hd1; /*!< 起始符1 固定值0xFB */
        uint8_t hd2; /*!< 起始符2 固定值0x04 */

        /*!< 校验部分 */
        uint8_t dst;     /*!< 目标地址 0：广播  1~254表示设备地址*/
        uint8_t src;     /*!< 源地址 1：广播  1~254设备地址 255未定义*/
        uint8_t cmd;     /*!< [0x01,0x9f]为产品的通用命令(0x50\0x61除外)，{0xa0~0xFF,0x50,0x61}为产品的专用命令*/
        uint8_t scmd;    /*!< 子命令 */
        uint8_t ctrl;    /*!< @ref DOIC_CTRL */
        uint8_t len;     /*!< 数据域长度 */
        uint8_t data[0]; /*!< 数据域 柔性数组,  crc16(0x1021)校验高位在前，低位在后*/
    } doic_data_t;

#ifdef __cplusplus
}
#endif
#endif
