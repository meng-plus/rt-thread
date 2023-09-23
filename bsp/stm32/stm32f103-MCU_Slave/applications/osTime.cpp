/**
 * @file osTime.cpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "osTime.hpp"

#include <rtthread.h>

void osTime::time_cb(osTime *param)
{
    param->Tick();
}

osTime::osTime(const char *name, uint8_t flag, void *parameter, uint32_t time)
{
    this->parameter = parameter;
    handle = rt_timer_create(
        name,
        (void (*)(void *parameter))time_cb,
        this,
        time,
        flag);
}

int32_t osTime::control(int cmd, void *arg)
{
    return rt_timer_control((rt_timer_t)handle, cmd, arg);
}

void osTime::setTime(uint32_t time)
{
    rt_timer_control((rt_timer_t)handle, RT_TIMER_CTRL_SET_TIME, &time);
}

void osTime::start()
{
    rt_timer_start((rt_timer_t)handle);
}

void osTime::stop()
{
    rt_timer_stop((rt_timer_t)handle);
}

osTime::~osTime()
{
    rt_timer_delete((rt_timer_t)handle);
}
