/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-06-18     thread-liu        the first version
 * 2020-10-09     thread-liu   Porting for stm32h7xx
 */

#include <board.h>
#include <rtthread.h>
#include "hc32l196_dac.h"
#if defined(BSP_USING_DAC1) || defined(BSP_USING_DAC2)
#include "dac_config.h"

//#define DRV_DEBUG
#define LOG_TAG "drv.dac"
#include <drv_log.h>
typedef struct DAC_HandleTypeDef
{
    uint8_t rev; /*!< 保留字段 */
} DAC_HandleTypeDef;

static DAC_HandleTypeDef dac_config[] =
    {
#ifdef BSP_USING_DAC1
        DAC1_CONFIG,
#endif

#ifdef BSP_USING_DAC2
        DAC2_CONFIG,
#endif
};

struct stm32_dac
{
    struct rt_dac_device stm32_dac_device;
};

static struct stm32_dac stm32_dac_obj[sizeof(dac_config) / sizeof(dac_config[0])];


static rt_err_t stm32_dac_enabled(struct rt_dac_device *device, rt_uint32_t channel)
{
    RT_ASSERT(device != RT_NULL);
    //DAC_HandleTypeDef *stm32_dac_handler = device->parent.user_data;

    if (channel != 1)
    {
        LOG_E("dac channel must be 1");
        return -RT_ERROR;
    }
    stc_dac_cfg_t dac_initstruct;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralDac, TRUE); ///< 使能DAC模块的时钟

    dac_initstruct.boff_t = DacBoffDisable;
    dac_initstruct.ten_t  = DacTenDisable;
    dac_initstruct.sref_t = DacVoltageAvcc;
    dac_initstruct.mamp_t = DacMenp4095;
    dac_initstruct.tsel_t = DacSwTriger;   ///< 软件触发方式
    dac_initstruct.align  = DacRightAlign; ///< 右对齐
    Dac_Init(&dac_initstruct);

    Dac_Cmd(TRUE);

    return RT_EOK;
}

static rt_err_t stm32_dac_disabled(struct rt_dac_device *device, rt_uint32_t channel)
{
    
    RT_ASSERT(device != RT_NULL);
    //DAC_HandleTypeDef *stm32_dac_handler = device->parent.user_data;

    if (channel != 1)
    {
        LOG_E("dac channel must be 1");
        return -RT_ERROR;
    }
    Dac_Cmd(FALSE);

    return RT_EOK;
}

static rt_uint8_t stm32_dac_get_resolution(struct rt_dac_device *device)
{
    RT_ASSERT(device != RT_NULL);
    DAC_HandleTypeDef *stm32_dac_handler = device->parent.user_data;
    (void)stm32_dac_handler;

    /* Only has supported DAC_ALIGN_12B_R, so it will return 12 bits */
    return 12;
}

static rt_err_t stm32_set_dac_value(struct rt_dac_device *device, rt_uint32_t channel, rt_uint32_t *value)
{
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(value != RT_NULL);
    //DAC_HandleTypeDef *stm32_dac_handler = device->parent.user_data;

    if (channel != 1)
    {
        LOG_E("dac channel must be 1");
        return -RT_ERROR;
    }

    Dac_SetChannelData(DacRightAlign, DacBit12, *value);
    Dac_SoftwareTriggerCmd();

    return RT_EOK;
}

static const struct rt_dac_ops stm_dac_ops =
    {
        .disabled       = stm32_dac_disabled,
        .enabled        = stm32_dac_enabled,
        .convert        = stm32_set_dac_value,
        .get_resolution = stm32_dac_get_resolution,
};

static int stm32_dac_init(void)
{
    int result = RT_EOK;
    /* save dac name */
    char name_buf[5] = {'d', 'a', 'c', '0', 0};
    int i            = 0;

    /* register dac device */
    if (rt_hw_dac_register(&stm32_dac_obj[i].stm32_dac_device, name_buf, &stm_dac_ops, &dac_config) == RT_EOK)
    {
        LOG_D("%s init success", name_buf);
    }
    else
    {
        LOG_E("%s register failed", name_buf);
        result = -RT_ERROR;
    }
    return result;
}
INIT_DEVICE_EXPORT(stm32_dac_init);

#endif /* BSP_USING_DAC */
