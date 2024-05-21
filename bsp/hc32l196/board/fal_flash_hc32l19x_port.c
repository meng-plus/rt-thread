/**
 * @file fal_flash_hc32l19x_port.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief hc32L19x 内部flash操作说明
 * 根据官方文档要求，操作flash的代码段必须在前32k内，否则会报错
 * 具体操作方式修改link.icf文件，限制flash操作在0x00000000-0x0001FFFF内
 * @version 0.1
 * @date 2024-05-21
 *
 * @copyright Copyright (c) 2024
 *
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

    return size;
}

static int erase(long offset, size_t size)
{
    en_result_t flash_status;
    size_t erased_size = 0;
    uint32_t cur_erase_sector;
    uint32_t addr = hc32l19x_onchip_flash.addr + offset;

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
    return size;
}

const struct fal_flash_dev hc32l19x_onchip_flash =
    {
        .name       = "hc32_onchip",
        .addr       = 0x00000000,
        .len        = 256 * 1024,
        .blk_size   = 512,
        .ops        = {init, read, write, erase},
        .write_gran = 8
};

