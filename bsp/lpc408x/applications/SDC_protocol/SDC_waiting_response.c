/**
 * @file SDC_waiting_response.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief SDC 报文解析
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "SDC_protocol.h"
#include "Session_master.h"
#include "ipc/ringbuffer.h"
extern struct rt_ringbuffer *rb_sdc;

TR_CHECK_RES_E SDC_waiting_response(transport_t *pTr) /*!< 等待帧数据 */
{

    uint16_t read_len = pTr->read_len;
    uint8_t repeat = 0;
    uint16_t data_len = 0;
    if (rb_sdc == 0)
        return TR_CHECK_ERR_LEN;

    while (1)
    {
        uint16_t offset = 0;
        if (rt_ringbuffer_data_len(rb_sdc) == 0)
        {
            repeat = 1;
            break;
        }
        if (read_len < 3)
        {

            offset = rt_ringbuffer_getchar(rb_sdc, read_len + pTr->rxBuff);
            if (offset && pTr->rxBuff[read_len] != 0xFB)
            {
                read_len = pTr->read_len = 0;
                continue;
            }
        }
        else if (read_len < 7)
        {
            offset = rt_ringbuffer_getchar(rb_sdc, read_len + pTr->rxBuff);
        }
        else
        {
            data_len = pTr->rxBuff[6];
            offset = rt_ringbuffer_get(rb_sdc, read_len + pTr->rxBuff, data_len + 8 - read_len);
        }

        read_len += offset;
        if (read_len > 8 && data_len + 8 <= read_len)
        {
            repeat = 0;
            uint16_t crc_get = SDC_getCrc((sdc_data_t *)pTr->rxBuff);
            uint16_t crc_calc = SDC_checkCrc((sdc_data_t *)pTr->rxBuff);
            if (crc_get == crc_calc)
            { /*!< 校验通过 */
                read_len = 0;
                break;
            }
            else
            {
                read_len = 0;
            }
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
