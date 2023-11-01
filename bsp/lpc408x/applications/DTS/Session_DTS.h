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
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "rtdbg.h"

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
    typedef struct _THREAD_DTS
    {
        session_master_t m_session;
        struct rt_semaphore rx_sem;
#if !defined(RT_USING_POSIX_STDIO) && defined(RT_USING_DEVICE)
        rt_device_t device;
#endif
        rt_thread_t tid;
        uint8_t status;          /*!< 0: stop 1:init 2:read 3:error */
        uint8_t offline : 1;     /*!< 1: 掉线*/
        uint8_t update_flag : 1; /*!< 1: 数据有刷新通知*/
        uint8_t target;
        uint8_t part_idx;
        uint8_t part_num; /*!< 本轮扫描的数量 */
        uint8_t repeat;

        uint8_t sel_chn; //[DTS_CHANNEL_NUM]; /*!< 刷新通道选择 */
        /** modbus config */
        uint8_t ctx_send_buf[AGILE_MODBUS_MAX_ADU_LENGTH];
        uint8_t ctx_read_buf[AGILE_MODBUS_MAX_ADU_LENGTH];
        agile_modbus_rtu_t ctx_rtu;
        uint32_t delayms; /*!< 刷新周期 0为最快 */
        dts_data_t data;
    } thread_dts_t;

    extern thread_dts_t *pthread_dts;
    extern const session_msg_t dts_msg_array[DTS_MSG_NUM];
    void session_dts_init(session_master_t *pSession);
    int session_dts_tick(session_master_t *pSession);
    void session_dts_change(session_master_t *pSession, DTS_MSG_ID_E id);
    int32_t session_dts_request(session_master_t *se_handle, DTS_MSG_ID_E id);
    int32_t session_dts_response(session_master_t *se_handle, DTS_MSG_ID_E id);
#ifdef __cplusplus
}
#endif
#endif
