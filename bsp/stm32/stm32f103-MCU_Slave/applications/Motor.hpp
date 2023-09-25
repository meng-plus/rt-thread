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
#include "Gpio.hpp"
#include "singleton.h"
class CMotor : public OHOS::DelayedRefSingleton<CMotor>
{
private:
    void *pwm_dev;
    CGpioY m_Dir;

    void pwm_set(uint32_t period, uint32_t pulse);

public:
    CMotor(/* args */);
    virtual ~CMotor();
    void enable();
    void disable();
    /**
     * @brief Set the Ratio object
     *
     * @param per ranger [-100%,100%]
     */
    void setRatio(float per);
};
