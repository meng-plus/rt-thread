/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-10     armink       first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#define LOG_TAG              __FILE__
#define LOG_LVL              LOG_LVL_DBG
#include "rtdbg.h"
int main(void)
{
    LOG_I("Hello RT-Thread!");
    LOG_D("Let's start developing\n");

    return RT_EOK;
}