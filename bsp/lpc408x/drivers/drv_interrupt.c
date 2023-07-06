/**
 * @file drv_interrupt.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief  LPC4088中断处理函数
 * @version 0.1
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "LPC407x_8x_177x_8x.h"

__weak void NMI_Handler() //
{
}
__weak void HardFault_Handler() //
{
  while (1)
    ;
}
__weak void MemManage_Handler() //
{
}
__weak void BusFault_Handler() //
{
}
__weak void UsageFault_Handler() //
{
}
__weak void SVC_Handler() //
{
}
__weak void DebugMon_Handler() //
{
}
__weak void PendSV_Handler() //
{
}
__weak void SysTick_Handler() //
{
}
__weak void WDT_IRQHandler() //            ; 16: Watchdog Timer
{
}
__weak void TIMER0_IRQHandler() //         ; 17: Timer0
{
}
__weak void TIMER1_IRQHandler() //         ; 18: Timer1
{
}
__weak void TIMER2_IRQHandler() //         ; 19: Timer2
{
}
__weak void TIMER3_IRQHandler() //         ; 20: Timer3
{
}
__weak void UART0_IRQHandler() //          ; 21: UART0
{
}
__weak void UART1_IRQHandler() //          ; 22: UART1
{
}
__weak void UART2_IRQHandler() //          ; 23: UART2
{
}
__weak void UART3_IRQHandler() //          ; 24: UART3
{
}
__weak void PWM1_IRQHandler() //           ; 25: PWM1
{
}
__weak void I2C0_IRQHandler() //           ; 26: I2C0
{
}
__weak void I2C1_IRQHandler() //           ; 27: I2C1
{
}
__weak void I2C2_IRQHandler() //           ; 28: I2C2
{
}
__weak void SSP0_IRQHandler() //           ; 30: SSP0
{
}
__weak void SSP1_IRQHandler() //           ; 31: SSP1
{
}
__weak void PLL0_IRQHandler() //           ; 32: PLL0 Lock (Main PLL)
{
}
__weak void RTC_IRQHandler() //            ; 33: Real Time Clock
{
}
__weak void EINT0_IRQHandler() //          ; 34: External Interrupt 0
{
}
__weak void EINT1_IRQHandler() //          ; 35: External Interrupt 1
{
}
__weak void EINT2_IRQHandler() //          ; 36: External Interrupt 2
{
}
__weak void EINT3_IRQHandler() //          ; 37: External Interrupt 3
{
}
__weak void ADC_IRQHandler() //            ; 38: A/D Converter
{
}
__weak void BOD_IRQHandler() //            ; 39: Brown-Out Detect
{
}
__weak void USB_IRQHandler() //            ; 40: USB
{
}
__weak void CAN_IRQHandler() //            ; 41: CAN
{
}
__weak void DMA_IRQHandler() //            ; 42: General Purpose DMA
{
  void DMA_UART_isr(void);
  DMA_UART_isr();
}
__weak void I2S_IRQHandler() //            ; 43: I2S
{
}
__weak void ENET_IRQHandler() //           ; 44: Ethernet
{
}
__weak void MCI_IRQHandler() //		  ; 45: MCI Card
{
}
__weak void MCPWM_IRQHandler() //          ; 46: Motor Control PWM
{
}
__weak void QEI_IRQHandler() //            ; 47: Quadrature Encoder Interface
{
}
__weak void PLL1_IRQHandler() //           ; 48: PLL1 Lock (USB PLL)
{
}
__weak void USBActivity_IRQHandler() //	  ; 49: USB Activity Interrupt
{
}
__weak void CANActivity_IRQHandler() //	  ; 50: CAN Activity Interrupt
{
}
__weak void UART4_IRQHandler() //          ; 51: UART4
{
}
__weak void SSP2_IRQHandler() //		  ; 52: SSP2
{
}
__weak void LCD_IRQHandler() //		  ; 53: LCD
{
}
__weak void GPIO_IRQHandler() //		  ; 54: GPIO
{
}
__weak void PWM0_IRQHandler() //		  ; 55: PWM0
{
}
__weak void EEPROM_IRQHandler() //	  ; 56: EEPROM
{
}
