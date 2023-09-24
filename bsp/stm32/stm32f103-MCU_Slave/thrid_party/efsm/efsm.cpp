/**
 * @file efsm.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 事件驱动型有限状态机：Event Finite State Machine
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "efsm.hpp"

CEfsmManage::CEfsmManage(/* args */)
{
}

CEfsmManage::~CEfsmManage()
{
}

void CEfsmManage::transfer(CEfsmNode *curr, CEfsmNode *next)
{
    if (next)
    {
        if (curr)
        {
            curr->exit();
        }
        next->init();
    }
}
