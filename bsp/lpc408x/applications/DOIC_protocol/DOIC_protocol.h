/**
 * @file DOIC_protocol_def.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 分布式在线仪表通信协议
 * @version 0.1
 * @date 2023-11-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __DOIC_PROTOCOL_H_
#define __DOIC_PROTOCOL_H_

#include "DOIC_protocol_def.h"
#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief 主机报文解析映射表
     *
     */
    typedef struct DOIC_MAP
    {
        uint8_t cmd;
        uint8_t scmd;
        int8_t (*func)(doic_data_t *pdata, doic_data_t *pdst);
    } doic_map_t;

    typedef struct DOIC_MASTER
    {
        doic_data_t *pdata;
        const doic_map_t *map;
        uint16_t map_num;
        uint8_t addr; /*!< 本机地址 */
    } doic_master_t;

    /**
     * @brief 解析回应的报文
     *
     * @param pdoic
     * @return int8_t
     */
    void DOIC_master_init(doic_master_t *phander, doic_map_t *pmap, uint16_t map_num);
    /**
     * @brief 数据透传——0x41  0x01 查询下位机上传的透
     *
     * @param phander 主机句柄
     * @param slave_addr 从机地址
     * @param buff 透传数据
     * @param len 透传长度
     * @return 报文长度
     */
    uint16_t DOIC_master_0x4181(doic_master_t *phander, uint8_t slave_addr, uint8_t sub_addr, uint8_t *buff, uint8_t len);
    /**
     * @brief 报文解析
     *
     * @param phander 主机句柄
     * @param pdst  回应报文句柄
     * @return int8_t 0正常
     */
    int8_t DOIC_deal(doic_master_t *phander, doic_data_t *pdst);
    uint16_t DOIC_getCrc(doic_data_t *pdata);
    uint16_t DOIC_checkCrc(doic_data_t *pdata);
    /**
     * @brief 计算crc并追加到最后
     *
     * @param pdata
     * @return uint16_t
     */
    uint16_t DOIC_addCrc(doic_data_t *pdata);
#ifdef __cplusplus
}
#endif
#endif
