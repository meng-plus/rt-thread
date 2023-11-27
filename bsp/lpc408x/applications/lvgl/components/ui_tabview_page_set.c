#include "ui_tabview_page_set.h"
#include "ui_comp.h"
#include "system_var.h"
#include "ui_comp_WIFI.h"
#include "ui_comp_ble.h"
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
    enum UI_COMP_SET comp_id = (enum UI_COMP_SET)((uint32_t)lv_event_get_user_data(e));
    if (obj && code == LV_EVENT_CLICKED)
    {
        lv_obj_t *sub_obj = NULL;
        switch (comp_id)
        {
        case SET_BTN_WIFI:
            sub_obj = ui_comp_get_child(lv_obj_get_parent(obj), SET_PAGE_WIFI);
            break;
        case SET_BTN_BLE:
            sub_obj = ui_comp_get_child(lv_obj_get_parent(obj), SET_PAGE_BLE);
        default:
            break;
        }
        lv_obj_t *tableview = ui_comp_get_child(lv_scr_act(), SCREEN_P01_TABLE);
        if (tableview && sub_obj)
        {
            lv_obj_add_flag(tableview, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(sub_obj, LV_OBJ_FLAG_HIDDEN);
            /*!< 通知page 当前处于活动状态 */
            lv_event_send(sub_obj, LV_EVENT_NOTIFY_PAGE_CHANGE, NULL);
            lv_event_send(sub_obj, LV_EVENT_NOTIFY_PAGE_ACT, NULL);
        }
    }
}
lv_obj_t *ui_tabview_page_set_create(lv_obj_t *tableview)
{
    lv_obj_t *obj;
    obj = lv_tabview_add_tab(tableview, TEXT_SET);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_CHANGE, NULL);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_ACT, NULL);
    lv_obj_add_event_cb(obj, lv_event_value_update, LV_EVENT_NOTIFY_UPDATE, NULL); /*!< 通知刷新后，初始化所有控件 */

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);

    lv_obj_t *wifi_obj = lv_btn_create(obj);
    lv_obj_set_grid_cell(wifi_obj, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_t *lable = lv_label_create(wifi_obj);
    lv_obj_align(lable, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(lable, TEXT_WIFI_SET);
    lv_obj_t *obj_page_wifi = ui_comp_wifi_set_create(lv_obj_get_parent(tableview));
    lv_obj_add_flag(obj_page_wifi, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_grid_cell(obj_page_wifi, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);

    lv_obj_add_event_cb(wifi_obj, btn_event_clicked, LV_EVENT_CLICKED, (void *)SET_BTN_WIFI);

    lv_obj_t *ble_obj = lv_btn_create(obj);
    lv_obj_set_grid_cell(ble_obj, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    lable = lv_label_create(ble_obj);
    lv_obj_align(lable, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(lable, TEXT_BLUETOOTH_SET);
    lv_obj_add_event_cb(ble_obj, btn_event_clicked, LV_EVENT_CLICKED, (void *)SET_BTN_BLE);

    lv_obj_t *obj_page_ble = ui_comp_ble_set_create(lv_obj_get_parent(tableview));
    lv_obj_add_flag(obj_page_ble, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_grid_cell(obj_page_ble, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * SET_NUM);
    lv_memset_00(children, sizeof(lv_obj_t *) * SET_NUM);
    children[SET_PAGE_BLE] = obj_page_ble;
    children[SET_PAGE_WIFI] = obj_page_wifi;

    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);

    return obj;
}
