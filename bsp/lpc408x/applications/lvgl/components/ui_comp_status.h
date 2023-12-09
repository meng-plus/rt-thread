/**
 * @file ui_comp_status.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 设备状态展示页面
 * @version 0.1
 * @date 2023-12-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UI_COMP_STATUS_H_
#define __UI_COMP_STATUS_H_

#include "../ui.h"

#ifdef __cplusplus
extern "C"
{
#endif
    enum UI_COMP_STATUS
    {
        STATUS_TITLE,      /*!< 标题 */
        STATUS_TABLE,      /*!< 表格 */
        STATUS_BTN_CHECK,  /*!< 自检 */
        STATUS_BTN_RETURN, /*!< 返回 */
        STATUS_NUM,
    };
    lv_obj_t *ui_comp_status_create(lv_obj_t *parent);
#ifdef __cplusplus
}
#endif

#endif
