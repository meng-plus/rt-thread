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
                if (psen->type < SEN_NUM && rom_sensor_var[psen->type].symbolname)
                {
                    lv_table_set_cell_value_fmt(sub_obj, i + 1, 0, rom_sensor_var[psen->type].symbolname);
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
        lv_table_set_cell_value(sub_obj, 0, 0, "Name");
        lv_table_set_cell_value(sub_obj, 0, 1, "chn");
        lv_table_set_cell_value(sub_obj, 0, 2, "addr");
        lv_table_set_cell_value(sub_obj, 0, 3, "dev_addr");
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
                lv_obj_t *mbox1 = lv_msgbox_create(NULL, "error", "create error", NULL, true);
                lv_obj_center(mbox1);
            }
        }
        break;
        case SEN_CONFIG_RESTORE:
        {
            var_reload(&g_sensor_param);
            lv_event_send(lv_obj_get_parent(obj), LV_EVENT_NOTIFY_UPDATE, (void *)-1);
            lv_obj_t *mbox1 = lv_msgbox_create(NULL, "restore", "Successfully restore", NULL, true);
            lv_obj_center(mbox1);
        }
        break;
        case SEN_CONFIG_SAVE:
        {
            int8_t ret = var_save(&g_sensor_param);
            if (ret == 0)
            {
                lv_obj_t *mbox1 = lv_msgbox_create(NULL, "save", "Successfully saved", NULL, true);
                lv_obj_center(mbox1);
            }
            else
            {

                rt_sprintf(buff, "Error code (0x%0X)", ret);
                lv_obj_t *mbox1 = lv_msgbox_create(NULL, "save", buff, NULL, true);
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
static void btnm_event_handler(lv_event_t *e);
static void ta_event_cb(lv_event_t *e)
{
    lv_indev_t *indev = lv_indev_get_act();
    if (indev == NULL)
        return;
    lv_indev_type_t indev_type = lv_indev_get_type(indev);

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    lv_obj_t *kb = lv_event_get_user_data(e);

    if (code == LV_EVENT_CLICKED &&
        (LV_INDEV_TYPE_POINTER == indev_type || indev_type == LV_INDEV_TYPE_ENCODER))
    {
        if (lv_obj_has_flag(kb, LV_OBJ_FLAG_HIDDEN))
        {
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_remove_event_cb(kb, btnm_event_handler);
        }
        lv_obj_add_event_cb(kb, btnm_event_handler, LV_EVENT_VALUE_CHANGED, ta);
    }

    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL)
    {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_event_cb(kb, btnm_event_handler);
    }
}
static void btnm_event_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *ta = lv_event_get_user_data(e);
    const char *txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));

    if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)
        lv_textarea_del_char(ta);
    else if (strcmp(txt, LV_SYMBOL_NEW_LINE) == 0)
        lv_event_send(ta, LV_EVENT_READY, NULL);
    else
        lv_textarea_add_text(ta, txt);
}

typedef struct table_edit
{
    uint16_t row;
    uint16_t col;
    lv_obj_t *obj;
} table_edit_t;

static void table_edit_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    table_edit_t *pedit = (table_edit_t *)lv_event_get_user_data(e);
    if (pedit && (pedit->row > 0) && (pedit->row <= g_sensor_param.sensor_num))
    {
        uint8_t idx = pedit->row - 1;

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
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }

        break;
        case 0x01: /*!< chn */
            g_sensor_param.sen_config[idx].chn = atoi(string);
            break;
        case 0x02: /*!< addr */
            g_sensor_param.sen_config[idx].addr = atoi(string);
            break;
        case 0x03: /*!< dev_addr */
            g_sensor_param.sen_config[idx].dev_addr = atoi(string);
            break;
        default:
            break;
        }
        LOG_D("[%d,%d]:%s", pedit->row, pedit->col, string);
        lv_event_send(lv_obj_get_parent(pedit->obj), LV_EVENT_NOTIFY_UPDATE, (void *)(1 << SEN_CONFIG_TABLE));
    }
}

static void table_event_pressed(lv_event_t *e)
{
    static table_edit_t s_edit;
    s_edit.obj = lv_event_get_target(e);
    lv_table_get_selected_cell(s_edit.obj, &s_edit.row, &s_edit.col);
    LOG_D("[%d,%d]", s_edit.row, s_edit.col);
    const char *string = lv_table_get_cell_value(s_edit.obj, s_edit.row, s_edit.col);

    if ((s_edit.row > 0) && (s_edit.row <= g_sensor_param.sensor_num))
    {
        if (s_edit.col == 0)
        {
            static lv_obj_t *dd = NULL;
            if (NULL == dd)
            {
                dd = lv_dropdown_create(lv_layer_top());
                lv_dropdown_clear_options(dd);
                for (size_t i = 0; i < SEN_NUM; i++)
                {
                    if (rom_sensor_var[i].symbolname)
                    {
                        char strbuff[16];
                        rt_sprintf(strbuff, "%d-%s", i, rom_sensor_var[i].symbolname);
                        lv_dropdown_add_option(dd, strbuff, LV_DROPDOWN_POS_LAST);
                    }
                }
                lv_obj_align(dd, LV_ALIGN_CENTER, 0, 20);
            }
            else
            {
                lv_obj_clear_flag(dd, LV_OBJ_FLAG_HIDDEN);
                lv_obj_remove_event_cb(dd, table_edit_event_cb);
            }
            lv_obj_add_event_cb(dd, table_edit_event_cb, LV_EVENT_VALUE_CHANGED, &s_edit);
        }
        else if (s_edit.col > 0 && s_edit.col < 4)
        {
            lv_obj_t *kb = ui_edit_create(lv_layer_top());
            lv_obj_set_size(kb, 800, 480);
            ui_edit_add_event(kb, table_edit_event_cb, LV_EVENT_READY, &s_edit);
            ui_edit_pop_with_default(kb, string);
        }
    }
}
