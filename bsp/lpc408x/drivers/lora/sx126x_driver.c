/*
****************************************Copyright(c)************************************
**                                  Zhengzhou GL. TECH Co.,Ltd
**___________________________File Info_________________________
**文件名称：sx126x_driver.c
**工程项目：通用工程
**说    明：本文件用于SEMTECH公司SX126X以及LLCC68系列射频模块控制
            本文件同样可以用于stm32WL系列射频模块的控制（同IP核）
            使用本文件需要根据主控芯片外部定义相关底层操作函数
**
**作    者：吴树          日    期：2022年02月11日
**建立版本：V1.1
**
**________________________modification_________________________
**版本            日期               升级记录
**V1.0        2022年01月10日     初始版本构建
**V1.1        2022年02月11日     增加对stm32wl系列的支持，增加芯片引脚功能配置
*****************************************************************************************
*/

#include "sx126x_driver.h"

/*
==================================================================
== 宏定义
==================================================================
*/

// sx126x的频率步进参数
#define SX126X_XTAL_FREQ 32000000                                // 外部晶振频率
#define SX126X_FREQ_STEP (((double)SX126X_XTAL_FREQ / 33554432)) // 2^25次方

SX126x_LORA_PACKT_PARAS_T sx126x_packet_params = {
    12,
    SX126X_LORA_PACKET_VARIABLE_LENGTH,
    255,
    SX126X_LORA_CRC_ON,
    SX126X_LORA_IQ_NORMAL}; // 暂时使用默认参数

SX126x_RADIO_GFSK_PACKET_PARAM_T sx126x_gfsk_packet_params = {
    64,
    SX126X_RADIO_PREAMBLE_DETECTOR_16_BITS,
    8,
    SX126X_RADIO_ADDRESSCOMP_FILT_OFF,
    SX126X_RADIO_PACKET_VARIABLE_LENGTH,
    255,
    SX126X_RADIO_CRC_OFF, SX126X_RADIO_DC_FREEWHITENING}; // 暂时使用默认参数

/*
==================================================================
== 内部变量定义
==================================================================
*/

static const uint8_t sx126xSfTransferBuffer[8] = {5, 6, 7, 8, 9, 10, 11, 12};      // lora sf对应的配置参数
static const uint8_t sx126xBwTransferBuffer[10] = {6, 5, 4, 3, 10, 2, 9, 1, 8, 0}; // lora bw对应的配置参数
static const uint8_t sx126xCrTransferBuffer[4] = {1, 2, 3, 4};                     // lora cr对应的配置参数

static const uint8_t sx126xGfskShapingTransferBuffer[5] = {0, 8, 9, 10, 11}; // gfsk 脉冲成型配置参数
static const uint8_t sx126xGfskBwTransferBuffer[21] = {
    0x1F,
    0x17,
    0x0F,
    0x1E,
    0x16,
    0x0E,
    0x1D,
    0x15,
    0x0D,
    0x1C,
    0x14,
    0x0C,
    0x1B,
    0x13,
    0x0B,
    0x1A,
    0x12,
    0x0A,
    0x19,
    0x11,
    0x09,
}; // gfsk 带宽配置参数

/////////////////////////////异常记录控制命令//////////////////////////////////////

// 多模组共用，每当busy引脚卡死后，该数值加1，提供函数接口读取和清0，外部可以调用用于检测之前操作的模组是否存在busy卡死问题
// 该参数不主动清0
static uint16_t sx126x_error_flag = 0;

/*
==================================================================
== 函数定义
==================================================================
*/

/*
==================================================================
** 函数名称： Sx126xWaitOnBusy
** 功    能： 循环读取芯片busy引脚状态，用于等待芯片处于可操作状态
** 输入参数： spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 返回值0表示正常，1表示芯片出现问题，需要修复
==================================================================
*/
uint8_t Sx126xWaitOnBusy(LORA_CONTROL_FUNC_T *spi_control)
{
    uint16_t times = 500; // 暂定

    while ((spi_control->BusyReadFunc()) && (times--))
        ;

    if (times == 0) // busy引脚一直处于高电平，芯片异常
    {
        sx126x_error_flag++;
        return 1;
    }
    else
        return 0;
}

/*
==================================================================
** 函数名称： Sx126xReadErrorFlag
** 功    能： 读取错误计数，对外提供
** 输入参数： 无
** 输出参数： 无
** 返    回： 返回值表示自行判定的异常次数
==================================================================
*/
uint16_t Sx126xReadErrorFlag(void)
{
    return sx126x_error_flag;
}

/*
==================================================================
** 函数名称： Sx126xClearErrorFlag
** 功    能： 主动清0错误计数，一般在初始化前执行，对外提供
** 输入参数： 无
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xClearErrorFlag(void)
{
    sx126x_error_flag = 0;
}

/////////////////////////////基本读写命令、读写寄存器和缓存函数////////////////////////

/*
==================================================================
** 函数名称： Sx126xWriteCommand
** 功    能： sx126x 写命令，内部命令，不对外提供
** 输入参数： command：接口指令类型，参考手册以及芯片宏定义说明
              buffer：指向写入命令具体参数的缓存的指针
              size：写入数据长度
             spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xWriteCommand(SX126x_COMMANDS_E command, uint8_t *buffer, uint16_t size, LORA_CONTROL_FUNC_T *spi_control)
{
    uint16_t i;

    if (Sx126xWaitOnBusy(spi_control)) // 等待芯片处于可通信状态
    {
        return; // 芯片异常
    }

    spi_control->NssControlFunc(0); // 片选低有效

    spi_control->SpiByteWriteReadFunc((uint8_t)command); // 写入命令

    for (i = 0; i < size; i++) // 写入具体数据
    {
        spi_control->SpiByteWriteReadFunc(buffer[i]);
    }

    spi_control->NssControlFunc(1); // 取消片选
}

/*
==================================================================
** 函数名称： Sx126xReadCommand
** 功    能： sx126x 读命令，内部命令，不对外提供
** 输入参数： command：接口指令类型，参考手册以及芯片宏定义说明
              buffer：指向存储返回参数的缓存的指针
              size：读取数据长度
             spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xReadCommand(SX126x_COMMANDS_E command, uint8_t *buffer, uint16_t size, LORA_CONTROL_FUNC_T *spi_control)
{
    uint16_t i;

    if (Sx126xWaitOnBusy(spi_control)) // 等待芯片处于可通信状态
    {
        return;
    }

    spi_control->NssControlFunc(0); // 片选低有效

    spi_control->SpiByteWriteReadFunc((uint8_t)command); // 写入命令
    spi_control->SpiByteWriteReadFunc(0x00);             // 多写入一个0，表示读取

    for (i = 0; i < size; i++) // 读取数据到缓存
    {
        buffer[i] = spi_control->SpiByteWriteReadFunc(0);
    }
    spi_control->NssControlFunc(1); // 取消片选
}

/*
==================================================================
** 函数名称： Sx126xWriteBuffer
** 功    能： sx126x 往数据缓存区写入数据，内部命令，不对外提供
** 输入参数： offset：写入地址偏移
              buffer：指向写入数据的缓存的指针
              size：写入数据长度
             spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xWriteBuffer(uint8_t offset, uint8_t *buffer, uint8_t size, LORA_CONTROL_FUNC_T *spi_control)
{
    uint16_t i;

    if (Sx126xWaitOnBusy(spi_control)) // 等待芯片处于可通信状态
    {
        return;
    }

    spi_control->NssControlFunc(0); // 片选低有效

    spi_control->SpiByteWriteReadFunc(SX126X_RADIO_WRITE_BUFFER); // 默认写入缓存命令
    spi_control->SpiByteWriteReadFunc(offset);                    // 写入地址偏移
    for (i = 0; i < size; i++)                                    // 写入具体数据
    {
        spi_control->SpiByteWriteReadFunc(buffer[i]);
    }
    spi_control->NssControlFunc(1); // 取消片选
}

/*
==================================================================
** 函数名称： Sx126xReadBuffer
** 功    能： sx126x 从数据缓存区读取数据，内部命令，不对外提供
** 输入参数： offset：读取地址偏移
              buffer：指向存储返回参数的缓存的指针
              size：读取数据长度
             spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xReadBuffer(uint8_t offset, uint8_t *buffer, uint8_t size, LORA_CONTROL_FUNC_T *spi_control)
{
    uint16_t i;
    if (Sx126xWaitOnBusy(spi_control)) // 等待芯片处于可通信状态
    {
        return;
    }

    spi_control->NssControlFunc(0); // 片选低有效

    spi_control->SpiByteWriteReadFunc(SX126X_RADIO_READ_BUFFER); // 默认读取缓存命令
    spi_control->SpiByteWriteReadFunc(offset);                   // 写入地址偏移
    spi_control->SpiByteWriteReadFunc(0);                        // 读取流程需要多发送一个0
    for (i = 0; i < size; i++)                                   // 读取数据
    {
        buffer[i] = spi_control->SpiByteWriteReadFunc(0);
    }
    spi_control->NssControlFunc(1); // 取消片选
}

/*
==================================================================
** 函数名称： Sx126xWriteRegisters
** 功    能： sx126x 从起始位置开始修改寄存器值，可以连续修改多个寄存器
              内部命令，不对外提供
** 输入参数： address：起始寄存器地址
              buffer：指向写入数据的缓存的指针
              size：写入数据长度
             spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xWriteRegisters(uint16_t address, uint8_t *buffer, uint16_t size, LORA_CONTROL_FUNC_T *spi_control)
{
    uint16_t i;

    if (Sx126xWaitOnBusy(spi_control)) // 等待芯片处于可通信状态
    {
        return;
    }
    spi_control->NssControlFunc(0); // 片选低有效

    spi_control->SpiByteWriteReadFunc(SX126X_RADIO_WRITE_REGISTER);
    spi_control->SpiByteWriteReadFunc((address & 0xFF00) >> 8);
    spi_control->SpiByteWriteReadFunc(address & 0x00FF);

    for (i = 0; i < size; i++)
    {
        buffer[i] = spi_control->SpiByteWriteReadFunc(buffer[i]);
    }

    spi_control->NssControlFunc(1); // 取消片选
}

/*
==================================================================
** 函数名称： Sx126xWriteRegister
** 功    能： sx126x 单独配置一个寄存器
              内部命令，不对外提供
** 输入参数： address：起始寄存器地址
              value：寄存器配置值
             spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xWriteRegister(uint16_t address, uint8_t value, LORA_CONTROL_FUNC_T *spi_control)
{
    Sx126xWriteRegisters(address, &value, 1, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xReadRegisters
** 功    能： sx126x 从起始位置开始读取寄存器值，可以连续读取多个寄存器
              内部命令，不对外提供
** 输入参数： address：起始寄存器地址
              buffer：指向读取数据的缓存的指针
              size：读取数据长度
             spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xReadRegisters(uint16_t address, uint8_t *buffer, uint16_t size, LORA_CONTROL_FUNC_T *spi_control)
{
    uint16_t i;
    if (Sx126xWaitOnBusy(spi_control)) // 等待芯片处于可通信状态
    {
        return;
    }

    spi_control->NssControlFunc(0); // 片选低有效

    spi_control->SpiByteWriteReadFunc(SX126X_RADIO_READ_REGISTER);
    spi_control->SpiByteWriteReadFunc((address & 0xFF00) >> 8);
    spi_control->SpiByteWriteReadFunc(address & 0x00FF);
    spi_control->SpiByteWriteReadFunc(0);
    for (i = 0; i < size; i++)
    {
        buffer[i] = spi_control->SpiByteWriteReadFunc(0);
    }
    spi_control->NssControlFunc(1); // 取消片选
}

/*
==================================================================
** 函数名称： SX126xReadRegister
** 功    能： sx126x 单独读取一个寄存器
              内部命令，不对外提供
** 输入参数： address：起始寄存器地址
             spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 读取的寄存器的值
==================================================================
*/
uint8_t SX126xReadRegister(uint16_t address, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t data;
    Sx126xReadRegisters(address, &data, 1, spi_control);
    return data;
}

/*
==================================================================
** 函数名称： SX126xGetStatus
** 功    能： 读取当前芯片状态，可用于外部获取芯片状态验证
** 输入参数： spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 读取的芯片状态
==================================================================
*/
SX126x_RADIO_STATUS_T SX126xGetStatus(LORA_CONTROL_FUNC_T *spi_control)
{
    //    uint8_t stat = 0;
    SX126x_RADIO_STATUS_T status;

    Sx126xReadCommand(SX126X_RADIO_GET_STATUS, (uint8_t *)&status, 1, spi_control);

    return status;
}

/*
==================================================================
** 函数名称： SX126xGetDeviceErrors
** 功    能：获取设备错误状态
** 输入参数： spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 读取的芯片状态
==================================================================
*/
SX126x_ERRORS_T SX126xGetDeviceErrors(LORA_CONTROL_FUNC_T *spi_control)
{
    SX126x_ERRORS_T error;

    Sx126xReadCommand(SX126X_RADIO_GET_ERROR, (uint8_t *)&error, 2, spi_control);
    return error;
}

/*
==================================================================
** 函数名称： SX126xClearDeviceErrors
** 功    能：清除设备错误状态
** 输入参数： spi_control:底层驱动操作函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void SX126xClearDeviceErrors(LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[2] = {0x00, 0x00};
    Sx126xWriteCommand(SX126X_RADIO_CLR_ERROR, buf, 2, spi_control);
}

//////////////////////////////////////////内部配置命令/////////////////////////////

/*
==================================================================
** 函数名称： SX126xSetStopRxTimerOnPreambleDetect
** 功    能： 配置前导码检测停止接收超时判定，
                            不开启在检测到同步字或帧头后才会停止超时计时判定
** 输入参数： enable：开启该功能标志位，1表示开启
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void SX126xSetStopRxTimerOnPreambleDetect(uint8_t enable, LORA_CONTROL_FUNC_T *spi_control)
{
    Sx126xWriteCommand(SX126X_RADIO_SET_STOPRXTIMERONPREAMBLE, (uint8_t *)&enable, 1, spi_control);
}

/*
==================================================================
** 函数名称： SX126xSetLoRaSymbNumTimeout
** 功    能： 尚未研究
** 输入参数： SymbNum：
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void SX126xSetLoRaSymbNumTimeout(uint8_t SymbNum, LORA_CONTROL_FUNC_T *spi_control)
{
    Sx126xWriteCommand(SX126X_RADIO_SET_LORASYMBTIMEOUT, &SymbNum, 1, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xSetRegulatorMode
** 功    能： 配置供电模式
** 输入参数： mode：供电模式SX126X_USE_LDO，SX126X_USE_DCDC
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetRegulatorMode(SX126x_REGULATOR_MODES_E mode, LORA_CONTROL_FUNC_T *spi_control)
{
    Sx126xWriteCommand(SX126X_RADIO_SET_REGULATORMODE, (uint8_t *)&mode, 1, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xSetStandbyMode
** 功    能： 配置待机模式，主要是待机状态下的时钟选择
** 输入参数： standbyConfig：SX126X_STDBY_RC，SX126X_STDBY_XOSC
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetStandbyMode(SX126x_STANDBY_MODES_E standbyConfig, LORA_CONTROL_FUNC_T *spi_control)
{
    Sx126xWriteCommand(SX126X_RADIO_SET_STANDBY, (uint8_t *)&standbyConfig, 1, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xSetTx
** 功    能： 启动发送模式
** 输入参数： timeout：用于判定是否在规定的时间内产生TX_TXDONE中断，
                                            否则产生TIMEOUT中断，0表示不开启此功能
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetTx(uint32_t timeout, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[3];

    buf[0] = (uint8_t)((timeout >> 16) & 0xFF);
    buf[1] = (uint8_t)((timeout >> 8) & 0xFF);
    buf[2] = (uint8_t)(timeout & 0xFF);
    Sx126xWriteCommand(SX126X_RADIO_SET_TX, buf, 3, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xSetDio2AsRfSwitchCtrl
** 功    能： DIO2功能控制
** 输入参数： enable：功能选择，1表示将dio2作为收发切换自动控制引脚
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetDio2AsRfSwitchCtrl(uint8_t enable, LORA_CONTROL_FUNC_T *spi_control)
{
    Sx126xWriteCommand(SX126X_RADIO_SET_RFSWITCHMODE, &enable, 1, spi_control);
}

/*
==================================================================
** 函数名称： SX126xSetDio3AsTcxoCtrl
** 功    能： DIO3功能控制
** 输入参数： enable：功能选择，1表示将dio3作为温补晶振的控制，未使用过
              timeout：未研究
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void SX126xSetDio3AsTcxoCtrl(SX126x_TCXO_CTRL_VOLTAGE_E tcxoVoltage, uint32_t timeout, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[4];

    buf[0] = tcxoVoltage & 0x07;
    buf[1] = (uint8_t)((timeout >> 16) & 0xFF);
    buf[2] = (uint8_t)((timeout >> 8) & 0xFF);
    buf[3] = (uint8_t)(timeout & 0xFF);

    Sx126xWriteCommand(SX126X_RADIO_SET_TCXOMODE, buf, 4, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xSetPacketType
** 功    能： 设置SUBGHZ的工作模式，支持GFSK和LORA两种工作模式
** 输入参数： packetType:工作模式
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 检验配置是否成功，成功返回0，失败返回1
==================================================================
*/
uint8_t Sx126xSetPacketType(SX126x_PACKET_TYPES_E packetType, LORA_CONTROL_FUNC_T *spi_control)
{
    Sx126xWriteCommand(SX126X_RADIO_SET_PACKETTYPE, (uint8_t *)&packetType, 1, spi_control);

    // 测试用，在进行工作模式切换后，通过回读，可以确认是否配置成功，用于验证spi通信是否正常
    uint8_t temp = 0xff;
    Sx126xReadCommand(SX126X_RADIO_GET_PACKETTYPE, &temp, 1, spi_control);

    if (temp == packetType)
        return 0; // 检测配置成功
    else
        return 1; // 检测配置失败
}

/*
==================================================================
** 函数名称： Sx126xSetPaConfig
** 功    能： 根据发射频率，确定功放配置
** 输入参数： 自动配置，参数含义参见手册
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetPaConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[4];

    buf[0] = paDutyCycle;
    buf[1] = hpMax;
    buf[2] = deviceSel;
    buf[3] = paLut;
    Sx126xWriteCommand(SX126X_RADIO_SET_PACONFIG, buf, 4, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xSetTxParams
** 功    能： 设置发射参数，请注意针对1261和1262,不同发射功率对应的功放配置参数不同
                            针对1261，发射功率可调整范围为-17到+14dbm
                            针对1262，发射功率可调整范围为-9到+22dbm
** 输入参数： type：芯片类型
                            power：输出功率
                            rampTime：功放斜坡时间
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetTxParams(LORA_CHIPTYPE_E type, int8_t power, SX126x_RADIO_RAMP_TIMES_E rampTime, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[2]; // 第一个字节为配置功率

    switch (type)
    {
    case LORA_1261:
    {
        if (power > 15) // 配置超过最大功率
        {
            return;
        }
        else if (power == 15) // 仅在某些场景下可使用
        {
            Sx126xSetPaConfig(0x06, 0x00, 0x01, 0x01, spi_control);
            buf[0] = 14;
        }
        else if (power > 10)
        {
            Sx126xSetPaConfig(0x04, 0x00, 0x01, 0x01, spi_control);
            buf[0] = power;
        }
        else if (power >= -17)
        {
            Sx126xSetPaConfig(0x01, 0x00, 0x01, 0x01, spi_control);
            buf[0] = power + 3;
        }
        else
        {
            return;
        }

        Sx126xWriteRegister(0x08E7, 0x18, spi_control); // current max is 60 mA for the whole device
        break;
    }
    case LORA_1262:
    case LORA_LLCC68:
    {
        // 针对高功率发射，主要调整PA参数；针对低功率发射，使用低功耗PA，主要调整发射功率配置
        if (power > 22) // 配置超过最大功率
            return;
        else if (power > 20) // 最大功率发射
        {
            Sx126xSetPaConfig(0x04, 0x07, 0x00, 0x01, spi_control);
            buf[0] = power;
        }
        else if (power > 17)
        {
            Sx126xSetPaConfig(0x03, 0x05, 0x00, 0x01, spi_control);
            buf[0] = power + 2;
        }
        else if (power > 14)
        {
            Sx126xSetPaConfig(0x02, 0x03, 0x00, 0x01, spi_control);
            buf[0] = power + 5;
        }
        else if (power >= -9)
        {
            Sx126xSetPaConfig(0x02, 0x02, 0x00, 0x01, spi_control);
            buf[0] = power;
        }
        else // 配置超过最小功率
        {
            return;
        }

        Sx126xWriteRegister(0x08E7, 0x38, spi_control); // current max 140mA for the whole device
        break;
    }
    default:
        return;
    }

    buf[1] = (uint8_t)rampTime;
    Sx126xWriteCommand(SX126X_RADIO_SET_TXPARAMS, buf, 2, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xSetLoraModulationParams
** 功    能： 设置ora模式的调制参数,包括sf，bw，cr和低速率优化
** 输入参数： modulationParams：通用lora射频配置参数
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetLoraModulationParams(LORA_MODULE_PARAS_T modulationParams, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    buf[0] = sx126xSfTransferBuffer[modulationParams.sf]; // 注意芯片配置参数的转换
    buf[1] = sx126xBwTransferBuffer[modulationParams.bw];
    buf[2] = sx126xCrTransferBuffer[modulationParams.cr];
    buf[3] = modulationParams.ldro;

    Sx126xWriteCommand(SX126X_RADIO_SET_MODULATIONPARAMS, buf, 8, spi_control); // 共8个字节，还是发完全部命令
}

/*
==================================================================
** 函数名称： Sx126xSetGfskModulationParams
** 功    能： 设置SUBGHZ_的gfsk模式的调制参数,包括br,pulseshape,bw,fdev
** 输入参数： modulationParams：通用射频配置参数
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetGfskModulationParams(LORA_GFSK_MODULE_PARAS_T modulationParams, LORA_CONTROL_FUNC_T *spi_control)
{
    uint32_t temp = 0;
    float temp_f = 0;
    uint8_t buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    temp = 1024000000 / modulationParams.br;
    buf[0] = (temp >> 16);
    buf[1] = (temp >> 8);
    buf[2] = temp;

    buf[3] = sx126xGfskShapingTransferBuffer[modulationParams.pulseshape]; // 注意芯片配置参数的转换

    buf[4] = sx126xGfskBwTransferBuffer[modulationParams.bw];

    temp_f = (float)modulationParams.fdev * 1.048576f;
    temp = (uint32_t)temp_f;

    buf[5] = (temp >> 16);
    buf[6] = (temp >> 8);
    buf[7] = temp;

    Sx126xWriteCommand(SX126X_RADIO_SET_MODULATIONPARAMS, buf, 8, spi_control); // 共8个字节
}

/*
==================================================================
** 函数名称： Sx126xSetLoRaPacketParams
** 功    能： 设置lora模式组帧参数，包括前导码长度、报头形式（是否固定发送长度），
                                    净荷长度（如果固定），crc校验，位翻转标志
                            需要注意，前导码长度使用了两个字节，不同于sx1280只是用1个字节
                            暂时全部使用默认配置，packetParams在本文件完成默认值定义，
                                    非必要暂不提供修改接口，后续根据需要调整
** 输入参数： packetParams：组帧参数配置
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetLoRaPacketParams(SX126x_LORA_PACKT_PARAS_T packetParams, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    buf[0] = (packetParams.PreambleLength >> 8) & 0xFF;
    buf[1] = packetParams.PreambleLength;
    buf[2] = packetParams.HeaderType;
    buf[3] = packetParams.PayloadLength;
    buf[4] = packetParams.CrcMode;
    buf[5] = packetParams.InvertIQ;

    Sx126xWriteCommand(SX126X_RADIO_SET_PACKETPARAMS, buf, 9, spi_control); // 共9个字节，还是发完全部命令
}

/*
==================================================================
** 函数名称： Sx126xSetGfskPacketParams
** 功    能： 设置1262的GFSK模式组帧参数
                            暂时全部使用默认配置，packetParams在本文件完成默认值定义，
                                    非必要暂不提供修改接口，后续根据需要调整
** 输入参数： packetParams：组帧参数配置
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetGfskPacketParams(SX126x_RADIO_GFSK_PACKET_PARAM_T packetParams, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    buf[0] = (packetParams.PreambleLength >> 8) & 0xFF;
    buf[1] = packetParams.PreambleLength;
    buf[2] = packetParams.DetectorLength;
    buf[3] = packetParams.SyncWordLength;
    buf[4] = packetParams.AddComp;
    buf[5] = packetParams.PacketType;
    buf[6] = packetParams.PayloadLength;
    buf[7] = packetParams.CRCType;
    buf[8] = packetParams.Whitening;

    Sx126xWriteCommand(SX126X_RADIO_SET_PACKETPARAMS, buf, 9, spi_control); // 共9个字节，还是发完全部命令
}

/*
==================================================================
** 函数名称： Sx126xSetBufferBaseAddresses
** 功    能： 设置1262收发缓存的分配起始地址
                            为了实现最大长度的收发，收发共用全部缓存，起始地址均为0
** 输入参数： txBaseAddress：发送缓存基地址
                            rxBaseAddress：接收缓存基地址
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetBufferBaseAddresses(uint8_t txBaseAddress, uint8_t rxBaseAddress, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[2];
    buf[0] = txBaseAddress;
    buf[1] = rxBaseAddress;
    Sx126xWriteCommand(SX126X_RADIO_SET_BUFFERBASEADDRESS, buf, 2, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xGetRssiInst
** 功    能： 获取实时RSSI值
** 输入参数： spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
int8_t Sx126xGetRssiInst(LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[1];
    int8_t rssi = 0;

    Sx126xReadCommand(SX126X_RADIO_GET_RSSIINST, buf, 1, spi_control);

    rssi = -buf[0] >> 1; // Signal power in dBm = –RssiInst/2 (dBm)
    return rssi;
}

/*
==================================================================
** 函数名称： Sx126xGetLoRaPacketStatus
** 功    能： 获取接收信号质量相关的参数，工作在lora模式下使用
** 输入参数： spi_control：基本控制函数指针
** 输出参数： pktStatus：回读值存储变量指针
** 返    回： 无
==================================================================
*/
void Sx126xGetLoRaPacketStatus(LORA_RX_SIGNAL_STATUS_T *pktStatus, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t status[3];

    Sx126xReadCommand(SX126X_RADIO_GET_PACKETSTATUS, status, 3, spi_control);

    pktStatus->lora_signal_status.rssipkt = -status[0] >> 1;
    (status[1] < 128) ? (pktStatus->lora_signal_status.snr = status[1] >> 2) : (pktStatus->lora_signal_status.snr = ((status[1] - 256) >> 2));
    pktStatus->lora_signal_status.rssilora = -status[2] >> 1;
}

/*
==================================================================
** 函数名称： Sx126xGetGfskPacketStatus
** 功    能： 获取接收信号的参数，工作在GFSK模式下使用
** 输入参数： spi_control：基本控制函数指针
** 输出参数： pktStatus：回读值存储变量指针
** 返    回： 无
==================================================================
*/
void Sx126xGetGfskPacketStatus(LORA_RX_SIGNAL_STATUS_T *pktStatus, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t status[3];

    Sx126xReadCommand(SX126X_RADIO_GET_PACKETSTATUS, status, 3, spi_control);

    pktStatus->gfsk_signal_status.rxstatus = status[0];
    pktStatus->gfsk_signal_status.rssisync = -status[1] >> 1;
    pktStatus->gfsk_signal_status.rssiavg = -status[2] >> 1;
}

//////////////////////////////////////////暂时采用固定参数，后续可能进行扩展的内部配置//////////////////////////////////

/*
==================================================================
** 函数名称： Sx126xSetSleep
** 功    能： 进入休眠，暂时使用固定配置
                            不开启RTC自动唤醒，热启动
                            热启动时，仅进入休眠前的模式配置会被保留，其他模式配置不会被保留
** 输入参数： spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetSleep(LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t sleep_config = 0x04;
    Sx126xWriteCommand(SX126X_RADIO_SET_SLEEP, &sleep_config, 1, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xSetStdby
** 功    能： 进入待机，暂时使用固定配置
** 输入参数： spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetStdby(LORA_CONTROL_FUNC_T *spi_control)
{
    Sx126xSetStandbyMode(SX126X_STDBY_RC, spi_control); // 进入STANDBY_RC模式
}

/*
==================================================================
** 函数名称： Sx126xSetFs
** 功    能： 进入FS模式
** 输入参数： spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetFs(LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t temp;
    Sx126xWriteCommand(SX126X_RADIO_SET_FS, &temp, 0, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xSetTxContinuousWave
** 功    能： 进入连续发送模式模式，测试用
** 输入参数： spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetTxContinuousWave(LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t temp;
    Sx126xWriteCommand(SX126X_RADIO_SET_TXCONTINUOUSWAVE, &temp, 0, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xCalibrateImage
** 功    能： 频率校准，不同频段，参数不同
                            在切换频率前，如果频率发生了范围变换，需要进行本操作
                            暂时在每次切换频率时均自动进行调用
** 输入参数： freq：工作频率
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xCalibrateImage(uint32_t freq, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t calFreq[2];

    if (freq > 885000000)
    {
        calFreq[0] = 0xE1;
        calFreq[1] = 0xE9;
    }
    else if (freq > 850000000)
    {
        calFreq[0] = 0xD7;
        calFreq[1] = 0xD8;
    }
    else if (freq > 770000000)
    {
        calFreq[0] = 0xC1;
        calFreq[1] = 0xC5;
    }
    else if (freq > 460000000)
    {
        calFreq[0] = 0x75;
        calFreq[1] = 0x81;
    }
    else if (freq > 425000000)
    {
        calFreq[0] = 0x6B;
        calFreq[1] = 0x6F;
    }
    Sx126xWriteCommand(SX126X_RADIO_CALIBRATEIMAGE, calFreq, 2, spi_control);
}

/*
==================================================================
** 函数名称： SUBGHZ_SetRfFrequency
** 功    能： 设置工作频率
** 输入参数：	frequency：工作频率
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetRfFrequency(uint32_t frequency, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[4];
    uint32_t freq;

    // 未判定，每次切换频率都进行校准
    // 实际很多应用没有必要，考虑合理的实现方式
    Sx126xCalibrateImage(frequency, spi_control);

    freq = (uint32_t)((double)frequency / (double)SX126X_FREQ_STEP);
    buf[0] = (uint8_t)((freq >> 24) & 0xFF);
    buf[1] = (uint8_t)((freq >> 16) & 0xFF);
    buf[2] = (uint8_t)((freq >> 8) & 0xFF);
    buf[3] = (uint8_t)(freq & 0xFF);

    Sx126xWriteCommand(SX126X_RADIO_SET_RFFREQUENCY, buf, 4, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xSetCadParams
** 功    能： 设置cad参数，暂未用到，未详细研究
** 输入参数：	未详细研究
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetCadParams(SX126x_RADIO_LORA_CAD_SYMBOLS_E cadSymbolNum, uint8_t cadDetPeak, uint8_t cadDetMin,
                        SX126x_RADIO_CAD_EXIT_MODES_E cadExitMode, uint32_t cadTimeout, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[7];

    buf[0] = (uint8_t)cadSymbolNum;
    buf[1] = cadDetPeak;
    buf[2] = cadDetMin;
    buf[3] = (uint8_t)cadExitMode;
    buf[4] = (uint8_t)((cadTimeout >> 16) & 0xFF);
    buf[5] = (uint8_t)((cadTimeout >> 8) & 0xFF);
    buf[6] = (uint8_t)(cadTimeout & 0xFF);
    Sx126xWriteCommand(SX126X_RADIO_SET_CADPARAMS, buf, 7, spi_control);
}

/*
==================================================================
** 函数名称： SUBGHZ_GetRxBufferStatus
** 功    能： 获取接收数据信息，分为长度和起始指针
** 输入参数：	payloadLength：存放接收数据长度的变量指针
                            rxStartBufferPointer：存取接收数据起始地址的变量指针
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xGetRxBufferStatus(uint8_t *payloadLength, uint8_t *rxStartBufferPointer, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t status[2];

    Sx126xReadCommand(SX126X_RADIO_GET_RXBUFFERSTATUS, status, 2, spi_control);

    //	//当lora模式下，使用固定报头模式时，净荷数据长度需要从寄存器直接读取
    //	if((SX126xReadRegister(0x0704,spi_control)>> 7 == 1))  //使用固定报头模式
    //	{
    //		*payloadLength = SX126xReadRegister(0x0702,spi_control);
    //	}
    //	else
    //	{
    //		*payloadLength = status[0];
    //	}

    *payloadLength = status[0];
    *rxStartBufferPointer = status[1];
}

//////////////////////////////////////////初始化函数//////////////////////////////////

/*
==================================================================
** 函数名称： Sx126xWakeup
** 功    能： 芯片从休眠状态唤醒，通过片选引脚实现
** 输入参数： spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 返回值非0表示唤醒异常
==================================================================
*/
uint8_t Sx126xWakeup(LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t temp = 0;
    spi_control->NssControlFunc(0);       // 片选低有效
    temp = Sx126xWaitOnBusy(spi_control); // 等待芯片处于可通信状态
    spi_control->NssControlFunc(1);       // 取消片选

    return temp;
}

/*
==================================================================
** 函数名称： Sx126xSetDioIrqParams
** 功    能： 配置中断引脚功能和中断掩码
                            需要根据外部电路进行匹配
** 输入参数：
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetDioIrqParams(uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[8];

    buf[0] = (uint8_t)((irqMask >> 8) & 0x00FF);
    buf[1] = (uint8_t)(irqMask & 0x00FF);
    buf[2] = (uint8_t)((dio1Mask >> 8) & 0x00FF);
    buf[3] = (uint8_t)(dio1Mask & 0x00FF);
    buf[4] = (uint8_t)((dio2Mask >> 8) & 0x00FF);
    buf[5] = (uint8_t)(dio2Mask & 0x00FF);
    buf[6] = (uint8_t)((dio3Mask >> 8) & 0x00FF);
    buf[7] = (uint8_t)(dio3Mask & 0x00FF);
    Sx126xWriteCommand(SX126X_RADIO_CFG_DIOIRQ, buf, 8, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xInit
** 功    能： 根据配置参数，进行模块初始化
** 输入参数：	setting：射频参数配置变量指针
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 返回值0表示初始化成功，非0表示失败
==================================================================
*/
uint8_t Sx126xInit(const LORA_RADIO_SETTING_T *setting, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t state = 0;

    Sx126xWakeup(spi_control); // 做一次唤醒确认，理论上初始化应该伴随硬件复位，但存在不能硬件复位的设计

    Sx126xSetStandbyMode(SX126X_STDBY_RC, spi_control); // 进入STANDBY_RC模式

    Sx126xSetDio2AsRfSwitchCtrl(1, spi_control); // 1表示开启，使用dio2来控制收发射频开关切换 //和设计匹配

    Sx126xSetRegulatorMode(SX126X_USE_DCDC, spi_control); // dcdc供电

    // DIO3可以用来作为温补晶振的反馈控制输出，本系统不使用，所以无相关配置
    // SX126xSetDio3AsTcxoCtrl(SX126X_TCXO_CTRL_1_7V, 320,spi_control);

    // 工作模式切换，需要注意模式切换以后，相关的工作模式配置会被清空，需要重新配置
    if (setting->mode == WORK_MODEM_FSK) // 如果使用GFSK通信模式
    {
        state = Sx126xSetPacketType(SX126X_PACKET_TYPE_GFSK, spi_control);      // 配置为gfsk工作模式
        Sx126xSetGfskModulationParams(setting->gfsk_module_paras, spi_control); // 设置gfsk工作模式参数
        Sx126xSetGfskPacketParams(sx126x_gfsk_packet_params, spi_control);      // 设置gfsk组帧形式
    }
    else if (setting->mode == WORK_MODEM_LORA) // 如果使用lora通信模式
    {
        state = Sx126xSetPacketType(SX126X_PACKET_TYPE_LORA, spi_control);     // 配置为lora工作模式
        Sx126xSetLoraModulationParams(setting->modulationparams, spi_control); // 设置lora工作模式参数
        Sx126xSetLoRaPacketParams(sx126x_packet_params, spi_control);          // 设置lora组帧形式
    }
    else
    {
        return 1;
    }

    if (state) // 状态读取出错
    {
        return 2;
    }

    Sx126xSetRfFrequency(setting->frequency, spi_control); // 设置工作频率，包含了修正频率校准参数  	//测试发现，频率配置如果放到工作模式切换前面，会概率性产生错误，原因未明

    Sx126xSetBufferBaseAddresses(0x00, 0x00, spi_control); // 设置收发缓存起始位置，共用缓存

    Sx126xSetTxParams(setting->type, setting->txpower, SX126X_RADIO_RAMP_40_US, spi_control); // 设置发射功率，注意为实际值

    // 设置中断类型，允许接收完成中断、接收超时中断、发送完成中断、CRC校验错误中断
    // 后面调整为开放接口自由配置
    // 暂时全部由DIO1引脚输出
    Sx126xSetDioIrqParams(SX126X_IRQ_TX_DONE | SX126X_IRQ_RX_DONE | SX126X_IRQ_RX_TX_TIMEOUT | SX126X_IRQ_CRC_ERROR,
                          SX126X_IRQ_TX_DONE | SX126X_IRQ_RX_DONE | SX126X_IRQ_RX_TX_TIMEOUT | SX126X_IRQ_CRC_ERROR, // DIO1
                          SX126X_IRQ_RADIO_NONE,
                          SX126X_IRQ_RADIO_NONE,
                          spi_control); // 只开启TXdone中断，并且映射到DIO1

    //	SX126xSetStopRxTimerOnPreambleDetect(0,spi_control);
    //	SX126xSetLoRaSymbNumTimeout(0,spi_control);

    if (sx126x_error_flag)
        return 3;
    else
        return 0;
}

/*
==================================================================
** 函数名称： Sx126xGetIrqStatus
** 功    能： 获取中断状态，在外部MCU中断函数中进行处理
** 输入参数：	spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
uint16_t Sx126xGetIrqStatus(LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t irqStatus[2];
    Sx126xReadCommand(SX126X_RADIO_GET_IRQSTATUS, irqStatus, 2, spi_control);

    return (irqStatus[0] << 8) | irqStatus[1];
}

/*
==================================================================
** 函数名称： Sx126xGetIrqStateSimplify
** 功    能： 按照lora统一的中断类型进行中断类型返回
** 输入参数：	spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
LORA_IRQ_STATE_E Sx126xGetIrqStateSimplify(LORA_CONTROL_FUNC_T *spi_control)
{
    uint16_t irq_flag = 0;

    irq_flag = Sx126xGetIrqStatus(spi_control);

    if ((irq_flag & (SX126X_IRQ_RX_DONE | SX126X_IRQ_CRC_ERROR)) == SX126X_IRQ_RX_DONE) // 接收完成，并且没有CRC校验错误
        return LORA_IRQ_RXDONE;
    else if ((irq_flag & SX126X_IRQ_TX_DONE) == SX126X_IRQ_TX_DONE)
        return LORA_IRQ_TXDONE;
    else if ((irq_flag & SX126X_IRQ_RX_TX_TIMEOUT) == SX126X_IRQ_RX_TX_TIMEOUT)
        return LORA_IRQ_TIMEOUT;
    else
        return LORA_IRQ_ERROR; // 既不是发送完成也不是接收完成
}

/*
==================================================================
** 函数名称： Sx126xClearIrqStatus
** 功    能： 清除中断标志位
** 输入参数：	irq：待清除的中断标志位
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xClearIrqStatus(uint16_t irq, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t buf[2];

    buf[0] = (uint8_t)(((uint16_t)irq >> 8) & 0x00FF);
    buf[1] = (uint8_t)((uint16_t)irq & 0x00FF);
    Sx126xWriteCommand(SX126X_RADIO_CLR_IRQSTATUS, buf, 2, spi_control);
}

/*
==================================================================
** 函数名称： Sx126xLoraSendData
** 功    能： 通过Lora模式进行数据发送
** 输入参数：	index：数据缓存指针
                            length：带发送数据长度
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xLoraSendData(uint8_t *index, uint8_t length, LORA_CONTROL_FUNC_T *spi_control)
{
    // 发射需要配置发射信息长度
    sx126x_packet_params.PayloadLength = length;
    Sx126xSetLoRaPacketParams(sx126x_packet_params, spi_control);

    Sx126xWriteBuffer(0x00, index, length, spi_control); // 将数据写入缓存，默认起始地址为0

    Sx126xSetTx(0, spi_control); // 启动发送，暂时未配置发送超时
}

/*
==================================================================
** 函数名称： Sx126xGfskSendData
** 功    能： 通过gfsk模式进行数据发送
** 输入参数：	index：数据缓存指针
                            length：带发送数据长度
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xGfskSendData(uint8_t *index, uint8_t length, LORA_CONTROL_FUNC_T *spi_control)
{
    // 发射需要配置发射信息长度
    sx126x_gfsk_packet_params.PayloadLength = length;
    Sx126xSetGfskPacketParams(sx126x_gfsk_packet_params, spi_control);

    Sx126xWriteBuffer(0x00, index, length, spi_control); // 将数据写入缓存，默认起始地址为0

    Sx126xSetTx(0, spi_control); // 启动发送，暂时未配置发送超时
}

/*
==================================================================
** 函数名称： Sx126xGetPayload
** 功    能： 将接收的数据读取到指定的缓存，可以设定最大接收长度防止数据溢出
** 输入参数：	buffer：接收数据的缓存指针
                            max_size：允许接收的最大长度
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
uint8_t Sx126xGetPayload(uint8_t *buffer, uint8_t max_size, LORA_CONTROL_FUNC_T *spi_control)
{
    uint8_t length = 0;
    uint8_t offset = 0;

    Sx126xGetRxBufferStatus(&length, &offset, spi_control); // 获取buffer信息
    if (length > max_size)
    {
        return 0;
    }
    Sx126xReadBuffer(offset, buffer, length, spi_control);
    return length;
}

/*
==================================================================
** 函数名称： Sx126xSetRx
** 功    能： 芯片配置进入接收模式
** 输入参数：	timeout表示接收超时时间，单位us，最大设定范围262秒
                                0表示单次不设定接收超时
                                0xffffffff表示进入连续接收模式，接收完成后会自动重启接收
                            spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetRx(uint32_t timeout, LORA_CONTROL_FUNC_T *spi_control)
{
    uint32_t timeout_temp = 0;

    if (timeout == 0xffffffff)
        timeout_temp = 0xffffffff;
    else if (timeout > 0xf000000)
        timeout_temp = 0xfffffe;
    else
        timeout_temp = timeout * 10 / 156; // 匹配时间，该寄存器单位为15.6us

    uint8_t buf[3];

    buf[0] = (uint8_t)((timeout_temp >> 16) & 0xFF);
    buf[1] = (uint8_t)((timeout_temp >> 8) & 0xFF);
    buf[2] = (uint8_t)(timeout_temp & 0xFF);
    Sx126xWriteCommand(SX126X_RADIO_SET_RX, buf, 3, spi_control);
}
