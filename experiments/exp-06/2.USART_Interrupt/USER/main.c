#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

void nvic_init(void){
	NVIC_InitTypeDef nvic_init;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  // ���ȼ�����0
	nvic_init.NVIC_IRQChannel = USART1_IRQn;  // USART1�жϺ�
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;  // ��ռ���ȼ�0
	nvic_init.NVIC_IRQChannelSubPriority = 0;  // ��Ӧ���ȼ�0
	NVIC_Init(&nvic_init);
}

void usart_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ�ܴ���GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	 //ʹ�ܴ�������ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef usart_init;
	
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	usart_init.USART_BaudRate = 115200;  // ����������
	usart_init.USART_WordLength = USART_WordLength_8b;  // ���ݳ���
	usart_init.USART_StopBits = USART_StopBits_1;  // ֹͣλ
	usart_init.USART_Parity = USART_Parity_No;  // У��λ
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // Ӳ��������
	usart_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  // ģʽѡ��
	
	USART_Init(USART1, &usart_init);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // ���������ж�
	nvic_init();
	USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE)){  // ������յ�����
		USART_SendData(USART1, USART_ReceiveData(USART1));  // ��������
		while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));  // �ȴ��������
	}
}

int main(void){
	usart_init();
	while(1);
	return 0;
}