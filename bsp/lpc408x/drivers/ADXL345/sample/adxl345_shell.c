#include "adxl345.h"
#include <rtthread.h>
#include <stdlib.h>
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"


#ifdef RT_USING_FINSH
#include <finsh.h>
#include <msh_parse.h>
#include <stdio.h>
uint8_t ADXL345_Read(uint8_t reg, uint8_t *pdt, uint8_t len);
uint8_t ADXL345_WriteReg(uint8_t reg, uint8_t dt);
static void _adx345_cmd_usage()
{
    rt_kprintf("rtc [option]\n");
    rt_kprintf("  show:get realtime\n");
    rt_kprintf("  set:eg. rtc set YY-MM-DD HH-MM-SS\n");
}
static void _adx345_show()
{
    uint8_t st;
    ADXL345ReadReg(0x30, &st);
    LOG_D("0x30 : 0x%02x \r\n", st);
}
void _adx345_cmd_read(int argc, char *argv[])
{
    if (argc < 4)
    {
        _adx345_cmd_usage();
        return;
    }
    if (!msh_isint(argv[2]))
    {
        return;
    }
    uint16_t addr = atoi(argv[2]);
    uint8_t buff[1];

    ADXL345_Read(addr, buff, sizeof(buff));

    LOG_D("read :%02x %02x", addr, buff[0]);
}
void _adx345_cmd_write(int argc, char *argv[])
{
    if (argc < 4)
    {
        _adx345_cmd_usage();
        return;
    }

    if (!msh_isint(argv[2]) || !msh_isint(argv[3]))
    {
        return;
    }
    uint16_t addr = atoi(argv[2]);
    uint8_t buff[1] = {atol(argv[3])};

    ADXL345_WriteReg(addr, buff[0]);

    LOG_D("write :%02x %d", addr, buff[0]);
}
void adxl345_cmd(int argc, char *argv[])
{
    if (argc < 2)
    {
        _adx345_cmd_usage();
        return;
    }
    if (0 == rt_strcmp("show", argv[1]))
    {
        _adx345_show();
    }
    else if (0 == rt_strcmp("read", argv[1]))
    {
        _adx345_cmd_read(argc, argv);
    }
    else if (0 == rt_strcmp("write", argv[1]))
    {
        _adx345_cmd_write(argc, argv);
    }
}
MSH_CMD_EXPORT_ALIAS(adxl345_cmd, adxl345, adxl345 cmd)
#endif
