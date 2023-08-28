/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-18     Meco Man      First version
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <rtconfig.h>
#define LV_USE_SYSMON 1
#define LV_USE_DEV_VERSION
#define LV_USE_PERF_MONITOR         1
#define LV_COLOR_DEPTH              32
#define LV_HOR_RES_MAX          BSP_LCD_WIDTH
#define LV_VER_RES_MAX          BSP_LCD_HEIGHT

#define CONFIG_LV_SPRINTF_USE_FLOAT 1
#define LV_SPRINTF_USE_FLOAT 0

#define USE_WIN32DRV                1
#define WIN32DRV_MONITOR_ZOOM       1

//#define LV_USE_FS_WIN32             1

#ifdef PKG_USING_LV_MUSIC_DEMO
/* music player demo */
#define LV_USE_DEMO_RTT_MUSIC       0
#define LV_DEMO_RTT_MUSIC_AUTO_PLAY 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_16 1
#define LV_COLOR_SCREEN_TRANSP      1
#endif /* PKG_USING_LV_MUSIC_DEMO */

#ifndef LV_USE_PASSWD
#ifdef _LV_KCONFIG_PRESENT
#ifdef CONFIG_LV_USE_PASSWD
#define LV_USE_PASSWD CONFIG_LV_USE_PASSWD
#else
#define LV_USE_PASSWD 0
#endif
#else
#define LV_USE_PASSWD 1
#endif
#endif

#endif





















































