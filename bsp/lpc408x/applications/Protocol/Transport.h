/**
 * @file Transport.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 传输层框架
 * - [ ] 数据等待
 * - [ ] 帧拼接
 * - [ ] 帧校验
 * - [ ] 超时检测
 * - [ ] 帧提取
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __TRANSPORT_H_
#define __TRANSPORT_H_

#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum TR_CHECK_RES
    {
        TR_CHECK_NONE,     /*!< 无帧头 */
        TR_CHECK_ERR_ADDR, /*!< 地址错误*/
        TR_CHECK_ERR_LEN,  /*!< 长度不足 */
        TR_CHECK_ERR_CRC,  /*!< 检验错误 */
        TR_CHECK_FRAME,    /*!< 完整帧 */
    } TR_CHECK_RES_E;
    /**
     * @brief 工作模式
     *
     */
    typedef enum TR_MODE
    {
        TR_FRAME_HEAD = 0x01,    /*!< 帧头检测 */
        TR_FRAME_LENTH = 0x02,   /*!< 帧长度检测 */
        TR_FRAME_TIMEOUT = 0x04, /*!< 帧超时检测 */
    } TR_MODE_E;

    typedef enum _TR_CONTROL
    {
        TR_SET_WAITING_RESPONSE
    } TR_CONTROL_E;

    typedef struct _TRANSPORT transport_t;

    struct _TRANSPORT
    {
        uint32_t mode; /*!< 工作模式标识 */
        TR_CHECK_RES_E(*waiting_response)
        (transport_t *pTr); /*!< 等待帧数据 */
        uint8_t *rxBuff;
        uint16_t len;
        uint16_t read_len;
    };
    void tr_init(transport_t *pTr, uint8_t *buff, uint16_t len);
    void tr_control(transport_t *pTr, uint32_t cmd, void *pParam);
#ifdef __cplusplus
}
#endif

#endif
