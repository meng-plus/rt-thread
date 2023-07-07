#include "fm31xx.h"
#include <rtthread.h>
#include <stdlib.h>
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"

int Fm31xxWriteReg(uint8_t reg, uint8_t *pdt, uint8_t len);
int Fm31xxReadReg(uint8_t reg, uint8_t *pdt, uint8_t len);

#ifdef RT_USING_FINSH
#include <finsh.h>
#include <msh_parse.h>
#include <stdio.h>
static void _rtc_cmd_usage()
{
    rt_kprintf("rtc [option]\n");
    rt_kprintf("  show:get realtime\n");
    rt_kprintf("  set:eg. rtc set YY-MM-DD HH-MM-SS\n");
}
static void _rtc_show()
{
    RTC_TIME dtm;

    RTCReadTime(&dtm);
    LOG_D("time %d-%d-%d %d:%d:%d \r\n", dtm.year, dtm.month, dtm.date, dtm.hour, dtm.minute, dtm.second);
}
void rtc_cmd_set(int argc, char *argv[])
{
    if (argc < 3)
    {
        _rtc_cmd_usage();
        return;
    }
    RTC_TIME dtm;
    int num = sscanf(argv[2], "%hd-%hhd-%hhd", &dtm.year, &dtm.month, &dtm.date);
    if (num != 3)
    {
        rt_kprintf("cmd error %d %s\n", 2, argv[2]);
        return;
    }
    num = sscanf(argv[3], "%hhd:%hhd:%hhd", &dtm.hour, &dtm.minute, &dtm.second);
    if (num != 3)
    {
        rt_kprintf("cmd error %d %s\n", 3, argv[3]);
        return;
    }
    LOG_D("mew:time %d-%d-%d %d:%d:%d \r\n", dtm.year, dtm.month, dtm.date, dtm.hour, dtm.minute, dtm.second);
    RTCWriteTime(&dtm);
}
void rtc_cmd_read(int argc, char *argv[])
{
    uint8_t reg = 0;
    if (argc < 4)
    {
        _rtc_cmd_usage();
        return;
    }
    if (0 == rt_strcmp("reg", argv[2]))
    {
        reg = 1;
    }
    else if (0 == rt_strcmp("mem", argv[2]))
    {
        reg = 0;
    }
    else
    {
        return;
    }
    if (!msh_isint(argv[3]))
    {
        return;
    }
    uint16_t addr = atoi(argv[3]);
    uint8_t buff[1];
    if (reg)
    {
        Fm31xxReadReg(addr, buff, sizeof(buff));
    }
    else
    {
        E2PromRead(addr, buff, sizeof(buff));
    }
    LOG_D("read :%02x %02x", addr, buff[0]);
}
void rtc_cmd_write(int argc, char *argv[])
{
    uint8_t reg = 0;
    if (argc < 4)
    {
        _rtc_cmd_usage();
        return;
    }
    if (0 == rt_strcmp("reg", argv[2]))
    {
        reg = 1;
    }
    else if (0 == rt_strcmp("mem", argv[2]))
    {
        reg = 0;
    }
    else
    {
        return;
    }
    if (!msh_isint(argv[3]) || !msh_isint(argv[4]))
    {
        return;
    }
    uint16_t addr = atoi(argv[3]);
    uint8_t buff[1] = {atol(argv[4])};
    if (reg)
    {
        Fm31xxWriteReg(addr, buff, sizeof(buff));
    }
    else
    {
        E2PromWrite(addr, buff, sizeof(buff));
    }
    LOG_D("write :%02x %d", addr, buff[0]);
}
void rtc_cmd(int argc, char *argv[])
{
    if (argc < 2)
    {
        _rtc_cmd_usage();
        return;
    }
    if (0 == rt_strcmp("show", argv[1]))
    {
        _rtc_show();
    }
    else if (0 == rt_strcmp("set", argv[1]))
    {
        rtc_cmd_set(argc, argv);
    }
    else if (0 == rt_strcmp("read", argv[1]))
    {
        rtc_cmd_read(argc, argv);
    }
    else if (0 == rt_strcmp("write", argv[1]))
    {
        rtc_cmd_write(argc, argv);
    }
}
MSH_CMD_EXPORT_ALIAS(rtc_cmd, rtc, rtc_cmd)
#endif
