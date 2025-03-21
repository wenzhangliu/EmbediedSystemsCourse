#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"

void delay(u32 t)
{
	for(u32 i=0; i<t; i++){}
}

int main(void){
	// 配置PB口（输出）
	GPIO_InitTypeDef gpio_init_B;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能PB时钟
	gpio_init_B.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出模式
	gpio_init_B.GPIO_Pin = GPIO_Pin_5;
	gpio_init_B.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init_B);
	// 配置PE口（输出）
	GPIO_InitTypeDef gpio_init_E;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //使能PB时钟
	gpio_init_E.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
	gpio_init_E.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOE, &gpio_init_E);
	u8 key_value = 0;
	while(1){
		// 循环检测按键
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))
		{
			delay(1000);
			if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)==0)
			{
				key_value = !key_value;
			}
		}
		if(key_value)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		}
			
	}
	return 0;
}