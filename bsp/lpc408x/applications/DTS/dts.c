#include "dts.h"

void dts_init(dts_data_t *dts_ptr)
{
    if (dts_ptr == NULL)
        return;
    memset(dts_ptr, 0, sizeof(dts_data_t));
}
