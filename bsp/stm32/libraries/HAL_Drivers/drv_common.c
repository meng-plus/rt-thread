/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-7      SummerGift   first version
 */

#include "drv_common.h"
#include <board.h>

#ifdef RT_USING_BUILTIN_FDT
#include <drivers/ofw_fdt.h>
#include <drivers/pic.h>

extern const unsigned char rt_hw_builtin_fdt[];
#endif

#if defined(RT_USING_PIN) && !defined(RT_USING_DM)
#include <drv_gpio.h>
#endif

#ifdef RT_USING_SERIAL
#ifdef RT_USING_SERIAL_V2
#include <drv_usart_v2.h>
#else
#include <drv_usart.h>
#endif /* RT_USING_SERIAL */
#endif /* RT_USING_SERIAL_V2 */

#define DBG_TAG "drv_common"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#ifdef RT_USING_FINSH
#include <finsh.h>
static void reboot(uint8_t argc, char **argv)
{
    rt_hw_cpu_reset();
}
MSH_CMD_EXPORT(reboot, Reboot System);
#endif /* RT_USING_FINSH */

extern __IO uint32_t uwTick;
static uint32_t _systick_ms = 1;

/* SysTick configuration */
void rt_hw_systick_init(void)
{
    // Updates the variable SystemCoreClock
    SystemCoreClockUpdate();

    HAL_SYSTICK_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    NVIC_SetPriority(SysTick_IRQn, 0xFF);

    _systick_ms = 1000u / RT_TICK_PER_SECOND;
    if (_systick_ms == 0)
    {
        _systick_ms = 1;
    }
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
    {
        HAL_IncTick();
    }

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

uint32_t HAL_GetTick(void)
{
    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
    {
        HAL_IncTick();
    }

    return uwTick;
}

void HAL_IncTick(void)
{
    uwTick += _systick_ms;
}

void HAL_SuspendTick(void)
{
}

void HAL_ResumeTick(void)
{
}

void HAL_Delay(__IO uint32_t Delay)
{
    if (rt_thread_self())
    {
        rt_thread_mdelay(Delay);
    }
    else
    {
        for (rt_uint32_t count = 0; count < Delay; count++)
        {
            rt_hw_us_delay(1000);
        }
    }
}

/* re-implement tick interface for STM32 HAL */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
#ifndef SOC_SERIES_STM32MP1
    rt_hw_systick_init();
#endif

    /* Return function status */
    return HAL_OK;
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void _Error_Handler(char *s, int num)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    LOG_E("Error_Handler at file:%s num:%d", s, num);
    while (1)
    {
    }
    /* USER CODE END Error_Handler */
}

/**
 * This function will delay for some us.
 *
 * @param us the delay time of us
 */
void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint64_t ticks;
    rt_uint32_t told, tnow, tcnt = 0;
    rt_uint32_t reload = SysTick->LOAD;

    ticks = us * (reload / (1000000 / RT_TICK_PER_SECOND));
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

void MPU_Config(void);

#ifdef RT_USING_BUILTIN_FDT
static int rt_stm32_fdt_init(void)
{
    if (rt_fdt_prefetch((void *)rt_hw_builtin_fdt) != RT_EOK ||
        rt_fdt_unflatten() != RT_EOK)
    {
        LOG_E("Builtin FDT initialization failed");
        RT_ASSERT(0);
    }

#ifdef RT_USING_PIC
    if (rt_pic_init() != RT_EOK || rt_pic_irq_init() != RT_EOK)
    {
        LOG_E("PIC initialization failed");
        RT_ASSERT(0);
    }
#endif

    return 0;
}
#if defined(RT_USING_COMPONENTS_INIT)
/* "1" runs before pin ("1.0"). Heap is already ready. */
INIT_EARLY_EXPORT(rt_stm32_fdt_init);
#endif
#endif

#if !defined(RT_USING_COMPONENTS_INIT) || defined(RT_USING_CONSOLE)
static int rt_stm32_console_init(void)
{
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

#if defined(RT_USING_CONSOLE) && defined(RT_USING_NANO)
    extern void rt_hw_console_init(void);
    rt_hw_console_init();
#endif

    return 0;
}
#if defined(RT_USING_COMPONENTS_INIT)
/* After pin ("1.0"), uart ("1.1") and null ("1.2"). */
INIT_EARLY_EXPORT_3(rt_stm32_console_init);
#endif
#endif

/**
 * This function will initial STM32 board.
 *
 * Cache, HAL, clock and heap stay here. Their order is fixed and they must
 * finish before any INIT_EARLY function. FDT, pin, uart, null and the console
 * are registered in the early stage, then board drivers run.
 */
rt_weak void rt_hw_board_init(void)
{
#ifdef BSP_SCB_ENABLE_I_CACHE
    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();
#endif

#ifdef BSP_SCB_ENABLE_D_CACHE
    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();
#endif

    /* HAL_Init() function is called at the beginning of the program */
    HAL_Init();

    /* System clock initialization */
    SystemClock_Config();

#if defined(RT_USING_HEAP)
    /* Heap initialization */
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#else
#if defined(RT_USING_BUILTIN_FDT)
    rt_stm32_fdt_init();
#endif
#if defined(RT_USING_PIN) && !defined(RT_USING_DM)
    rt_hw_pin_init();
#endif
#ifdef RT_USING_SERIAL
    rt_hw_usart_init();
#endif
    rt_stm32_console_init();
#endif
}
