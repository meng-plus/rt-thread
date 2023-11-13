/**
 * @file ui_comp_edit.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief  输入菜单编辑界面
 * @version 0.1
 * @date 2023-11-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _UI_COMP_EDIT_H
#define _UI_COMP_EDIT_H

#include "../ui.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // COMPONENT stateBar
    enum UI_COMP_EDIT
    {
        EDIT_TEXTAREA,
        EDIT_KEYBOARD,
        EDIT_NUM,
    };
    lv_obj_t *ui_edit_create(lv_obj_t *comp_parent);
    void ui_edit_pop_with_default(lv_obj_t *obj, const char *string);
    /**
     * @brief
     *
     * @param obj
     * @param event_cb
     * @param filter  LV_EVENT_READY
     * @param user_data
     * @return true
     * @return false
     */
    bool ui_edit_add_event(lv_obj_t *obj, lv_event_cb_t event_cb, lv_event_code_t filter,
                           void *user_data);
    const char *ui_edit_get_text(lv_obj_t *obj);
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
