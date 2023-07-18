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
#include "LPC407x_8x_177x_8x.h"
#include "drv_keyIrd.h"



/*=============================================
==
===============================================*/
#define KEY_BASE_TIME	(400 / 2)		//0脉冲为400us
#define numof(array) (sizeof(array)/sizeof(array[0]))
/*=============================================
==
===============================================*/
static TypeKey keyValue =
{
	KEY_NULL,
	KEY_EVENT_NONE,
	KEY_STATUS_UP,
	0,
};

static uint8_t cntKeyCode = 0;
static int32_t cntKeyDown = 0;

static const uint8_t		keyCodeTab[] = {
	/* 0x00	*/
	KEY_SEN_COMMON,		KEY_SEN_O2,		KEY_SEN_CO,		KEY_SEN_CO2,
	KEY_SEN_TEMP_HUM,	KEY_SEN_MULT,	KEY_SEN_CH4,	KEY_SEN_PRESS,
	KEY_SEN_DUST,		KEY_UP,			KEY_LEFT,		0xff,
	KEY_FUNC,			0xff,			0xff,			0xff,
	/* 0x10	*/
	0xff,				KEY_DOWN,		KEY_RIGHT,		KEY_SEN_FLOW,
	0xff,				0xff,			0xff,			0xff,
	0xff,				KEY_UP_DOWN,	0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/* 0x20	*/
	KEY_CALIB_UP,		KEY_ENTER,		KEY_CALIB_ADDR,	KEY_CALIB_FUNC,
	KEY_CALIB_ENTER,	KEY_CALIB_DOWN,	KEY_CALIB_ESC,	KEY_ADDR_UP,
	KEY_ADDR_FUNC,		KEY_ADDR_ENTER,	0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/*0x30	*/
	KEY_ADDR_DOWN,		KEY_ENTER_DOWN,	0xff,			KEY_FUNC_UP,
	KEY_FUNC_ENTER,		KEY_FUNC_DOWN,	KEY_FUNC_ESC,	0xff,
	0xff,				KEY_ENTER_UP,	0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/*0x40	*/
	0xff,				KEY_ESC,		KEY_CALIB,		0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				KEY_UP_ESC,		0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/*0x50	*/
	0xff,				KEY_DOWN_ESC,	0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/*0x60	*/
	0xff,				KEY_ENTER_ESC,	0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/*0x70	*/
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/*0x80	*/
	KEY_POWER_CALIB,	KEY_POWER,		KEY_ADDR,		KEY_ADDR_POWER,
	0xff,				0xff,			0xff,			0xff,
	0xff,				KEY_POWER_UP,	0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/*0x90	*/
	KEY_FUNC_POWER,		KEY_POWER_DOWN,	0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/*0xA0	*/
	0xff,				KEY_POWER_ENTER,0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/*0xB0	*/
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	/*0xC0	*/
	0xff,				KEY_POWER_ESC,	0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
	0xff,				0xff,			0xff,			0xff,
};

/*=============================================
==
===============================================*/
TypeKeyValue KeyDecode(uint32_t keycode);

/**********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
***********************************************************************************/
void KeyInit(void)
{
	//===============
	//== GPIO
	//================
	LPC_SC->PCONP |= (1 << 15);
	LPC_IOCON->P0_23 = 	0x83;	// Digital mode/T3_CAP0

	//===============
	//== Timer3
	//================
	LPC_SC->PCONP |= (1 << 23);

	//
	LPC_TIM3->TCR = (1<<1);       /* counting  disable, set reset */
	LPC_TIM3->TCR = 0;            /* release reset */
	LPC_TIM3->CTCR = 0;
	LPC_TIM3->IR = 0x3f;
	LPC_TIM3->TC = 0;
	LPC_TIM3->PR = PeripheralClock / 1000000ul - 1;		//1000000Hz
	LPC_TIM3->MR0 = 0xffffffff;				//100Hz
	LPC_TIM3->MCR = 0;
	LPC_TIM3->CCR = (1 << 0) | 	// CAP0RE rising edge
		  			//(0 << 1) | 	// CAP0FE falling edge
		  			(1 << 1) |    //hwx 2019.10.30 change.
		  			(1 << 2);	// CAP0I
	LPC_TIM3->TCR = (1 << 0);	//Counter Enable

	NVIC_EnableIRQ(TIMER3_IRQn);
	NVIC_SetPriority(TIMER3_IRQn, (3 << 2) | (0 << 0));
}



/**********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
***********************************************************************************/
void KeyRead(TypeKey *key)
{
 	key->value  = keyValue.value;
	key->event  = keyValue.event;
	key->status = keyValue.status;
	key->time   = keyValue.time;
}

/**********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
***********************************************************************************/
void KeyClearEvent(void)
{
	keyValue.event = KEY_EVENT_NONE;
}

/**********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
***********************************************************************************/
void KeyClearTime(void)
{
  	keyValue.event 	= KEY_EVENT_NONE;
	keyValue.time	= 0;
	cntKeyDown 		= -1000000000;
}

/**********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
***********************************************************************************/
void TIMER3_IRQHandler(void)
{
	TypeKeyValue key;
	static uint32_t keycode = 0;
	static uint8_t flgFreq = 0;
	static uint32_t cntUp = 0;

	//MR0
	if ((LPC_TIM3->IR & (1 << 0)) != 0)
	{
	  	LPC_TIM3->IR = (1 << 0);

		cntKeyCode = 0;
		keyValue.status = KEY_STATUS_UP;
		LPC_TIM3->MCR &= (~(1 << 0));		//LPC_TIMER3->MR0 match interrupt disable
		LPC_TIM3->CCR = (1 << 0) | (0 << 1) | (1 << 2);	// CAP0I
	}

	//MR1
	if ((LPC_TIM3->IR & (1 << 1)) != 0)
	{
	  	LPC_TIM3->IR = (1 << 1);

		if (flgFreq == 0)
		{
			cntKeyDown = 0;
			keyValue.status = KEY_STATUS_UP;
			LPC_TIM3->MCR &= (~(1 << 3));		//LPC_TIMER3->MR0 match interrupt disable
			LPC_TIM3->CCR = (1 << 0) | (0 << 1) | (1 << 2);	// CAP0I
		}
		else							//继续下一个200ms检测
		{
			flgFreq = 0;
			LPC_TIM3->MR1 = LPC_TIM3->TC + 100000;

			cntKeyDown		+= 100;
			keyValue.time 	+= 100;

			if (cntKeyDown == 1500)
			{
				keyValue.event = KEY_EVENT_NEW;
			}
			else if (cntKeyDown == 2000)
			{
				keyValue.event = KEY_EVENT_NEW;
			}
			else if (cntKeyDown == 2300)
			{
				cntKeyDown = 2000;

				keyValue.event = KEY_EVENT_NEW;
			}
		}
	}


	//CR0
	if ((LPC_TIM3->IR & (1 << 4)) != 0)
	{
		LPC_TIM3->IR = (1 << 4);

		cntKeyCode++;
		if ((cntKeyCode & 0x01) != 0)
		{
			cntUp = LPC_TIM3->CR0;
			LPC_TIM3->CCR = (0 << 0) | (1 << 1) | (1 << 2);	// CAP0I

			if (cntKeyCode == 1)
			{
				LPC_TIM3->MR0 = LPC_TIM3->TC + 100000;
				LPC_TIM3->MCR |= (1 << 0);			//LPC_TIMER3->MR0 match interrupt enable
			}
		}
		else
		{
			uint32_t	m;

			m = LPC_TIM3->CR0 - cntUp;
			LPC_TIM3->CCR = (1 << 0) | (0 << 1) | (1 << 2);	// CAP0I

			if (m > 1 * KEY_BASE_TIME && m < 3 * KEY_BASE_TIME)
			{
				flgFreq = 1;
				keycode = (keycode >> 1);
			}
			else if (m > 5 * KEY_BASE_TIME && m < 7 * KEY_BASE_TIME)
			{
				flgFreq = 1;
				keycode = (keycode >> 1) | 0x800000;
			}
			else
			{
				cntKeyCode = 0;

				return;
			}

			if (cntKeyCode >= 48)
			{
				cntKeyCode = 0;

				key = KeyDecode(keycode);
				if (key != KEY_NULL)
				{
					keyValue.value 	= key;
					LPC_TIM3->MCR &= (~(1 << 0));		//LPC_TIMER3->MR0 match interrupt disable

					if (keyValue.status == KEY_STATUS_UP)
					{
						keyValue.status = KEY_STATUS_DOWN;
						keyValue.event 	= KEY_EVENT_NEW;
						keyValue.time 	= 0;
						cntKeyDown 		= 0;

						LPC_TIM3->MR1 = LPC_TIM3->TC + 100000;	//100ms内无脉冲则说明无按键按下
						LPC_TIM3->MCR |= (1 << 3);		//LPC_TIMER3->MR1 match interrupt disable
					}
				}
			}
		}
	}
}

/**********************************************************************************
** 函数名称:
** 功    能:
** 输入参数:
** 输出参数:
** 返    回:
***********************************************************************************/
TypeKeyValue KeyDecode(uint32_t keycode)
{
	if ((keycode & 0xfff) != ((keycode >> 12) & 0xfff))
		return KEY_NULL;

	keycode &= 0xfff;

	if ((keycode & 0x07) != 0x07)
		return KEY_NULL;

	keycode >>= 3;
	if (keycode >= numof(keyCodeTab))
		return KEY_NULL;

	if (keyCodeTab[keycode] == 0xff)
		return KEY_NULL;

	return (TypeKeyValue)keyCodeTab[keycode];
}


