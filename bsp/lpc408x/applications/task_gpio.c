/**
 * @file task_gpio.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 通用的输入输出
 * @version 0.1
 * @date 2023-11-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "task_gpio.h"
#include "drv_pin.h"
static const rt_base_t outpin[OUT_NUM] = {
    [OUT_A] = {GET_PIN(0, 29)},
    [OUT_B] = {GET_PIN(0, 30)},
    [OUT_C] = {GET_PIN(0, 14)},
    [PUMP_AIR] = {GET_PIN(4, 31)},
};
static const rt_base_t inpin[INPUT_NUM] = {
    [INPUT_S1] = {GET_PIN(4, 15)},
    [INPUT_S2] = {GET_PIN(5, 01)},
    [INPUT_S3] = {GET_PIN(5, 02)},
    [INPUT_S4] = {GET_PIN(4, 24)},
    [INPUT_S5] = {GET_PIN(4, 19)},
    [INPUT_S6] = {GET_PIN(0, 13)},
};
int task_gpio_init()
{
    for (int i = 0; i < OUT_NUM; i++)
    {
        rt_pin_write(outpin[i], 0);
        rt_pin_mode(outpin[i], PIN_MODE_OUTPUT);
    }
    for (int i = 0; i < INPUT_NUM; i++)
    {
        rt_pin_write(inpin[i], 0);
        rt_pin_mode(inpin[i], PIN_MODE_INPUT);
    }
    return 0;
}
INIT_DEVICE_EXPORT(task_gpio_init);

void gpio_set(enum GPIO_OUTPUT pin, uint8_t en)
{
    rt_pin_write(outpin[pin], en);
}
uint8_t gpio_read(enum GPIO_INPUT pin)
{
    return rt_pin_read(inpin[pin]) & 0xFF;
}
