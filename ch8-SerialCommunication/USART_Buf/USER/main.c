#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

#define RX_BUF_SIZE 500  // 接收缓冲区大小

unsigned char rx_buf[RX_BUF_SIZE];  // 接收缓冲区
uint32_t rx_buf_size=RX_BUF_SIZE;  // 当前接收缓冲区的大小

void nvic_init(void){
	NVIC_InitTypeDef nvic_init;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  // 优先级分组0
	nvic_init.NVIC_IRQChannel = USART1_IRQn;  // USART1中断号
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;  // 抢占优先级0
	nvic_init.NVIC_IRQChannelSubPriority = 0;  // 响应优先级0
	NVIC_Init(&nvic_init);
}

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
	
	usart_init.USART_BaudRate = 115200;  // 波特率设置
	usart_init.USART_WordLength = USART_WordLength_8b;  // 数据长度
	usart_init.USART_StopBits = USART_StopBits_1;  // 停止位
	usart_init.USART_Parity = USART_Parity_No;  // 校验位
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 硬件流控制
	usart_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  // 模式选择
	
	USART_Init(USART1, &usart_init);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // 启动接收中断
	nvic_init();
	USART_Cmd(USART1, ENABLE);
}

unsigned char usart_recv_byte(void){
	while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));  // 查询是否接收到数据
	return USART_ReceiveData(USART1);
}

void usart_send_byte(unsigned char data){
	USART_SendData(USART1, data);  // 发送数据
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  // 等待数据发送完成
}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE)){  // 如果接收到数据
		if(rx_buf_size < RX_BUF_SIZE){
			rx_buf[rx_buf_size++] = usart_recv_byte();  // 接收到的数据放在队列中
		}
	}
}

int main(void){
	usart_init();
	while(1){
		if(rx_buf_size > 0){
			usart_send_byte(rx_buf[--rx_buf_size]);  // 通过串口把缓冲区的数据发回上位机
		}
	}
	return 0;
}