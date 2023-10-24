/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-26     armink       the first version
 */
#include <stdbool.h>
#include <fal.h>
#include "drv_extflash.h"

static int init(void)
{
    /* do nothing now */
   return rt_hw_extflash_init();
}

static int read(long offset, uint8_t *buf, size_t size)
{
    SPIFI_ERR_T err;
    uint32_t addr = lpc4088_extflash.addr + offset;
    rt_enter_critical();
    err = spifiRead(pSPIFIHandle, addr, (uint32_t *)buf, size);
    if (err != SPIFI_ERR_NONE)
    {
        rt_exit_critical();
        return -err;
    }
    spifiDevSetMemMode(pSPIFIHandle, true);
    rt_exit_critical();
    return 0;
}
/**
 * @brief
 *
 * @param offset  ???
 * @param buf
 * @param size
 * @return int
 */
static int write(long offset, const uint8_t *buf, size_t size)
{
    SPIFI_ERR_T err;
    uint32_t addr = lpc4088_extflash.addr + offset;
    uint32_t mask;
    uint32_t sendBytes;
    rt_enter_critical();
    mask = ~(pSPIFIHandle->pInfoData->pageSize - 1);
    sendBytes = size;
    if (sendBytes > pSPIFIHandle->pInfoData->pageSize)
        sendBytes = pSPIFIHandle->pInfoData->pageSize;

    if ((addr & mask) != ((addr + sendBytes) & mask))
        sendBytes = ((addr + sendBytes) & mask) - addr;

    err = pSPIFIHandle->pFamFx->pageProgram(pSPIFIHandle, addr, (const uint32_t *)buf, sendBytes);
    if (err != SPIFI_ERR_NONE)
    {
        rt_exit_critical();
        return err;
    }
    addr += sendBytes;
    buf += sendBytes;
    size -= sendBytes;

    err = spifiProgram(pSPIFIHandle, addr, (const uint32_t *)buf, size);
    if (err != SPIFI_ERR_NONE)
    {
        rt_exit_critical();
        return -err;
    }
    spifiDevSetMemMode(pSPIFIHandle, true);
    rt_exit_critical();
    return 0;
}

static int erase(long offset, size_t size)
{
    SPIFI_ERR_T err;
    uint32_t addr = lpc4088_extflash.addr + offset;
    rt_enter_critical();
    err = spifiEraseByAddr(pSPIFIHandle, addr, addr + size - 1);
    if (err != SPIFI_ERR_NONE)
    {
        rt_exit_critical();
        return -err;
    }
    spifiDevSetMemMode(pSPIFIHandle, true);
    rt_exit_critical();
    return 0;
}

const struct fal_flash_dev lpc4088_extflash =
    {
        .name = "lpc4088_extflash",
        .addr = 0x28000000,
        .len = 4 * 1024 * 1024,
        .blk_size = 64 * 1024,
        .ops = {init, read, write, erase},
        .write_gran = 32,
};
