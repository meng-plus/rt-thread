/**
 * @file thread_DOIC.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 下行设备通信
 * @version 0.1
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __THREAD_DOIC_H_
#define __THREAD_DOIC_H_

#include <stdint.h>
#include "Session_master.h"
#include <rtdevice.h>
#include "DOIC_protocol.h"
#include "SDC_protocol.h"
#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum TH_DOIC_CMD
    {
        TH_DOIC_STOP = 0,
        TH_DOIC_START,
        TH_DOIC_SYNC,      /*!< NULL */
        TH_DOIC_SET_DELAY, /*!< uint32_t */
        TH_DOIC_GET_DELAY, /*!< uint32_t */
    } TH_DOIC_CMD_E;

    typedef struct THREAD_DOIC
    {
        session_master_t m_session;
        struct rt_semaphore rx_sem;
#if !defined(RT_USING_POSIX_STDIO) && defined(RT_USING_DEVICE)
        rt_device_t device;
#endif
        rt_thread_t tid;
        uint32_t delayms; /*!< 刷新周期 0为最快 */
        uint8_t send_buf[512];
        uint8_t tx_len;
        uint8_t read_buf[512];
        uint8_t rx_len;
        doic_master_t m_DOIC_master;

        session_master_t m_DSCsession;
        sdc_master_t m_SDC_master;
        uint8_t sdc_send_buf[512];
        uint8_t sdc_read_buf[512];
        uint8_t sdc_rx_len;

    } thread_doic_t;

    void thread_DOIC_control(TH_DOIC_CMD_E cmd, void *param);
#ifdef __cplusplus
}
#endif

#endif
