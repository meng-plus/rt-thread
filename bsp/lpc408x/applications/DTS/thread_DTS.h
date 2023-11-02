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
        TH_DTS_STOP = 0,
        TH_DTS_START,
        TH_DTS_SYNC,          /*!< NULL */
        TH_DTS_SET_DELAY,     /*!< uint32_t */
        TH_DTS_GET_DELAY,     /*!< uint32_t */
        TH_DTS_SET_CHN_RANGE, /*!< uint32_t */
        TH_DTS_GET_DATA,      /*!< dts_data_t* */
        TH_DTS_GET_SYS_INFO,
        TH_DTS_GET_CHN_INFO,
        TH_DTS_GET_PART_INFO,

    } TH_DTS_CMD_E;
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
    void thread_DTS_control(TH_DTS_CMD_E cmd, void *param);
#ifdef __cplusplus
}
#endif

#endif
