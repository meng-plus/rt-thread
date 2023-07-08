/**
 * @file fm31XX.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "adxl345.h"
#include <rtthread.h>
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"
/*=============================================
==
===============================================*/

/** ADXL345 port */
void adxl345_hw_i2c_init();
uint8_t ADXL345_Read(uint8_t reg, uint8_t *pdt, uint8_t len);
uint8_t ADXL345_WriteReg(uint8_t reg, uint8_t dt);

int ADXL345Init()
{
    uint8_t devid;
    uint8_t res;

    adxl345_hw_i2c_init();

    res = ADXL345ReadReg(0x00, &devid);
    if (res != 0 || devid != 0xe5)
        return -1;

    ADXL345_WriteReg(0x2C, 0x08); // 速率设定为12.5 参考pdf13页
    ADXL345_WriteReg(0x2D, 0x08); // 选择电源模式   参考pdf24页	0000 1000 开启测量模式
    ADXL345_WriteReg(0x1D, 0x30); // 设置THRESH_TAP(敲击门限) 62.5mg/LSB   0xff=16g  此时试取3g
    ADXL345_WriteReg(0x21, 0x11); // 设置DUR（敲击持续时间） 625us/LSB  此时试取10ms
    ADXL345_WriteReg(0x2A, 0x07); // 设置各轴方向是否参加敲击检测  xyz全参与检测
    ADXL345_WriteReg(0x2E, 0x00); // 设置中断前应先禁用中断
    ADXL345_WriteReg(0x2F, 0x00); // 设置中断引脚映射 将单击时间映射到INT1上
    ADXL345_WriteReg(0x31, 0x00); // 使中断低电平有效
    ADXL345_WriteReg(0x2E, 0x40); // 使能 SINGLE_TAP 中断
    ADXL345_WriteReg(0x1E, 0x00); // X 偏移量 根据测试传感器的状态写入pdf29页
    ADXL345_WriteReg(0x1F, 0x00); // Y 偏移量 根据测试传感器的状态写入pdf29页
    ADXL345_WriteReg(0x20, 0x05); // Z 偏移量 根据测试传感器的状态写入pdf29页
    return 0;
}
INIT_DEVICE_EXPORT(ADXL345Init);
/**********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
***********************************************************************************/
uint8_t ADXL345GetTapState(void)
{
    uint8_t st;
    // TODO:待完善
    ADXL345ReadReg(0x30, &st);
    return st;
    // if (Chip_GPIO_GetPinState(LPC_GPIO, 2, 25) != 0)
    // {
    //     ADXL345ReadReg(0x30, &st);
    //     return 1;
    // }
    // else
    // {
    //     return 0;
    // }
}
uint8_t ADXL345ReadReg(uint8_t reg, uint8_t *dt)
{
    return ADXL345_Read(reg, dt, 1);
}
