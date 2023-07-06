/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-03-15     Liuguang     the first version.
 * 2019-07-19     Magicoe      The first version for LPC55S6x
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <drivers/rtc.h>
#include "drv_rtc.h"
#include "lpc_rtc.h"
#ifdef RT_USING_RTC
static rt_err_t init(void)
{
    RTC_Init(LPC_RTC);
    RTC_Cmd(LPC_RTC, ENABLE);
    return RT_EOK;
}
static rt_err_t get_secs(time_t *sec)
{
    RTC_TIME_Type fullTime;
    RTC_GetFullTime(LPC_RTC, &fullTime);
    struct tm tm;
    tm.tm_sec = fullTime.SEC;
    tm.tm_min = fullTime.MIN;
    tm.tm_hour = fullTime.HOUR;
    tm.tm_mday = fullTime.DOY;
    tm.tm_mon = fullTime.DOM;
    tm.tm_year = fullTime.YEAR - 1900;

    *sec = mktime(&tm);
    return RT_EOK;
}
static rt_err_t set_secs(time_t *sec)
{
    struct tm *tm_ptr = localtime(sec);
    RTC_TIME_Type fullTime;
    fullTime.SEC = tm_ptr->tm_sec;
    fullTime.MIN = tm_ptr->tm_min;
    fullTime.HOUR = tm_ptr->tm_hour;
    fullTime.DOY = tm_ptr->tm_mday;
    fullTime.DOM = tm_ptr->tm_mon;
    fullTime.YEAR = tm_ptr->tm_year + 1900;;
    RTC_SetFullTime(LPC_RTC, &fullTime);
    return RT_EOK;
}
static rt_err_t get_alarm(struct rt_rtc_wkalarm *alarm)
{
    return RT_EOK;
}
static rt_err_t set_alarm(struct rt_rtc_wkalarm *alarm)
{
    return RT_EOK;
}
static rt_err_t get_timeval(struct timeval *tv)
{
    RTC_TIME_Type fullTime;
    RTC_GetFullTime(LPC_RTC, &fullTime);
    struct tm tm;
    tm.tm_sec = fullTime.SEC;
    tm.tm_min = fullTime.MIN;
    tm.tm_hour = fullTime.HOUR;
    tm.tm_mday = fullTime.DOY;
    tm.tm_mon = fullTime.DOM;
    tm.tm_year = fullTime.YEAR - 1900;

    tv->tv_sec = mktime(&tm);
    return RT_EOK;
}
static rt_err_t set_timeval(struct timeval *tv)
{
    struct tm *tm_ptr = localtime(&tv->tv_sec);
    RTC_TIME_Type fullTime;
    fullTime.SEC = tm_ptr->tm_sec;
    fullTime.MIN = tm_ptr->tm_min;
    fullTime.HOUR = tm_ptr->tm_hour;
    fullTime.DOY = tm_ptr->tm_mday;
    fullTime.DOM = tm_ptr->tm_mon;
    fullTime.YEAR = tm_ptr->tm_year + 1900;
    RTC_SetFullTime(LPC_RTC, &fullTime);
    return RT_EOK;
}
const struct rt_rtc_ops lpc_rtc_ops =
    {
        init,
        get_secs,
        set_secs,
        get_alarm,
        set_alarm,
        get_timeval,
        set_timeval,
};
static rt_rtc_dev_t rtc_device;
int rt_hw_rtc_init(void)
{
    rt_err_t ret = RT_EOK;

    rtc_device.ops = &lpc_rtc_ops;
    ret = rt_hw_rtc_register(&rtc_device, "rtc", RT_DEVICE_FLAG_RDWR, NULL);
    if (ret != RT_EOK)
    {
        return ret;
    }

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_rtc_init);

#endif /*RT_USING_RTC */
