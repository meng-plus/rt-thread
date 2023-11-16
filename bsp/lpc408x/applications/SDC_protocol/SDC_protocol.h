
/**
 * @file SDC_protocol_def.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 分站下行通信协议
 * @version 0.1
 * @date 2023-11-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __SDC_PROTOCOL_H_
#define __SDC_PROTOCOL_H_

#include "SDC_protocol_def.h"
#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * @brief 主机报文解析映射表
     *
     */
    typedef struct SDC_MAP
    {
        uint8_t cmd;
        int8_t (*func)(sdc_data_t *pdata, sdc_data_t *pdst);
    } sdc_map_t;

    typedef struct DSC_MASTER
    {
        sdc_data_t *pdata;
        const sdc_map_t *map;
        uint16_t map_num;
        uint8_t addr; /*!< 本机地址 */
    } sdc_master_t;

    /**
     * @brief 解析回应的报文
     *
     * @param psdc
     * @return int8_t
     */
    void SDC_master_init(sdc_master_t *phander, uint8_t *buff, uint16_t len);
    /**
     * @brief 分站查询模拟量传感器
     *
     * @param phander 设备句柄
     * @param slave_addr 目标设备地址
     * @param sen_type 传感器类型 @ref SENSOR_DEF
     * @param chn 通道号
     * @return uint16_t
     */
    uint16_t SDC_master_0x10(sdc_master_t *phander, uint8_t slave_addr, enum SENSOR_DEF sensor_type, uint8_t chn);
    /**
     * @brief 报文解析
     *
     * @param phander 主机句柄
     * @param pdst  回应报文句柄
     * @return int8_t 0正常
     */
    int8_t SDC_deal(sdc_master_t *phander, sdc_data_t *pdst);
    uint16_t SDC_getCrc(sdc_data_t *pdata);
    uint16_t SDC_checkCrc(sdc_data_t *pdata);
    /**
     * @brief 计算crc并追加到最后
     *
     * @param pdata
     * @return uint16_t
     */
    uint16_t SDC_addCrc(sdc_data_t *pdata);

#ifdef __cplusplus
}
#endif

#endif
