/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-05     DongBowen    first version
 */

#include "board.h"

#include <rtthread.h>
#include <rtdevice.h>
#include "system_var.h"

#define LOG_TAG "main"
#define DRV_DEBUG

#include <drv_log.h>
#define LED_PIN    GET_PIN(B, 13)
#define LED_PERIOD (RT_TICK_PER_SECOND / 2)

extern void ad5161_set(rt_uint8_t value);
extern void laser_dac_out(rt_uint32_t value);
int main(void)
{
    LOG_D("HF_version %s,SF_version %s", HF_VERSION_STR, APP_VERSION_STR);
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);

    ad5161_set(g_prod_param.ad5161_value);
    LOG_I("ad5161_value %d", g_prod_param.ad5161_value);
    laser_dac_out(g_prod_param.dac_value);
    LOG_I("dac_value %d", g_prod_param.dac_value);

    while (1)
    {
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_delay(LED_PERIOD / 2);
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_delay(LED_PERIOD / 2);
    };
}

