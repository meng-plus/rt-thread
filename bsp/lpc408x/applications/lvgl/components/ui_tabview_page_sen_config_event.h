/**
 * @file ui_tabview_page_sen_config_event.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 指定页面专用的事件处理函数，不可他用
 * @version 0.1
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "ui_tabview_page_sen_config.h"
#include "ui_comp.h"
#include "system_var.h"

static void lv_event_value_changed(lv_event_t *e)
{
    // char *param = lv_event_get_param(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *dd = ui_comp_get_child(obj, SEN_CONFIG_RS485_2_BAUD);
    if (dd && g_var_work.dts)
    {
    }
}
static void rs485_baud_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (obj && code == LV_EVENT_VALUE_CHANGED)
    {
        uint32_t chn_sel = 0;
        chn_sel = lv_dropdown_get_selected(obj);
    }
}
static void btnm_event_handler(lv_event_t *e);
static void ta_event_cb(lv_event_t *e)
{
    lv_indev_t *indev = lv_indev_get_act();
    if (indev == NULL)
        return;
    lv_indev_type_t indev_type = lv_indev_get_type(indev);

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    lv_obj_t *kb = lv_event_get_user_data(e);

    if (code == LV_EVENT_CLICKED &&
        (LV_INDEV_TYPE_POINTER == indev_type || indev_type == LV_INDEV_TYPE_ENCODER))
    {
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        // lv_group_focus_obj(kb);
        lv_obj_add_event_cb(kb, btnm_event_handler, LV_EVENT_VALUE_CHANGED, ta);
    }

    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)
    {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_event_cb(kb, btnm_event_handler);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_CLICK_FOCUSABLE); /*To keep the text area focused on button clicks*/
    }
}
static void btnm_event_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *ta = lv_event_get_user_data(e);
    const char *txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));

    if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)
        lv_textarea_del_char(ta);
    else if (strcmp(txt, LV_SYMBOL_NEW_LINE) == 0)
        lv_event_send(ta, LV_EVENT_READY, NULL);
    else
        lv_textarea_add_text(ta, txt);
}
