#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"

void led_init(void)
{
	GPIO_InitTypeDef gpio_init_B;  	// ����PB�ڣ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ��PBʱ��
	gpio_init_B.GPIO_Mode = GPIO_Mode_Out_PP;  //�������ģʽ
	gpio_init_B.GPIO_Pin = GPIO_Pin_5;  // ѡ���5������
	gpio_init_B.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init_B);
}

u8 led_light(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == 1){  //���LED��Ϩ��״̬���������
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		return 1;}  //����ֵΪ1��ʾLED��Ϩ��״̬��������
	else{
		return 0;}  //����ֵΪ0��ʾLED�Ѿ�Ϊ����״̬��
}

u8 led_dark(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == 0){  //���LED�ǵ���״̬����Ϩ��
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		return 1;}  //����ֵΪ1��ʾLED�ɵ���״̬���ɹ�Ϩ��
	else{
		return 0;}  //����ֵΪ0��ʾLED�Ѿ�ΪϨ��״̬��
}

int main(void){
	led_init();  // ����GPIO��ʼ������
	int delay = 1000000;
	while(1){
		//����PB5Ϊ�͵�ƽ�����ݵ�·ͼLED����
		led_light();
		for(int i=0;i<delay;i++){}
		//����PB5Ϊ�ߵ�ƽ�����ݵ�·ͼLED����
		led_dark();
		for(int i=0;i<delay;i++){}
	}
	return 0;
}
