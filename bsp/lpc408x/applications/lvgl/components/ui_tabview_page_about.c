#include "ui_tabview_page_about.h"
#include "ui_comp.h"
#include "system_var.h"

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
    // lv_obj_t *sub_obj = NULL;
    //
    //     sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_2);
    //     if (sub_obj && (mask & (1 << SEN_CONFIG_RS485_2)))
    //         if (g_sensor_param.dev_config[0].en)
    //         {
    //             lv_obj_add_state(sub_obj, LV_STATE_CHECKED);
    //         }
    //         else
    //         {
    //             lv_obj_clear_flag(sub_obj, LV_STATE_CHECKED);
    //         }
    //     }
}

lv_obj_t *ui_tabview_page_about_create(lv_obj_t *tableview)
{
    lv_obj_t *obj;
    obj = lv_tabview_add_tab(tableview, TEXT_ABOUT);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_CHANGE, NULL);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_ACT, NULL);
    lv_obj_add_event_cb(obj, lv_event_value_update, LV_EVENT_NOTIFY_UPDATE, NULL); /*!< 通知刷新后，初始化所有控件 */

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);

    lv_obj_t *lable_SOFT_VER = lv_label_create(obj);
    lv_obj_set_grid_cell(lable_SOFT_VER, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_label_set_text_fmt(lable_SOFT_VER, "%s:%s", TEXT_SOFT_VER, SOFT_VER);
    lv_obj_align(lable_SOFT_VER, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_t *lable_HF_VER = lv_label_create(obj);
    lv_obj_set_grid_cell(lable_HF_VER, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);

    lv_label_set_text_fmt(lable_HF_VER, "%s:%s", TEXT_HARDWARE_VER, HARD_VER);
    lv_obj_align(lable_HF_VER, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * ABOUT_NUM);
    lv_memset_00(children, sizeof(lv_obj_t *) * ABOUT_NUM);

    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);

    return obj;
}
