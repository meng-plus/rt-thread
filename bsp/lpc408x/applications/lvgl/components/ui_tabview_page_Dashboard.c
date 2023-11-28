#include "ui_tabview_page_Dashboard.h"
#include "ui_comp_page_Dashboard_ctl.h"
#include "ui_comp_page_Dashboard_sensor.h"
#include "ui_comp.h"
#include "system_var.h"
#include "thread_DTS.h"
#include "stdio.h"
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
    if (NULL == pdts || NULL == obj)
        return;

    static uint8_t offline_last = 1;
    if (offline_last != (pdts->offline > 5))
    { /*!< 掉线刷新显示 */
        offline_last = (pdts->offline > 5);
        _lv_event_child_notify(obj, LV_EVENT_NOTIFY_UPDATE, NULL);
    }
    if (0 == pdts->offline)
    {
        uint32_t chn_sel = pdts->sel_chn;
        if (NULL == pdts->update_flag)
            return;

        dts_data_t *pdata = NULL;
        thread_DTS_control(TH_DTS_GET_DATA, &pdata);
        if (NULL == pdata)
            return;

        lv_obj_t *pchart = ui_comp_get_child(obj, DASHBOARD_CHART);
        if (NULL == pchart)
            return;

        lv_chart_series_t *pser = lv_chart_get_series_next(pchart, NULL);
        if (NULL == pser)
            return;

        if (0 == pdata->channel[chn_sel].partition)
        {
            lv_chart_set_all_value(pchart, pser, 0);
            return;
        }

        lv_chart_set_x_start_point(pchart, pser, 0);
        lv_chart_set_point_count(pchart, pdata->channel[chn_sel].partition); // 设置X轴范围（示例中设置为0到100）
        lv_chart_set_range(pchart, LV_CHART_AXIS_PRIMARY_X, 0, pdata->channel[chn_sel].partition);

        for (size_t i = 0; i < pdata->channel[chn_sel].partition; i++)
        {
            lv_chart_set_next_value(pchart, pser, (lv_coord_t)(pdata->partition[chn_sel][i].temp_max_real * 0.01 - 200));
            // lv_chart_set_next_value2(pchart, pser, i, pdata->partition[chn_sel][i].temp_max_real);
        }
    }
}
static lv_chart_cursor_t *cursor;
static void event_cb(lv_event_t *e)
{
    static int32_t last_id = -1;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        last_id = lv_chart_get_pressed_point(obj);
        if (last_id != LV_CHART_POINT_NONE)
        {
            lv_chart_set_cursor_point(obj, cursor, NULL, last_id);
        }
    }
    else if (code == LV_EVENT_DRAW_PART_END)
    {
        lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
        if (!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_CURSOR))
            return;
        if (dsc->p1 == NULL || dsc->p2 == NULL || dsc->p1->y != dsc->p2->y || last_id < 0)
            return;
        lv_chart_series_t *pser = lv_chart_get_series_next(obj, NULL);
        //        lv_coord_t *data_array = lv_chart_get_y_array(obj, pser);
        //        lv_coord_t v = data_array[last_id];
        char buf[16];

        dts_data_t *pdata = NULL;
        thread_DTS_control(TH_DTS_GET_DATA, &pdata);
        thread_dts_t *pdts = g_var_work.dts;
        snprintf(buf, sizeof(buf), "%dm\r\n%.2f", pdata->partition[pdts->sel_chn][last_id].loc_max_real, pdata->partition[pdts->sel_chn][last_id].temp_max_real * 0.01 - 200);

        lv_point_t size;
        lv_txt_get_size(&size, buf, LV_FONT_DEFAULT, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

        lv_area_t a;
        a.y2 = dsc->p1->y - 5;
        a.y1 = a.y2 - size.y - 10;
        a.x1 = dsc->p1->x + 10;
        a.x2 = a.x1 + size.x + 10;

        lv_draw_rect_dsc_t draw_rect_dsc;
        lv_draw_rect_dsc_init(&draw_rect_dsc);
        draw_rect_dsc.bg_color = lv_palette_main(LV_PALETTE_BLUE);
        draw_rect_dsc.radius = 3;

        lv_draw_rect(dsc->draw_ctx, &draw_rect_dsc, &a);

        lv_draw_label_dsc_t draw_label_dsc;
        lv_draw_label_dsc_init(&draw_label_dsc);
        draw_label_dsc.color = lv_color_white();
        a.x1 += 5;
        a.x2 -= 5;
        a.y1 += 5;
        a.y2 -= 5;
        lv_draw_label(dsc->draw_ctx, &draw_label_dsc, &a, buf, NULL);
    }
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
        timer = lv_timer_create(time_update, 1000, obj);
        _lv_event_child_notify(obj, LV_EVENT_NOTIFY_PAGE_ACT, NULL);
    }
    if (code == LV_EVENT_NOTIFY_PAGE_CHANGE)
    {
        if (timer)
        {
            lv_timer_del(timer);
            timer = 0;
        }
        _lv_event_child_notify(obj, LV_EVENT_NOTIFY_PAGE_CHANGE, NULL);
    }
}
static void lv_event_value_update(lv_event_t *e)
{
    uint32_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_NOTIFY_UPDATE)
    {
        _lv_event_child_notify(obj, LV_EVENT_VALUE_CHANGED, NULL);
    }
}
lv_obj_t *ui_tabview_page_dashboard_create(lv_obj_t *tableview)
{
    lv_obj_t *obj;
    obj = lv_tabview_add_tab(tableview, TEXT_DASHBOARD);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_CHANGE, NULL);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_ACT, NULL);
    lv_obj_add_event_cb(obj, lv_event_value_update, LV_EVENT_NOTIFY_UPDATE, NULL); /*!< 通知刷新后，初始化所有控件 */

    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), 16, 100, LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), 16, 50, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);
    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);

    lv_obj_t *ui_Chart1 = lv_chart_create(obj);
    lv_obj_set_grid_cell(ui_Chart1, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_set_style_pad_top(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(obj, 40, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_chart_set_type(ui_Chart1, LV_CHART_TYPE_LINE);
    // lv_chart_set_type(ui_Chart1, LV_CHART_TYPE_SCATTER);

    // lv_chart_set_point_count(ui_Chart1, 125);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_X, 10, 5, 5, 2, true, 50);
    lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
    // lv_chart_set_axis_tick(ui_Chart1, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 5, 2, true, 50);
    lv_obj_set_style_size(ui_Chart1, 0, LV_PART_INDICATOR);
    lv_obj_add_event_cb(ui_Chart1, event_cb, LV_EVENT_ALL, NULL);
    lv_chart_series_t *ui_Chart1_series_1 = lv_chart_add_series(ui_Chart1, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    cursor = lv_chart_add_cursor(ui_Chart1, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

    lv_obj_t *slider_x;
    slider_x = lv_slider_create(obj);
    lv_obj_set_grid_cell(slider_x, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_slider_set_range(slider_x, LV_IMG_ZOOM_NONE, LV_IMG_ZOOM_NONE * 10);
    lv_obj_add_event_cb(slider_x, slider_x_event_cb, LV_EVENT_VALUE_CHANGED, ui_Chart1);
    // lv_obj_set_size(slider_x, 200, 10);
    lv_obj_align_to(slider_x, ui_Chart1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    lv_obj_t *slider_y = lv_slider_create(obj);
    lv_obj_set_grid_cell(slider_y, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_slider_set_range(slider_y, LV_IMG_ZOOM_NONE, LV_IMG_ZOOM_NONE * 10);
    lv_obj_add_event_cb(slider_y, slider_y_event_cb, LV_EVENT_VALUE_CHANGED, ui_Chart1);
    // lv_obj_set_size(slider_y, 10, 150);
    lv_obj_align_to(slider_y, ui_Chart1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    lv_obj_t *sensor_obj = ui_Dashboard_sensor_create(obj);
    lv_obj_set_grid_cell(sensor_obj, LV_GRID_ALIGN_START, 2, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 3);
    lv_obj_t *dashboard_ctl = ui_Dashboard_ctl_create(obj);
    lv_obj_set_grid_cell(dashboard_ctl, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 1);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * DASHBOARD_NUM);
    lv_memset_00(children, sizeof(lv_obj_t *) * DASHBOARD_NUM);
    children[DASHBOARD_CHART] = ui_Chart1;
    children[DASHBOARD_SLIDER_X] = slider_x;
    children[DASHBOARD_SLIDER_Y] = slider_y;
    children[DASHBOARD_CTL] = dashboard_ctl;
    children[DASHBOARD_SENSOR] = sensor_obj;

    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);

    return obj;
}
