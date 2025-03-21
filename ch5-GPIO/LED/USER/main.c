#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"

int main(void){
	GPIO_InitTypeDef gpio_init;
	//使能PB5时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//将PB5设置为推挽输出
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_5;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init);
	int delay = 1000000;
	while(1){
		//设置PB5为低电平，根据电路图LED灯亮
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		for(int i=0;i<delay;i++){}
		//设置PB5为高电平，根据电路图LED灯灭
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		for(int i=0;i<delay;i++){}
	}
	return 0;
}