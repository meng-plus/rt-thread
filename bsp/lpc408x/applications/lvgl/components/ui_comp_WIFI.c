#include "ui_comp_WIFI.h"
#include "ui_comp.h"
#include "system_var.h"
#include "ui_P01main.h"

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
}

static void btn_event_clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_WIFI comp_id = (enum UI_COMP_WIFI)((uint32_t)lv_event_get_user_data(e));
    if (obj && code == LV_EVENT_CLICKED)
    {
        switch (comp_id)
        {
        case WIFI_BTN_EXIT:
        {
            lv_obj_t *tableview = ui_comp_get_child(lv_scr_act(), SCREEN_P01_TABLE);
            lv_obj_clear_flag(tableview, LV_OBJ_FLAG_HIDDEN);

            lv_obj_t *wifi_page = lv_obj_get_parent(obj);
            lv_obj_add_flag(wifi_page, LV_OBJ_FLAG_HIDDEN);
        }
        break;
        case WIFI_BTN_GET_NAME:

            break;
        case WIFI_BTN_GET_IP:

            break;
        case WIFI_EDIT_ID:
        {
            lv_obj_t *kb = ui_comp_get_child(lv_obj_get_parent(obj), WIFI_KB);
            if (kb)
            {
                lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
                lv_keyboard_set_textarea(kb, obj);
            }
        }
        break;
        }
    }
}
static void child_event_child_changed(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_WIFI comp_id = (enum UI_COMP_WIFI)((uint32_t)lv_event_get_user_data(e));
    if (obj && code == LV_EVENT_CHILD_CHANGED)
    {
        switch (comp_id)
        {
        case WIFI_SW_ENABLE:
        {
            lv_obj_t *tableview = ui_comp_get_child(lv_scr_act(), SCREEN_P01_TABLE);
            lv_obj_clear_flag(tableview, LV_OBJ_FLAG_HIDDEN);

            lv_obj_t *wifi_page = lv_obj_get_parent(obj);
            lv_obj_add_flag(wifi_page, LV_OBJ_FLAG_HIDDEN);
        }
        break;
        case WIFI_SW_SEND_MSG:

            break;
        }
    }
}
static void ta_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_WIFI comp_id = (enum UI_COMP_WIFI)((uint32_t)lv_event_get_user_data(e));
    if (obj &&
        ((code == LV_EVENT_DEFOCUSED) ||
         (code == LV_EVENT_READY)))
    {
        if (comp_id == WIFI_EDIT_ID)
        {
            lv_obj_t *kb = ui_comp_get_child(lv_obj_get_parent(obj), WIFI_KB);
            if (kb)
            {
                lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
                lv_keyboard_set_textarea(kb, obj);
            }
        }
    }
}

lv_obj_t *ui_comp_wifi_set_create(lv_obj_t *parent)
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
    lv_obj_t *lable;

    /////////////////////////////////////////////////////////
    lv_obj_t *lable_msg = lv_label_create(obj);
    lv_obj_set_grid_cell(lable_msg, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 4);
    lv_obj_align(lable_msg, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(lable_msg, "");

    lable = lv_label_create(obj);
    lv_label_set_text(lable, TEXT_OPEN);
    lv_obj_set_grid_cell(lable, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_align(lable, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t *btn_sw = lv_switch_create(obj);
    lv_obj_set_grid_cell(btn_sw, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(btn_sw, child_event_child_changed, LV_EVENT_CHILD_CHANGED, (void *)WIFI_SW_ENABLE);

    lv_obj_t *btn_getName = lv_btn_create(obj);
    lv_obj_set_grid_cell(btn_getName, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 3, 1);
    lable = lv_label_create(btn_getName);
    lv_label_set_text(lable, TEXT_GET_NAME);
    lv_obj_align(lable, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btn_getName, btn_event_clicked, LV_EVENT_CLICKED, (void *)WIFI_BTN_GET_NAME);

    lv_obj_t *btn_getIp = lv_btn_create(obj);
    lv_obj_set_grid_cell(btn_getIp, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 3, 1);
    lable = lv_label_create(btn_getIp);
    lv_label_set_text(lable, TEXT_GET_IP);
    lv_obj_align(lable, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btn_getIp, btn_event_clicked, LV_EVENT_CLICKED, (void *)WIFI_BTN_GET_IP);

    lable = lv_label_create(obj);
    lv_label_set_text(lable, TEXT_SEND_MSG);
    lv_obj_set_grid_cell(lable, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_CENTER, 4, 1);
    lv_obj_align(lable, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *btn_sw_sendMsg = lv_switch_create(obj);
    lv_obj_set_grid_cell(btn_sw_sendMsg, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 4, 1);
    lv_obj_add_event_cb(btn_sw_sendMsg, child_event_child_changed, LV_EVENT_CHILD_CHANGED, (void *)WIFI_SW_SEND_MSG);

    lv_obj_t *edit_id = lv_textarea_create(obj);
    lv_obj_set_grid_cell(edit_id, LV_GRID_ALIGN_STRETCH, 1, 2,
                         LV_GRID_ALIGN_STRETCH, 5, 1);
    lv_obj_add_event_cb(edit_id, btn_event_clicked, LV_EVENT_CLICKED, (void *)WIFI_EDIT_ID);
    lv_obj_add_event_cb(edit_id, ta_event, LV_EVENT_DEFOCUSED, (void *)WIFI_EDIT_ID);
    lv_obj_add_event_cb(edit_id, ta_event, LV_EVENT_READY, (void *)WIFI_EDIT_ID);
    lv_textarea_set_one_line(edit_id, 1);
    lv_textarea_set_accepted_chars(edit_id, "1234567890");
    lv_textarea_set_max_length(edit_id, 3);
    lv_textarea_set_placeholder_text(edit_id, "ID");
    lv_obj_set_scroll_dir(edit_id, LV_DIR_NONE);
    lv_obj_t *kb = lv_keyboard_create(obj);
    lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
    lv_obj_set_style_text_font(kb, LV_FONT_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_keyboard_set_textarea(kb, obj);
    lv_obj_set_size(kb, 400, 300);
    lv_obj_set_pos(kb, 0, 0);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t *btn_return = lv_btn_create(obj);
    lv_obj_set_grid_cell(btn_return, LV_GRID_ALIGN_STRETCH, 1, 2,
                         LV_GRID_ALIGN_STRETCH, 6, 1);
    lable = lv_label_create(btn_return);
    lv_label_set_text(lable, TEXT_EXIT);
    lv_obj_align(lable, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btn_return, btn_event_clicked, LV_EVENT_CLICKED, (void *)WIFI_BTN_EXIT);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * WIFI_NUM);
    lv_memset_00(children, sizeof(lv_obj_t *) * WIFI_NUM);
    children[WIFI_EDIT_ID] = edit_id;
    children[WIFI_LABLE_MSG] = lable_msg;
    children[WIFI_KB] = kb;

    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);

    return obj;
}
