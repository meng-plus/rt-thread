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
#include "CAdc.hpp"

CAdc::CAdc(const char *name)
{ /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(name);
    selectChn(1);
    enable();
}

void CAdc::selectChn(uint8_t chn)
{
    m_chn = chn;
}

void CAdc::enable()
{ /* 使能设备 */
    rt_adc_enable(adc_dev, m_chn);
}

void CAdc::disable()
{
    rt_adc_disable(adc_dev, m_chn);
}

uint16_t CAdc::readraw()
{ /* 读取采样值 */
    enable();
    return rt_adc_read(adc_dev, m_chn);
}

float CAdc::readVoltage()
{
    return readraw() * 1.0 / 0xFFF * 3.3;
}

CAdc::~CAdc()
{
}
