/**
 * @file Transport.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-10-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Transport.h"

void tr_init(transport_t *pTr, uint8_t *buff, uint16_t len)
{
    if (pTr == 0)
        return;
    pTr->rxBuff = buff;
    pTr->len = len;
    pTr->read_len = 0;
}

void tr_control(transport_t *pTr, uint32_t cmd, void *pParam)
{
    if (0 == pTr)
        return;

    switch (cmd)
    {
    case TR_SET_WAITING_RESPONSE:
        pTr->waiting_response = (TR_CHECK_RES_E(*)(transport_t *))pParam;
        break;

    default:
        break;
    }
}
