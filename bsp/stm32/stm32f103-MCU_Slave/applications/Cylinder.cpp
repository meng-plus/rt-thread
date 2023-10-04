/**
 * @file Cylinder.cpp
 * @author ????plus (chengmeng_2@outlook.com)
 * @brief ???б┴?ид
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Cylinder.hpp"

CCylinder::CCylinder(const char *name, rt_base_t O, rt_base_t i0, rt_base_t i1)
    : m_name(name), m_O(O), m_i0Irq(i0), m_i1(i1)
{
    CGpioArg *argPtr = new CGpioArg();
    argPtr->name = m_name;
    argPtr->m_id = CGpioArg::ID::I0;
    m_i0Irq.arg = argPtr;
    setStatus(CYLINDER_STATUS::RESET);
    reset();
    m_status = CYLINDER_STATUS::RESET;
}
CCylinder::~CCylinder()
{
    if (m_i0Irq.arg)
        delete m_i0Irq.arg;
}
void CCylinder::AddObserver(const std::shared_ptr<OHOS::Observer> &o)
{
    m_i0Irq.AddObserver(o);
}
void CCylinder::IrqEnable(bool en)
{
    m_i0Irq.IrqEnable(en);
}
void CCylinder::set()
{
    m_O.set();
    setStatus(CYLINDER_STATUS::SET);
    m_timestamp = rt_tick_get();
}

void CCylinder::reset()
{
    m_O.reset();
    setStatus(CYLINDER_STATUS::RESET);
    m_timestamp = rt_tick_get();
}

rt_int8_t CCylinder::getOut()
{
    return m_O.read();
}

uint8_t CCylinder::read_i0()
{
    return m_i0Irq.read();
}

uint8_t CCylinder::read_i1()
{
    return m_i1.read();
}

CYLINDER_STATUS CCylinder::getStatus()
{
    return m_status;
}
CYLINDER_STATUS CCylinder::getStatus_target()
{
    return m_status_target;
}

void CCylinder::setStatus(CYLINDER_STATUS newStatus)
{
    m_status_target = newStatus;
}

rt_tick_t CCylinder::getActDiff()
{
    return rt_tick_get() - m_timestamp;
}
