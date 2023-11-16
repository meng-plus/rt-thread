

#include "DOIC_map.h"

int8_t DOIC_deal_0x4101(doic_data_t *pdata, doic_data_t *pdst);

doic_map_t g_doic_map[] = {
    {.cmd = 0x41, .scmd = 0x01, .func = DOIC_deal_0x4101},
};
uint16_t g_doic_map_num = sizeof(g_doic_map) / sizeof(g_doic_map[0]);
