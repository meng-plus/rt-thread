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

#ifndef __FM31XX_H_
#define __FM31XX_H_

#include <stdint.h>
#include "fm31xx_def.h"
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        uint16_t year;
        uint8_t month;
        uint8_t date;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t week;
    } RTC_TIME;
    int Fm31xxInit(void);

    int WDogStart(uint16_t ms);
    int WDogStop(void);
    int WDogFeed(void);

    int RTCEnable(uint8_t ben);
    int RTCWriteTime(RTC_TIME *tm);
    int RTCReadTime(RTC_TIME *tm);

    int RTCCalMode(uint8_t ben);
    int RTCCalGet(int8_t *cal);
    int RTCCalSet(int8_t cal);

    int E2PromProtect(uint8_t ben);

    /** FM31XX port */
    int E2PromWrite(uint16_t addr, void *p, uint32_t len);
    int E2PromRead(uint16_t addr, void *p, uint32_t len);
#ifdef __cplusplus
}
#endif
#endif
