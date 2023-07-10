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

#include "lpc_iap.h"

static int init(void)
{
    /* do nothing now */
    return 0;
}

static int read(long offset, uint8_t *buf, size_t size)
{
    size_t i;
    uint32_t addr = offset;
    for (i = 0; i < size; i++, addr++, buf++)
    {
        *buf = *(uint8_t *)addr;
    }
    return size;
}
/**
 * @brief
 *
 * @param offset  ÐèÒª
 * @param buf
 * @param size
 * @return int
 */
static int write(long offset, const uint8_t *buf, size_t size)
{
    uint32_t addr = offset;
    return -CopyRAM2Flash((uint8_t *)addr, (uint8_t *)buf, IAP_WRITE_256);
}

static int erase(long offset, size_t size)
{
    return -EraseSector(GetSecNum(offset), GetSecNum(offset + size));
}

const struct fal_flash_dev lpc4088_onchip_flash =
    {
        .name = "lpc4088_onchip",
        .addr = 0x00000000,
        .len = 512 * 1024,
        .blk_size = 4 * 1024,
        .ops = {init, read, write, erase},
        .write_gran = 8};
