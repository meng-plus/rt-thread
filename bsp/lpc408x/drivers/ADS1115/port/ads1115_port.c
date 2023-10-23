
#include "ads1115_def.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "rtdevice.h"
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"

typedef struct ads1115_user_data
{
    struct rt_i2c_bus_device *hi2c;
} ads1115_user_data_t;
static ads1115_user_data_t userData;

////模拟I2C口线定义
// static TypeI2CSIMPORT commPort={
//                                          {GPIOB,GPIO_PIN_10},
//                                          {GPIOB,GPIO_PIN_11},
//	                                };
/**
 * I2C write data then read data
 */
static ads1115_err read(const struct __ads1115_i2c *i2c, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    ads1115_err result = ADS1115_ERR_NOT_FOUND;
    // uint8_t send_data, read_data;
    ads1115_user_data_t *user_data = i2c->user_data;
    /**
     * add your i2c write and read code
     */
    if (user_data != NULL)
    {

        result = ADS1115_SUCCESS;
        struct rt_i2c_msg msg[2];
        msg[0].addr = i2c->addr;
        msg[0].flags = RT_I2C_WR;
        msg[0].len = 1;
        msg[0].buf = (rt_uint8_t *)&MemAddress;
        msg[1].addr = i2c->addr;
        msg[1].flags = RT_I2C_RD;
        msg[1].len = Size;
        msg[1].buf = pData;

        if (rt_i2c_transfer(user_data->hi2c, msg, 2) == 2)
        {
            result = ADS1115_SUCCESS;
        }
        else
        {
            LOG_E("i2c bus read failed!\r\n");
            result = ADS1115_ERR_TIMEOUT;
        }
    }

    return result;
}
static ads1115_err write(const struct __ads1115_i2c *i2c, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
    ads1115_err result = ADS1115_ERR_NOT_FOUND;
    // uint8_t send_data, read_data;
    ads1115_user_data_t *user_data = i2c->user_data;
    /**
     * add your i2c write and read code
     */
    if (user_data != NULL)
    {
        struct rt_i2c_msg msg[1];
        uint8_t buff[16];
        msg[0].addr = i2c->addr;
        msg[0].flags = RT_I2C_WR;
        msg[0].len = Size + 1;
        msg[0].buf = (rt_uint8_t *)buff;
        buff[0] = MemAddress;
        rt_memcpy(buff + 1, pData, Size);

        if (rt_i2c_transfer(user_data->hi2c, msg, 1) == 1)
        {
            result = ADS1115_SUCCESS;
        }
        else
        {
            LOG_E("i2c bus write failed!\r\n");
            result = ADS1115_ERR_TIMEOUT;
        }
    }
    return result;
}

ads1115_err ads1115_i2c_port_init(ads1115_i2c_t ads1115)
{
    ads1115_err result = ADS1115_ERR_NOT_FOUND;
    if (strcmp(ads1115->name, "i2c2") == 0)
    {
        userData.hi2c = rt_i2c_bus_device_find(ads1115->name);
        ads1115->write = write; // Required
        ads1115->read = read;
        ads1115->lock = NULL;
        ads1115->unlock = NULL;
        ads1115->user_data = &userData;
        result = ADS1115_SUCCESS;
    }
    return result;
}
int ads1115_init();
INIT_DEVICE_EXPORT(ads1115_init);
/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void ads1115_log_debug(const char *file, const long line, const char *format, ...)
{
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[ADS1115](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void ads1115_log_info(const char *format, ...)
{
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    printf("[ADS1115]");
    /* must use vprintf to print */
    vprintf(format, args);
    printf("\n");
    va_end(args);
}
