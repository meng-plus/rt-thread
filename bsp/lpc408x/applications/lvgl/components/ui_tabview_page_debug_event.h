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
#include "ui_comp.h"
#include "system_var.h"
#include <stdlib.h>
#include <stdio.h>
#include "sensor_var.h"
#include "ui_comp_edit.h"
static const uint32_t baud_array[] = {4800, 9600, 19200, 38400, 115200};
static void lv_event_notify_page(lv_event_t *e)
{
    /*!< 通知发生了页面也换，可能不是活动状态，将关闭刷新功能 */
    uint32_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_NOTIFY_PAGE_ACT)
    {
        lv_event_send(obj, LV_EVENT_NOTIFY_UPDATE, (void *)-1);
    }
    if (code == LV_EVENT_NOTIFY_PAGE_CHANGE)
    {
        // lv_event_send(obj, LV_EVENT_NOTIFY_UPDATE, (void *)-1);
    }
}

static void lv_event_value_update(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    uint32_t mask = (uint32_t)lv_event_get_param(e);
    lv_obj_t *sub_obj = NULL;
//
//    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_2);
//    if (sub_obj && (mask & (1 << SEN_CONFIG_RS485_2)))
//        if (g_sensor_param.dev_config[0].en)
//        {
//            lv_obj_add_state(sub_obj, LV_STATE_CHECKED);
//        }
//        else
//        {
//            lv_obj_clear_flag(sub_obj, LV_STATE_CHECKED);
//        }
//    }
}
static void lv_event_clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_DEBUG comp_id = (enum UI_COMP_DEBUG)((uint32_t)lv_event_get_user_data(e));
    if (obj && code == LV_EVENT_CLICKED)
    {
//        char buff[32];
//        switch (comp_id)
//        {
//        case SEN_CONFIG_DEL: /*!< 增删传感器配置 */
//        {
//            // sensor_del(&g_sensor_param, sel);
//            lv_event_send(lv_obj_get_parent(obj), LV_EVENT_NOTIFY_UPDATE, (void *)(1 << SEN_CONFIG_TABLE));
//        }
//        break;
//        }
    }
}
static void child_event_value_changed(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_DEBUG comp_id = (uint32_t)lv_event_get_user_data(e);
    if (obj && code == LV_EVENT_NOTIFY_UPDATE)
    {
        uint32_t chn_sel = 0;

//        switch (comp_id)
//        {
//        case SEN_CONFIG_RS485_2:
//            g_sensor_param.dev_config[0].en = NULL != (lv_obj_get_state(obj) & LV_STATE_CHECKED);
//            break;
//        }
    }
}
