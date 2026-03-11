# 第6章 STM32中断

```
作者：柳文章
日期：2025.3.28
```

## 异常与中断

所有能打断正常执行流的事件都称为异常。

- 异常：因Cortex-M3核的活动产生，在执行指令或访问存储器时产生，对Cortex-M3核是'同步'的；
- 中断：‘突发事件’，请求信号来自Cortex-M3核的外面，对Cortex-M3核来说是‘异步’的。
- 外部中断：在芯片级别，通过EXTI控制的中断（来自引脚、RTC闹钟、USB唤醒、PVD输出），叫做“外部中断”。

## STM32中断通道

STM32目前支持的中断总数最多84个，其中16个内核异常和Cortex-M3内核有关，不可更改，级别最高。

不同型号的芯片中断向量表参考对应的手册。

## 嵌套向量中断控制器

嵌套中断向量控制器（Nested Vectored Interrupt Controller, NVIC）属于Cortex内核器件，内核异常和外部中断都由它来处理。

### NVIC寄存器

每个寄存器有240位（30 Bytes）。

| 缩写   | 全称                                  | 中文         |
|------|-------------------------------------|------------|
| ISER | Interrupt Set Enable Register       | 中断使能设置寄存器组 |
| ICER | Interrupt Clear Enable Register     | 中断使能清除寄存器组 |
| ISPR | Interrupt Set Pending Register      | 中断挂起设置寄存器组 |
| ICPR | Interrupt Clear Pending Register    | 中断挂起清除寄存器组 |
| IABR | Interrupt Active Bit Register       | 中断激活位寄存器组  |
| IPR  | Interrupt Priority Register         | 中断优先级寄存器组  |
| STIR | Software Trigger Interrupt Register | 软件触发中断寄存器  |

定义NVIC_TypeDef的结构体在库文件[core_cm3.h](https://github.com/wenzhangliu/EmbediedSystemsCourse/blob/main/common/CORE/core_cm3.h)中：

```c
typedef struct
{
    __IO uint32_t ISER[8];                      /*!< Offset: 0x000  Interrupt Set Enable Register           */
       uint32_t RESERVED0[24];                                   
    __IO uint32_t ICER[8];                      /*!< Offset: 0x080  Interrupt Clear Enable Register         */
       uint32_t RSERVED1[24];                                    
    __IO uint32_t ISPR[8];                      /*!< Offset: 0x100  Interrupt Set Pending Register          */
       uint32_t RESERVED2[24];                                   
    __IO uint32_t ICPR[8];                      /*!< Offset: 0x180  Interrupt Clear Pending Register        */
       uint32_t RESERVED3[24];                                   
    __IO uint32_t IABR[8];                      /*!< Offset: 0x200  Interrupt Active bit Register           */
       uint32_t RESERVED4[56];                                   
    __IO uint8_t  IP[240];                      /*!< Offset: 0x300  Interrupt Priority Register (8Bit wide) */
       uint32_t RESERVED5[644];                                  
    __O  uint32_t STIR;                         /*!< Offset: 0xE00  Software Trigger Interrupt Register     */
}  NVIC_Type;
```

### 系统控制寄存器组（SCB）

在中断配置的时候用到：

| 缩写    | 全称                                           | 中文           |
|-------|----------------------------------------------|--------------|
| CPUID | CPUID Base Register                          | CPU ID基址寄存器  |
| ICSR  | Interrupt Control State Register             | 中断控制状态寄存器    |
| VTOR  | Vector Table Offset Register                 | 向量表偏移量寄存器    |
| AIRCR | Application Interrupt/Reset Control Register | 应用中断和复位控制寄存器 |
| SCR   | System Control Register                      | 系统控制寄存器      |
| CCR   | Configuration Control Register               | 配置和控制寄存器     |
| SHPR  | System Handlers Priority Register            | 系统处理优先级寄存器   |
| SHCSR | System Hannler Control and State Register    | 系统处理控制和状态寄存器 |
| CFSR  | Configurable Fault Status Register           | 可配置故障状态寄存器   |
| HFSR  | Hard Fault Status Register                   | 硬件失效状态寄存器    |
| DFSR  | Debuf Fault Status Register                  | 调试失效状态寄存器    |
| MMFAR | Mem Manage Address Register                  | 内存管理地址寄存器    |
| BFAR  | Bus Fault Address Register                   | 总线失效地址寄存器    |
| AFSR  | Auxilliary Fault Status Register             | 辅助失效状态寄存器    |

定义SCB_Type的结构体在库文件[core_cm3.h](https://github.com/wenzhangliu/EmbediedSystemsCourse/blob/main/common/CORE/core_cm3.h)中：

```c
typedef struct
{
    __I  uint32_t CPUID;                        /*!< Offset: 0x00  CPU ID Base Register                                  */
    __IO uint32_t ICSR;                         /*!< Offset: 0x04  Interrupt Control State Register                      */
    __IO uint32_t VTOR;                         /*!< Offset: 0x08  Vector Table Offset Register                          */
    __IO uint32_t AIRCR;                        /*!< Offset: 0x0C  Application Interrupt / Reset Control Register        */
    __IO uint32_t SCR;                          /*!< Offset: 0x10  System Control Register                               */
    __IO uint32_t CCR;                          /*!< Offset: 0x14  Configuration Control Register                        */
    __IO uint8_t  SHP[12];                      /*!< Offset: 0x18  System Handlers Priority Registers (4-7, 8-11, 12-15) */
    __IO uint32_t SHCSR;                        /*!< Offset: 0x24  System Handler Control and State Register             */
    __IO uint32_t CFSR;                         /*!< Offset: 0x28  Configurable Fault Status Register                    */
    __IO uint32_t HFSR;                         /*!< Offset: 0x2C  Hard Fault Status Register                            */
    __IO uint32_t DFSR;                         /*!< Offset: 0x30  Debug Fault Status Register                           */
    __IO uint32_t MMFAR;                        /*!< Offset: 0x34  Mem Manage Address Register                           */
    __IO uint32_t BFAR;                         /*!< Offset: 0x38  Bus Fault Address Register                            */
    __IO uint32_t AFSR;                         /*!< Offset: 0x3C  Auxiliary Fault Status Register                       */
    __I  uint32_t PFR[2];                       /*!< Offset: 0x40  Processor Feature Register                            */
    __I  uint32_t DFR;                          /*!< Offset: 0x48  Debug Feature Register                                */
    __I  uint32_t ADR;                          /*!< Offset: 0x4C  Auxiliary Feature Register                            */
    __I  uint32_t MMFR[4];                      /*!< Offset: 0x50  Memory Model Feature Register                         */
    __I  uint32_t ISAR[5];                      /*!< Offset: 0x60  ISA Feature Register                                  */
} SCB_Type; 
```

### 中断和异常处理

STM32的中断和异常是分别处理的。

#### 中断处理：

- 中断挂起寄存器（ISPR）：挂起暂停正在运行的中断；
- 中断解挂控制寄存器（ICPR）：解除被挂起的寄存器；
- 中断使能寄存器（ISER）：对相应的中断进行使能，使其可以响应中断；
- 中断清除寄存器（ICER）：屏蔽相应的中断；
- 中断优先级寄存器（IPR）：设置优先级；
- 中断激活位寄存器（IABR）：只读寄存器，用于查看当前位对应的中断是否正在被执行。

#### 异常处理：

- 中断控制及状态寄存器（ICSR）：很多位段用于调试目的。
- 系统处理优先级寄存器（SHPR）：对优先级进行配置。
- 系统处理控制和状态寄存器（SHCSR）：使能由于硬件失效、总线失效、以及存储器管理失效引发的特殊异常。

### NVIC配置过程

#### NVIC_PriorityGroupConfig

设置优先级分组：先占优先级和从优先级。

#### NVIC_Init

根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器。

NVIC_InitTypeDef定义位于文件[misc.h](https://github.com/wenzhangliu/EmbediedSystemsCourse/blob/main/common/STM32F10x_FWLib/inc/misc.h)：

```c
typedef struct
{
    uint8_t NVIC_IRQChannel;                    /*!< Specifies the IRQ channel to be enabled or disabled.
                                                   This parameter can be a value of @ref IRQn_Type 
                                                   (For the complete STM32 Devices IRQ Channels list, please
                                                    refer to stm32f10x.h file) */
    
    uint8_t NVIC_IRQChannelPreemptionPriority;  /*!< Specifies the pre-emption priority for the IRQ channel
                                                   specified in NVIC_IRQChannel. This parameter can be a value
                                                   between 0 and 15 as described in the table @ref NVIC_Priority_Table */
    
    uint8_t NVIC_IRQChannelSubPriority;         /*!< Specifies the subpriority level for the IRQ channel specified
                                                   in NVIC_IRQChannel. This parameter can be a value
                                                   between 0 and 15 as described in the table @ref NVIC_Priority_Table */
    
    FunctionalState NVIC_IRQChannelCmd;         /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
                                                   will be enabled or disabled. 
                                                   This parameter can be set either to ENABLE or DISABLE */   
} NVIC_InitTypeDef;
```

#### 

## EXTI外部中断

### EXTI寄存器

| 寄存器                   | 功能                                                |
|-----------------------|---------------------------------------------------|
| 中断屏蔽寄存器（EXTI_IMR）     | 0：屏蔽来自线x上的中断请求；<br>1：开放来自线x上的中断请求                 |
| 事件屏蔽寄存器（EXTI_EMR）     | 0：屏蔽来自线x上的事件请求；<br>1：开放来自线x上的事件请求                 |
| 上升沿触发选择寄存器（EXTI_RTSR） | 0：禁止输入线x上的上升沿触发（中断和事件）；<br>1：允许输入线x上的上升沿触发（中断和事件） |
| 下降沿触发选择寄存器（EXTI_FTSR） | 0：禁止输入线x上的下降沿触发（中断和事件）；<br>1：允许输入线x上的下降沿触发（中断和事件） |
| 软件中断事件寄存器（EXTI_SWIER） | 当该位为"0"时，写“1”将设置EXTI_PR中相应的挂起位                    |
| 挂起寄存器（EXTI_PR）        | 0：没有发生触发请求；<br>1：发生了选择的触发请求                       |

### AFIO寄存器

STM32芯片的每个管脚都可以为中断源，但是需要通过复用重映射功能来进行指定。

- 事件控制寄存器（AFIO_EVCR）
- 外部中断配置寄存器1-4（AFIO_EXTICR1-AFIO_EXTICR4）

### EXTI库函数

| 函数名                      | 描述                                 |
|--------------------------|------------------------------------|
| EXTI_DeInit              | 将外设EXTI寄存器重设为缺省值                   |
| EXTI_Init                | 根据EXTI_InitStruc中指定的参数初始化外设EXTI寄存器 |
| EXTI_StructInit          | 把EXTI_InitStruct中的每个参数按缺省值填入       |
| EXTI_GenerateSWInterrupt | 产生一个软件中断                           |
| EXTI_GetFlagStatus       | 检查指定的EXTI线路标志位设置与否                 |
| EXTI_ClearFlag           | 清除EXTI线路挂起标志位                      |
| EXTI_GetITStatus         | 检查指定的EXTI线路触发请求发生与否                |
| EXTI_ClearITPendingBit   | 清除EXTI线路挂起位                        |

#### EXTI_Init函数

位于文件[stm32f10x_exti.h](https://github.com/wenzhangliu/EmbediedSystemsCourse/blob/main/common/STM32F10x_FWLib/inc/stm32f10x_exti.h)中：

```c
typedef struct
{
    uint32_t EXTI_Line;               /*!< Specifies the EXTI lines to be enabled or disabled.
                                         This parameter can be any combination of @ref EXTI_Lines */
    
    EXTIMode_TypeDef EXTI_Mode;       /*!< Specifies the mode for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */
    
    EXTITrigger_TypeDef EXTI_Trigger; /*!< Specifies the trigger signal active edge for the EXTI lines.
                                         This parameter can be a value of @ref EXTIMode_TypeDef */
    
    FunctionalState EXTI_LineCmd;     /*!< Specifies the new state of the selected EXTI lines.
                                         This parameter can be set either to ENABLE or DISABLE */ 
}EXTI_InitTypeDef;
```

- ``EXTI_Line``: 选择外部EXTI0-EXTI18。
- ``EXTI_Mode``: 选择事件请求还是中断请求。

| EXTI_Mode           | 描述            |
|---------------------|---------------|
| EXTI_Mode_Event     | 设置EXTI线路为事件请求 |
| EXTI_Mode_Interrupt | 设置EXTI线路为中断请求 |

- ``EXTI_Trigger``: 设置了被使能线路的触发边沿。

| EXTI_Trigger                | 描述                 |
|-----------------------------|--------------------|
| EXTI_Trigger_Falling        | 设置输入线路下降沿为中断请求     |
| EXTI_Trigger_Rising         | 设置输入线路上升沿为中断请求     |
| EXTI_Trigger_Rising_Falling | 设置输入线路上升沿和下降沿为中断请求 |

- ``EXTI_LineCmd``：定义选中线路的新状态，取值：ENABLE或DISABLE。

### EXTI中断操作过程

#### 步骤1：设定好中断通道

配置GPIO端口的工作方式，配置GPIO和EXTI映射关系。

#### 步骤2：中断处理

配置EXTI触发条件，配置相应NVIC，根据中断编号对应到中断向量表查找中断服务函数``xxx_IRQHandler(void)``的入口地址（即函数指针）。

#### 步骤3：中断响应

当到达中断触发条件时，内核从主程序先跳转到相应的中断向量处，然后根据中断向量提供地址信息，又跳转到中断服务函数``xxx_IRQHandler(void)``入口地址，
并在执行完中断服务函数程序后，返回主程序处恢复执行。

## 作业

利用中断，控制小灯亮灭。
