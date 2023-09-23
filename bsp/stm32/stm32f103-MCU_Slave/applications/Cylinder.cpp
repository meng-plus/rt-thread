/**
 * @file Cylinder.cpp
 * @author ÃÉÃÉplus (chengmeng_2@outlook.com)
 * @brief Æû¸×Àà
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Cylinder.hpp"

CCylinder::CCylinder(rt_base_t O, rt_base_t i0, rt_base_t i1)
    : m_O(O), m_i0(i0), m_i1(i1)
{
    rt_pin_mode(m_O, PIN_MODE_OUTPUT);
    rt_pin_mode(m_i0, PIN_MODE_INPUT);
    rt_pin_mode(m_i1, PIN_MODE_INPUT);
}

void CCylinder::set()
{
    rt_pin_write(m_O, 1);
}

void CCylinder::reset()
{
    rt_pin_write(m_O, 0);
}

rt_int8_t CCylinder::getOut()
{
    return rt_pin_read(m_O);
}

CCylinder::~CCylinder()
{
    rt_pin_mode(m_O, PIN_MODE_OUTPUT);
    rt_pin_mode(m_i0, PIN_MODE_INPUT);
    rt_pin_mode(m_i1, PIN_MODE_INPUT);
}
