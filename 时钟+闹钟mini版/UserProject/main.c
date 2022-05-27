#include "stm32f10x.h"
#include "Key.h"
#include "Led.h"
#include "delay.h"
#include "smg.h"
#include "timer.h"
#include "delay.h"
#include "beef.h"

u8 sec,min,hour;//ʱ��
u8 h,m,s;//����
u8 setflag;//ʱ������
u8 a = 0;
u16 count;
u8 countflag;//��������ֵ
u8 flag;//��������

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Timer3_Init(9999,7199);
	Timer2_Init(9999,7199);
	LED_Init();
	Key_Init();
	delay_init();
	Smg_Init();
	
	hour = 13;
	min = 19;
	sec = 13;
	
	h = 0;
	m = 0;
	s = 0;
	
	GPIO_SetBits(GPIOD,GPIO_Pin_2);//�� LED1
	
	while(1)
	{		
		//ʵʱʱ����ʾ
		Smg_DisPlay();
		Display_Data(hour,min,sec);
		
		
		//ʱ������
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)//WK_UP
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
			{
				while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)));
				setflag = 1;
				TIM_Cmd(TIM3,DISABLE);
			}
		}
		while(setflag)
		{
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
			{
				//�˳�
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
				{
					while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)));
					setflag = 0;
					TIM_Cmd(TIM3,ENABLE);
					break;
				}
			}
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)//KEY1 hour
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)));
					hour++;
					if(hour == 24)
						hour = 0;
				}
			}
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)//KEY0 min
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)));
					min++;
					if(min == 60)
						min = 0;
				}
			}
			Smg_DisPlay();
			Display_Data(hour,min,sec);
		}
	
		//��������
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)//KEY0
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)
			{
				while(!(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)));
				countflag = 1;//������������ģʽ
				if(a == 0)
				{
					a = 1;
					TIM_Cmd(TIM2,DISABLE);
				}
				else
				{
					a = 0;
					countflag = 0;//�˳���ʱģʽ
					TIM_Cmd(TIM2,ENABLE);//������ʱ��ֹͣ��ʱ
					count = 0;
				}
			}
		}
		while(countflag)
		{
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)//KEY0
		  {
				//�˳���������ģʽ ����ʱ������ģʽ
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)));
					countflag = 0;
					TIM_Cmd(TIM2,ENABLE);//������ʱ��2��ʱ
					flag = 1;
					break;
				}
		  }
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)//KEY1 h
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)));
					m++;
					if(m == 60)
						m = 0;
				}
			}
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)//WK_UP m
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
				{
					while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)));
					h++;
					if(h == 24)
						h = 0;
				}
			}
			Smg_DisPlay();
			Display_Data(h,m,s);
		}
		
		while(flag)
		{
			Smg_DisPlay();
			Display_Data(hour,min,sec);
			//����
			if(count == CountTime(h,m))
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);//��
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)//KEY1
				{
					delay_ms(10);
					if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
					{
						while(!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)));
						GPIO_SetBits(GPIOD,GPIO_Pin_2);//�� LED1
						TIM_Cmd(TIM2,DISABLE);
						flag = 0;
						break;
					}
				}
			}
		}
		
	}
	return 0;
}