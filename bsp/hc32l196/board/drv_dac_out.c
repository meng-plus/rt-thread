#include <board.h>
#include <rtdevice.h>
#include <drv_log.h>
#define DAC_DEVICE_NAME "dac0"


void laser_dac_out(rt_uint32_t value)
{
    static rt_dac_device_t dac_device = RT_NULL;

    if (dac_device == RT_NULL)
    {
        dac_device = (rt_dac_device_t)rt_device_find(DAC_DEVICE_NAME);
        if (dac_device == RT_NULL)
        {
            LOG_E("dac device %s not found!", DAC_DEVICE_NAME);
            return;
        }
    }
    rt_dac_write(dac_device, 1, value);
}
#ifdef RT_USING_FINSH
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <finsh.h>
#include <msh_parse.h>
#include "system_var.h"

static void _dac_out_cmd_print_usage(void)
{
    rt_kprintf("Usage: \n");
    rt_kprintf("          <value>    - range[0,4096]->0V-3V3\n");
    rt_kprintf("laser_dac write <value>    - write dac value on the channel\n");
    rt_kprintf("laser_dac disable         - disable dac\n");
    rt_kprintf("laser_dac enable          - enable dac\n");
}

static int laser_dac_cmd(int argc, char **argv)
{
    int result                        = RT_EOK;
    static rt_dac_device_t dac_device = RT_NULL;
    char *result_str;

    if (argc > 1)
    {
        if (dac_device == RT_NULL)
        {
            dac_device = (rt_dac_device_t)rt_device_find(argv[2]);
            result_str = (dac_device == RT_NULL) ? "failure" : "success";
            rt_kprintf("probe %s %s \n", argv[2], result_str);
        }

        if (dac_device == RT_NULL)
        {
            rt_kprintf("Please using 'dac probe <dac_name>' first\n");
            return -RT_ERROR;
        }
        if (!strcmp(argv[1], "enable"))
        {
            if (argc == 2)
            {
                result     = rt_dac_enable(dac_device, 1);
                result_str = (result == RT_EOK) ? "success" : "failure";
                rt_kprintf("%s channel %d enables %s \n", dac_device->parent.parent.name, 1, result_str);
            }
            else
            {
                rt_kprintf("dac enable <channel>   - enable dac channel\n");
            }
        }
        else if (!strcmp(argv[1], "write"))
        {
            if (argc == 4)
            {
                int32_t i32 = atoi(argv[2]);

                if (i32 > 4096)
                {
                    rt_kprintf("value(%d) >255  out of range\n", i32);
                      return -RT_ERROR;
                }
                rt_uint16_t write_value = i32;
                rt_dac_write(dac_device, 1, write_value);
                if (g_prod_param.dac_value != write_value)
                {
                    g_prod_param.dac_value = write_value;
                    var_save(&g_prod_param);
                }

                rt_kprintf("%s channel %d write value is %d \n", dac_device->parent.parent.name, 1, atoi(argv[2]));
            }
            else
            {
                rt_kprintf("dac write <channel> <value>     - write dac value on the channel\n");
            }
        }
        else if (!strcmp(argv[1], "disable"))
        {
            if (argc == 3)
            {
                result     = rt_dac_disable(dac_device, 1);
                result_str = (result == RT_EOK) ? "success" : "failure";
                rt_kprintf("%s channel %d disable %s \n", dac_device->parent.parent.name, 1, result_str);
            }
            else
            {
                rt_kprintf("dac disable  - disable dac channel\n");
            }
        }
        else
        {
            rt_kprintf("Unknown command. Please enter 'dac' for help\n");
        }
    }
    else
    {
        _dac_out_cmd_print_usage();
        result = -RT_ERROR;
    }
    return RT_EOK;
}
MSH_CMD_EXPORT(laser_dac_cmd, laser_dac control out);

#endif /* RT_USING_FINSH */
