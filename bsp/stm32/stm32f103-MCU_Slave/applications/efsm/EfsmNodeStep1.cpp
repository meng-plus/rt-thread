/**
 * @file EfsmNodeStep1.hpp
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief 状态机的空闲状态
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "EfsmNodeStep1.hpp"
#include "EfsmTaskCy.hpp"

static const uint8_t cyMapStep1[] = {0, 1, 2, 3};
static const uint8_t cyMapStep2[][2] = {{1, 3}, {0, 2}, {1, 3}, {0, 2}};
static const uint8_t cyMapStep3[] = {2, 3, 0, 1};
static const uint8_t cyMapStep4[] = {4, 4, 4, 4};

CEfsmNodeStep1::CEfsmNodeStep1(/* args */)
    : x0_start(GET_PIN(A, 0)),
      x13_dir1(GET_PIN(A, 15)),
      x14_dir2(GET_PIN(B, 4))
{
}
void CEfsmNodeStep1::init()
{
    m_step = 0;
    m_cy_ptr = &CylinderTime::GetInstance();
}
void CEfsmNodeStep1::exit()
{
}
void CEfsmNodeStep1::action(const CEfsmManage *parent, const CEfsmEvent *event_ptr)
{
}
void CEfsmNodeStep1::tick()
{
    switch (m_step)
    {
    case 0:
        if (x0_start.read() == 0)
        {
            m_step++;
        }
        break;
    case 1: /*!<  check start */
        if (x0_start.read())
        {
            m_step++;
            m_dir |= x13_dir1.read() ? 0x02 : 0x00;
            m_dir |= x14_dir2.read() ? 0x01 : 0x00;
            CEfsmTaskCy::GetInstance().setDir(m_dir);

            m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->set();
        }
        break;
    case 2: /*!< 等待第一步到位 */
        if (CYLINDER_STATUS::SET == m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->getStatus() &&
            m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->getActDiff() > 50)
        {
            m_step++;

            // CEfsmEvent event(EFSM_EVENT::NEXT);
            // CEfsmTaskCy::GetInstance().action(&event);
        }
        /*!< 步骤2**************************************** */
    case 3:
        m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][0]]->set();
        m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][1]]->set();

        break;
    default:
        break;
    }
}
