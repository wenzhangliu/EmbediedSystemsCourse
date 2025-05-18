# 如何使用Proteus进行仿真实验

如果没有STM32开发板或者最小系统，将无法在硬件基础上完成实验课。

替代的方案是，使用Proteus软件进行模拟。以下是如何使用该软件部署本实验课的一般性步骤。（以GPIO实验为例，其他实验步骤类似）

## 安装Proteus软件

### 步骤1：下载软件

Proteus是一款部分功能免费的软件，可免费获取，但是高阶功能需要付费。本课程将使用其基础功能。

（该软件仅支持Windows系统）

安装包地址：[网友分享，点击这里](https://docs.qq.com/doc/DS3Rrdnl5bE1FS09H)

### 步骤2：安装过程

- 解压安装包，得到 Proteus 8.16 的安装文件；
- 暂时关闭杀毒软件；
- 进入``proteus8.16``文件夹，以管理员身份运行``Proteus8.16.SP3.exe``;
  - 点击``Next``;
  - 勾选"I accept the terms of this agreement"，点击"Next";
  - 勾选"Use a locally installed license key"，点击"Next";
  - 若第一次安装，需要激活：
    - 点击"Browse For Key File";
    - 找到压缩包解压目录，进入"2.和谐工具"文件夹；
    - 选中``Licence2.lxk``文件，点击"打开";
    - 点击"Install";
    - 新弹出窗口"Labcenter Licence Manager 1.11"，点击"是"，确认安装许可证文件;
    - 若窗口右侧出现一样的内容，说明许可证文件安装成功，点击"Close"关闭当前窗口。
  - 进入新窗口"Import Legacy Styles, Templates and Libraries"，默认三项均不勾选，点击"Next";
  - 选择安装类型："Typical";
  - 等待安装完成。
- 软件破解方法：
  - 回到解压目录，进入"2.和谐工具"文件夹;
  - 管理员身份运行"Patch-Proteus-8.16-SP3-36097.0.exe"（记得暂时关闭杀毒软件）;
  - 语言选择默认English，点击"OK";
  - 选择默认路径；
  - 点击"Start"，开始破解。
  - 点击"Exit"退出程序。

参考链接：[Proteus8.16仿真软件安装图文教程（Proteus 8 Professional）](https://blog.csdn.net/weixin_52733843/article/details/134715782)

### 步骤3：使用方法

- 新建工程，选择工程路径；
- 选择``Create a schematic from the selected template``，选择``DEFAULT``，点击"Next";
- 选择``Do not create a PCB layout``，点击"Next";
- 选择``Create Firmware Project``：
  - ``Family``选择``Cortex-M3``;
  - ``Controller``选择``STM32F103C8``;
  - ``Compiler``选择``GCC for ARM (not configured)``;
  - 勾选``Create Quick Start Files``，点击"Next";
  - 点击``Finish``完成工程创建。
- 进入``Schematic Capture``窗口，出现芯片原理图。
- 搭建最小系统电路：包含晶振、电源等，[参考这里](https://zhuanlan.zhihu.com/p/5403780335)，（注意引脚选择）。
- 搭建满足当前实验要求的外围电路，如LED灯、按键等。
- 利用[Keil编程](https://github.com/wenzhangliu/EmbediedSystemsCourse/tree/main/ch4-STM32-Start)，生成HEX文件;
- 将生成的HEX文件添加到STM32芯片上：
  - 进入Proteus软件，双击芯片原理图；
  - 配置Program File，选择HEX文件；
  - Crystal Frequency选择"8M";
  - 点击``确定``。
- 点击左下角``Run``图标，运行程序。
- 观察芯片引脚，如果出现预期效果，说明仿真成功。

