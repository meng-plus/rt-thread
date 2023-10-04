/**
 * @file Applications.cpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Applications.hpp"
#include "LedTime.hpp"
#include "CylinderTime.hpp"
CApplications::CApplications()
{
    /* enter interrupt */
    rt_interrupt_enter();

    m_taskCy_ptr = &CTimeTaskcy::GetInstance();
    std::shared_ptr<Observer> led_ptr(&CLedTime::GetInstance()); /*!< ????? */
    std::shared_ptr<CTimeTaskcy> cy_ptr(m_taskCy_ptr);
    CylinderTime::GetInstance().AddObserver(led_ptr); /*!< ????? */
    CylinderTime::GetInstance().AddObserver(cy_ptr);  /*!< ????? */
    m_motorPtr = &CMotor::GetInstance();
    /* leave interrupt */
    rt_interrupt_leave();
}

CApplications::~CApplications()
{
}

void CApplications::thread()
{
    m_taskCy_ptr->control(TASK_CY_CMD::start);
    while (1)
    {
        rt_thread_delay(1000);
    }
}
