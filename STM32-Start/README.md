# 第4章 STM32库函数及程序开发

```
作者：柳文章
日期：2025.3.09
```

目录索引:

- [开发环境安装](#开发环境安装)
- [驱动安装](#驱动安装usb驱动)
- [新建工程](#新建工程)
- [运行第一个程序（点亮LED小灯）](#运行第一个程序点亮led灯)

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

- 首先在网上搜“Keil5注册机下载“，随便找一个下载下来；
- 暂时关闭电脑杀毒软件，以管理员身份打开``Keil uVision5``；
- 点击``file``->``License Management``，复制``Computer ID（CID）``；
- 找到“Keil5“注册机，右击管理员身份打开，将复制好的CID粘贴进去；
- 确保注册机中的``Target``选为'ARM'，点击``Generate``生成激活码，复制它；
- 回到``Keil uVision5``软件，将刚刚复制好的激活码粘贴，点击``Add LIC``，完成激活。（记得恢复杀毒软件）

```attention
关于简体中文不能显示的问题：点击Edit->Configuration...->Editor->Encoding, 选择 Chiense GB2312 (Simplified)，点击 OK。
```

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

- 进入官网（[https://www.wch.cn/downloads/CH341SER_EXE.html](https://www.wch.cn/downloads/CH341SER_EXE.html)），或直接在网上搜CH340，下载``CH341SER.exe``可执行文件；
- 双击``CH341SER.exe``，在弹窗中点击``安装``，完成安装。

### 安装 J-Link/ST-Link 驱动

根据所购买开发板选择安装J-Link还是ST-Link。

- J-Link安装方法：
    - 进入官网下载：[https://www.segger.com/downloads/jlink/JLink_Windows_beta.exe](https://www.segger.com/downloads/jlink/JLink_Windows_beta.exe)；
    - 双击安装程序``JLink_Windows_V818_x86_64``，一路确定到底，完成安装。
- ST-Link安装方法：
    - 进入官网下载：[https://www.st.com.cn/zh/development-tools/stsw-link009.html#get-software](https://www.st.com.cn/zh/development-tools/stsw-link009.html#get-software)；
    - 新建文件夹，命名和下载下来的压缩包一样，即“en.stsw-link009”，然后将压缩包解压至该新建文件夹中；
    - 在电脑USB端口插上ST-Link设备，打开电脑设备管理器，找到``其他设备``或``通用串行总线控制器``，看是否有带感叹号的ST-Link标识，标识设备已识别出；
    - 鼠标右击该标识，点击``更新驱动程序``，然后点击``浏览我的电脑以查找驱动程序``，然后选中前面新创建的用于存放驱动程序的文件夹，点击确定即可完成安装。

## 新建工程

完成以上步骤之后，我们就可以开始新建一个工程了。

### 步骤1：新建目录

- 首先创建一个新目录作为项目的根目录，例如命名为``HelloWorld``；
- 在新目录下新创建四个文件夹，分别为：
    - ``/USER``（存放自己的项目主程序）;
    - ``/CORE``（存放核心文件和启动文件）;
    - ``/OBJ``（存放编译过程文件及hex文件）;
    - ``/STM32F10x_FWLIB``（存放库函数）；
- 双击``Keil uVision5``，点击``Project``->``New uVision Project...``，进入``HelloWorld/USER``文件夹，输入工程名称如``HelloWorld``，点击``保存``；
- 选择对应的STM32微控制器型号，例如本课教学所用的"STM32F103VE"型号；
- 在新弹出的窗口中直接点击``OK``，完成新工程创建。

当完成以上步骤后，可以打开``HelloWorld/USER``文件夹，其目录结构如下：

```plaintext
USER/
│── DebugConfig/
│   ├── Target_1_STM32F103VE_1.0.0.dbgconf
│── Listings/
│── Objects/
│── HelloWorld.uvoptx
│── HelloWorld
```

### 步骤2：文件准备

基于库函数开发，需要首先[下载固件库](#第-4-步下载固件库)。

- 复制库文件：找到固件库目录："STM32F10x_StdPeriph_Lib_v3.5.0/Libraries/STM32F10x_StdPeriph_Driver/"，复制其中的``/inc``和``/src``两个文件夹，然后回到项目根目录下的``/STM32F10x_FWLIB``文件夹下，鼠标右击粘贴至此；
- 复制核心文件：找到固件库目录："STM32F10x_StdPeriph_Lib_v3.5.0/Libraries/CMSIS/CM3/CoreSupport/"，复制其中的``core_cm3.c``和``core_cm3.h``两个文件夹，然后回到项目根目录下的``/CORE``文件夹下，鼠标右击粘贴至此；
- 复制启动文件：找到固件库目录："STM32F10x_StdPeriph_Lib_v3.5.0/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/arm/"，复制其中的``startup_stm32f10x_hd.s``（表示大容量，要根据具体型号选择对应的启动文件，[参考该链接](https://blog.csdn.net/AUST_xiaocizhen/article/details/136309026)）启动文件，然后回到项目根目录下的``/CORE``文件夹下，鼠标右击粘贴至此；
- 复制主目录文件：找到固件库目录："STM32F10x_StdPeriph_Lib_v3.5.0/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/"，复制其中的``stm32f10x.h``, ``system_stm32f10x.c``和``system_stm32f10x.h``三个文件，然后回到项目根目录下的``/USER``文件夹下，鼠标右击粘贴至此；
- 找到固件库目录："STM32F10x_StdPeriph_Lib_v3.5.0/Project/STM32F10x_StdPeriph_Template/"，复制其中的``main.c``, ``stm32f10x_conf.h``, ``stm32f10x_it.c``, ``stm32f10x_it.h``四个文件，然后回到项目根目录下的``/USER``文件夹下，鼠标右击粘贴至此。

至此，新建工程所需的所有文件就都复制到项目根目录下了。

### 步骤3：将文件导入工程中

由于步骤2复制的文件只是在工程的目录里，并没有被工程引用，因此工程中无法找到对应的文件。

- 打开``Keil uVision5``进入刚才创建的工程窗口，右击Target 1，选择``Manage Project Items...``；
- 可以修改``Target 1``名称为``HelloWorld``（非必须步骤）；
- 依次新建三个group，并导入文件：
    - USER：导入工程根目录下的"/USER"文件夹下的三个.c文件：``main.c``, ``stm32f10x_it.c``, ``system_stm32f10x.c``;
    - CORE：导入工程根目录下的"/CORE"文件夹下的``core_cm3.c``和``startup_stm32f10x_hd.s``文件（文件类型需要选择All files）;
    - FWLIB：导入工程根目录下的"/STM32F10x_FWLIB/src"文件夹下的所有.c文件。

### 步骤4：魔术棒配置

点击魔术棒按钮``Options for Target...``:

- 点击``Output``，将以下三个选项选中：``Debug Information``, ``Create Hex File``, ``Browse Information``;
- 点击``Select Folder for Objects...``，选择前面在项目根目录下创建的``/OBJ``文件夹。
- 点击``C/C++``选项卡，选择``Include Paths``，将所有放了头文件的路径都包含进去，本例程包含：``/CORE``, ``/STM32F10x_FWLIB/inc/``, ``/USER``；
- 添加宏定义：点击``C/C++``选项卡，在``Define``一栏输入字符串："STM32F10X_HD,USE_STDPERIPH_DRIVER"。
- 点击编译（或重写main.c后编译）。main.c程序可重写如下：
  ```c
  #include "stm32f10x.h"
  
  int main(void)
  {
    return 0;
  }
  ```
- 点击魔法棒，点击``Debug``选项卡，选择正确的下载调试工具，例如本课教学所用的为``ST-Link Debugger``，此外还要点击``Settings``选项卡，将``Target Com``下的``Port``选对，例如本科教学所用的开发板用的是"JTAG"端口，如果选错则无法下载程序。

### 步骤5：编译下载

完成以上四个步骤后，即可对程序进行编译下载。

- 点击左上角编译按钮，完成编译（如有错误，则编译不成功）；
- 点击``LOAD``按钮，开始下载。

## 运行第一个程序（点亮LED灯）

### 创建新工程

按照前面的步骤，[新建一个工程](#新建工程)，例如可以命名为："LED"。

### 编写main.c

点亮LED灯需要用到GPIO模块，因此，我们需要导入与之相关的库函数：

```c
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
```

接下来我们需要查看一下开发板上的LED小灯电路图，例如在本教学课程中，一共包含 LED0 和 LED1 两个小灯，分别和STM32芯片的 PB5 和 PE5 相连。引脚为高电平，LED小灯灭，引脚为低电平，LED小灯亮。

以LED0小灯为例，将PB5引脚初始化。初始化程序如下：

```c
int main(void)
{
    GPIO_InitTypeDef gpio_init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init.GPIO_Pin = GPIO_Pin_5;
    gpio_init.GPOI_Speed = GPOP_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_init);
    while(1)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    }
    return 0;
}
```

接下来编译程序，下载至STM32微控制器内，点击开发板上的Reset按钮，观察小灯现象。