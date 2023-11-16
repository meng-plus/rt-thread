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
#include <rtthread.h>
#include <rtdevice.h>
#include "Session_DTS.h"
#include "system_var.h"
#include "thread_DTS.h"
thread_dts_t *pthread_dts;

static rt_err_t finsh_rx_ind(rt_device_t dev, rt_size_t size)
{
    rt_serial_t *serial = (rt_serial_t *)dev;
    if (dev != pthread_dts->device)
    {
        return RT_EOK;
    }
    if (serial->event == RT_SERIAL_EVENT_RX_TIMEOUT)
    {
        if (dev->user_data == pthread_dts)
        {
            /* release semaphore to let finsh thread rx data */
            rt_sem_release(&pthread_dts->rx_sem);
        }
    }
    return RT_EOK;
}

void thread_dts_entry(void *param)
{
    if (pthread_dts->device == RT_NULL)
    {
        rt_device_t dev = RT_NULL;
        dev = rt_device_find("uart3");

        /* step2：修改串口配置参数 */
        struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 初始化配置参数 */
        config.baud_rate = BAUD_RATE_115200;                       // 修改波特率为 9600
        config.data_bits = DATA_BITS_8;                            // 数据位 8
        config.stop_bits = STOP_BITS_1;                            // 停止位 1
        config.bufsz = 1024;                                       // 修改缓冲区 buff size 为 128
        config.parity = PARITY_NONE;                               // 无奇偶校验位

        /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
        rt_device_control(dev, RT_DEVICE_CTRL_CONFIG, &config);
        if (rt_device_open(dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                                    RT_DEVICE_FLAG_INT_TX) == RT_EOK)
        {
            if (pthread_dts->device != RT_NULL)
            {
                /* close old finsh device */
                rt_device_close(pthread_dts->device);
                rt_device_set_rx_indicate(pthread_dts->device, RT_NULL);
            }
            dev->user_data = pthread_dts;
            pthread_dts->device = dev;
            rt_device_set_rx_indicate(dev, finsh_rx_ind);
        }
        pthread_dts->delayms = 100;
    }
    agile_modbus_t *ctx = &pthread_dts->ctx_rtu._ctx;
    agile_modbus_rtu_init(&pthread_dts->ctx_rtu, pthread_dts->ctx_send_buf, sizeof(pthread_dts->ctx_send_buf), pthread_dts->ctx_read_buf, sizeof(pthread_dts->ctx_read_buf));
    agile_modbus_set_slave(ctx, DTS_ADDRESS);
    LOG_I("Running.");
    session_master_t *pSession = &pthread_dts->m_session;
    dts_init(&pthread_dts->data);
    session_dts_init(&pthread_dts->m_session);
    while (1)
    {
        session_dts_tick(pSession);
        rt_thread_yield();
        if (pthread_dts->delayms)
            rt_thread_mdelay(pthread_dts->delayms);
    }
}
int thread_DTS_init()
{
    pthread_dts = rt_calloc(1, sizeof(thread_dts_t));
    if (pthread_dts == 0)
    {
        LOG_E("pdts_data == NULL,need %d Byte", sizeof(dts_data_t));
        return -1;
    }
    g_var_work.dts = pthread_dts;

    pthread_dts->tid = rt_thread_create("th_dts", thread_dts_entry, RT_NULL,
                                        4096, RT_MAIN_THREAD_PRIORITY, 20);
    RT_ASSERT(pthread_dts->tid != RT_NULL);
    if (pthread_dts->tid)
    {
        rt_sem_init(&pthread_dts->rx_sem, "dtsrx", 0, 0);
        rt_thread_startup(pthread_dts->tid);
        thread_DTS_control(TH_DTS_SYNC, NULL);
    }
    return 0;
}
INIT_APP_EXPORT(thread_DTS_init);
void thread_DTS_control(TH_DTS_CMD_E cmd, void *param)
{
    switch (cmd)
    {
    case TH_DTS_SYNC:
        if (pthread_dts)
        {
            pthread_dts->delayms = g_prod_param.dts_delayms;
        }
        break;
    case TH_DTS_SET_CHN_RANGE:
        if (pthread_dts && param)
        {
            uint32_t sel_chn = *(uint32_t *)param;
            if (sel_chn < pthread_dts->data.system.chn_num)
            {
                pthread_dts->sel_chn = sel_chn;
                pthread_dts->m_session.msg_id = DTS_CHN;
            }
        }
        break;
    case TH_DTS_SET_DELAY:
    {
        if (param != NULL)
        {
            uint32_t delayms = *(uint32_t *)param;
            if (delayms > g_var_init.dts_delayms_max)
            {
                return;
            }
            g_prod_param.dts_delayms = delayms;
            if (pthread_dts)
            {
                pthread_dts->delayms = delayms;
            }
        }
    }
    break;
    case TH_DTS_GET_DELAY:
    {
        if (param != NULL)
        {
            *(uint32_t *)param = g_prod_param.dts_delayms;
            if (pthread_dts)
            {
                pthread_dts->delayms = g_prod_param.dts_delayms;
            }
        }
    }
    break;
    case TH_DTS_GET_DATA:
        if (pthread_dts && param)
        {
            pthread_dts->update_flag = 0;
            (*(void **)param) = (void *)&pthread_dts->data;
        }
        break;
    case TH_DTS_GET_SYS_INFO:
        pthread_dts->update_flag = 0;

        break;
    case TH_DTS_GET_CHN_INFO:
        pthread_dts->update_flag = 0;

        break;
    case TH_DTS_GET_PART_INFO:

        break;
    default:
        break;
    }
}
