#include "ui_comp_status.h"
#include "ui_comp.h"
#include "system_var.h"

#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "rtdbg.h"
lv_obj_t *win = NULL;
static void event_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    uint8_t id = lv_obj_get_index(obj);
    LOG_I("Button %d clicked", id);
    if (3 == id)
    {
        lv_obj_del(win);
    }
}

lv_obj_t *ui_comp_status_create(lv_obj_t *obj)
{
    win = lv_win_create(lv_scr_act(), 40);
    lv_obj_set_style_text_font(win, &ui_font_simfang16, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *btn;
    btn = lv_win_add_btn(win, LV_SYMBOL_LEFT, 40);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_win_add_title(win, TEXT_STATUS_MSG);

    btn = lv_win_add_btn(win, LV_SYMBOL_RIGHT, 40);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 60);
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t *cont = lv_win_get_content(win); /*Content can be added here*/
    lv_obj_t *label = lv_label_create(cont);
    static lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
    lv_obj_center(cont);

    return obj;
}
