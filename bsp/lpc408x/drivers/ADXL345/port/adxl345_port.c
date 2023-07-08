/**
 * @file adxl345_port.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief FM31xx接口文件
 * @version 0.1
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "adxl345.h"
#include "rtdevice.h"

#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"
#define ADXL345_I2C_BUS "i2c0"
#define ADXL345_I2C_ADDR 0x1D //(0x3a)

static struct rt_i2c_bus_device *adxl345_handle = NULL;

static rt_err_t adxl345_read_reg(uint16_t dev_id, rt_uint16_t addr, uint8_t addr_len, rt_uint8_t *data, rt_uint8_t data_size)
{
    if (adxl345_handle == RT_NULL)
    {
        return -RT_ERROR;
    }
    struct rt_i2c_msg msg[2];
    msg[0].addr = dev_id;
    msg[0].flags = RT_I2C_WR;
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

    if (rt_i2c_transfer(adxl345_handle, msg, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        LOG_E("i2c bus read failed!\r\n");
        return -RT_ERROR;
    }
}

int adxl345_write_reg(uint8_t reg, uint8_t *pdt, uint8_t len)
{
    uint8_t dev_id = ADXL345_I2C_ADDR;
    uint8_t buff[16];
    uint16_t addr = reg;
    if (adxl345_handle == RT_NULL)
    {
        return -RT_ERROR;
    }
    struct rt_i2c_msg msg[1];
    msg[0].addr = dev_id;
    msg[0].flags = RT_I2C_WR;
    msg[0].len = len + 1;
    msg[0].buf = (rt_uint8_t *)buff;
    buff[0] = addr;
    rt_memcpy(buff + 1, pdt, len);

    if (rt_i2c_transfer(adxl345_handle, msg, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        LOG_E("i2c bus write failed!\r\n");
        return -RT_ERROR;
    }
}

void adxl345_hw_i2c_init()
{
    adxl345_handle = rt_i2c_bus_device_find(ADXL345_I2C_BUS);
    if (adxl345_handle == RT_NULL)
    {
        LOG_E("i2c bus device %s not found!\r\n", ADXL345_I2C_BUS);
        return;
    }
}

uint8_t ADXL345_Read(uint8_t reg, uint8_t *pdt, uint8_t len)
{
    return adxl345_read_reg(ADXL345_I2C_ADDR, reg, 1, pdt, len);
}

uint8_t ADXL345_WriteReg(uint8_t reg, uint8_t dt)
{
    return adxl345_write_reg(reg, &dt, 1);
}
