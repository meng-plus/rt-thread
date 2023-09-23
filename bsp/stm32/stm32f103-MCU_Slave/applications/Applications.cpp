/**
 * @file Applications.cpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Applications.hpp"
#include "LedTime.hpp"
#include "CylinderTime.hpp"
CApplications::CApplications()
    : osThread(__FILE__)
{
    CLedTime::GetInstance();
    CylinderTime::GetInstance();
}

CApplications::~CApplications()
{
}

void CApplications::thread()
{
    while (1)
    {

        rt_thread_delay(1000);
    }
}
