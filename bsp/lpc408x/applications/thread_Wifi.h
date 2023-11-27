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
#ifndef __THREAD_WIFI_H_
#define __THREAD_WIFI_H_

#include <stdint.h>
#include "ui_comp_WIFI.h"
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _THREAD_Wifi
    {
        struct rt_semaphore rx_sem;
#if !defined(RT_USING_POSIX_STDIO) && defined(RT_USING_DEVICE)
        rt_device_t device;
#endif
        rt_thread_t tid;

        uint8_t status;          /*!< 0: stop 1:init 2:read 3:error */
        uint8_t offline;         /*!< 1: 掉线*/
        uint8_t update_flag : 1; /*!< 1: 数据有刷新通知*/

        /** modbus config */
        uint8_t Wifi_send_buf[256];
        uint8_t Wifi_read_buf[256];
        uint32_t read_len;
        uint32_t delayms; /*!< 刷新周期 0为最快 */

        /** lvgl  */
        lv_obj_t *obj_ble_wifi;

    } thread_Wifi_t;

    extern thread_Wifi_t *pthread_Wifi;

    extern uint8_t WIFICloseStep;
    extern uint8_t WIFISetStep;
    extern uint8_t WIFISend_Flag;

    extern uint8_t BLECloseStep;
    extern uint8_t BLESetStep;
    extern uint8_t BLESend_Flag;

    extern uint8_t ReceiveFlag;

    int thread_Wifi_init(void);

    void WIFI_IP_Edit(const uint8_t *pstr);
    void WIFI_Get_Name(void);
    void WIFI_Get_IP(void);

    void BLE_Get_Name(void);
    void BLE_Get_Addr(void);

#ifdef __cplusplus
}
#endif

#endif
