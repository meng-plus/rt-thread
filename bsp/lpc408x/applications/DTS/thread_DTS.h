/**
 * @file thread_DTS.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 测温主机数据收发线程
 * @version 0.1
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __THREAD_DTS_H_
#define __THREAD_DTS_H_

#include <stdint.h>
#include "Session_DTS.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum TH_DTS_CMD
    {
        TH_DTH_STOP = 0,
        TH_DTH_START,
        TH_DTH_SYNC,          /*!< NULL */
        TH_DTH_SET_DELAY,     /*!< uint32_t */
        TH_DTH_GET_DELAY,     /*!< uint32_t */
        TH_DTH_SET_CHN_RANGE, /*!< uint32_t */
        TH_DTH_GET_DATA,      /*!< dts_data_t* */
        TH_DTH_GET_SYS_INFO,
        TH_DTH_GET_CHN_INFO,
        TH_DTH_GET_PART_INFO,

    } TH_DTH_CMD_E;

    void thread_DTS_control(TH_DTH_CMD_E cmd, void *param);
#ifdef __cplusplus
}
#endif

#endif
