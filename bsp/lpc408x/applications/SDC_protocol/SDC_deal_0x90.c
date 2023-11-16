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

int8_t SDC_deal_0x90(sdc_data_t *pdata, sdc_data_t *pdst)
{
    if ((pdata != NULL) && (pdata->length != 0))
    {
        sdc_0x90_t *pVal =(sdc_0x90_t *) pdata->data;
    }
    return 0;
}
