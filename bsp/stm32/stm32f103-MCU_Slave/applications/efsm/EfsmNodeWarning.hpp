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

class CEfsmNodeStep1 : public CEfsmNode
{
private:
    /* data */
public:
    CEfsmNodeStep1(/* args */) = default;
    virtual ~CEfsmNodeStep1() = default;
    virtual void init(){

    };
    virtual void exit(){};
    virtual void action(const CEfsmManage *parent, const CEfsmEvent *event_ptr){

    };
    virtual void tick(){

    };
};
