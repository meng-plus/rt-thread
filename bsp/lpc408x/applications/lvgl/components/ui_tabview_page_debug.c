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

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), 16, 100, LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), 16, 50, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * DEBUG_NUM);
    lv_memset_00(children, sizeof(lv_obj_t *) * DEBUG_NUM);
    // children[DASHBOARD_CHART] = ui_Chart1;
    // children[DASHBOARD_SLIDER_X] = slider_x;
    // children[DASHBOARD_SLIDER_Y] = slider_y;
    // children[DASHBOARD_CTL] = dashboard_ctl;

    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);

    //_lv_event_child_notify(obj, LV_EVENT_VALUE_CHANGED, NULL);
    for (size_t i = 0; i < DEBUG_NUM; i++)
    {
        if (children[i])
            lv_obj_add_event_cb(children[i], child_event_value_changed, LV_EVENT_VALUE_CHANGED, (void *)i);
    }
    /** 定时刷新的 任务*/
    // lv_timer_create(time_update, 1000, obj);

    return obj;
}
