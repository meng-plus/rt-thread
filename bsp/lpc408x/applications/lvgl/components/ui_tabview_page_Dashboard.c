#include "ui_tabview_page_Dashboard.h"

lv_obj_t *ui_tabview_page_dashboard_create(lv_obj_t *tableview)
{
    lv_obj_t *obj;
    obj = lv_tabview_add_tab(tableview, "dashboard");

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), 50, 50, LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), 25, 25, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);

    lv_obj_t *ui_Chart1 = lv_chart_create(obj);
    lv_obj_set_grid_cell(ui_Chart1, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    //lv_obj_set_size(ui_Chart1, lv_pct(60), lv_pct(60));
    lv_chart_set_type(ui_Chart1, LV_CHART_TYPE_LINE);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_X, 10, 5, 5, 2, true, 50);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 5, 2, true, 25);
    lv_chart_series_t *ui_Chart1_series_1 = lv_chart_add_series(ui_Chart1, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    static lv_coord_t ui_Chart1_series_1_array[] = {0, 10, 20, 40, 80, 80, 40, 20, 10, 0};
    lv_chart_set_ext_y_array(ui_Chart1, ui_Chart1_series_1, ui_Chart1_series_1_array);
    // lv_chart_add_series(ui_Chart1, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_next_value(ui_Chart1, ui_Chart1_series_1, 99);

    lv_obj_t *label_obj = lv_label_create(obj);
    lv_obj_set_grid_cell(label_obj, LV_GRID_ALIGN_CENTER, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_label_set_text_fmt(label_obj, "hello word");
    lv_obj_t *label_obj2 = lv_label_create(obj);
    lv_obj_set_grid_cell(label_obj2, LV_GRID_ALIGN_START, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_label_set_text_fmt(label_obj2, "hello word2");
    return obj;
}
