/**
 * @file efsm.hpp
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 事件驱动型有限状态机：Event Finite State Machine
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#include <stdint.h>
#include "vector"
#define EFSM_SAFE_RUN(ptr, fun) \
    if (ptr && (ptr->fun))      \
    {                           \
        ptr->fun();             \
    }
/**
 * @brief 可变参数的安全运行
 *
 */
#define EFSM_SAFE_RUN_ARGV(ptr, fun, ...) \
    if (ptr && (ptr->fun))                \
    {                                     \
        ptr->fun(__VA_ARGS__);            \
    }

/**
 * @brief 系统级别支持的事件,用户自定义请在EFSM_EVENT_USER后续扩展
 *
 */
enum class EFSM_EVENT
{
    IDLE,
    TICK,
    START,
    STOP,
    PREV,
    NEXT,
    WARNING,
    ERROR,
    VALUE,
    USER = 0x80,
};
class CEfsmManage;
class CEfsmEvent
{
public:
    EFSM_EVENT m_event;

public:
    CEfsmEvent(EFSM_EVENT event)
        : m_event(event){};
    virtual ~CEfsmEvent() = default;
};
class CEfsmNode
{
public:
    CEfsmNode(/* args */) = default;
    virtual ~CEfsmNode() = default;
    virtual void init() = 0;
    virtual void exit() = 0;
    virtual void action(const CEfsmManage *parent, const CEfsmEvent *event_ptr) = 0;
    virtual void tick() = 0;
};

class CEfsmManage
{
protected:
    std::vector<CEfsmNode *> m_nodeArr;
    uint8_t m_idx;

protected:
    void transfer(CEfsmNode *curr, CEfsmNode *next);

public:
    CEfsmManage(/* args */);
    virtual ~CEfsmManage();
    virtual void action(const CEfsmEvent *Event_ptr) = 0;
};
