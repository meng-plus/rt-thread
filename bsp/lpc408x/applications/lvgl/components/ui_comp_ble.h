/**
 * @file ui_comp_BLE.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief BLE配置页面
 * @version 0.1
 * @date 2023-11-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UI_COMP_BLE_H_
#define __UI_COMP_BLE_H_

#include "../ui.h"

#ifdef __cplusplus
extern "C"
{
#endif
    enum UI_COMP_BLE
    {
        BLE_LABEL_MSG,
        BLE_SW_ENABLE,
        BLE_BTN_GET_NAME,
        BLE_BTN_GET_IP,
        BLE_SW_SEND_MSG,
        // BLE_EDIT_ID,

        BLE_BTN_EXIT,
        // BLE_KB, /*!< 键盘 */
        BLE_NUM,
    };
    lv_obj_t *ui_comp_ble_set_create(lv_obj_t *parent);
#ifdef __cplusplus
}
#endif

#endif
