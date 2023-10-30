/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-06-01     tyustli     the first version
 */

#ifndef __AD7843_H_
#define __AD7843_H_
#include <rtthread.h>
#include <rtdevice.h>

struct rt_touch_ad7843
{
    struct rt_touch_device parent;
    struct rt_spi_device spi;
    rt_uint16_t min_raw_x;
    rt_uint16_t min_raw_y;
    rt_uint16_t max_raw_x;
    rt_uint16_t max_raw_y;
};

typedef struct rt_touch_ad7843 *rt_touch_ad7843_t;

// int rt_hw_ad7843_init(const char *name, struct rt_touch_config *cfg);
rt_touch_ad7843_t hw_ad7843_init(char *spi_bus_name, rt_base_t cs_pin, rt_base_t irq_pin,
                           rt_int32_t range_x, rt_int32_t range_y,
                           rt_uint16_t min_raw_x, rt_uint16_t min_raw_y,
                           rt_uint16_t max_raw_x, rt_uint16_t max_raw_y);
rt_bool_t hw_ad7843_touchpad_is_pressed(struct rt_touch_device *touch);
void hw_ad7843_calibration(rt_uint16_t min_raw_x, rt_uint16_t min_raw_y,
                           rt_uint16_t max_raw_x, rt_uint16_t max_raw_y);

#endif
