/**
 * @file CylinderTime.hpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "osTime.hpp"
#include "singleton.h"
#include <drv_gpio.h>
#include "stdbool.h"

class Cled
{
private:
    rt_base_t m_pin;
    bool m_flag : 1;

public:
    Cled(rt_base_t pin)
      :m_flag(0)
    {
        m_pin = pin;
        rt_pin_mode(pin, PIN_MODE_OUTPUT);
        set(m_flag);
    }
    ~Cled()
    {
        rt_pin_mode(m_pin, PIN_MODE_INPUT);
    }
    void set(bool bflag)
    {
        m_flag = bflag;
        rt_pin_write(m_pin, m_flag);
    }
    void Toggle()
    {
        set(!m_flag);
    }
};

class CLedTime : public osTime, public OHOS::DelayedRefSingleton<CLedTime>
{
private:
    virtual void Tick();

protected:
    Cled m_run;
    Cled m_warning;
    Cled m_err;

public:
    CLedTime(/* args */);
    virtual ~CLedTime();
};
