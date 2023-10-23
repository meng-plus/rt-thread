/**
 * @file task_led.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_led.h"

static rt_timer_t timer_led = NULL;
void task_led(void *param)
{
    rt_led_toggle(LED_0);
}

int task_led_init()
{

    timer_led = rt_timer_create("task.led",
                                task_led,
                                NULL,
                                500,
                                RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
    if (timer_led)
    {
        rt_timer_start(timer_led);
    }
}
INIT_APP_EXPORT(task_led_init);
