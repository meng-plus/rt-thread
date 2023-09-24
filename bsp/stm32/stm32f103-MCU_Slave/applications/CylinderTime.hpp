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
#include "osThread.hpp"
#include "Cylinder.hpp"
#include "singleton.h"
#include "observer.h"
enum class Cylinder_DEV
{
    A,
    B,
    C,
    D,
    E,
    NUM
};

class ObsertverCyVal : public OHOS::ObserverArg
{
public:
    CCylinder *Cylinder;
    CYLINDER_STATUS status;
    rt_tick_t timeout;

public:
    ObsertverCyVal(CCylinder *cy, CYLINDER_STATUS st, rt_tick_t diff)
        : Cylinder(cy), status(st), timeout(diff)
    {
    }
    virtual ~ObsertverCyVal() = default;
};
enum class CylinderTimeCmd
{
    RESET,
    STOP,
};

class CylinderTime : public osThread,
                     public OHOS::DelayedRefSingleton<CylinderTime>,
                     public OHOS::Observable
{
public:
    CCylinder *m_Cylinder[(uint8_t)Cylinder_DEV::NUM];

protected:
    virtual void Tick();
protected:
    virtual void thread();
public:
    CylinderTime(/* args */);
    virtual ~CylinderTime();
    void control(CylinderTimeCmd cmd, void *param = NULL);
};
