/****************************************Copyright (c)************************************
**                               Zhengzhou GL. TECH Co.,Ltd
**----------------------------------------File Info---------------------------------------
** 文件名称：
** 工程项目：
** 说    明：
**
** 作    者：              日    期：
** 建立版本：
**
**----------------------------------------modification------------------------------------
** 作    者：
** 日    期：
** 版    本：            标    记：
** 说    明：
**
******************************************************************************************/

#ifndef __DRV_KEY_H_
#define __DRV_KEY_H_

#include <stdint.h>
/*=============================================
==
===============================================*/
// 按键键值
typedef enum
{
	KEY_NULL,
	KEY_LEFT,	// 上
	KEY_UP,		// 下
	KEY_RIGHT,	// 左
	KEY_DOWN,	// 右

	KEY_ENTER,	// Enter键
	KEY_ESC,	// Esc键

	KEY_POWER,	// 电源

	KEY_ENTER_UP,	// ENTER + UP 组合键
	KEY_ENTER_DOWN,	// ENTER + DOWN 组合键
	KEY_UP_DOWN,	// UP + DOWN 组合键
	KEY_CALIB,
	KEY_ADDR,
	KEY_FUNC,
	KEY_FUNC_POWER,
	KEY_ADDR_FUNC,
	KEY_ADDR_POWER,
	KEY_FUNC_UP,

	KEY_POWER_CALIB,
	KEY_POWER_UP,
	KEY_POWER_ENTER,
	KEY_POWER_DOWN,
	KEY_POWER_ESC,

	KEY_ADDR_UP,
	KEY_ADDR_ENTER,
	KEY_ADDR_DOWN,
//	KEY_ADDR_ESC,

	KEY_FUNC_ENTER,
	KEY_FUNC_DOWN,
	KEY_FUNC_ESC,

	KEY_CALIB_UP,
	KEY_CALIB_ADDR,
	KEY_CALIB_FUNC,
	KEY_CALIB_ENTER,
	KEY_CALIB_DOWN,
	KEY_CALIB_ESC,

	KEY_ENTER_ESC,

	KEY_DOWN_ESC,
	KEY_UP_ESC,

	KEY_SEN_COMMON,		// 通用
	KEY_SEN_O2,			// O2
	KEY_SEN_CO,			// CO
	KEY_SEN_CO2,		// CO2
	KEY_SEN_TEMP_HUM,	// 温湿度
	KEY_SEN_MULT,		// 多参数
	KEY_SEN_CH4,		// CH4
	KEY_SEN_PRESS,		// 压力
	KEY_SEN_DUST,		// 粉尘
	KEY_SEN_FLOW,		// 风速
}TypeKeyValue;

// 按键事件;
typedef enum
{
	KEY_EVENT_NONE,
	KEY_EVENT_NEW,
}TypeKeyEvent;

// 按键状态
typedef enum
{
	KEY_STATUS_UP,
	KEY_STATUS_DOWN,
}TypeKeyStatus;




// 按键状态
typedef struct
{
	TypeKeyValue	value;		// 按键
	TypeKeyEvent	event;		// 事件
	TypeKeyStatus	status;		// 状态
	uint16_t			time;		// 按下按键的时间(单位ms)
}TypeKey;
/*=============================================
==
===============================================*/
void KeyInit(void);
void KeyRead(TypeKey *key);
void KeyClearEvent(void);
void KeyClearTime(void);
uint16_t KeyGetFreeTime(void);

#endif
