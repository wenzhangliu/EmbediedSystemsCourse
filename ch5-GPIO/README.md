# 第5章 GPIO接口

```
作者：柳文章
日期：2025.3.16
```

目录索引:

- [GPIO结构及特点](#gpio结构及特点)
  - [GPIO功能模式](#gpio功能模式)
  - [GPIO特点](#gpio特点)
- [GPIO寄存器和库函数](#gpio寄存器和库函数)
  - [GPIO寄存器](#gpio寄存器)
  - [GPIO库函数](#gpio库函数)
    - [1. GPIO_Init函数](#1-gpio_init函数)
    - [2. GPIO_SetBits函数](#2-gpio_setbits函数)
    - [3. GPIO_ResetBits函数](#3-gpio_resetbits函数)
    - [4. GPIO_ReadInputDataBit](#4-gpio_readinputdatabit函数)
- [案例：按键控制小灯](#案例按键控制小灯)
    - [新建工程](#新建工程)
    - [查看电路原理图](#查看电路原理图)
    - [编写main.c](#编写mainc)
      - [导入库函数](#1-导入库函数)
      - [初始化程序](#2-初始化程序)
      - [延时程序](#3-延时程序)
      - [主循环](#4-主循环)
- [完整代码](#完整代码)

## GPIO结构及特点

GPIO输出模式：

| 输出模式     | 输出信号来源 | 推挽或开漏 |
|----------|--------|-------|
| 通用开漏输出   | 输出高低电平 | 开漏    |
| 通用推挽输出   |        | 推挽    |
| 复用开漏输出   | 片上外设   | 开漏    |
| 复用推挽输出   |        | 推挽    |

GPIO输入模式：

| 输入模式 | 输入信号去向       | 上拉或下拉 |
|---------|--------------|-------|
| 模拟输入 | 片上模拟外设ADC    | 无     |
| 浮空输入 | 输入数据寄存器或片上外设 | 无     |
| 下拉输入 | 输入数据寄存器或片上外设 | 下拉    |
| 上拉输入 | 输入数据寄存器或片上外设 | 上拉    |

### GPIO功能模式

- GPIO复用功能：除I/O外，还可用于串口、定时器、I2C等。
- GPIO数字输入模式：将不规则脉冲信号整合为矩阵脉冲信号。
- 模拟输入模式：接收模拟量。
- 推挽输出模式：通用推挽输出、复用推挽输出。
- 开漏输出模式：GPIO输出1时引脚悬空，输出0时，引脚接地。
- GPIO输出速度设定：2MHz, 10MHz, 50MHz。
- 钳位功能：防止外部输入的电压过高或过低。

### GPIO特点

- I/O口电平兼容：除了模拟量最大承受电压为3.6V，其余多数I/O口兼容5V输入，且所有I/O口兼容CMOS和TTL。
- I/O口驱动能力：最大可吸收25mA电流。
- 内部上拉/下拉设置。
- I/O口可配置为外部中断口。
- 具有独立的唤醒I/O口。
- 具备锁存功能。
- 具有侵入检测引脚。

## GPIO寄存器和库函数

### GPIO寄存器

GPIO寄存器功能表：

| 寄存器                    | 功能                                                  |
|------------------------|-----------------------------------------------------|
| 端口配置低寄存器（GPIO_CRL）     | 配置端口低8位IO口Px0-Px7                                   |
| 端口配置高寄存器（GPIO_CRH）     | 配置端口高8位IO口Px8-Px15                                  |
| 端口输入数据寄存器（GPIO_IDR）    | 获取16个IO口上的输入数据                                      |
| 端口输出数据寄存器（GPIO_ODR）    | 控制16个IO口输出数据                                        |
| 端口位设置/清除寄存器（GPIO_BSRR） | 高16位，用于清除Px0-Px15对应的位为0；<br>低16位，用于设置Px0-Px15对应的位为1 |
| 端口位清除寄存器（GPIO_BRR）     | 低16位，用于清除Px0-Px15对应的位为0                             |
| 端口配置锁定寄存器（GPIO_LCKR）   | 第16位，配置端口锁键；<br>15-0位，配置Px0-Px15对应的锁键               |

### GPIO库函数

GPIO库函数列表：

| 函数名                    | 描述                                   |
|------------------------|--------------------------------------|
| GPIO_DeInit            | 将外设GPIOx寄存器重设为缺省值                    |
| GPIO_AFIODeInit        | 将复用功能重设为缺省值                          |
| GPIO_Init              | 根据GPIO_InitStruct中指定的参数初始化外设GPIOx寄存器 |
| GPIO_StructInit        | 把GPIO——InitStruct中的每一个参数按缺省值填入       |
| GPIO_ReadInputDataBit  | 读取指定端口管脚的输入                          |
| GPIO_ReadInputData     | 读取指定的GPIO端口输入                        |
| GPIO_ReadOutputDataBit | 读取指定端口管脚的输出                          |
| GPIO_ReadOutputData    | 读取指定的GPIO端口输出                        |
| GPIO_SetBits           | 设置指定的数据端口位                           |
| GPIO_ResetBits         | 清除指定的数据端口位                           |
| GPIO_WriteBit          | 设置或者清除指定的数据端口位                       |
| GPIO_Write             | 向指定GPIO数据端口写入数据                      |
| GPIO_PinLockConfig     | 锁定GPIO管脚设置寄存器                        |
| GPIO_EventOutputConfig | 选择GPIO管脚用作事件输出                       |
| GPIO_EventOutputCmd    | 使能或者失能事件输出                           |
| GPIO_PinRemapConfig    | 改变指定引脚的映射                            |
| GPIO_EXTILineConfig    | 选择GPIO管脚用作外部中断线路                     |

#### 1. GPIO_Init函数

```c
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct){}
```

参数说明：

- ``GPIOx``: x可以是A、B、C、D或E，用于选择GPIO；
- ``GPIO_InitStruc``: 指向结构体``GPIO_TypeDef``的指针，包含了外设GPIO的配置信息。
- ``GPIO_TypeDef``结构体描述：
  ```c
    typedef struct
    {
      uint16_t GPIO_Pin;             /*!< Specifies the GPIO pins to be configured.
                                          This parameter can be any value of @ref GPIO_pins_define */
    
      GPIOSpeed_TypeDef GPIO_Speed;  /*!< Specifies the speed for the selected pins.
                                          This parameter can be a value of @ref GPIOSpeed_TypeDef */
    
      GPIOMode_TypeDef GPIO_Mode;    /*!< Specifies the operating mode for the selected pins.
                                          This parameter can be a value of @ref GPIOMode_TypeDef */
    }GPIO_InitTypeDef;
  ```
  - ``GPIO_Pin``：选择待设置的GPIO管脚，使用操作符‘|’可以一次设置多个管脚，也可以用官方定义好的``GPIO_Pin_x``来描述选中管脚x。
  - ``GPIO_Speed``：设置选中管脚的速度，可选值：GPIO_Speed_10MHz、GPIO_Speed_2MHz、GPIO_Speed_50MHz。
  - ``GPIO_Mode``：设置选中管脚的工作状态，可选值如下表所示：

| GPIO_Mode             | 描述     |
|-----------------------|--------|
| GPIO_Mode_AIN         | 模拟输入   |
| GPIO_Mode_IN_FLOATING | 浮空输入   |
| GPIO_Mode_IPD         | 下拉输入   |
| GPIO_Mode_IPU         | 上拉输入   |
| GPIO_Mode_Out_OD      | 开漏输出   |
| GPIO_Mode_Out_PP      | 推挽输出   |
| GPIO_Mode_AF_OD       | 复用开漏输出 |
| GPIO_Mode_AF_PP       | 复用推挽输出 |

#### 2. GPIO_SetBits函数

功能：设置输出引脚为高电平。

源码：
```c
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    
    GPIOx->BSRR = GPIO_Pin;
}
```

参数说明：

- ``GPIOx``：x可以是A、B、C、D或E，用于选择GPIO。
- ``GPIO_Pin``: 选择待设置的GPIO管脚，使用操作符‘|’可以一次设置多个管脚，也可以用官方定义好的``GPIO_Pin_x``来描述选中管脚x。

示例：设置GPIOB口的引脚1和引脚15（PB_1和PB_15）为高电平

```c
GPIO_SetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_15)
```

#### 3. GPIO_ResetBits函数

功能：设置输出引脚为低电平。

源码:
```c
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    
    GPIOx->BRR = GPIO_Pin;
}
```

参数说明：

- ``GPIOx``：x可以是A、B、C、D或E，用于选择GPIO。
- ``GPIO_Pin``: 选择待设置的GPIO管脚，使用操作符‘|’可以一次设置多个管脚，也可以用官方定义好的``GPIO_Pin_x``来描述选中管脚x。

示例：设置GPIOB口的引脚1和引脚15（PB_1和PB_15）为低电平

```c
GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_15)
```

#### 4. GPIO_ReadInputDataBit函数

功能：读取指定端口具体引脚输入数据。

源码：
```c
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;
  
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
    
    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}
```

参数说明：

- ``GPIOx``：x可以是A、B、C、D或E，用于选择GPIO。
- ``GPIO_Pin``: 待读取的端口位。

示例：读取GPIOB引脚5（PB_5）的数值
```c
u8 ReadValue
ReadValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
```

## 案例：按键控制小灯

### 新建工程

参考[第四章教程新建工程](https://github.com/wenzhangliu/EmbediedSystemsCourse/tree/main/ch4-STM32-Start)（此处略）。

### 查看电路原理图

在编写主程序之前，必须清楚STM32的外围电路结构，特别是引脚的选择。

例如，在教学用的开发板上，其引脚和外围期间的关系对应如下：

| 管脚  | 器件   | 功能                          |
|-----|------|-----------------------------|
| PB5 | LED0 | 输出3.3V，LED0灭；<br>输出0V，LED0亮 |
| PE5 | LED1 | 输出3.3V，LED1灭；<br>输出0V，LED1亮 |
| PE4 | KEY0 | 按下，输入0V；<br>松开，输入3.3V       |
| PE3 | KEY1 | 按下，输入0V；<br>松开，输入3.3V       |


_注意：以上关系不适用其他开发板，务必找到自己开发板或自搭电路原理图，确定好关系才能正确控制LED灯亮灭。_

### 编写main.c

#### 1. 导入库函数

```c
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
```

#### 2. 初始化程序

```c
// 配置PB口（输出）
GPIO_InitTypeDef gpio_init_B;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能PB时钟
gpio_init_B.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出模式
gpio_init_B.GPIO_Pin = GPIO_Pin_5;
gpio_init_B.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOB, &gpio_init_B);
// 配置PE口（输出）
GPIO_InitTypeDef gpio_init_E;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //使能PB时钟
gpio_init_E.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
gpio_init_E.GPIO_Pin = GPIO_Pin_4;
GPIO_Init(GPIOE, &gpio_init_E);
```

#### 3. 延时程序

```c
void delay(u32 t)
{
    for(u32 i=0; i<t; i++){}
}
```

#### 4. 主循环

```c
while(1)
{
    // 循环检测按键
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))
    {
        delay(1000);  // 调用延时程序，按键消抖
        if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)==0)
        {
            key_value = !key_value;
        }
    }
    if(key_value)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    }
}
```

## 完整代码

- [LED灯闪烁](https://github.com/wenzhangliu/EmbediedSystemsCourse/blob/main/ch5-GPIO/LED/USER/main.c)
- [按键控制小灯](https://github.com/wenzhangliu/EmbediedSystemsCourse/blob/main/ch5-GPIO/LeyLED/USER/main.c)
