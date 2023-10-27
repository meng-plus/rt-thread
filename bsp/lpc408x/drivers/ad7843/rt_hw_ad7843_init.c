#include "rtdevice.h"
#define DBG_TAG "drv.ad7843"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>
#include "drv_pin.h"
#include "ad7843.h"
#include <finsh.h>
#include <msh_parse.h>
rt_ad7843_t dev;
static int rt_hw_ad7843_init(void)
{
    dev = hw_ad7843_init("sspi1", GET_PIN(2, 23), GET_PIN(0, 12),
                         480, 272,
                         0, 0,
                         480, 272);
    if (RT_NULL == dev)
    {
        return -RT_ERROR;
    }
    struct rt_touch_data read_data;
    rt_memset(&read_data, 0, sizeof(struct rt_touch_data));
    // rt_xpt2046_t touch = (rt_xpt2046_t)rt_device_find(touch_name);
    if (rt_device_open((rt_device_t)(&dev->parent), RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        LOG_E(LOG_TAG " cannot open touch device named \n");
        return 0;
    }
    if (rt_device_read((rt_device_t)(&dev->parent), 0, &read_data, 1) == 1)
    {

        LOG_I(LOG_TAG " point(%d,%d) st=%d", read_data.x_coordinate, read_data.y_coordinate, read_data.event);
    }
    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_ad7843_init);

void ad7843_test(int argc, char *argv[])
{
    if (argc < 3)
    {

        return;
    }

    if (!msh_isint(argv[1]) || !msh_isint(argv[2]))
    {
        return;
    }
    uint32_t count = atoi(argv[1]);
    uint32_t delayms = atoi(argv[2]);
    struct rt_touch_data read_data;
    for (size_t i = 0; i < count; i++)
    {
        rt_memset(&read_data, 0, sizeof(struct rt_touch_data));
        if (rt_device_read((rt_device_t)(&dev->parent), 0, &read_data, 1) == 1)
        {

            LOG_I(LOG_TAG " point(%d,%d) st=%d", read_data.x_coordinate, read_data.y_coordinate, read_data.event);
        }
        rt_thread_mdelay(delayms);
    }
}

MSH_CMD_EXPORT(ad7843_test, ad7843 test)
