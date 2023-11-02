/**
 * @file thread_DOIC.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 下行设备通信
 * @version 0.1
 * @date 2023-11-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "system_var.h"
#include "thread_DOIC.h"
thread_doic_t *pthread_doic;

static rt_err_t finsh_rx_ind(rt_device_t dev, rt_size_t size)
{
    rt_serial_t *serial = (rt_serial_t *)dev;
    if (dev != pthread_doic->device)
    {
        return RT_EOK;
    }
    if (serial->event == RT_SERIAL_EVENT_RX_TIMEOUT)
    {
        if (dev->user_data == pthread_doic)
        {
            /* release semaphore to let finsh thread rx data */
            rt_sem_release(&pthread_doic->rx_sem);
        }
    }
    return RT_EOK;
}

void thread_doic_entry(void *param)
{
    if (pthread_doic->device == RT_NULL)
    {
        rt_device_t dev = RT_NULL;
        dev = rt_device_find("uart2");

        /* step2：修改串口配置参数 */
        struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 初始化配置参数 */
        config.baud_rate = BAUD_RATE_115200;                       // 修改波特率为 115200
        config.data_bits = DATA_BITS_8;                            // 数据位 8
        config.stop_bits = STOP_BITS_1;                            // 停止位 1
        config.bufsz = 1024;                                       // 修改缓冲区 buff size 为 128
        config.parity = PARITY_NONE;                               // 无奇偶校验位

        /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
        rt_device_control(dev, RT_DEVICE_CTRL_CONFIG, &config);
        if (rt_device_open(dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                                    RT_DEVICE_FLAG_INT_TX) == RT_EOK)
        {
            if (pthread_doic->device != RT_NULL)
            {
                /* close old finsh device */
                rt_device_close(pthread_doic->device);
                rt_device_set_rx_indicate(pthread_doic->device, RT_NULL);
            }
            dev->user_data = pthread_doic;
            pthread_doic->device = dev;
            rt_device_set_rx_indicate(dev, finsh_rx_ind);
        }
        pthread_doic->delayms = 100;
    }
    LOG_I("Running.");
    session_master_t *pSession = &pthread_doic->m_session;
    //session_doic_init(&pthread_doic->m_session);
    tr_init(&pSession->transport, pthread_doic->read_buf, pthread_doic->rx_len);
    DOIC_master_t *phander = &pthread_doic->m_DOIC_master;
    DOIC_master_init(phander, NULL, 0);
    TR_CHECK_RES_E DOIC_waiting_response(transport_t * pTr);
    while (1)
    {
        // session_doic_tick(pSession);
        TR_CHECK_RES_E res = DOIC_waiting_response(&pSession->transport);
        if (TR_CHECK_FRAME == res)
        {
            DOIC_deal(phander, (doic_data_t *)pthread_doic->read_buf);
        }
        rt_thread_yield();
        if (pthread_doic->delayms)
            rt_thread_mdelay(pthread_doic->delayms);
    }
}
int thread_DOIC_init()
{
    pthread_doic = rt_calloc(1, sizeof(thread_doic_t));
    if (pthread_doic == 0)
    {
        LOG_E("pdoic_data == NULL,need %d Byte", sizeof(doic_data_t));
        return -1;
    }
    // g_var_work.doic = pthread_doic;

    pthread_doic->tid = rt_thread_create("th_doic", thread_doic_entry, RT_NULL,
                                         4096, RT_MAIN_THREAD_PRIORITY, 20);
    RT_ASSERT(pthread_doic->tid != RT_NULL);
    if (pthread_doic->tid)
    {
        rt_sem_init(&pthread_doic->rx_sem, "doicrx", 0, 0);
        rt_thread_startup(pthread_doic->tid);
        // thread_DOIC_control(TH_DTH_SYNC, NULL);
    }
    return 0;
}
INIT_APP_EXPORT(thread_DOIC_init);

void thread_DOIC_control(TH_DOIC_CMD_E cmd, void *param)
{
}
