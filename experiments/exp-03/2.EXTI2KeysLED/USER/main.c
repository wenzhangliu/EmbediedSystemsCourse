#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"

u8 key_flag = 0;

void delay(u32 t)
{
	for(u32 i=0;i<t;i++){}
}

static void exti_init(void)  // �����ⲿ�ж�
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // ʹ��AFIOʱ��
	EXTI_InitTypeDef exti_init_PE4;
	exti_init_PE4.EXTI_Line = EXTI_Line4;  // ��������PE4���ţ��ⲿ�ж���4
	exti_init_PE4.EXTI_LineCmd = ENABLE;   // �ⲿ�ж�ʹ��
	exti_init_PE4.EXTI_Mode = EXTI_Mode_Interrupt;  // �ⲿ�ж�ģʽ
	exti_init_PE4.EXTI_Trigger = EXTI_Trigger_Falling;  //�½��ش����ж�
	EXTI_Init(&exti_init_PE4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);  //������������PE4
}

static void nvic_init(void)  //�����ж�
{
	NVIC_InitTypeDef nvic_init_PE4;
	nvic_init_PE4.NVIC_IRQChannel = EXTI4_IRQn;  // �ⲿ4�ж�
	nvic_init_PE4.NVIC_IRQChannelCmd = ENABLE;  // �ж�ʹ��
	nvic_init_PE4.NVIC_IRQChannelPreemptionPriority = 1;  // ��ռ���ȼ�1
	nvic_init_PE4.NVIC_IRQChannelSubPriority = 1;  // ��Ӧ���ȼ�1
	NVIC_Init(&nvic_init_PE4);
}

void led_init(void)
{
	GPIO_InitTypeDef gpio_init_B;  	// ����PB�ڣ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ��PBʱ��
	gpio_init_B.GPIO_Mode = GPIO_Mode_Out_PP;  //�������ģʽ
	gpio_init_B.GPIO_Pin = GPIO_Pin_5;
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

void EXTI4_IRQHandler(void)
{
	if(key_press() == 1)
	{
		if(!led_light()){  //С��״̬��תһ��
			led_dark();
		}
		while(key_press());
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
