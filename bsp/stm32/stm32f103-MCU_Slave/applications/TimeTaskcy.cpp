/**
 * @file TimeTaskcy.cpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "TimeTaskcy.hpp"

static const uint8_t cyMapStep1[] = {0, 1, 2, 3};
static const uint8_t cyMapStep2[][2] = {{1, 3}, {0, 2}, {1, 3}, {0, 2}};
static const uint8_t cyMapStep3[] = {2, 3, 0, 1};
static const uint8_t cyMapStep4[] = {4, 4, 4, 4};

CTimeTaskcy::CTimeTaskcy(/* args */)
    : osTime("Taskcy", RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER, NULL, 500),
      x0_start(GET_PIN(A, 0)),
      x13_dir1(GET_PIN(A, 15)),
      x14_dir2(GET_PIN(B, 4))
{
    m_step = 0;
    m_cy_ptr = &CylinderTime::GetInstance();
}

CTimeTaskcy::~CTimeTaskcy()
{
}

void CTimeTaskcy::control(TASK_CY_CMD cmd, void *param)
{
    switch (cmd)
    {
    case TASK_CY_CMD::idle:
        m_step = 0xFF;
        break;
    case TASK_CY_CMD::start:
        m_step = 0;
        m_status = cmd;
        break;
    case TASK_CY_CMD::holdon:

        break;
    case TASK_CY_CMD::holdoff:

        break;

    case TASK_CY_CMD::warning:
        break;

    default:
        break;
    }
}

void CTimeTaskcy::Tick()
{
    switch (m_step)
    {
    case 0xFF: /*!< stop idle */
        CylinderTime::GetInstance().control(CylinderTimeCmd::RESET);
        break;
    case 0xFE: /*!< warning */

        break;
    case 0xFD: /*!< hold */
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
            m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->set();
        }
        break;
    case 2: /*!< ??????? */
        if (CYLINDER_STATUS::SET == m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->getStatus() &&
            m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->getActDiff() > 50)
        {
            m_step++;

            // CEfsmEvent event(EFSM_EVENT::NEXT);
            // CEfsmTaskCy::GetInstance().action(&event);
        }
        /*!< ??2**************************************** */
    case 3:
        m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][0]]->set();
        m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][1]]->set();

        break;
    default:
        break;
    }
}

void CTimeTaskcy::Update(const OHOS::Observable *o, const OHOS::ObserverArg *arg)
{

}
