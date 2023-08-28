/**
 * @file lv_group_manage.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 管理用户组，方便遥控操作
 * @version 0.1
 * @date 2023-08-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __LV_GROUP_MANAGE_H_
#define __LV_GROUP_MANAGE_H_

#include "lvgl.h"
#include "lv_group.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct lv_group_manage
    {
        lv_group_t *curr; /*!< 当前在页面的group */
        lv_group_t *def;  /*!< 备份的group指针 */

    } lv_group_manage_t;

    void group_init_begin(lv_group_manage_t *group_manage_ptr);
    void group_init_end(lv_group_manage_t *group_manage_ptr);
    void group_init_loaded(lv_group_manage_t *group_manage_ptr);
    void group_init_unloaded(lv_group_manage_t *group_manage_ptr);
#ifdef __cplusplus
}
#endif
#endif
