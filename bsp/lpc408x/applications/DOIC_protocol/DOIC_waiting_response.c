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

#include "thread_DOIC.h"
TR_CHECK_RES_E DOIC_waiting_response(transport_t *pTr) /*!< 等待帧数据 */
{
    thread_doic_t *pDts = (thread_doic_t *)pTr;
    uint16_t read_len = 0;
    uint8_t repeat = 0;
    uint16_t data_len = 0;
    while (1)
    {
        uint16_t offset = 0;
        if (read_len == 0)
        {
            offset = rt_device_read(pDts->device, -1, read_len + pTr->rxBuff, 1);
            if (offset && pTr->rxBuff[0] != 0xFB)
            {
                continue;
            }
        }
        else if (read_len == 1)
        {
            offset = rt_device_read(pDts->device, -1, read_len + pTr->rxBuff, 1);
            if (offset && pTr->rxBuff[1] != 0x04)
            {
                read_len = 0;
                continue;
            }
        }
        else if (read_len < 8)
        {
            offset = rt_device_read(pDts->device, -1, read_len + pTr->rxBuff, 1);
            if (read_len + offset >= 8)
            {
                data_len = pTr->rxBuff[7];
            }
        }
        else
        {
            offset = rt_device_read(pDts->device, -1, read_len + pTr->rxBuff, data_len + 8+2 - read_len);
        }

        read_len += offset;
        if (read_len > 8 && data_len + 8 <= read_len)
        {
            repeat = 0;
            uint16_t crc_get = DOIC_getCrc((doic_data_t *)pTr->rxBuff);
            uint16_t crc_calc = DOIC_checkCrc((doic_data_t *)pTr->rxBuff);
            if (crc_get == crc_calc)
            { /*!< 校验通过 */
                break;
            }
            else
            {
                read_len = 0;
            }
        }
        if (offset)
            continue;

        if (RT_EOK != rt_sem_take(&pDts->rx_sem, 1000)) // RT_WAITING_FOREVER
        {
            repeat = 1;
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
