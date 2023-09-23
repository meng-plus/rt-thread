/**
 * @file product_def.h
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief 产品的专用定义
 * @version 0.1
 * @date 2023-09-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __PRODUCT_DEF_H_
#define __PRODUCT_DEF_H_
#include "main.h"

#define X_PORT(n) X##n##_GPIO_Port
#define X_PIN(n) X##n##_Pin

#define I_START_PORT X_PORT(0)
#define I_START_PIN X_PIN(0)

#define I_EN_PORT X_PORT(1)
#define I_ED_PIN X_PIN(1)

#define I_0A_PORT X_PORT(2)
#define I_0A_PIN X_PIN(2)
#define I_1A_PORT X_PORT(3)
#define I_1A_PIN X_PIN(3)

#define I_0B_PORT X_PORT(4)
#define I_0B_PIN X_PIN(4)
#define I_1B_PORT X_PORT(5)
#define I_1B_PIN X_PIN(5)

#define I_0C_PORT X_PORT(6)
#define I_0C_PIN X_PIN(6)
#define I_1C_PORT X_PORT(7)
#define I_1C_PIN X_PIN(7)

#define I_0D_PORT X_PORT(8)
#define I_0D_PIN X_PIN(8)
#define I_1D_PORT X_PORT(9)
#define I_1D_PIN X_PIN(9)

#define I_0E_PORT X_PORT(10)
#define I_0E_PIN X_PIN(10)
#define I_1E_PORT X_PORT(11)
#define I_1E_PIN X_PIN(11)

#define I_MOTOR_EN_PORT X_PORT(12)
#define I_MOTOR_EN_PIN X_PIN(12)

/** 输出Io重定义 */
#define Y_PORT(n) Y##n##_GPIO_Port
#define Y_PIN(n) Y##n##_Pin

#define O_F_PORT Y_PORT(0)
#define O_F_PIN Y_PIN(0)
#define O_G_PORT Y_PORT(1)
#define O_G_PIN Y_PIN(1)
#define O_A_PORT Y_PORT(2)
#define O_A_PIN Y_PIN(2)
#define O_B_PORT Y_PORT(3)
#define O_B_PIN Y_PIN(3)
#define O_C_PORT Y_PORT(4)
#define O_C_PIN Y_PIN(4)
#define O_D_PORT Y_PORT(5)
#define O_D_PIN Y_PIN(5)
#define O_E_PORT Y_PORT(6)
#define O_E_PIN Y_PIN(6)

#define O_MOTOR_WARNING_PORT Y_PORT(7)
#define O_MOTOR_WARNING_PIN Y_PIN(7)
#define O_ERROR_PORT Y_PORT(8)
#define O_ERROR_PIN Y_PIN(8)

#define O_LED_PORT LED_GPIO_Port
#define O_LED_PIN LED_Pin

typedef struct __GPIO
{
    GPIO_TypeDef *port;
    uint16_t pin;
} gpio_t;

#endif //__PRODUCT_DEF_H_
