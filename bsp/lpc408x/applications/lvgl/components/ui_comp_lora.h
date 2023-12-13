/**
 * @file ui_comp_LORA.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief LORA配置页面
 * @version 0.1
 * @date 2023-11-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __UI_COMP_LORA_H_
#define __UI_COMP_LORA_H_

#include "../ui.h"

#ifdef __cplusplus
extern "C"
{
#endif
    enum UI_COMP_LORA
    {
        LORA_SW_SEND,
        LORA_LABEL_MSG,
        LORA_BTN_EXIT,
        LORA_NUM,
    };
    lv_obj_t *ui_comp_lora_set_create(lv_obj_t *parent);
#ifdef __cplusplus
}
#endif

#endif
