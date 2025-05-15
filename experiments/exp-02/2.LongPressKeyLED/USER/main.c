#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"

void delay(u32 t)
{
	for(u32 i=0; i<t; i++){}
}

void delay_long(u32 t1, u32 t2)
{
	for(u32 i=0; i<t1; i++){
		for(u32 i=0; i<t2; i++){}
	}
}

void led_init(void)
{
	GPIO_InitTypeDef gpio_init_B;  	// ����PB�ڣ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ��PBʱ��
	gpio_init_B.GPIO_Mode = GPIO_Mode_Out_PP;  //�������ģʽ
	gpio_init_B.GPIO_Pin = GPIO_Pin_5;  // ѡ���5������
	gpio_init_B.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init_B);
}

u8 led_light(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == 1){  //���LED��Ϩ��״̬���������
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		return 1;}  //����ֵΪ1��ʾLED��Ϩ��״̬��������
	else{
		return 0;}  //����ֵΪ0��ʾLED�Ѿ�Ϊ����״̬��
}

u8 led_dark(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == 0){  //���LED�ǵ���״̬����Ϩ��
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		return 1;}  //����ֵΪ1��ʾLED�ɵ���״̬���ɹ�Ϩ��
	else{
		return 0;}  //����ֵΪ0��ʾLED�Ѿ�ΪϨ��״̬��
}


void key_init(void)
{
	GPIO_InitTypeDef gpio_init_E;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //ʹ��PEʱ��
	gpio_init_E.GPIO_Mode = GPIO_Mode_IPU;  // ��������ģʽ
	gpio_init_E.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
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

int main(void){
	led_init();
	key_init();
	u8 key_value = 0;
	while(1){
		// ѭ����ⰴ��
		if(key_press()){
			delay_long(100, 60000);  // ���Ƴ����ӳ�ʱ��
			if(key_press()){
				key_value = !key_value;
				if(key_value){
					led_light();
				}
				else{
					led_dark();
				}
				while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0);  // �ȴ������ͷ�
			}
		}
	}
	return 0;
}
