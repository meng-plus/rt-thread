

#include <rtthread.h>
#define LOG_TAG "shell.app"
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"

#include "TimeTaskcy.hpp"
#include "CAdc.hpp"
#include "Motor.hpp"

#ifdef RT_USING_FINSH
#include <finsh.h>
#include <msh_parse.h>
#include <stdio.h>
#include "CylinderTime.hpp"
static void _app_cmd_usage()
{
    rt_kprintf("dev [option]\n");
    rt_kprintf("show cy: see Cylinder \n");
    rt_kprintf("set 1 on/off: set Cylinder \n");
}

void dev_show(int argc, char *argv[])
{
    if (argc < 3)
    {
        _app_cmd_usage();
        return;
    }
    if (0 == rt_strcmp("cy", argv[2]))
    {
        LOG_D("show cy all status");
        CylinderTime &cy_ptr = CylinderTime::GetInstance();
        for (size_t i = 0; i < (uint8_t)Cylinder_DEV::NUM; i++)
        {
            CCylinder *dev_ptr = cy_ptr.m_Cylinder[i];
            rt_kprintf("%s [%d] o:%d i0:%d i1:%d\n", dev_ptr->m_name, dev_ptr->m_status, dev_ptr->getOut(), dev_ptr->read_i0(), dev_ptr->read_i1());
        }
    }
    else if (0 == rt_strcmp("adc", argv[2]))
    {
        uint16_t raw = CAdc::GetInstance().readraw();
        float voltage = CAdc::GetInstance().readVoltage();
        rt_kprintf("adc raw:%d vol:%f\n", raw, voltage);
    }
    else
    {
        LOG_E("不支持");
        _app_cmd_usage();
    }
}
void dev_set(int argc, char *argv[])
{
    if (argc < 3)
    {
        _app_cmd_usage();
        return;
    }
    int8_t i = atoi(argv[2]);
    bool flag = 0;
    if (0 == rt_strcmp("on", argv[3]))
    {
        flag = true;
    }
    else if (0 == rt_strcmp("off", argv[3]))
    {
        flag = false;
    }
    else
    {
        LOG_E("不支持");
        _app_cmd_usage();
        return;
    }
    if (i < (uint8_t)Cylinder_DEV::NUM)
    {
        CylinderTime &cy_ptr = CylinderTime::GetInstance();
        CCylinder *dev_ptr = cy_ptr.m_Cylinder[i];
        if (flag)
        {
            dev_ptr->set();
        }
        else
        {
            dev_ptr->reset();
        }
        LOG_D("set %s %d", dev_ptr->m_name, flag);
    }
    else
    {
        LOG_E("out of rang  i>= MAX(%d)", i, (uint8_t)Cylinder_DEV::NUM);
    }
}
void dev_taskcy(int argc, char *argv[])
{
    if (argc < 3)
    {
        _app_cmd_usage();
        return;
    }
    if (0 == rt_strcmp("start", argv[2]))
    {
        CTimeTaskcy::GetInstance().control(TASK_CY_CMD::start);
        rt_kprintf("CTimeTaskcy start\n");
    }
    else if (0 == rt_strcmp("stop", argv[2]))
    {
        CTimeTaskcy::GetInstance().control(TASK_CY_CMD::stop);
        rt_kprintf("CTimeTaskcy stop\n");
    }
    else
    {
        LOG_E(" not support");
    }
}
void dev_motor(int argc, char *argv[])
{
    if (argc < 3)
    {
        _app_cmd_usage();
        return;
    }
    if (0 == rt_strcmp("vol", argv[2]))
    {
        float vol = atof(argv[3]);
        CMotor::GetInstance().setVoltageTarget(vol);
    }else
    {
        float per = atof(argv[2]);
        CMotor::GetInstance().setRatio(per);
        rt_kprintf("CMotor set per %f\n", per);
    }

}
void app_shell(int argc, char *argv[])
{
    if (argc < 2)
    {
        _app_cmd_usage();
        return;
    }
    if (0 == rt_strcmp("show", argv[1]))
    {
        dev_show(argc, argv);
    }
    if (0 == rt_strcmp("set", argv[1]))
    {
        dev_set(argc, argv);
    }
    if (0 == rt_strcmp("taskcy", argv[1]))
    {
        dev_taskcy(argc, argv);
    }
    if (0 == rt_strcmp("motor", argv[1]))
    {
        dev_motor(argc, argv);
    }
}

MSH_CMD_EXPORT_ALIAS(app_shell, dev, shell dev info)

#endif
