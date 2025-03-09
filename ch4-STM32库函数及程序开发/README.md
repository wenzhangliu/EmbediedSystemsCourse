# 第4章 STM32库函数及程序开发

## 开发环境安装

### 第 1 步：安装MDK

官网最新版下载链接：[https://www.keil.com/download/product/](https://www.keil.com/download/product/)

历史版本下载链接：[https://www.keil.com/update/rvmdk.asp](https://www.keil.com/update/rvmdk.asp)

```attention
注意：下载官方最新版在安装过程中可能会遇到各种问题，建议安装旧版更加稳定，如MDK527.
```

安装结束后，会提示安装Packs（固件包）。该窗口可直接关闭，也可等待下载结束再关闭。
（本课程所涉及的固件包在下一步单独安装）

安装结束后，桌面会出现一个``Keil uVision5``的图标，即表示安装成功。

### 第 2 步：激活 Keil uVision5

安装成功后的``Keil uVision5``只支持试用，需要激活才能长时间使用。

- 首先在网上找“Keil5“注册机；
- 暂时关闭电脑杀毒软件，以管理员身份打开``Keil uVision5``；
- 点击``file``->``License Management``，复制``Computer ID（CID）``；
- 找到“Keil5“注册机，右击管理员身份打开，将复制好的CID粘贴进去；
- 确保注册机中的``Target``选为'ARM'，点击``Generate``生成激活码，复制它；
- 回到``Keil uVision5``软件，将刚刚复制好的激活码粘贴，点击``Add LIC``，完成激活。

### 第 3 步：安装STM32固件包

首先，根据STM32微控制器系列型号选择固件包下载。

固件包下载链接：[https://www.keil.arm.com/packs/](https://www.keil.arm.com/packs/)

进入网站后：
- 搜索“STM32F1”；
- 点击下方“STM32F1xx_DFP”一栏右侧的``Download version xxx``下载。
- 下载后，文件名为“keil.STM32Fxx_DFP.x.x.x”，双击安装（安装目录自动识别为MDK所设定的Packs目录）。

### 第 4 步：下载固件库

本课程基于库函数开发STM32，因此需要下载相应的固件库。

一般情况下，开发板供应商会提供相应的固件库，如果没有提供，也可以在官网上自行下载：
- 登入[https://www.st.com/content/st_com/en.html](https://www.st.com/content/st_com/en.html)（需要按照网站指示注册个账号）;
- 注册账号后，依次点击``Design Tools``->``Tools & software``->``Embeded software``->``STM32 embeded software``->``STM32 Standard Peripheral Libraries``->``F1``->``Select version``->``3.5.0``，最后点击下载。

## 驱动安装（USB驱动）

以上步骤完成了STM32开发所需要的基本软件环境，接下来最后一步需要安装USB驱动，以便开发好的程序能够顺利下载至芯片内部，或者实现硬件调试功能等。

### 安装 CH340 驱动

### 安装 ST-Link 驱动
