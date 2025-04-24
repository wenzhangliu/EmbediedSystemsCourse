#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

u16 period_class = 10;
u16 data_index = 0;
u16 data[110]={1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 22, 25, 28};

void tim2_init(u16 prescaler, u16 period)  // 时钟2配置程序
{
	TIM_TimeBaseInitTypeDef tim_base_init;
	TIM_OCInitTypeDef tim_oc_init;
	NVIC_InitTypeDef nvic_init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //TIM2时钟使能
	
	//设定定时器定时
	//计算公式：计数器频率=72MHz / prescaler；
	//定时时间 = period / 计数器频率 = prescaler * period / 72MHz
	tim_base_init.TIM_Prescaler = prescaler - 1;  //预分频（时钟分频）：prescaler
	tim_base_init.TIM_Period = period - 1;  //自动重装载寄存器周期的值（定时时间）累计period次产生一次中断；
	tim_base_init.TIM_CounterMode = TIM_CounterMode_Up;  //计数方向：向上计数
	TIM_TimeBaseInit(TIM2, &tim_base_init);
	
	//设置TIM2输出通道为PWM1模式
	tim_oc_init.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_init.TIM_OCPolarity = TIM_OCPolarity_Low;  //LED是给低电平点亮
	tim_oc_init.TIM_OutputState = TIM_OutputState_Enable;  //输出状态使能
	tim_oc_init.TIM_Pulse = 0;
	TIM_OC2Init(TIM2, &tim_oc_init);
	
	//设置TIM2的更新中断
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);  //清除TIM2更新中断标志
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  //使能TIM2更新中断
	nvic_init.NVIC_IRQChannel = TIM2_IRQn;  //更新中断
	nvic_init.NVIC_IRQChannelCmd = ENABLE;  //打开中断通道
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0; //抢占式优先级
	nvic_init.NVIC_IRQChannelSubPriority = 0;  //响应式优先级
	NVIC_Init(&nvic_init);
	
	TIM_Cmd(TIM2, ENABLE);  //定时器2工作使能
}

void TIM2_IRQHandler(void)  //中断服务程序
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		if(!--period_class){
			period_class = 10;  //每溢出10次就更改脉宽，保证每种PWM输出一段时间
			if(++data_index >= 110){
				data_index = 0;  //重新从起始数值设定脉宽
			}
			TIM_SetCompare2(TIM2, data[data_index]);  //设定脉宽比较值
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除中断标志位
	}
}

int main(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	tim2_init(200, 1024);  //定时0.5秒。
	while(1){}
	return 0;
}