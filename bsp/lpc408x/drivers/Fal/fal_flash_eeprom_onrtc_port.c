/**
 * @file fal_flash_eeprom_onchip_port.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-07-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <fal.h>

#include "fm31xx.h"
static uint8_t buff[512];
static int init(void)
{    
    rt_memset(buff, 0xFF, sizeof(buff));
    /* do nothing now */
    Fm31xxInit();
    return 0;
}
static int read(long offset, uint8_t *buf, size_t size)
{
    uint32_t addr = eeprom_onRTC_flash.addr + offset;
    return E2PromRead(addr, buf, size);
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    uint32_t addr = eeprom_onRTC_flash.addr + offset;
    return E2PromWrite(addr, (void *)buf, size);
}

static int erase(long offset, size_t size)
{


    if (offset + size > eeprom_onRTC_flash.len)
    {
        return 0;
    }
    for (long page_address = offset; size > 0; page_address += 512)
    {
        uint32_t addr = eeprom_onRTC_flash.addr + page_address;

        if (size >= 512)
        {
            E2PromWrite(addr, (void *)buff, 512);
            size -= 512;
        }
        else
        {
            E2PromWrite(addr, (void *)buff, size);
            size = 0;
        }
    }

    return size;
}

const struct fal_flash_dev eeprom_onRTC_flash =
    {
        .name = "eeprom_onRTC",
        .addr = 0,
        .len = 32 * 1024,
        .blk_size = 512,
        .ops = {init, read, write, erase},
        .write_gran = 8};
