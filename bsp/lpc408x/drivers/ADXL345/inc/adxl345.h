/**
 * @file fm31xx.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief FM31256
 * @version 0.1
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __ADXL345_H_
#define __ADXL345_H_

#include <stdint.h>
#include "adxl345_def.h"
#ifdef __cplusplus
extern "C"
{
#endif
    int ADXL345Init(void);
    uint8_t ADXL345GetTapState(void);
    uint8_t ADXL345ReadReg(uint8_t reg, uint8_t *dt);
#ifdef __cplusplus
}
#endif
#endif
