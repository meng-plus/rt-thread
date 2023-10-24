/**
 * @file rt_hw_extflash.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 扩展的外部flash
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "drv_extflash.h"
#include <board.h>
#include <stdint.h>
#include <stdbool.h>
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_INFO
#include "ulog.h"
#define LPC_SPIFI_BASE 0x20094000
/** SPIFI memory base address */
#define SPIFLASH_BASE_ADDRESS (0x28000000)

int SPIFI_Init(void);
SPIFI_HANDLE_T *pSPIFIHandle;
/* Local memory, 32-bit aligned that will be used for driver context (handle) */
static uint32_t lmem[21];
static uint8_t init_ok = 0;
int rt_hw_extflash_init(void)
{
    if (init_ok)
        return 0;
    init_ok = 1;
    /* init SPIFI clock and pins */
    LPC_SC->PCONP |= (1UL << 16);           /* enable SPIFI power/clock   */
    LPC_SC->PCONP |= ((uint32_t)(1 << 15)); // CLKPWR_PCONP_PCGPIO
    LPC_IOCON->P2_7 &= ~(7UL << 0);
    LPC_IOCON->P2_7 |= (5UL << 0); /* SPIFI_CSN = P2.7  (FUNC 5) */
    LPC_IOCON->P0_22 &= ~(7UL << 0);
    LPC_IOCON->P0_22 |= (5UL << 0); /* SPIFI_CLK = P0.22 (FUNC 5) */
    LPC_IOCON->P0_15 &= ~(7UL << 0);
    LPC_IOCON->P0_15 |= (5UL << 0); /* SPIFI_IO2 = P0.15 (FUNC 5) */
    LPC_IOCON->P0_16 &= ~(7UL << 0);
    LPC_IOCON->P0_16 |= (5UL << 0); /* SPIFI_IO3 = P0.16 (FUNC 5) */
    LPC_IOCON->P0_17 &= ~(7UL << 0);
    LPC_IOCON->P0_17 |= (5UL << 0); /* SPIFI_IO1 = P0.17 (FUNC 5) */
    LPC_IOCON->P0_18 &= ~(7UL << 0);
    LPC_IOCON->P0_18 |= (5UL << 0); /* SPIFI_IO0 = P0.18 (FUNC 5) */

    /*=========================
    =
    ==========================*/
    /* Initialize the SPIFI									*/
    spifiInit(LPC_SPIFI_BASE, true);
    /* register support for the family(s) we may want to work with				*/
    spifiRegisterFamily(SPIFI_REG_FAMILY_SpansionS25FLP);
    int8_t id = spifiGetSuppFamilyCount();
    if (id == 0)
        return 1;

    /* Get required memory for detected device, this may vary per device family */
    size_t size = spifiGetHandleMemSize(LPC_SPIFI_BASE);
    if (size == 0)
        return 2;

    /* Initialize and detect a device and get device context */
    pSPIFIHandle = spifiInitDevice(&lmem, sizeof(lmem), LPC_SPIFI_BASE, SPIFLASH_BASE_ADDRESS);
    if (pSPIFIHandle == NULL)
        return 3;

    /* Enable quad.  If not supported it will be ignored */
    spifiDevSetOpts(pSPIFIHandle, SPIFI_OPT_USE_QUAD, true);

    /* Enter memMode */
    spifiDevSetMemMode(pSPIFIHandle, true);

    LOG_I("version:0x%0x", spifiGetLibVersion());
    LOG_I("SuppFamilyName:%s", spifiGetSuppFamilyName(0));

    LOG_D("spifiCtrlAddr:0x%0x", pSPIFIHandle->pInfoData->spifiCtrlAddr);
    LOG_D("baseAddr:0x%0x", pSPIFIHandle->pInfoData->baseAddr);
    LOG_D("numBlocks:0x%0x", pSPIFIHandle->pInfoData->numBlocks);
    LOG_D("blockSize:0x%0x", pSPIFIHandle->pInfoData->blockSize);
    LOG_D("numSubBlocks:0x%0x", pSPIFIHandle->pInfoData->numSubBlocks);
    LOG_D("subBlockSize:0x%0x", pSPIFIHandle->pInfoData->subBlockSize);
    LOG_D("pageSize:0x%0x", pSPIFIHandle->pInfoData->pageSize);
    LOG_D("maxReadSize:0x%0x", pSPIFIHandle->pInfoData->maxReadSize);
    LOG_D("pDevName:%s", pSPIFIHandle->pInfoData->pDevName);
    LOG_I("pInfoData");

    LOG_D("name:%s", pSPIFIHandle->pInfoData->pDeviceData->pDevName);
    LOG_D("id.extCount:0x%0x", pSPIFIHandle->pInfoData->pDeviceData->id.extCount);
    LOG_D("caps:0x%0x", pSPIFIHandle->pInfoData->pDeviceData->caps);
    LOG_D("blks:0x%0x", pSPIFIHandle->pInfoData->pDeviceData->blks);
    LOG_D("blkSize:0x%0x", pSPIFIHandle->pInfoData->pDeviceData->blkSize);
    LOG_D("subBlks:0x%0x", pSPIFIHandle->pInfoData->pDeviceData->subBlks);
    LOG_D("subBlkSize:0x%0x", pSPIFIHandle->pInfoData->pDeviceData->subBlkSize);
    LOG_D("pageSize:0x%0x", pSPIFIHandle->pInfoData->pDeviceData->pageSize);
    LOG_D("maxReadSize:0x%0x", pSPIFIHandle->pInfoData->pDeviceData->maxReadSize);
    LOG_D("maxClkRate:0x%0x", pSPIFIHandle->pInfoData->pDeviceData->maxClkRate);
    LOG_D("maxProgramRate:0x%0x", pSPIFIHandle->pInfoData->pDeviceData->maxProgramRate);

    return 0;
}
