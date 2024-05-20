/**
 * @file system_upload.cpp
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief 更新系统配置到参数
 * @version 0.1
 * @date 2024-03-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __SYSTEM_UPLOAD_H_
#define __SYSTEM_UPLOAD_H_

#include "system_var.h"
#ifdef __cplusplus
extern "C" {
#endif

enum __SYSTEM_UPLOAD
{
    SYSTEM_UPLOAD_ALL,
};
/**
     * @brief 更新参数到设备
     *
     */
void system_upload(enum __SYSTEM_UPLOAD item);

#ifdef __cplusplus
}
#endif
#endif //__SYSTEM_UPLOAD_H_
