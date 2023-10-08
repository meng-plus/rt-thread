# STM32F405 红外成像 BSP 说明

## 简介

本文档为 STM32F405 开发板的 BSP (板级支持包) 说明。

主要内容如下：

- 开发板资源介绍
- BSP 快速上手
- 进阶使用方法

通过阅读快速上手章节开发者可以快速地上手该 BSP，将 RT-Thread 运行在开发板上。在进阶使用指南章节，将会介绍更多高级功能，帮助开发者利用 RT-Thread 驱动更多板载资源。

## 开发板介绍


开发板型号: 300049Z1V2

开发板外观如下图所示：

![board]()

该开发板常用 **板载资源** 如下：

- MCU：STM32F405RG，主频 168MHz，1024KB FLASH ，196KB RAM
- 常用外设
  - UART1/2，SPI1/2,I2C1/2,
  - RTC
  - CH4
  - 看门狗
  - ADXL345
  - DS18B20
  - LED
  - out pin
  - key
  - ir_key




## 使用说明

开发环境推荐iar ,仿真器使用jlink,使用最新的 jlink驱动，J-Link RTT Viewer作为命令交互入口
### 快速上手

本 BSP 为开发者提供 MDK4、MDK5 和 IAR 工程，并且支持 GCC 开发环境。下面以 MDK5 开发环境为例，介绍如何将系统运行起来。

#### 硬件连接

使用数据线连接开发板到 PC，打开电源开关。

#### 编译下载

方式一:

双击 project.eww 文件，打开 IAR 工程，编译并下载程序到开发板。

> 工程默认配置使用JLINK仿真器下载程序，在通过JLINK仿真器连接开发板的基础上，点击下载按钮即可下载程序到开发板

方式二:

> 在ENV工具中,使用scons命令进行编译,生成.bin文件,然后使用Jlink仿真器下载到开发板中

#### 运行结果

下载程序成功之后，系统会自动运行。

仿真器连接设备J-Link RTT Viewer打开, 可以看到 RT-Thread 的输出信息:

```bash
 \ | /
- RT -     Thread Operating System
 / | \     4.0.0 build Feb 13 2019
 2006 - 2018 Copyright by rt-thread team
msh >
```
### 进阶使用

如果需更多高级功能，需要利用 ENV 工具对BSP 进行配置，步骤如下：

1. 在 bsp 下打开 env 工具。

2. 输入`menuconfig`命令配置工程，配置好之后保存退出。

3. 输入`pkgs --update`命令更新软件包。

4. 输入`scons --target=mdk4/mdk5/iar` 命令重新生成工程。

本章节更多详细的介绍请参考 [STM32 系列 BSP 外设驱动使用教程](../docs/STM32系列BSP外设驱动使用教程.md)。

## 注意事项

暂无

## 联系人信息

维护人:
