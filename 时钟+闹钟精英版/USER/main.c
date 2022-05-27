#include "stm32f10x.h"
#include "Key.h"
#include "Led.h"
#include "delay.h"
#include "smg.h"
#include "timer.h"
#include "delay.h"
#include "beep.h"

u8 sec,min,hour;//ʱ��
u8 h,m,s;//����
u8 setflag;//ʱ������
u8 a = 0;
u16 count;
u8 countflag;//��������
u8 flag;//����

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Timer3_Init(9999,7199);
	Timer2_Init(9999,7199);
	LED_Init();
	Key_Init();
	delay_init();
	Smg_Init();
	Beep_Init();
	
	hour = 0;
	min = 0;
	sec = 0;
	
	h = 0;
	m = 0;
	s = 0;
	
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
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);//�� LED0
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
			{
				//�˳�
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
				{
					while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)));
					setflag = 0;
					TIM_Cmd(TIM3,ENABLE);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);//��
					break;
				}
			}
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)//KEY1 hour
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)));
					hour++;
					if(hour == 24)
						hour = 0;
				}
			}
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)//KEY0 min
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)));
					min++;
					if(min == 60)
						min = 0;
				}
			}
			Smg_DisPlay();
			Display_Data(hour,min,sec);
		}
	
		//��������
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)//KEY0
		{
			delay_ms(10);
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
			{
				while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)));
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
					TIM_Cmd(TIM2,ENABLE);//������ʱ��
					count = 0;
				}
			}
		}
		while(countflag)
		{
			GPIO_ResetBits(GPIOE,GPIO_Pin_5);//�� LED1
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)//KEY0
		  {
				//�˳���������ģʽ ����ʱ������ģʽ
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)));
					countflag = 0;
					flag = 1;
					TIM_Cmd(TIM2,ENABLE);//������ʱ��2��ʱ
					GPIO_SetBits(GPIOE,GPIO_Pin_5);//��
					break;
				}
		  }
			if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)//KEY1 h
			{
				delay_ms(10);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
				{
					while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)));
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
		
		//����
		while(flag)
		{
			Smg_DisPlay();
			Display_Data(hour,min,sec);
			if(count == CountTime(h,m))
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_8);//��
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);//LED0 ��
				delay_ms(100);
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)//KEY1
				{
					delay_ms(10);
					if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
					{
						while(!(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)));
						flag = 0;
						GPIO_ResetBits(GPIOB,GPIO_Pin_8);//��
						GPIO_SetBits(GPIOB,GPIO_Pin_5);//�� LED0
						TIM_Cmd(TIM2,DISABLE);
						break;
					}
				}
			}
		}
	}
	return 0;
}