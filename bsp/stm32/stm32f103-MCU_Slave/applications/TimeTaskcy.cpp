/**
 * @file TimeTaskcy.cpp
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "TimeTaskcy.hpp"
#include "Motor.hpp"
#define LOG_TAG "task.cy"
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"
static const uint8_t cyMapStep1[] = {0, 1, 2, 3};
static const uint8_t cyMapStep2[][2] = {{1, 3}, {0, 2}, {1, 3}, {0, 2}};
static const uint8_t cyMapStep3[] = {2, 3, 0, 1};
static const uint8_t cyMapStep4[] = {4, 4, 4, 4};
CylinderTime *g_cy_ptr = NULL;
CTimeTaskcy::CTimeTaskcy(/* args */)
    : osTime("Taskcy", RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER, NULL, 200),
      x0_start(GET_PIN(A, 0)),
      x1_En(GET_PIN(B, 10)),
      x13_dir1(GET_PIN(A, 15)),
      x14_dir2(GET_PIN(B, 4)),
      Y0_OF(GET_PIN(B, 2))
{
    m_step = 0;
    m_cy_ptr = &CylinderTime::GetInstance();
    g_cy_ptr = &CylinderTime::GetInstance();
    start();
}

CTimeTaskcy::~CTimeTaskcy()
{
}

void CTimeTaskcy::control(TASK_CY_CMD cmd, void *param)
{
    switch (cmd)
    {
    case TASK_CY_CMD::stop:
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
    if (x1_En.read() == 0)
    {
        m_step = 0;
    }
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
            m_dir = 0;
            m_dir |= x13_dir1.read() ? 0x02 : 0x00;
            m_dir |= x14_dir2.read() ? 0x01 : 0x00;
            m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->set();
            const float volmap[] = {0.265, 1.115, 1.95, 2.84};
            LOG_D("vol:%d", (int)(volmap[m_dir] * 100));
            CMotor::GetInstance().setVoltageTarget(volmap[m_dir]);
        }
        break;
    case 2: /*!< 等待第一步就位 */
        if (CYLINDER_STATUS::SET == m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->getStatus() &&
            m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->getActDiff() > 50)
        {
            m_step++;
        }
        break;
        /*!< 步骤2**************************************** */
    case 3:
        m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][0]]->set();
        m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][1]]->set();
        m_step++;
        break;
    case 4: /*!< 等待 CylinderTime 通知就位*/
        /*!< 立刻断开第一步的输出 */
        break;
    case 5: /*!< 第一步复位 */
        if (CYLINDER_STATUS::RESET == m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->getStatus())
        {
            m_step++;
        }
        break;
        /*!< 步骤3 *************************************** */
    case 6: /*!< 输出第三步 */
        if ((CYLINDER_STATUS::SET == m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][0]]->getStatus()) &&
            (CYLINDER_STATUS::SET == m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][1]]->getStatus()))
        {
            m_cy_ptr->m_Cylinder[cyMapStep3[m_dir]]->set();
            m_step++;
        }
        break;
    case 7: /*!< 第二步撤回 */

        break;
    case 8: /*!< 动作就绪 */
        if ((CYLINDER_STATUS::SET == m_cy_ptr->m_Cylinder[cyMapStep3[m_dir]]->getStatus()) &&
            (CYLINDER_STATUS::RESET == m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][0]]->getStatus()) &&
            (CYLINDER_STATUS::RESET == m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][1]]->getStatus()))
        {
            m_step++;
        }
        break;
    /*!< 步骤4 ************************************** */
    case 9: /*!< 第四步 输出 */
        m_cy_ptr->m_Cylinder[cyMapStep4[m_dir]]->set();
        m_step++;
        break;
    case 10: /*!< 收回第三步 第四步 */
        break;
    case 12: /*!< 完成 */
        m_step = 0;
        break;
    default:
        break;
    }
    static uint8_t m_step_last;
    if (m_step_last != m_step)
    {
        LOG_D("[%d]step change %d->%d", rt_tick_get(), m_step_last, m_step);
        m_step_last = m_step;
    }
}

void CTimeTaskcy::Update(const OHOS::Observable *o, const OHOS::ObserverArg *arg)
{
    if (o == m_cy_ptr)
    { /*!< 气缸的事件更新通知 */
        const ObsertverCyVal *cyVal_ptr = static_cast<const ObsertverCyVal *>(arg);
        if (m_step == 4)
        {
            if (((cyVal_ptr->status == CYLINDER_STATUS::MID) ||
                 (cyVal_ptr->status == CYLINDER_STATUS::SET) ||
                 (cyVal_ptr->status == CYLINDER_STATUS::SETING)) &&
                ((cyVal_ptr->Cylinder == m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][0]]) ||
                 (cyVal_ptr->Cylinder == m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][1]])))
            {
                m_cy_ptr->m_Cylinder[cyMapStep1[m_dir]]->reset();
                m_step++;
            }
        }
        else if (m_step == 7)
        {
            if (((cyVal_ptr->status == CYLINDER_STATUS::MID) ||
                 (cyVal_ptr->status == CYLINDER_STATUS::SET) ||
                 (cyVal_ptr->status == CYLINDER_STATUS::SETING)) &&
                ((cyVal_ptr->Cylinder == m_cy_ptr->m_Cylinder[cyMapStep3[m_dir]])))
            {
                m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][0]]->reset();
                m_cy_ptr->m_Cylinder[cyMapStep2[m_dir][1]]->reset();
                m_step++;
            }
        }
        else if (m_step == 10)
        {
            const ObsertverCyVal *cyVal_ptr = static_cast<const ObsertverCyVal *>(arg);
            if (((cyVal_ptr->status == CYLINDER_STATUS::MID) ||
                 (cyVal_ptr->status == CYLINDER_STATUS::SET) ||
                 (cyVal_ptr->status == CYLINDER_STATUS::SETING)) &&
                ((cyVal_ptr->Cylinder == m_cy_ptr->m_Cylinder[cyMapStep4[m_dir]])))
            {
                m_cy_ptr->m_Cylinder[cyMapStep3[m_dir]]->reset();
                m_step++;
            }
        }
        else if (m_step == 11)
        {
            if ((cyVal_ptr->status == CYLINDER_STATUS::SET) &&
                (cyVal_ptr->Cylinder == m_cy_ptr->m_Cylinder[cyMapStep4[m_dir]]))
            {
                m_cy_ptr->m_Cylinder[cyMapStep4[m_dir]]->reset();
                m_step++;
            }
        }
    }
}
