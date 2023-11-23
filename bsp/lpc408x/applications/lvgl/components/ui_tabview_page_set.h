/**
 * @file ui_tabview_page_set.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 设置页面
 * @version 0.1
 * @date 2023-11-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UI_COMP_TABLEVIEW_SET_H_
#define __UI_COMP_TABLEVIEW_SET_H_

#include "../ui.h"

#ifdef __cplusplus
extern "C"
{
#endif
    enum UI_COMP_SET
    {
        SET_WIFI,
        SET_BLUETOOTH,
        SET_NUM,
    };
    lv_obj_t *ui_tabview_page_set_create(lv_obj_t *tableview);
#ifdef __cplusplus
}
#endif

#endif
