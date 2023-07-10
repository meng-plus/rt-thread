/**
 * @file fm31xx_port.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief FM31xx接口文件
 * @version 0.1
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "fm31xx.h"
#include "rtdevice.h"

#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"
#define FM31XX_I2C_BUS "i2c0"
static struct rt_i2c_bus_device *Fm31XX_handle = NULL;

static rt_err_t fm31xx_read_reg(uint16_t dev_id, rt_uint16_t addr, uint8_t addr_len, rt_uint8_t *data, rt_uint8_t data_size)
{
    if (Fm31XX_handle == RT_NULL)
    {
        return -RT_ERROR;
    }
    struct rt_i2c_msg msg[2];
    msg[0].addr = dev_id;
    msg[0].flags = RT_I2C_WR | RT_I2C_NO_STOP;
    msg[0].len = addr_len;
    msg[0].buf = (rt_uint8_t *)&addr;
    if (addr_len == 2)
    {
        uint8_t tmp = msg[0].buf[0];
        msg[0].buf[0] = msg[0].buf[1];
        msg[0].buf[1] = tmp;
    }
    msg[1].addr = dev_id;
    msg[1].flags = RT_I2C_RD;
    msg[1].len = data_size;
    msg[1].buf = data;

    if (rt_i2c_transfer(Fm31XX_handle, msg, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        LOG_E("i2c bus read failed!\r\n");
        return -RT_ERROR;
    }
}

static rt_err_t fm31xx_write_reg(uint16_t dev_id, rt_uint16_t addr, uint8_t addr_len, rt_uint8_t *data, rt_uint8_t data_size)
{
    if (Fm31XX_handle == RT_NULL)
    {
        return -RT_ERROR;
    }
    struct rt_i2c_msg msg[2];
    msg[0].addr = dev_id;
    msg[0].flags = RT_I2C_WR | RT_I2C_NO_STOP;
    msg[0].len = addr_len;
    msg[0].buf = (rt_uint8_t *)&addr;
    if (addr_len == 2)
    {
        uint8_t tmp = msg[0].buf[0];
        msg[0].buf[0] = msg[0].buf[1];
        msg[0].buf[1] = tmp;
    }

    msg[1].addr = dev_id;
    msg[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msg[1].len = data_size;
    msg[1].buf = data;
    if (rt_i2c_transfer(Fm31XX_handle, msg, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        LOG_E("i2c bus write failed!\r\n");
        return -RT_ERROR;
    }
}

void fm31xx_hw_i2c_init()
{
    Fm31XX_handle = rt_i2c_bus_device_find(FM31XX_I2C_BUS);
    if (Fm31XX_handle == RT_NULL)
    {
        LOG_E("i2c bus device %s not found!\r\n", FM31XX_I2C_BUS);
        return;
    }
}
int E2PromWrite(uint16_t addr, void *p, uint16_t len)
{
    return fm31xx_write_reg(FM31XX_ADDR_MEM, addr, 2, p, len);
}
int E2PromRead(uint16_t addr, void *p, uint16_t len)
{
    return fm31xx_read_reg(FM31XX_ADDR_MEM, addr, 2, p, len);
}
int Fm31xxWriteReg(uint8_t reg, uint8_t *pdt, uint8_t len)
{
    return fm31xx_write_reg(FM31XX_ADDR_REG, reg, 1, pdt, len);
}

/**********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
***********************************************************************************/
int Fm31xxReadReg(uint8_t reg, uint8_t *pdt, uint8_t len)
{
    return fm31xx_read_reg(FM31XX_ADDR_REG, reg, 1, pdt, len);
}
