
#include "delay.h"
extern void Delayxus(u32 nus)
{
	u32 temp;
	SysTick->LOAD = 9*nus;//Systickʱ��ԴΪ8��Ƶ 72M/8=9M
	SysTick->VAL = 0x00;//�����Ĵ�������
	SysTick->CTRL = 0x01;//Systick������ʹ�� ʱ��ԴΪHCLK/8
	do
	{
		temp = SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));//�жϱ�־λ��ʹ��λ
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0x00;
}
/********************************************************************
*��    ��:Delay_nms(u32 nms)
*��    ��:��ʱn���� ���ֵ:18642
*��ڲ���:nms
*���ڲ���:��
*˵    ��:��
*���÷�������
********************************************************************/
extern void Delayxms(u32 nms)
{
	u32 temp;
	SysTick->LOAD = 9000*nms;//Systickʱ��ԴΪ8��Ƶ 72M/8=9M
	SysTick->VAL = 0x00;//�����Ĵ�������
	SysTick->CTRL = 0x01;//Systick���������� ʱ��ԴΪHCLK/8
	do
	{
		temp = SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));//�жϱ�־λ��ʹ��λ
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0x00;
}
