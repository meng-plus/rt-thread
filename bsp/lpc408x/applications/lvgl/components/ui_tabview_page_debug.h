/**
 * @file ui_tabview_page_Dashboard.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief  数据展示页面
 * @version 0.1
 * @date 2023-10-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UI_COMP_TABLEVIEW_PAGE_DEBUG_H_
#define __UI_COMP_TABLEVIEW_PAGE_DEBUG_H_

#include "../ui.h"

#ifdef __cplusplus
extern "C"
{
#endif
    enum UI_COMP_DEBUG
    {
        DEBUG_LABLE_WARING,
        DEBUG_LABLE_CTRL1,
        DEBUG_LABLE_CTRL2,
        DEBUG_LABLE_CTRL3,
        DEBUG_BT_WARING,
        DEBUG_BT_CTRL1,
        DEBUG_BT_CTRL2,
        DEBUG_BT_CTRL3,

        DEBUG_NUM,
    };
    lv_obj_t *ui_tabview_page_debug_create(lv_obj_t *tableview);
#ifdef __cplusplus
}
#endif

#endif
