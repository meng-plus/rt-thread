/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-05-18     Bernard      The first version for LPC40xx
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "drv_uart.h"
#include "lpc_gpio.h"
#include "lpc_pinsel.h"
#include "lpc_uart.h"
#include "lpc_gpdma.h"
#ifdef RT_USING_SERIAL
typedef struct drv_uart_pin
{
    uint8_t port;
    uint8_t pin;
    uint8_t nFunc;
} drv_uart_pin_t;

typedef struct lpc_uart
{
    const char *name;
    UART_ID_Type uart_id;
    IRQn_Type UART_IRQn;
    uint8_t nPriority;   /* 串口中断模式下的中断优先级		*/
    uint8_t nTxDMAReq;   /* TX DMA Request Number			*/
    uint8_t nRxDMAReq;   /* RX DMA Request Number			*/
    uint8_t nTxDMAChnnl; /* TX DMA Chnnl		*/
    uint8_t nRxDMAChnnl; /* RX DMA Chnnl		*/
    uint8_t dma_conn_tx;
    uint8_t dma_conn_rx;
    uint8_t rs485 : 1;
    drv_uart_pin_t tx;
    drv_uart_pin_t rx;
    drv_uart_pin_t oe;
} lpc_uart_t;
typedef struct drv_uart_ops
{
    struct rt_uart_ops ops;
    lpc_uart_t lpc_uart[SERIAL_NUM];
} drv_uart_ops_t;

static rt_err_t lpc_configure(struct rt_serial_device *serial, struct serial_configure *cfg);
static rt_err_t lpc_control(struct rt_serial_device *serial, int cmd, void *arg);
static int lpc_putc(struct rt_serial_device *serial, char c);
static int lpc_getc(struct rt_serial_device *serial);
rt_ssize_t dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction);
static rt_serial_t serial[SERIAL_NUM];
const static drv_uart_ops_t lpc_uart_ops = {
    .ops =
        {
            lpc_configure,
            lpc_control,
            lpc_putc,
            lpc_getc,
            dma_transmit,
        },
    .lpc_uart = {
#ifdef BSP_USING_BOARD_315101Z2V2
        [UART_0] = {
            .name = "uart0",
            .uart_id = UART_0,
            .UART_IRQn = UART0_IRQn,
            .nPriority = 2,
            .nTxDMAReq = 10,
            .nRxDMAReq = 11,
            .nTxDMAChnnl = 0,
            .nRxDMAChnnl = 1,
            .dma_conn_tx = GPDMA_CONN_UART0_Tx,
            .dma_conn_rx = GPDMA_CONN_UART0_Rx,
            .rs485 = 0,

            .tx = {0, 2, 1},
            .rx = {0, 3, 1},
            .oe = {0, 14, 0},
        },
        [UART_1] = {
            .name = "uart1",
            .uart_id = UART_1,
            .UART_IRQn = UART1_IRQn,
            .nPriority = 2,
            .nTxDMAReq = 12,
            .nRxDMAReq = 13,
            .nTxDMAChnnl = 2,
            .nRxDMAChnnl = 3,
            .dma_conn_tx = GPDMA_CONN_UART1_Tx,
            .dma_conn_rx = GPDMA_CONN_UART1_Rx,
            .rs485 = 0,

            .tx = {0, 15, 1},
            .rx = {0, 16, 1},
            .oe = {0, 6, 4},
        },
        [UART_2] = {
            .name = "uart2",
            .uart_id = UART_2,
            .UART_IRQn = UART2_IRQn,
            .nPriority = 1,
            .nTxDMAReq = 14,
            .nRxDMAReq = 15,
            .nTxDMAChnnl = 4,
            .nRxDMAChnnl = 5,
            .dma_conn_tx = GPDMA_CONN_UART2_Tx,
            .dma_conn_rx = GPDMA_CONN_UART2_Rx,
            .rs485 = 1,

            .tx = {4, 22, 2},
            .rx = {4, 23, 2},
            .oe = {1, 19, 6},
        },
        [UART_3] = {
            .name = "uart3",
            .uart_id = UART_3,
            .UART_IRQn = UART3_IRQn,
            .nPriority = 2,
            .nTxDMAReq = 10,
            .nRxDMAReq = 11,
            .nTxDMAChnnl = 0,
            .nRxDMAChnnl = 0,
            .dma_conn_tx = GPDMA_CONN_UART3_Tx,
            .dma_conn_rx = GPDMA_CONN_UART3_Rx,
            .rs485 = 1,

            .tx = {0, 25, 3},
            .rx = {0, 26, 3},
            .oe = {1, 30, 3},
        },
        [UART_4] = {
            .name = "uart4",
            .uart_id = UART_4,
            .UART_IRQn = UART4_IRQn,
            .nPriority = 2,
            .nTxDMAReq = 12,
            .nRxDMAReq = 13,
            .nTxDMAChnnl = 0,
            .nRxDMAChnnl = 1,
            .dma_conn_tx = GPDMA_CONN_UART4_Tx,
            .dma_conn_rx = GPDMA_CONN_UART4_Rx,
            .rs485 = 1,
            .tx = {5, 4, 4},
            .rx = {5, 3, 4},
            .oe = {0, 21, 3},
        },
#endif
    }};

static void uart_gpio_init(const lpc_uart_t *uart)
{
    RT_ASSERT(NULL != uart);

    PINSEL_ConfigPin(uart->tx.port, uart->tx.pin, uart->tx.nFunc); // 0: Select GPIO (Default)
    GPIO_SetDir(uart->tx.port, uart->tx.pin, 1);
    PINSEL_SetPinMode(uart->tx.port, uart->tx.pin, PINSEL_BASICMODE_PULLUP);

    PINSEL_ConfigPin(uart->rx.port, uart->rx.pin, uart->rx.nFunc); // 0: Select GPIO (Default)
    GPIO_SetDir(uart->rx.port, uart->rx.pin, 0);
    // PINSEL_SetPinMode(uart->tx.port, uart->tx.pin, PINSEL_BASICMODE_PULLUP);
    if (uart->rs485)
    {
        PINSEL_ConfigPin(uart->oe.port, uart->oe.pin, uart->oe.nFunc); // 0: Select GPIO (Default)
        GPIO_SetDir(uart->oe.port, uart->oe.pin, 1);
        PINSEL_SetPinMode(uart->oe.port, uart->oe.pin, PINSEL_BASICMODE_PULLUP);
    }
}

static rt_err_t lpc_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    const lpc_uart_t *uart;

    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);
    uart = (lpc_uart_t *)serial->parent.user_data;
    uart_gpio_init(uart);
    UART_CFG_Type UART_InitStruct;

    // UART_ConfigStructInit(&UART_InitStruct);
    UART_InitStruct.Baud_rate = cfg->baud_rate;
    UART_InitStruct.Databits = (UART_DATABIT_Type)cfg->data_bits;
    UART_InitStruct.Parity = (UART_PARITY_Type)cfg->parity;
    UART_InitStruct.Stopbits = (UART_STOPBIT_Type)cfg->stop_bits;
    UART_Init(uart->uart_id, &UART_InitStruct);

    UART_FIFO_CFG_Type FIFOCfg;
    // UART_FIFOConfigStructInit(&FIFOCfg);
    FIFOCfg.FIFO_DMAMode = ENABLE;
    FIFOCfg.FIFO_Level = UART_FIFO_TRGLEV3;
    FIFOCfg.FIFO_ResetRxBuf = ENABLE;
    FIFOCfg.FIFO_ResetTxBuf = ENABLE;
    UART_FIFOConfig(uart->uart_id, &FIFOCfg);

    UART_TxCmd(uart->uart_id, ENABLE);
    if (uart->rs485 != 0)
    {
        UART1_RS485_CTRLCFG_Type RS485ConfigStruct;
        RS485ConfigStruct.AutoDirCtrl_State = ENABLE;
        RS485ConfigStruct.DirCtrlPol_Level = SET;
        RS485ConfigStruct.DelayValue = 1;
        UART_RS485Config(uart->uart_id, &RS485ConfigStruct);
    }
    return RT_EOK;
}

static rt_err_t lpc_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    uint32_t RT_DEVICE_FLAG = (uint32_t)arg;
    struct lpc_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct lpc_uart *)serial->parent.user_data;

    switch (cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
        if (RT_DEVICE_FLAG_INT_RX == RT_DEVICE_FLAG)
        {
            UART_IntConfig(uart->uart_id, UART_INTCFG_RBR, DISABLE);
        }
        else if (RT_DEVICE_FLAG_INT_TX == RT_DEVICE_FLAG)
        {
            UART_IntConfig(uart->uart_id, UART_INTCFG_THRE, DISABLE);
        }
        else if (RT_DEVICE_FLAG_DMA_TX == RT_DEVICE_FLAG)
        {
            GPDMA_ChannelCmd(uart->nTxDMAChnnl, DISABLE);
        }
        else if (RT_DEVICE_FLAG_DMA_RX == RT_DEVICE_FLAG)
        {
            GPDMA_ChannelCmd(uart->nRxDMAChnnl, DISABLE);
        }

        break;
    case RT_DEVICE_CTRL_SET_INT:
        /* enable rx irq */

        NVIC_SetPriority(uart->UART_IRQn, NVIC_EncodePriority(2, uart->nPriority, 0));
        NVIC_EnableIRQ(uart->UART_IRQn);
        if (RT_DEVICE_FLAG_INT_RX == RT_DEVICE_FLAG)
        {
            UART_IntConfig(uart->uart_id, UART_INTCFG_RBR, ENABLE);
        }
        else if (RT_DEVICE_FLAG_INT_TX == RT_DEVICE_FLAG)
        {
            UART_IntConfig(uart->uart_id, UART_INTCFG_THRE, ENABLE);
        }
        break;
    case RT_DEVICE_CTRL_CONFIG: // for rx dma
        if (RT_DEVICE_FLAG_DMA_TX == RT_DEVICE_FLAG ||
            RT_DEVICE_FLAG_DMA_RX == RT_DEVICE_FLAG)
        {
            NVIC_EnableIRQ(DMA_IRQn);
        }
        break;
    }

    return RT_EOK;
}

static int lpc_putc(struct rt_serial_device *serial, char c)
{
    struct lpc_uart *uart;
    uart = (struct lpc_uart *)serial->parent.user_data;
    rt_uart_rs485_dir(uart->uart_id, 1);
    UART_Send(uart->uart_id, (uint8_t *)&c, 1, BLOCKING);
    rt_uart_rs485_dir(uart->uart_id, 0);
    //    while (!(uart->UART->LSR & 0x20))
    //        ;
    //    uart->UART->THR = c;

    return 1;
}

static int lpc_getc(struct rt_serial_device *serial)
{
    struct lpc_uart *uart;

    uart = (struct lpc_uart *)serial->parent.user_data;
    __IO uint32_t *LSR = NULL;
    switch (uart->uart_id)
    {
    case UART_0:
        LSR = (__IO uint32_t *)&LPC_UART0->LSR;
        break;
    case UART_1:
        LSR = (__IO uint32_t *)&LPC_UART1->LSR;
        break;
    case UART_2:
        LSR = (__IO uint32_t *)&LPC_UART2->LSR;
        break;
    case UART_3:
        LSR = (__IO uint32_t *)&LPC_UART3->LSR;
        break;
    case UART_4:
        LSR = (__IO uint32_t *)&LPC_UART4->LSR;
        break;
    }
    if (*LSR & UART_LSR_RDR)
        return UART_ReceiveByte(uart->uart_id);
    else
        return -1;
}
rt_ssize_t dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
{
    struct lpc_uart *uart;

    uart = (struct lpc_uart *)serial->parent.user_data;
    if (RT_SERIAL_DMA_TX == direction)
    {
        rt_uart_rs485_dir(uart->uart_id, 1);
        // TODO: need  fix it
        GPDMA_Channel_CFG_Type GPDMAChannelConfig;
        GPDMAChannelConfig.ChannelNum = uart->nTxDMAChnnl;
        GPDMAChannelConfig.TransferSize = size;
        GPDMAChannelConfig.TransferWidth = GPDMA_WIDTH_BYTE;
        GPDMAChannelConfig.SrcMemAddr = (uint32_t)buf;
        //  GPDMAChannelConfig.DstMemAddr;// needn't config it
        GPDMAChannelConfig.TransferType = GPDMA_TRANSFERTYPE_M2P;
        GPDMAChannelConfig.DstConn = uart->dma_conn_tx;
        GPDMAChannelConfig.DMALLI = 0;
        GPDMA_Setup(&GPDMAChannelConfig);
        GPDMA_ChannelCmd(uart->nTxDMAChnnl, ENABLE);
    }
    else
    {
        GPDMA_Channel_CFG_Type GPDMAChannelConfig;
        GPDMAChannelConfig.ChannelNum = uart->nRxDMAChnnl;
        GPDMAChannelConfig.TransferSize = size;
        GPDMAChannelConfig.TransferWidth = GPDMA_WIDTH_BYTE;
        // GPDMAChannelConfig.SrcMemAddr = (uint32)&pUART->RBR;
        GPDMAChannelConfig.DstMemAddr = (uint32_t)buf; // need  config it
        GPDMAChannelConfig.TransferType = GPDMA_TRANSFERTYPE_P2M;
        GPDMAChannelConfig.SrcConn = uart->dma_conn_rx;
        GPDMAChannelConfig.DMALLI = 0;
        GPDMA_Setup(&GPDMAChannelConfig);

        GPDMA_ChannelCmd(uart->nRxDMAChnnl, ENABLE);
    }
    return 0;
}
void rt_uart_rs485_dir(UART_ID_Type SERIAL_ID, uint8_t dir)
{
    uint8_t id = (uint8_t)SERIAL_ID;
    RT_ASSERT(id < SERIAL_NUM);
    // if (dir)
    //     GPIO_SetValue(lpc_uart_ops.lpc_uart[id].oe.port, 0x01 << lpc_uart_ops.lpc_uart[id].oe.pin);
    // else
    //     GPIO_ClearValue(lpc_uart_ops.lpc_uart[id].oe.port, 0x01 << lpc_uart_ops.lpc_uart[id].oe.pin);
}
void uart_isr(rt_serial_t *serial_ptr)
{
    volatile uint32_t IIR, tmp;
    struct lpc_uart *uart;

    uart = (struct lpc_uart *)serial->parent.user_data;
    /* enter interrupt */
    rt_interrupt_enter();

    IIR = UART_GetIntId(uart->uart_id);
    if (UART_IIR_INTID_RDA & IIR)
    {
        rt_hw_serial_isr(serial_ptr, RT_SERIAL_EVENT_RX_IND);
    }
    if (UART_IIR_INTID_CTI & IIR)
    { // rt_hw_serial_isr(serial_ptr, RT_SERIAL_EVENT_RX_TIMEOUT);
        rt_hw_serial_isr(serial_ptr, RT_SERIAL_EVENT_RX_IND);
    }
    if (UART_IIR_INTID_THRE & IIR)
    {
        rt_hw_serial_isr(serial_ptr, RT_SERIAL_EVENT_TX_DONE);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}
#ifdef BSP_USING_UART0

void UART0_IRQHandler()
{
    uart_isr(&serial[UART_0]);
}
#endif
#ifdef BSP_USING_UART1

void UART1_IRQHandler()
{
    uart_isr(&serial[UART_1]);
}
#endif
#ifdef BSP_USING_UART2
void UART2_IRQHandler()
{
    uart_isr(&serial[UART_2]);
}
#endif
#ifdef BSP_USING_UART3
void UART3_IRQHandler()
{
    uart_isr(&serial[UART_3]);
}
#endif
#ifdef BSP_USING_UART4
void UART4_IRQHandler()
{
    uart_isr(&serial[UART_4]);
}
#endif
#ifdef RT_SERIAL_USING_DMA
void DMA_UART_isr(void)
{

    for (size_t i = 0; i < SERIAL_NUM; i++)
    {
        if ((LPC_GPDMA->IntStat & GPDMA_DMACIntTCStat_Ch(lpc_uart_ops.lpc_uart[i].nTxDMAChnnl)) != 0)
        {
            rt_hw_serial_isr(&serial[i], RT_SERIAL_EVENT_TX_DMADONE);
            GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, lpc_uart_ops.lpc_uart[i].nTxDMAChnnl);
        }
        if ((LPC_GPDMA->IntStat & GPDMA_DMACIntTCStat_Ch(lpc_uart_ops.lpc_uart[i].nRxDMAChnnl)) != 0)
        {
            rt_hw_serial_isr(&serial[i], RT_SERIAL_EVENT_RX_DMADONE);
            GPDMA_ClearIntPending(GPDMA_STATCLR_INTTC, lpc_uart_ops.lpc_uart[i].nRxDMAChnnl);
        }
    }
}
#endif
static rt_err_t rx_indicate(rt_device_t dev, rt_size_t size)
{ // 接受回调
    return 0;
}
static rt_err_t tx_complete(rt_device_t dev, void *buffer)
{ // 发送回调
    struct lpc_uart *uart;

    uart = (struct lpc_uart *)dev->user_data;
    rt_uart_rs485_dir(uart->uart_id, 0);
    return 0;
}

int rt_hw_uart_init(void)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    for (int i = 0; i < SERIAL_NUM; i++)
    {
        serial[i].ops = &lpc_uart_ops.ops;
        serial[i].config = config;
        /* register UART1 device */
        rt_hw_serial_register(&serial[i],
                              lpc_uart_ops.lpc_uart[i].name,
                              RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                              (void *)&lpc_uart_ops.lpc_uart[i]);
        rt_device_set_rx_indicate(&serial[i].parent, rx_indicate);
        rt_device_set_tx_complete(&serial[i].parent, tx_complete);
    }
    return 0;
}
INIT_BOARD_EXPORT(rt_hw_uart_init);
#endif /* RT_USING_SERIAL */
