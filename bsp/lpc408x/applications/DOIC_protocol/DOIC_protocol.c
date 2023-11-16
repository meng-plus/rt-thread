#include "DOIC_protocol.h"
#include <string.h>
#include "DOIC_map.h"
void DOIC_master_init(doic_master_t *phander, uint8_t *buff, uint16_t len)
{
    if (phander)
    {
        phander->map = g_doic_map;
        phander->map_num = g_doic_map_num;
        phander->pdata = (doic_data_t *)buff;
    }
}

uint16_t DOIC_master_0x4181(doic_master_t *phander, uint8_t slave_addr, uint8_t sub_addr, uint8_t *buff, uint8_t len)
{
    doic_data_t *pdata = phander->pdata;
    const uint8_t cmd_pre[] = {0xFB, 0x04, 00, 00, 0x41, 0x81, 0};
    if (0 == phander || 0 == pdata)
    {
        return 0;
    }
    memcpy(pdata, cmd_pre, sizeof(cmd_pre));
    pdata->dst = slave_addr;
    pdata->src = phander->addr;
    pdata->len = len + 1;
    pdata->data[0] = sub_addr;
    memcpy(pdata->data + 1, buff, len);
    return DOIC_addCrc(pdata);
}

int8_t DOIC_deal(doic_master_t *phander, doic_data_t *pdst)
{
    if (0 == phander || 0 == phander->map)
    {
        return -1;
    }
    doic_data_t *pdata = phander->pdata;
    const doic_map_t *pmap;
    for (size_t i = 0; i < phander->map_num; i++)
    {
        pmap = &phander->map[i];
        if (pdst->cmd == pmap->cmd &&
            pdst->scmd == pmap->scmd &&
            pmap->func)
        {
            return pmap->func(pdata, pdst);
        }
    }

    return -1;
}

uint16_t DOIC_getCrc(doic_data_t *pdoic)
{
    uint16_t crc = 0;
    if (NULL == pdoic)
        return 0;
    crc = pdoic->data[pdoic->len + 1];
    crc |= pdoic->data[pdoic->len] << 8;

    return crc;
}

uint16_t DOIC_checkCrc(doic_data_t *pdoic)
{
    uint8_t i;
    uint16_t crc = 0;
    uint8_t len;
    uint8_t *pnt = NULL;
    if (NULL == pdoic)
        return 0;

    pnt = (uint8_t *)&pdoic->dst;
    len = pdoic->len + 6;
    while (len-- != 0)
    {
        for (i = 0x80; i != 0; i /= 2)
        {
            if ((crc & 0x8000) != 0)
            {
                crc *= 2;
                crc ^= 0x1021;
            } // 余式CRC 乘以2 再求CRC
            else
                crc *= 2;
            if ((*pnt & i) != 0)
                crc ^= 0x1021; // 再加上本位的CRC
        }
        pnt++;
    }
    return crc;
}

uint16_t DOIC_addCrc(doic_data_t *pdata)
{
    uint16_t crc = DOIC_checkCrc(pdata);
    pdata->data[pdata->len] = crc >> 8;
    pdata->data[pdata->len + 1] = crc & 0xFF;
    return (uint32_t)(&pdata->data[pdata->len + 2]) - (uint32_t)pdata;
}
