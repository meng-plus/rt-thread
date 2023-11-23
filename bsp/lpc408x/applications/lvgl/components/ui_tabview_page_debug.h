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
        DEBUG_INPUT_S1,
        DEBUG_INPUT_S2,
        DEBUG_INPUT_S3,
        DEBUG_INPUT_S4,
        DEBUG_INPUT_S5,
        DEBUG_INPUT_S6,
        DEBUG_OUT_A,
        DEBUG_OUT_B,
        DEBUG_OUT_C,
        DEBUG_PUMP_AIR,
        DEBUG_NUM,
    };
    lv_obj_t *ui_tabview_page_debug_create(lv_obj_t *tableview);
#ifdef __cplusplus
}
#endif

#endif
