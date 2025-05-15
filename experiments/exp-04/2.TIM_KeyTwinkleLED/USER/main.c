#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

void delay(u32 t)
{
	for(u32 i=0; i<t; i++){}
}

void tim2_init(u16 prescaler, u16 period)  // ʱ��2���ó���
{
	TIM_TimeBaseInitTypeDef tim_base_init;
	NVIC_InitTypeDef nvic_init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //TIM2ʱ��ʹ��
	
	//�趨��ʱ����ʱ
	//���㹫ʽ��������Ƶ��=72MHz / prescaler��
	//��ʱʱ�� = period / ������Ƶ�� = prescaler * period / 72MHz
	TIM_TimeBaseStructInit(&tim_base_init);
	tim_base_init.TIM_Prescaler = prescaler - 1;  //Ԥ��Ƶ��ʱ�ӷ�Ƶ����prescaler
	tim_base_init.TIM_Period = period - 1;  //�Զ���װ�ؼĴ������ڵ�ֵ����ʱʱ�䣩�ۼ�period�β���һ���жϣ�
	tim_base_init.TIM_ClockDivision = TIM_CKD_DIV1;  //ʱ��ָ�ֵ
	tim_base_init.TIM_CounterMode = TIM_CounterMode_Up;  //�����������ϼ���
	TIM_TimeBaseInit(TIM2, &tim_base_init);
	
	//����TIM2�ĸ����ж�
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //ʹ��TIM2�����ж�
	nvic_init.NVIC_IRQChannel = TIM2_IRQn;  //�����ж�
	nvic_init.NVIC_IRQChannelCmd = ENABLE;  //���ж�ͨ��
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0; //��ռʽ���ȼ�
	nvic_init.NVIC_IRQChannelSubPriority = 2;  //��Ӧʽ���ȼ�
	NVIC_Init(&nvic_init);
	
	TIM_Cmd(TIM2, ENABLE);  //��ʱ��2����ʹ��
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

void TIM2_IRQHandler(void)  //�жϷ������
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
		if(!led_light()){  //С��״̬��תһ��
			led_dark();
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //����жϱ�־λ
	}
}

int main(void){
	led_init();
	key_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	u16 blink_interval = 5000; // 5000��ʾ0.5s
	tim2_init(7200, blink_interval);
	led_light();
	u8 speed_mode = 0;
	while(1){
		if(key_press()){
			speed_mode = (speed_mode + 1) % 3;
			switch(speed_mode){
				case 0: blink_interval = 5000; break;
				case 1: blink_interval = 2500; break;
				case 2: blink_interval = 1000; break;
			}
			// �����µ�ARRֵ
			TIM2->ARR = blink_interval - 1;
			while(key_press());
		}
	}
	return 0;
}