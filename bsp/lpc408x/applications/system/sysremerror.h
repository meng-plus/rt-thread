/**********************************Copyright (c)********************************
**                            ZhengZhou GuangLi Tech CO.,LTD
**                                All rights reserved.
**
**----------------------------------项目信息------------------------------------
** 文件名称： error.h
** 说    明： 运行过程中出现错误的错误标志设置程序和管理程序头文件
**
**----------------------------------版本历史------------------------------------
** 版本号	修改者	完成日期		内容
**
*******************************************************************************/

#ifndef __SYSERROR_H
#define __SYSERROR_H

/*
*********************************************************************************************************
*                                           包含头文件
*********************************************************************************************************
*/
#include <stdint.h>
/*
*********************************************************************************************************
*                                           类型和宏定义
*********************************************************************************************************
*/
typedef enum
{
    COM0RxBuf_OV = 0,    /*   端口0接收缓冲区溢出标志   */
    COM1RxBuf_OV,        /*   端口1接收缓冲区溢出标志   */
    COM2RxBuf_OV,        /*   端口2接收缓冲区溢出标志   */
    COM3RxBuf_OV,        /*   端口3接收缓冲区溢出标志   */
    COM4RxBuf_OV,        /*   端口4接收缓冲区溢出标志   */
    Err_FM31256_NOACK,   /*   FM31256无应答   */
    Err_MALLOC,          /*   动态内存分配失败  */
    ERR_FPGA_COMM,       /*  与FPGA板通讯异常错误   */
    ERR_O2REPSEN_COMM,   /*  氧气补偿传感器通讯错误   */
    ERR_O2REPSEN_SIGNAL, /*  氧气补偿传感器 信号检测异常    */
    ERR_ADS_COMM,        /*  ADS1115通讯异常 	*/
    ERR_H2_COMM,         /*氢气传感器通讯异常*/
    ERR_C3H6_COMM,       /*丙烯传感器通讯异常*/
    ERR_OUTTEMPRANGE,    /*光声池温度超范围*/
    ERR_HEATING,         /*预热中*/
    ERR_DZFBUSY,         /*设备忙*/
    ERR_TCTRL_COMM,      /*温控板通讯异常*/
    ERR_LSC_COMM,        /*温湿度通讯或读取异常*/
    ERR_COMM_DS18B20,    /* DS18B20温度异常		*/
    ERR_HUMI_VALUE,      /* 湿度报警 */
    ERR_CO2_ALERT,       /*!< CO2超限报警 */
    ERR_COMM_CAN,        /* CAN控制板通讯		*/
    ERR_MEAS_C2H2,       /*测量异常*/
    ERR_MEAS_CH4,
    ERR_MEAS_CO2,
    ERR_MEAS_O2,
    ERR_MEAS_CO,
    ERR_MEAS_C2H4,
    ERR_MEAS_C2H6,
    ERR_MEAS_H2,
    ERR_MEAS_N2,
    ERR_CELLTEMP, /*!< 气室温度异常 */
                  /*!< [330,339]在SelfCheck中 */
    // ERR_MEAS_TEMP, /*!< 测量温度超限 */
    ERR_END /*   结束标志  */
} ERROR_ENUM_T;

#define ERR_TAB_MAX_NUM 10

/*
*********************************************************************************************************
*                                           输出的函数
*********************************************************************************************************
*/
void ErrorManageInit(void);
int ErrorFlagSet(ERROR_ENUM_T err);
void ErrorFlagClear(ERROR_ENUM_T err);
void ErrorFlagMark(ERROR_ENUM_T err);

int ErrorCodeListGet(uint16_t *pbuf);
int ErrorGetCodeList(uint8_t *pbuf);
int ErrorCodeNumGet(void);

int ErrorCheck(ERROR_ENUM_T err);
uint16_t ErrorCodeGet(ERROR_ENUM_T err);
const char *ErrorNameGet(ERROR_ENUM_T err);

#endif /* __SYSERROR_H */
/***************************** END OF FILE ************************************/
