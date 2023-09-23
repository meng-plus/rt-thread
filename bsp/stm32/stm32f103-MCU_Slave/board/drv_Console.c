/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-10-30     SummerGift   first version
 * 2020-03-16     SummerGift   add device close feature
 * 2020-03-20     SummerGift   fix bug caused by ORE
 * 2020-05-02     whj4674672   support stm32h7 uart dma
 * 2020-09-09     forest-rain  support stm32wl uart
 * 2020-10-14     Dozingfiretruck   Porting for stm32wbxx
 */

#include "board.h"
#include "drv_Console.h"
#include "drv_config.h"
#include "SEGGER_RTT.h"
// #define DRV_DEBUG
#define LOG_TAG "drv.drv_Console"
#include <drv_log.h>
#define BufferIndex 0
static rt_timer_t RTT_time_handle;
// static uint8_t RTT_RX_Buff[32];
struct rt_serial_device serial_RTT;
void rtt_cb(void *param)
{
    //    unsigned NumBytesRead = SEGGER_RTT_Read(BufferIndex, RTT_RX_Buff, sizeof(RTT_RX_Buff));
    rt_hw_serial_isr(&serial_RTT, RT_SERIAL_EVENT_RX_IND);
}
static rt_err_t stm32_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    SEGGER_RTT_Init();
    if (RTT_time_handle == NULL)
    {
        RTT_time_handle = rt_timer_create(
            "rtt",
            rtt_cb,
            0,
            100,
            RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
    }
    return RT_EOK;
}

static rt_err_t stm32_control(struct rt_serial_device *serial, int cmd, void *arg)
{

    switch (cmd)
    {
    /* disable interrupt */
    case RT_DEVICE_CTRL_CLR_INT:
        break;

    /* enable interrupt */
    case RT_DEVICE_CTRL_SET_INT:
        rt_timer_start(RTT_time_handle);
        break;

    case RT_DEVICE_CTRL_CLOSE:

        break;
    }
    return RT_EOK;
}

static int stm32_putc(struct rt_serial_device *serial, char c)
{

    return SEGGER_RTT_PutChar(BufferIndex, c);
}

static rt_ssize_t stm32_dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
{
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(buf != RT_NULL);
    if (size == 0)
    {
        return 0;
    }
    if (SEGGER_RTT_WriteNoLock(BufferIndex, buf, size) == HAL_OK)
    {
        return size;
    }
    else
    {
        return 0;
    }
}
static int stm32_getc(struct rt_serial_device *serial)
{
    return SEGGER_RTT_GetKey();
}

static const struct rt_uart_ops RTT_uart_ops =
    {
        .configure = stm32_configure,
        .control = stm32_control,
        .putc = stm32_putc,
        .getc = stm32_getc,
        .dma_transmit = stm32_dma_transmit};

int rt_hw_RTT_init(void)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    rt_err_t result = 0;

    /* init UART object */
    serial_RTT.ops = &RTT_uart_ops;
    serial_RTT.config = config;

    /* register UART device */
    result = rt_hw_serial_register(&serial_RTT, "RTT",
                                   RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, NULL);
    RT_ASSERT(result == RT_EOK);
    return result;
}
