# 《嵌入式系统实验》

代码语言：C语言

教学芯片：STM32F103系列。

## 课程大纲

- GPIO输出控制实验
  - 了解[STM32的GPIO推挽输出原理](https://github.com/wenzhangliu/EmbediedSystemsCourse/tree/main/ch5-GPIO)；
  - 搭建（已经搭建好的，如开发板，请查阅）LED小灯电路；
  - 实验1：点亮LED小灯；
  - 实验2：实现LED小灯闪烁。
- GPIO输入控制实验
  - 了解[STM32的GPIO输入原理](https://github.com/wenzhangliu/EmbediedSystemsCourse/tree/main/ch5-GPIO)；
  - LED及按键硬件电路；
  - 掌握按键去抖操作；
  - 实验1：利用按键实现LED灯亮灭。
  - 实验2：实现利用按键长按动作控制LED灯亮灭。
- 外部中断实验
  - 了解[STM32外部中断原理](https://github.com/wenzhangliu/EmbediedSystemsCourse/tree/main/ch6-STM32-Interrupt)；
  - 实验1：利用中断捕获按键按下事件，并点亮LED小灯。
  - 实验2：利用中断捕获按键按下事件，控制小灯亮灭状态（按下一次，切换亮灭状态）。
- 定时器实验
  - 了解[定时器原理](https://github.com/wenzhangliu/EmbediedSystemsCourse/tree/main/ch7-TIM)；
  - 实验1：利用定时器实现小灯规律闪烁（精确到秒）；
  - 实验2：利用按键控制小灯闪烁频率（两个按键，分别用于控制频率增加和减小）。
- PWM实验
  - 掌握[PWM原理](https://github.com/wenzhangliu/EmbediedSystemsCourse/tree/main/ch7-TIM#%E8%BE%93%E5%87%BApwm%E6%A8%A1%E5%BC%8F)；
  - 实验1：利用PWM控制小灯亮度；
  - 实验2：利用按键控制小灯亮度（调节PWM占空比，两个按键，一加一减）；
  - 实验3：利用PWM实现呼吸灯效果。
- 串口通信实验
  - 掌握[USART原理](https://github.com/wenzhangliu/EmbediedSystemsCourse/tree/main/ch8-SerialCommunication)；
  - 实验1：利用查询方式实现电脑主机和单片机之间的通信；
  - 实验2：利用中断方式实现电脑主机和单片机之间的通信；
  - 实验3：利用环形队列方式实现电脑主机和单片机之间的通信。

## 实验课要点

- 同学们自带笔记本电脑+STM32单片机开发板。

- 没有开发板的同学，在笔记本电脑上自行安装 Proteus 仿真软件（[参考](https://blog.csdn.net/qq_29734297/article/details/122432150)）。（可行，但不推荐）

- 有开发板的同学，需要按照理论课所述要求，自行安装Keil、驱动等软件开发环境。（[参考这里](https://github.com/wenzhangliu/EmbediedSystemsCourse/tree/main/ch4-STM32-Start)）

- 可2-3人一组合作完成实验，但是实验报告需要每位同学独立完成。（禁止抄袭，一旦发现，双方作业均为无效）

