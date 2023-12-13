#include "ui_comp_status.h"
#include "ui_comp.h"
#include "system_var.h"
#include <time.h>
#include "error_code.h"
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "rtdbg.h"

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
    }
}

static void lv_event_value_update(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    // 更新设备状态
    lv_obj_t *table = ui_comp_get_child(obj, STATUS_TABLE);
    error_code_t *current_error = error_code_get_list();
    uint8_t error_cnt = 1;
    while (current_error != NULL)
    {
        if (error_code_is_error(current_error))
        {
            // 打印出错的信息
            lv_table_set_cell_value_fmt(table, error_cnt, 0, "%d", error_cnt);
            lv_table_set_cell_value_fmt(table, error_cnt, 1, "%d", current_error->code);
            struct tm *curTime = localtime(&current_error->err_st);
            lv_table_set_cell_value_fmt(table, error_cnt, 2, "%02d-%02d %02d:%02d:%02d", curTime->tm_mon + 1, curTime->tm_mday,
                                        curTime->tm_hour, curTime->tm_min, curTime->tm_sec);
            lv_table_set_cell_value_fmt(table, error_cnt, 3, "%d", current_error->err_cnt);
            lv_table_set_cell_value(table, error_cnt, 4, current_error->description);

            error_cnt++;
        }
        current_error = error_code_get_next_list(current_error);
    }
    lv_table_set_row_cnt(table, error_cnt);
}
