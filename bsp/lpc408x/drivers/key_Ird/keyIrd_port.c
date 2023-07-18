#include <rtthread.h>
#include "drv_keyIrd.h"

#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"

static int keyIrd_port_init()
{
    KeyInit();
    return 0;
}
INIT_BOARD_EXPORT(keyIrd_port_init);
