#include "ui_tabview_page_debug.h"
#include "ui_comp.h"
#include "system_var.h"
#include "ui_tabview_page_debug_event.h"

lv_obj_t *ui_tabview_page_debug_create(lv_obj_t *tableview)
{
    lv_obj_t *obj;
    obj = lv_tabview_add_tab(tableview, TEXT_DEBUG);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_CHANGE, NULL);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_ACT, NULL);
    lv_obj_add_event_cb(obj, lv_event_value_update, LV_EVENT_NOTIFY_UPDATE, NULL); /*!< 通知刷新后，初始化所有控件 */

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * DEBUG_NUM);
    lv_memset_00(children, sizeof(lv_obj_t *) * DEBUG_NUM);

    for (size_t i = DEBUG_INPUT_S1; i <= DEBUG_INPUT_S6; i++)
    {
        lv_obj_t *lable_input = lv_label_create(obj);
        lv_obj_set_grid_cell(lable_input, LV_GRID_ALIGN_END, 0, 1,
                             LV_GRID_ALIGN_STRETCH, i, 1);
        lv_label_set_text_fmt(lable_input, "%s S%d", TEXT_INPUT, i + 1);
        lv_obj_align(lable_input, LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_t *led_input = lv_led_create(obj);
        lv_obj_set_grid_cell(led_input, LV_GRID_ALIGN_START, 1, 1,
                             LV_GRID_ALIGN_CENTER, i, 1);
        lv_led_set_brightness(led_input, 255);
        lv_obj_set_style_shadow_spread(led_input, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_led_set_color(led_input, lv_palette_main(LV_PALETTE_GREEN));
        lv_obj_set_size(led_input, 32, 32);
        children[i] = led_input;
    }
    for (size_t i = DEBUG_OUT_A; i < DEBUG_PUMP_AIR; i++)
    {
        uint8_t idx = i - DEBUG_OUT_A;
        lv_obj_t *btn_out = lv_btn_create(obj);
        lv_obj_set_grid_cell(btn_out, LV_GRID_ALIGN_STRETCH, 2, 1,
                             LV_GRID_ALIGN_STRETCH, idx, 1);
        lv_obj_add_event_cb(btn_out, btn_event_clicked, LV_EVENT_CLICKED, (void *)i);

        lv_obj_t *lable_output = lv_label_create(btn_out);
        lv_obj_align(lable_output, LV_ALIGN_CENTER, 0, 0);
        lv_label_set_text_fmt(lable_output, "%s %d %s", TEXT_OUTPUT, i + 1, TEXT_OFF);

        children[i] = lable_output;
    }

    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);

    return obj;
}
