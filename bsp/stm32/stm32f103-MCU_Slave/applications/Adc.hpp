/**
 * @file Adc.hpp
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief 模拟采集
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "rtdevice.h"
#include "singleton.h"
class CAdc : public OHOS::DelayedRefSingleton<CAdc>
{
private:
    rt_adc_device_t adc_dev; /* ADC 设备句柄 */
    uint8_t m_chn;

public:
    CAdc(const char *name = "adc1");
    virtual ~CAdc();
    void selectChn(uint8_t chn);
    void enable();
    void disable();
    uint16_t readraw();
    float readVoltage();
};
