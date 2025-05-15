#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"

void delay(u32 t)
{
	for(u32 i=0; i<t; i++){}
}

void delay_long(u32 t1, u32 t2)
{
	for(u32 i=0; i<t1; i++){
		for(u32 i=0; i<t2; i++){}
	}
}

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

int main(void){
	led_init();
	key_init();
	u8 key_value = 0;
	while(1){
		// 循环检测按键
		if(key_press()){
			delay_long(100, 60000);  // 控制长按延长时间
			if(key_press()){
				key_value = !key_value;
				if(key_value){
					led_light();
				}
				else{
					led_dark();
				}
				while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0);  // 等待按键释放
			}
		}
	}
	return 0;
}
