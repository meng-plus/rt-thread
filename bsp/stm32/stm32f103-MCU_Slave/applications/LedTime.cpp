/**
 * @file CLedTime.cpp
 * @author §è§Ú§è§Úplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "LedTime.hpp"

#include "product_def.h"
CLedTime::CLedTime(/* args */)
    : osTime(__FILE__, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER, NULL, 500),
      m_run(GET_PIN(A, 3)),
      m_warning(GET_PIN(C, 13)),
      m_err(GET_PIN(B, 9))
{
    start();
}

CLedTime::~CLedTime()
{
}
void CLedTime::Tick()
{
    m_run.Toggle();
}
