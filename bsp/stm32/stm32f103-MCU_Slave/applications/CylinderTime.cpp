/**
 * @file CylinderTime.cpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "CylinderTime.hpp"

#include <rtthread.h>
#define LOG_TAG "shell.cyTime"
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"
CylinderTime::CylinderTime(/* args */)
    : osTime("CylinderTime", RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER, NULL, 10)
{
    m_Cylinder[(uint8_t)Cylinder_DEV::A] = new CCylinder("A", GET_PIN(B, 0), GET_PIN(B, 11), GET_PIN(B, 12));
    m_Cylinder[(uint8_t)Cylinder_DEV::B] = new CCylinder("B", GET_PIN(A, 4), GET_PIN(B, 13), GET_PIN(B, 14));
    m_Cylinder[(uint8_t)Cylinder_DEV::C] = new CCylinder("C", GET_PIN(A, 5), GET_PIN(B, 15), GET_PIN(B, 3));
    m_Cylinder[(uint8_t)Cylinder_DEV::D] = new CCylinder("D", GET_PIN(A, 6), GET_PIN(A, 8), GET_PIN(A, 9));
    m_Cylinder[(uint8_t)Cylinder_DEV::E] = new CCylinder("E", GET_PIN(A, 7), GET_PIN(A, 10), GET_PIN(A, 11));

    start();
}

CylinderTime::~CylinderTime()
{
}
void CylinderTime::Tick()
{
    SetChanged();
    for (size_t i = 0; i < (uint8_t)Cylinder_DEV::NUM; i++)
    {
        CCylinder *dev_ptr = m_Cylinder[i];
        CYLINDER_STATUS status_new = dev_ptr->m_status;
        if ((CYLINDER_STATUS::SET == dev_ptr->getStatus_target()) &&
            (CYLINDER_STATUS::SET != dev_ptr->m_status))
        {

            switch (dev_ptr->m_status)
            {
            case CYLINDER_STATUS::RESET:

                if (0 == dev_ptr->read_i0())
                {
                    status_new = CYLINDER_STATUS::MID_NO;
                }
                break;
            case CYLINDER_STATUS::MID_NO:
                if (dev_ptr->read_i0())
                {
                    status_new = CYLINDER_STATUS::MID;
                }
                break;
            case CYLINDER_STATUS::MID:
                if (!dev_ptr->read_i0())
                {
                    status_new = CYLINDER_STATUS::SETING;
                }
                break;

            default:
                break;
            }
            if (dev_ptr->read_i1())
            {
                status_new = CYLINDER_STATUS::SET;
            }
        }

        if ((CYLINDER_STATUS::RESET == dev_ptr->getStatus_target()) &&
            (CYLINDER_STATUS::RESET != dev_ptr->m_status))
        {
            CYLINDER_STATUS status_new = dev_ptr->m_status;
            switch (dev_ptr->m_status)
            {
            case CYLINDER_STATUS::SET:
                if (!dev_ptr->read_i1())
                {
                    status_new = CYLINDER_STATUS::MID_NO;
                }
                break;
            case CYLINDER_STATUS::MID_NO:
                if (dev_ptr->read_i0())
                {
                    status_new = CYLINDER_STATUS::MID;
                }
                break;
            case CYLINDER_STATUS::MID:
                if (!dev_ptr->read_i0())
                {
                    status_new = CYLINDER_STATUS::RESET;
                }
                break;

            default:
                break;
            }
        }

        if (status_new != dev_ptr->m_status)
        {
            dev_ptr->m_status = status_new;
            ObsertverCyVal notify(dev_ptr, dev_ptr->getStatus(), dev_ptr->getActDiff());
            NotifyObservers(&notify);
        }
        else if (dev_ptr->getActDiff() > 1000)
        {
            ObsertverCyVal notify(dev_ptr, dev_ptr->getStatus(), dev_ptr->getActDiff());
            NotifyObservers(&notify);
            dev_ptr->setStatus(CYLINDER_STATUS::ERROR);
        }
    }
}
void CylinderTime::control(CylinderTimeCmd cmd, void *param)
{
    switch (cmd)
    {
    case CylinderTimeCmd::RESET:
        for (size_t i = 0; i < (uint8_t)Cylinder_DEV::NUM; i++)
        {
            CCylinder *dev_ptr = m_Cylinder[i];
            dev_ptr->reset();
        }
        break;

    default:
        break;
    }
}
