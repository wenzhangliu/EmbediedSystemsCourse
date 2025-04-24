#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

void tim2_init(void)  // 时钟2配置程序
{
	TIM_TimeBaseInitTypeDef tim_base_init;
	NVIC_InitTypeDef nvic_init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //TIM2时钟使能
	
	//计数器配置
	tim_base_init.TIM_Prescaler = 7200 - 1;  //不设预分频
	tim_base_init.TIM_Period = 10000 - 1;  //装载周期设置最大，不影响程序
	tim_base_init.TIM_ClockDivision = TIM_CKD_DIV1;  //时间分割值
	tim_base_init.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向：向上计数
	TIM_TimeBaseInit(TIM2, &tim_base_init);
	TIM_ETRClockMode1Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);  //定时器设置为下降沿触发计数
	
	//设置TIM2的更新中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //使能TIM2更新中断
	nvic_init.NVIC_IRQChannel = TIM2_IRQn;  //更新中断
	nvic_init.NVIC_IRQChannelCmd = ENABLE;  //打开中断通道
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0; //抢占式优先级
	nvic_init.NVIC_IRQChannelSubPriority = 0;  //响应式优先级
	NVIC_Init(&nvic_init);
}

void led_init(void)
{
	GPIO_InitTypeDef gpio_init_B;  	// 配置PB口（输出）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能PB时钟
	gpio_init_B.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出模式
	gpio_init_B.GPIO_Pin = GPIO_Pin_5;
	gpio_init_B.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init_B);
}

void key_init(void)
{
	GPIO_InitTypeDef gpio_init_E;  // 配置PE口为输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //使能PE时钟
	gpio_init_E.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
	gpio_init_E.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOE, &gpio_init_E);
}

u8 led_light(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == 1){  //如果LED是熄灭状态，则点亮。
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		return 1;}  //返回值为1表示LED由熄灭状态被点亮。
	else{
		return 0;}  //返回值为0表示LED已经为点亮状态。
}

u8 led_dark(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == 0){  //如果LED是点亮状态，则熄灭。
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		return 1;}  //返回值为1表示LED由点亮状态被成功熄灭。
	else{
		return 0;}  //返回值为0表示LED已经为熄灭状态。
}

void TIM2_IRQHandler(void)  //中断服务程序
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Trigger)){  //TIM2触发中断源
		led_dark();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志位
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