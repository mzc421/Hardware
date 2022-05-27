#include "show.h"
#include "oled.h"

extern float length;
//extern�����ڱ������ߺ���ǰ���Ա�ʾ�������ߺ����Ķ����ڱ���ļ��У���ʾ�����������˱����ͺ���ʱ������ģ����Ѱ���䶨�塣
//������Ҫע�⣬���� extern �����������Զ�Σ�������ֻ��һ�Ρ�

void oled_show(void)
{
	int int_length,decimal_lenght;
	int_length = (uint16_t)length;
	decimal_lenght = length * 1000 - int_length * 1000;	
	
	OLED_ShowString(00,00,"HCSR:");
	OLED_ShowNum(37,00,int_length,4,12);
	OLED_ShowString(63,00,".");
	OLED_ShowNum(69,00,decimal_lenght,3,12);
	OLED_ShowString(88,00,"cm");	
}
