/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author           Notes
 * 2015-09-02     heyuanjie87      the first version
 */

#ifndef DRV_LED_H__
#define DRV_LED_H__

#include "rtdevice.h"
#ifdef __cplusplus
extern "C"
{
#endif

    enum LED_PIN
    {
        LED_0,
        LED_1,
        LED_2,
        LED_3,
    };

    void rt_LED_write(enum LED_PIN pin, rt_uint8_t value);
    rt_base_t rt_led_set(rt_base_t idx, rt_bool_t);

    int rt_hw_led_init(void);
#ifdef __cplusplus
}
#endif
#endif
