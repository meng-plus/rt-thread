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
#include "drv_rtc.h"

#include "fm31xx.h"
#ifdef RT_USING_RTC
static rt_err_t init(void)
{

    return RT_EOK;
}
static rt_err_t get_secs(time_t *sec)
{
    RTC_TIME dtm;
    RTCReadTime(&dtm);
    struct tm tm;
    tm.tm_sec = dtm.second;
    tm.tm_min = dtm.minute;
    tm.tm_hour = dtm.hour;
    tm.tm_mday = dtm.date;
    tm.tm_mon = dtm.month;
    tm.tm_year = dtm.year + 2000 - 1900;

    *sec = mktime(&tm);
    return RT_EOK;
}
static rt_err_t set_secs(time_t *sec)
{
    struct tm *tm_ptr = localtime(sec);
    RTC_TIME dtm;
    dtm.second = tm_ptr->tm_sec;
    dtm.minute = tm_ptr->tm_min;
    dtm.hour = tm_ptr->tm_hour;
    dtm.date = tm_ptr->tm_mday;
    dtm.month = tm_ptr->tm_mon;
    dtm.year = tm_ptr->tm_year + 1900 - 2000;
    RTCWriteTime(&dtm);
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
    time_t sec;
    get_secs(&sec);

    tv->tv_sec = sec;
    return RT_EOK;
}
static rt_err_t set_timeval(struct timeval *tv)
{
    time_t sec;
    sec = tv->tv_sec;
    set_secs(&sec);

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
