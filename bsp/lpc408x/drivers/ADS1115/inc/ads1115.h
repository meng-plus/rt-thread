#ifndef __ADS1115_H_
#define __ADS1115_H_
/* Includes ------------------------------------------------------------------*/
#include "ads1115_def.h"
#ifdef _cplusplus
extern "C"
{
#endif //_cplusplus

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */
    ads1115_err ads1115_init(void);
    ads1115_dev *ads1115_get_device(size_t index);
    ads1115_err ads1115_device_init(ads1115_dev *pDev);
    ads1115_err ads1115_reg_read(const ads1115_dev *pDev, uint8_t addr, uint8_t *rev_buf, uint32_t len);
    ads1115_err ads1115_reg_write(const ads1115_dev *pDev, uint8_t addr, uint8_t *buf, uint32_t len);

    ads1115_err ads1115_readConver(const ads1115_dev *pDev, uint16_t *Value);
    ads1115_err ads1115_readConverVoltage(const ads1115_dev *pDev, float *voltage);
    ads1115_err ads1115_readConfig(const ads1115_dev *pDev, uint16_t *Value);
    ads1115_err ads1115_writeConfig(const ads1115_dev *pDev, uint16_t config);
    ads1115_err ads1115_readLoThresh(const ads1115_dev *pDev, uint16_t *Value);
    ads1115_err ads1115_writeLoThresh(const ads1115_dev *pDev, uint16_t Value);
    ads1115_err ads1115_readHiThresh(const ads1115_dev *pDev, uint16_t *Value);
    ads1115_err ads1115_writeHiThresh(const ads1115_dev *pDev, uint16_t Value);

    /**
     * @brief  配置启动测量
     * @param  pDev 设备句柄
     * @param  os 启用测量 @ref ADS1115_OS_XXX
     * @param  st true应用配置 false只存到RAM
     * @retval
     */
    void ads1115_setconfigOS(ads1115_dev *pDev, uint16_t os, bool st);
    void ads1115_setconfigMUX(ads1115_dev *pDev, uint16_t Mux, bool st);
    void ads1115_setconfigPGA(ads1115_dev *pDev, uint16_t PGA, bool st);
    void ads1115_setconfigMODE(ads1115_dev *pDev, uint16_t MODE, bool st);
    void ads1115_setconfigDR(ads1115_dev *pDev, uint16_t DR, bool st);
    void ads1115_setconfigCompMode(ads1115_dev *pDev, uint16_t compMode, bool st);
    void ads1115_setconfigCompPol(ads1115_dev *pDev, uint16_t CompPol, bool st);
    void ads1115_setconfigCompLat(ads1115_dev *pDev, uint16_t CompLat, bool st);
    void ads1115_setconfigCompQue(ads1115_dev *pDev, uint16_t que, bool st);

#ifdef _cplusplus
}
#endif

#endif //__ads1115_H_
