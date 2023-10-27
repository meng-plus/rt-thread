/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-5      SummerGift   first version
 */

#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__

#include <rtthread.h>
#include "rtdevice.h"
#include <rthw.h>
#include <board.h>
#include "drv_pin.h"
#include <ipc/completion.h>

#ifdef __cplusplus
extern "C"
{
#endif

    rt_err_t rt_hw_spi_device_attach(const char *bus_name, const char *device_name, rt_base_t cs_pin);

#ifdef __cplusplus
}
#endif

#ifdef BSP_USING_SPI1
#ifndef SPI1_BUS_CONFIG
#define SPI1_BUS_CONFIG        \
    {                          \
        .Instance = LPC_SSP0,  \
        .bus_name = "spi1",    \
        .irq_type = SSP0_IRQn, \
        .sck = {2, 22, 2},     \
        .ssel = {2, 23, 2},    \
        .miso = {2, 26, 2},    \
        .mosi = {2, 27, 2},    \
    }
#endif /* SPI1_BUS_CONFIG */
#endif /* BSP_USING_SPI1 */
#ifdef BSP_USING_SPI2
#ifndef SPI2_BUS_CONFIG
#define SPI2_BUS_CONFIG        \
    {                          \
        .Instance = LPC_SSP1,  \
        .bus_name = "spi2",    \
        .irq_type = SSP1_IRQn, \
        .ssel = {0, 6, 2},     \
        .sck = {0, 7, 2},      \
        .miso = {0, 8, 2},     \
        .mosi = {0, 9, 2},     \
    }
#endif /* SPI2_BUS_CONFIG */
#endif /* BSP_USING_SPI2 */
// #ifdef BSP_USING_SPI3
// #ifndef SPI3_BUS_CONFIG
// #define SPI3_BUS_CONFIG        \
//     {                          \
//         .Instance = LPC_SSP2,  \
//         .bus_name = "spi3",    \
//         .irq_type = SSP2_IRQn, \
//         .ssel = {0, 6, 2},     \
//         .sck = {0, 7, 2},      \
//         .miso = {0, 8, 2},     \
//         .mosi = {0, 9, 2},     \
//     }
// #endif /* SPI3_BUS_CONFIG */
// #endif /* BSP_USING_SPI3 */

struct lpc_spi_config
{
    LPC_SSP_TypeDef *Instance;
    char *bus_name;
    drv_pin_cfg_t sck;
    drv_pin_cfg_t ssel;
    drv_pin_cfg_t miso;
    drv_pin_cfg_t mosi;

    IRQn_Type irq_type;
    struct dma_config *dma_rx, *dma_tx;
};

struct lpc_spi_device
{
    rt_uint32_t pin;
    char *bus_name;
    char *device_name;
};

/* lpc spi dirver class */
struct lpc_spi
{
    LPC_SSP_TypeDef *handle;
    struct lpc_spi_config *config;
    struct rt_spi_configuration *cfg;

    struct rt_spi_bus spi_bus;

    struct rt_completion cpt;
};

#endif /*__DRV_SPI_H__ */
