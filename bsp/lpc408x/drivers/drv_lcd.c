/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-08-08     Yang        the first version
 */

#include <board.h>
#include <rtdevice.h>
#include <drv_lcd.h>
#include <string.h>
#include <stdbool.h>
#include "LPC407x_8x_177x_8x.h"
#include "lpc_lcd.h"
/*
======================================================================
==
======================================================================
*/
#define PWM_PERIODS_FREQ 1000

/*=============================================
==
===============================================*/
#define C_GLCD_REFRESH_FREQ (50)
#define C_GLCD_H_SIZE BSP_LCD_WIDTH
#define C_GLCD_H_PULSE 41      // 12
#define C_GLCD_H_FRONT_PORCH 4 // 10
#define C_GLCD_H_BACK_PORCH 45 // 19
#define C_GLCD_V_SIZE BSP_LCD_HEIGHT
#define C_GLCD_V_PULSE 10      // 9//2
#define C_GLCD_V_FRONT_PORCH 6 // 5//2
#define C_GLCD_V_BACK_PORCH 12 // 11//1

#define C_GLCD_CLK_PER_LINE (C_GLCD_H_SIZE + C_GLCD_H_PULSE + C_GLCD_H_FRONT_PORCH + C_GLCD_H_BACK_PORCH)
#define C_GLCD_LINES_PER_FRAME (C_GLCD_V_SIZE + C_GLCD_V_PULSE + C_GLCD_V_FRONT_PORCH + C_GLCD_V_BACK_PORCH)
#define C_GLCD_PIX_CLK 9000000 //(C_GLCD_CLK_PER_LINE * C_GLCD_LINES_PER_FRAME * C_GLCD_REFRESH_FREQ)

#define LCD_BITS_PER_PIXEL BSP_LCD_BPP

static rt_uint16_t *lcd_framebuffer = RT_NULL;
static rt_uint16_t *_rt_framebuffer = RT_NULL;
static struct rt_device_graphic_info _lcd_info;
static struct rt_device lcd;

static void lcd_gpio_init(void)
{

    LPC_SC->PCONP |= (1 << 15); // GPIO

    LPC_IOCON->P2_12 = 0x25; // Red 0	LCD_VD[3]
    LPC_IOCON->P2_6 = 0x27;  // Red 1	LCD_VD[4]
    LPC_IOCON->P0_10 = 0x27; // Red 2	LCD_VD[5]
    LPC_IOCON->P2_8 = 0x27;  // Red 3	LCD_VD[6]
    LPC_IOCON->P2_9 = 0x27;  // Red 4	LCD_VD[7]

    LPC_IOCON->P1_20 = 0x27; // Green 0	LCD_VD[10]
    LPC_IOCON->P1_21 = 0x27; // Green 1	LCD_VD[11]
    LPC_IOCON->P1_22 = 0x27; // Green 2	LCD_VD[12]
    LPC_IOCON->P1_23 = 0x27; // Green 3	LCD_VD[13]
    LPC_IOCON->P1_24 = 0x27; // Green 4	LCD_VD[14]
    LPC_IOCON->P1_25 = 0x27; // Green 5	LCD_VD[15]

    LPC_IOCON->P2_13 = 0x27; // Blue 0	LCD_VD[19]
    LPC_IOCON->P1_26 = 0x27; // Blue 1	LCD_VD[20]
    LPC_IOCON->P1_27 = 0x27; // Blue 2	LCD_VD[21]
    LPC_IOCON->P1_28 = 0x27; // Blue 3	LCD_VD[22]
    LPC_IOCON->P1_29 = 0x27; // Blue 4	LCD_VD[23]

    LPC_IOCON->P2_0 = 0x20; // LCD_PWR
    LPC_IOCON->P2_1 = 0x20; // LCD_LE
    LPC_IOCON->P2_2 = 0x27; // LCD_DCLK
    LPC_IOCON->P2_3 = 0x27; // LCD_FP
    LPC_IOCON->P2_4 = 0x27; // LCD_ENAB_M
    LPC_IOCON->P2_5 = 0x27; // LCD_LP

    LPC_GPIO2->SET |= (1 << 0);
    LPC_GPIO2->SET |= (1 << 1);

    LPC_GPIO2->DIR |= (1 << 0);
    LPC_GPIO2->DIR |= (1 << 1);
}

void LCD_IRQHandler(void)
{
    //    uint32_t intStatus = LCDC_GetEnabledInterruptsPendingStatus(LCD);
    //
    //    LCDC_ClearInterruptsStatus(LCD, intStatus);
    //
    //    if (intStatus & kLCDC_VerticalCompareInterrupt)
    //    {
    //        // s_frameEndFlag = true;
    //    }
    //__DSB();
}
/**********************************************************************************
** 函数名称:
** �?    �?:
** 输入参数:
** 输出参数:
** �?    �?:
***********************************************************************************/
void LcdPowerCtrl(bool st)
{
    uint32_t i;

    if (st)
    {
        LPC_LCD->CTRL |= (1ul << 0);
        for (i = 10000; i; i--)
            ;
        LPC_LCD->CTRL |= (1ul << 11);
    }
    else
    {
        LPC_LCD->CTRL &= (~(1ul << 11));
        for (i = 10000; i; i--)
            ;
        LPC_LCD->CTRL &= (~(1ul << 0));
    }
}

/**********************************************************************************
** 函数名称:
** �?    �?:
** 输入参数:
** 输出参数:
** �?    �?:
***********************************************************************************/
void LcdBackLightCtrl(bool st)
{

    if (st != 0)
        LPC_GPIO2->CLR = (1 << 0); /* Enable power */
    else
        LPC_GPIO2->SET = (1 << 0); /* Disable power */
}
/* RT-Thread Device Interface */
static rt_err_t rt_lcd_init(rt_device_t dev)
{

    /* Init GLCD cotroller */
    LPC_SC->PCONP |= (1 << 0); /* enable LCD controller clock */
    LPC_LCD->CRSR_CTRL &= ~(1 << 0);
    ;                                     /* Disable cursor */
    LPC_LCD->CTRL = (0 << 0) |            /* disable GLCD controller */
                    (6 << 1) |            /* 16 bpp 5:6:5*/
                    (1 << 5) |            /* TFT panel */
                    (0 << 7) |            /* single panel */
                    (1 << 8) |            /* notmal output */
                    (0 << 9) |            /* little endian byte order */
                    (0 << 10) |           /* little endian pix order */
                    (0 << 11);            /* disable power */
                                          /* init pixel clock */
                                          //	LPC_SC->LCD_CFG = PeripheralClock / (uint32_t)C_GLCD_PIX_CLK;
                                          //	LPC_SC->LCD_CFG = 0;
    uint32_t temp = 5;                    // PeripheralClock / (uint32_t)C_GLCD_PIX_CLK;
    LPC_LCD->POL = ((temp & 0x1f) << 0) | /* PCD_LO */
                   (0 << 5) |             /* clock source for the LCD block is HCLK */
                   ((1 - 1) << 6) |       /* ACB */
                   (1 << 11) |            /* LCDFP pin is active LOW and inactive HIGH */
                   (1 << 12) |            /* LCDLP pin is active LOW and inactive HIGH */
                   (0 << 13) |            /* data is driven out into the LCD on the rising edge */
                   (0 << 14) |            /* active high */
                   ((C_GLCD_H_SIZE - 1) << 16) |
                   (0 << 26) |          /* bypass inrenal clk divider */
                   ((temp >> 5) << 27); /* PCD_HO */

    /* init Horizontal Timing */
    LPC_LCD->TIMH = ((C_GLCD_H_BACK_PORCH - 1) << 24) |
                    ((C_GLCD_H_FRONT_PORCH - 1) << 16) |
                    ((C_GLCD_H_PULSE - 1) << 8) |
                    (((C_GLCD_H_SIZE / 16) - 1) << 2);

    /* init Vertical Timing */
    LPC_LCD->TIMV = (C_GLCD_V_BACK_PORCH << 24) |
                    (C_GLCD_V_FRONT_PORCH << 16) |
                    (C_GLCD_V_PULSE << 10) |
                    ((C_GLCD_V_SIZE - 1) << 0);

    /* Frame Base Address doubleword aligned */
    LPC_LCD->UPBASE = (rt_uint32_t)lcd_framebuffer;
    LPC_LCD->LPBASE = (rt_uint32_t)lcd_framebuffer;
    for (volatile uint32_t i = 50000; i; i--)
        ;
    return RT_EOK;
}

static rt_err_t rt_lcd_control(rt_device_t dev, int cmd, void *args)
{
    switch (cmd)
    {
    case RTGRAPHIC_CTRL_RECT_UPDATE:
    {
        struct rt_device_rect_info *rect_info = (struct rt_device_rect_info *)args;

        /* 先指向绘�? buff 显示 */
        LPC_LCD->UPBASE = (rt_uint32_t)_rt_framebuffer;

        /* 从绘�? buff copy 数据到显�? buff */
        if (rect_info->width * rect_info->height < BSP_LCD_WIDTH * BSP_LCD_HEIGHT / 5 * 3)
        {
            int index = 0;
            rt_uint8_t *p = (rt_uint8_t *)lcd_framebuffer;
            rt_uint8_t *q = (rt_uint8_t *)_rt_framebuffer;

            p += (rect_info->x + rect_info->y * BSP_LCD_WIDTH) * sizeof(rt_uint16_t);
            q += (rect_info->x + rect_info->y * BSP_LCD_WIDTH) * sizeof(rt_uint16_t);

            for (index = 0; index < rect_info->height; index++)
            {
                memcpy((void *)p, (void *)q, sizeof(rt_uint16_t) * rect_info->width);
                p += BSP_LCD_WIDTH * sizeof(rt_uint16_t);
                q += BSP_LCD_WIDTH * sizeof(rt_uint16_t);
            }
        }
        else
        {
            memcpy((void *)lcd_framebuffer, _rt_framebuffer, sizeof(rt_uint16_t) * BSP_LCD_HEIGHT * BSP_LCD_WIDTH);
        }
        /* 指回显示 buff */
        LPC_LCD->UPBASE = (rt_uint32_t)lcd_framebuffer;
        rt_kprintf("====> rect_info : %d %d %d %d\n", rect_info->x, rect_info->y, rect_info->width, rect_info->height);
    }
    break;
    case RTGRAPHIC_CTRL_POWERON:
        break;
    case RTGRAPHIC_CTRL_POWEROFF:
        break;
    case RTGRAPHIC_CTRL_GET_INFO:
        memcpy(args, &_lcd_info, sizeof(_lcd_info));
        break;
    case RTGRAPHIC_CTRL_SET_MODE:
        break;
    }

    return RT_EOK;
}

rt_uint16_t *get_lcd_framebuffer(void)
{
    return lcd_framebuffer;
}

void lcd_clear(rt_uint16_t color)
{
    volatile rt_uint16_t *p = (rt_uint16_t *)lcd_framebuffer;
    int x, y;

    for (y = 0; y <= BSP_LCD_HEIGHT; y++)
    {
        for (x = 0; x <= BSP_LCD_WIDTH; x++)
        {
            *p++ = color; /* red */
        }
    }
}
void lcd_fill_array(rt_uint16_t x_start, rt_uint16_t y_start, rt_uint16_t x_end, rt_uint16_t y_end, void *pcolor)
{
    uint16_t *color_ptr = (uint16_t *)pcolor;
   uint32_t offset;
    for (size_t y = y_start; y <= y_end; y++)
    {
        for (size_t x = x_start; x <= x_end; x++)
        {
            offset=y * BSP_LCD_WIDTH + x;
            lcd_framebuffer[offset] = *color_ptr;
            color_ptr++;
        }
    }
}
int rt_hw_lcd_init(void)
{
    _rt_framebuffer = rt_malloc_align(sizeof(rt_uint16_t) * BSP_LCD_HEIGHT * BSP_LCD_WIDTH, 32);
    if (_rt_framebuffer == RT_NULL)
        return 0; /* no memory yet */
    rt_memset(_rt_framebuffer, 0, sizeof(rt_uint16_t) * BSP_LCD_HEIGHT * BSP_LCD_WIDTH);
    lcd_framebuffer = rt_malloc_align(sizeof(rt_uint16_t) * BSP_LCD_HEIGHT * BSP_LCD_WIDTH, 32);
    rt_memset(lcd_framebuffer, 0, sizeof(rt_uint16_t) * BSP_LCD_HEIGHT * BSP_LCD_WIDTH);
    _lcd_info.bits_per_pixel = LCD_BITS_PER_PIXEL;
    _lcd_info.pixel_format = RTGRAPHIC_PIXEL_FORMAT_BGR565; // RTGRAPHIC_PIXEL_FORMAT_ARGB888
    _lcd_info.framebuffer = (void *)_rt_framebuffer;
    _lcd_info.width = BSP_LCD_WIDTH;
    _lcd_info.height = BSP_LCD_HEIGHT;

    /* init device structure */
    lcd.type = RT_Device_Class_Graphic;
    lcd.init = rt_lcd_init;
    lcd.open = RT_NULL;
    lcd.close = RT_NULL;
    lcd.control = rt_lcd_control;
    lcd.user_data = (void *)&_lcd_info;

    /* register lcd device to RT-Thread */
    rt_device_register(&lcd, "lcd", RT_DEVICE_FLAG_RDWR);
    lcd_gpio_init();
    rt_lcd_init(&lcd);
    LcdPowerCtrl(1);
    LcdBackLightCtrl(1);

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_lcd_init);
void lcd_test(void)
{
    lcd_clear(0xf800);
    rt_thread_delay(200);
    lcd_clear(0x07e0);
    rt_thread_delay(200);
    lcd_clear(0x001f);
    rt_thread_delay(200);
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(lcd_clear, lcd_clear);
FINSH_FUNCTION_EXPORT(lcd_test, lcd_test);
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(lcd_clear, lcd_clear);
MSH_CMD_EXPORT(lcd_test, lcd_test);
#endif
#endif
