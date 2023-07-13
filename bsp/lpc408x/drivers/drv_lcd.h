#ifndef __DRV_LCD_H__
#define __DRV_LCD_H__


int rt_hw_lcd_init(void);
void lcd_fill_array(rt_uint16_t x_start, rt_uint16_t y_start, rt_uint16_t x_end, rt_uint16_t y_end, void *pcolor);
#endif

