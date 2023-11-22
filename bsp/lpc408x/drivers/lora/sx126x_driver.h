/*
****************************************Copyright(c)************************************
**                                  Zhengzhou GL. TECH Co.,Ltd
**___________________________File Info_________________________
**文件名称：sx126x_driver.h
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

#ifndef _SX126X_DRIVER_H_
#define _SX126X_DRIVER_H_

#include "lora_driver_base.h" //使用lora系列芯片通用控制方案

/*
==================================================================
== 变量类型声明
==================================================================
*/

// sx126x 当前工作状态，共用体变量，方便修改和读取
typedef union RadioStatus_u
{
    uint8_t Value;
    struct
    {                          // bit order is lsb -> msb
        uint8_t Reserved : 1;  //!< Reserved
        uint8_t CmdStatus : 3; //!< Command status
        uint8_t ChipMode : 3;  //!< Chip mode
        uint8_t CpuBusy : 1;   //!< Flag for CPU radio busy
    } Fields;
} SX126x_RADIO_STATUS_T;

// sx126x 待机模式参数，时钟周期不同
typedef enum
{
    SX126X_STDBY_RC = 0x00,
    SX126X_STDBY_XOSC = 0x01,
} SX126x_STANDBY_MODES_E;

// sx126x 供电模式，包含LDO和DCDC两种
typedef enum
{
    SX126X_USE_LDO = 0x00, // default
    SX126X_USE_DCDC = 0x01,
} SX126x_REGULATOR_MODES_E;

// sx126x 数据包类型
typedef enum
{
    SX126X_PACKET_TYPE_GFSK = 0x00,
    SX126X_PACKET_TYPE_LORA = 0x01,
} SX126x_PACKET_TYPES_E;

// sx126x 功放斜坡时间
typedef enum
{
    SX126X_RADIO_RAMP_10_US = 0x00,
    SX126X_RADIO_RAMP_20_US = 0x01,
    SX126X_RADIO_RAMP_40_US = 0x02,
    SX126X_RADIO_RAMP_80_US = 0x03,
    SX126X_RADIO_RAMP_200_US = 0x04,
    SX126X_RADIO_RAMP_800_US = 0x05,
    SX126X_RADIO_RAMP_1700_US = 0x06,
    SX126X_RADIO_RAMP_3400_US = 0x07,
} SX126x_RADIO_RAMP_TIMES_E;

// sx126x  CAD检测符号数
typedef enum
{
    SX126X_LORA_CAD_01_SYMBOL = 0x00,
    SX126X_LORA_CAD_02_SYMBOL = 0x01,
    SX126X_LORA_CAD_04_SYMBOL = 0x02,
    SX126X_LORA_CAD_08_SYMBOL = 0x03,
    SX126X_LORA_CAD_16_SYMBOL = 0x04,
} SX126x_RADIO_LORA_CAD_SYMBOLS_E;

// sx126x CAD检测完成后的工作模式切换控制
typedef enum
{
    SX126X_LORA_CAD_ONLY = 0x00,
    SX126X_LORA_CAD_RX = 0x01,
    SX126X_LORA_CAD_LBT = 0x10,
} SX126x_RADIO_CAD_EXIT_MODES_E;

//////////////////////////////////////GFSK工作模式配置////////////////////////////////////////////////

// sx126x  GSFK模式下接收最小端射频前导码长度
typedef enum
{
    SX126X_RADIO_PREAMBLE_DETECTOR_OFF = 0x00,     //!< Preamble detection length off
    SX126X_RADIO_PREAMBLE_DETECTOR_08_BITS = 0x04, //!< Preamble detection length 8 bits
    SX126X_RADIO_PREAMBLE_DETECTOR_16_BITS = 0x05, //!< Preamble detection length 16 bits
    SX126X_RADIO_PREAMBLE_DETECTOR_24_BITS = 0x06, //!< Preamble detection length 24 bits
    SX126X_RADIO_PREAMBLE_DETECTOR_32_BITS = 0x07, //!< Preamble detection length 32 bit
} SX126x_RADIO_PREAMBLE_DETECTION_E;

// sx126x GSFK模式下射频同步字检测
typedef enum
{
    SX126X_RADIO_ADDRESSCOMP_FILT_OFF = 0x00, //!< No correlator turned on, i.e. do not search for SyncWord
    SX126X_RADIO_ADDRESSCOMP_FILT_NODE = 0x01,
    SX126X_RADIO_ADDRESSCOMP_FILT_NODE_BROAD = 0x02,
} SX126x_RADIO_ADDRESS_COMP_E;

// sx126x GSFK模式下发送数据包长度控制方式
typedef enum
{
    SX126X_RADIO_PACKET_FIXED_LENGTH = 0x00,    //!< The packet is known on both sides, no header included in the packet
    SX126X_RADIO_PACKET_VARIABLE_LENGTH = 0x01, //!< The packet is on variable size, header included
} SX126x_RADIO_PACKET_LENGTH_MODES_E;

// sx126x GSFK模式下射频CRC校验形式
typedef enum
{
    SX126X_RADIO_CRC_OFF = 0x01, //!< No CRC in use
    SX126X_RADIO_CRC_1_BYTES = 0x00,
    SX126X_RADIO_CRC_2_BYTES = 0x02,
    SX126X_RADIO_CRC_1_BYTES_INV = 0x04,
    SX126X_RADIO_CRC_2_BYTES_INV = 0x06,
    SX126X_RADIO_CRC_2_BYTES_IBM = 0xF1,
    SX126X_RADIO_CRC_2_BYTES_CCIT = 0xF2,
} SX126x_RADIO_CRC_TYPES_E;

// sx126x GSFK模式下白化操作配置
typedef enum
{
    SX126X_RADIO_DC_FREE_OFF = 0x00,
    SX126X_RADIO_DC_FREEWHITENING = 0x01,
} SX126x_RADIO_DC_FREE_E;

// 自定义sx126x GFSK组帧数据结构体
typedef struct
{
    uint16_t PreambleLength;                          // 前导码长度，2字节
    SX126x_RADIO_PREAMBLE_DETECTION_E DetectorLength; // 前导码探测长度
    uint8_t SyncWordLength;                           // 同步字长度，单位bit，0到64（最多8字节），存储地址在0x06c0到0x06c7）配置了需要进行配置
    SX126x_RADIO_ADDRESS_COMP_E AddComp;              // 地址过滤配置，node地址在0x06CD，boradcast地址在0x06CE
    SX126x_RADIO_PACKET_LENGTH_MODES_E PacketType;    // 数据包长度模式，是否约定固定长度
    uint8_t PayloadLength;                            // 数据长度，发送的数据长度，或最大的接收长度
    SX126x_RADIO_CRC_TYPES_E CRCType;                 // CRC校验模式，如果启用注意需要进行对应的配置
    SX126x_RADIO_DC_FREE_E Whitening;                 // 是否进行白花
} SX126x_RADIO_GFSK_PACKET_PARAM_T;

//////////////////////////////////////LoRa工作模式配置////////////////////////////////////////////////

// sx126x lora模式下数据包长度控制方式
typedef enum
{
    SX126X_LORA_PACKET_VARIABLE_LENGTH = 0x00, // 可变长度，在header中传输
    SX126X_LORA_PACKET_FIXED_LENGTH = 0x01,    // 固定长度，收发双端提前协同，传输中不包含header
} SX126x_LORA_PACKET_LENGTHS_MODES_E;

// sx126x lora模式下CRC校验是否开启
typedef enum
{
    SX126X_LORA_CRC_ON = 0x01,  //!< CRC activated
    SX126X_LORA_CRC_OFF = 0x00, //!< CRC not used
} SX126x_LORA_CRC_MODES_E;

// sx126x LORA模式下位翻转控制
typedef enum
{
    SX126X_LORA_IQ_NORMAL = 0x00,
    SX126X_LORA_IQ_INVERTED = 0x01,
} SX126x_LORA_IQ_MODES_E;

// 自定义sx126x lora组帧数据结构体，包括前导码长度、报头形式（是否固定发送长度），净荷长度（如果固定），crc校验，位翻转标志
// 与1280不同，前导码长度使用两个字节
typedef struct
{
    uint16_t PreambleLength;
    SX126x_LORA_PACKET_LENGTHS_MODES_E HeaderType;
    uint8_t PayloadLength;
    SX126x_LORA_CRC_MODES_E CrcMode;
    SX126x_LORA_IQ_MODES_E InvertIQ;
} SX126x_LORA_PACKT_PARAS_T;

//////////////////////////////////////模块工作模式配置////////////////////////////////////////////////

// sx126x TCXO 温度补偿晶振 dio3 控制电压配置值
typedef enum
{
    SX126X_TCXO_CTRL_1_6V = 0x00,
    SX126X_TCXO_CTRL_1_7V = 0x01,
    SX126X_TCXO_CTRL_1_8V = 0x02,
    SX126X_TCXO_CTRL_2_2V = 0x03,
    SX126X_TCXO_CTRL_2_4V = 0x04,
    SX126X_TCXO_CTRL_2_7V = 0x05,
    SX126X_TCXO_CTRL_3_0V = 0x06,
    SX126X_TCXO_CTRL_3_3V = 0x07,
} SX126x_TCXO_CTRL_VOLTAGE_E;

// sx126x 中断掩码变量
typedef enum
{
    SX126X_IRQ_RADIO_NONE = 0x0000,
    SX126X_IRQ_TX_DONE = 0x0001,
    SX126X_IRQ_RX_DONE = 0x0002,
    SX126X_IRQ_PREAMBLE_DETECTED = 0x0004,
    SX126X_IRQ_SYNCWORD_VALID = 0x0008,
    SX126X_IRQ_HEADER_VALID = 0x0010,
    SX126X_IRQ_HEADER_ERROR = 0x0020,
    SX126X_IRQ_CRC_ERROR = 0x0040,
    SX126X_IRQ_CAD_DONE = 0x0080,
    SX126X_IRQ_CAD_ACTIVITY_DETECTED = 0x0100,
    SX126X_IRQ_RX_TX_TIMEOUT = 0x0200,
    SX126X_IRQ_RADIO_ALL = 0xFFFF,
} SX126x_IRQ_MASKS_E;

// sx126x 控制命令集合
typedef enum
{
    SX126X_RADIO_GET_STATUS = 0xC0,
    SX126X_RADIO_WRITE_REGISTER = 0x0D,
    SX126X_RADIO_READ_REGISTER = 0x1D,
    SX126X_RADIO_WRITE_BUFFER = 0x0E,
    SX126X_RADIO_READ_BUFFER = 0x1E,
    SX126X_RADIO_SET_SLEEP = 0x84,
    SX126X_RADIO_SET_STANDBY = 0x80,
    SX126X_RADIO_SET_FS = 0xC1,
    SX126X_RADIO_SET_TX = 0x83,
    SX126X_RADIO_SET_RX = 0x82,
    SX126X_RADIO_SET_RXDUTYCYCLE = 0x94,
    SX126X_RADIO_SET_CAD = 0xC5,
    SX126X_RADIO_SET_TXCONTINUOUSWAVE = 0xD1,
    SX126X_RADIO_SET_TXCONTINUOUSPREAMBLE = 0xD2,
    SX126X_RADIO_SET_PACKETTYPE = 0x8A,
    SX126X_RADIO_GET_PACKETTYPE = 0x11,
    SX126X_RADIO_SET_RFFREQUENCY = 0x86,
    SX126X_RADIO_SET_TXPARAMS = 0x8E,
    SX126X_RADIO_SET_PACONFIG = 0x95,
    SX126X_RADIO_SET_CADPARAMS = 0x88,
    SX126X_RADIO_SET_BUFFERBASEADDRESS = 0x8F,
    SX126X_RADIO_SET_MODULATIONPARAMS = 0x8B,
    SX126X_RADIO_SET_PACKETPARAMS = 0x8C,
    SX126X_RADIO_GET_RXBUFFERSTATUS = 0x13,
    SX126X_RADIO_GET_PACKETSTATUS = 0x14,
    SX126X_RADIO_GET_RSSIINST = 0x15,
    SX126X_RADIO_GET_STATS = 0x10,
    SX126X_RADIO_RESET_STATS = 0x00,
    SX126X_RADIO_CFG_DIOIRQ = 0x08,
    SX126X_RADIO_GET_IRQSTATUS = 0x12,
    SX126X_RADIO_CLR_IRQSTATUS = 0x02,
    SX126X_RADIO_CALIBRATE = 0x89,
    SX126X_RADIO_CALIBRATEIMAGE = 0x98,
    SX126X_RADIO_SET_REGULATORMODE = 0x96,
    SX126X_RADIO_GET_ERROR = 0x17,
    SX126X_RADIO_CLR_ERROR = 0x07,
    SX126X_RADIO_SET_TCXOMODE = 0x97,
    SX126X_RADIO_SET_TXFALLBACKMODE = 0x93,
    SX126X_RADIO_SET_RFSWITCHMODE = 0x9D,
    SX126X_RADIO_SET_STOPRXTIMERONPREAMBLE = 0x9F,
    SX126X_RADIO_SET_LORASYMBTIMEOUT = 0xA0,
} SX126x_COMMANDS_E;

///*!
// * \brief Represents the Rx internal counters values when GFSK or LoRa packet type is used
// */
// typedef struct
//{
//    RadioPacketTypes_t                    packetType;       //!< Packet to which the packet status are referring to.
//    uint16_t PacketReceived;
//    uint16_t  CrcOk;
//    uint16_t  LengthError;
//}RxCounter_t;

///*!
// * \brief Represents a calibration configuration
// */
// typedef union
//{
//    struct
//    {
//        uint8_t RC64KEnable    : 1;                             //!< Calibrate RC64K clock
//        uint8_t RC13MEnable    : 1;                             //!< Calibrate RC13M clock
//        uint8_t PLLEnable      : 1;                             //!< Calibrate PLL
//        uint8_t ADCPulseEnable : 1;                             //!< Calibrate ADC Pulse
//        uint8_t ADCBulkNEnable : 1;                             //!< Calibrate ADC bulkN
//        uint8_t ADCBulkPEnable : 1;                             //!< Calibrate ADC bulkP
//        uint8_t ImgEnable      : 1;
//        uint8_t                : 1;
//    }Fields;
//    uint8_t Value;
//}CalibrationParams_t;

// sx126x 简单描述可能存在的射频错误代码
typedef union
{
    struct
    {
        uint8_t Rc64kCalib : 1; //!< RC 64kHz oscillator calibration failed
        uint8_t Rc13mCalib : 1; //!< RC 13MHz oscillator calibration failed
        uint8_t PllCalib : 1;   //!< PLL calibration failed
        uint8_t AdcCalib : 1;   //!< ADC calibration failed
        uint8_t ImgCalib : 1;   //!< Image calibration failed
        uint8_t XoscStart : 1;  //!< XOSC oscillator failed to start
        uint8_t PllLock : 1;    //!< PLL lock failed
        uint8_t BuckStart : 1;  //!< Buck converter failed to start
        uint8_t PaRamp : 1;     //!< PA ramp failed
        uint8_t : 7;            //!< Reserved
    } Fields;
    uint16_t Value;
} SX126x_ERRORS_T;

/*
==================================================================
== 函数声明
==================================================================
*/

/*
==================================================================
** 函数名称： Sx126xReadErrorFlag
** 功    能： 读取错误计数，对外提供
** 输入参数： 无
** 输出参数： 无
** 返    回： 返回值表示自行判定的异常次数
==================================================================
*/
uint16_t Sx126xReadErrorFlag(void);

/*
==================================================================
** 函数名称： Sx126xClearErrorFlag
** 功    能： 主动清0错误计数，一般在初始化前执行，对外提供
** 输入参数： 无
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xClearErrorFlag(void);

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
uint8_t Sx126xInit(const LORA_RADIO_SETTING_T *setting, LORA_CONTROL_FUNC_T *spi_control);

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
void Sx126xSetRfFrequency(uint32_t frequency, LORA_CONTROL_FUNC_T *spi_control);

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
void Sx126xSetLoraModulationParams(LORA_MODULE_PARAS_T modulationParams, LORA_CONTROL_FUNC_T *spi_control);

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
void Sx126xSetGfskModulationParams(LORA_GFSK_MODULE_PARAS_T modulationParams, LORA_CONTROL_FUNC_T *spi_control);

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
void Sx126xSetTxParams(LORA_CHIPTYPE_E type, int8_t power, SX126x_RADIO_RAMP_TIMES_E rampTime, LORA_CONTROL_FUNC_T *spi_control);

/*
==================================================================
** 函数名称： Sx126xGetIrqStateSimplify
** 功    能： 按照lora统一的中断类型进行中断类型返回
** 输入参数：	spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
LORA_IRQ_STATE_E Sx126xGetIrqStateSimplify(LORA_CONTROL_FUNC_T *spi_control);

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
void Sx126xClearIrqStatus(uint16_t irq, LORA_CONTROL_FUNC_T *spi_control);

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
uint8_t Sx126xGetPayload(uint8_t *buffer, uint8_t max_size, LORA_CONTROL_FUNC_T *spi_control);

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
void Sx126xLoraSendData(uint8_t *index, uint8_t length, LORA_CONTROL_FUNC_T *spi_control);

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
void Sx126xGfskSendData(uint8_t *index, uint8_t length, LORA_CONTROL_FUNC_T *spi_control);

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
void Sx126xSetRx(uint32_t timeout, LORA_CONTROL_FUNC_T *spi_control);

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
void Sx126xSetSleep(LORA_CONTROL_FUNC_T *spi_control);

/*
==================================================================
** 函数名称： Sx126xSetStdby
** 功    能： 进入待机，暂时使用固定配置
** 输入参数： spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetStdby(LORA_CONTROL_FUNC_T *spi_control);

/*
==================================================================
** 函数名称： Sx126xWakeup
** 功    能： 芯片从休眠状态唤醒，通过片选引脚实现
** 输入参数： spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 返回值非0表示唤醒异常
==================================================================
*/
uint8_t Sx126xWakeup(LORA_CONTROL_FUNC_T *spi_control);

/*
==================================================================
** 函数名称： Sx126xGetRssiInst
** 功    能： 获取实时RSSI值
** 输入参数： spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
int8_t Sx126xGetRssiInst(LORA_CONTROL_FUNC_T *spi_control);

/*
==================================================================
** 函数名称： Sx126xGetLoRaPacketStatus
** 功    能： 获取接收信号质量相关的参数，工作在lora模式下使用
** 输入参数： spi_control：基本控制函数指针
** 输出参数： pktStatus：回读值存储变量指针
** 返    回： 无
==================================================================
*/
void Sx126xGetLoRaPacketStatus(LORA_RX_SIGNAL_STATUS_T *pktStatus, LORA_CONTROL_FUNC_T *spi_control);

/*
==================================================================
** 函数名称： Sx126xGetGfskPacketStatus
** 功    能： 获取接收信号的参数，工作在GFSK模式下使用
** 输入参数： spi_control：基本控制函数指针
** 输出参数： pktStatus：回读值存储变量指针
** 返    回： 无
==================================================================
*/
void Sx126xGetGfskPacketStatus(LORA_RX_SIGNAL_STATUS_T *pktStatus, LORA_CONTROL_FUNC_T *spi_control);

/*
==================================================================
** 函数名称： Sx126xSetTxContinuousWave
** 功    能： 进入连续发送模式模式，测试用
** 输入参数： spi_control：基本控制函数指针
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void Sx126xSetTxContinuousWave(LORA_CONTROL_FUNC_T *spi_control);

#endif
