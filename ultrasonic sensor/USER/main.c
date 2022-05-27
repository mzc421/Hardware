#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "oled.h"
#include "show.h"
#include "led.h"
#include "hc_sr04.h"
#include "Timer.h"

float length;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	LED_Init();
	delay_init();
	Timer2_Init(999,71);//100Hz
	uart_init(9600);//����1��ʼ��

	OLED_Init();  //OLED��ʼ��

	HC_SR04_Init(); //������ PA6Ϊ�����Trig   PA7Ϊ�����Echo
		
  while(1)	
	{
		LED0 =~ LED0;
		length = Senor_Using();
		//������ʾ
		printf("��õľ���Ϊ��%.3f",length);
		//OLED��ʾ
		oled_show();
		
//		GPIO_ResetBits(GPIOA,GPIO_Pin_8);//��PA8��0	
//		delay_ms(800);
//		GPIO_SetBits(GPIOA,GPIO_Pin_8);//��PA8��1
//		delay_ms(800);
	}
}
