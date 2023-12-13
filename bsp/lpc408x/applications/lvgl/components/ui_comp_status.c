#include "ui_comp_status.h"
#include "ui_comp.h"
#include "system_var.h"
#include <time.h>

#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "rtdbg.h"

#include "ui_comp_status_event.h"
lv_obj_t *win = NULL;
static void event_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    uint8_t id = lv_obj_get_index(obj);
    if (3 == id)
    {
        lv_obj_del(win);
    }
}
static void time_update(struct _lv_timer_t *ptime)
{
    lv_event_send(ptime->user_data, LV_EVENT_NOTIFY_UPDATE, (void *)-1);
}
static void del_component_timer_event_cb(lv_event_t *e)
{
    lv_timer_t *time_data_update = lv_event_get_user_data(e);
    lv_timer_del(time_data_update);
}
static void draw_part_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_param(e);
    /*If the cells are drawn...*/
    if (dsc->part == LV_PART_ITEMS)
    {
        uint32_t row = dsc->id / lv_table_get_col_cnt(obj);
        uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);

        /*Make the texts in the first cell center aligned*/
        if (row == 0)
        {
            dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
            dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_BLUE), dsc->rect_dsc->bg_color, LV_OPA_20);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
        /*In the first column align the texts to the right*/
        else if (col == 0)
        {
            dsc->label_dsc->align = LV_TEXT_ALIGN_RIGHT;
        }

        /*MAke every 2nd row grayish*/
        if ((row != 0 && row % 2) == 0)
        {
            dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_GREY), dsc->rect_dsc->bg_color, LV_OPA_10);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
    }
}

lv_obj_t *ui_comp_status_create(lv_obj_t *obj)
{
    win = lv_win_create(lv_scr_act(), 40);
    lv_obj_set_style_text_font(win, &ui_font_simfang16, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(win, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_CHANGE, NULL);
    lv_obj_add_event_cb(win, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_ACT, NULL);
    lv_obj_add_event_cb(win, lv_event_value_update, LV_EVENT_NOTIFY_UPDATE, NULL); /*!< 通知刷新后，初始化所有控件 */

    lv_obj_t *btn;
    btn = lv_win_add_btn(win, LV_SYMBOL_LEFT, 40);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_win_add_title(win, TEXT_STATUS_MSG);

    btn = lv_win_add_btn(win, LV_SYMBOL_RIGHT, 40);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 60);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t *cont = lv_win_get_content(win); /*Content can be added here*/
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(8), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_center(cont);

    lv_obj_t *tab_obj = lv_table_create(cont);
    lv_obj_set_grid_cell(tab_obj, LV_GRID_ALIGN_STRETCH, 0, 5,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_align(tab_obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_outline_width(tab_obj, 2, LV_PART_ITEMS);
    lv_obj_set_scroll_dir(tab_obj, LV_DIR_VER);
    lv_obj_add_event_cb(tab_obj, draw_part_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);

    lv_table_set_row_cnt(tab_obj, 2); /*Not required but avoids a lot of memory reallocation lv_table_set_set_value*/
    lv_table_set_col_cnt(tab_obj, 4);

    /*Don't make the cell pressed, we will draw something different in the event*/
    lv_obj_remove_style(tab_obj, NULL, LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_table_add_cell_ctrl(tab_obj, 1, 0, LV_TABLE_CELL_CTRL_CUSTOM_1);
    lv_table_set_cell_value(tab_obj, 0, 0, TEXT_INDEX);
    lv_table_set_cell_value(tab_obj, 0, 1, TEXT_ERROR_CODE);
    lv_table_set_cell_value(tab_obj, 0, 2, TEXT_TIME);
    lv_table_set_cell_value(tab_obj, 0, 3, TEXT_ERR_CNT);
    lv_table_set_cell_value(tab_obj, 0, 4, TEXT_ERR_MSG);
    lv_table_set_col_width(tab_obj, 0, 16 * 4);
    lv_table_set_col_width(tab_obj, 1, 16 * 6);
    lv_table_set_col_width(tab_obj, 2, 16 * 12);
    lv_table_set_col_width(tab_obj, 3, 16 * 8);
    lv_table_set_col_width(tab_obj, 4, 16 * 18); // 16 * 20

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * STATUS_NUM);
    lv_memset_00(children, sizeof(lv_obj_t *) * STATUS_NUM);
    children[STATUS_TABLE] = tab_obj;

    lv_obj_add_event_cb(win, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(win, del_component_child_event_cb, LV_EVENT_DELETE, children);

    lv_timer_t *time_data_update = lv_timer_create(time_update, 1000, win);

    /** add user event */
    lv_obj_add_event_cb(win, del_component_timer_event_cb, LV_EVENT_DELETE, time_data_update);

    return obj;
}
