/**
 * @file Motor.hpp
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief 直流电机控制器
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Motor.hpp"
#include "rtdevice.h"
#define PWM_DEV_NAME "pwm4" /* PWM设备名称 */
#define PWM_DEV_CHANNEL 1   /* PWM通道 */

CMotor::CMotor(/* args */)
    : m_Dir(GET_PIN(B, 5))
{
    pwm_dev = rt_device_find(PWM_DEV_NAME);
    setRatio(0);
    enable();
}

CMotor::~CMotor()
{
}

void CMotor::pwm_set(uint32_t period, uint32_t pulse)
{ /* 设置PWM周期和脉冲宽度 */
    rt_pwm_set((struct rt_device_pwm *)pwm_dev, PWM_DEV_CHANNEL, period, pulse);
}

void CMotor::enable()
{
    rt_pwm_enable((struct rt_device_pwm *)pwm_dev, PWM_DEV_CHANNEL);
}

void CMotor::disable()
{
    rt_pwm_disable((struct rt_device_pwm *)pwm_dev, PWM_DEV_CHANNEL);
    m_Dir.write(false);
}

void CMotor::setRatio(float per)
{
    if (per > 1.0)
    {
        return;
    }
    else if (per > 0)
    {
        pwm_set(1000, (uint32_t)(per * 1000));
        m_Dir.write(false);
    }
    else if (per == 0)
    {
        pwm_set(1000, 1000);
        m_Dir.write(true);
    }
    else if (per > -1.0)
    {
        m_Dir.write(true);
        pwm_set(1000, (uint32_t)((1 - per) * 1000));
    }
}
