

#include "SDC_map.h"

int8_t SDC_deal_0x90(sdc_data_t *pdata, sdc_data_t *pdst);
sdc_map_t g_sdc_map[] = {
    {.cmd = 0x90, .func = SDC_deal_0x90},
};
uint16_t g_sdc_map_num = sizeof(g_sdc_map) / sizeof(g_sdc_map[0]);
