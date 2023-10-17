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

#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "rtdbg.h"

struct thread_DTS
{
    struct rt_semaphore rx_sem;
#if !defined(RT_USING_POSIX_STDIO) && defined(RT_USING_DEVICE)
    rt_device_t device;
#endif
    rt_thread_t tid;

    struct Protocol pl;

} thread_dts;

static rt_err_t finsh_rx_ind(rt_device_t dev, rt_size_t size)
{
    RT_ASSERT(comm_host != RT_NULL);
    if (dev->user_data == &thread_dts)
    {
        /* release semaphore to let finsh thread rx data */
        rt_sem_release(&thread_dts.rx_sem);
    }

    return RT_EOK;
}
int DTS_getchar()
{
    char ch = 0;
    while (rt_device_read(thread_dts.device, -1, &ch, 1) != 1)
    {
        rt_sem_take(thread_dts.rx_sem, RT_WAITING_FOREVER);
    }
    return ch;
}
void thread_dts_entry(void *param)
{
    int ch;
    if (thread_dts.device == RT_NULL)
    {
        rt_device_t dev = RT_NULL;
        dev = rt_device_find("uart1");
        if (rt_device_open(dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                                    RT_DEVICE_FLAG_STREAM) == RT_EOK)
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
    PL_DTS_init(&thread_dts.pl);
    while (1)
    {
        ch = (int)DTS_getchar();
        if (ch < 0)
        {
            continue;
        }
        pl_pushOneByte(&thread_dts.pl, ch); /*!< 来一个字节 */
        enum PL_CHECK_RES res = pl_check(thread_dts.pl);
        if (res != PL_CHECK_ERR_LEN ||
            res != PL_CHECK_FRAME)
        {
            pl_clearAuto(thread_dts.pl);
        }
        rt_thread_yield();
    }
}
int thread_DTS_init()
{

    thread_dts.tid = rt_thread_create("th_dts", thread_dts_entry, RT_NULL,
                                      RT_MAIN_THREAD_STACK_SIZE, RT_MAIN_THREAD_PRIORITY, 20);
    RT_ASSERT(thread_dts.tid != RT_NULL);
    if (thread_dts.tid)
    {
        rt_sem_init(&thread_dts.rx_sem, "dtsrx", 0, 0);
        rt_thread_startup(thread_dts.tid);
    }
    return 0;
}
INIT_APP_EXPORT(thread_DTS_init);
