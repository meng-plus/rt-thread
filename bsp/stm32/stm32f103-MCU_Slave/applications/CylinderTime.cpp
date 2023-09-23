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

CylinderTime::CylinderTime(/* args */)
    : osTime(__FILE__, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER, NULL, 1)
{
    m_Cylinder[(uint8_t)Cylinder_DEV::A] = new CCylinder(GET_PIN(B, 0), GET_PIN(B, 11), GET_PIN(B, 12));
    m_Cylinder[(uint8_t)Cylinder_DEV::B] = new CCylinder(GET_PIN(A, 4), GET_PIN(B, 13), GET_PIN(B, 14));
    m_Cylinder[(uint8_t)Cylinder_DEV::C] = new CCylinder(GET_PIN(A, 5), GET_PIN(B, 15), GET_PIN(B, 3));
    m_Cylinder[(uint8_t)Cylinder_DEV::D] = new CCylinder(GET_PIN(A, 6), GET_PIN(A, 8), GET_PIN(A, 9));
    m_Cylinder[(uint8_t)Cylinder_DEV::E] = new CCylinder(GET_PIN(A, 7), GET_PIN(A, 10), GET_PIN(A, 11));

    start();
}

CylinderTime::~CylinderTime()
{
}
void CylinderTime::Tick()
{
    for (size_t i = 0; i < (uint8_t)Cylinder_DEV::NUM; i++)
    {
        CCylinder *dev_ptr = m_Cylinder[i];
        if (dev_ptr->m_status == CYLINDER_STATUS::RESETING)
        {
            if (dev_ptr->read_i0() == 1)
            {
                dev_ptr->m_status = CYLINDER_STATUS::RESET;
            }
        }
        else if (dev_ptr->m_status == CYLINDER_STATUS::SETING)
        {
            if (dev_ptr->read_i1() == 1)
            {
                dev_ptr->m_status = CYLINDER_STATUS::SET;
            }
        }
    }
}
