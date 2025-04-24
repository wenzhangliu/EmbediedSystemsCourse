#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

u16 period_class = 10;
u16 data_index = 0;
u16 data[110]={1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 22, 25, 28};

void tim2_init(u16 prescaler, u16 period)  // ʱ��2���ó���
{
	TIM_TimeBaseInitTypeDef tim_base_init;
	TIM_OCInitTypeDef tim_oc_init;
	NVIC_InitTypeDef nvic_init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //TIM2ʱ��ʹ��
	
	//�趨��ʱ����ʱ
	//���㹫ʽ��������Ƶ��=72MHz / prescaler��
	//��ʱʱ�� = period / ������Ƶ�� = prescaler * period / 72MHz
	tim_base_init.TIM_Prescaler = prescaler - 1;  //Ԥ��Ƶ��ʱ�ӷ�Ƶ����prescaler
	tim_base_init.TIM_Period = period - 1;  //�Զ���װ�ؼĴ������ڵ�ֵ����ʱʱ�䣩�ۼ�period�β���һ���жϣ�
	tim_base_init.TIM_CounterMode = TIM_CounterMode_Up;  //�����������ϼ���
	TIM_TimeBaseInit(TIM2, &tim_base_init);
	
	//����TIM2���ͨ��ΪPWM1ģʽ
	tim_oc_init.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_init.TIM_OCPolarity = TIM_OCPolarity_Low;  //LED�Ǹ��͵�ƽ����
	tim_oc_init.TIM_OutputState = TIM_OutputState_Enable;  //���״̬ʹ��
	tim_oc_init.TIM_Pulse = 0;
	TIM_OC2Init(TIM2, &tim_oc_init);
	
	//����TIM2�ĸ����ж�
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);  //���TIM2�����жϱ�־
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //ʹ��TIM2�����ж�
	nvic_init.NVIC_IRQChannel = TIM2_IRQn;  //�����ж�
	nvic_init.NVIC_IRQChannelCmd = ENABLE;  //���ж�ͨ��
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0; //��ռʽ���ȼ�
	nvic_init.NVIC_IRQChannelSubPriority = 0;  //��Ӧʽ���ȼ�
	NVIC_Init(&nvic_init);
	
	TIM_Cmd(TIM2, ENABLE);  //��ʱ��2����ʹ��
}

void TIM2_IRQHandler(void)  //�жϷ������
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		if(!--period_class){
			period_class = 10;  //ÿ���10�ξ͸���������֤ÿ��PWM���һ��ʱ��
			if(++data_index >= 110){
				data_index = 0;  //���´���ʼ��ֵ�趨����
			}
			TIM_SetCompare2(TIM2, data[data_index]);  //�趨����Ƚ�ֵ
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //����жϱ�־λ
	}
}

int main(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	tim2_init(200, 1024);  //��ʱ0.5�롣
	while(1){}
	return 0;
}