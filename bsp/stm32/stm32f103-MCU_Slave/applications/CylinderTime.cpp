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
    : osThread("cyThread", NULL, 512, 15)
//: osTime("CylinderTime", RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER, NULL, 2)
{
    m_Cylinder[(uint8_t)Cylinder_DEV::A] = new CCylinder("A", GET_PIN(B, 0), GET_PIN(B, 11), GET_PIN(B, 12));
    m_Cylinder[(uint8_t)Cylinder_DEV::B] = new CCylinder("B", GET_PIN(A, 4), GET_PIN(B, 13), GET_PIN(B, 14));
    m_Cylinder[(uint8_t)Cylinder_DEV::C] = new CCylinder("C", GET_PIN(A, 5), GET_PIN(B, 15), GET_PIN(B, 3));
    m_Cylinder[(uint8_t)Cylinder_DEV::D] = new CCylinder("D", GET_PIN(A, 6), GET_PIN(A, 8), GET_PIN(A, 9));
    m_Cylinder[(uint8_t)Cylinder_DEV::E] = new CCylinder("E", GET_PIN(A, 7), GET_PIN(A, 10), GET_PIN(A, 0));
    for (size_t i = 0; i < (uint8_t)Cylinder_DEV::NUM; i++)
    {
        m_Cylinder[i]->AddObserver(std::shared_ptr<OHOS::Observer>(this));
        m_Cylinder[i]->IrqEnable(true);
    }

    startup();
}

CylinderTime::~CylinderTime()
{
}
void CylinderTime::thread()
{
    while (1)
    {
        rt_thread_delay(200);
        Tick();
    }
}
void CylinderTime::Tick()
{
    for (size_t i = 0; i < (uint8_t)Cylinder_DEV::NUM; i++)
    {
        UpdateCylinder(i);
    }
}
void CylinderTime::Update(const Observable *o, const OHOS::ObserverArg *arg)
{
    const CGpioArg *argPtr = (const CGpioArg *)arg;
    if (argPtr != NULL)
    {
        // LOG_D("%s %d  val:%d ", argPtr->name, argPtr->m_id, argPtr->val);
        const char strTable[] = "ABCDE";
        for (uint8_t i = 0; i < (uint8_t)Cylinder_DEV::NUM; i++)
        {
            if (argPtr->name[0] == strTable[i])
            {
                UpdateCylinder(i);
                // rt_kprintf("%s irq:%d\r\n",argPtr->name,argPtr->val );
            }
        }
    }
}
void CylinderTime::UpdateCylinder(uint8_t idx)
{
    SetChanged();
    CCylinder *dev_ptr = m_Cylinder[idx];
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
            else
            {
                status_new = CYLINDER_STATUS::MID;
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
        //        if (dev_ptr->getActDiff() > 1000)
        //        {
        //            ObsertverCyVal notify(dev_ptr, dev_ptr->getStatus(), dev_ptr->getActDiff());
        //            NotifyObservers(&notify);
        //            dev_ptr->setStatus(CYLINDER_STATUS::ERROR);
        //        }
    }

    if ((CYLINDER_STATUS::RESET == dev_ptr->getStatus_target()) &&
        (CYLINDER_STATUS::RESET != dev_ptr->m_status))
    {
        switch (dev_ptr->m_status)
        {
        case CYLINDER_STATUS::SET:
            if (!dev_ptr->read_i1())
            {
                if (dev_ptr->read_i0())
                {
                    status_new = CYLINDER_STATUS::MID;
                }
                else
                {
                    status_new = CYLINDER_STATUS::MID_NO;
                }
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
        //        if (dev_ptr->getActDiff() > 1000)
        //        {
        //            ObsertverCyVal notify(dev_ptr, dev_ptr->getStatus(), dev_ptr->getActDiff());
        //            NotifyObservers(&notify);
        //            dev_ptr->setStatus(CYLINDER_STATUS::ERROR);
        //        }
    }

    if (status_new != dev_ptr->m_status)
    {
        // rt_kprintf("%s:status %d->%d\r\n",dev_ptr->m_name,dev_ptr->m_status,status_new );

        dev_ptr->m_status = status_new;
        ObsertverCyVal notify(dev_ptr, dev_ptr->getStatus(), dev_ptr->getActDiff());
        NotifyObservers(&notify);
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
