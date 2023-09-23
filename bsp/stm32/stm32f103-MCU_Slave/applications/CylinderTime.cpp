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
    : osTime(__FILE__, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER, NULL, 1),
      A(GET_PIN(B, 0), GET_PIN(B, 11), GET_PIN(B, 12)),
      B(GET_PIN(A, 4), GET_PIN(B, 13), GET_PIN(B, 14)),
      C(GET_PIN(A, 5), GET_PIN(B, 15), GET_PIN(B, 3)),
      D(GET_PIN(A, 6), GET_PIN(A, 8), GET_PIN(A, 9)),
      E(GET_PIN(A, 7), GET_PIN(A, 10), GET_PIN(A, 11))
{
    start();
}

CylinderTime::~CylinderTime()
{
}
void CylinderTime::Tick()
{

}
