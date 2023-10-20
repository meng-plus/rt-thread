#include "dts.h"
#include <string.h>
void dts_init(dts_data_t *dts_ptr)
{
    if (0 == dts_ptr)
        return;
    memset(dts_ptr, 0, sizeof(dts_data_t));
}
