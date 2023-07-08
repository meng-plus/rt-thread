/**
 * @file drv_watchdog.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-07-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <rtdevice.h>
#include <rtdbg.h>
#include "fm31xx.h"
#ifdef RT_USING_WDT
#define BSP_USING_WDT
enum
{
    DEV_WDT1,
    DEV_MAX,
};

static rt_watchdog_t lpc_watchdog[DEV_MAX];

static rt_err_t lpc_wdog_init(rt_watchdog_t *wdt)
{

    return RT_EOK;
}

static rt_err_t lpc_wdog_control(rt_watchdog_t *wdt, int cmd, void *args)
{
    RT_ASSERT(wdt != NULL);
    static uint16_t timeOut = 3000;
    switch (cmd)
    {
    case RT_DEVICE_CTRL_WDT_GET_TIMEOUT:
    {
        *(uint16_t *)args = timeOut;
    }
    break;
    case RT_DEVICE_CTRL_WDT_SET_TIMEOUT:
    {
        RT_ASSERT(*(uint16_t *)args != 0);
        timeOut = *(uint16_t *)args;
        WDogStop();
    }
    break;
    case RT_DEVICE_CTRL_WDT_KEEPALIVE:
    {
        WDogFeed();
    }
    break;
    case RT_DEVICE_CTRL_WDT_START:
    {
        WDogStart(timeOut);
    }
    break;
    case RT_DEVICE_CTRL_WDT_STOP:
    {
        WDogStop();
    }
    break;
    default:
        return RT_EINVAL;
    }

    return RT_EOK;
}

static struct rt_watchdog_ops lpc_wdog_ops =
    {
        .init = lpc_wdog_init,
        .control = lpc_wdog_control,
};

int rt_hw_wdt_init(void)
{
    rt_err_t ret = RT_EOK;
    for (int idx = 0; idx < sizeof(lpc_watchdog) / sizeof(lpc_watchdog[0]); idx++)
    {
        lpc_watchdog[idx].ops = &lpc_wdog_ops;
        ret = rt_hw_watchdog_register(&lpc_watchdog[idx], "wdt",
                                      RT_DEVICE_FLAG_DEACTIVATE, RT_NULL);
        if (ret != RT_EOK)
        {
            LOG_E("rt device register failed %d\n", ret);
        }
    }

    return ret;
}

INIT_DEVICE_EXPORT(rt_hw_wdt_init);

#endif
