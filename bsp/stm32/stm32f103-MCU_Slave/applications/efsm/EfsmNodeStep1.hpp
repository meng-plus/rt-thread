/**
 * @file EfsmNodeStep1.hpp
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
#include "Gpio.hpp"
#include "CylinderTime.hpp"
class CEfsmNodeStep1 : public CEfsmNode
{
private:

    uint8_t m_dir;
    uint8_t m_step;
    CGpioX x0_start;
    CGpioX x13_dir1;
    CGpioX x14_dir2;
    CylinderTime* m_cy_ptr ;
    public : CEfsmNodeStep1(/* args */);
    virtual ~CEfsmNodeStep1() = default;
    virtual void init();
    virtual void exit();
    virtual void action(const CEfsmManage *parent, const CEfsmEvent *event_ptr);
    virtual void tick();
};
