/**
 * @file gui_def.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief GUI结构框架
 * 便于项目的开发工作
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __GUI_DEF_H_
#define __GUI_DEF_H_

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct _GUI_DEF gui_def_t;
    struct _GUI_DEF
    {
        lv_obj_t *obj;
        const char *name;
        void (*init)();
        void (*destory)();
    };

#ifdef __cplusplus
}
#endif
#endif
