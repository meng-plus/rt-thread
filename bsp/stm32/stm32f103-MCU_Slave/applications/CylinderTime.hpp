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
class CylinderTime : public osTime, public OHOS::DelayedRefSingleton<CylinderTime>
{
private:
    CCylinder A;
    CCylinder B;
    CCylinder C;
    CCylinder D;
    CCylinder E;

protected:
    virtual void Tick();

public:
    CylinderTime(/* args */);
    virtual ~CylinderTime();
};
