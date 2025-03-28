#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"

u8 key_flag = 0;

void delay(u32 t)
{
	for(u32 i=0;i<t;i++){}
}

static void exti_init(void)  // 配置外部中断
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 使能AFIO时钟
	EXTI_InitTypeDef exti_init_PE4;
	exti_init_PE4.EXTI_Line = EXTI_Line4;  // 按键连接PE4引脚，外部中断线4
	exti_init_PE4.EXTI_LineCmd = ENABLE;   // 外部中断使能
	exti_init_PE4.EXTI_Mode = EXTI_Mode_Interrupt;  // 外部中断模式
	exti_init_PE4.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发中断
	EXTI_Init(&exti_init_PE4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);  //配置连接引脚PE4
}

static void nvic_init(void)  //配置中断
{
	NVIC_InitTypeDef nvic_init_PE4;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  //配置中断组别0
	nvic_init_PE4.NVIC_IRQChannel = EXTI4_IRQn;  // 外部4中断
	nvic_init_PE4.NVIC_IRQChannelCmd = ENABLE;  // 中断使能
	nvic_init_PE4.NVIC_IRQChannelPreemptionPriority = 1;  // 抢占优先级1
	nvic_init_PE4.NVIC_IRQChannelSubPriority = 1;  // 响应优先级1
	NVIC_Init(&nvic_init_PE4);
}

void led_init(void)
{
	GPIO_InitTypeDef gpio_init_B;  	// 配置PB口（输出）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能PB时钟
	gpio_init_B.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出模式
	gpio_init_B.GPIO_Pin = GPIO_Pin_5;
	gpio_init_B.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init_B);
}

void led_light(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void led_dark(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void key_init(void)
{
	GPIO_InitTypeDef gpio_init_E;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //使能PE时钟
	gpio_init_E.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
	gpio_init_E.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOE, &gpio_init_E);
}

char key_press(void)
{
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
	{
		delay(1000);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0)
		{
			return 1;
		}
	}
	return 0;
}

void EXTI4_IRQHandler(void)
{
	if(key_press() == 1)
	{
		led_light();
	}
	else
	{
		led_dark();
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

int main(void){
	led_init();
	key_init();
	exti_init();
	nvic_init();
	led_dark();
	while(1);
	return 0;
}
