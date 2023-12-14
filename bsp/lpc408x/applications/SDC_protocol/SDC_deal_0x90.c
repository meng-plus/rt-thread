/**
 * @file SDC_deal_0x10.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "SDC_protocol.h"
#include "system_var.h"
#include "rt_err_code.h"
int8_t SDC_deal_0x90(sdc_data_t *pdata, sdc_data_t *pdst)
{
    if ((pdata == NULL) || (pdata->length == 0))
    {
        return -1;
    }
    sdc_0x90_t *pVal = (sdc_0x90_t *)pdata->data;
    for (size_t i = 0; i < g_sensor_param.sensor_num; i++)
    {
        if (pVal->SenName == g_sensor_param.sen_config[i].type)
        {
            memcpy(&g_var_work.Sensor[i], pVal, sizeof(sdc_0x90_t));
            rt_tick_t tick_cur = rt_tick_get_millisecond();
            g_var_work.Sensor_tick_last[i] = tick_cur;

            error_code_clear_flag(rt_err_code_find(g_sensor_param.sen_config[i].type));

            return 1;
        }
    }

    return 0;
}
