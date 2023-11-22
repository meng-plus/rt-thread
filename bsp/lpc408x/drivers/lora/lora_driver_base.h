/*
****************************************Copyright(c)************************************
**                                  Zhengzhou GL. TECH Co.,Ltd
**___________________________File Info_________________________
**文件名称：lora_driver_base.h
**工程项目：通用工程
**说    明：本文件用于SEMTECH公司SX系列射频模块及其扩展模组控制，
            多系列模组通用，支持单板多模组控制
            使用本文件需要根据主控芯片外部定义相关底层操作函数
**
**作    者：吴树          日    期：2022年02月11日
**建立版本：V1.1
**
**________________________modification_________________________
**版本            日期               升级记录
**V1.0        2022年01月10日     初始版本构建
**V1.1        2022年02月11日     增加对stm32wl系列的支持
*****************************************************************************************
*/

#ifndef _LORA_DRIVER_BASE_H_
#define _LORA_DRIVER_BASE_H_

#include <stdint.h> //使用简写变量类型名称

/*
==================================================================
== 变量类型声明
==================================================================
*/

//////////////////////////////////////模组配置////////////////////////////////////////////////

// 加载的LORA模块类型变量,用于兼容替换时方便驱动统一
// 当前，1262和LLCC68暂时公用驱动
// 1261和1262公用驱动，仅在配置发射功率时不同，需要通过传递模块类型进行区分
typedef enum
{
    LORA_NONE = 0,
    LORA_1276, // 历史使用版本，已基本被1262替代，原驱动未移植
    LORA_1261,
    LORA_1262,
    LORA_1280, // 2.4Glora版本，以后可能用得到，原驱动暂未移植
    LORA_LLCC68,
    LORA_STM32WL
} LORA_CHIPTYPE_E;

// lora模块的射频工作模式，支持lora模式和FSK模式
// 暂未完成sx1280的BLE工作模式的驱动控制
typedef enum
{
    WORK_MODEM_FSK = 0,
    WORK_MODEM_LORA,
    WORK_MODEM_BLE,    // sx1280专用，支持BLE通信模式
    WORK_MODEM_RANGING // sx1280专用，支持lora测距模式
} LORA_RADIO_MODES_E;

//////////////////////////////////////lora通信模式下使用////////////////////////////////////////////////

// lora模式下的扩频因子
typedef enum
{
    LORA_SF5 = 0,
    LORA_SF6,
    LORA_SF7,
    LORA_SF8,
    LORA_SF9,
    LORA_SF10,
    LORA_SF11,
    LORA_SF12
} LORA_SF_E;

// lora模式下的带宽，如果后续提供sx1280支持需要进行扩展和重定义
typedef enum
{
    LORA_BW_500K = 0,
    LORA_BW_250K = 1,
    LORA_BW_125K = 2,
    LORA_BW_062K = 3,
    LORA_BW_041K = 4,
    LORA_BW_031K = 5,
    LORA_BW_020K = 6,
    LORA_BW_015K = 7,
    LORA_BW_010K = 8,
    LORA_BW_007K = 9
} LORA_BW_E;

// lora模式下的循环编码率
typedef enum
{
    LORA_CR_4_5 = 0,
    LORA_CR_4_6,
    LORA_CR_4_7,
    LORA_CR_4_8
} LORA_CR_E;

// lora调制参数控制结构体变量，作为基本配置参数结构体的一个组成部分
// 当lora模块工作在lora通信模式下使用
typedef struct
{
    LORA_SF_E sf; // 扩频因子
    LORA_BW_E bw; // 带宽
    LORA_CR_E cr; // 循环编码率
    uint8_t ldro; // 低速率优化，0表示不开启，1表示开启（low_data_rate_optimize）
} LORA_MODULE_PARAS_T;

// lora模块lora工作模式下接收信号质量的结构体变量
// 针对lora通信模式，使用数据帧平均RSSI，估算的数据帧平均SNR以及估算的lora扩频以后的信号RSSI
// 某些情况下，rssi会出现-128以下的数值，因此统一使用16bit数据进行表示
typedef struct
{
    int16_t rssipkt;
    int16_t snr;
    int16_t rssilora;
} LORA_LORA_RX_SIGNAL_STATUS_T;

//////////////////////////////////////gfsk通信模式下使用////////////////////////////////////////////////

// GFSK模式下调制赋型参数,
typedef enum
{
    GFSK_SHAPING_OFF = 0,
    GFSK_SHAPING_G_BT_03,
    GFSK_SHAPING_G_BT_05,
    GFSK_SHAPING_G_BT_07,
    GFSK_SHAPING_G_BT_1,
} LORA_GFSK_MOD_SHAPINGS_E;

// GFSK模式下的接收带宽参数
typedef enum
{
    GFSK_RX_BW_4800 = 0,
    GFSK_RX_BW_5800,
    GFSK_RX_BW_7300,
    GFSK_RX_BW_9700,
    GFSK_RX_BW_11700,
    GFSK_RX_BW_14600,
    GFSK_RX_BW_19500,
    GFSK_RX_BW_23400,
    GFSK_RX_BW_29300,
    GFSK_RX_BW_39000,
    GFSK_RX_BW_46900,
    GFSK_RX_BW_58600,
    GFSK_RX_BW_78200,
    GFSK_RX_BW_93800,
    GFSK_RX_BW_117300,
    GFSK_RX_BW_156200,
    GFSK_RX_BW_187200,
    GFSK_RX_BW_234300,
    GFSK_RX_BW_312000,
    GFSK_RX_BW_373600,
    GFSK_RX_BW_467000,
} LORA_GFSK_RX_BW_E;

// gfsk调制参数控制结构体变量，作为基本配置参数结构体的一个组成部分
// 当lora模块工作在fsk通信模式下使用
typedef struct
{
    uint32_t br;                         // bitrate，GFSK通信速率   //取值范围600-300000，单位bps
    LORA_GFSK_MOD_SHAPINGS_E pulseshape; // 脉冲形状，指高斯低通滤波器的参数
    LORA_GFSK_RX_BW_E bw;                // 带宽，带宽参数和频率偏差、通信速率有直接关联,需要注意配置对应关系
    uint32_t fdev;                       // 频率偏差
} LORA_GFSK_MODULE_PARAS_T;

// lora基本配置参数，包括中心频率、发射功率以及调制参数
// 需要注意，sx127x系列的发射功率不能直接使用dbm值，需要根据配置进行调整
// sx127x系列，使用pa boost引脚时，配置的发射功率为db值+128，使用低功率引脚时，配置的发射功率值为db值+112
// sx128x,sx126x,LLCC68系列，发射功率可直接配置
// 调制参数，对于使用lora通信模式和gfsk通信模式配置不同，需要分别应用不同的参数，未使用模式的参数可以不配置
typedef struct
{
    LORA_CHIPTYPE_E type;                       // lora芯片类型
    LORA_RADIO_MODES_E mode;                    // 工作模式，本驱动支持FSK和LORA模式
    uint32_t frequency;                         // 工作频率
    uint8_t txpower;                            // 发射功率
    LORA_MODULE_PARAS_T modulationparams;       // 调制参数,lora模式使用
    LORA_GFSK_MODULE_PARAS_T gfsk_module_paras; // 调制参数，GFSK模式使用
} LORA_RADIO_SETTING_T;

// lora模块gfsk工作模式下接收信号质量的结构体变量
typedef struct
{
    uint16_t rxstatus;
    int16_t rssisync; // 同步地址的rssi
    int16_t rssiavg;  // 整包数据平均的rssi
} LORA_GFSK_RX_SIGNAL_STATUS_T;

//////////////////////////////////////模块通用参数////////////////////////////////////////////////

// lora模块中断类型表述，当前仅支持接收完成、发送完成中断处理,其他中断类型均标注为错误中断
// 如果要兼容sx1280芯片，需要添加测距相关状态，暂未实现
typedef enum
{
    LORA_IRQ_ERROR,
    LORA_IRQ_RXDONE,
    LORA_IRQ_TXDONE,
    LORA_IRQ_TIMEOUT,
} LORA_IRQ_STATE_E;

// lora模块的接收信号质量的共用体变量，兼容lora和gfsk工作模式
typedef union
{
    LORA_LORA_RX_SIGNAL_STATUS_T lora_signal_status;
    LORA_GFSK_RX_SIGNAL_STATUS_T gfsk_signal_status;
} LORA_RX_SIGNAL_STATUS_T;

// lora模块基本控制函数结构体，需要根据主控电路在外部进行定义并注册
typedef struct
{
    uint8_t (*SpiByteWriteReadFunc)(uint8_t data); // spi读写函数，data为写入值，返回值为读出值  //驱动暂时使用片选引脚单独控制，spi单字节写入方案，避免设置内部缓存，方便命令拼接操作
    void (*NssControlFunc)(uint8_t value);         // spi片选引脚控制函数，0表示低电平，1表示高电平，芯片片选低电平有效
    void (*ResetControlFunc)(uint8_t value);       // 芯片reset引脚控制函数，0表示低电平，1表示高电平,本系列芯片低电平复位有效
    uint32_t (*BusyReadFunc)(void);                // 芯片BUSY引脚状态读取函数，返回值0表示低电平，非0表示高电平;部分芯片不使用busy引脚时，对应函数可采用固定时隙延时，默认返回低电平，将不具备异常检测能力
    void (*DelayUsFunc)(uint32_t value);           // 微秒级延时函数
} LORA_CONTROL_FUNC_T;

#endif
