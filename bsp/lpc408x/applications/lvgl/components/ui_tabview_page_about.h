/**
 * @file ui_tabview_page_ABOUT.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief  数据展示页面
 * @version 0.1
 * @date 2023-10-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UI_COMP_TABLEVIEW_PAGE_ABOUT_H_
#define __UI_COMP_TABLEVIEW_PAGE_ABOUT_H_

#include "../ui.h"

#ifdef __cplusplus
extern "C"
{
#endif
    enum UI_COMP_ABOUT
    {
        ABOUT_SF_VERSION,
        ABOUT_HF_VERSION,

        ABOUT_NUM,
    };
    lv_obj_t *ui_tabview_page_about_create(lv_obj_t *tableview);
#ifdef __cplusplus
}
#endif

#endif
