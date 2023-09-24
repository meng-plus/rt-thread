/**
 * @file CEfsmTaskCy.hpp
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "efsm.hpp"
#include "singleton.h"

class CEfsmTaskCy : public CEfsmManage, public OHOS::DelayedRefSingleton<CEfsmTaskCy>
{
private:
    uint8_t m_dir; /*!<[0,3] 多路状态机的执行条件 */
    uint8_t m_dir_MAX = 4;

public:
    CEfsmTaskCy(/* args */);
    virtual ~CEfsmTaskCy();
    virtual void action(const CEfsmEvent *Event_ptr);
    uint8_t getDir();
    void setDir(uint8_t dir);
};
