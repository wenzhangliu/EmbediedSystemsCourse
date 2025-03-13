#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"

int main(void){
	GPIO_InitTypeDef gpio_init;
	//ʹ��PB5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//��PB5����Ϊ�������
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_5;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init);
	int delay = 1000000;
	while(1){
		//����PB5Ϊ�͵�ƽ�����ݵ�·ͼLED����
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		for(int i=0;i<delay;i++){}
		//����PB5Ϊ�ߵ�ƽ�����ݵ�·ͼLED����
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		for(int i=0;i<delay;i++){}
	}
	return 0;
}