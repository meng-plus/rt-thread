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
#include "Gpio.hpp"
enum class CYLINDER_STATUS
{
    RESET, /*!< 复位 */
    SETING,
    SET, /*!< 置位 */
    RESETING,
    MID_NO,
    MID,   /*!< 中间位置 */
    ERROR, /*!< 超时动作 */
};

class CCylinder
{
public:
    const char *m_name;

    CGpioIrq m_i0Irq;                /*!< 位中信号 0 有信号*/
    CGpioX m_i1;                     /*!< 位末信号 */
    CGpioY m_O;                      /*!< 气缸输出 1 有信号*/
    CYLINDER_STATUS m_status;        /*!< 当前动作 */
    CYLINDER_STATUS m_status_target; /*!< 目标动作 */
    rt_tick_t m_timestamp;

public:
    CCylinder(const char *name, rt_base_t O, rt_base_t i0, rt_base_t i1);
    /**
     * @brief Adds the specified observer to the set of observers.
     *
     * If `o` is valid and does not exist in the observer set, the observer
     * will be added; otherwise, this function will return directly.
     */
    void AddObserver(const std::shared_ptr<OHOS::Observer> &o);
    void IrqEnable(bool en);

    void set();
    void reset();
    rt_int8_t getOut();
    uint8_t read_i0();
    uint8_t read_i1();
    CYLINDER_STATUS getStatus();        /*!< 当前状态 */
    CYLINDER_STATUS getStatus_target(); /*!< 目标动作 */
    void setStatus(CYLINDER_STATUS newStatus);
    /*!< 获得上次活动的差值 */
    rt_tick_t getActDiff();
    ~CCylinder();
};
