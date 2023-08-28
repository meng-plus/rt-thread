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
#include "rtdef.h"
rt_weak void NMI_Handler() //
{
}
rt_weak void HardFault_Handler() //
{
  while (1)
    ;
}
rt_weak void MemManage_Handler() //
{
}
rt_weak void BusFault_Handler() //
{
}
rt_weak void UsageFault_Handler() //
{
}
rt_weak void SVC_Handler() //
{
}
rt_weak void DebugMon_Handler() //
{
}
rt_weak void PendSV_Handler() //
{
}
rt_weak void SysTick_Handler() //
{
}
rt_weak void WDT_IRQHandler() //            ; 16: Watchdog Timer
{
}
rt_weak void TIMER0_IRQHandler() //         ; 17: Timer0
{
}
rt_weak void TIMER1_IRQHandler() //         ; 18: Timer1
{
}
rt_weak void TIMER2_IRQHandler() //         ; 19: Timer2
{
}
rt_weak void TIMER3_IRQHandler() //         ; 20: Timer3
{
}
rt_weak void UART0_IRQHandler() //          ; 21: UART0
{
}
rt_weak void UART1_IRQHandler() //          ; 22: UART1
{
}
rt_weak void UART2_IRQHandler() //          ; 23: UART2
{
}
rt_weak void UART3_IRQHandler() //          ; 24: UART3
{
}
rt_weak void PWM1_IRQHandler() //           ; 25: PWM1
{
}
rt_weak void I2C0_IRQHandler() //           ; 26: I2C0
{
}
rt_weak void I2C1_IRQHandler() //           ; 27: I2C1
{
}
rt_weak void I2C2_IRQHandler() //           ; 28: I2C2
{
}
rt_weak void SSP0_IRQHandler() //           ; 30: SSP0
{
}
rt_weak void SSP1_IRQHandler() //           ; 31: SSP1
{
}
rt_weak void PLL0_IRQHandler() //           ; 32: PLL0 Lock (Main PLL)
{
}
rt_weak void RTC_IRQHandler() //            ; 33: Real Time Clock
{
}
rt_weak void EINT0_IRQHandler() //          ; 34: External Interrupt 0
{
}
rt_weak void EINT1_IRQHandler() //          ; 35: External Interrupt 1
{
}
rt_weak void EINT2_IRQHandler() //          ; 36: External Interrupt 2
{
}
rt_weak void EINT3_IRQHandler() //          ; 37: External Interrupt 3
{
}
rt_weak void ADC_IRQHandler() //            ; 38: A/D Converter
{
}
rt_weak void BOD_IRQHandler() //            ; 39: Brown-Out Detect
{
}
rt_weak void USB_IRQHandler() //            ; 40: USB
{
}
rt_weak void CAN_IRQHandler() //            ; 41: CAN
{
}
rt_weak void DMA_IRQHandler() //            ; 42: General Purpose DMA
{
  void DMA_UART_isr(void);
  DMA_UART_isr();
}
rt_weak void I2S_IRQHandler() //            ; 43: I2S
{
}
rt_weak void ENET_IRQHandler() //           ; 44: Ethernet
{
}
rt_weak void MCI_IRQHandler() //		  ; 45: MCI Card
{
}
rt_weak void MCPWM_IRQHandler() //          ; 46: Motor Control PWM
{
}
rt_weak void QEI_IRQHandler() //            ; 47: Quadrature Encoder Interface
{
}
rt_weak void PLL1_IRQHandler() //           ; 48: PLL1 Lock (USB PLL)
{
}
rt_weak void USBActivity_IRQHandler() //	  ; 49: USB Activity Interrupt
{
}
rt_weak void CANActivity_IRQHandler() //	  ; 50: CAN Activity Interrupt
{
}
rt_weak void UART4_IRQHandler() //          ; 51: UART4
{
}
rt_weak void SSP2_IRQHandler() //		  ; 52: SSP2
{
}
rt_weak void LCD_IRQHandler() //		  ; 53: LCD
{
}
rt_weak void GPIO_IRQHandler() //		  ; 54: GPIO
{
}
rt_weak void PWM0_IRQHandler() //		  ; 55: PWM0
{
}
rt_weak void EEPROM_IRQHandler() //	  ; 56: EEPROM
{
}
