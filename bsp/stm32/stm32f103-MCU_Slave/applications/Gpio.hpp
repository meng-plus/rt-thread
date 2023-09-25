/**
 * @file Gpio.hpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief PLC ‰»Î–≈∫≈
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <rtthread.h>
#include <drv_gpio.h>

class CGpio
{
private:
    rt_base_t m_pin;

public:
    CGpio(rt_base_t pin) : m_pin(pin)
    {
    }
    void pin_mode(rt_uint8_t mode)
    {
        rt_pin_mode(m_pin, mode);
    }
    virtual rt_int8_t read()
    {
        return rt_pin_read(m_pin);
    };
    void write(rt_int8_t flag)
    {
        rt_pin_write(m_pin, flag);
    }

    virtual ~CGpio() = default;
};

class CGpioX : public CGpio
{
private:
public:
    CGpioX(rt_base_t pin) : CGpio(pin)
    {
        pin_mode(PIN_MODE_INPUT);
    }
    virtual rt_int8_t read() { return CGpio::read() == 0; }

    ~CGpioX() = default;
};
class CGpioY : public CGpio
{
public:
    CGpioY(rt_base_t pin) : CGpio(pin)
    {
        pin_mode(PIN_MODE_OUTPUT);
    }
    void set()
    {
        write(1);
    };
    void reset() { write(0); };
    virtual ~CGpioY() = default;
};
