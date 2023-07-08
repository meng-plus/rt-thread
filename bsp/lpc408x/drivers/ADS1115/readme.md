# ADXL345驱动包

1. 介绍

   ```shell
   路径
   ├── SConscript
   ├── inc
   │   ├── adxl345.h
   │   └── adxl345_def.h
   ├── port
   │   └── adxl345_port.c
   ├── readme.md
   ├── sample
   │   └── adxl345_shell.c
   └── src
       └── adxl345.c
   ```
   
   配置方式
   
   请在 menuconfig中配置添加此组件
   
   | 配置                    | 说明                    |
   | ----------------------- | ----------------------- |
   | BSP_USING_ADXL345       | 使能此组件              |
   | BSP_USING_ADXL345_SHELL | 使能组件的shell调试命令 |
   |                         |                         |
2. 支持功能

   单击 双击 运动 跌落 等功能 详情见数据手册 

   当前设备IIC协议 地址 0x1D

3. shell命令支持

   1. adxl345 show 

      展示0x30寄存器结果

   2. adxl345 write  addr  value

      向指定addr写入value

   3. adxl345  read addr

      读取addr数据
