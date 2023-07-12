/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtconfig.h>
#include <board.h>

/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev lpc4088_onchip_flash;
extern const struct fal_flash_dev lpc4088_extflash;
extern const struct fal_flash_dev eeprom_onchip_flash;
extern const struct fal_flash_dev eeprom_onRTC_flash;
/* flash device table */ //&nor_flash0,
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &lpc4088_onchip_flash,                                           \
    &eeprom_onchip_flash,                                            \
    &eeprom_onRTC_flash,                                             \
    &lpc4088_extflash,                                               \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                                      \
{                                                                                           \
    {FAL_PART_MAGIC_WORD,        "bl",   "lpc4088_onchip",           0,       64*0x400, 0}, \
    {FAL_PART_MAGIC_WORD,      "app0",   "lpc4088_onchip",    64*0x400, (512-64)*0x400, 0}, \
    {FAL_PART_MAGIC_WORD,  "eeprom0" ,    "eeprom_onchip",           0,           4032, 0}, \
    {FAL_PART_MAGIC_WORD,  "eeprom1" ,     "eeprom_onRTC",           0,    0x100*0x400, 0}, \
    {FAL_PART_MAGIC_WORD,      "app1", "lpc4088_extflash",           0,    0x200*0x400, 0}, \
    {FAL_PART_MAGIC_WORD, "easyflash", "lpc4088_extflash", 0x200*0x400,    0x600*0x400, 0}, \
    {FAL_PART_MAGIC_WORD,  "download", "lpc4088_extflash", 0x800*0x400,    0x200*0x400, 0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
