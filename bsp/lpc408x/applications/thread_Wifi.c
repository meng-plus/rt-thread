/**
 * @file thread_Wifi.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief �������������շ��߳�
 * @version 0.1
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "system_var.h"
#include "thread_Wifi.h"
#include "CommLora.h"
#include "drv_pin.h"
//////////////////////////////
thread_Wifi_t *pthread_Wifi;

uint8_t WIFICloseStep;
uint8_t WIFISetStep;
uint8_t WIFISend_Flag;

uint8_t BLECloseStep;
uint8_t BLESetStep;
uint8_t BLESend_Flag;

uint8_t ReceiveFlag;

static uint32_t NetComm_Timer;
static uint32_t NetSend_Timer;
static uint32_t BlueComm_Timer;
static uint32_t BLESend_Timer;

static uint8_t IPData;
///////////////////////////////
void thread_Wifi_entry(void *param);
int thread_Wifi_init(void);

static rt_err_t finsh_rx_ind(rt_device_t dev, rt_size_t size);
static TR_CHECK_RES_E waiting_response(thread_Wifi_t *pTr); /*!< �ȴ�֡���� */

static void WIFISetLCloseTask(void);
static void WIFISetOpenTask(void);
static void WIFISendTask(void);

static void BLESetLCloseTask(void);
static void BLESetOpenTask(void);
static void BLESendTask(void);

static void Wifi_PutString(char *pdt);

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
void thread_Wifi_entry(void *param)
{

	rt_pin_write(GET_PIN(0, 4), 1);
	rt_pin_mode(GET_PIN(0, 4), PIN_MODE_OUTPUT);
	Wifi_Power_Ctrl(1); // Loraģ�� 5V3��
	while (1)
	{
		WIFISetOpenTask();
		WIFISetLCloseTask();
		WIFISendTask();

		BLESetLCloseTask();
		BLESetOpenTask();
		BLESendTask();
		ReceiveFlag = waiting_response(pthread_Wifi);
		if (ReceiveFlag == TR_CHECK_FRAME)
		{
			// TODO: show msg
			if (pthread_Wifi->obj_ble_wifi)
			{
				lv_obj_t *lable_msg = ui_comp_get_child(pthread_Wifi->obj_ble_wifi, WIFI_LABEL_MSG);
				if (lable_msg)
				{
					lv_label_set_text(lable_msg, (char const *)pthread_Wifi->Wifi_read_buf);
				}
			}
			else
			{
				LOG_D(" wifi msg :\r\n%s\r\n", pthread_Wifi->Wifi_read_buf);
			}
		}
		rt_thread_yield();
		if (pthread_Wifi->delayms)
			rt_thread_mdelay(pthread_Wifi->delayms);
	}
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
int thread_Wifi_init(void)
{
	// init uart0
	pthread_Wifi = rt_calloc(1, sizeof(thread_Wifi_t));
	if (pthread_Wifi == 0)
	{
		LOG_E("pdts_data == NULL,need %d Byte", sizeof(dts_data_t));
		return -1;
	}
	if (pthread_Wifi->device == RT_NULL)
	{
		rt_device_t dev = RT_NULL;
		dev = rt_device_find("uart0");

		/* step2���޸Ĵ������ò��� */
		struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* ��ʼ�����ò��� */
		config.baud_rate = BAUD_RATE_115200;					   // �޸Ĳ�����Ϊ 9600
		config.data_bits = DATA_BITS_8;							   // ����λ 8
		config.stop_bits = STOP_BITS_1;							   // ֹͣλ 1
		config.bufsz = 1024;									   // �޸Ļ����� buff size Ϊ 128
		config.parity = PARITY_NONE;							   // ����żУ��λ

		/* step3�����ƴ����豸��ͨ�����ƽӿڴ�����������֣�����Ʋ��� */
		rt_device_control(dev, RT_DEVICE_CTRL_CONFIG, &config);
		if (rt_device_open(dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
									RT_DEVICE_FLAG_INT_TX) == RT_EOK)
		{
			if (pthread_Wifi->device != RT_NULL)
			{
				/* close old finsh device */
				rt_device_close(pthread_Wifi->device);
				rt_device_set_rx_indicate(pthread_Wifi->device, RT_NULL);
			}
			dev->user_data = pthread_Wifi;
			pthread_Wifi->device = dev;
			rt_device_set_rx_indicate(dev, finsh_rx_ind);
		}
		pthread_Wifi->delayms = 100;
	}

	// creat task
	pthread_Wifi->tid = rt_thread_create("th_Wifi", thread_Wifi_entry, RT_NULL,
										 4096, RT_MAIN_THREAD_PRIORITY, 20);
	RT_ASSERT(pthread_Wifi->tid != RT_NULL);
	if (pthread_Wifi->tid)
	{
		rt_sem_init(&pthread_Wifi->rx_sem, "Wifirx", 0, 0);
		rt_thread_startup(pthread_Wifi->tid);
	}
	return 0;
}
INIT_APP_EXPORT(thread_Wifi_init);

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
static rt_err_t finsh_rx_ind(rt_device_t dev, rt_size_t size)
{
	rt_serial_t *serial = (rt_serial_t *)dev;
	if (dev != pthread_Wifi->device)
	{
		return RT_EOK;
	}
	if (serial->event == RT_SERIAL_EVENT_RX_TIMEOUT)
	{
		if (dev->user_data == pthread_Wifi)
		{
			/* release semaphore to let finsh thread rx data */
			rt_sem_release(&pthread_Wifi->rx_sem);
		}
	}
	return RT_EOK;
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
static TR_CHECK_RES_E waiting_response(thread_Wifi_t *pTr) /*!< �ȴ�֡���� */
{
	uint16_t read_len = 0;
	while (1)
	{
		uint16_t offset = rt_device_read(pTr->device, -1, read_len + pTr->Wifi_read_buf, sizeof(pTr->Wifi_read_buf) - read_len);
		read_len += offset;

		if (sizeof(pTr->Wifi_read_buf) - read_len == 0)
			break;

		if (offset)
			continue;

		if (RT_EOK != rt_sem_take(&pTr->rx_sem, 50)) // RT_WAITING_FOREVER
			break;
	}
	pTr->read_len = read_len;
	if (read_len == 0)
	{
		return TR_CHECK_ERR_LEN;
	}
	else
	{
		pTr->Wifi_read_buf[read_len] = '\0';
		return TR_CHECK_FRAME;
	}
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
static void Wifi_PutString(char *pdt)
{
	thread_Wifi_t *pTr = pthread_Wifi;
	size_t len = strlen(pdt);
	memcpy(pTr->Wifi_send_buf, pdt, len);
	rt_device_write(pTr->device, 0, pTr->Wifi_send_buf, len);
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
static void WIFISetLCloseTask(void)
{
	if (WIFICloseStep == 1)
	{
		WIFISetStep = 0;
		WIFISend_Flag = 0;

		Wifi_PutString("AT+RST\r\n");

		NetComm_Timer = rt_tick_get_millisecond();
		WIFICloseStep = 2;
	}
	else if (WIFICloseStep == 2)
	{
		if ((uint32_t)(rt_tick_get_millisecond() - NetComm_Timer) < 1000)
			return;
		Wifi_PutString("AT+CWMODE=0\r\n");

		WIFICloseStep = 0;
	}
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
static void WIFISetOpenTask(void)
{
	if (WIFISetStep == 1)
	{
		WIFICloseStep = 0;
		Wifi_PutString("AT+CWMODE=2\r\n");

		NetComm_Timer = rt_tick_get_millisecond();
		WIFISetStep = 2;
	}
	else if (WIFISetStep == 2)
	{
		if ((uint32_t)(rt_tick_get_millisecond() - NetComm_Timer) < 1000)
			return;
		Wifi_PutString("AT+CIPMUX=1\r\n");

		NetComm_Timer = rt_tick_get_millisecond();
		WIFISetStep = 3;
	}
	else if (WIFISetStep == 3)
	{
		if ((uint32_t)(rt_tick_get_millisecond() - NetComm_Timer) < 1000)
			return;
		Wifi_PutString("AT+CIPSERVER=1,4001\r\n");

		WIFISetStep = 0;
	}
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
static void WIFISendTask(void)
{
	if (WIFISend_Flag == 1)
	{
		Wifi_PutString("AT+CIPSENDEX=0,140\r\n");
		WIFISend_Flag = 2;
		NetSend_Timer = rt_tick_get_millisecond();
	}

	if (WIFISend_Flag == 2)
	{
		if ((uint32_t)(rt_tick_get_millisecond() - NetSend_Timer) < 1000)
			return;
		Wifi_PutString("Wifi Communication\r\n");
		NetSend_Timer = rt_tick_get_millisecond();
                
                Wifi_PutString("\\0");
                WIFISend_Flag=1;
	}
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
void WIFI_IP_Edit(const uint8_t *pstr)
{
	IPData = atoi((char *)pstr);

	if (IPData < 1)
		IPData = 1;
	else if (IPData > 9)
		IPData = 9;

	if (IPData == 1)
		Wifi_PutString("AT+CIPAP=\"192.168.1.1\",\"192.168.1.1\",\"255.255.255.0\"\r\n");
	else if (IPData == 2)
		Wifi_PutString("AT+CIPAP=\"192.168.2.1\",\"192.168.2.1\",\"255.255.255.0\"\r\n");
	else if (IPData == 3)
		Wifi_PutString("AT+CIPAP=\"192.168.3.1\",\"192.168.3.1\",\"255.255.255.0\"\r\n");
	else if (IPData == 4)
		Wifi_PutString("AT+CIPAP=\"192.168.4.1\",\"192.168.4.1\",\"255.255.255.0\"\r\n");
	else if (IPData == 5)
		Wifi_PutString("AT+CIPAP=\"192.168.5.1\",\"192.168.5.1\",\"255.255.255.0\"\r\n");
	else if (IPData == 6)
		Wifi_PutString("AT+CIPAP=\"192.168.6.1\",\"192.168.6.1\",\"255.255.255.0\"\r\n");
	else if (IPData == 7)
		Wifi_PutString("AT+CIPAP=\"192.168.7.1\",\"192.168.7.1\",\"255.255.255.0\"\r\n");
	else if (IPData == 8)
		Wifi_PutString("AT+CIPAP=\"192.168.8.1\",\"192.168.8.1\",\"255.255.255.0\"\r\n");
	else if (IPData == 9)
		Wifi_PutString("AT+CIPAP=\"192.168.9.1\",\"192.168.9.1\",\"255.255.255.0\"\r\n");
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
void WIFI_Get_IP(void)
{
	Wifi_PutString("AT+CIPAP?\r\n");
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
void WIFI_Get_Name(void)
{
	Wifi_PutString("AT+CWSAP?\r\n");
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
static void BLESetLCloseTask(void)
{
	if (BLECloseStep == 1)
	{
		BLESetStep = 0;
		BLESend_Flag = 0;
		Wifi_PutString("AT+RST\r\n");

		BlueComm_Timer = rt_tick_get_millisecond();
		BLECloseStep = 2;
	}
	else if (BLECloseStep == 2)
	{
		if ((uint32_t)(rt_tick_get_millisecond() - BlueComm_Timer) < 1000)
			return;
		BlueComm_Timer = rt_tick_get_millisecond();
		Wifi_PutString("AT+BLEINIT=0\r\n");

		BLECloseStep = 0;
	}
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
static void BLESetOpenTask(void)
{
	if (BLESetStep == 1)
	{
		BLECloseStep = 0;
		Wifi_PutString("AT+BLEINIT=2\r\n");

		BlueComm_Timer = rt_tick_get_millisecond();
		BLESetStep = 2;
	}
	else if (BLESetStep == 2)
	{
		if ((uint32_t)(rt_tick_get_millisecond() - BlueComm_Timer) < 1000)
			return;
		Wifi_PutString("AT+BLEGATTSSRVCRE\r\n");

		BlueComm_Timer = rt_tick_get_millisecond();
		BLESetStep = 3;
	}
	else if (BLESetStep == 3)
	{
		if ((uint32_t)(rt_tick_get_millisecond() - BlueComm_Timer) < 1000)
			return;
		Wifi_PutString("AT+BLEGATTSSRVSTART\r\n");

		BlueComm_Timer = rt_tick_get_millisecond();
		BLESetStep = 4;
	}
	else if (BLESetStep == 4)
	{
		if ((uint32_t)(rt_tick_get_millisecond() - BlueComm_Timer) < 1000)
			return;
		Wifi_PutString("AT+BLEADVSTART\r\n");

		BLESetStep = 0;
	}
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
static void BLESendTask(void)
{
	if (BLESend_Flag == 1)
	{
		Wifi_PutString("AT+BLEGATTSNTFY=0,1,6,140\r\n");
		BLESend_Flag = 2;
		BLESend_Timer = rt_tick_get_millisecond();
	}

	if (BLESend_Flag == 2)
	{
		if ((uint32_t)(rt_tick_get_millisecond() - BLESend_Timer) < 500)
			return;
		Wifi_PutString("BLUE Communication \r\n");
		BlueComm_Timer = rt_tick_get_millisecond();
	}
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
void BLE_Get_Name(void)
{
	Wifi_PutString("AT+BLENAME?\r\n");
}

/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
void BLE_Get_Addr(void)
{
	Wifi_PutString("AT+BLEADDR?\r\n");
}
