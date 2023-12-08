/**
 * @file ui_tabview_page_sen_config_event.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 指定页面专用的事件处理函数，不可他用
 * @version 0.1
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "ui_tabview_page_sen_config.h"
#include "ui_comp.h"
#include "system_var.h"
#include <stdlib.h>
#include <stdio.h>
#include "sensor_var.h"
#include "ui_comp_edit.h"
static const uint32_t baud_array[] = {4800, 9600, 19200, 38400, 115200};
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
        // lv_event_send(obj, LV_EVENT_NOTIFY_UPDATE, (void *)-1);
    }
}

static void lv_event_value_update(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    uint32_t mask = (uint32_t)lv_event_get_param(e);
    lv_obj_t *sub_obj = NULL;

    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_2);
    if (sub_obj && (mask & (1 << SEN_CONFIG_RS485_2)))
        if (g_sensor_param.dev_config[0].en)
        {
            lv_obj_add_state(sub_obj, LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_clear_state(sub_obj, LV_STATE_CHECKED);
        }

    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_3);
    if (sub_obj && (mask & (1 << SEN_CONFIG_RS485_3)))
        if (g_sensor_param.dev_config[1].en)
        {
            lv_obj_add_state(sub_obj, LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_clear_state(sub_obj, LV_STATE_CHECKED);
        }

    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_2_BAUD);
    if (sub_obj && (mask & (1 << SEN_CONFIG_RS485_2_BAUD)))
    {
        size_t i;
        for (i = 0; i < sizeof(baud_array) / sizeof(baud_array[0]); i++)
        {
            if (g_sensor_param.dev_config[0].baud == baud_array[i])
            {
                lv_dropdown_set_selected(sub_obj, i);
            }
        }
    }
    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_3_BAUD);
    if (sub_obj && (mask & (1 << SEN_CONFIG_RS485_3_BAUD)))
    {
        size_t i;
        for (i = 0; i < sizeof(baud_array) / sizeof(baud_array[0]); i++)
        {
            if (g_sensor_param.dev_config[1].baud == baud_array[i])
            {
                lv_dropdown_set_selected(sub_obj, i);
            }
        }
    }
    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_2_ADDR);
    if (sub_obj && (mask & (1 << SEN_CONFIG_RS485_2_ADDR)))
    {
        char strbuff[6];
        rt_sprintf(strbuff, "%d", g_sensor_param.dev_config[0].addr);
        lv_label_set_text(lv_textarea_get_label(sub_obj), strbuff);
    }
    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_RS485_3_ADDR);
    if (sub_obj && (mask & (1 << SEN_CONFIG_RS485_3_ADDR)))
    {
        char strbuff[6];
        rt_sprintf(strbuff, "%d", g_sensor_param.dev_config[1].addr);
        lv_label_set_text(lv_textarea_get_label(sub_obj), strbuff);
    }

    /**
     * @brief sensor table init
     *
     */

    sub_obj = ui_comp_get_child(obj, SEN_CONFIG_TABLE);
    if (sub_obj && (mask & (1 << SEN_CONFIG_TABLE)))
    {
        lv_table_set_col_cnt(sub_obj, 4);
        if (g_sensor_param.sensor_num == 0)
        {
            lv_table_set_row_cnt(sub_obj, 1);
        }
        else
        {
            lv_table_set_row_cnt(sub_obj, g_sensor_param.sensor_num + 1);

            for (size_t i = 0; i < g_sensor_param.sensor_num; i++)
            {
                sensor_config_t *psen = &g_sensor_param.sen_config[i];
                if (psen->type < SEN_NUM && rom_sensor_var[psen->type].fullname)
                {
                    lv_table_set_cell_value_fmt(sub_obj, i + 1, 0, rom_sensor_var[psen->type].fullname);
                }
                else
                {
                    lv_table_set_cell_value_fmt(sub_obj, i + 1, 0, "unknown");
                }
                lv_table_set_cell_value_fmt(sub_obj, i + 1, 1, "%d", psen->chn);
                lv_table_set_cell_value_fmt(sub_obj, i + 1, 2, "%d", psen->addr);
                lv_table_set_cell_value_fmt(sub_obj, i + 1, 3, "%d", psen->dev_addr);
            }
        }
        lv_table_set_cell_value(sub_obj, 0, 0, TEXT_NAME);
        lv_table_set_cell_value(sub_obj, 0, 1, TEXT_CHN);
        lv_table_set_cell_value(sub_obj, 0, 2, TEXT_ADDR);
        lv_table_set_cell_value(sub_obj, 0, 3, TEXT_UART_ADDR);
    }
}
static void lv_event_clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_SEN_CONFIG comp_id = (enum UI_COMP_SEN_CONFIG)(uint32_t)lv_event_get_user_data(e);
    if (obj && code == LV_EVENT_CLICKED)
    {
        char buff[32];
        switch (comp_id)
        {
        case SEN_CONFIG_DEL: /*!< 增删传感器配置 */
        {
            if (g_sensor_param.sensor_num > 1)
            {
                sensor_del(&g_sensor_param, g_sensor_param.sensor_num - 1);
            }
            lv_event_send(lv_obj_get_parent(obj), LV_EVENT_NOTIFY_UPDATE, (void *)(1 << SEN_CONFIG_TABLE));
        }
        break;
        case SEN_CONFIG_NEW:
        {
            uint8_t ret = sensor_new(&g_sensor_param);
            lv_event_send(lv_obj_get_parent(obj), LV_EVENT_NOTIFY_UPDATE, (void *)(1 << SEN_CONFIG_TABLE));
            if (ret == 0)
            {
                lv_obj_t *mbox1 = lv_msgbox_create(NULL, TEXT_ERROR, TEXT_ERROR_CREATE, NULL, true);
                lv_obj_set_style_text_font(mbox1, &ui_font_simfang16, LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_obj_center(mbox1);
            }
        }
        break;
        case SEN_CONFIG_RESTORE:
        {
            var_reload(&g_sensor_param);
            lv_event_send(lv_obj_get_parent(obj), LV_EVENT_NOTIFY_UPDATE, (void *)-1);
            lv_obj_t *mbox1 = lv_msgbox_create(NULL, TEXT_RESTORE, TEXT_RESTORE_OK, NULL, true);
            lv_obj_set_style_text_font(mbox1, &ui_font_simfang16, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_center(mbox1);
        }
        break;
        case SEN_CONFIG_SAVE:
        {
            int8_t ret = var_save(&g_sensor_param);
            if (ret == 0)
            {
                lv_obj_t *mbox1 = lv_msgbox_create(NULL, TEXT_SAVE, TEXT_SAVE_OK, NULL, true);
                lv_obj_set_style_text_font(mbox1, &ui_font_simfang16, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_center(mbox1);
            }
            else
            {

                rt_sprintf(buff, "%s (0x%02x)", TEXT_ERROR_CODE, ret);
                lv_obj_t *mbox1 = lv_msgbox_create(NULL, TEXT_SAVE, buff, NULL, true);
                lv_obj_set_style_text_font(mbox1, &ui_font_simfang16, LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_obj_center(mbox1);
            }
        }
        break;
        default:
            break;
        }
    }
}
static void child_event_value_changed(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    enum UI_COMP_SEN_CONFIG comp_id = (enum UI_COMP_SEN_CONFIG)(uint32_t)lv_event_get_user_data(e);
    if (obj && code == LV_EVENT_VALUE_CHANGED)
    {
        uint32_t chn_sel = 0;

        switch (comp_id)
        {
        case SEN_CONFIG_RS485_2:
            g_sensor_param.dev_config[0].en = NULL != (lv_obj_get_state(obj) & LV_STATE_CHECKED);
            break;
        case SEN_CONFIG_RS485_2_ADDR:

            g_sensor_param.dev_config[0].addr = atoi(lv_textarea_get_text(obj));

            break;
        case SEN_CONFIG_RS485_2_BAUD:
            chn_sel = lv_dropdown_get_selected(obj);
            if (chn_sel < sizeof(baud_array) / sizeof(baud_array[0]))
            {
                g_sensor_param.dev_config[0].baud = baud_array[chn_sel];
            }
            break;
        case SEN_CONFIG_RS485_3:
            g_sensor_param.dev_config[1].en = NULL != (lv_obj_get_state(obj) & LV_STATE_CHECKED);
            break;
        case SEN_CONFIG_RS485_3_ADDR:
            g_sensor_param.dev_config[1].addr = atoi(lv_textarea_get_text(obj));
            break;
        case SEN_CONFIG_RS485_3_BAUD:
            chn_sel = lv_dropdown_get_selected(obj);
            if (chn_sel < sizeof(baud_array) / sizeof(baud_array[0]))
            {
                g_sensor_param.dev_config[1].baud = baud_array[chn_sel];
            }
            break;
        case SEN_CONFIG_RESTORE:
            var_reload(&g_sensor_param);
            lv_event_send(lv_obj_get_parent(obj), LV_EVENT_NOTIFY_UPDATE, (void *)-1);
            break;
        case SEN_CONFIG_SAVE:
            var_save(&g_sensor_param);
            break;
        default:
            break;
        }
    }
}

typedef struct table_edit
{
    uint16_t row;
    uint16_t col;
    lv_obj_t *obj;
} table_edit_t;

static void table_edit_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    table_edit_t *pedit = (table_edit_t *)lv_event_get_user_data(e);

    if (pedit && (pedit->row > 0) && (pedit->row <= g_sensor_param.sensor_num))
    {
        uint8_t idx = pedit->row - 1;
        uint8_t value = 0;
        const char *string = NULL;
        if (pedit->col)
            string = lv_label_get_text(lv_textarea_get_label(obj));

        switch (pedit->col)
        {
        case 0x00: /*!< type */
        {
            char buf[16];
            lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
            g_sensor_param.sen_config[idx].type = atoi(buf);
            lv_obj_add_flag(lv_obj_get_parent(obj), LV_OBJ_FLAG_HIDDEN);
        }

        break;
        case 0x01: /*!< chn */
            value = atoi(string);
            if (value < 32)
                g_sensor_param.sen_config[idx].chn = value;
            break;
        case 0x02: /*!< addr */
            value = atoi(string);
            g_sensor_param.sen_config[idx].addr = value;
            break;
        case 0x03: /*!< dev_addr */
            value = atoi(string);
            if (value < 10)
                g_sensor_param.sen_config[idx].dev_addr = value;
            break;
        default:
            break;
        }
        LOG_D("[%d,%d]:%s", pedit->row, pedit->col, string);
        lv_event_send(lv_obj_get_parent(pedit->obj), LV_EVENT_NOTIFY_UPDATE, (void *)(1 << SEN_CONFIG_TABLE));
    }
}
static table_edit_t s_edit;
static void table_event_pressed(lv_event_t *e)
{
    s_edit.obj = lv_event_get_target(e);
    lv_table_get_selected_cell(s_edit.obj, &s_edit.row, &s_edit.col);
    const char *string = lv_table_get_cell_value(s_edit.obj, s_edit.row, s_edit.col);

    if ((s_edit.row > 0) && (s_edit.row <= g_sensor_param.sensor_num))
    {
        if (s_edit.col == 0)
        {
            static lv_obj_t *dd = NULL;
            if (NULL == dd)
            {
                lv_obj_t *parent = NULL;
                if (parent == NULL)
                {
                    parent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
                    LV_ASSERT_MALLOC(parent);
                    lv_obj_class_init_obj(parent);
                    lv_obj_clear_flag(parent, LV_OBJ_FLAG_IGNORE_LAYOUT);
                    lv_obj_set_size(parent, LV_PCT(100), LV_PCT(100));
                }

                dd = lv_dropdown_create(parent);
                lv_obj_set_style_text_font(dd, &ui_font_simfang16, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_t *list = lv_dropdown_get_list(dd);
                lv_obj_set_style_text_font(list, &ui_font_simfang16, LV_PART_MAIN | LV_STATE_DEFAULT);

                lv_dropdown_clear_options(dd);
                for (size_t i = 0; i < SEN_NUM; i++)
                {
                    if (rom_sensor_var[i].fullname)
                    {
                        char strbuff[16];
                        rt_sprintf(strbuff, "%d-%s", i, rom_sensor_var[i].fullname);
                        lv_dropdown_add_option(dd, strbuff, LV_DROPDOWN_POS_LAST);
                    }
                }
                lv_obj_align(dd, LV_ALIGN_CENTER, 0, 20);
                lv_obj_add_event_cb(dd, table_edit_event_cb, LV_EVENT_VALUE_CHANGED, &s_edit);
                lv_obj_add_event_cb(dd, table_edit_event_cb, LV_EVENT_DEFOCUSED, NULL);
            }
            else
            {
                lv_obj_clear_flag(lv_obj_get_parent(dd), LV_OBJ_FLAG_HIDDEN);
            }
            lv_dropdown_open(dd);
        }
        else if (s_edit.col > 0 && s_edit.col < 4)
        {
            lv_obj_t *sub_obj = NULL;
            sub_obj = ui_comp_get_child(lv_obj_get_parent(s_edit.obj), SEN_CONFIG_KB);
            ui_edit_pop_with_default(sub_obj, string);
        }
    }
}
