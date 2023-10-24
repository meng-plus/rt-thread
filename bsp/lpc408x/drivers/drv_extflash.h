/**
 * @file rt_hw_extflash.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 扩展的外部flash
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __DRV_EXTFLASH_H_
#define __DRV_EXTFLASH_H_

#include "spifilib_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
    extern SPIFI_HANDLE_T *pSPIFIHandle;
    int rt_hw_extflash_init(void);
#ifdef __cplusplus
}
#endif
#endif
