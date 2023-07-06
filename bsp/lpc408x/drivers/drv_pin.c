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
#include <rtthread.h>
#include "board.h"
#include "drv_pin.h"
#include "lpc_gpio.h"
#include "lpc_pinsel.h"
/* Private typedef -----------------------------------------------------------*/
typedef struct pin_ctrl
{
    uint8_t port;
    uint8_t pin;
    char *name;
} pin_ctrl_t;
typedef struct drv_pin
{
    struct rt_pin_ops ops;
    pin_ctrl_t ctrl[PIN_NUM];
} drv_pin_t;

/* Private define ------------------------------------------------------------*/
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
const drv_pin_t drv_pin_opt = {.ops = {
                                   .pin_mode = _pin_mode,
                                   .pin_write = _pin_write,
                                   .pin_read = _pin_read,
                                   .pin_attach_irq = _pin_attach_irq,
                                   .pin_detach_irq = _pin_detach_irq,
                                   .pin_irq_enable = _pin_irq_enable,
                                   .pin_get = _pin_get,
                               },
                               .ctrl = {
                                   [PIN_LED_0] = {4, 26, "P4.26"}, /*!< P4.26 */
                                   [PIN_LED_1] = {4, 27, "P4.27"}, /*!< P4.27 */
                                   [PIN_LED_2] = {4, 29, "P4.29"}, /*!< P4.29 */
                                   [PIN_LED_3] = {4, 30, "P4.30"}, /*!< P4.30 */
                               }};
int rt_hw_pin_init()
{
    rt_device_pin_register(DEV_PIN_NAME, &drv_pin_opt.ops, NULL);
    rt_pin_mode(PIN_LED_0, PIN_MODE_OUTPUT_OD);
    rt_pin_write(PIN_LED_0, 0);
    return 0;
}
INIT_BOARD_EXPORT(rt_hw_pin_init);

void _pin_mode(struct rt_device *device, rt_base_t pin, rt_uint8_t mode)
{
    RT_ASSERT(pin < PIN_NUM);
    const pin_ctrl_t *pin_ctrl_ptr = &drv_pin_opt.ctrl[pin];
    PINSEL_ConfigPin(pin_ctrl_ptr->port, pin_ctrl_ptr->pin, 0); // 0: Select GPIO (Default)
    switch (mode)
    {
    case PIN_MODE_OUTPUT:
        GPIO_SetDir(pin_ctrl_ptr->port, 0x01 << pin_ctrl_ptr->pin, 1);
        break;
    case PIN_MODE_INPUT:
        GPIO_SetDir(pin_ctrl_ptr->port, 0x01 << pin_ctrl_ptr->pin, 0);
        break;
    case PIN_MODE_INPUT_PULLUP:
        GPIO_SetDir(pin_ctrl_ptr->port, 0x01 << pin_ctrl_ptr->pin, 0);
        PINSEL_SetPinMode(pin_ctrl_ptr->port, pin_ctrl_ptr->pin, PINSEL_BASICMODE_PULLUP);
        break;
    case PIN_MODE_INPUT_PULLDOWN:
        GPIO_SetDir(pin_ctrl_ptr->port, 0x01 << pin_ctrl_ptr->pin, 0);
        PINSEL_SetPinMode(pin_ctrl_ptr->port, pin_ctrl_ptr->pin, PINSEL_BASICMODE_PULLDOWN);
        break;
    case PIN_MODE_OUTPUT_OD:
        GPIO_SetDir(pin_ctrl_ptr->port, 0x01 << pin_ctrl_ptr->pin, 1);
        PINSEL_SetOpenDrainMode(pin_ctrl_ptr->port,pin_ctrl_ptr->pin, ENABLE);
        break;
    }
}
void _pin_write(struct rt_device *device, rt_base_t pin, rt_uint8_t value)
{
    RT_ASSERT(pin < PIN_NUM);
    const pin_ctrl_t *pin_ctrl_ptr = &drv_pin_opt.ctrl[pin];
    if (!value)
        GPIO_SetValue(pin_ctrl_ptr->port, 0x01 << pin_ctrl_ptr->pin);
    else
        GPIO_ClearValue(pin_ctrl_ptr->port, 0x01 << pin_ctrl_ptr->pin);
}
rt_int8_t _pin_read(struct rt_device *device, rt_base_t pin)
{
    RT_ASSERT(pin < PIN_NUM);
    const pin_ctrl_t *pin_ctrl_ptr = &drv_pin_opt.ctrl[pin];
    return GPIO_ReadValue(pin_ctrl_ptr->port) == (0x01 << pin_ctrl_ptr->pin);
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
    for (size_t i = 0; i < PIN_NUM; i++)
    {
        if (0 == rt_strcmp(name, drv_pin_opt.ctrl[i].name))
        {
            return i;
        }
    }

    return -1;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
void ls_pin(rt_uint32_t led_num, rt_uint32_t value)
{
    for (size_t i = 0; i < PIN_NUM; i++)
    {
        if (i <= LED_END)
            rt_kprintf(" LED:P%d.%d \r\n", drv_pin_opt.ctrl[i].port, drv_pin_opt.ctrl[i].pin);
        else
            rt_kprintf(" PIN:P%d.%d \r\n", drv_pin_opt.ctrl[i].port, drv_pin_opt.ctrl[i].pin);
    }
}
MSH_CMD_EXPORT(ls_pin, list pin)
#endif
