# ADXL345������

1. ����

   ```shell
   ·��
   ������ SConscript
   ������ inc
   ��   ������ adxl345.h
   ��   ������ adxl345_def.h
   ������ port
   ��   ������ adxl345_port.c
   ������ readme.md
   ������ sample
   ��   ������ adxl345_shell.c
   ������ src
       ������ adxl345.c
   ```
   
   ���÷�ʽ
   
   ���� menuconfig��������Ӵ����
   
   | ����                    | ˵��                    |
   | ----------------------- | ----------------------- |
   | BSP_USING_ADXL345       | ʹ�ܴ����              |
   | BSP_USING_ADXL345_SHELL | ʹ�������shell�������� |
   |                         |                         |
2. ֧�ֹ���

   ���� ˫�� �˶� ���� �ȹ��� ����������ֲ� 

   ��ǰ�豸IICЭ�� ��ַ 0x1D

3. shell����֧��

   1. adxl345 show 

      չʾ0x30�Ĵ������

   2. adxl345 write  addr  value

      ��ָ��addrд��value

   3. adxl345  read addr

      ��ȡaddr����
