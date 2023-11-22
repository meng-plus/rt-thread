/*
***************************************Copyright (c)************************************
**                               Zhengzhou GL. TECH Co.,Ltd
**----------------------------------------File Info---------------------------------------
** 文件名称：
** 工程项目：
** 说    明：
**
** 作    者：              日    期：
** 建立版本：
**
**----------------------------------------modification------------------------------------
** 作    者：
** 日    期：
** 版    本：            标    记：
** 说    明：
**
*****************************************************************************************
*/

// #include "LPC407x_8x.h"
#include "LPC407x_8x_177x_8x.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "sx126x_driver.h"
#include "lora_base_control.h"
#include "CommLora.h"
// #include <glmath.h>
// #include "hSch.h"
// #include "IAP.h"

// #include "rtdbg.h"

/*
==========================================================================
= 宏定义
==========================================================================
*/
#define COMM_WLESS_MAXTEXT 240 /* 数据传输最大数据字节数 */
#define WLESS_BDCTADDR 255     /* 广播地址 */
#define TIME_UINT 500          /* 注册时间单位 ms*/
#define TIMESLICE_UINT 100     /* 时间片单位 ms*/

#define RELCONTIME 30    /* 解除连接固定基站限制时间 s*/
#define RETURNCOMTIME 20 /* 在线仪器返回公共信道时间 s*/

/*
==========================================================================
= 变量定义
==========================================================================
*/
extern LORA_CHIPTYPE_E g_loraType[];

char LoraBuf[100];
uint8_t LoraDipflag;
uint32_t loradata;

//=========================
//= 无线配置
//=========================
// 参数配置
static LORA_RADIO_SETTING_T g_loraTestConfig =
    {
        .type = LORA_1262,                   // 驱动模式
        .mode = WORK_MODEM_LORA,             // 通讯模式
        .frequency = 902000000,              // 通讯信道
        .txpower = 20,                       // 发送功率
        .modulationparams.sf = LORA_SF7,     // 扩频因子
        .modulationparams.bw = LORA_BW_500K, // 带宽
        .modulationparams.cr = LORA_CR_4_5,  // 循环编码率
        .modulationparams.ldro = 0,          // 低速率优化，0表示不开启，1表示开启（low_data_rate_optimize）
};

// 无线驱动版权信息
static const char g_systemCopyright[] = "Zhengzhou GL. TECH Co.,Ltd";

//=========================
//= 无线通讯
//=========================

/*
==========================================================================
= 函数定义
==========================================================================
*/
/*
*********************************************************************************
** 函数名称: int Power_GetState(POWER_PORT_E port)
** 功    能: 获取电源模块状态
** 输入参数: port			电源模块接口
** 输出参数: 无
** 返    回: 电源模块状态。
				0		关闭
				1		打开
**********************************************************************************
*/

/*==================================================================
** 函数名称：
** 功    能：
** 输入参数：
** 输出参数：
** 返    回：
==================================================================*/
void Lora_Power_Ctrl(uint8_t state)
{
  if (state == 0)
	LPC_GPIO2->CLR = (1ul << 21);
	else
	LPC_GPIO2->SET = (1ul << 21);
}


/*
*********************************************************************************
** 函数名称: int Power_GetState(POWER_PORT_E port)
** 功    能: 获取电源模块状态
** 输入参数: port			电源模块接口
** 输出参数: 无
** 返    回: 电源模块状态。
				0		关闭
				1		打开
**********************************************************************************
*/
uint8_t LoraPower_GetState(void)
{
  if ((LPC_GPIO2->PIN & (1ul << 21)) == 0)
		return 0;
	else
		return 1;
}


/*==================================================================
** 函数名称：
** 功    能：
** 输入参数：
** 输出参数：
** 返    回：
==================================================================*/
void Wifi_Power_Ctrl(uint8_t state)
{
  if (state == 0)
	LPC_GPIO0->CLR = (1ul << 4);
	else
	LPC_GPIO0->SET = (1ul << 4);
}


/*
*********************************************************************************
** 函数名称: int Power_GetState(POWER_PORT_E port)
** 功    能: 获取电源模块状态
** 输入参数: port			电源模块接口
** 输出参数: 无
** 返    回: 电源模块状态。
				0		关闭
				1		打开
**********************************************************************************
*/
uint8_t WifiPower_GetState(void)
{
  if ((LPC_GPIO0->PIN & (1ul << 4)) == 0)
		return 0;
	else
		return 1;
}
/*
*********************************************************************************
** 函数名称:
** 功    能: Lora模组初始化
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
void CommLoraInit(void)
{
    Lora_Power_Ctrl(1); // Lora模块 5V3板
    //DelayMs(50);

    g_loraTestConfig.frequency = (900 + 2) * 1000000;

    //=========================
    //= 传感器初始化
    //=========================
    LORA_HwInit(0);

    g_loraType[0] = g_loraTestConfig.type;

    LORA_DriverRegistration(g_systemCopyright, g_loraType, g_loraControlFuncs);

    LORA_HardwareReset(0); // 模块复位

    if (LORA_Init(&g_loraTestConfig, 0) == 0)
    {
        LORA_SetReceive(0xffffffff, 0);
    }

    LORA_ClearIrqFlag(0);
}

/*
*********************************************************************************
** 函数名称:
** 功    能: Lora模组休眠
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
void CommLoraDeInit(void)
{
    LORA_SetSleep(0);
    Lora_Power_Ctrl(0); // Lora模块 5V3板
}

/*
*********************************************************************************
** 函数名称: CommWLessHandler
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
void CommLoraHandler(void)
{
    uint8_t LoraRev_Buff[COMM_WLESS_MAXTEXT];
    LORA_IRQ_STATE_E state;
    int len;

    if (LoraPower_GetState() == 0)
        return;

    //=========================
    //= 通讯处理
    //=========================
    if (LORA_GetFlagIRQ(0) != 0)
    {
        state = LORA_GetIrqFlag(0);

        switch (state)
        {
        case LORA_IRQ_RXDONE: /* 接收数据			*/
            len = LORA_ReceiveData(LoraRev_Buff, COMM_WLESS_MAXTEXT, 0);
            if (len < 10)
                break;

            if (len == LoraRev_Buff[0])
            {
                memcpy(&LoraBuf[0], LoraRev_Buff, len);
                LoraDipflag = 1;
            }
            break;

        case LORA_IRQ_TXDONE: /* 发送数据			*/
            LORA_SetReceive(0xffffffff, 0);
            break;

        case LORA_IRQ_TIMEOUT: /* 超时					*/
            LORA_SetReceive(0xffffffff, 0);
            break;
        default:
            break;
        }

        LORA_ClearIrqFlag(0);
    }
}

/*
*********************************************************************************
** 函数名称: CommWLessTick
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/

void CommLoraTick(void)
{
    if (LoraPower_GetState() == 0)
        return;

    if (loradata == 0)
        return;

    WIFI_DATA Wifi_Data_Temp;
    //
    //    Wifi_Data_Temp.len = sizeof(WIFI_DATA);
    //    Wifi_Data_Temp.workMode = workMode;
    //    Wifi_Data_Temp.workChnnl = workChnnl;
    //    Wifi_Data_Temp.pree_pump_state = pumpMp.state.pree;
    //    Wifi_Data_Temp.pree_pump_Err = pumpMp.state.preeErr;
    //
    //    Wifi_Data_Temp.msr_pump_state = pumpMp.state.msr;
    //    Wifi_Data_Temp.msr_pump_Err = pumpMp.state.msrErr;
    //
    //    Wifi_Data_Temp.Press_state = pumpPress.state.msrAct;
    //    Wifi_Data_Temp.Press_Value = pumpPress.state.press;
    //
    //    Wifi_Data_Temp.cntWorkTime = cntWorkTime;
    //
    //    Wifi_Data_Temp.chnnlFlowAvail = chnnlFlowAvail;
    //    Wifi_Data_Temp.chnnlFlow = chnnlFlow;

    LORA_SendDataLora((uint8_t *)(&Wifi_Data_Temp), sizeof(WIFI_DATA), 0);
}

/*
*********************************************************************************
** 函数名称: GetRelConCnt
** 功    能: 查询当前通讯是否进入限制状态
             若通讯进入限制状态一直未解除，仪器不可提前休眠
             需等限制结束后，再等待至少10s保证仪器可以与其他基站通讯
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
uint8_t GetWLessChnnl(void)
{
    return (uint8_t)((g_loraTestConfig.frequency / 1000000) % 900);
}

/*
*********************************************************************************
** 函数名称:
** 功    能: 切换无线信道
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
void CommPrvChnnlConfig(uint8_t chnnl)
{
    g_loraTestConfig.frequency = (900 + chnnl) * 1000000;
    LORA_SetFrequency(g_loraTestConfig.frequency, 0);

    LORA_SetReceive(0xffffffff, 0);
}
