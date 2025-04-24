#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

void tim2_init(void)  // ʱ��2���ó���
{
	TIM_TimeBaseInitTypeDef tim_base_init;
	NVIC_InitTypeDef nvic_init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //TIM2ʱ��ʹ��
	
	//����������
	tim_base_init.TIM_Prescaler = 7200 - 1;  //����Ԥ��Ƶ
	tim_base_init.TIM_Period = 10000 - 1;  //װ������������󣬲�Ӱ�����
	tim_base_init.TIM_ClockDivision = TIM_CKD_DIV1;  //ʱ��ָ�ֵ
	tim_base_init.TIM_CounterMode = TIM_CounterMode_Up;  //�����������ϼ���
	TIM_TimeBaseInit(TIM2, &tim_base_init);
	TIM_ETRClockMode1Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);  //��ʱ������Ϊ�½��ش�������
	
	//����TIM2�ĸ����ж�
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //ʹ��TIM2�����ж�
	nvic_init.NVIC_IRQChannel = TIM2_IRQn;  //�����ж�
	nvic_init.NVIC_IRQChannelCmd = ENABLE;  //���ж�ͨ��
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0; //��ռʽ���ȼ�
	nvic_init.NVIC_IRQChannelSubPriority = 0;  //��Ӧʽ���ȼ�
	NVIC_Init(&nvic_init);
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

void key_init(void)
{
	GPIO_InitTypeDef gpio_init_E;  // ����PE��Ϊ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //ʹ��PEʱ��
	gpio_init_E.GPIO_Mode = GPIO_Mode_IPU;  // ��������ģʽ
	gpio_init_E.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOE, &gpio_init_E);
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

void TIM2_IRQHandler(void)  //�жϷ������
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Trigger)){  //TIM2�����ж�Դ
		led_dark();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //����жϱ�־λ
	}
}

int main(void){
	led_init();
	key_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//tim2_init();
	led_dark();
	u16 count = 2;
	while(1){
		if(TIM_GetCounter(TIM2) > 2){
			led_light();
		}
	}
	return 0;
}