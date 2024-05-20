/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-26     armink       the first version
 */

#include <fal.h>

#include <hc32l196_flash.h>

/**
 * Get the sector of a given address
 *
 * @param address flash address
 *
 * @return The sector of a given address
 */
static uint32_t stm32_get_sector(uint32_t address)
{
    uint32_t sector = 0;

    sector = address / 0x1FF;
    return sector;
}

/**
 * Get the sector size
 *
 * @param sector sector
 *
 * @return sector size
 */
static uint32_t stm32_get_sector_size(uint32_t sector)
{
    return hc32l19x_onchip_flash.blk_size;
}
static int init(void)
{
    /* do nothing now */
    Flash_Init(12, TRUE);
    return 0;
}

static int read(long offset, uint8_t *buf, size_t size)
{
    size_t i;
    uint32_t addr = hc32l19x_onchip_flash.addr + offset;
    for (i = 0; i < size; i++, addr++, buf++)
    {
        *buf = *(uint8_t *)addr;
    }

    return size;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    size_t i;
    uint32_t read_data;
    uint32_t addr = hc32l19x_onchip_flash.addr + offset;

    Flash_UnlockAll();
    for (i = 0; i < size; i++, buf++, addr++)
    {
        /* write data */
        Flash_WriteByte(addr, *buf);
        read_data = *(uint8_t *)addr;
        /* check data */
        if (read_data != *buf)
        {
            return -1;
        }
    }
    Flash_LockAll();

    return size;
}

static int erase(long offset, size_t size)
{
    en_result_t flash_status;
    size_t erased_size = 0;
    uint32_t cur_erase_sector;
    uint32_t addr = hc32l19x_onchip_flash.addr + offset;

    /* start erase */
    Flash_UnlockAll();
    /* it will stop when erased size is greater than setting size */
    while (erased_size < size)
    {
        cur_erase_sector = stm32_get_sector(addr + erased_size);
        flash_status     = Flash_SectorErase(addr + erased_size);
        if (flash_status != Ok)
        {
            return -1;
        }
        erased_size += stm32_get_sector_size(cur_erase_sector);
    }
    Flash_LockAll();

    return size;
}

const struct fal_flash_dev hc32l19x_onchip_flash =
    {
        .name       = "hc32_onchip",
        .addr       = 0x08000000,
        .len        = 256 * 1024,
        .blk_size   = 512,
        .ops        = {init, read, write, erase},
        .write_gran = 8
};

