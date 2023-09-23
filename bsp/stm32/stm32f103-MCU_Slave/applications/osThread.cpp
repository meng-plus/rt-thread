/**
 * @file osThread.cpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "osThread.hpp"
#include <rtthread.h>

osThread::osThread(const char *name, void *parameter, rt_uint32_t stack_size, rt_uint8_t priority, rt_uint32_t tick)
{
    m_handle = rt_thread_create(name,
                                (void (*)(void *parameter)) entry,
                                this,
                                stack_size,
                                priority,
                                tick);
}

osThread::~osThread()
{
}
int32_t osThread::startup()
{
    return rt_thread_startup((rt_thread_t)m_handle);
}
int32_t osThread::suspend()
{
    return rt_thread_suspend((rt_thread_t)m_handle);
}
void osThread::entry(osThread *handle)
{
    handle->thread();
}
