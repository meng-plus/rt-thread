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

#include "dts.h"
#include "agile_modbus.h"
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "rtdbg.h"

typedef struct _THREAD_DTS
{
    struct rt_semaphore rx_sem;
#if !defined(RT_USING_POSIX_STDIO) && defined(RT_USING_DEVICE)
    rt_device_t device;
#endif
    rt_thread_t tid;
    uint8_t status; /*!< 0: stop 1:init 2:read 3:error */
    uint8_t target;
    uint8_t repeat;
    /** modbus config */
    uint8_t ctx_send_buf[AGILE_MODBUS_MAX_ADU_LENGTH];
    uint8_t ctx_read_buf[AGILE_MODBUS_MAX_ADU_LENGTH];
    agile_modbus_rtu_t ctx_rtu;

    dts_data_t data;
} thread_dts_t;

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
int DTS_getchar()
{
    char ch = 0;
    while (rt_device_read(pthread_dts->device, -1, &ch, 1) != 1)
    {
        if (RT_EOK != rt_sem_take(&pthread_dts->rx_sem, 100)) // RT_WAITING_FOREVER
        {
            break;
        }
    }
    return ch;
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
    }
    agile_modbus_t *ctx = &pthread_dts->ctx_rtu._ctx;
    agile_modbus_rtu_init(&pthread_dts->ctx_rtu, pthread_dts->ctx_send_buf, sizeof(pthread_dts->ctx_send_buf), pthread_dts->ctx_read_buf, sizeof(pthread_dts->ctx_read_buf));
    agile_modbus_set_slave(ctx, DTS_ADDRESS);
    LOG_I("Running.");
    while (1)
    {
        switch (pthread_dts->status)
        {
        case 0: /*!< 检测设备是否在线 */
        {
            rt_thread_mdelay(1000);
            int send_len = agile_modbus_serialize_read_registers(ctx, 1, 2);
            rt_device_write(pthread_dts->device, 0, ctx->send_buf, send_len);
        }
        break;
        case 1:
        {
            int send_len = agile_modbus_serialize_read_registers(ctx, 21, sizeof(dts_channel_t) / 2);
            rt_device_write(pthread_dts->device, 0, ctx->send_buf, send_len);

        }
        break;
        case 2:
        {
            int send_len = agile_modbus_serialize_read_registers(ctx, 200, sizeof(dts_chn_Data_t) / 2);
            rt_device_write(pthread_dts->device, 0, ctx->send_buf, send_len);
        }
        break;
        default:
            break;
        }

        // ch = (int)DTS_getchar();
        uint16_t read_len = 0;
        pthread_dts->repeat = 0;
//        while (1)
//        {
//            if (RT_EOK != rt_sem_take(&pthread_dts->rx_sem, 1000)) // RT_WAITING_FOREVER
//            {
//                pthread_dts->repeat = 1;
//                 continue;
//            }
//            read_len=0;
//            uint16_t offset = rt_device_read(pthread_dts->device, -1, ctx->read_buf, ctx->read_bufsz );
//            rt_kprintf("leng:%d\r\n",offset);
//            LOG_HEX("data",8,ctx->read_buf,offset);
//        }
        
        while (1)
        {
            if (RT_EOK != rt_sem_take(&pthread_dts->rx_sem, 1000)) // RT_WAITING_FOREVER
            {
                pthread_dts->repeat = 1;
                 break;
            }            

            uint16_t offset = rt_device_read(pthread_dts->device, -1, read_len + ctx->read_buf, ctx->read_bufsz - read_len);
            read_len += offset;
            if (read_len > 3 && ctx->read_buf[2] + 5 <= read_len)
            {
                pthread_dts->repeat = 0;

                break;
            }
        }

        if (pthread_dts->repeat)
        {
            continue;
        }

        switch (pthread_dts->status)
        {
        case 0: /*!< 检测设备是否在线 */
        {
            int rc = agile_modbus_deserialize_read_registers(ctx, read_len, (uint16_t *)&pthread_dts->data.system);
            if (rc < 0)
            {
                LOG_W("Receive failed.");
                if (rc != -1)
                    LOG_W("Error code:%d", -128 - rc);

                continue;
            }
            if (pthread_dts->data.system.status)
            {
                pthread_dts->status = 1;
            }
        }
        break;
        case 1: /*!< 读取状态信息 */
        {
            int rc = agile_modbus_deserialize_read_registers(ctx, read_len, (uint16_t *)&pthread_dts->data.channel[0]);
            if (rc < 0)
            {
                LOG_W("Receive failed.");
                if (rc != -1)
                    LOG_W("Error code:%d", -128 - rc);
                pthread_dts->status = 0;
                continue;
            }
            pthread_dts->status = 2;
        }
        break;
        case 2: /*!< 读取温度数据 */
        {
            int rc = agile_modbus_deserialize_read_registers(ctx, read_len, (uint16_t *)&pthread_dts->data.partition[0]);
            if (rc < 0)
            {
                LOG_W("Receive failed.");
                if (rc != -1)
                    LOG_W("Error code:%d", -128 - rc);
                pthread_dts->status = 0;
                continue;
            }
            rt_thread_mdelay(1000);
        }
        break;
        default:
            break;
        }

        // rt_device_write(pthread_dts->device, 0, ctx->read_buf, length);
        // LOG_D("%d-> %s", length, ctx->read_buf);
        rt_thread_yield();
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
    pthread_dts->tid = rt_thread_create("th_dts", thread_dts_entry, RT_NULL,
                                        4096, RT_MAIN_THREAD_PRIORITY, 20);
    RT_ASSERT(pthread_dts->tid != RT_NULL);
    if (pthread_dts->tid)
    {
        rt_sem_init(&pthread_dts->rx_sem, "dtsrx", 0, 0);
        rt_thread_startup(pthread_dts->tid);
    }
    return 0;
}
INIT_APP_EXPORT(thread_DTS_init);
