/**
 * @file ui_tableview_measure.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "../ui.h"
#define DBG_TAG "LVGL.sys"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>
#include "varDef.h"
#include <stdio.h>
#include <math.h>
#include "ui_tableview_P02_baseSetting.h"
#include "spinbox_ex.h"
lv_obj_t *lv_tablepage_baseSetting_create(lv_obj_t *parent)
{

	lv_obj_set_align(parent, LV_ALIGN_BOTTOM_MID);
	lv_obj_set_style_text_font(parent, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_left(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	// 出厂编号
	lv_obj_t *p_panel = lv_obj_create(parent);
	lv_obj_set_flex_flow(p_panel, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(p_panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

	lv_obj_set_size(p_panel, 16 * 6 + 8 * 10 + 50, 80);
	lv_obj_t *p_lable = lv_label_create(p_panel);
	lv_label_set_text(p_lable, "出厂编号：");
	lv_obj_set_style_text_font(p_lable, &ui_font_simhei16, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_t *ui_Spinbox1 = lv_spinbox_create(p_panel);

	lv_obj_set_size(ui_Spinbox1, 8 * 10, 40);
	// lv_obj_set_align(ui_Spinbox1, LV_ALIGN_TOP_LEFT);
	spinbox_ex_set_digit_format(ui_Spinbox1, 2, 0);
	spinbox_ex_set_range(ui_Spinbox1, 0, 999999);
	spinbox_ex_set_value(ui_Spinbox1, 150001);
	spinbox_ex_set_digit_format(ui_Spinbox1, 6, 0);

	// 按键组
	lv_obj_t *p_btn = lv_btnmatrix_create(parent);
	lv_obj_set_style_text_font(p_lable, &ui_font_simhei16, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_size(p_btn, 16 * 6 + 8 * 10 + 50, 35 * 5);

	//static  char* strbuff[] = { "保存出厂设置", "\n", "恢复出厂设置", "\n", "恢复原始设置", "\n", "数据清除", "\n", "系统复位",  "" };
	static  char* strbuff[] = { "save ", "\n", "restor", "\n", "恢复原始设置", "\n", "数据清除", "\n", "系统复位",  "" };
	lv_btnmatrix_set_map(p_btn, strbuff);

	return parent;
}
