/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-06-01     tyustli     the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include <string.h>
#include <stdlib.h>
#include "ad7843.h"
#define DBG_TAG "drv.ad7843"
#define DBG_LVL DBG_WARNING
#include <rtdbg.h>

/* hardware section */

static rt_err_t ad7843_control(struct rt_touch_device *device, int cmd, void *data)
{
    if (cmd == RT_TOUCH_CTRL_GET_ID)
    {
    }

    if (cmd == RT_TOUCH_CTRL_GET_INFO)
    {
    }

    switch (cmd)
    {
    case RT_TOUCH_CTRL_SET_X_RANGE: /* set x range */
    {
        break;
    }
    case RT_TOUCH_CTRL_SET_Y_RANGE: /* set y range */
    {
        break;
    }
    case RT_TOUCH_CTRL_SET_X_TO_Y: /* change x y */
    {
        break;
    }
    case RT_TOUCH_CTRL_SET_MODE: /* change int trig type */
    {
        rt_uint16_t trig_type;
        trig_type = *(rt_uint16_t *)data;

        switch (trig_type)
        {
        case RT_DEVICE_FLAG_INT_RX:

            break;
        case RT_DEVICE_FLAG_RDONLY:

            break;
        default:
            break;
        }
        break;
    }
    default:
    {
        break;
    }
    }

    return RT_EOK;
}
static const rt_uint8_t ad7843_tx_data[] = {
    0x80 | 0x50,
    0,
    0x80 | 0x10,
    0,
    0,
};
rt_bool_t hw_ad7843_touchpad_is_pressed(struct rt_touch_device *touch)
{
    if (touch && rt_pin_read(touch->config.irq_pin.pin) == 0)
    {
        return 1;
    }
    return 0;
}
void hw_ad7843_calibration(rt_uint16_t min_raw_x, rt_uint16_t min_raw_y, rt_uint16_t max_raw_x, rt_uint16_t max_raw_y)
{
    rt_touch_ad7843_t dev_obj = NULL;

    /*Your code comes here*/

    dev_obj = (rt_touch_ad7843_t)rt_device_find("tp0");
    if (dev_obj != NULL)
    {
        dev_obj->min_raw_x = min_raw_x;
        dev_obj->min_raw_y = min_raw_y;
        dev_obj->max_raw_x = max_raw_x;
        dev_obj->max_raw_y = max_raw_y;
    }
}
static rt_size_t ad7843_readpoint(struct rt_touch_device *touch, void *buf, rt_size_t touch_num)
{
    if (touch_num == 0)
    {
        return 0;
    }
    rt_touch_ad7843_t dev = (rt_touch_ad7843_t)touch;
    struct rt_touch_data *touch_data = (struct rt_touch_data *)buf;
    rt_uint8_t rx_data[sizeof(ad7843_tx_data)];
    rt_spi_transfer(&dev->spi, ad7843_tx_data, rx_data, sizeof(rx_data));
    rt_uint8_t x_count = 1;
    rt_uint8_t y_count = 1;
    rt_uint32_t x_cum = 0;
    rt_uint32_t y_cum = 0;

    // LOG_HEX("touch_data", 8, rx_data, sizeof(rx_data));
    uint16_t x = rx_data[1];
    uint16_t y = rx_data[3];
    x = (x << 5) | (rx_data[2] & 0x1F);
    y = (y << 5) | (rx_data[4] & 0x1F);
    if (!hw_ad7843_touchpad_is_pressed(touch))
    {
        touch_data->event = RT_TOUCH_EVENT_NONE;
        return 0;
    }
    touch_data->event = RT_TOUCH_EVENT_DOWN;

    x_cum = x;
    y_cum = y;
    touch_data->x_coordinate = (rt_uint16_t)(((float)x_cum / x_count - dev->min_raw_x) / (dev->max_raw_x - dev->min_raw_x) * dev->parent.info.range_x);
    touch_data->y_coordinate = (rt_uint16_t)(((float)y_cum / y_count - dev->min_raw_y) / (dev->max_raw_y - dev->min_raw_y) * dev->parent.info.range_y);
    LOG_D("point[%d,%d]  raw(%d %d)", touch_data->x_coordinate, touch_data->y_coordinate, x_cum, y_cum);
    return 1;
}

static struct rt_touch_ops touch_ops =
    {
        .touch_readpoint = ad7843_readpoint,
        .touch_control = ad7843_control,
};

rt_touch_ad7843_t hw_ad7843_init(char *spi_bus_name, rt_base_t cs_pin, rt_base_t irq_pin,
                                 rt_int32_t range_x, rt_int32_t range_y,
                                 rt_uint16_t min_raw_x, rt_uint16_t min_raw_y,
                                 rt_uint16_t max_raw_x, rt_uint16_t max_raw_y)
{

    rt_touch_ad7843_t dev_obj = (rt_touch_ad7843_t)rt_calloc(1, sizeof(struct rt_touch_ad7843));

    if (dev_obj == RT_NULL)
        return 0;

    /* hardware init */

    rt_pin_mode(irq_pin, PIN_MODE_INPUT);

    /* interface bus */
    struct rt_spi_device *spi_device = &dev_obj->spi;
    rt_uint8_t dev_num = 0;
    char dev_name[RT_NAME_MAX];
    do
    {
        rt_sprintf(dev_name, "%s%d", spi_bus_name, dev_num++);
        if (dev_num == 255)
        {
            return RT_NULL;
        }
    } while (rt_device_find(dev_name));

    if (RT_EOK != rt_spi_bus_attach_device_cspin(spi_device, dev_name, spi_bus_name, cs_pin, RT_NULL))
    {
        LOG_E("Failed to attach the spi device.");
        return 0;
    }
    struct rt_spi_configuration cfg;
    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_3 | RT_SPI_MSB;
    cfg.max_hz = 500 * 1000; /* 42M,SPI max 42MHz,lcd 4-wire spi */

    rt_spi_configure(spi_device, &cfg);

    dev_obj->min_raw_x = min_raw_x;
    dev_obj->min_raw_y = min_raw_y;
    dev_obj->max_raw_x = max_raw_x;
    dev_obj->max_raw_y = max_raw_y;
    dev_obj->parent.info.type = RT_TOUCH_TYPE_RESISTANCE;
    dev_obj->parent.info.vendor = RT_TOUCH_VENDOR_UNKNOWN;
    dev_obj->parent.info.point_num = 1;
    dev_obj->parent.info.range_x = range_x;
    dev_obj->parent.info.range_y = range_y;

    dev_obj->parent.config.irq_pin.pin = irq_pin;
    dev_obj->parent.config.irq_pin.mode = PIN_MODE_INPUT_PULLUP;
    dev_obj->parent.ops = &touch_ops;
    /* register touch device */
    dev_num = 0;
    do
    {
        rt_sprintf(dev_name, "tp%d", dev_num++);
        if (dev_num == 255)
        {
            // rt_device_destroy(&(dev_obj->parent));
            return RT_NULL;
        }
    } while (rt_device_find(dev_name));
    rt_hw_touch_register(&dev_obj->parent, dev_name, RT_DEVICE_FLAG_INT_RX, RT_NULL);

    LOG_I("touch device ad7843 init success\n");

    return dev_obj;
}

/************************** end of file ********************************/
