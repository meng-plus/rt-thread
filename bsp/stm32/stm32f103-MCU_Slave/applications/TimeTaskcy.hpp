/**
 * @file TimeTaskcy.hpp
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief 气缸控制运行任务
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#include <rtthread.h>
#include "osTime.hpp"
#include "observer.h"
#include "singleton.h"
#include "Gpio.hpp"
#include "CylinderTime.hpp"
enum class TASK_CY_CMD
{
    idle,
    start,
    holdon,
    holdoff,
    warning,
};

class CTimeTaskcy : public osTime,
                    public OHOS::DelayedRefSingleton<CTimeTaskcy>,
                    public OHOS::Observer

{
private:
    uint8_t m_dir; // 方向检测结果
    uint8_t m_step;
    CGpioX x0_start;
    CGpioX x13_dir1;
    CGpioX x14_dir2;
    CylinderTime *m_cy_ptr;
    TASK_CY_CMD m_status;

private:
    virtual void Tick();
    virtual void Update(const OHOS::Observable *o, const OHOS::ObserverArg *arg);

public:
    CTimeTaskcy(/* args */);
    virtual ~CTimeTaskcy();
    void control(TASK_CY_CMD cmd, void *param = NULL);
};
