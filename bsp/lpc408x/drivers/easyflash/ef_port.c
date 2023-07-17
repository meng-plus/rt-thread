#include <rtthread.h>
#include <easyflash.h>

#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"

static int ef_port_init()
{
    easyflash_init();
    ef_print_env();
    return 0;
}
INIT_COMPONENT_EXPORT(ef_port_init);
