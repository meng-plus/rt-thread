#include "ui_comp_status.h"
#include "ui_comp.h"
#include "system_var.h"

lv_obj_t *ui_comp_status_create(lv_obj_t *parent)
{
    lv_obj_t *obj;
    if (parent == NULL)
    {
        parent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
        LV_ASSERT_MALLOC(parent);
        lv_obj_class_init_obj(parent);
        lv_obj_clear_flag(parent, LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_set_size(parent, LV_PCT(100), LV_PCT(100));
    }
    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(8), LV_GRID_FR(2), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(parent, col_dsc, row_dsc);

    lv_obj_t *ui_title = lv_label_create(obj);
    lv_obj_set_grid_cell(ui_title, LV_GRID_ALIGN_START, 0, 1,
                         LV_GRID_ALIGN_CENTER, 0, 2);
    lv_obj_set_width(ui_title, 429);
    lv_obj_set_height(ui_title, 30);
    lv_obj_set_align(ui_title, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_title, "");
    lv_obj_set_style_text_color(ui_title, lv_color_hex(0x00B2B2), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_title, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_title, &ui_font_simhei24, (lv_part_t)LV_PART_MAIN | LV_STATE_DEFAULT);

 
    lv_obj_center(obj);
    return obj;
}
