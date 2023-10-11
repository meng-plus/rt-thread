/**
 * @file drv_pin.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <rtdevice.h>
#include "board.h"
#include "drv_pin.h"
#include "lpc_gpio.h"
#include "lpc_pinsel.h"
#include <ctype.h>
#include <stdlib.h>
/* Private typedef -----------------------------------------------------------*/
typedef struct drv_pin
{
    struct rt_pin_ops ops;
} drv_pin_t;

/* Private define ------------------------------------------------------------*/
#define PIN_NUM(port, no) (((((port)&0x1Fu) << 5) | ((no)&0x1Fu)))
#define PIN_PORT(pin) ((uint8_t)(((pin) >> 5) & 0x1Fu))
#define PIN_NO(pin) ((uint8_t)((pin)&0x1Fu))
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void _pin_mode(struct rt_device *device, rt_base_t pin, rt_uint8_t mode);
static void _pin_write(struct rt_device *device, rt_base_t pin, rt_uint8_t value);
static rt_int8_t _pin_read(struct rt_device *device, rt_base_t pin);
static rt_err_t _pin_attach_irq(struct rt_device *device, rt_base_t pin, rt_uint8_t mode, void (*hdr)(void *args), void *args);
static rt_err_t _pin_detach_irq(struct rt_device *device, rt_base_t pin);
static rt_err_t _pin_irq_enable(struct rt_device *device, rt_base_t pin, rt_uint8_t enabled);
static rt_base_t _pin_get(const char *name);
/* Private functions ---------------------------------------------------------*/
const drv_pin_t drv_pin_opt = {
    .ops = {
        .pin_mode = _pin_mode,
        .pin_write = _pin_write,
        .pin_read = _pin_read,
        .pin_attach_irq = _pin_attach_irq,
        .pin_detach_irq = _pin_detach_irq,
        .pin_irq_enable = _pin_irq_enable,
        .pin_get = _pin_get,
    }};
int rt_hw_pin_init()
{
    rt_device_pin_register("pin", &drv_pin_opt.ops, NULL);
    return 0;
}
INIT_BOARD_EXPORT(rt_hw_pin_init);

void _pin_mode(struct rt_device *device, rt_base_t pin, rt_uint8_t mode)
{
    PINSEL_ConfigPin(PIN_PORT(pin), PIN_NO(pin), 0); // 0: Select GPIO (Default)
    switch (mode)
    {
    case PIN_MODE_OUTPUT:
        GPIO_SetDir(PIN_PORT(pin), 0x01 << PIN_NO(pin), 1);
        break;
    case PIN_MODE_INPUT:
        GPIO_SetDir(PIN_PORT(pin), 0x01 << PIN_NO(pin), 0);
        break;
    case PIN_MODE_INPUT_PULLUP:
        GPIO_SetDir(PIN_PORT(pin), 0x01 << PIN_NO(pin), 0);
        PINSEL_SetPinMode(PIN_PORT(pin), PIN_NO(pin), PINSEL_BASICMODE_PULLUP);
        break;
    case PIN_MODE_INPUT_PULLDOWN:
        GPIO_SetDir(PIN_PORT(pin), 0x01 << PIN_NO(pin), 0);
        PINSEL_SetPinMode(PIN_PORT(pin), PIN_NO(pin), PINSEL_BASICMODE_PULLDOWN);
        break;
    case PIN_MODE_OUTPUT_OD:
        GPIO_SetDir(PIN_PORT(pin), 0x01 << PIN_NO(pin), 1);
        PINSEL_SetOpenDrainMode(PIN_PORT(pin), PIN_NO(pin), ENABLE);
        break;
    }
}
void _pin_write(struct rt_device *device, rt_base_t pin, rt_uint8_t value)
{
    if (value)
        GPIO_SetValue(PIN_PORT(pin), 0x01 << PIN_NO(pin));
    else
        GPIO_ClearValue(PIN_PORT(pin), 0x01 << PIN_NO(pin));
}
rt_int8_t _pin_read(struct rt_device *device, rt_base_t pin)
{
    return 0 != (GPIO_ReadValue(PIN_PORT(pin)) & (0x01 << PIN_NO(pin)));
}
rt_err_t _pin_attach_irq(struct rt_device *device, rt_base_t pin, rt_uint8_t mode, void (*hdr)(void *args), void *args)
{
    return 0;
}
rt_err_t _pin_detach_irq(struct rt_device *device, rt_base_t pin)
{
    return 0;
}
rt_err_t _pin_irq_enable(struct rt_device *device, rt_base_t pin, rt_uint8_t enabled)
{
    return 0;
}
rt_base_t _pin_get(const char *name)
{
    rt_base_t pin = 0;
    int hw_port_num, hw_pin_num = 0;
    int name_len;

    name_len = rt_strlen(name);

    if ((name_len < 4) || (name_len >= 6))
    {
        goto out;
    }
    if ((toupper(name[0]) != 'P') || (name[2] != '.'))
    {
        goto out;
    }

    if (isalpha(name[1]))
    {
        hw_port_num = (int)(toupper(name[1]) - 'A');
    }
    else if (isdigit(name[1]))
    {
        hw_port_num = (int)(name[1] - '0');
    }
    else
    {
        goto out;
    }

    for (uint8_t i = 3; i < name_len; i++)
    {
        if (!isdigit(name[i]))
        {
            return -RT_EINVAL;
        }
    }
    hw_pin_num = atoi(name + 3);
    if (hw_pin_num >= 32)
    {
        return -RT_EINVAL;
    }
    pin = PIN_NUM(hw_port_num, hw_pin_num);

    return pin;
out:
    rt_kprintf("Px.y  x:A~Z  y:0-32, e.g. PA.0\n");
    return -RT_EINVAL;
}
