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
#pragma once
#include <drv_gpio.h>
#include "osThread.hpp"
#include "observer.h"
#include "singleton.h"
#include "Gpio.hpp"
#include "PID.h"
class CMotor : public osThread,
               public OHOS::DelayedRefSingleton<CMotor>
{
private:
    PIDController pid;
    CGpioY m_X11dir;
    // void *pwm_1;
    void *pwm_2;
    CGpioY m_Y7_Warning;
    CGpioX X12_MotorEn;
    float m_VoltageTarget;
    float m_per;
    uint8_t m_step;

private:
    virtual void thread();

public:
    CMotor(/* args */);
    virtual ~CMotor();
    /*!< 目标电压 */
    void setVoltageTarget(float target);
    void enable();
    void disable();
    void setFreq(int32_t freq);
    /**
     * @brief Set the Ratio object
     *
     * @param per ranger [-100%,100%]
     */
    void setRatio(float per);
};
