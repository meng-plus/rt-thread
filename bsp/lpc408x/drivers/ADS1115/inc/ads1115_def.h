
#ifndef __ADS1115_DEF_H_
#define __ADS1115_DEF_H_
/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "ads1115_cfg.h"
#ifdef _cplusplus
extern "C"
{
#endif //_cplusplus
/* debug print function. Must be implement by user. */
#ifdef ADS1115_DEBUG_MODE
#ifndef ADS1115_DEBUG
#define ADS1115_DEBUG(...) ads1115_log_debug(__FILE__, __LINE__, __VA_ARGS__)
#endif /* ADS1115_DEBUG */
#else
#define ADS1115_DEBUG(...)
#endif /* ADS1115_DEBUG_MODE */

#ifndef ADS1115_INFO
#define ADS1115_INFO(...) ads1115_log_info(__VA_ARGS__)
#endif

/* assert for developer. */
#ifdef ADS1115_DEBUG_MODE
#define ADS1115_ASSERT(EXPR)                                                 \
    if (!(EXPR))                                                             \
    {                                                                        \
        ADS1115_DEBUG("(%s) has assert failed at %s.", #EXPR, __FUNCTION__); \
        while (1)                                                            \
            ;                                                                \
    }
#else
#define ADS1115_ASSERT(EXPR)
#endif

/* Exported macro ------------------------------------------------------------*/
//*************ADDR Initial********************/
#define ADS1115_ADDRESS_GND 0x90
#define ADS1115_ADDRESS_VDD 0x92
#define ADS1115_ADDRESS_SDA 0x94
#define ADS1115_ADDRESS_SCL 0x96

#define ADS1115_ADDRESS ADS1115_ADDRESS_GND      // ADDR PIN ->GND
#define ADS1115_ADDRESS_W ADS1115_ADDRESS | 0x00 // write address
#define ADS1115_ADDRESS_R ADS1115_ADDRESS | 0x01 // read address

/************POINTER REGISTER*****************/
#define ADS1115_Pointer_ConverReg 0x00   // Convertion register (read-only)
#define ADS1115_Pointer_ConfigReg 0x01   // Config register(read/Write)
#define ADS1115_Pointer_LoThreshReg 0x02 // Lo_thresh register(read/Write)
#define ADS1115_Pointer_HiThreshReg 0x03 // Hi_thresh register(read/Write)

/************CONFIG REGISTER*****************/

// Bit[15]
#define ADS1115_OS_OperationalStatus 0x0000 // No Effect
#define ADS1115_OS_SingleConverStart 0x8000 // Begin a single conversion
// Bits[14:12]
#define ADS1115_MUX_Differ_01 0x0000 // AINp=AIN0, AINn=AIN1(default)
#define ADS1115_MUX_Differ_03 0x1000 // AINp=AIN0, AINn=AIN3
#define ADS1115_MUX_Differ_13 0x2000 // AINp=AIN1, AINn=AIN3
#define ADS1115_MUX_Differ_23 0x3000 // AINp=AIN2, AINn=AIN3
#define ADS1115_MUX_Channel_0 0x4000 // AINp=AIN0, AINn=GND
#define ADS1115_MUX_Channel_1 0x5000 // AINp=AIN1, AINn=GND
#define ADS1115_MUX_Channel_2 0x6000 // AINp=AIN2, AINn=GND
#define ADS1115_MUX_Channel_3 0x7000 // AINp=AIN3, AINn=GND
// Bits[11:9]
#define ADS1115_PGA_6144 0x0000 // FS=6.144V
#define ADS1115_PGA_4096 0x0200 // FS=4.096V
#define ADS1115_PGA_2048 0x0400 // FS=2.048V(default)
#define ADS1115_PGA_1024 0x0600 // FS=1.024V
#define ADS1115_PGA_0512 0x0800 // FS=0.512V
#define ADS1115_PGA_0256 0x0A00 // FS=0.256V
// Bit[8]
#define ADS1115_MODE_ContinuConver 0x0000 // Continuous conversion mode
#define ADS1115_MODE_SingleConver 0x0100  // Power-down single-shot mode(default)
// Bits[7:5]
#define ADS1115_DataRate_8 0x0000   // Data Rate = 8
#define ADS1115_DataRate_16 0x0020  // Data Rate = 16
#define ADS1115_DataRate_32 0x0040  // Data Rate = 32
#define ADS1115_DataRate_64 0x0060  // Data Rate = 64
#define ADS1115_DataRate_128 0x0080 // Data Rate = 128(default)
#define ADS1115_DataRate_250 0x00A0 // Data Rate = 250
#define ADS1115_DataRate_475 0x00C0 // Data Rate = 475
#define ADS1115_DataRate_860 0x00E0 // Data Rate = 860
// Bit[4]
#define ADS1115_COMP_MODE_0 0x0000 // Traditional comparator with hysteresis
#define ADS1115_COMP_MODE_1 0x0010 // Window comparator
// Bit[3]
#define ADS1115_COMP_POL_0 0x0000 // Active low
#define ADS1115_COMP_POL_1 0x0008 // Active high
// Bit[2]
#define ADS1115_COMP_LAT_0 0x0000 // Non-latching comparator
#define ADS1115_COMP_LAT_1 0x0004 // Latching comparator
// Bits[1:0]
#define ADS1115_COMP_QUE_0 0x0000 // Assert after one conversion
#define ADS1115_COMP_QUE_1 0x0001 // Assert after two conversion
#define ADS1115_COMP_QUE_2 0x0002 // Assert after four conversion
#define ADS1115_COMP_QUE_3 0x0003 // Disable Comparator

    /* Exported types ------------------------------------------------------------*/
    /**
     * error code
     */
    typedef enum
    {
        ADS1115_SUCCESS = 0,               /**< success */
        ADS1115_ERR_NOT_FOUND = 1,         /**< not found or not supported */
        ADS1115_ERR_WRITE = 2,             /**< write error */
        ADS1115_ERR_READ = 3,              /**< read error */
        ADS1115_ERR_TIMEOUT = 4,           /**< timeout error */
        ADS1115_ERR_ADDR_OUT_OF_BOUND = 5, /**< address is out of flash bound */
    } ads1115_err;
    /**
     * I2C device
     */
    typedef struct __ads1115_i2c
    {
        /* I2C device name */
        char *name;
        uint8_t addr;
        /* I2C bus write read data function */
        ads1115_err (*read)(const struct __ads1115_i2c *i2c, uint16_t MemAddress, uint8_t *pData, uint16_t Size);
        ads1115_err (*write)(const struct __ads1115_i2c *i2c, uint16_t MemAddress, uint8_t *pData, uint16_t Size);

        /* lock I2C bus */
        void (*lock)(const struct __ads1115_i2c *i2c);
        /* unlock I2C bus */
        void (*unlock)(const struct __ads1115_i2c *i2c);
        /* some user data */
        void *user_data;
    } ads1115_i2c, *ads1115_i2c_t;
    typedef struct __ads1115_dev
    {
        char *name; /*<!device name */
        ads1115_i2c i2c;
        bool init_ok;
        union
        {
            uint16_t val;
            struct __CONFIG
            { /*<!????  */
                uint16_t comp_que : 2;
                uint16_t comp_lat : 1;
                uint16_t comp_pol : 1;
                uint16_t comp_mode : 1;
                uint16_t dr : 3;
                uint16_t mode : 1;
                uint16_t pga : 3;
                uint16_t mux : 3;
                uint16_t os : 1;
            } cfg;
        } config;
    } ads1115_dev;

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
#ifdef _cplusplus
}
#endif
#endif //__ADS1115_DEF_H_
