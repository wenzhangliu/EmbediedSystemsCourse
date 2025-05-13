#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

#define RX_BUF_SIZE 500  // ���ջ�������С

unsigned char rx_buf[RX_BUF_SIZE];  // ���ջ�����
uint32_t rx_buf_size=RX_BUF_SIZE;  // ��ǰ���ջ������Ĵ�С

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

unsigned char usart_recv_byte(void){
	while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));  // ��ѯ�Ƿ���յ�����
	return USART_ReceiveData(USART1);
}

void usart_send_byte(unsigned char data){
	USART_SendData(USART1, data);  // ��������
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  // �ȴ����ݷ������
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE)){  // ������յ�����
		if(rx_buf_size < RX_BUF_SIZE){
			rx_buf[rx_buf_size++] = usart_recv_byte();  // ���յ������ݷ��ڶ�����
		}
	}
}

int main(void){
	usart_init();
	while(1){
		if(rx_buf_size > 0){
			usart_send_byte(rx_buf[--rx_buf_size]);  // ͨ�����ڰѻ����������ݷ�����λ��
		}
	}
	return 0;
}