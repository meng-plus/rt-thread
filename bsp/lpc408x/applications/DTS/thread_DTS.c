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
} thread_dts_t;

thread_dts_t thread_dts;
/** modbus config */
uint8_t ctx_send_buf[AGILE_MODBUS_MAX_ADU_LENGTH];
uint8_t ctx_read_buf[AGILE_MODBUS_MAX_ADU_LENGTH];
agile_modbus_rtu_t ctx_rtu;

static rt_err_t finsh_rx_ind(rt_device_t dev, rt_size_t size)
{
    rt_serial_t *serial = (rt_serial_t *)dev;
    if (dev != thread_dts.device)
    {
        return RT_EOK;
    }
    if (serial->event == RT_SERIAL_EVENT_RX_TIMEOUT)
    {
        if (dev->user_data == &thread_dts)
        {
            /* release semaphore to let finsh thread rx data */
            rt_sem_release(&thread_dts.rx_sem);
        }
    }
    return RT_EOK;
}
int DTS_getchar()
{
    char ch = 0;
    while (rt_device_read(thread_dts.device, -1, &ch, 1) != 1)
    {
        if (RT_EOK != rt_sem_take(&thread_dts.rx_sem, 100)) // RT_WAITING_FOREVER
        {
            break;
        }
    }
    return ch;
}
void thread_dts_entry(void *param)
{
    int ch;
    if (thread_dts.device == RT_NULL)
    {
        rt_device_t dev = RT_NULL;
        dev = rt_device_find("uart3");

        /* step2：修改串口配置参数 */
        struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 初始化配置参数 */
        config.baud_rate = BAUD_RATE_115200;                       // 修改波特率为 9600
        config.data_bits = DATA_BITS_8;                            // 数据位 8
        config.stop_bits = STOP_BITS_1;                            // 停止位 1
        config.bufsz = 128;                                        // 修改缓冲区 buff size 为 128
        config.parity = PARITY_NONE;                               // 无奇偶校验位

        /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
        rt_device_control(dev, RT_DEVICE_CTRL_CONFIG, &config);
        if (rt_device_open(dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                                    RT_DEVICE_FLAG_INT_TX) == RT_EOK)
        {
            if (thread_dts.device != RT_NULL)
            {
                /* close old finsh device */
                rt_device_close(thread_dts.device);
                rt_device_set_rx_indicate(thread_dts.device, RT_NULL);
            }
            dev->user_data = &thread_dts;
            thread_dts.device = dev;
            rt_device_set_rx_indicate(dev, finsh_rx_ind);
        }
    }
    agile_modbus_t *ctx = &ctx_rtu._ctx;
    agile_modbus_rtu_init(&ctx_rtu, ctx_send_buf, sizeof(ctx_send_buf), ctx_read_buf, sizeof(ctx_read_buf));
    agile_modbus_set_slave(ctx, DTS_ADDRESS);
    LOG_I("Running.");
    while (1)
    {
        switch (thread_dts.status)
        {
        case 1:
        {
            int send_len = agile_modbus_serialize_read_registers(ctx, 1, 2);
            rt_device_write(thread_dts.device, 0, ctx->send_buf, send_len);
        }
        break;

        default:
            break;
        }
        // ch = (int)DTS_getchar();
        if (RT_EOK != rt_sem_take(&thread_dts.rx_sem, 10000)) // RT_WAITING_FOREVER
        {
            continue;
        }
        uint16_t length = rt_device_read(thread_dts.device, -1, ctx->read_buf, ctx->read_bufsz);
        rt_device_write(thread_dts.device, 0, ctx->read_buf, length);
        LOG_D("%d-> %s", length, ctx->read_buf);
        rt_thread_yield();
    }
}
int thread_DTS_init()
{

    thread_dts.tid = rt_thread_create("th_dts", thread_dts_entry, RT_NULL,
                                      4096, RT_MAIN_THREAD_PRIORITY, 20);
    RT_ASSERT(thread_dts.tid != RT_NULL);
    if (thread_dts.tid)
    {
        rt_sem_init(&thread_dts.rx_sem, "dtsrx", 0, 0);
        rt_thread_startup(thread_dts.tid);
    }
    return 0;
}
INIT_APP_EXPORT(thread_DTS_init);
