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

void lv_tablepage_about_create(lv_obj_t *parent)
{

	lv_obj_set_align(parent, LV_ALIGN_BOTTOM_MID);
	lv_obj_set_style_text_font(parent, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

	// lv_obj_set_layout(parent, LV_LAYOUT_FLEX);
	// lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW_WRAP);
	// lv_obj_set_flex_align(parent, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
	lv_obj_set_style_pad_left(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);


	const char* strbuff1 = "全激光式火情检测主机\n软件版本：V1 .0 20230808\n生产厂商：光力科技\n电话： +86(371)67858887\n传真： + 86(371)67991111\n免费电话：400-0057-577\n网址：http://www.gltech.cn\nEmail:info@gltech.cn";
	const char *strbuff2 = "遥控器功能：\n<▲> 上移动光标或数据递增\n<▼> 下移动光标或数据递减\n<◀> 左移光标\n<▶> 右移光标\n<确认> 确认操作或执行\n<取消> 取消操作或退出 ";
	lv_obj_t* plv_obj = lv_label_create(parent);
	lv_label_set_text(plv_obj, strbuff1);
	lv_obj_set_style_text_font(plv_obj, &ui_font_simhei12, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_label_set_long_mode(plv_obj, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(plv_obj, 0, 0);
	lv_obj_set_size(plv_obj, lv_pct(47), lv_pct(100));
	lv_obj_set_scrollbar_mode(plv_obj, LV_SCROLLBAR_MODE_OFF);

	lv_obj_set_style_border_width(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(plv_obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	// v_obj_set_style_text_font(plv_obj, &lv_font_simhei_16, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(plv_obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(plv_obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(plv_obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	 plv_obj = lv_label_create(parent);
	lv_label_set_text(plv_obj, strbuff2);
	lv_obj_set_style_text_font(plv_obj, &ui_font_simhei12, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_label_set_long_mode(plv_obj, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(plv_obj, lv_pct(50), 0);
	lv_obj_set_size(plv_obj, lv_pct(47), lv_pct(100));
	lv_obj_set_scrollbar_mode(plv_obj, LV_SCROLLBAR_MODE_OFF);

	lv_obj_set_style_border_width(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_radius(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(plv_obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	// v_obj_set_style_text_font(plv_obj, &lv_font_simhei_16, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(plv_obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(plv_obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(plv_obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(plv_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

}
