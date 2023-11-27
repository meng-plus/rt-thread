/**
 * @file fm31XX.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "adxl345.h"
#include <rtthread.h>
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"
/*=============================================
==
===============================================*/

/** ADXL345 port */
void adxl345_hw_i2c_init();
uint8_t ADXL345_Read(uint8_t reg, uint8_t *pdt, uint8_t len);
uint8_t ADXL345_WriteReg(uint8_t reg, uint8_t dt);

int ADXL345Init()
{
    uint8_t devid;
    uint8_t res;

    adxl345_hw_i2c_init();

    res = ADXL345ReadReg(0x00, &devid);
    if (res != 0 || devid != 0xe5)
        return -1;

    ADXL345_WriteReg(0x2C, 0x08); // �����趨Ϊ12.5 �ο�pdf13ҳ
    ADXL345_WriteReg(0x2D, 0x08); // ѡ���Դģʽ   �ο�pdf24ҳ	0000 1000 ��������ģʽ
    ADXL345_WriteReg(0x1D, 0x30); // ����THRESH_TAP(�û�����) 62.5mg/LSB   0xff=16g  ��ʱ��ȡ3g
    ADXL345_WriteReg(0x21, 0x11); // ����DUR���û�����ʱ�䣩 625us/LSB  ��ʱ��ȡ10ms
    ADXL345_WriteReg(0x2A, 0x07); // ���ø��᷽���Ƿ�μ��û����  xyzȫ������
    ADXL345_WriteReg(0x2E, 0x00); // �����ж�ǰӦ�Ƚ����ж�
    ADXL345_WriteReg(0x2F, 0x00); // �����ж�����ӳ�� ������ʱ��ӳ�䵽INT1��
    ADXL345_WriteReg(0x31, 0x00); // ʹ�жϵ͵�ƽ��Ч
    ADXL345_WriteReg(0x2E, 0x40); // ʹ�� SINGLE_TAP �ж�
    ADXL345_WriteReg(0x1E, 0x00); // X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
    ADXL345_WriteReg(0x1F, 0x00); // Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
    ADXL345_WriteReg(0x20, 0x05); // Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
    return 0;
}
INIT_DEVICE_EXPORT(ADXL345Init);
/**********************************************************************************
** ��������:
** ��    ��:
** �������:
** �������:
** ��    ��:
***********************************************************************************/
uint8_t ADXL345GetTapState(void)
{
    uint8_t st;
    // TODO:������
    ADXL345ReadReg(0x30, &st);
    return st;
    // if (Chip_GPIO_GetPinState(LPC_GPIO, 2, 25) != 0)
    // {
    //     ADXL345ReadReg(0x30, &st);
    //     return 1;
    // }
    // else
    // {
    //     return 0;
    // }
}
uint8_t ADXL345ReadReg(uint8_t reg, uint8_t *dt)
{
    return ADXL345_Read(reg, dt, 1);
}
