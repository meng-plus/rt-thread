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
#include "user_gpio.h"

static void time_update(lv_timer_t *ptime)
{
    lv_obj_t *obj = (lv_obj_t *)ptime->user_data;
    lv_event_send(obj, LV_EVENT_NOTIFY_UPDATE, (void *)0x3F);
}
static void lv_event_notify_page(lv_event_t *e)
{
    static lv_timer_t *timer = 0;
    /*!< 通知发生了页面也换，可能不是活动状态，将关闭刷新功能 */
    uint32_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_NOTIFY_PAGE_ACT)
    {
        lv_event_send(obj, LV_EVENT_NOTIFY_UPDATE, (void *)-1); /** 定时刷新的 任务*/
        if (timer == 0)
        {
            timer = lv_timer_create(time_update, 1000, obj);
        }
    }
    if (code == LV_EVENT_NOTIFY_PAGE_CHANGE)
    {
        // lv_event_send(obj, LV_EVENT_NOTIFY_UPDATE, (void *)-1);
        if (timer)
        {
            lv_timer_del(timer);
            timer = 0;
        }
    }
}

static void lv_event_value_update(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    uint32_t mask = (uint32_t)lv_event_get_param(e);
    lv_obj_t *sub_obj = NULL;
    for (size_t i = DEBUG_INPUT_S1; i <= DEBUG_INPUT_S6; i++)
    {
        sub_obj = ui_comp_get_child(obj, i);
        if (sub_obj && (mask & (1 << i)))
            if (gpio_read(INPUT_S1 + i))
            {
                lv_led_off(sub_obj);
            }
            else
            {
                lv_led_on(sub_obj);
            }
    }
    for (size_t i = DEBUG_OUT_A; i <= DEBUG_PUMP_AIR; i++)
    {
        sub_obj = ui_comp_get_child(obj, i);
        if (sub_obj && (mask & (1 << i)))
        {
            if (gpio_get(OUT_A + i - DEBUG_OUT_A))
            {
                lv_label_set_text_fmt(sub_obj, "%s %d %s", TEXT_OUTPUT, i - DEBUG_OUT_A + 1, TEXT_ON);
            }
            else
            {
                lv_label_set_text_fmt(sub_obj, "%s %d %s", TEXT_OUTPUT, i - DEBUG_OUT_A + 1, TEXT_OFF);
            }
        }
    }
}

static void btn_event_clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_DEBUG comp_id = (enum UI_COMP_DEBUG)((uint32_t)lv_event_get_user_data(e));
    if (obj && code == LV_EVENT_CLICKED)
    {
        switch (comp_id)
        {
        case DEBUG_OUT_A:
            gpio_set(OUT_A, !gpio_get(OUT_A));
            break;
        case DEBUG_OUT_B:
            gpio_set(OUT_B, !gpio_get(OUT_B));
            break;
        case DEBUG_OUT_C:
            gpio_set(OUT_C, !gpio_get(OUT_C));
            break;
        case DEBUG_PUMP_AIR:
            gpio_set(PUMP_AIR, !gpio_get(PUMP_AIR));
            break;
        }
        lv_event_send(lv_obj_get_parent(obj), LV_EVENT_NOTIFY_UPDATE, (void *)(0x01 << comp_id));
    }
}
static void child_event_value_changed(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_DEBUG comp_id = (uint32_t)lv_event_get_user_data(e);
    if (obj && code == LV_EVENT_VALUE_CHANGED)
    {
        // uint32_t chn_sel = 0;
        // lv_event_send(lv_obj_get_parent(obj), LV_EVENT_NOTIFY_UPDATE, (void *)(1 << comp_id));
    }
}
