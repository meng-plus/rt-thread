#include "ui.h"
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"

#ifdef RT_USING_FINSH
#include <finsh.h>

void lvgl_cmd(int argc, char *argv[])
{
    if (argc < 1)
    {
        LOG_RAW("eg: lvgl 0");
        return;
    }
    if (0 == rt_strcmp("0", argv[1]))
    {
        lv_disp_load_scr(ui_P00startup);
    }
    else if (0 == rt_strcmp("1", argv[1]))
    {
        lv_disp_load_scr(ui_P01main);
    }
}
MSH_CMD_EXPORT_ALIAS(lvgl_cmd, lvgl, GUI½çÃæ)
#endif
