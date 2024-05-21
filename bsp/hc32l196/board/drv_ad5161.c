/**
 * @file drv_ad5161.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief AD5161电位器
 * SPI/IIC驱动AD5161电位器
 * 当前工程使用SPI
 * https://www.analog.com/cn/products/ad5161.html
 *
 * @version 0.1
 * @date 2024-05-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <board.h>
#include <rtdevice.h>
#include <drv_log.h>
#define AD5161_SPI_DEVICE_NAME "sspi10"

static int rt_hw_ad5161_init(void)
{
    struct rt_spi_device *spi_device = RT_NULL;

    spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device));
    if (RT_NULL == spi_device)
    {
        LOG_E("Failed to malloc the spi device.");
        return -RT_ENOMEM;
    }
    if (RT_EOK != rt_spi_bus_attach_device_cspin(spi_device, AD5161_SPI_DEVICE_NAME, "sspi1", GET_PIN(A, 3), RT_NULL))
    {
        LOG_E("Failed to attach the spi device.");
        return -RT_ERROR;
    }
    struct rt_spi_configuration cfg;
    cfg.data_width = 8;
    cfg.mode       = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
    cfg.max_hz     = 20 * 1000 * 1000; /* 20M */
    rt_spi_configure(spi_device, &cfg);
    return RT_EOK;
}

/* 导出到自动初始化 */
INIT_DEVICE_EXPORT(rt_hw_ad5161_init);

#ifdef RT_USING_FINSH
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <finsh.h>
#include <msh_parse.h>

static void _ad5161_cmd_print_usage(void)
{
    rt_kprintf("ad5161 [option] value\n");
    rt_kprintf(" range 0~255, AW 22.6k->78\n");
    rt_kprintf("     write:    write ad5161 res value\n");
    rt_kprintf("               e.g. MSH >ad5161 write 128\n");
    rt_kprintf("     help:     this help list\n");
}

static void spi_ad5161_cmd(int argc, char *argv[])
{
    struct rt_spi_device *spi_dev_ad5161;
    char name[RT_NAME_MAX];
    if (argc < 2)
    {
        _ad5161_cmd_print_usage();
        return;
    }
    if (rt_strcmp(argv[1], "write") == 0)
    {
        if (msh_isint(argv[2]) == RT_TRUE)
        { /* 查找 spi 设备获取设备句柄 */
            spi_dev_ad5161 = (struct rt_spi_device *)rt_device_find(AD5161_SPI_DEVICE_NAME);
            if (!spi_dev_ad5161)
            {
                rt_kprintf("spi sample run failed! can't find %s device!\n", name);
            }
            rt_uint8_t w25x_write_value = atoi(argv[2]);
            struct rt_spi_message msg1;
            msg1.send_buf   = &w25x_write_value;
            msg1.recv_buf   = RT_NULL;
            msg1.length     = 1;
            msg1.cs_take    = 1;
            msg1.cs_release = 0;
            msg1.next       = RT_NULL;
            rt_spi_transfer_message(spi_dev_ad5161, &msg1);
        }
    }
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT_ALIAS(spi_ad5161_cmd, ad5161, ad5161 write value);
#endif //RT_USING_FINSH
