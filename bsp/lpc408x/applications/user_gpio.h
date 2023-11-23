/**
 * @file task_gpio.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 通用的输入输出
 * @version 0.1
 * @date 2023-11-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __USER_GPIO_H_
#define __USER_GPIO_H_

#ifdef __cplusplus
extern "C"
{
#endif

    enum GPIO_OUTPUT
    {
        OUT_A,
        OUT_B,
        OUT_C,
        PUMP_AIR,
        OUT_NUM,
    };
    enum GPIO_INPUT
    {
        INPUT_S1,
        INPUT_S2,
        INPUT_S3,
        INPUT_S4,
        INPUT_S5,
        INPUT_S6,
        INPUT_NUM,
    };
    /**
     * @brief 控制输出
     *
     * @param pin 端口号 @GPIO_OUTPUT
     * @param en 1:有输出 0:无输出
     */
    void gpio_set(enum GPIO_OUTPUT pin, uint8_t en);
    /**
     * @brief 控制输出
     *
     * @param pin 端口号 @GPIO_OUTPUT
     * @param en 1:有输出 0:无输出
     */
    uint8_t gpio_get(enum GPIO_OUTPUT pin);
    /**
     * @brief 读取输入信号
     * 硬件中 浮空为1 有信号为0
     * @param pin  端口号@GPIO_INPUT
     * @return uint8_t
     */
    uint8_t gpio_read(enum GPIO_INPUT pin);

#ifdef __cplusplus
}
#endif

#endif
