#include "ui_tabview_page_sen_config.h"
#include "ui_comp.h"
#include "system_var.h"
#include "ui_tabview_page_sen_config_event.h"
lv_obj_t *ui_tabview_page_sen_config_create(lv_obj_t *tableview)
{
    lv_obj_t *obj;
    obj = lv_tabview_add_tab(tableview, "config");
    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);
    char *baud_str = "4800\n9600\n19200\n38400\n115200";
    static const char *btnm_map[] = {"1", "2", "3", "\n",
                                     "4", "5", "6", "\n",
                                     "7", "8", "9", "\n",
                                     LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""};
    lv_obj_t *btnm = lv_btnmatrix_create(lv_layer_top());

    lv_obj_add_flag(btnm, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(btnm, 400, 300);
    lv_obj_align(btnm, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_btnmatrix_set_one_checked(btnm, 1);
    lv_obj_clear_flag(btnm, LV_OBJ_FLAG_CLICK_FOCUSABLE); /*To keep the text area focused on button clicks*/
    lv_btnmatrix_set_map(btnm, btnm_map);

    lv_obj_t *rs485_2_en_obj = lv_checkbox_create(obj);
    lv_obj_set_grid_cell(rs485_2_en_obj, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);
    lv_checkbox_set_text(rs485_2_en_obj, "RS485_2");

    /*!< addr set */
    /*Create the one-line mode text area*/
    lv_obj_t *rs485_2_addr_obj = lv_textarea_create(obj);
    lv_obj_set_grid_cell(rs485_2_addr_obj, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);
    lv_textarea_set_one_line(rs485_2_addr_obj, true);
    lv_textarea_set_password_mode(rs485_2_addr_obj, false);
    lv_textarea_set_placeholder_text(rs485_2_addr_obj, "slave addr");
    lv_textarea_set_accepted_chars(rs485_2_addr_obj, "0123456789");
    lv_textarea_set_max_length(rs485_2_addr_obj, 4);
    lv_obj_clear_flag(rs485_2_addr_obj, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_add_event_cb(rs485_2_addr_obj, ta_event_cb, LV_EVENT_ALL, btnm);
    /*!< baud */
    lv_obj_t *rs485_2_baud_obj = lv_dropdown_create(obj);
    lv_obj_set_grid_cell(rs485_2_baud_obj, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_align(rs485_2_baud_obj, LV_ALIGN_TOP_MID, 0, 20);
    {

        lv_dropdown_set_options(rs485_2_baud_obj, baud_str);
    }

    lv_obj_t *rs485_3_en_obj = lv_checkbox_create(obj);
    lv_obj_set_grid_cell(rs485_3_en_obj, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_CENTER, 1, 1);
    lv_checkbox_set_text(rs485_3_en_obj, "RS485_3");

    /*!< addr set */
    /*Create the one-line mode text area*/
    lv_obj_t *rs485_3_addr_obj = lv_textarea_create(obj);
    lv_obj_set_grid_cell(rs485_3_addr_obj, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_CENTER, 1, 1);
    lv_textarea_set_one_line(rs485_3_addr_obj, true);
    lv_textarea_set_password_mode(rs485_3_addr_obj, false);
    lv_textarea_set_placeholder_text(rs485_3_addr_obj, "slave addr");
    lv_textarea_set_accepted_chars(rs485_3_addr_obj, "0123456789");
    lv_textarea_set_max_length(rs485_3_addr_obj, 4);
    lv_obj_clear_flag(rs485_3_addr_obj, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_add_event_cb(rs485_3_addr_obj, ta_event_cb, LV_EVENT_ALL, btnm);
    /*!< baud */
    lv_obj_t *rs485_3_baud_obj = lv_dropdown_create(obj);
    lv_obj_set_grid_cell(rs485_3_baud_obj, LV_GRID_ALIGN_STRETCH, 2, 1,
                         LV_GRID_ALIGN_CENTER, 1, 1);
    lv_obj_align(rs485_3_baud_obj, LV_ALIGN_TOP_MID, 0, 20);
    {

        lv_dropdown_set_options(rs485_3_baud_obj, baud_str);
    }
    lv_obj_t *btn_restore = lv_btn_create(obj);
    lv_obj_add_event_cb(btn_restore, lv_event_clicked, LV_EVENT_CLICKED, (void *)SEN_CONFIG_RESTORE);
    lv_obj_set_grid_cell(btn_restore, LV_GRID_ALIGN_STRETCH, 4, 1,
                         LV_GRID_ALIGN_CENTER, 5, 1);
    lv_obj_align(btn_restore, LV_ALIGN_CENTER, 0, 40);
    // lv_obj_add_flag(btn_restore, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_height(btn_restore, LV_SIZE_CONTENT);
    lv_obj_t *label = lv_label_create(btn_restore);
    lv_label_set_text(label, "restore");
    lv_obj_center(label);

    lv_obj_t *btn_save = lv_btn_create(obj);
    lv_obj_add_event_cb(btn_save, lv_event_clicked, LV_EVENT_CLICKED, (void *)SEN_CONFIG_SAVE);
    lv_obj_set_grid_cell(btn_save, LV_GRID_ALIGN_STRETCH, 5, 1,
                         LV_GRID_ALIGN_CENTER, 5, 1);
    lv_obj_align(btn_save, LV_ALIGN_CENTER, 0, 40);
    // lv_obj_add_flag(btn_save, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_height(btn_save, LV_SIZE_CONTENT);

    label = lv_label_create(btn_save);
    lv_label_set_text(label, "save");
    lv_obj_center(label);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * SEN_CONFIG_NUM);
    children[SEN_CONFIG_RS485_2] = rs485_2_en_obj;
    children[SEN_CONFIG_RS485_2_ADDR] = rs485_2_addr_obj;
    children[SEN_CONFIG_RS485_2_BAUD] = rs485_2_baud_obj;
    children[SEN_CONFIG_RS485_3] = rs485_3_en_obj;
    children[SEN_CONFIG_RS485_3_ADDR] = rs485_3_addr_obj;
    children[SEN_CONFIG_RS485_3_BAUD] = rs485_3_baud_obj;
    children[SEN_CONFIG_RESTORE] = btn_restore;
    children[SEN_CONFIG_SAVE] = btn_save;
    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);
    lv_obj_add_event_cb(obj, lv_event_value_changed, LV_EVENT_VALUE_CHANGED, NULL); /*!< 通知刷新后，初始化所有控件 */

    for (size_t i = 0; i < SEN_CONFIG_NUM; i++)
    {
        if (children[i])
            lv_obj_add_event_cb(children[i], child_event_value_changed, LV_EVENT_VALUE_CHANGED, (void *)i);
    }

    //_lv_event_child_notify(obj, LV_EVENT_VALUE_CHANGED, NULL);

    return obj;
}
