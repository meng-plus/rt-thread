/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */
#include <rtthread.h>
#include "board.h"
#include "drv_pin.h"
#include <ctype.h>
#include <stdlib.h>
#include "drv_led.h"
#define LED_DEVICE_CTRL 0x81 /*LED control command*/

#define LED_NUM 4
struct led_ctrl
{
    rt_base_t pin;
};

struct lpc_led
{
    /* inherit from rt_device */
    struct rt_device parent;

    struct led_ctrl ctrl[LED_NUM];
};

static struct lpc_led led = {.ctrl = {
                                 GET_PIN(4, 30),
                                 GET_PIN(4, 29),
                                 GET_PIN(4, 27),
                                 GET_PIN(4, 26),
                             }};
void rt_led_write(enum LED_PIN pin, rt_uint8_t value)
{
    rt_pin_write(led.ctrl[pin].pin, !value);
}
void rt_led_toggle(enum LED_PIN epin)
{
    rt_base_t pin = led.ctrl[epin].pin;
    rt_pin_write(pin, !rt_pin_read(pin));
}
static rt_err_t led_init(rt_device_t dev)
{
    for (int index = 0; index < LED_NUM; index++)
    {
        rt_pin_write(led.ctrl[index].pin, 1);
        rt_pin_mode(led.ctrl[index].pin, PIN_MODE_OUTPUT);
    }
    return RT_EOK;
}

static rt_err_t led_open(rt_device_t dev, rt_uint16_t oflag)
{

    return RT_EOK;
}

static rt_err_t led_close(rt_device_t dev)
{
    return RT_EOK;
}

static rt_ssize_t led_read(rt_device_t dev, rt_off_t pos, void *buffer,
                              rt_size_t size)
{
    rt_ubase_t index = 0;
    rt_ubase_t nr = size;
    rt_uint8_t *value = buffer;

    RT_ASSERT(dev == &led.parent);

    for (int index = 0; (pos + index < LED_NUM) && index < nr; index++)
    {
        *value = (!rt_pin_read(led.ctrl[pos + index].pin)) + '0';
        value++;
    }
    return index;
}

static rt_ssize_t led_write(rt_device_t dev, rt_off_t pos,
                               const void *buffer, rt_size_t size)
{
    rt_ubase_t index = 0;
    rt_ubase_t nw = size;
    const rt_uint8_t *value = buffer;

    RT_ASSERT(dev == &led.parent);
    RT_ASSERT((pos + size) <= LED_NUM);

    for (index = 0; index < nw; index++)
    {
        if (isdigit(*value))
        {
            rt_pin_write(led.ctrl[index].pin, !(*value - '0'));
        }
        else
        {
            return -RT_EINVAL;
        }
        value++;
    }
    return index;
}

static rt_err_t rt_led_control(rt_device_t dev, int cmd, void *args)
{
    RT_ASSERT(dev == &led.parent);

    if (cmd == LED_DEVICE_CTRL)
    {
        rt_uint32_t *led_num = args;
        *led_num = LED_NUM;
    }
    return RT_EOK;
}

int rt_hw_led_init(void)
{
    led.parent.type = RT_Device_Class_Pin;
    led.parent.rx_indicate = RT_NULL;
    led.parent.tx_complete = RT_NULL;
    led.parent.init = led_init;
    led.parent.open = led_open;
    led.parent.close = led_close;
    led.parent.read = led_read;
    led.parent.write = led_write;
    led.parent.control = rt_led_control;
    led.parent.user_data = RT_NULL;

    /* register a character device */
    rt_device_register(&led.parent, "led", RT_DEVICE_FLAG_RDWR);
    led_init(&led.parent);
    return 0;
}
INIT_DEVICE_EXPORT(rt_hw_led_init);

#ifdef RT_USING_FINSH
#include <finsh.h>
void led_test(rt_uint32_t led_num, rt_uint32_t value)
{
    rt_uint8_t led_value = value;
    led_write(&led.parent, led_num, &led_value, 1);
}
FINSH_FUNCTION_EXPORT(led_test, e.g
                      : led_test(0, 100).)
#endif
