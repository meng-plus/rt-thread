/**
 * @file ui_comp_WIFI.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief WIFI配置页面
 * @version 0.1
 * @date 2023-11-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UI_COMP_WIFI_H_
#define __UI_COMP_WIFI_H_

#include "../ui.h"

#ifdef __cplusplus
extern "C"
{
#endif
    enum UI_COMP_WIFI
    {
        WIFI_SW_ENABLE,
        WIFI_BTN_GET_NAME,
        WIFI_BTN_GET_IP,
        WIFI_SW_SEND_MSG,
        WIFI_EDIT_ID,
        WIFI_LABEL_MSG,
        WIFI_BTN_EXIT,
        WIFI_KB, /*!< 键盘 */
        WIFI_NUM,
    };
    lv_obj_t *ui_comp_wifi_set_create(lv_obj_t *parent);
#ifdef __cplusplus
}
#endif

#endif
