/*
****************************************Copyright(c)************************************
**                                  Zhengzhou GL. TECH Co.,Ltd
**___________________________File Info_________________________
**文件名称：lora_base_control.c
**工程项目：MIOT，TEST_BOARD,射频模块测试项目
**说    明：本文件用于MIOT项目中的TEST_BOARD硬件板，提供lora模块操作的基本函数
**
**作    者：吴树          日    期：2022年01月20日
**建立版本：V1.0
**
**________________________modification_________________________
**版本            日期               升级记录
**V1.0        2022年01月20日     初始版本构建
*****************************************************************************************
*/
// #include "LPC407x_8x.h"
#include "LPC407x_8x_177x_8x.h"
#include "lora_base_control.h" //使用lora模块基本操作函数

/*
===============================================================================
==
===============================================================================
*/

// hwx 2023.08.10 add.
void Chip_GPIO_InputPullCfg(uint32_t pin, uint8_t value);
void Chip_GPIO_SetPinState(LPC_GPIO_TypeDef *PORT, uint8_t pin, uint8_t value);
void Chip_GPIO_DirCfgIn(LPC_GPIO_TypeDef *PORT, uint8_t pin);
void Chip_GPIO_DirCfgOut(LPC_GPIO_TypeDef *PORT, uint8_t pin);
uint8_t Chip_GPIO_ReadPinState(LPC_GPIO_TypeDef *PORT, uint8_t pin);
uint8_t LORA_SPI_Get(void);
uint8_t LORA_Bussy_Get(void);
void Lora_Power_Ctrl(uint8_t state);
void LORA_DelayUs(volatile uint32_t time);
uint8_t LoraPower_GetState(void);

/*
===============================================================================
==
===============================================================================
*/
// hwx 2023.08.10 redefine.
// 该版本底板仅1个lora
#define LORA1_CS_PORT LPC_GPIO1 // CS	P1.0
#define LORA1_CS_PIN 0
#define LORA1_CS_IOCN 100

#define LORA1_SCLK_PORT LPC_GPIO1
#define LORA1_SCLK_PIN 14 // SCK 	P1.14
#define LORA1_SCLK_IOCN 114

#define LORA1_MOSI_PORT LPC_GPIO4
#define LORA1_MOSI_PIN 28 // MOSI	P4.28
#define LORA1_MOSI_IOCN 428

#define LORA1_MISO_PORT LPC_GPIO1
#define LORA1_MISO_PIN 8 // MISO	P1.8
#define LORA1_MISO_IOCN 108

// DIO1		IRQ
#define LORA1_DIO1_PORT LPC_GPIO0 // DIO1	P0.6
#define LORA1_DIO1_PIN 6
#define LORA1_DIO1_IOCN 006
#define LORA1_DIO1_ISR_HANDLER GPIO_IRQHandler

#define LORA1_BUSY_PORT LPC_GPIO1 // BUSY	P1.9
#define LORA1_BUSY_PIN 9
#define LORA1_BUSY_IOCN 109

#define LORA1_RESET_PORT LPC_GPIO1 // RST	P1.1
#define LORA1_RESET_PIN 1
#define LORA1_RESET_IOCN 101

#define LORA1_Power_PORT LPC_GPIO2 // POWER	P2.21
#define LORA1_Power_PIN 21
#define LORA1_Power_IOCN 221

// hwx 2023.08.10 redefine.
#define SPI_CS_0 Chip_GPIO_SetPinState(LORA1_CS_PORT, LORA1_CS_PIN, 0)
#define SPI_CS_1 Chip_GPIO_SetPinState(LORA1_CS_PORT, LORA1_CS_PIN, 1)

#define SPI_SCLK_0 Chip_GPIO_SetPinState(LORA1_SCLK_PORT, LORA1_SCLK_PIN, 0)
#define SPI_SCLK_1 Chip_GPIO_SetPinState(LORA1_SCLK_PORT, LORA1_SCLK_PIN, 1)

#define SPI_MOSI_0 Chip_GPIO_SetPinState(LORA1_MOSI_PORT, LORA1_MOSI_PIN, 0)
#define SPI_MOSI_1 Chip_GPIO_SetPinState(LORA1_MOSI_PORT, LORA1_MOSI_PIN, 1)

#define SPI_RESET_0 Chip_GPIO_SetPinState(LORA1_RESET_PORT, LORA1_RESET_PIN, 0)
#define SPI_RESET_1 Chip_GPIO_SetPinState(LORA1_RESET_PORT, LORA1_RESET_PIN, 1)

#define SPI_MISO_RD LORA_SPI_Get()
#define SPI_BUSY_RD LORA_Bussy_Get()

#define LORA_IO_LPM(x, y, w)                          \
    {                                                 \
        Chip_GPIO_DirCfgIn(x, y);                     \
        Chip_GPIO_InputPullCfg(w, GPIO_PULL_DISABLE); \
    }

/*
==================================================================
== 变量定义
==================================================================
*/

// 定义系统中使用的射频模块类型
LORA_CHIPTYPE_E g_loraType[MAX_LORA_NUM] = {LORA_1262};

static uint8_t flagIRQ[MAX_LORA_NUM] = {0};

/*
==================================================================
== 函数定义
==================================================================
*/
void LORA_DelayUs(volatile uint32_t time)
{
    // DelayUs(time);      //外部定义delay函数

    // hwx 2023.08.10 change.   ;
    while (time)
    {
        for (volatile int i = 20; i != 0; i--) // 4
        {
        }

        time--;
    }
}

/*
==================================================================
** 函数名称： LORA1_SpiWriteReadByte
** 功    能： lora1模块spi读写一个字节,模拟spi方式
** 输入参数： out：spi写入数据
** 输出参数： 无
** 返    回： spi回读的数据
==================================================================
*/
uint8_t LORA1_SpiWriteReadByte(uint8_t out)
{
    uint8_t i;
    uint8_t temp = 0;

    SPI_SCLK_0; // 拉低数据线

    for (i = 0; i < 8; i++) // 总共8个bit
    {
        if (out & 0x80) // 根据数据确认MOSI状态
            SPI_MOSI_1; // bit1
        else
            SPI_MOSI_0; // bit0

        SPI_SCLK_1; // 拉高时钟线

        LORA_DelayUs(2); // 延时一下  ，在操作1280时，延时不能大于4个us

        temp = (temp << 1);     // 将之前读取数据左移一位
        if (SPI_MISO_RD != 0)   // 获取当前MISO状态
            temp = temp | 0x01; // 如果为高，该位置1，默认为0

        out = (out << 1); // 准备发送下一个bit的数据
        SPI_SCLK_0;       // 拉低时钟线
        LORA_DelayUs(2);  // 延时一下
    }
    return temp;
}

/*
==================================================================
** 函数名称： LORA1_NssControl
** 功    能： lora1模块片选引脚控制
** 输入参数： cmd：片选引脚控制，0表示低电平（低电平有效），非0表示高电平
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA1_NssControl(uint8_t cmd)
{
    if (cmd == 0)
        SPI_CS_0;
    else
        SPI_CS_1;

    LORA_DelayUs(4); // 延时一下？
}

/*
==================================================================
** 函数名称： LORA1_ResetControl
** 功    能： lora1模块复位引脚控制
** 输入参数： cmd：片选引脚控制，0表示低电平（低电平有效），非0表示高电平
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA1_ResetControl(uint8_t cmd)
{
    if (cmd == 0)
    {
        SPI_RESET_0;
    }
    else
    {
        SPI_RESET_1;
    }
}

/*
==================================================================
** 函数名称： LORA1_BusyCheck
** 功    能： lora1模块busy引脚状态读取
** 输入参数： 无
** 输出参数： 无
** 返    回： 非0表示busy引脚高
==================================================================
*/
uint32_t LORA1_BusyCheck(void)
{
    return SPI_BUSY_RD;
}

/*==================================================================
** 函数名称： LORA1_GpioInit
** 功    能： lora1模块引脚初始化
** 输入参数： 无
** 输出参数： 无
** 返    回： 无
==================================================================*/
// hwx 2023.08.10 redefine.
void LORA1_GpioInit(void)
{
    // GPIO Power
    LPC_SC->PCONP |= (1 << 15);

    ////////////////////////////////////////
    // GPIO	IOCON set
    LPC_IOCON->P1_9 = (00 << 0) | // IOCON_FUNC0            //lora BUSY
                                  //			  (00 << 3) ;     //IOCON_MODE_INACT	3
                                  //(1 << 3) ;        //pull down
                      (2 << 3);   // pull up

    LPC_IOCON->P1_8 = (00 << 0) | // IOCON_FUNC0            //lora MISO
                                  //			  (00 << 3) ;     //IOCON_MODE_INACT	3
                                  //(1 << 3) ;        //pull down
                      (2 << 3);   // pull up

    LPC_IOCON->P1_14 = (00 << 0) | // IOCON_FUNC0            //lora CLK
                       (2 << 3) |  // pull up
                       (1 << 5) |
                       (0 << 10);

    LPC_IOCON->P4_28 = (00 << 0) | // IOCON_FUNC0            //lora MOSI
                       (2 << 3) |  // pull up
                       (1 << 5) |
                       (0 << 10);

    LPC_IOCON->P1_0 = (00 << 0) | // IOCON_FUNC0            //lora CS
                      (00 << 3);  // IOCON_MODE_INACT

    LPC_IOCON->P1_1 = (00 << 0) | // IOCON_FUNC0            //lora RES
                      (2 << 3) |  // pull up
                      (1 << 5) |
                      (0 << 10);

    // Setup port direction
    LPC_GPIO1->DIR &= ~(1 << 9); // lora BUSY
    LPC_GPIO1->DIR &= ~(1 << 8); // lora MISO

    LPC_GPIO1->DIR |= 1 << 14; // lora CLK
    LPC_GPIO4->DIR |= 1 << 28; // lora MOSI
    LPC_GPIO1->DIR |= 1 << 0;  // lora CS
    LPC_GPIO1->DIR |= 1 << 1;  // lora RES

    // hwx 2023.08.10 add.lora POWER
    LPC_GPIO2->DIR |= (1 << 21);

    // hwx 2023.08.10 add.//lora POWER
    LPC_IOCON->P2_21 = (00 << 0) | // IOCON_FUNC0
                       (2 << 3) |  // pull up
                       (1 << 5) |
                       (0 << 10);

    // out Value
    // LPC_GPIO1->SET	= 1 << 14;       //clk
    // LPC_GPIO4->CLR	= 1 << 28;      // MOSI  Dout
    // LPC_GPIO1->SET	= 1 << 0;       //CS
    // LPC_GPIO1->SET	= 1 << 1;       //RES

    //////////////////////////////////////
    // INTERRUPT
    // lora DIO1 Configure GPIO interrupt pin as input
    LPC_IOCON->P0_6 = (00 << 0) | // IOCON_FUNC0
                                  //(00 << 3) ;     //IOCON_MODE_INACT	3
                      //(1 << 3) ;        //pull down
                      (2 << 3); // pull up

    // direction
    LPC_GPIO0->DIR &= ~(1 << 6);

    // Configure the GPIO interrupt
    LPC_GPIOINT->IO0IntEnR = 1 << 6;

    // Enable interrupt in the NVIC
    NVIC_SetPriority(GPIO_IRQn, 12);
    NVIC_ClearPendingIRQ(GPIO_IRQn);
    NVIC_EnableIRQ(GPIO_IRQn);
}

/*
*********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
void Chip_GPIO_InputPullCfg(uint32_t pin, uint8_t value)
{
    switch (pin)
    {
    case LORA1_CS_IOCN: //	P1_0
        LPC_IOCON->P1_0 = (00 << 0) | (value << 3);
        break;

    case LORA1_SCLK_IOCN: // P1_14
        LPC_IOCON->P1_14 = (00 << 0) | (value << 3);
        break;

    case LORA1_MOSI_IOCN: // P4_28
        LPC_IOCON->P4_28 = (00 << 0) | (value << 3);
        break;

    case LORA1_MISO_IOCN: // P1_8
        LPC_IOCON->P1_8 = (00 << 0) | (value << 3);
        break;

    case LORA1_DIO1_IOCN: // P0_6
        LPC_IOCON->P0_6 = (00 << 0) | (value << 3);
        break;

    case LORA1_BUSY_IOCN: // P1_9
        LPC_IOCON->P1_9 = (00 << 0) | (value << 3);
        break;

    case LORA1_RESET_IOCN: // P1_1
        LPC_IOCON->P1_1 = (00 << 0) | (value << 3);
        break;
    }
}

/*
*********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
void Chip_GPIO_SetPinState(LPC_GPIO_TypeDef *PORT, uint8_t pin, uint8_t value)
{
    if (value != 0)
        PORT->SET = 1 << pin;
    else
        PORT->CLR = 1 << pin;
}

/*
*********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
void Chip_GPIO_DirCfgIn(LPC_GPIO_TypeDef *PORT, uint8_t pin)
{
    PORT->DIR &= ~(1 << pin);
}

/*
*********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
void Chip_GPIO_DirCfgOut(LPC_GPIO_TypeDef *PORT, uint8_t pin)
{
    PORT->DIR |= 1 << pin;
}

/*
*********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
uint8_t Chip_GPIO_ReadPinState(LPC_GPIO_TypeDef *PORT, uint8_t pin)
{
    if ((PORT->PIN) & (1ul << pin) == 0)
        return 0;
    else
        return 1;
}

/*
*********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
uint8_t LORA_SPI_Get(void)
{
    if ((LORA1_MISO_PORT->PIN & (1ul << LORA1_MISO_PIN)) == 0)
        return 0;
    else
        return 1;
}

/*
*********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
**********************************************************************************
*/
uint8_t LORA_Bussy_Get(void)
{
    if ((LORA1_BUSY_PORT->PIN & (1ul << LORA1_BUSY_PIN)) == 0)
        return 0;
    else
        return 1;
}

/*
==================================================================
** 函数名称： LORA1_GpioDeInit
** 功    能：
** 输入参数： 无
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA1_GpioDeInit(void)
{
    LORA_IO_LPM(LORA1_MOSI_PORT, LORA1_MOSI_PIN, LORA1_MOSI_IOCN);

    LORA_IO_LPM(LORA1_MISO_PORT, LORA1_MISO_PIN, LORA1_MOSI_IOCN);

    LORA_IO_LPM(LORA1_DIO1_PORT, LORA1_DIO1_PIN, LORA1_MOSI_IOCN);

    LORA_IO_LPM(LORA1_CS_PORT, LORA1_CS_PIN, LORA1_MOSI_IOCN);

    LORA_IO_LPM(LORA1_SCLK_PORT, LORA1_SCLK_PIN, LORA1_MOSI_IOCN);

    LORA_IO_LPM(LORA1_BUSY_PORT, LORA1_BUSY_PIN, LORA1_MOSI_IOCN);

    LORA_IO_LPM(LORA1_RESET_PORT, LORA1_RESET_PIN, LORA1_MOSI_IOCN);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

// 对于无外部功放控制的函数，设置对应函数为NULL
LORA_CONTROL_FUNC_T g_loraControlFuncs[MAX_LORA_NUM] =
    {
        {LORA1_SpiWriteReadByte,
         LORA1_NssControl,
         LORA1_ResetControl,
         LORA1_BusyCheck,
         LORA_DelayUs},
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
==================================================================
** 函数名称： LORA_GpioInit
** 功    能： lora模块引脚初始化
** 输入参数： num：对应的lora模块编号，从0开始
** 输出参数： 无
** 返    回： 无
==================================================================
*/
void LORA_HwInit(int num)
{
    switch (num)
    {
    case 0:
        LORA1_GpioInit();
        break;
    default:
        break;
    }
}

/*
==================================================================
** 函数名称：
** 功    能：
** 输入参数：
** 输出参数：
** 返    回：
==================================================================
*/
int LORA_GetFlagIRQ(int num)
{
    if (flagIRQ[num] == 0)
        return 0;

    flagIRQ[num] = 0;

    return 1;
}
/*
==================================================================
** 函数名称：
** 功    能：
** 输入参数：
** 输出参数：
** 返    回：
==================================================================
*/
// void LORA1_DIO1_ISR_HANDLER(void)
// {
//     // MAP_GPIO_clearInterruptFlag(LORA1_DIO1_PORT, (1 << LORA1_DIO1_PIN));
//     // hwx 2023.08.10 change.
//     LPC_GPIOINT->IO0IntClr = 1 << 6;

//     flagIRQ[0] = 1;
// }
void LORA1_DIO1_update_flag(uint8_t idx, uint8_t flag)
{
    flagIRQ[0] = 1;
}
