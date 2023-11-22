/*
****************************************Copyright(c)************************************
**                                  Zhengzhou GL. TECH Co.,Ltd
**___________________________File Info_________________________
**文件名称：lora_radio_control.h
**工程项目：
**说    明：本文件用于SEMTECH公司SX系列射频模块控制，多系列模组通用，支持单板多模组控制
            使用本文件需要根据主控芯片外部定义相关底层操作函数
**
**作    者：吴树          日    期：2022年01月10日
**建立版本：V1.0
**
**________________________modification_________________________
**版本            日期               升级记录
**V1.0        2022年01月10日     初始版本构建
*****************************************************************************************
*/

#include "lora_radio_control.h" //使用lora驱动
#include <string.h>             //使用字符串比较函数   //华为要求使用安全函数，持续研究

// 为了方便使用功能，loranum从0开始编号
// 外部定义参数，使用方式一般是在外部根据板卡上的lora数量定义对应数据的数组，并把数组起始指针赋值给该参数

/*
==================================================================
== 变量声明
==================================================================
*/

static LORA_CHIPTYPE_E *loraChipTypeIndex = NULL;         // 板卡使用的lora模块类型变量的起始指针，该参数在外部文档定义，需要和用户编辑模块支持类型匹配
static LORA_CONTROL_FUNC_T *loraControlFuncsIndex = NULL; // lora控制函数指针变量的起始指针，该参数在外部文档定义

/*
==================================================================
== 函数定义
==================================================================
*/

/*
==================================================================
** 函数名称： LORA_DriverRegistration
** 功    能： lora操作函数注册，支持多模组注册方案,需要在使用本驱动前调用
              因为lora驱动支持多类型模组，因此同时注册芯片类型
              注册过程中，将验证版权信息
** 输入参数： index：版权校验信息指针
              type：使用模块类型数据变量指针，多模块使用数组名形式传递
                          control：操作函数变量指针，多模块使用数组名形式传递
** 输出参数： 无
** 返    回： 返回值0表示注册成功，返回值1表示注册失败
==================================================================
*/
uint8_t LORA_DriverRegistration(const char *index, LORA_CHIPTYPE_E *type, LORA_CONTROL_FUNC_T *control)
{
    char password[] = "Zhengzhou GL. TECH Co.,Ltd";

    if (strcmp(index, password)) // 声明比对失败
    {
        return 1; // 表明注册失败，请检查版权验证信息
    }

    loraChipTypeIndex = type;
    loraControlFuncsIndex = control;

    return 0; // 注册成功
}

/*
==================================================================
** 函数名称： LORA_HardwareReset
** 功    能： lora 复位控制函数，实现lora芯片的硬件复位
** 输入参数： loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_HardwareReset(uint8_t loranum)
{
    (loraControlFuncsIndex + loranum)->ResetControlFunc(0); // 拉低复位引脚
    (loraControlFuncsIndex + loranum)->DelayUsFunc(5000);   // 1ms足够了，手册只需要微秒级,多预留一些余量
    (loraControlFuncsIndex + loranum)->ResetControlFunc(1);
    (loraControlFuncsIndex + loranum)->DelayUsFunc(10000); // 预留足够的时间，保证芯片复位完成，reset之后要约5ms之后才能操作
}

/*
==================================================================
** 函数名称： LORA_Init
** 功    能： lora 射频配置初始化控制函数，需要传递lora模块的工作模式和配置参数
** 输入参数： settings：射频配置参数变量指针
              loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 返回值0表示初始化成功，非0表示模块访问失败
==================================================================
*/
uint8_t LORA_Init(const LORA_RADIO_SETTING_T *settings, uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum)) // 判定模块类型
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        return (Sx126xInit(settings, (loraControlFuncsIndex + loranum)));
#endif

    default:
        return 1;
    }
}

/*
==================================================================
** 函数名称： LORA_SetSleep
** 功    能： 切换lora工作状态到休眠状态
** 输入参数： loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_SetSleep(uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xSetSleep(loraControlFuncsIndex + loranum);
        break;
#endif

    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_SetStdby
** 功    能： 切换lora工作状态到待机状态
** 输入参数： loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_SetStdby(uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xSetStdby(loraControlFuncsIndex + loranum);
        break;

#endif

    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_SetReceive
** 功    能： 切换lora工作状态到接收状态
** 输入参数： timeout：接收超时配置，0表示不开启超时判定
                      0xffffffff表示开启连续接收，接收完成后自动切换再次接收
                                            其他配置值均表示单次接收并开启超时判定
                                            单位为微秒（us）
             loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_SetReceive(uint32_t timeout, uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xSetRx(timeout, (loraControlFuncsIndex + loranum));
        break;

#endif

    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_Awake
** 功    能： 从休眠模式下切换lora工作状态到待机状态
** 输入参数： loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_Awake(uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xWakeup(loraControlFuncsIndex + loranum);
        break;

#endif

    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_SendDataLora
** 功    能： 使用lora方式发送信息
              本版本驱动，不对外部额外的功放进行控制，
                            如果使用带有外部功放或低噪放的模块，需要自行添加额外的控制
** 输入参数： index：待发送数据的指针
             length：待发送数据的长度
             loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_SendDataLora(uint8_t *index, uint8_t length, uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xLoraSendData(index, length, (loraControlFuncsIndex + loranum));
        break;
#endif

    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_SendDataGfsk
** 功    能： 使用gfsk方式发送信息
              本版本驱动，不对外部额外的功放进行控制，
                            如果使用带有外部功放或低噪放的模块，需要自行添加额外的控制
** 输入参数： index：待发送数据的指针
             length：待发送数据的长度
             loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_SendDataGfsk(uint8_t *index, uint8_t length, uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xGfskSendData(index, length, (loraControlFuncsIndex + loranum));
        break;
#endif

    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_ReceiveData
** 功    能： 提取lora模块的接收数据到指定缓存
** 输入参数： index：接收数据的缓存指针
             max：允许接收数据的最大长度，如果lora接收数据过长将会丢弃
             loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 接收数据的长度
==================================================================
*/
uint8_t LORA_ReceiveData(uint8_t *index, uint8_t max, uint8_t loranum)
{
    uint8_t length = 0;

    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        length = Sx126xGetPayload(index, max, (loraControlFuncsIndex + loranum));
        break;
#endif

    default:
        break;
    }
    return length;
}

/*
==================================================================
** 函数名称： LORA_GetIrqFlag
** 功    能： lora 提取中断标志
** 输入参数： loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 经过处理和统一后的中断类型
             当前支持接收完成、发送完成、以及错误中断
==================================================================
*/
LORA_IRQ_STATE_E LORA_GetIrqFlag(uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        return Sx126xGetIrqStateSimplify(loraControlFuncsIndex + loranum);
#endif

    default:
        return LORA_IRQ_ERROR;
    }
}

/*
==================================================================
** 函数名称： LORA_ClearIrqFlag
** 功    能： lora 清除中断标志
** 输入参数： loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_ClearIrqFlag(uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xClearIrqStatus(SX126X_IRQ_RADIO_ALL, (loraControlFuncsIndex + loranum));
        break;
#endif
    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_SetFrequency
** 功    能： lora配置工作频率
** 输入参数： fre:工作频率，单位Hz
             loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_SetFrequency(uint32_t fre, uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xSetRfFrequency(fre, (loraControlFuncsIndex + loranum));
        break;
#endif

    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_SetLoraModuleParas
** 功    能： lora配置lora模式下的调制参数，包括sf，bw，cr以及低速率优化
** 输入参数： settings:lora模式下的调制参数变量
             loranum：控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_SetLoraModuleParas(LORA_MODULE_PARAS_T settings, uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xSetLoraModulationParams(settings, (loraControlFuncsIndex + loranum));
        break;
#endif
    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_SetGfskModuleParas
** 功    能： lora配置gfsk模式下的调制参数
** 输入参数： settings:lora模式下的调制参数变量
              loranum:控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_SetGfskModuleParas(LORA_GFSK_MODULE_PARAS_T settings, uint8_t loranum)
{
    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xSetGfskModulationParams(settings, (loraControlFuncsIndex + loranum));
        break;
#endif
    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_GetRssiSnrLora
** 功    能： 在lora工作模式下，获取模块接收信号的信号质量相关的参数
** 输入参数： loranum:控制的lora编号，从0开始
** 输出参数： pktStatus:获取的信号质量参数
** 返    回： 无
==================================================================
*/
void LORA_GetRssiSnrLora(LORA_RX_SIGNAL_STATUS_T *pktStatus, uint8_t loranum)
{

    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xGetLoRaPacketStatus(pktStatus, (loraControlFuncsIndex + loranum));
        break;
#endif
    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_GetRssiSnrGfsk
** 功    能： 在gfsk工作模式下，获取模块接收信号的信号质量相关的参数
** 输入参数： loranum:控制的lora编号，从0开始
** 输出参数： pktStatus:获取的信号质量参数
** 返    回： 无
==================================================================
*/
void LORA_GetRssiSnrGfsk(LORA_RX_SIGNAL_STATUS_T *pktStatus, uint8_t loranum)
{

    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xGetGfskPacketStatus(pktStatus, (loraControlFuncsIndex + loranum));
        break;
#endif
    default:
        break;
    }
}

/*
==================================================================
** 函数名称： LORA_SetTxContinuousWave
** 功    能： lora配置为连续发送模式，不需要额外的中断处理，功率测试专用
** 输入参数： loranum:控制的lora编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_SetTxContinuousWave(uint8_t loranum)
{

    switch (*(loraChipTypeIndex + loranum))
    {
#ifdef LORA_SX126x_SUPPORT
    case LORA_1261:
    case LORA_1262:
    case LORA_LLCC68:
        Sx126xSetTxContinuousWave(loraControlFuncsIndex + loranum);
        break;
#endif

    default:
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
