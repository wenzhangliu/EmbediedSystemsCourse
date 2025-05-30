# 第七章 定时器原理及应用

```
作者：柳文章
日期：2025.3.31
```

## STM32定时器概述

定时器功能比较：

| 定时器      | 计数器分辨率 | 计数器类型      | 预分频系统           | 产生DMA请求 | 捕获/比较通道 | 互补输出 |
|----------|--------|------------|-----------------|---------|---------|------|
| TIM1     | 16位    | 向上、向下、向上/下 | 1-65536之间的任意整数  | 可以      | 4       | 有    |
| TIM2、3、4 | 16位    |向上、向下、向上/下| 1-65536之间的任意整数  | 可以      | 4       | 没有   |

- 高级控制定时器（TIM1）：分配到6个通道的三相PWM发生器，具有带死区插入的互补PWM输出，也可以当做完整的通用定时器。
- 通用定时器（TIMx）：通用。
- 看门狗定时器：程序开始运行，看门狗开始倒计数。如果程序运行正常，看门狗复位，重新开始计数。如果看门狗减到0则认为程序运行不正常，强制整个系统复位。
- 系统滴答定时器：专用于实时操作系统，也可当成一个标准的递减计数器。
- 实时时钟：为操作系统提供了一个可靠的时间，并且在断电的情况下，RTC也可以通过电池供电一直运行下去。通过配置，可准确地每秒钟中断一次。

## 高级/通用定时器

STM32主要包括高级和通用两种定时器：

通用定时器（TIM2、TIM3、TIM4）功能：
- 16位向上、向下、向上/向下自动装载计数器。
- 16位可编程预分频器，计数器时钟频率的分频系数为1-65536之间的任意数值。
- 4个独立通道：输入捕获、输出比较、PWM生成、单脉冲模式输出。
- 使用外部信号控制定时器和定时器互连的同步电路。
- 以下事件发生会产生中断/DMA：
  - 更新：计数器溢出，计数器初始化。
  - 触发事件：计数器启动、停止、初始化、内部/外部触发计数。
  - 输入捕获。
  - 输出比较。
- 支持针对定位的增量（正交）编码器和霍尔传感器电路。
- 触发输入作为外部时钟或者按周期的电流管理。

高级定时器TIM1，在通用定时器基础上新增如下功能：

- 死区时间可编程的互补PWM输出：对应的引脚输出有TIM1_CH1和TIM1_CH1N正反互补输出（通用定时器只有TIMx_CHx正输出）。
- 允许在指定数目的计数器周期之后更新定时器寄存器的重复计数器。
- 刹车输入信号可以将定时器输出信号置于复位状态或者一个已知状态。
- 刹车信号输入产生中断/DMA。

_本课程只涉及通用定时器功能。_

### 主要寄存器

通用定时器TIMx主要寄存器功能说明：

| 寄存器名称                             | 功能               |
|-----------------------------------|------------------|
| 控制寄存器（TIMx_CR1、TIMx_CR2）          | 用于控制独立通用定时器      |
| 模式控制寄存器（TIMx_SMCR）                | 用于从模式控制          |
| 状态寄存器（TIMx_SR）                    | 保存定时器状态          |
| 捕获/比较模式寄存器（TIMx_CCMR1、TIMx_CCMR2） | 用于捕获/比较模式        |
| 捕获/比较使能寄存器（TIMx_CCER）             | 用于允许捕获/比较        |
| 计数器（TIMx_CNT）                     | 用于保存计数器的计数值      |
| 预分频器（TIMx_PSC）                    | 用于设置预分频器的值       |
| 自动重装载寄存器（TIMx_ARR）                | 保存计数器自动重装载的计数值   |
| 捕获/比较寄存器1—4（TIMx_CCR1-TIMx_CCR4）  | 保存捕获/比较通道1-4的计数值 |

### 主要库函数

此处仅介绍常用库函数：

#### TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)

功能：对定时器的时间基数单位参数初始化。

结构体说明：

```c
typedef struct
{
  uint16_t TIM_Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock.
                                       This parameter can be a number between 0x0000 and 0xFFFF */

  uint16_t TIM_CounterMode;       /*!< Specifies the counter mode.
                                       This parameter can be a value of @ref TIM_Counter_Mode */

  uint16_t TIM_Period;            /*!< Specifies the period value to be loaded into the active
                                       Auto-Reload Register at the next update event.
                                       This parameter must be a number between 0x0000 and 0xFFFF.  */ 

  uint16_t TIM_ClockDivision;     /*!< Specifies the clock division.
                                      This parameter can be a value of @ref TIM_Clock_Division_CKD */

  uint8_t TIM_RepetitionCounter;  /*!< Specifies the repetition counter value. Each time the RCR downcounter
                                       reaches zero, an update event is generated and counting restarts
                                       from the RCR value (N).
                                       This means in PWM mode that (N+1) corresponds to:
                                          - the number of PWM periods in edge-aligned mode
                                          - the number of half PWM period in center-aligned mode
                                       This parameter must be a number between 0x00 and 0xFF. 
                                       @note This parameter is valid only for TIM1 and TIM8. */
} TIM_TimeBaseInitTypeDef;
```

- TIM_Period: 在下一个更新事件装入活动的自动重装载寄存器周期的值。它的取值必须在0x0000和0xFFFF之间。
- TIM_Prescaler：设置了用来作为TIMx时钟频率除数的预分频值。它的取值必须在0x0000和0xFFFF之间。
- TIM_ClockDivision：设置时钟分割。
- TIM_CounterMode：选择计数器模式（取值：TIM_CounterMode_Up、TIM_CounterMode_Down、TIM_CounterMode_CenterAligned1、TIM_CounterMode_CenterAligned2、TIM_CounterMode_CenterAligned3）。

#### TIM_OCxInit(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)

功能：对TIM的输出通道x外设参数初始化。

结构体说明：

```c
typedef struct
{
  uint16_t TIM_OCMode;        /*!< Specifies the TIM mode.
                                   This parameter can be a value of @ref TIM_Output_Compare_and_PWM_modes */

  uint16_t TIM_OutputState;   /*!< Specifies the TIM Output Compare state.
                                   This parameter can be a value of @ref TIM_Output_Compare_state */

  uint16_t TIM_OutputNState;  /*!< Specifies the TIM complementary Output Compare state.
                                   This parameter can be a value of @ref TIM_Output_Compare_N_state
                                   @note This parameter is valid only for TIM1 and TIM8. */

  uint16_t TIM_Pulse;         /*!< Specifies the pulse value to be loaded into the Capture Compare Register. 
                                   This parameter can be a number between 0x0000 and 0xFFFF */

  uint16_t TIM_OCPolarity;    /*!< Specifies the output polarity.
                                   This parameter can be a value of @ref TIM_Output_Compare_Polarity */

  uint16_t TIM_OCNPolarity;   /*!< Specifies the complementary output polarity.
                                   This parameter can be a value of @ref TIM_Output_Compare_N_Polarity
                                   @note This parameter is valid only for TIM1 and TIM8. */

  uint16_t TIM_OCIdleState;   /*!< Specifies the TIM Output Compare pin state during Idle state.
                                   This parameter can be a value of @ref TIM_Output_Compare_Idle_State
                                   @note This parameter is valid only for TIM1 and TIM8. */

  uint16_t TIM_OCNIdleState;  /*!< Specifies the TIM Output Compare pin state during Idle state.
                                   This parameter can be a value of @ref TIM_Output_Compare_N_Idle_State
                                   @note This parameter is valid only for TIM1 and TIM8. */
} TIM_OCInitTypeDef;
```

- TIM_OCMode选择定时器模式（TIM_OCMode_Timing, TIM_OCMode_Active, TIM_OCMode_Inactive, TIM_OCMode_Toggle, TIM_OCMode_PWM1, TIM_OCMode_PWM2）.
- TIM_Pulse：设置了待装入捕获比较寄存器的脉冲值。取值必须在0x0000和0xFFFF之间。
- TIM_OCPolarity: 输出极性。（TIM_OCPolarity_High，TIM_OCPolarity_Low）。

#### void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)

功能：对TIMx的输入外设参数初始化。

结构体说明：

```c
typedef struct
{

  uint16_t TIM_Channel;      /*!< Specifies the TIM channel.
                                  This parameter can be a value of @ref TIM_Channel */

  uint16_t TIM_ICPolarity;   /*!< Specifies the active edge of the input signal.
                                  This parameter can be a value of @ref TIM_Input_Capture_Polarity */

  uint16_t TIM_ICSelection;  /*!< Specifies the input.
                                  This parameter can be a value of @ref TIM_Input_Capture_Selection */

  uint16_t TIM_ICPrescaler;  /*!< Specifies the Input Capture Prescaler.
                                  This parameter can be a value of @ref TIM_Input_Capture_Prescaler */

  uint16_t TIM_ICFilter;     /*!< Specifies the input capture filter.
                                  This parameter can be a number between 0x0 and 0xF */
} TIM_ICInitTypeDef;
```

- TIM_Channel：选择通道。
- TIM_ICPolarity：输入活动边沿。
- TIM_ICSelection：选择输入。
- TIM_ICPrescaler：设置输入捕获预分频器。
- TIM_ICFilter：选择输入比较滤波器。

#### void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState)

功能：配置中断。

输入参数``TIM_IT``使能或失能TIM中断。

| TIM_IT         | 描述           |
|----------------|--------------|
| TIM_IT_Update  | TIM中断源       |
| TIM_IT_CC1     | TIM捕获/比较1中断源 |
| TIM_IT_CC2     | TIM捕获/比较2中断源 |
| TIM_IT_CC3     | TIM捕获/比较3中断源 |
| TIM_IT_CC4     | TIM捕获/比较4中断源 |
| TIM_IT_Trigger | TIM触发中断源     |

#### void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter)

功能：配置外部时钟模式。

- TIM_ExtTRGPrescaler：设置TIMx外部触发预分频。
- TIM_ExtTRGPolarity：设置TIMx外部触发极性。

#### void TIM_SetCompare1(TIM_TypeDef* TIMx, uint16_t Compare1)

功能：设置TIM2、3、4捕获比较1-4寄存器值。

- uint16_t TIM_GetCapture1(TIM_TypeDef* TIMx): 表示获得TIMx输入捕获1-4的值。

### 定时器计数器基本说明

#### 时钟源

- 内部时钟源（CK_INT）：内部时钟源作为时钟时，定时器时钟不是直接来自APB1或APB2，而是来自于输入为APB1或APB2的一个倍频器。
- 外部时钟源：
  - 外部时钟模式1：外部输入脚（TIx）。
  - 外部时钟模式2：外部触发输入（ETR）。
- 内部触发输入（ITRx）：使用一个定时器作为另一个定时器的预分频器。

#### 计数模式

- 向上计数模式；
- 向下计数模式；
- 中央对齐模式（向上/向下计数）：计数器从0计数至计数值，产生计数器溢出事件，然后向下计数到1再产生一个计数器溢出事件，。。。循环往复。

### 定时器功能

对内部时钟源进行计数，实现较为精确的计时。

案例：利用定时器实现1秒延迟，实现小灯闪烁。

（见本目录下示例代码）

### 计数器功能

对引脚上的输入信号进行统计。引脚输入信号可以作为计数时钟，输入引脚可以为TIMx_CHx, TIMx_ETR引脚。

案例：对外部按键次数进行计数，当达到预设数字时，小灯点亮。

### 捕获模式功能

输入捕获模式可以用来测量脉冲宽度或频率。

通过检测 TIMx_CHx 上的边沿信号，在边沿信号发生跳变（如上升沿/下降沿）时，将当前定时器的值（TIMx_CNT）存放到对应通道的捕获/比较寄存器（TIMx_CCRx）里，完成一次捕获。
同时还可以配置捕获时是否触发中断/DMA。

案例：对按键按下去的时长进行测量，并以该时长设置小灯闪烁频率。

### 输出PWM模式

PWM（Pulse Width Modulation），脉冲宽度调制技术：通过对一系列脉冲的宽度进行调制，获得需要的波形。

PWM输出计算公式：

$$占空比=\frac{脉冲宽度}{脉冲周期} * 100\%$$

好处：从处理器到被控系统信号都是数字形式，无需进行数模转换，这样可以将噪声影响降到最小。

（一般噪声不会对数字信号造成从0到1或者从1到0的影响。）

STM微控制器的每个通用定时器都有4路PWM输出TIMx_CH1~TIMx_CH4，每个都对应一个引脚。

高级定时器有更多路PWM输出：

| 复用功能映像    | TIM1_REMAP[1:0]=00 |
|-----------|--------------------|
| TIM1_ETR  | PA12               |
| TIM1_CH1  | PA8                |
| TIM1_CH2  | PA9                |
| TIM1_CH3  | PA10               |
| TIM1_CH4  | PA11               |
| TIM1_BKIN | PB12               |
| TIM1_CH1N | PB13               |
| TIM1_CH2N | PB14               |
| TIM1_CH3N | PB15               |

案例：利用PWM实现呼吸灯效果。

[推荐视频](https://www.bilibili.com/video/BV1Yx4y1x7xY?spm_id_from=333.788.videopod.sections&vd_source=b5f9a01aec9e413ef6288712a9ec68bd)

### 定时器输出比较模式

设置比较数值并写入比较寄存器，定时器会不停将该内容和计数器内容比较，一旦条件成立，则产生输出，如使能中断，则产生中断；如使能引脚输出，则产生输出波形。

案例：分别产生2s、1s的方波。

### PWM输入捕获模式

TIMx可以检测某个通道对应引脚上的电平边沿（上升或下降），并在电平边沿产生的时刻将当前定时器计数值写入捕获/比较寄存器。
PWM输入捕获功能可以测量定时器某个通道上的PWM信号频率与占空比。

### 其他功能模块

#### 编码器接口模式

编码器广泛用于测量电机转速和方向，基本原理见教材。

STM32定时器的两个输入端口TI1和TI2被用来作为增量编码器的接口，
假定计数器已经启动，则计数器由TI1FP1或TI2FP2上的电平跳变驱动。
TI1FP1和TI2FP2是TI1和TI2在通过输入滤波器和极性控制后的信号，如果没有滤波和变相，则TI1FP1=TI1，TI2FP2=TI2.
根据两个输入信号的跳变顺序，产生计数脉冲和方向信号。
通过读取计数器信息，得到电机速度和转向。

#### 单脉冲模式

单脉冲模式（One Pause Mode, OPM），允许计数器响应一个激励，并在一个程序可控的延时之后，产生一个脉宽可控的脉冲。

## 系统滴答定时器（Systick）

Systick定时器被捆绑在NVIC中，用于产生SYSTICK异常（异常号：15）。

### Systick工作分析及配置

Systick是个24位定时器，一次最多可以计数2^24个脉冲，计数值保存在``当前值寄存器STK_VAL(Systick current value register)``中，
只能向下计数。减至0时，由硬件自动把``重载寄存器STK_LOAD(Systick reload value register)``中保存的数据加载到``STK_VAL``，重新向下计数。
计数至0时，可触发异常，产生中断事件。

案例：利用Systick定时器产生1ms延迟的程序。

## 看门狗定时器

看门狗（WDT，Watch Dog Timer）是微控制器的一个组成部分，实际上是一个计数器，功能如下：
- 给看门狗一个数字，程序开始运行后看门狗开始倒计数。
- 程序运行正常，过一段时间CPU发出指令让看门狗复位，重新计数。
- 看门狗减到0，认为程序工作不正常，强制整个系统复位。

### 独立看门狗

独立看门狗：Independent WDG（IWDG），是基于一个12位的递减计数器和一个8位的预分频器。
使用的是独立时钟，由专用的32KHz的低速时钟来驱动。因此，即使主时钟发生故障它仍然有效。

IWDT适用于那些需要看门狗作为一个在主程序之外能够完全独立工作、并且对时间精度要求较低的场合。

主要寄存器：
- 键值寄存器（IWDG_KR）
- 预分频寄存器（IWDG_PR）
- 重装载寄存器（IWDG_RLR）

### 窗口看门狗

窗口看门狗（Window WDG, WWDG）由APB1时钟分频后得到的时钟驱动，通过可配置的时间窗口来检测应用程序非正常的过迟或过早的行为。

### 独立看门狗与窗口看门狗区别

| 独立看门狗                  | 窗口看门狗              |
|------------------------|--------------------|
| 没有中断                   | 有中断                |
| 有软硬件之分                 | 只有软件控制             |
| 有下限                    | 有下限和上限             |
| 12位递减                  | 7位递减               |
| 用的是内部大约40KHz RC振荡器     | 用的是系统时钟``APB1ENR`` |
| 独立于系统之外，不受系统影响的系统故障探测器 | 系统内部的故障探测器，时钟与系统相同 |

## 实时时钟（RTC）

实时时钟，RTC（Real-Time Clock），为操作系统提供了一个可靠的时间，并在断电情况下通过电池供电一直运行下去。

RTC内部维持了一个独立的定时器，通过配置，可以准确地每秒钟中断一次，也可以提供时钟日历的功能。

