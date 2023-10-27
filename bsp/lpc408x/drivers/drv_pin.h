/**
 * @file drv_pin.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief ͨ�����������
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
#define GET_PIN(PORTx, PIN) (rt_base_t)((32 * PORTx) + PIN)
    typedef struct drv_lpc_pin
    {
        uint8_t port;
        uint8_t pin;
        uint8_t nFunc;
    } drv_pin_cfg_t;
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions prototypes ---------------------------------------------*/
    int rt_hw_pin_init();
#ifdef __cplusplus
}
#endif

#endif //__DRV_PIN_H_
