/**********************************Copyright (c)********************************
**                            ZhengZhou GuangLi Tech CO.,LTD
**                                All rights reserved.
**
**----------------------------------项目信息------------------------------------
** 文件名称： CommHost.h
** 工程项目：
** 说    明： 通讯处理程序头文件。
**
**----------------------------------版本历史------------------------------------
** 版本号	修改者	完成日期		内容
** V1.0     郭洪斌  2013.12
*******************************************************************************/

#ifndef __COMM_LORA_H_
#define __COMM_LORA_H_

/*
*********************************************************************************************************
*                                           包含头文件
*********************************************************************************************************
*/
#include <stdint.h>
// hwx 2023.08.10 add.
typedef struct WIFI_DATA
{
    uint8_t len;

    uint8_t workMode;
    uint8_t workChnnl;
    uint8_t pree_pump_state;
    uint8_t pree_pump_Err;

    uint8_t msr_pump_state;
    uint8_t msr_pump_Err;
    uint8_t Press_state;
    uint8_t chnnlFlowAvail;

    uint16_t cntWorkTime;

    float Press_Value;
    float chnnlFlow;

} WIFI_DATA;

extern char LoraBuf[100];   // 收到新数据的缓存区
extern uint8_t LoraDipflag; // 收到新数据标识
extern uint32_t loradata;   // 数据发送标识
/*
*********************************************************************************************************
*                                           类型和宏定义
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           输出的函数
*********************************************************************************************************
*/
void CommLoraInit(void);
void CommLoraDeInit(void);
void CommLoraHandler(void);
void CommLoraTick(void);
uint8_t GetWLessChnnl(void);
void CommPrvChnnlConfig(uint8_t chnnl);
void Wifi_Power_Ctrl(uint8_t state);

#endif

/***************************** END OF FILE ************************************/
