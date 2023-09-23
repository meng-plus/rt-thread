/**
 * @file Cylinder.hpp
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief 气缸基类
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "product_def.h"
#include <drv_gpio.h>

enum class CYLINDER_STATUS
{
    RESET,
    SETING,
    SET,
    RESETING,
};
class CCylinder
{
public:
    rt_base_t m_O;  /*!< 气缸输出 1 有信号*/
    rt_base_t m_i0; /*!< 位中信号 0 有信号*/
    rt_base_t m_i1; /*!< 位末信号 */
    CYLINDER_STATUS m_status;

public:
    CCylinder(rt_base_t O, rt_base_t i0, rt_base_t i1);
    void set();
    void reset();
    rt_int8_t getOut();
    CYLINDER_STATUS getStatus();
    ~CCylinder();
};
