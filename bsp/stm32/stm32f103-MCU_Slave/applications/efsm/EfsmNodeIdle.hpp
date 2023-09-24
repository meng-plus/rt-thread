/**
 * @file EfsmNodeIdle.hpp
 * @author ÃÉÃÉplus (chengmeng_2@outlook.com)
 * @brief ×´Ì¬»úµÄ¿ÕÏÐ×´Ì¬
 * @version 0.1
 * @date 2023-09-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "efsm.hpp"
#include "CylinderTime.hpp"
class CEfsmNodeIdle : public CEfsmNode
{
private:
    /* data */
public:
    CEfsmNodeIdle(/* args */) = default;
    virtual ~CEfsmNodeIdle() = default;
    virtual void init()
    {
        CylinderTime::GetInstance().control(CylinderTimeCmd::RESET);
    };
    virtual void exit(){};
    virtual void action(const CEfsmManage *parent, const CEfsmEvent *event_ptr){

    };
    virtual void tick(){

    };
};
