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
#define DBG_TAG "LVGL.ms"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>
#include "lv_table.h"
#include "varDef.h"
#include "stdio.h"

typedef enum _REAL_VALUE
{
	REAL_TEMP,
	ERAL_PRESS,
	ERAL_TEMP_1,
	ERAL_FLOW,
	ERAL_NUM,
} REAL_VALUE_E;

typedef struct _REAL_VALUE_INFO
{
	char *name;
	uint8_t dot;
	char *unit;
} real_value_info_t;
static const real_value_info_t s_read_value_info[] = {
	[REAL_TEMP] = {"温度", 1, "℃"},
	[ERAL_PRESS] = {"压力", 1, "kPa"},
	[ERAL_TEMP_1] = {"气室", 1, "℃"},
	[ERAL_FLOW] = {"流量", 1, "L/min"},

};
static lv_obj_t *s_spangroup[ERAL_NUM];
static lv_obj_t *ui_table;

static lv_timer_t *time_handle = NULL;
static void time_reflash_cb(struct _lv_timer_t *);
void draw_part_event_cb(lv_event_t *e);
static void ui_event_tablepage_measure(lv_event_t *e);
void lv_tablepage_measure_create(lv_obj_t *parent)
{

	lv_obj_set_align(parent, LV_ALIGN_TOP_LEFT);
	lv_obj_set_layout(parent, LV_LAYOUT_FLEX);
	lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW_WRAP);
	lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
	lv_obj_set_style_pad_left(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	ui_table = lv_table_create(parent);
	lv_obj_set_align(ui_table, LV_ALIGN_TOP_MID);
	lv_obj_set_size(ui_table, lv_pct(100), lv_pct(60));
	lv_obj_set_pos(ui_table, 0, 0);
	// lv_obj_set_style_border_width(ui_table, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui_table, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui_table, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui_table, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui_table, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);

	lv_table_set_col_cnt(ui_table, 4);
	// lv_table_set_col_width(ui_table, 0, 80);
	lv_table_set_col_width(ui_table, 1, 80);
	// lv_table_set_col_width(ui_table, 2, 80);
	lv_table_set_col_width(ui_table, 3, 80);
	// lv_obj_set_style_text_font(ui_table, &ui_font_heiti12, LV_PART_MAIN | LV_STATE_DEFAULT);
	/*Fill the first column*/
	uint8_t en_count = 2;
	lv_table_set_cell_value(ui_table, 0, 0, "类型");
	lv_table_set_cell_value(ui_table, 0, 1, "数据");
	lv_table_set_cell_value(ui_table, 0, 2, "类型");
	lv_table_set_cell_value(ui_table, 0, 3, "数据");

	/*Set a smaller height to the ui_table. It'll make it scrollable*/
	// lv_obj_set_height(ui_table, 100);
	lv_obj_center(ui_table);
	/*Add an event callback to to apply some custom drawing*/
	lv_obj_add_event_cb(ui_table, draw_part_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
	lv_obj_add_event_cb(ui_table, ui_event_tablepage_measure, LV_EVENT_ALL, NULL);
	time_handle = lv_timer_create(time_reflash_cb, 1000, NULL);

	// Write codes P01_main_spangroup_8
	for (uint8_t i = 0; i < ERAL_NUM; i++)
	{
		const real_value_info_t *pInfo = &s_read_value_info[i];
		lv_obj_t *spangroup = lv_spangroup_create(parent);
		s_spangroup[i] = spangroup;
		lv_spangroup_set_align(spangroup, LV_TEXT_ALIGN_LEFT);
		lv_spangroup_set_overflow(spangroup, LV_SPAN_OVERFLOW_CLIP);
		lv_spangroup_set_mode(spangroup, LV_SPAN_MODE_BREAK);
		lv_obj_set_style_pad_left(spangroup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(spangroup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(spangroup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(spangroup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

		/* creat span*/
		lv_span_t *span;
		span = lv_spangroup_new_span(spangroup);
		lv_span_set_text(span, pInfo->name);
		lv_style_set_text_color(&span->style, lv_color_hex(0x000000));
		lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_NONE);
		span = lv_spangroup_new_span(spangroup);
		lv_span_set_text(span, "28.0");
		lv_style_set_text_color(&span->style, lv_color_hex(0x2195f6));
		lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_NONE);
		lv_style_set_text_font(&span->style, &ui_font_simhei24);
		span = lv_spangroup_new_span(spangroup);
		lv_span_set_text(span, pInfo->unit);
		lv_style_set_text_color(&span->style, lv_color_hex(0x000000));
		lv_style_set_text_decor(&span->style, LV_TEXT_DECOR_NONE);
		lv_obj_set_scrollbar_mode(spangroup, LV_SCROLLBAR_MODE_OFF);
		lv_obj_set_size(spangroup, 200, 25);
	}
}
void draw_part_event_cb(lv_event_t *e)
{
	lv_obj_t *obj = lv_event_get_target(e);
	lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
	/*If the cells are drawn...*/
	if (dsc->part == LV_PART_ITEMS)
	{
		uint32_t row = dsc->id / lv_table_get_col_cnt(obj);
		uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);

		/*Make the texts in the first cell center aligned*/
		if (row == 0)
		{
			dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
			dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_BLUE), dsc->rect_dsc->bg_color, LV_OPA_20);
			dsc->rect_dsc->bg_opa = LV_OPA_COVER;
		}
		/*In the first column align the texts to the right*/
		else if (col == 0)
		{
			dsc->label_dsc->align = LV_TEXT_ALIGN_LEFT;
		}

		/*MAke every 2nd row grayish*/
		if ((row != 0 && row % 2) == 0)
		{
			dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_GREY), dsc->rect_dsc->bg_color, LV_OPA_10);
			dsc->rect_dsc->bg_opa = LV_OPA_COVER;
		}
	}
}
void ui_event_tablepage_measure(lv_event_t *e)
{
	lv_event_code_t event_code = lv_event_get_code(e);
	lv_obj_t *target = lv_event_get_target(e);
	if (event_code == LV_EVENT_SCREEN_LOADED)
	{
		// layout init
	}
	if (event_code == LV_EVENT_SCREEN_UNLOADED)
	{
		//_ui_screen_delete(&ui_P00startup);
		lv_timer_del(time_handle);
		time_handle = NULL;
	}
	if (event_code == LV_EVENT_DELETE)
	{
		LOG_I("LV_EVENT_DELETE");
		lv_timer_del(time_handle);
		time_handle = NULL;
		ui_table = NULL;
	}
}
void time_reflash_cb(struct _lv_timer_t *data)
{

	uint8_t en_count = 2;
	lv_table_set_cell_value(ui_table, 0, 0, "类型");
	lv_table_set_cell_value(ui_table, 0, 1, "数据");
	lv_table_set_cell_value(ui_table, 0, 2, "类型");
	lv_table_set_cell_value(ui_table, 0, 3, "数据");
	for (size_t i = 0; i < GAS_TYPE_NUM; i++)
	{
		char strbuff[32];
		if (g_varParam.m_sensor_config[i].en)
		{
			if (g_varInit.m_sensor_info[i].unit && g_varInit.m_sensor_info[i].unit[0] != '\0')
			{
				lv_table_set_cell_value_fmt(ui_table, en_count / 2, (en_count % 2) * 2, "%s(%s)", g_varInit.m_sensor_info[i].name, g_varInit.m_sensor_info[i].unit);
			}
			else
			{
				lv_table_set_cell_value(ui_table, en_count / 2, (en_count % 2) * 2, g_varInit.m_sensor_info[i].name);
			}
			sprintf(strbuff, "%.*f", g_varInit.m_sensor_info[i].dot, g_varWork.m_sensor_value[i].msr_view);
			lv_table_set_cell_value(ui_table, en_count / 2, (en_count % 2) * 2 + 1, strbuff);
			en_count++;
		}
	}
	for (uint8_t i = 0; i < ERAL_NUM; i++)
	{
		lv_span_t *span = lv_spangroup_get_child(s_spangroup[i], 1);
		char strbuff[32];
		sprintf(strbuff, "%.*f", s_read_value_info[i].dot, lv_rand(100, 400) / 10.0);
		lv_span_set_text(span, strbuff);
	}
}
