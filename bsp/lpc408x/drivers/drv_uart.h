/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2014-01-05     NongXiaoming      The first version for LPC40xx
 */

#ifndef DRV_UART_H__
#define DRV_UART_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum DRV_SERIAL_PORT
    {
        SERIAL_0 = 0,
        SERIAL_1,
        SERIAL_2,
        SERIAL_3,
        SERIAL_4,
        SERIAL_NUM,
    } DRV_SERIAL_NAME_E;

    int rt_hw_uart_init(void);

#ifdef __cplusplus
}
#endif
#endif
