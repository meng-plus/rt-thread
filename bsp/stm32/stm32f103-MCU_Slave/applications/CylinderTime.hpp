/**
 * @file CylinderTime.hpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "osTime.hpp"
#include "Cylinder.hpp"
#include "singleton.h"
enum class Cylinder_DEV
{
    A,
    B,
    C,
    D,
    E,
    NUM
};
class CylinderTime : public osTime, public OHOS::DelayedRefSingleton<CylinderTime>
{
public:
    CCylinder* m_Cylinder[(uint8_t)Cylinder_DEV::NUM];

protected:
    virtual void Tick();

public:
    CylinderTime(/* args */);
    virtual ~CylinderTime();
};
