#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

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
	
	usart_init.USART_BaudRate = 9600;  // ����������
	usart_init.USART_WordLength = USART_WordLength_8b;  // ���ݳ���
	usart_init.USART_StopBits = USART_StopBits_1;  // ֹͣλ
	usart_init.USART_Parity = USART_Parity_No;  // У��λ
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // Ӳ��������
	usart_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  // ģʽѡ��
	
	USART_Init(USART1, &usart_init);
	USART_Cmd(USART1, ENABLE);
}

void usart_send_byte(unsigned char data){
	USART_SendData(USART1, data);  // ��������
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  // �ȴ����ݷ������
}

unsigned char usart_recv_byte(void){
	while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));  // ��ѯ�Ƿ���յ�����
	return USART_ReceiveData(USART1);
}

int main(void){
	unsigned char data;
	usart_init();
	while(1){
		data = usart_recv_byte();  // ��ѯ��������
		usart_send_byte(data);  // ���յ�����������ͬ������
	}
	return 0;
}