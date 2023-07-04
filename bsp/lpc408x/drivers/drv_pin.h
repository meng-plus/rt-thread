/**
 * @file drv_pin.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 通用输入输出口
 * @version 0.1
 * @date 2023-07-04
 *
 * @copyright Copyright (c) 2023
 *
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRV_PIN_H_
#define __DRV_PIN_H_
/* Private includes ----------------------------------------------------------*/
#include <drivers/pin.h>

#ifdef __cplusplus
extern "C"
{
#endif
/* Exported types ------------------------------------------------------------*/
/**
 * @brief PIN序号只允许调整顺序，不得指定数值
 *
 */
typedef enum __DRV_PIN
{
    LED_0 = 0, /*!< P4.26 */
    LED_1,     /*!< P4.27 */
    LED_2,     /*!< P4.29 */
    LED_3,     /*!< P4.30 */
    LED_END = LED_3,
    RS485_DIR1, /*!< P5.2 UART2 */
    RS485_DIR2, /*!< P0.29 UART3 */
    RS485_DIR3, /*!< P0.7 UART4 */
    RS485_END,
    PIN_NUM, /*!< PIN计数 */
} DRV_PIN_E;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define DEV_PIN_NAME "PIN"
/* Exported functions prototypes ---------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif //__DRV_PIN_H_
