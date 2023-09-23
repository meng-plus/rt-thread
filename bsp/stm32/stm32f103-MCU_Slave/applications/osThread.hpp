/**
 * @file osThread.hpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief œﬂ≥Ã¿‡
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <stdint.h>

class osThread
{
private:
    void *m_handle;
    void *parameter;
    static void entry(osThread *handle);

protected:
    virtual void thread() = 0;

public:
    osThread(const char *name,
             void *parameter = 0,
             uint32_t stack_size = 1024,
             uint8_t priority = 25,
             uint32_t tick = 5);
    virtual ~osThread();
    int32_t startup();
    int32_t suspend();
};
