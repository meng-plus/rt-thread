/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-05-19     Bernard      The first version for LPC40xx
 */

#include <rtthread.h>

#include "drv_sdram.h"

#include <lpc_emc.h>
/*=============================================
==
===============================================*/
#define SYS_FREQ 120000000

#if SYS_FREQ == (120000000)
#define SDRAM_PERIOD 8.3 // 120MHz
#elif SYS_FREQ == (96000000)
#define SDRAM_PERIOD 10.4 // 96MHz
#elif SYS_FREQ == (72000000)
#define SDRAM_PERIOD 13.8 // 72MHz
#elif SYS_FREQ == (57000000)
#define SDRAM_PERIOD 17.4 // 57.6MHz
#elif SYS_FREQ == (48000000)
#define SDRAM_PERIOD 20.8 // 48MHz
#elif SYS_FREQ == (36000000)
#define SDRAM_PERIOD 27.8 // 36MHz
#elif SYS_FREQ == (24000000)
#define SDRAM_PERIOD 41.7 // 24MHz
#elif SYS_FREQ == (12000000)
#define SDRAM_PERIOD 83.3 // 24MHz
#else
#error Frequency not defined
#endif

#define P2C(Period) (((Period < SDRAM_PERIOD) ? 0 : (uint32_t)((float)Period / SDRAM_PERIOD)) + 1)

#define SDRAM_REFRESH 7813 //
#define SDRAM_TRP 20       //
#define SDRAM_TRAS 45
#define SDRAM_TAPR 1
#define SDRAM_TDAL 2 //
#define SDRAM_TWR 2
#define SDRAM_TRC 65 //
#define SDRAM_TRFC 65
#define SDRAM_TXSR 67
#define SDRAM_TRRD 15
#define SDRAM_TMRD 3

/*=============================================
==
===============================================*/
extern uint32_t SDRAM_BASE_ADDR;
void BSP_SDRAMInit(void) ;
int rt_hw_sdram_init(void)
{
    BSP_SDRAMInit();
    return 0;
}
void BSP_SDRAMInit(void)
{
    /* Enable EMC clock*/
    LPC_SC->PCONP |= (1 << 11);
    /*The EMC uses the same clock as the CPU*/
    LPC_SC->EMCCLKSEL = 0;
    /*Assign pins to SDRAM controller*/
    LPC_IOCON->P2_16 = 0x21; // EMC_CAS
    LPC_IOCON->P2_17 = 0x21; // EMC_RAS
    LPC_IOCON->P2_18 = 0x21; // EMC_CLK[0]
    LPC_IOCON->P2_20 = 0x21; // EMC_DYCS0
    LPC_IOCON->P2_24 = 0x21; // EMC_CKE0
    LPC_IOCON->P2_28 = 0x21; // EMC_DQM0
    LPC_IOCON->P2_29 = 0x21; // EMC_DQM1
    LPC_IOCON->P2_30 = 0x21; // EMC_DQM2
    LPC_IOCON->P2_31 = 0x21; // EMC_DQM3
    LPC_IOCON->P3_0 = 0x21;  // D0
    LPC_IOCON->P3_1 = 0x21;  // D1
    LPC_IOCON->P3_2 = 0x21;  // D2
    LPC_IOCON->P3_3 = 0x21;  // D3
    LPC_IOCON->P3_4 = 0x21;  // D4
    LPC_IOCON->P3_5 = 0x21;  // D5
    LPC_IOCON->P3_6 = 0x21;  // D6
    LPC_IOCON->P3_7 = 0x21;  // D7
    LPC_IOCON->P3_8 = 0x21;  // D8
    LPC_IOCON->P3_9 = 0x21;  // D9
    LPC_IOCON->P3_10 = 0x21; // D10
    LPC_IOCON->P3_11 = 0x21; // D11
    LPC_IOCON->P3_12 = 0x21; // D12
    LPC_IOCON->P3_13 = 0x21; // D13
    LPC_IOCON->P3_14 = 0x21; // D14
    LPC_IOCON->P3_15 = 0x21; // D15
    LPC_IOCON->P3_16 = 0x21; // D16
    LPC_IOCON->P3_17 = 0x21; // D17
    LPC_IOCON->P3_18 = 0x21; // D18
    LPC_IOCON->P3_19 = 0x21; // D19
    LPC_IOCON->P3_20 = 0x21; // D20
    LPC_IOCON->P3_21 = 0x21; // D21
    LPC_IOCON->P3_22 = 0x21; // D22
    LPC_IOCON->P3_23 = 0x21; // D23
    LPC_IOCON->P3_24 = 0x21; // D24
    LPC_IOCON->P3_25 = 0x21; // D25
    LPC_IOCON->P3_26 = 0x21; // D26
    LPC_IOCON->P3_27 = 0x21; // D27
    LPC_IOCON->P3_28 = 0x21; // D28
    LPC_IOCON->P3_29 = 0x21; // D29
    LPC_IOCON->P3_30 = 0x21; // D30
    LPC_IOCON->P3_31 = 0x21; // D31

    LPC_IOCON->P4_0 = 0x21;  // A0
    LPC_IOCON->P4_1 = 0x21;  // A1
    LPC_IOCON->P4_2 = 0x21;  // A2
    LPC_IOCON->P4_3 = 0x21;  // A3
    LPC_IOCON->P4_4 = 0x21;  // A4
    LPC_IOCON->P4_5 = 0x21;  // A5
    LPC_IOCON->P4_6 = 0x21;  // A6
    LPC_IOCON->P4_7 = 0x21;  // A7
    LPC_IOCON->P4_8 = 0x21;  // A8
    LPC_IOCON->P4_9 = 0x21;  // A9
    LPC_IOCON->P4_10 = 0x21; // A10
    LPC_IOCON->P4_11 = 0x21; // A11
    LPC_IOCON->P4_12 = 0x21; // A12
    LPC_IOCON->P4_13 = 0x21; // A13
    LPC_IOCON->P4_14 = 0x21; // A14
    LPC_IOCON->P4_25 = 0x21; // EMC_WE
    /*Init SDRAM controller*/
    LPC_SC->EMCDLYCTL = (0xF << 0) |  /* CMDDLY */
                        (0x15 << 8) | /* FBCLKDLY */
                        (0x0 << 16);  /* CLKOUT0DLY */

    LPC_EMC->Control = 1; /* enable EMC */
    LPC_EMC->DynamicReadConfig = 1;
    LPC_EMC->DynamicRasCas0 = 0x303;
    LPC_EMC->DynamicRP = P2C(SDRAM_TRP);
    LPC_EMC->DynamicRAS = P2C(SDRAM_TRAS);
    LPC_EMC->DynamicSREX = P2C(SDRAM_TXSR);
    LPC_EMC->DynamicAPR = SDRAM_TAPR;
    LPC_EMC->DynamicDAL = SDRAM_TDAL + P2C(SDRAM_TRP);
    LPC_EMC->DynamicWR = SDRAM_TWR;
    LPC_EMC->DynamicRC = P2C(SDRAM_TRC);
    LPC_EMC->DynamicRFC = P2C(SDRAM_TRFC);
    LPC_EMC->DynamicXSR = P2C(SDRAM_TXSR);
    LPC_EMC->DynamicRRD = P2C(SDRAM_TRRD);
    LPC_EMC->DynamicMRD = SDRAM_TMRD;
    LPC_EMC->DynamicConfig0 = 0x0004680; // 13 row, 9 - col, SDRAM
    // JEDEC General SDRAM Initialization Sequence
    // DELAY to allow power and clocks to stabilize ~100 us
    // NOP
    LPC_EMC->DynamicControl = 0x0183;
    for (volatile uint32_t i = 200 * 30; i; i--)
        ;
    // PALL
    LPC_EMC->DynamicControl = 0x0103;
    LPC_EMC->DynamicRefresh = 1;
    for (volatile uint32_t i = 256; i; --i)
        ; // > 128 clk
    LPC_EMC->DynamicRefresh = P2C(SDRAM_REFRESH) >> 4;
    // COMM
    LPC_EMC->DynamicControl = 0x0083;
    // Burst 4, Sequential, CAS-3
    volatile unsigned long Dummy = *(volatile unsigned short *)((uint32_t)&SDRAM_BASE_ADDR + (0x32UL << (13)));
    // NORM
    LPC_EMC->DynamicControl = 0x0000;
    LPC_EMC->DynamicConfig0 = 0x00084680;
    for (volatile uint32_t i = 100000; i; i--)
        ;
}
