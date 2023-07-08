#include "ads1115.h"
#include <rtthread.h>
#include <stdlib.h>
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"

#ifdef RT_USING_FINSH
#include <finsh.h>
#include <msh_parse.h>
#include <stdio.h>

static void _ads1115_cmd_usage()
{
    rt_kprintf("ads1115 [option]\n");
    rt_kprintf("  show: show something\n");
    rt_kprintf("  read:eg. ads1115 read 0000 \n");
    rt_kprintf("  write:eg. ads1115 write 001 1234 \n");
}
static void _ads1115_show()
{
    ads1115_err ads1115_err_st;
    ads1115_dev *ads1115_handle = ads1115_get_device(0);
    if (ads1115_handle)
    {
        ads1115_setconfigMUX(ads1115_handle, ADS1115_MUX_Channel_0, false);      /*<!通道0对GND  */
        ads1115_setconfigPGA(ads1115_handle, ADS1115_PGA_4096, false);           /*<!4.096V基准电压 */
        ads1115_setconfigMODE(ads1115_handle, ADS1115_MODE_SingleConver, false); /*<! 单次转换*/
        ads1115_setconfigDR(ads1115_handle, ADS1115_DataRate_64, false);         /*<! 采样周期*/
        ads1115_setconfigCompMode(ads1115_handle, ADS1115_COMP_MODE_0, false);
        ads1115_setconfigCompPol(ads1115_handle, ADS1115_COMP_POL_0, false);
        ads1115_setconfigCompLat(ads1115_handle, ADS1115_COMP_LAT_0, false);
        ads1115_setconfigCompQue(ads1115_handle, ADS1115_COMP_QUE_3, false);
        ads1115_setconfigOS(ads1115_handle, ADS1115_OS_SingleConverStart, true); /*<!启动转换 */
        uint16_t cfg;
        ads1115_err_st = ads1115_readConfig(ads1115_handle, &cfg);
        if (ads1115_err_st == ADS1115_SUCCESS)
        {
            LOG_D("ads1115 cfg:%02x", cfg);
        }
        else
        {
            LOG_E("ads1115 st:%d\r\n", ads1115_err_st);
        }
    }
    LOG_D("not find ");
}
void _ads1115_cmd_read(int argc, char *argv[])
{
    if (argc < 4)
    {
        _ads1115_cmd_usage();
        return;
    }
    if (!msh_isint(argv[2]))
    {
        return;
    }
    uint16_t addr = atoi(argv[2]);
    uint8_t buff[1];

    ads1115_dev *ptr = ads1115_get_device(0);
    if (ptr != NULL && ptr->i2c.write)
    {
             ads1115_reg_read(ptr,  addr, buff, sizeof(buff));
        LOG_D("read :%02x %02x", addr, buff[0]);
    }
    else
    {
        LOG_D("not find ");
    }
}
void _ads1115_cmd_write(int argc, char *argv[])
{
    if (argc < 4)
    {
        _ads1115_cmd_usage();
        return;
    }

    if (!msh_isint(argv[2]) || !msh_isint(argv[3]))
    {
        return;
    }
    uint16_t addr = atoi(argv[2]);
    uint8_t buff[1] = {atol(argv[3])};
    ads1115_dev *ptr = ads1115_get_device(0);
    if (ptr != NULL && ptr->i2c.write)
    {
        ads1115_reg_write(ptr,addr, buff, sizeof(buff));
        LOG_D("write :%02x %d", addr, buff[0]);
    }
    else
    {
        LOG_D("not find ");
    }
}
void ads1115_cmd(int argc, char *argv[])
{
    if (argc < 2)
    {
        _ads1115_cmd_usage();
        ads1115_init();
        return;
    }
    if (0 == rt_strcmp("show", argv[1]))
    {
        _ads1115_show();
    }
    else if (0 == rt_strcmp("read", argv[1]))
    {
        _ads1115_cmd_read(argc, argv);
    }
    else if (0 == rt_strcmp("write", argv[1]))
    {
        _ads1115_cmd_write(argc, argv);
    }
}
MSH_CMD_EXPORT_ALIAS(ads1115_cmd, ads1115, ads1115 cmd)
#endif
