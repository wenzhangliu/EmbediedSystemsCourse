#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"

void delay(u32 t)
{
	for(u32 i=0;i<t;i++){}
}

static void exti_init(void)  // 配置外部中断
{
	EXTI_InitTypeDef exti_init;
	exti_init.EXTI_Line = EXTI_Line4;  // 按键连接PE4引脚，外部中断线4
	exti_init.EXTI_LineCmd = ENABLE;   // 外部中断使能
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt;  // 外部中断模式
	exti_init.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发中断
	EXTI_Init(&exti_init);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);  //配置连接引脚PE4
}

static void nvic_init(void)  //配置中断
{
	NVIC_InitTypeDef nvic_init;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //配置中断组别1
	nvic_init.NVIC_IRQChannel = EXTI4_IRQn;  // 外部4中断
	nvic_init.NVIC_IRQChannelCmd = ENABLE;  // 中断使能
	nvic_init.NVIC_IRQChannelPreemptionPriority = 2;  // 抢占优先级2
	nvic_init.NVIC_IRQChannelSubPriority = 1;  // 响应优先级1
	NVIC_Init(&nvic_init);
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

void EXTI4_IRQHandler(void)
{
	led_light();
	EXTI_ClearITPendingBit(EXTI_Line4);
}

int main(void){
	led_init();
	key_init();
	exti_init();
	nvic_init();
	led_dark();
	while(1)
	{
		
	}
	return 0;
}
