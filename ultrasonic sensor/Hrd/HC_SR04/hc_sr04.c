#include "hc_sr04.h"
#include "stm32f10x.h"
#include "Timer.h"
#include "delay.h"

u8 overcount=0; //��¼��ʱ���������

void HC_SR04_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//PA6Ϊ�����Trig   PA7Ϊ�����Echo
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//����Ϊ�������
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//����Ϊ��������
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

//���
float Senor_Using(void)
{
	float length = 0,sum = 0;
	u16 tim;
	unsigned int i = 0;
	//��������ݼ���һ��ƽ��ֵ
	while(i != 5)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_6); //�����źţ���Ϊ�����ź�
		delay_us(20); //�ߵ�ƽ�źų���10us
		GPIO_ResetBits(GPIOA,GPIO_Pin_6); //�ȴ������ź�
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) == RESET);
		//�����źŵ�����������ʱ������
		TIM_Cmd(TIM2,ENABLE);
		i += 1; //ÿ�յ�һ�λ����ź�+1,�յ�5�ξͼ����ֵ
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7) == SET);//�����ź���ʧ
		TIM_Cmd(TIM2,DISABLE);//�رն�ʱ��
		tim = TIM_GetCounter(TIM2); //��ȡ��TIM3���Ĵ����еļ���ֵ
		length = (tim+overcount*1000)/58.0; //ͨ�������źż������
		sum = length+sum;
		TIM2->CNT = 0; //��TIM3�����Ĵ����ļ���ֵ����
		overcount = 0; //�ж������������
		delay_ms(100);
	}
	length = sum/5;
	return length; //������Ϊ��������ֵ
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET) //����Ƿ���TIM3�ж�
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update ); //����жϸ��±�־
		overcount++;
	}
}