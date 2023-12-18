/**
 * @file Session_DTS.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief   主机会话层
 * @version 0.1
 * @date 2023-10-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __SESSION_DTS_H_
#define __SESSION_DTS_H_

#include "Session_master.h"

#include "dts.h"
#include "agile_modbus.h"


#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum DTS_MSG_ID
    {
        DTS_SYS,  /*!< 获取设备信息 */
        DTS_CHN,  /*!< 读取通道信息 */
        DTS_PART, /*!< 读取分区数据 */
        DTS_MSG_NUM,
    } DTS_MSG_ID_E;

    void session_dts_init(session_master_t *pSession);
    int session_dts_tick(session_master_t *pSession);
    void session_dts_change(session_master_t *pSession, DTS_MSG_ID_E id);
    int32_t session_dts_request(session_master_t *se_handle, DTS_MSG_ID_E id);
    int32_t session_dts_response(session_master_t *se_handle, DTS_MSG_ID_E id);
#ifdef __cplusplus
}
#endif
#endif
