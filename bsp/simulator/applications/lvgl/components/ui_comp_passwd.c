/**
 * @file ui_comp_passwd.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 密码输入弹窗
 * @version 0.1
 * @date 2023-08-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "ui_comp_passwd.h"

#include "win32drv.h"
static lv_obj_t *spWin = NULL;
static lv_group_t *pGroup;
static passwd_cb_t userfunc_cb;
static void spanbox_event_cb(lv_event_t *e);

static void delete_event_cb(lv_event_t *e);

lv_obj_t *ui_comp_passwd_popup(lv_obj_t *parent, passwd_cb_t func_callback)
{

    userfunc_cb = func_callback;
    lv_obj_add_flag(parent, LV_OBJ_FLAG_CLICKABLE);
    if (spWin == NULL)
    {
        spWin = lv_win_create(parent, 40);
        lv_obj_set_style_text_font(spWin, &ui_font_simhei16, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_add_flag(spWin, LV_OBJ_FLAG_HIDDEN); // 隐藏控件
        lv_obj_set_align(spWin, LV_ALIGN_CENTER);
        lv_obj_set_pos(spWin, 0, 0);
        lv_obj_set_size(spWin, lv_pct(100), lv_pct(100));
        lv_win_add_title(spWin, "请输入密码");

        lv_obj_t *pBt = lv_win_add_btn(spWin, LV_SYMBOL_CLOSE, 60);
        lv_obj_set_style_text_font(pBt, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t *pContent = lv_win_get_content(spWin);
        lv_obj_t *pSpinBox = lv_spinbox_create(pContent);
        lv_obj_set_align(pSpinBox, LV_ALIGN_CENTER);
        lv_obj_set_pos(pSpinBox, 140, 0);
        lv_obj_set_size(pSpinBox, 130, 30);
        lv_obj_set_align(pSpinBox, LV_ALIGN_TOP_LEFT);
        lv_spinbox_set_digit_format(pSpinBox, 4, 0);
        lv_spinbox_set_range(pSpinBox, 0, 9999);
        
        // lv_group_focus_freeze(lv_group_get_default(), true);
        lv_group_remove_obj(lv_scr_act());
        lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);

        lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_PASSWD_NUM);
        children[UI_COMP_PASSWD_BT_CLOSE] = pBt;
        children[UI_COMP_PASSWD_ED_PASSSWD] = pSpinBox;

        lv_obj_add_event_cb(spWin, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
        lv_obj_add_event_cb(spWin, del_component_child_event_cb, LV_EVENT_DELETE, children);

        lv_obj_add_event_cb(pBt, delete_event_cb, LV_EVENT_CLICKED, spWin);
        lv_obj_add_event_cb(spWin, delete_event_cb, LV_EVENT_DELETE, spWin);

        /**
         * @brief add enter key
         *
         */
        lv_obj_add_event_cb(pSpinBox, spanbox_event_cb, LV_EVENT_KEY, spWin);
        if (pGroup == NULL)
        {
            pGroup = lv_group_create();
        }
        lv_group_add_obj(pGroup, pSpinBox);
        lv_group_add_obj(pGroup, pBt);
        lv_group_focus_obj(pSpinBox);
    }
    else
    {
        lv_obj_clear_flag(spWin, LV_OBJ_FLAG_HIDDEN);
        lv_obj_t *pSpinBox = ui_comp_get_child(spWin, UI_COMP_PASSWD_ED_PASSSWD);
        
        lv_spinbox_set_value(pSpinBox, 0);
    }
    lv_win32_add_all_input_devices_to_group(pGroup);
    lv_group_focus_obj(ui_comp_get_child(spWin, UI_COMP_PASSWD_ED_PASSSWD));
    return spWin;
}
void ui_comp_passwd_hidden(lv_obj_t* comp_obj)
{
    lv_win32_add_all_input_devices_to_group(NULL);
    lv_obj_clear_flag(lv_obj_get_parent(comp_obj), LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(spWin, LV_OBJ_FLAG_HIDDEN);

}
void delete_event_cb(lv_event_t *e)
{
    lv_obj_t *pObj = lv_event_get_current_target(e);
    lv_obj_t *pdata = (lv_obj_t *)lv_event_get_user_data(e);

    if (pdata == spWin)
    {
        ui_comp_passwd_hidden(pdata);
    }
    //if (pObj == ui_comp_get_child(spWin, UI_COMP_PASSWD_BT_CLOSE))
    //{

    //    if (userfunc_cb)
    //        userfunc_cb(e);
    //}
}
void spanbox_event_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *pObj = lv_event_get_current_target(e);
    lv_obj_t *pdata = (lv_obj_t *)lv_event_get_user_data(e);

    if (LV_EVENT_KEY == event_code)
    {
        lv_indev_type_t indev_type = lv_indev_get_type(lv_indev_get_act());
        uint32_t c = *((uint32_t *)lv_event_get_param(e)); /*uint32_t because can be UTF-8*/
        if (indev_type == LV_INDEV_TYPE_KEYPAD)
        {
            if (c == LV_KEY_ENTER)
            {
                LV_LOG_INFO("LV_KEY_ENTER :%d ", key);
                //lv_obj_add_flag(pdata, LV_OBJ_FLAG_HIDDEN);
                //lv_obj_clear_flag(lv_obj_get_parent(spWin), LV_OBJ_FLAG_CLICKABLE);
                if (userfunc_cb)
                    userfunc_cb(e);
            }
            if (c == LV_KEY_ESC)
            {
                LV_LOG_INFO("LV_KEY_ESC :%d ", key);
            }
        }
        LV_LOG_INFO("key :%d ", key);
    }
}
