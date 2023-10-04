/**
 * @file Applications.hpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief ≥Ã–Ú»Îø⁄
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "singleton.h"
#include "osThread.hpp"

#include "TimeTaskcy.hpp"
#include "Motor.hpp"
using namespace OHOS;
class CApplications :
    // public osThread,
    public OHOS::DelayedRefSingleton<CApplications>
{
protected:
    CTimeTaskcy *m_taskCy_ptr;
    CMotor *m_motorPtr;

public:
    CApplications();
    virtual ~CApplications();

protected:
    virtual void thread();
};
