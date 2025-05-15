#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

void delay(u32 t)
{
	for(u32 i=0; i<t; i++){}
}

void tim2_init(u16 prescaler, u16 period)  // 时钟2配置程序
{
	TIM_TimeBaseInitTypeDef tim_base_init;
	NVIC_InitTypeDef nvic_init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //TIM2时钟使能
	
	//设定定时器定时
	//计算公式：计数器频率=72MHz / prescaler；
	//定时时间 = period / 计数器频率 = prescaler * period / 72MHz
	TIM_TimeBaseStructInit(&tim_base_init);
	tim_base_init.TIM_Prescaler = prescaler - 1;  //预分频（时钟分频）：prescaler
	tim_base_init.TIM_Period = period - 1;  //自动重装载寄存器周期的值（定时时间）累计period次产生一次中断；
	tim_base_init.TIM_ClockDivision = TIM_CKD_DIV1;  //时间分割值
	tim_base_init.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向：向上计数
	TIM_TimeBaseInit(TIM2, &tim_base_init);
	
	//设置TIM2的更新中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //使能TIM2更新中断
	nvic_init.NVIC_IRQChannel = TIM2_IRQn;  //更新中断
	nvic_init.NVIC_IRQChannelCmd = ENABLE;  //打开中断通道
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0; //抢占式优先级
	nvic_init.NVIC_IRQChannelSubPriority = 2;  //响应式优先级
	NVIC_Init(&nvic_init);
	
	TIM_Cmd(TIM2, ENABLE);  //定时器2工作使能
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

void key_init(void)
{
	GPIO_InitTypeDef gpio_init_E;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //使能PE时钟
	gpio_init_E.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
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

void TIM2_IRQHandler(void)  //中断服务程序
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
		if(!led_light()){  //小灯状态翻转一次
			led_dark();
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志位
	}
}

int main(void){
	led_init();
	key_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	u16 blink_interval = 5000; // 5000表示0.5s
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
			// 设置新的ARR值
			TIM2->ARR = blink_interval - 1;
			while(key_press());
		}
	}
	return 0;
}