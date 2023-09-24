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
    : m_name(name), m_O(O), m_i0(i0), m_i1(i1)
{
    rt_pin_mode(m_O, PIN_MODE_OUTPUT);
    rt_pin_mode(m_i0, PIN_MODE_INPUT);
    rt_pin_mode(m_i1, PIN_MODE_INPUT);
    setStatus(CYLINDER_STATUS::RESET);
}

void CCylinder::set()
{
    rt_pin_write(m_O, 1);
    setStatus(CYLINDER_STATUS::SETING);
}

void CCylinder::reset()
{
    rt_pin_write(m_O, 0);
    setStatus(CYLINDER_STATUS::RESETING);
}

rt_int8_t CCylinder::getOut()
{
    return rt_pin_read(m_O) == 0;
}

uint8_t CCylinder::read_i0()
{
    return rt_pin_read(m_i0) == 0;
}

uint8_t CCylinder::read_i1()
{
    return rt_pin_read(m_i1) == 0;
}

CYLINDER_STATUS CCylinder::getStatus()
{
    return m_status;
}

CCylinder::~CCylinder()
{
    rt_pin_mode(m_O, PIN_MODE_INPUT);
    rt_pin_mode(m_i0, PIN_MODE_INPUT);
    rt_pin_mode(m_i1, PIN_MODE_INPUT);
}

void CCylinder::setStatus(CYLINDER_STATUS newStatus)
{
    m_timestamp = rt_tick_get();
    m_status = newStatus;
}

rt_tick_t CCylinder::getActDiff()
{
    return rt_tick_get() - m_timestamp;
}
