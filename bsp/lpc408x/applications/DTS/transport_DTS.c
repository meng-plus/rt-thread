/**
 * @file transport_DTS.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-10-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Session_DTS.h"
#include "thread_DTS.h"
TR_CHECK_RES_E waiting_response(transport_t *pTr) /*!< 等待帧数据 */
{
    thread_dts_t *pDts = (thread_dts_t *)pTr;
    agile_modbus_t *ctx = &pDts->ctx_rtu._ctx;
    uint16_t read_len = 0;
    uint8_t repeat = 0;
    while (1)
    {
        if (RT_EOK != rt_sem_take(&pDts->rx_sem, 1000)) // RT_WAITING_FOREVER
        {
            repeat = 1;
            break;
        }

        uint16_t offset = rt_device_read(pDts->device, -1, read_len + pTr->rxBuff, pTr->len - read_len);
        read_len += offset;
        if (read_len > 3 && ctx->read_buf[2] + 5 <= read_len)
        {
            repeat = 0;
            break;
        }
    }
    pTr->read_len = read_len;
    if (repeat == 1)
    {
        return TR_CHECK_ERR_LEN;
    }
    else
    {
        return TR_CHECK_FRAME;
    }
}
