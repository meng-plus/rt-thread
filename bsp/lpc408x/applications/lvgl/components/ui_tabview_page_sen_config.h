/**
 * @file ui_tabview_page_sen_config.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 传感器通信配置
 * @version 0.1
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UI_COMP_TABLEVIEW_PAGE_SEN_CONFIG_H_
#define __UI_COMP_TABLEVIEW_PAGE_SEN_CONFIG_H_

#include "../ui.h"

#ifdef __cplusplus
extern "C"
{
#endif
    enum UI_COMP_SEN_CONFIG
    {
        SEN_CONFIG_RS485_2,
        SEN_CONFIG_RS485_2_ADDR,
        SEN_CONFIG_RS485_2_BAUD,
        SEN_CONFIG_RS485_3,
        SEN_CONFIG_RS485_3_ADDR,
        SEN_CONFIG_RS485_3_BAUD,

        SEN_CONFIG_TABLE,
        SEN_CONFIG_DEL,
        SEN_CONFIG_NEW,
        SEN_CONFIG_RESTORE,
        SEN_CONFIG_SAVE,
        SEN_CONFIG_KB,
        SEN_CONFIG_NUM,
    };
    lv_obj_t *ui_tabview_page_sen_config_create(lv_obj_t *tableview);
#ifdef __cplusplus
}
#endif

#endif
