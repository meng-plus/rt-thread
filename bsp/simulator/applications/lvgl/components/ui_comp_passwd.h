/**
 * @file ui_comp_passwd.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 输入密码的弹窗
 * @version 0.1
 * @date 2023-08-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UI_COMP_PASSWD_H_
#define __UI_COMP_PASSWD_H_

#include "../ui.h"

#ifdef __cplusplus
extern "C"
{
#endif
    enum UI_COMP_PASSWD
    {
        UI_COMP_PASSWD_BT_CLOSE,
        UI_COMP_PASSWD_ED_PASSSWD,
        _UI_COMP_PASSWD_NUM,
    };
    typedef void (*passwd_cb_t)(lv_event_t *e);
    lv_obj_t *ui_comp_passwd_popup(lv_obj_t *parent, passwd_cb_t func_callback);
    void ui_comp_passwd_hidden(lv_obj_t* comp_obj);
#ifdef __cplusplus
}
#endif

#endif
