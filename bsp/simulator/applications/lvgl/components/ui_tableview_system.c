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

enum UI_COMP_SLIDER
{
	UI_COMP_SLIDER_PARENT = 0,
	UI_COMP_SLIDER_VALUE,
	UI_COMP_SLIDER_LABLE,
	_UI_COMP_SLIDER_NUM
};

static void time_reflash_cb(struct _lv_timer_t *);
static void ui_event_tablepage_system(lv_event_t *e);
static void ui_event_slider(lv_event_t* e);
void lv_tablepage_system_create(lv_obj_t *parent)
{
	lv_obj_add_event_cb(parent, ui_event_tablepage_system, LV_EVENT_ALL, NULL);
	lv_obj_set_align(parent, LV_ALIGN_BOTTOM_MID);
	lv_obj_set_style_text_font(parent, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

	// lv_obj_set_layout(parent, LV_LAYOUT_FLEX);
	// lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW_WRAP);
	// lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
	lv_obj_set_style_pad_left(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_t *plv_obj = lv_label_create(parent);
	lv_label_set_text(plv_obj, "通讯序号：\n上行波特率：\n下行波特率：\n背光延时(分)：\n系统时间：");
	lv_obj_set_style_text_font(plv_obj, &ui_font_simhei16, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_label_set_long_mode(plv_obj, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(plv_obj, 0, 0);
	lv_obj_set_size(plv_obj, 140, 150);
	lv_obj_set_scrollbar_mode(plv_obj, LV_SCROLLBAR_MODE_OFF);

	lv_obj_set_style_border_width(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(plv_obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	// v_obj_set_style_text_font(plv_obj, &lv_font_simhei_16, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(plv_obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(plv_obj, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(plv_obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	/*通讯序号*/
	lv_obj_t *ui_Spinbox1 = lv_spinbox_create(parent);
	lv_obj_set_pos(ui_Spinbox1, 140, 0);
	lv_obj_set_size(ui_Spinbox1, 130, 30);
	lv_obj_set_align(ui_Spinbox1, LV_ALIGN_TOP_LEFT);
	lv_spinbox_set_digit_format(ui_Spinbox1, 2, 0);

	lv_spinbox_set_range(ui_Spinbox1, 0, 99);
	lv_spinbox_set_step(ui_Spinbox1, (uint32_t)round(pow(10, 1 - 1)));

	const char *pBuadStr = "1200\n2400\n4800\n9600\n14400\n19200\n38400\n115200";
	/*上行波特率*/
	lv_obj_t *pddlist = lv_dropdown_create(parent);
	lv_dropdown_set_options(pddlist, pBuadStr);
	lv_obj_set_pos(pddlist, 140, 30);
	lv_obj_set_size(pddlist, 130, 30);
	lv_obj_set_scrollbar_mode(pddlist, LV_SCROLLBAR_MODE_OFF);
	/*下行波特率*/
	lv_obj_t *pddlist_1 = lv_dropdown_create(parent);
	lv_dropdown_set_options(pddlist_1, pBuadStr);
	lv_obj_set_pos(pddlist_1, 140, 60);
	lv_obj_set_size(pddlist_1, 130, 30);
	lv_obj_set_scrollbar_mode(pddlist_1, LV_SCROLLBAR_MODE_OFF);

	lv_obj_t *pslider = lv_slider_create(parent);
	lv_slider_set_range(pslider, 0, 100);
	lv_slider_set_value(pslider, 50, false);
	lv_obj_set_pos(pslider, 140, 90);
	lv_obj_set_size(pslider, 160, 16);
	lv_obj_set_scrollbar_mode(pslider, LV_SCROLLBAR_MODE_OFF);

	lv_obj_t * pLable = lv_label_create(parent);
	lv_obj_set_style_text_font(pLable, &ui_font_simhei16, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_label_set_text_fmt(pLable, "%d 分", lv_slider_get_value(pslider));
	lv_obj_set_pos(pLable, 350, 90);
	lv_obj_set_size(pLable, 60, 16);
	lv_obj_add_event_cb(pslider, ui_event_slider, LV_EVENT_VALUE_CHANGED, parent);

	lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_SLIDER_NUM);

	children[UI_COMP_SLIDER_PARENT] = parent;
	children[UI_COMP_SLIDER_VALUE] = pslider;
	children[UI_COMP_SLIDER_LABLE] = pLable;
	lv_obj_add_event_cb(parent, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
	lv_obj_add_event_cb(parent, del_component_child_event_cb, LV_EVENT_DELETE, children);
	/*date */
	lv_obj_t *pdate = lv_label_create(parent);
	lv_label_set_text(pdate, "2023/07/31");
	lv_obj_set_style_text_align(pdate, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_add_flag(pdate, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_set_pos(pdate, 133, 120);
	lv_obj_set_size(pdate, 130, 25);
	lv_obj_set_scrollbar_mode(pdate, LV_SCROLLBAR_MODE_OFF);
}
void ui_event_tablepage_system(lv_event_t *e)
{
	lv_event_code_t event_code = lv_event_get_code(e);
	lv_obj_t *target = lv_event_get_target(e);
	if (event_code == LV_EVENT_SCREEN_LOADED)
	{
		// layout init
	}
	if (event_code == LV_EVENT_SCREEN_UNLOADED)
	{
	}
	if (event_code == LV_EVENT_DELETE)
	{
		LOG_I("LV_EVENT_DELETE");
	}
}
void ui_event_slider(lv_event_t *e)
{
	lv_obj_t* target = lv_event_get_target(e);
	lv_obj_t *p_parent  = lv_event_get_user_data(e);
	lv_obj_t *p_value = ui_comp_get_child(p_parent, UI_COMP_SLIDER_VALUE);

	int32_t slider_value = lv_slider_get_value(p_value);
	lv_obj_t *p_lable = ui_comp_get_child(p_parent, UI_COMP_SLIDER_LABLE);
	lv_label_set_text_fmt(p_lable, "%d 分", slider_value);
}
