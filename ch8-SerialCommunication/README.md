# 串行通信

```
作者：柳文章
日期：2025.4.11
```

## 基本概念

- **串行通信**：Serial communication，是指在计算机总线或其他数据通道上，每次传输一个位元数据，并连续进行以上单次过程的通信方式。串行通信可被用于长距离通信和大多数计算网络。
  - 异步串行通信：接收器和发送器有各自的时钟，非同步工作。一般由1个起始位、7-9个数据位、1-2个停止位、1个校验位组成。起始位约定为0，空闲位约定为1。
  - 同步串行通信：接收器和发送器由同一个时钟源控制。
- **并行通信**：Parallel communication，是指在串行端口上通过一次同时传输若干位元数据的方式进行通信。
- **波特率（BR）**：单位时间传输的数据位数，单位: bps (bit per second)。
- **串行通信的校验**：异步通信时可能出现帧格式错误、超时错误等传输错误。因此，一般考虑在通信过程中对数据差错进行校验，保证传输无误。
- **传输方式**：
  - 单工：消息只能单向传播，如遥控器；
  - 半双工：数据可以在一个信号载体的两个方向传播，但是不能同时进行，如对讲机。
  - 全双工：允许数据在两个方向上同时传播，相当于两个单工通信的结合。

## 通用同步异步收发器（USART）

USART 全称为“universal synchronous and asynchronous receiver-transmitter”。

### USART概述

两个引脚：
- RX：接受数据串行输入。
- TX：发送数据输出。当发送器被禁止时，输出引脚恢复到它的I/O端口配置。当发送器被激活，并且不发送数据时，TX引脚处于高电平状态。
- 总线在发送或接收前应处于空闲状态。
- 一个起始位。
- 一个数据字（8或9位），最低有效位在前。
- 0.5、1.5、2个的停止位，由此表明帧的结束。
- 使用分数波特率发生器——12位整数和4位小数的表示方法。
- 一个状态寄存器（USART_SR）。
- 数据寄存器（USART_DR）。
- 一个波特率寄存器（USART_BRR），12位的整数和4位小数。
- 一个智能卡模式下的保护时间寄存器（USART_GTPR）。

#### I/O口说明

STM32F103芯片有三组USART模块：USART1、USART2、USART3。

USART I/O口说明表如下：

| 端口     | 引脚         | I/O口 | 重映射口 | 功能说明         |
|--------|------------|------|------|--------------|
| USART1 | USART1_TX  | PA9  | PB6  | 发送数据输出       |
|        | USART1_RX  | PA10 | PB7  | 接收数据输入       |
|        | USART1_CTS | PA11 |      | 清除发送         |
|        | USART1_RTS | PA12 |      | 发送请求         |
|        | USART1_CK  | PA8  |      | 同步模式时，作为同步时钟 |
| USART2 | USART2_TX  | PA2  | PD7  | 发送数据输出       |
|        | USART2_RX  | PA3  | PD6  | 接收数据输入       |
|        | USART2_CTS | PA0  | PD3  | 清除发送         |
|        | USART2_RTS | PA1  | PD4  | 发送请求         |
|        | USART2_CK  | PA4  | PD7  | 同步模式时，作为同步时钟 |
| USART3 | USART3_TX  | PB10 | PD8  | 发送数据输出       |
|        | USART3_RX  | PB11 | PD9  | 接收数据输入       |
|        | USART3_CTS | PB13 | PD11 | 清除发送         |
|        | USART3_RTS | PB14 | PD12 | 发送请求         |
|        | USART3_CK  | PB12 | PD10 | 同步模式时，作为同步时钟 |

#### 时钟管理

为了使能USART模块的工作时钟，需要使能串口以及对应的引脚时钟，以USART1异步基本串行通信模式为例，时钟配置如下：

```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE)
```

#### 中断源

USART模块对应的中断函数：
- ``void USART1_IRQHandler(void)``;
- ``void USART2_IRQHandler(void)``;
- ``void USART3_IRQHandler(void)``.
这些函数名称不能改变，已经在启动文件中做了指定。

有关USART的中断号需要在NVIC中断配置函数中指定，在``stm32f10x.h``头文件中，被设置为：``USART1_IRQn``, ``USART2_IRQn``, ``USART3_IRQn``。这些名称不能改变。

### USART帧格式

起始位 + 数据帧 + 可能的奇偶校验位

USART支持多种停止位的配置：
- 1个停止位：停止位位数的默认值；
- 2个停止位：可用于常规USART模式、单线模式以及调制解调器模式；
- 0.5个停止位：在智能卡模式下接收数据时使用；
- 1.5个停止位：在智能卡模式下发送和接收数据时使用。

### USART寄存器

### USART库函数

库函数列表：

| 函数名                 | 描述                 |
|---------------------|--------------------|
| USART_Init          | 初始化所使用的串口外设        |
| USART_ITConfig      | 使能或者失能指定的USART中断   |
| USART_SendData      | 通过外设USARTx发送单个数据   |
| USART_ReceiveData   | 返回USARTx最近解释接收到的数据 |
| USART_GetFlagStatus | 检查指定的USART标志位设置与否  |
| USART_ClearFlag     | 清除USARTx的待处理标志位    |
| USART_GetITStatus   | 检查指定的USART中断发生与否   |

初始化示例：
```c
USART_InitTypeDef usart_init = {
.USART_BaudRate = 9600,  // 波特率
.USART_WordLength = USART_WorldLength_8b,  //数据位8位
.USART_StopBits = USART_StopBits_1,  //1位停止位
.USART_Parity = USART_Parity_No,  //无奇偶校验
.USART_HardwareFlowControl = USART_HardwareFlowControl_None,  //无硬件流
.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,  //发送接收模式
};
.USART_Init(USART1, &usart_init);
.USART_Cmd(USART1, ENABLE);
```

## USART操作

### USART发送与接收

### 硬件流控制

### 奇偶校验

### USART中断请求

## STM32芯片USART其他功能模式及扩展说明

### USART其他功能模式

### RS-232接口

### 基于串口的无线通信

## USART应用实例

### 查询传送方式

### 中断传送方式

### 环形队列串口应用

## 串行同步通信（SPI）

### SPI接口概述

### SPI接口库函数

### SPI接口实例
