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
#include <stdlib.h>
#include <stdio.h>
static const uint32_t baud_array[] = {4800, 9600, 19200, 38400, 115200};
static void lv_event_value_changed(lv_event_t *e)
{
    // char *param = lv_event_get_param(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *sub_obj = NULL;

    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_2);
    if (sub_obj)
        if (g_sensor_param.dev_config[0].en)
        {
            lv_obj_add_state(sub_obj, LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_clear_flag(sub_obj, LV_STATE_CHECKED);
        }

    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_3);
    if (sub_obj)
        if (g_sensor_param.dev_config[1].en)
        {
            lv_obj_add_state(sub_obj, LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_clear_flag(sub_obj, LV_STATE_CHECKED);
        }

    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_2_BAUD);
    if (sub_obj)
    {
        size_t i;
        for (i = 0; i < sizeof(baud_array) / sizeof(baud_array[0]); i++)
        {
            if (g_sensor_param.dev_config[0].baud == baud_array[i])
            {
                lv_dropdown_set_selected(sub_obj, i);
            }
        }
    }
    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_3_BAUD);
    if (sub_obj)
    {

        size_t i;
        for (i = 0; i < sizeof(baud_array) / sizeof(baud_array[0]); i++)
        {
            if (g_sensor_param.dev_config[1].baud == baud_array[i])
            {
                lv_dropdown_set_selected(sub_obj, i);
            }
        }
    }
    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_2_ADDR);
    if (sub_obj)
    {
        lv_textarea_del_char(sub_obj);
        char strbuff[6];
        rt_sprintf(strbuff, "%4d", g_sensor_param.dev_config[0].addr);
        lv_textarea_add_text(sub_obj, strbuff);
    }
    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_3_ADDR);
    if (sub_obj)
    {
        lv_textarea_del_char(sub_obj);
        char strbuff[6];
        rt_sprintf(strbuff, "%4d", g_sensor_param.dev_config[1].addr);
        lv_textarea_add_text(sub_obj, strbuff);
    }
}
static void lv_event_clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_SEN_CONFIG comp_id = (enum UI_COMP_SEN_CONFIG)lv_event_get_user_data(e);
    if (obj && code == LV_EVENT_CLICKED)
    {
        switch (comp_id)
        {
        case SEN_CONFIG_RESTORE:
            var_reload(&g_sensor_param);
            lv_event_send(lv_obj_get_parent(obj), LV_EVENT_VALUE_CHANGED, NULL);
            break;
        case SEN_CONFIG_SAVE:
            var_save(&g_sensor_param);
            break;
        default:
            break;
        }
    }
}
static void child_event_value_changed(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_SEN_CONFIG comp_id = (enum UI_COMP_SEN_CONFIG)lv_event_get_user_data(e);
    if (obj && code == LV_EVENT_VALUE_CHANGED)
    {
        uint32_t chn_sel = 0;

        switch (comp_id)
        {
        case SEN_CONFIG_RS485_2:
            if (g_sensor_param.device_num > 0 &&
                g_sensor_param.dev_config)
            {
                g_sensor_param.dev_config[0].en = NULL != (lv_obj_get_state(obj) & LV_STATE_CHECKED);
            }
            break;
        case SEN_CONFIG_RS485_2_ADDR:
            if (g_sensor_param.device_num > 0 &&
                g_sensor_param.dev_config)
            {
                g_sensor_param.dev_config[0].addr = atoi(lv_textarea_get_text(obj));
            }
            break;
        case SEN_CONFIG_RS485_2_BAUD:
            chn_sel = lv_dropdown_get_selected(obj);
            if (g_sensor_param.dev_config &&
                chn_sel < sizeof(baud_array) / sizeof(baud_array[0]))
            {
                g_sensor_param.dev_config[0].baud = baud_array[chn_sel];
            }
            break;
        case SEN_CONFIG_RS485_3:
            if (g_sensor_param.device_num > 1 &&
                g_sensor_param.dev_config)
            {
                g_sensor_param.dev_config[1].en = NULL != (lv_obj_get_state(obj) & LV_STATE_CHECKED);
            }
            break;
        case SEN_CONFIG_RS485_3_ADDR:
            if (g_sensor_param.device_num > 1 &&
                g_sensor_param.dev_config)
            {
                g_sensor_param.dev_config[1].addr = atoi(lv_textarea_get_text(obj));
            }
            break;
        case SEN_CONFIG_RS485_3_BAUD:
            chn_sel = lv_dropdown_get_selected(obj);
            if (g_sensor_param.device_num > 1 &&
                g_sensor_param.dev_config &&
                chn_sel < sizeof(baud_array) / sizeof(baud_array[0]))
            {
                g_sensor_param.dev_config[1].baud = baud_array[chn_sel];
            }
            break;
        case SEN_CONFIG_RESTORE:
            var_reload(&g_sensor_param);
            lv_event_send(lv_obj_get_parent(obj), LV_EVENT_VALUE_CHANGED, NULL);
            break;
        case SEN_CONFIG_SAVE:
            var_save(&g_sensor_param);
            break;
        default:
            break;
        }
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
