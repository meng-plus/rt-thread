#include "ui_tabview_page_Dashboard.h"
#include "ui_comp_page_Dashboard_ctl.h"
#include "ui_comp.h"
#include "system_var.h"
#include "thread_DTS.h"
static void slider_x_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    int32_t v = lv_slider_get_value(obj);
    lv_obj_t *chart = lv_event_get_user_data(e);
    lv_chart_set_zoom_x(chart, v);
}

static void slider_y_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    int32_t v = lv_slider_get_value(obj);
    lv_obj_t *chart = lv_event_get_user_data(e);
    lv_chart_set_zoom_y(chart, v);
}
static void time_update(lv_timer_t *ptime)
{
    lv_obj_t *obj = (lv_obj_t *)ptime->user_data;
    thread_dts_t *pdts = g_var_work.dts;

    if (pdts)
    {
        static uint8_t offline_last = 1;

        if (offline_last != pdts->offline)
        { /*!< 掉线刷新显示 */
            offline_last = pdts->offline;
            _lv_event_child_notify(obj, LV_EVENT_VALUE_CHANGED, NULL);
        }
        else if (0 == pdts->offline)
        {
            uint32_t chn_sel = g_var_work.dts->sel_chn;
            dts_data_t *pdata = NULL;
            if (g_var_work.dts->update_flag)
            {

                thread_DTS_control(TH_DTH_GET_DATA, &pdata);
                if (NULL == pdata)
                    return;
            }
            lv_obj_t *pchart = ui_comp_get_child(obj, DASHBOARD_CHART);
            if (NULL == pchart)
            {
                return;
            }
            lv_chart_series_t *pser = lv_chart_get_series_next(pchart, NULL);
            if (NULL == pser)
                return;
            lv_chart_set_x_start_point(pchart, pser, 0);
            for (size_t i = 0; i < pdata->channel[chn_sel].partition; i++)
            {
                lv_chart_set_next_value(pchart, pser, pdata->partition[chn_sel][i].temp_max_real);
            }
        }
    }
}
lv_obj_t *ui_tabview_page_dashboard_create(lv_obj_t *tableview)
{
    lv_obj_t *obj;
    obj = lv_tabview_add_tab(tableview, "dashboard");

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), 16, 100, LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), 16, 50, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);

    lv_obj_t *ui_Chart1 = lv_chart_create(obj);
    lv_obj_set_grid_cell(ui_Chart1, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    lv_chart_set_type(ui_Chart1, LV_CHART_TYPE_LINE);

    lv_chart_set_point_count(ui_Chart1, 125);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_X, 10, 5, 5, 2, true, 50);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 5, 2, true, 25);
    lv_chart_series_t *ui_Chart1_series_1 = lv_chart_add_series(ui_Chart1, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    // static lv_coord_t ui_Chart1_series_1_array[] = {0, 10, 20, 40, 80, 80, 40, 20, 10, 0};
    // lv_chart_set_ext_y_array(ui_Chart1, ui_Chart1_series_1, ui_Chart1_series_1_array);
    // lv_chart_set_next_value(ui_Chart1, ui_Chart1_series_1, 99);

    lv_obj_t *slider_x;
    slider_x = lv_slider_create(obj);
    lv_obj_set_grid_cell(slider_x, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_slider_set_range(slider_x, LV_IMG_ZOOM_NONE, LV_IMG_ZOOM_NONE * 2);
    lv_obj_add_event_cb(slider_x, slider_x_event_cb, LV_EVENT_VALUE_CHANGED, ui_Chart1);
    // lv_obj_set_size(slider_x, 200, 10);
    lv_obj_align_to(slider_x, ui_Chart1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    lv_obj_t *slider_y = lv_slider_create(obj);
    lv_obj_set_grid_cell(slider_y, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_slider_set_range(slider_y, LV_IMG_ZOOM_NONE, LV_IMG_ZOOM_NONE * 2);
    lv_obj_add_event_cb(slider_y, slider_y_event_cb, LV_EVENT_VALUE_CHANGED, ui_Chart1);
    // lv_obj_set_size(slider_y, 10, 150);
    lv_obj_align_to(slider_y, ui_Chart1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    lv_obj_t *label_obj = lv_label_create(obj);
    lv_obj_set_grid_cell(label_obj, LV_GRID_ALIGN_START, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_label_set_text_fmt(label_obj, "hello word");
    lv_obj_t *dashboard_ctl = ui_Dashboard_ctl_create(obj);
    lv_obj_set_grid_cell(dashboard_ctl, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 1);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * DASHBOARD_NUM);
    children[DASHBOARD_CHART] = ui_Chart1;
    children[DASHBOARD_SLIDER_X] = slider_x;
    children[DASHBOARD_SLIDER_Y] = slider_y;
    children[DASHBOARD_CTL] = dashboard_ctl;

    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);

    _lv_event_child_notify(obj, LV_EVENT_VALUE_CHANGED, NULL);

    /** 定时刷新的 任务*/
    lv_timer_create(time_update, 1000, obj);

    return obj;
}
