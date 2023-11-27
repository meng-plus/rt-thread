/****************************************Copyright (c)************************************
**                               Zhengzhou GL. TECH Co.,Ltd
**----------------------------------------File Info---------------------------------------
** �ļ����ƣ�
** ������Ŀ��
** ˵    ����
**
** ��    �ߣ�              ��    �ڣ�
** �����汾��
**
**----------------------------------------modification------------------------------------
** ��    �ߣ�
** ��    �ڣ�
** ��    ����            ��    �ǣ�
** ˵    ����
**
******************************************************************************************/

#ifndef __DRV_KEY_H_
#define __DRV_KEY_H_

#include <stdint.h>
/*=============================================
==
===============================================*/
// ������ֵ
typedef enum
{
	KEY_NULL,
	KEY_LEFT,	// ��
	KEY_UP,		// ��
	KEY_RIGHT,	// ��
	KEY_DOWN,	// ��

	KEY_ENTER,	// Enter��
	KEY_ESC,	// Esc��

	KEY_POWER,	// ��Դ

	KEY_ENTER_UP,	// ENTER + UP ��ϼ�
	KEY_ENTER_DOWN,	// ENTER + DOWN ��ϼ�
	KEY_UP_DOWN,	// UP + DOWN ��ϼ�
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

	KEY_SEN_COMMON,		// ͨ��
	KEY_SEN_O2,			// O2
	KEY_SEN_CO,			// CO
	KEY_SEN_CO2,		// CO2
	KEY_SEN_TEMP_HUM,	// ��ʪ��
	KEY_SEN_MULT,		// �����
	KEY_SEN_CH4,		// CH4
	KEY_SEN_PRESS,		// ѹ��
	KEY_SEN_DUST,		// �۳�
	KEY_SEN_FLOW,		// ����
}TypeKeyValue;

// �����¼�;
typedef enum
{
	KEY_EVENT_NONE,
	KEY_EVENT_NEW,
}TypeKeyEvent;

// ����״̬
typedef enum
{
	KEY_STATUS_UP,
	KEY_STATUS_DOWN,
}TypeKeyStatus;




// ����״̬
typedef struct
{
	TypeKeyValue	value;		// ����
	TypeKeyEvent	event;		// �¼�
	TypeKeyStatus	status;		// ״̬
	uint16_t			time;		// ���°�����ʱ��(��λms)
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
