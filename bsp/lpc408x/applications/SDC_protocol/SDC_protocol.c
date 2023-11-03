
#include "SDC_protocol.h"
#include <string.h>
void SDC_master_init(sdc_master_t *phander, sdc_map_t *pmap, uint16_t map_num)
{
    if (phander && pmap)
    {
        phander->map = pmap;
        phander->map_num = map_num;
    }
}

uint16_t SDC_master_0x10(sdc_master_t *phander, uint8_t slave_addr, enum SENSOR_DEF sensor_type, uint8_t chn)
{

    sdc_data_t *pdata = phander->pdata;
    if (0 == phander || 0 == pdata)
    {
        return 0;
    }
    memset(pdata, 0xFB, 3);
    pdata->dstIP = slave_addr;
    pdata->srcIP = phander->addr;
    pdata->length = 3;
    pdata->cmd = 0x10;
    pdata->data[0] = sensor_type;
    pdata->data[1] = chn;

    return SDC_addCrc(pdata);
}

int8_t SDC_deal(sdc_master_t *phander, sdc_data_t *pdst)
{
    if (0 == phander || 0 == phander->map)
    {
        return -1;
    }
    sdc_data_t *pdata = phander->pdata;
    const sdc_map_t *pmap;
    for (size_t i = 0; i < phander->map_num; i++)
    {
        pmap = &phander->map[i];
        if (pdst->cmd == pmap->cmd &&
            pmap->func)
        {
            return pmap->func(pdata, pdst);
        }
    }

    return -1;
}
/**
 * @brief 校验多项式0x1021，分站发送时校验和高位在<前>，
 *                       传感器回复时校验和高位在<后>。
 *
 * @param psdc 收到的数据句柄
 * @return uint16_t
 */
uint16_t SDC_getCrc(sdc_data_t *psdc)
{
    uint16_t crc = 0;
    if (NULL == psdc)
        return 0;
    crc = psdc->data[psdc->length] << 8;
    crc |= psdc->data[psdc->length + 1];

    return crc;
}

uint16_t SDC_checkCrc(sdc_data_t *psdc)
{
    uint8_t i;
    uint16_t crc = 0;
    uint8_t len;
    uint8_t *pnt = NULL;
    if (NULL == psdc)
        return 0;

    pnt = (uint8_t *)&psdc->dstIP;
    len = psdc->length + 3;
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

uint16_t SDC_addCrc(sdc_data_t *pdata)
{
    uint16_t crc = SDC_checkCrc(pdata);
    pdata->data[pdata->length - 1] = crc >> 8;
    pdata->data[pdata->length] = crc & 0xFF;
    return (uint32_t)(&pdata->data[pdata->length + 1]) - (uint32_t)pdata;
}
