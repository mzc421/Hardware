#ifndef __SMG_H
#define __SMG_H			

#include "stm32f10x.h"

//#define RCLK PBout(10)//ʱ�������źš�����������Ч
//#define SCLK PBout(11)//�����źš���������������Ч
//#define DIO  PAout(8)//������������

void Smg_Init(void);
void Smg_DisPlay(void);
void Display_Data(u8 hour,u8 min,u8 sec);// Smg��ʾ
void Smg_OUT(u8 X);// Smg���ֽڴ�����λ����

#endif 