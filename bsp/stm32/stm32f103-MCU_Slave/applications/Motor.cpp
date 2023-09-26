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
#include "Adc.hpp"

#include "PID.h"
#define LOG_TAG "dev.motor"
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"
/* Controller parameters */
#define PID_KP 2.0f
#define PID_KI 0.05f
#define PID_KD 0.05f

#define PID_TAU 0.02f

#define PID_LIM_MIN -1.0f
#define PID_LIM_MAX 1.0f

#define PID_LIM_MIN_INT -1.0f
#define PID_LIM_MAX_INT 1.0f

#define SAMPLE_TIME_S 0.015f

#define PWM_DEV1_NAME "pwm3" /* PWM设备名称 */
#define PWM_DEV1_CHANNEL 2   /* PWM通道 */

#define PWM_DEV2_NAME "pwm4" /* PWM设备名称 */
#define PWM_DEV2_CHANNEL 1   /* PWM通道 */
#define PWM_PERIOD 10000000
CMotor::CMotor(/* args */)
    : osThread("motor", NULL, 1024, 15),
      m_Y7_Warning(GET_PIN(C, 13)),
      X12_MotorEn(GET_PIN(A, 12)),
      m_step(0)
{
    pid = {PID_KP, PID_KI, PID_KD,
           PID_TAU,
           PID_LIM_MIN, PID_LIM_MAX,
           PID_LIM_MIN_INT, PID_LIM_MAX_INT,
           SAMPLE_TIME_S};
    PIDController_Init(&pid);

    pwm_1 = rt_device_find(PWM_DEV1_NAME);
    pwm_2 = rt_device_find(PWM_DEV2_NAME);
    setRatio(0);
    rt_pwm_set_period((struct rt_device_pwm *)pwm_1, PWM_DEV1_CHANNEL, PWM_PERIOD);
    rt_pwm_set_period((struct rt_device_pwm *)pwm_2, PWM_DEV2_CHANNEL, PWM_PERIOD);
    rt_pwm_set((struct rt_device_pwm *)pwm_1, PWM_DEV1_CHANNEL, PWM_PERIOD, 0);
    rt_pwm_set((struct rt_device_pwm *)pwm_2, PWM_DEV2_CHANNEL, PWM_PERIOD, 0);
    enable();

    startup(); /*!< 启动定时器 */
}

CMotor::~CMotor()
{
}

void CMotor::enable()
{
    rt_pwm_enable((struct rt_device_pwm *)pwm_1, PWM_DEV1_CHANNEL);
    rt_pwm_enable((struct rt_device_pwm *)pwm_2, PWM_DEV2_CHANNEL);
}

void CMotor::disable()
{
    rt_pwm_disable((struct rt_device_pwm *)pwm_1, PWM_DEV1_CHANNEL);
    rt_pwm_disable((struct rt_device_pwm *)pwm_2, PWM_DEV2_CHANNEL);
}
void CMotor::thread()
{
    while (1)
    {

        if (m_step > 0)
        {
            /**电机启动，开始检测ADC */
            float voltage = CAdc::GetInstance().readVoltage();
            if (m_Y7_Warning.read() && (m_VoltageTarget * 0.995 < voltage) &&
                (voltage < m_VoltageTarget * 1.005))
            {
                m_Y7_Warning.reset();
            }
            else
            {
                if (!m_Y7_Warning.read())
                {
                    m_Y7_Warning.set();
                }
            }
            if (X12_MotorEn.read())
            {
                /* Simulate response using test system */
                // float setpoint = 1.0f; 目标电压

                /* Compute new control signal */
                PIDController_Update(&pid, m_VoltageTarget, voltage);
                setRatio(pid.out);
                LOG_D("vol:%d  pid.out:%d  p:%d i:%d d:%d \r\n", (int)(voltage * 100), (int)(pid.out * 100), (int)(pid.Kp * 100), (int)(pid.Ki * 100), (int)(pid.Kd * 100));
            }
            else
            {
                if (m_per)
                {
                    setRatio(0);
                }
            }
        }
        switch (m_step)
        {
        case 0: /*!< 待机状态 */
            /* code */
            break;
        case 1:

            break;
        default:
            break;
        }

        rt_thread_mdelay(100);
    }
}
void CMotor::setVoltageTarget(float target)
{
    m_VoltageTarget = target;
    m_step = 1;
}
void CMotor::setRatio(float per)
{
    if (per > 1.0)
    {
        return;
    }
    else if (per > 0)
    {
        rt_pwm_set_pulse((struct rt_device_pwm *)pwm_1, PWM_DEV1_CHANNEL, 0);
        rt_pwm_set_pulse((struct rt_device_pwm *)pwm_2, PWM_DEV2_CHANNEL, (per * PWM_PERIOD));
    }
    else if (per == 0)
    {
        rt_pwm_set_pulse((struct rt_device_pwm *)pwm_1, PWM_DEV1_CHANNEL, 0);
        rt_pwm_set_pulse((struct rt_device_pwm *)pwm_2, PWM_DEV2_CHANNEL, 0);
    }
    else if (per >= -1.0)
    {
        rt_pwm_set_pulse((struct rt_device_pwm *)pwm_1, PWM_DEV1_CHANNEL, (-per * PWM_PERIOD));
        rt_pwm_set_pulse((struct rt_device_pwm *)pwm_2, PWM_DEV2_CHANNEL, 0);
    }
    m_per = per;
}
