/**
 * @file lv_group_manage.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 管理控件，方便遥控操作
 * @version 0.1
 * @date 2023-08-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "lv_group_manage.h"
#include "lv_group.h"
#include "win32drv.h"
void group_init_begin(lv_group_manage_t *group_manage_ptr)
{
    if (NULL == group_manage_ptr)
    {
        return;
    }
    group_manage_ptr->def = lv_group_get_default();
    if (NULL == group_manage_ptr->curr)
    {
        group_manage_ptr->curr = lv_group_create();
        if (NULL == group_manage_ptr->curr)
        {
            return;
        }
    }
    lv_group_set_default(group_manage_ptr->curr);
}
void group_init_end(lv_group_manage_t *group_manage_ptr)
{
    lv_group_set_default(group_manage_ptr->def);
}
void group_init_loaded(lv_group_manage_t *group_manage_ptr)
{
    lv_group_set_default(group_manage_ptr->curr);
    lv_win32_add_all_input_devices_to_group(lv_group_get_default());
}
void group_init_unloaded(lv_group_manage_t *group_manage_ptr)
{
    lv_group_set_default(group_manage_ptr->def);
    lv_win32_add_all_input_devices_to_group(lv_group_get_default());
}
