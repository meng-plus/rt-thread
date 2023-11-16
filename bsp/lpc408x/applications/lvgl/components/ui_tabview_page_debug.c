#include "ui_tabview_page_debug.h"
#include "ui_comp.h"
#include "system_var.h"
#include "ui_tabview_page_debug_event.h"

lv_obj_t *ui_tabview_page_debug_create(lv_obj_t *tableview)
{
    lv_obj_t *obj;
    obj = lv_tabview_add_tab(tableview, "debug");
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_CHANGE, NULL);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_ACT, NULL);
    lv_obj_add_event_cb(obj, lv_event_value_update, LV_EVENT_NOTIFY_UPDATE, NULL); /*!< 通知刷新后，初始化所有控件 */

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);

    lv_obj_t *lable_Waring = lv_label_create(obj);
    lv_obj_set_grid_cell(lable_Waring, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_label_set_text(lable_Waring, "status");
    lv_obj_align(lable_Waring, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_t *btn_Waring = lv_btn_create(obj);
    lv_obj_set_grid_cell(btn_Waring, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_flag(btn_Waring, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * DEBUG_NUM);
    lv_memset_00(children, sizeof(lv_obj_t *) * DEBUG_NUM);
    children[DEBUG_LABLE_WARING] = lable_Waring;
    children[DEBUG_LABLE_CTRL1] = obj;
    children[DEBUG_LABLE_CTRL2] = obj;
    children[DEBUG_LABLE_CTRL3] = obj;
    children[DEBUG_BT_WARING] = btn_Waring;
    children[DEBUG_BT_CTRL1] = obj;
    children[DEBUG_BT_CTRL2] = obj;
    children[DEBUG_BT_CTRL3] = obj;

    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);

    for (size_t i = DEBUG_BT_WARING; i < DEBUG_NUM; i++)
    {
        if (children[i])
            lv_obj_add_event_cb(children[i], child_event_value_changed, LV_EVENT_VALUE_CHANGED, (void *)i);
    }
    return obj;
}
