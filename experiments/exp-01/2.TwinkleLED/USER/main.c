#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"

void led_init(void)
{
	GPIO_InitTypeDef gpio_init_B;  	// 配置PB口（输出）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能PB时钟
	gpio_init_B.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出模式
	gpio_init_B.GPIO_Pin = GPIO_Pin_5;  // 选择第5个引脚
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

int main(void){
	led_init();  // 调用GPIO初始化程序
	int delay = 1000000;
	while(1){
		//设置PB5为低电平，根据电路图LED灯亮
		led_light();
		for(int i=0;i<delay;i++){}
		//设置PB5为高电平，根据电路图LED灯灭
		led_dark();
		for(int i=0;i<delay;i++){}
	}
	return 0;
}
