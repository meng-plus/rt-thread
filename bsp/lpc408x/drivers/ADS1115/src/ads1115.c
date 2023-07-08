
/* Includes ------------------------------------------------------------------*/
#include "ads1115.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define numof(table) (sizeof(table) / sizeof(table[0]))
/* Private variables ---------------------------------------------------------*/
static ads1115_dev dev_table[] = ADS1115_DEV_TABLE;

/* Private function prototypes -----------------------------------------------*/
extern ads1115_err ads1115_i2c_port_init(ads1115_i2c_t ads1115);
ads1115_err ads1115_device_init(ads1115_dev *pDev);
static ads1115_err hardware_init(ads1115_dev *pDev);
static ads1115_err software_init(ads1115_dev *pDev);

/* ../port/ads1115_port.c */
extern void ads1115_log_debug(const char *file, const long line, const char *format, ...);
extern void ads1115_log_info(const char *format, ...);

/* Exported functions --------------------------------------------------------*/
ads1115_err ads1115_init()
{
    ads1115_err cur_result = ADS1115_SUCCESS, all_result = ADS1115_SUCCESS;
    for (int i = 0; i < numof(dev_table); i++)
    {
        if (dev_table[i].init_ok == 0)
        {
            /* initialize flash device index of flash device information table */
            cur_result = ads1115_device_init(&dev_table[i]);
        }

        if (cur_result != ADS1115_SUCCESS)
        {
            all_result = cur_result;
        }
    }
    return all_result;
}
ads1115_dev *ads1115_get_device(size_t index)
{
    if (index < numof(dev_table))
    {
        return &dev_table[index];
    }
    else
    {
        return NULL;
    }
}
ads1115_err ads1115_device_init(ads1115_dev *pDev)
{
    ads1115_err result = ADS1115_SUCCESS;

    /* hardware initialize */
    result = hardware_init(pDev);
    if (result == ADS1115_SUCCESS)
    {
        result = software_init(pDev);
    }
    if (result == ADS1115_SUCCESS)
    {
        pDev->init_ok = true;
        ADS1115_INFO("%s  device is initialize success.", pDev->name);
    }
    else
    {
        pDev->init_ok = false;
        ADS1115_INFO("Error: %s device is initialize fail.", pDev->name);
    }

    return result;
}
/**
 * hardware initialize
 */
static ads1115_err hardware_init(ads1115_dev *pDev)
{
    extern ads1115_err ads1115_i2c_port_init(ads1115_i2c_t ads1115_i2c);
    ads1115_err result = ADS1115_SUCCESS;
    ADS1115_ASSERT(pDev != NULL);

    result = ads1115_i2c_port_init(&pDev->i2c);
    return result;
}
/**
 * software initialize
 */
static ads1115_err software_init(ads1115_dev *pDev)
{
    uint8_t cmd_data[2];
    ads1115_err result = ADS1115_SUCCESS;
    result = ads1115_reg_read(pDev, ADS1115_Pointer_ConfigReg, cmd_data, sizeof(cmd_data));
    if (result == ADS1115_SUCCESS)
    {
        ADS1115_DEBUG("ADS1115 cfg: %02x%02xh", cmd_data[0], cmd_data[1]);
    }
    return result;
}
ads1115_err ads1115_reg_read(const ads1115_dev *pDev, uint8_t addr, uint8_t *rev_buf, uint32_t len)
{
    ADS1115_ASSERT(pDev);
    ads1115_err result = ADS1115_SUCCESS;
    if (pDev->i2c.lock != NULL)
    {
        pDev->i2c.lock(&pDev->i2c);
    }
    result = pDev->i2c.read(&pDev->i2c, addr, rev_buf, len);
    if (pDev->i2c.unlock != NULL)
    {
        pDev->i2c.unlock(&pDev->i2c);
    }
    return result;
}
ads1115_err ads1115_reg_write(const ads1115_dev *pDev, uint8_t addr, uint8_t *buf, uint32_t len)
{
    ADS1115_ASSERT(pDev != NULL && pDev->init_ok == 1);
    ads1115_err result = ADS1115_SUCCESS;
    if (pDev->i2c.lock != NULL)
    {
        pDev->i2c.lock(&pDev->i2c);
    }
    result = pDev->i2c.write(&pDev->i2c, addr, buf, len);
    if (pDev->i2c.unlock != NULL)
    {
        pDev->i2c.unlock(&pDev->i2c);
    }
    return result;
}
ads1115_err ads1115_readConver(const ads1115_dev *pDev, uint16_t *Value)
{
    uint8_t buff[2];
    ads1115_err err;
    err = ads1115_reg_read(pDev, ADS1115_Pointer_ConverReg, buff, sizeof(buff));
    if (Value != NULL)
    {

        *Value = buff[0] << 8 | buff[1];
    }
    return err;
}
ads1115_err ads1115_readConverVoltage(const ads1115_dev *pDev, float *voltage)
{
    ads1115_err err;
    uint16_t rawAdc;
    err = ads1115_readConver(pDev, &rawAdc);
    if (err == ADS1115_SUCCESS && voltage != NULL)
    {
        const float PGA2VoltageTab[] = {6.144, 4.096, 2.048, 1.24, 0.512, 0.256, 0.256, 0.256};

        float pga = PGA2VoltageTab[pDev->config.cfg.pga % sizeof(PGA2VoltageTab)];

        *voltage = rawAdc / 32767.0 * pga;
    }
    return err;
}
ads1115_err ads1115_readConfig(const ads1115_dev *pDev, uint16_t *Value)
{
    uint8_t buff[2];
    ads1115_err err;
    err = ads1115_reg_read(pDev, ADS1115_Pointer_ConfigReg, buff, sizeof(buff));
    if (Value != NULL)
    {
        *Value = buff[0] << 8 | buff[1];
    }
    return err;
}
ads1115_err ads1115_writeConfig(const ads1115_dev *pDev, uint16_t config)
{
    uint8_t buff[2];
    ads1115_err err;
    buff[0] = config >> 8;
    buff[1] = config & 0xFF;
    err = ads1115_reg_write(pDev, ADS1115_Pointer_ConfigReg, buff, sizeof(buff));
    return err;
}
ads1115_err ads1115_readLoThresh(const ads1115_dev *pDev, uint16_t *Value)
{
    uint8_t buff[2];
    ads1115_err err;
    err = ads1115_reg_read(pDev, ADS1115_Pointer_LoThreshReg, buff, sizeof(buff));
    if (Value != NULL)
    {
        *Value = buff[0] << 8 | buff[1];
    }
    return err;
}
ads1115_err ads1115_writeLoThresh(const ads1115_dev *pDev, uint16_t Value)
{
    uint8_t buff[2];
    ads1115_err err;
    buff[0] = Value >> 8;
    buff[1] = Value & 0xFF;
    err = ads1115_reg_write(pDev, ADS1115_Pointer_LoThreshReg, buff, sizeof(buff));
    return err;
}
ads1115_err ads1115_readHiThresh(const ads1115_dev *pDev, uint16_t *Value)
{
    uint8_t buff[2];
    ads1115_err err;
    err = ads1115_reg_read(pDev, ADS1115_Pointer_HiThreshReg, buff, sizeof(buff));
    if (Value != NULL)
    {
        *Value = buff[0] << 8 | buff[1];
    }
    return err;
}
ads1115_err ads1115_writeHiThresh(const ads1115_dev *pDev, uint16_t Value)
{
    uint8_t buff[2];
    ads1115_err err;
    buff[0] = Value >> 8;
    buff[1] = Value & 0xFF;
    err = ads1115_reg_write(pDev, ADS1115_Pointer_HiThreshReg, buff, sizeof(buff));
    return err;
}

/**
 * @brief  配置启动测量
 * @param  pDev 设备句柄
 * @param  os 启用测量 @ref ADS1115_OS_XXX
 * @param  st true应用配置 false只存到RAM
 * @retval
 */
void ads1115_setconfigOS(ads1115_dev *pDev, uint16_t os, bool st)
{
    ADS1115_ASSERT(pDev);
    ADS1115_ASSERT((os & ~(0x01 << 15)) == 0x00);
    pDev->config.cfg.os = os >> 15;
    if (st)
    {
        ads1115_writeConfig(pDev, pDev->config.val);
    }
}
void ads1115_setconfigMUX(ads1115_dev *pDev, uint16_t Mux, bool st)
{
    ADS1115_ASSERT(pDev);
    ADS1115_ASSERT((Mux & ~(0x07 << 12)) == 0x00);
    pDev->config.cfg.mux = (Mux >> 12) & 0x07;
    if (st)
    {
        ads1115_writeConfig(pDev, pDev->config.val);
    }
}
void ads1115_setconfigPGA(ads1115_dev *pDev, uint16_t PGA, bool st)
{
    ADS1115_ASSERT(pDev);
    ADS1115_ASSERT((PGA & ~(0x07 << 9)) == 0x00);
    pDev->config.cfg.pga = (PGA >> 9) & 0x07;
    if (st)
    {
        ads1115_writeConfig(pDev, pDev->config.val);
    }
}
void ads1115_setconfigMODE(ads1115_dev *pDev, uint16_t MODE, bool st)
{
    ADS1115_ASSERT(pDev);
    ADS1115_ASSERT((MODE & ~(0x01 << 8)) == 0x00);
    pDev->config.cfg.mode = (MODE >> 8) & 0x01;
    if (st)
    {
        ads1115_writeConfig(pDev, pDev->config.val);
    }
}
void ads1115_setconfigDR(ads1115_dev *pDev, uint16_t DR, bool st)
{
    ADS1115_ASSERT(pDev);
    ADS1115_ASSERT((DR & ~(0x07 << 5)) == 0x00);
    pDev->config.cfg.dr = (DR >> 5) & 0x07;
    if (st)
    {
        ads1115_writeConfig(pDev, pDev->config.val);
    }
}
void ads1115_setconfigCompMode(ads1115_dev *pDev, uint16_t compMode, bool st)
{
    ADS1115_ASSERT(pDev);
    ADS1115_ASSERT((compMode & ~(0x01 << 4)) == 0x00);
    pDev->config.cfg.comp_mode = (compMode >> 4) & 0x01;
    if (st)
    {
        ads1115_writeConfig(pDev, pDev->config.val);
    }
}
void ads1115_setconfigCompPol(ads1115_dev *pDev, uint16_t CompPol, bool st)
{
    ADS1115_ASSERT(pDev);
    ADS1115_ASSERT((CompPol & ~(0x01 << 3)) == 0x00);
    pDev->config.cfg.comp_pol = (CompPol >> 3) & 0x01;
    if (st)
    {
        ads1115_writeConfig(pDev, pDev->config.val);
    }
}
void ads1115_setconfigCompLat(ads1115_dev *pDev, uint16_t CompLat, bool st)
{
    ADS1115_ASSERT(pDev);
    ADS1115_ASSERT((CompLat & ~(0x01 << 2)) == 0x00);
    pDev->config.cfg.comp_lat = (CompLat >> 2) & 0x01;
    if (st)
    {
        ads1115_writeConfig(pDev, pDev->config.val);
    }
}
void ads1115_setconfigCompQue(ads1115_dev *pDev, uint16_t que, bool st)
{
    ADS1115_ASSERT(pDev);
    ADS1115_ASSERT((que & ~(0x03 << 0)) == 0x00);
    pDev->config.cfg.comp_que = (que >> 0) & 0x03;
    if (st)
    {
        ads1115_writeConfig(pDev, pDev->config.val);
    }
}
