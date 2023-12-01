#include "ui_tabview_page_sen_config.h"
#include "ui_comp.h"
#include "system_var.h"
#include "ui_tabview_page_sen_config_event.h"
lv_obj_t *ui_tabview_page_sen_config_create(lv_obj_t *tableview)
{
    lv_obj_t *obj;
    lv_obj_t *label;
    obj = lv_tabview_add_tab(tableview, TEXT_CONFIG);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_CHANGE, NULL);
    lv_obj_add_event_cb(obj, lv_event_notify_page, LV_EVENT_NOTIFY_PAGE_ACT, NULL);
    lv_obj_add_event_cb(obj, lv_event_value_update, LV_EVENT_NOTIFY_UPDATE, NULL); /*!< 通知刷新后，初始化所有控件 */

    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(obj, col_dsc, row_dsc);
    lv_obj_center(obj);

#if 0
    char *baud_str = "4800\n9600\n19200\n38400\n115200";
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
    lv_textarea_set_placeholder_text(rs485_2_addr_obj, TEXT_SLAVE_ADDR);
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
    lv_obj_set_style_text_font(rs485_2_baud_obj, LV_FONT_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
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
    lv_textarea_set_placeholder_text(rs485_3_addr_obj, TEXT_SLAVE_ADDR);
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
    lv_obj_set_style_text_font(rs485_3_baud_obj, LV_FONT_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
#endif
    /**
     * @addtogroup 传感器表格
     * @{
     */
    lv_obj_t *tab_obj = lv_table_create(obj);
    lv_obj_set_grid_cell(tab_obj, LV_GRID_ALIGN_STRETCH, 0, 5,
                         LV_GRID_ALIGN_STRETCH, 2, 4);
    // lv_table_set_col_width(tab_obj, 0,); // 设置第二列自适应宽度
    lv_obj_add_event_cb(tab_obj, table_event_pressed, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_align(tab_obj, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *btn_del = lv_btn_create(obj);
    lv_obj_add_event_cb(btn_del, lv_event_clicked, LV_EVENT_CLICKED, (void *)SEN_CONFIG_DEL);
    lv_obj_set_grid_cell(btn_del, LV_GRID_ALIGN_STRETCH, 5, 1,
                         LV_GRID_ALIGN_CENTER, 2, 1);
    lv_obj_align(btn_del, LV_ALIGN_CENTER, 0, 40);
    lv_obj_set_height(btn_del, LV_SIZE_CONTENT);
    label = lv_label_create(btn_del);
    lv_label_set_text(label, TEXT_DEL);
    lv_obj_center(label);

    lv_obj_t *btn_new = lv_btn_create(obj);
    lv_obj_add_event_cb(btn_new, lv_event_clicked, LV_EVENT_CLICKED, (void *)SEN_CONFIG_NEW);
    lv_obj_set_grid_cell(btn_new, LV_GRID_ALIGN_STRETCH, 5, 1,
                         LV_GRID_ALIGN_CENTER, 3, 1);
    lv_obj_align(btn_new, LV_ALIGN_CENTER, 0, 40);
    lv_obj_set_height(btn_new, LV_SIZE_CONTENT);

    label = lv_label_create(btn_new);
    lv_label_set_text(label, TEXT_NEW);
    lv_obj_center(label);
    /**
     * @}
     */

    /**
     * @brief 按键保存
     *
     */
    lv_obj_t *btn_restore = lv_btn_create(obj);
    lv_obj_add_event_cb(btn_restore, lv_event_clicked, LV_EVENT_CLICKED, (void *)SEN_CONFIG_RESTORE);
    lv_obj_set_grid_cell(btn_restore, LV_GRID_ALIGN_STRETCH, 5, 1,
                         LV_GRID_ALIGN_CENTER, 4, 1);
    lv_obj_align(btn_restore, LV_ALIGN_CENTER, 0, 40);
    // lv_obj_add_flag(btn_restore, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_height(btn_restore, LV_SIZE_CONTENT);
    label = lv_label_create(btn_restore);
    lv_label_set_text(label, TEXT_RESTORE);
    lv_obj_center(label);

    lv_obj_t *btn_save = lv_btn_create(obj);
    lv_obj_add_event_cb(btn_save, lv_event_clicked, LV_EVENT_CLICKED, (void *)SEN_CONFIG_SAVE);
    lv_obj_set_grid_cell(btn_save, LV_GRID_ALIGN_STRETCH, 5, 1,
                         LV_GRID_ALIGN_CENTER, 5, 1);
    lv_obj_align(btn_save, LV_ALIGN_CENTER, 0, 40);
    // lv_obj_add_flag(btn_save, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_height(btn_save, LV_SIZE_CONTENT);

    label = lv_label_create(btn_save);
    lv_label_set_text(label, TEXT_SAVE);
    lv_obj_center(label);

    lv_obj_t *kb = ui_edit_create(lv_layer_top());
    lv_obj_set_size(kb, 800, 480);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    ui_edit_add_event(kb, table_edit_event_cb, LV_EVENT_READY, &s_edit);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * SEN_CONFIG_NUM);
    lv_memset_00(children, sizeof(lv_obj_t *) * SEN_CONFIG_NUM);
    //    children[SEN_CONFIG_RS485_2] = rs485_2_en_obj;
    //    children[SEN_CONFIG_RS485_2_ADDR] = rs485_2_addr_obj;
    //    children[SEN_CONFIG_RS485_2_BAUD] = rs485_2_baud_obj;
    //    children[SEN_CONFIG_RS485_3] = rs485_3_en_obj;
    //    children[SEN_CONFIG_RS485_3_ADDR] = rs485_3_addr_obj;
    //    children[SEN_CONFIG_RS485_3_BAUD] = rs485_3_baud_obj;
    children[SEN_CONFIG_TABLE] = tab_obj;
    children[SEN_CONFIG_DEL] = btn_del;
    children[SEN_CONFIG_NEW] = btn_new;
    children[SEN_CONFIG_RESTORE] = btn_restore;
    children[SEN_CONFIG_SAVE] = btn_save;
    children[SEN_CONFIG_KB] = kb;
    lv_obj_add_event_cb(obj, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(obj, del_component_child_event_cb, LV_EVENT_DELETE, children);

    for (size_t i = 0; i < SEN_CONFIG_NUM; i++)
    {
        if (children[i])
            lv_obj_add_event_cb(children[i], child_event_value_changed, LV_EVENT_VALUE_CHANGED, (void *)i);
    }
    return obj;
}
