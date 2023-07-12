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

#include "lpc_eeprom.h"

static int init(void)
{
    /* do nothing now */
    EEPROM_Init();
    return 0;
}

static int read(long offset, uint8_t *buf, size_t size)
{
    uint32_t addr = eeprom_onchip_flash.addr + offset;
    EEPROM_Read(addr % 64, addr / 0x64, buf, MODE_8_BIT, size);
    return size;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    uint32_t addr = eeprom_onchip_flash.addr + offset;
    EEPROM_Write(addr % 64, addr / 0x64, (void *)buf, MODE_8_BIT, size);
    return size;
}

static int erase(long offset, size_t size)
{
    for (long page_address = offset; page_address < size; page_address += 64)
    {
        // EEPROM_Erase(page_address / 64);
    }

    return size;
}

const struct fal_flash_dev eeprom_onchip_flash =
    {
        .name = "eeprom_onchip",
        .addr = 0,
        .len = 4032,
        .blk_size = 64,
        .ops = {init, read, write, erase},
        .write_gran = 8};
