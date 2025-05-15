#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

void usart_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能串口GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	 //使能串口外设时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef usart_init;
	
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	usart_init.USART_BaudRate = 9600;  // 波特率设置
	usart_init.USART_WordLength = USART_WordLength_8b;  // 数据长度
	usart_init.USART_StopBits = USART_StopBits_1;  // 停止位
	usart_init.USART_Parity = USART_Parity_No;  // 校验位
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 硬件流控制
	usart_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  // 模式选择
	
	USART_Init(USART1, &usart_init);
	USART_Cmd(USART1, ENABLE);
}

void usart_send_byte(unsigned char data){
	USART_SendData(USART1, data);  // 发送数据
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  // 等待数据发送完成
}

unsigned char usart_recv_byte(void){
	while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));  // 查询是否接收到数据
	return USART_ReceiveData(USART1);
}

int main(void){
	unsigned char data;
	usart_init();
	while(1){
		data = usart_recv_byte();  // 查询接收数据
		usart_send_byte(data);  // 接收到数据则发送相同的数据
	}
	return 0;
}