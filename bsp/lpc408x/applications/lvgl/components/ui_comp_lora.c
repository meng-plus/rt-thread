#include "ui_comp_lora.h"
#include "ui_comp.h"
#include "system_var.h"
#include "ui_P01main.h"
#include "CommLora.h"
#include "stdio.h"
static void time_update(lv_timer_t *ptime)
{
    lv_obj_t *obj = (lv_obj_t *)ptime->user_data;
    lv_event_send(obj, LV_EVENT_NOTIFY_UPDATE, (void *)(0x01 << LORA_LABEL_MSG));
}
static void lv_event_notify_page(lv_event_t *e)
{
    static lv_timer_t *timer = 0;
    /*!< 通知发生了页面也换，可能不是活动状态，将关闭刷新功能 */
    uint32_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_NOTIFY_PAGE_ACT)
    {
        lv_event_send(obj, LV_EVENT_NOTIFY_UPDATE, (void *)-1);
        /** 定时刷新的 任务*/
        if (timer == 0)
        {
            timer = lv_timer_create(time_update, 500, obj);
        }
    }
    if (code == LV_EVENT_NOTIFY_PAGE_CHANGE)
    {

        if (timer)
        {
            lv_timer_del(timer);
            timer = 0;
        }
    }
}

static void lv_event_value_update(lv_event_t *e)
{
    uint32_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    uint32_t mask = (uint32_t)lv_event_get_param(e);
    if (code == LV_EVENT_NOTIFY_UPDATE)
    {
        lv_obj_t *sub_obj = ui_comp_get_child(obj, LORA_LABLE_EN);
        if (sub_obj && (mask & (0x01 << LORA_LABLE_EN)))
        {
            lv_label_set_text(sub_obj, loradata ? TEXT_SEND : TEXT_REC);
            sub_obj = ui_comp_get_child(obj, LORA_SW_SEND);
            if (loradata)
            {
                lv_obj_add_state(sub_obj, LV_STATE_CHECKED);
            }
            else
            {
                lv_obj_clear_state(sub_obj, LV_STATE_CHECKED);
            }
        }
        sub_obj = ui_comp_get_child(obj, LORA_LABEL_MSG);
        if (sub_obj && (mask & (0x01 << LORA_LABEL_MSG)))
        {
            if (LoraDipflag)
            {
                LoraDipflag = 0;
                WIFI_DATA Wifi_Data_Temp;
                lv_memcpy(&Wifi_Data_Temp, LoraBuf, sizeof(Wifi_Data_Temp));
                char strbuff[512];
                char buf[32];
                uint16_t offset = 0;

                time_t nTime = time(NULL);
                struct tm *curTime = localtime(&nTime);
                offset += rt_sprintf(strbuff, "%4d-%02d-%02d %02d:%02d:%02d\r\n", curTime->tm_year + 1900, curTime->tm_mon + 1, curTime->tm_mday,
                                     curTime->tm_hour, curTime->tm_min, curTime->tm_sec);
                if (Wifi_Data_Temp.workMode == 0)
                    strcpy(buf, TEXT_IDLE);
                else if (Wifi_Data_Temp.workMode == 0x0d)
                    strcpy(buf, "排水");
                else if (Wifi_Data_Temp.workMode == 0x01)
                    strcpy(buf, "预抽");
                else if (Wifi_Data_Temp.workMode == 0x04)
                    strcpy(buf, "置换");
                else if (Wifi_Data_Temp.workMode == 0x08)
                    strcpy(buf, TEXT_MEAS);
                else if (Wifi_Data_Temp.workMode == 0x20)
                    strcpy(buf, "反吹");
                else
                    strcpy(buf, "      ");
                offset += snprintf(strbuff + offset, sizeof(strbuff) - offset, "%s\t%s", TEXT_WORK_MODE, buf);

                strbuff[offset] = '\0';
                lv_label_set_text(sub_obj, strbuff);
            }
            else
            {
                // lv_label_set_text(sub_obj, "");
            }
        }
    }
}

static void btn_event_clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_LORA comp_id = (enum UI_COMP_LORA)((uint32_t)lv_event_get_user_data(e));
    if (obj && code == LV_EVENT_CLICKED)
    {
        switch (comp_id)
        {
        case LORA_BTN_EXIT:
        {
            lv_obj_t *taloraview = ui_comp_get_child(lv_scr_act(), SCREEN_P01_TABLE);
            lv_obj_clear_flag(taloraview, LV_OBJ_FLAG_HIDDEN);

            lv_obj_t *lora_page = lv_obj_get_parent(obj);
            lv_obj_add_flag(lora_page, LV_OBJ_FLAG_HIDDEN);
        }
        break;
        }
    }
}
static void child_event_value_changed(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_LORA comp_id = (enum UI_COMP_LORA)((uint32_t)lv_event_get_user_data(e));
    if (obj && code == LV_EVENT_VALUE_CHANGED)
    {
        switch (comp_id)
        {
        case LORA_SW_SEND:
        {
            if (lv_obj_has_state(obj, LV_STATE_CHECKED))
            {
                loradata = 1;
            }
            else
            {
                loradata = 0;
            }
            lv_event_send(lv_obj_get_parent(obj), LV_EVENT_NOTIFY_UPDATE, (void *)(0x01 << LORA_LABLE_EN));
        }
        break;
        }
    }
}

lv_obj_t *ui_comp_lora_set_create(lv_obj_t *parent)
{
    lv_obj_t *obj;
    obj = lv_obj_create(parent);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_CHANGE, NULL);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_ACT, NULL);
    lv_obj_add_event_cb(obj, lv_event_value_update, LV_EVENT_NOTIFY_UPDATE, NULL); /*!< 通知刷新后，初始化所有控件 */

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(8), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);
    lv_obj_set_style_text_font(obj, &ui_font_simfang16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *label;

    /////////////////////////////////////////////////////////
    lv_obj_t *label_msg = lv_label_create(obj);
    lv_obj_set_grid_cell(label_msg, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 4);
    lv_obj_align(label_msg, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(label_msg, "");

    lv_obj_t *label_sw = lv_label_create(obj);
    lv_label_set_text(label_sw, TEXT_REC);
    lv_obj_set_grid_cell(label_sw, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_CENTER, 5, 1);
    lv_obj_align(label_sw, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *btn_sw = lv_switch_create(obj);
    lv_obj_set_grid_cell(btn_sw, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(btn_sw, child_event_value_changed, LV_EVENT_VALUE_CHANGED, (void *)LORA_SW_SEND);

    lv_obj_t *btn_return = lv_btn_create(obj);
    lv_obj_set_grid_cell(btn_return, LV_GRID_ALIGN_STRETCH, 1, 2,
                         LV_GRID_ALIGN_STRETCH, 6, 1);
    label = lv_label_create(btn_return);
    lv_label_set_text(label, TEXT_EXIT);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btn_return, btn_event_clicked, LV_EVENT_CLICKED, (void *)LORA_BTN_EXIT);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * LORA_NUM);
    lv_memset_00(children, sizeof(lv_obj_t *) * LORA_NUM);
    children[LORA_LABLE_EN] = label_sw;
    children[LORA_SW_SEND] = btn_sw;
    children[LORA_LABEL_MSG] = label_msg;

    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);

    return obj;
}
