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

    /*!< 分布式在线通信协议栈初始化 */
    session_master_t *pSession = &pthread_doic->m_session;
    transport_t *pTransport = &pSession->transport;
    tr_init(pTransport, pthread_doic->read_buf, pthread_doic->rx_len);
    TR_CHECK_RES_E DOIC_waiting_response(transport_t * pTr);
    tr_control(pTransport, TR_SET_WAITING_RESPONSE, (void *)DOIC_waiting_response);
    doic_master_t *phander_doic = &pthread_doic->m_DOIC_master;
    DOIC_master_init(phander_doic, pthread_doic->send_buf, sizeof(pthread_doic->send_buf));

    /*!< 分布式下行通信协议初始化 */
    session_master_t *pSDCSession = &pthread_doic->m_DSCsession;
    transport_t *pSDCTransport = &pSDCSession->transport;
    tr_init(pSDCTransport, pthread_doic->sdc_read_buf, sizeof(pthread_doic->sdc_read_buf));
    TR_CHECK_RES_E SDC_waiting_response(transport_t * pTr); /*!< 等待帧数据 */
    tr_control(pSDCTransport, TR_SET_WAITING_RESPONSE, (void *)SDC_waiting_response);
    sdc_master_t *phander_sdc = &pthread_doic->m_SDC_master;
    SDC_master_init(phander_sdc, pthread_doic->sdc_send_buf, sizeof(pthread_doic->sdc_send_buf));
    while (1)
    {
        TR_CHECK_RES_E res;
        if (pTransport && pTransport->waiting_response)
        {
            res = pTransport->waiting_response(pTransport);
            if (TR_CHECK_FRAME == res)
            {
                DOIC_deal(phander_doic, (doic_data_t *)pTransport->rxBuff);
            }
        }
#if 1
        static uint8_t idx = 0;
        if (g_sensor_param.sensor_num <= idx)
        {
            idx = 0;
        }
        uint16_t sdc_len = 0;
        static uint8_t sen_idx = 0;
        sensor_config_t *pSensor_config = &g_sensor_param.sen_config[idx];
        sdc_len = SDC_master_0x10(phander_sdc, pSensor_config->addr, pSensor_config->type, pSensor_config->chn);
        uint16_t len = DOIC_master_0x4181(phander_doic, g_sensor_param.dev_config[0].addr, pSensor_config->dev_addr, (uint8_t *)phander_sdc->pdata, sdc_len);

        idx++;

#else
        const enum SENSOR_DEF table[] = {SEN_ENVI_TEMP, SEN_CO, SEN_SMOG, SOUND_LIGHT_ALARM};
        uint16_t sdc_len = 0;
        static uint8_t sen_idx = 0;
        sdc_len = SDC_master_0x10(phander_sdc, sen_idx + 1, table[sen_idx], 0);
        sen_idx = (sen_idx + 1) % sizeof(table) / sizeof(table[0]);

        uint16_t len = DOIC_master_0x4181(phander_doic, 12, 1, (uint8_t *)phander_sdc->pdata, sdc_len);
#endif
        rt_device_write(pthread_doic->device, 0, phander_doic->pdata, len);

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
