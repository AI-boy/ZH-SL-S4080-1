/******************************************************************************
**                           ־����ҵ���޹�˾
**
**                           
**
**--------------�����ļ���Ϣ---------------------------------------------------
** ��   ��   ��	: delay.c
** ��ǰ    �汾	: V1.0 
** ��        �� : 
** ��   ��   �� : xc8
** Ӳ        �� : 
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ��ʼ��������	: 2017-12-11
** ��   ��   ��	: ��ά��
** ��  	     �� : V1.0
** �衡      �� : 

**-----------------------------------------------------------------------------
*******************************************************************************/

//**--------------ͷ�ļ�-------------------------------------------------------
#include <xc.h>
#include <GenericTypeDefs.h>

//**--------------ȫ�ֱ���-----------------------------------------------------

//**--------------��������-----------------------------------------------------

//**--------------��������-----------------------------------------------------
/******************************************************************************
** �������� : delay_1us
** ������� : ��
** ������� : ��
** �������� : ��ʱ1us
** ȫ�ֱ��� : ��
** ��    �� : ��ά��
** ��    �� : 2017-12-11
** ��    �� : ��
** ��    �� : V0.1
*******************************************************************************/
void delay_1us(void) // ��ʱ1us����Ϊϵͳʱ����32MHZ��һ��ָ��ʱ����1/32M * 4 = 125ns = 0.125us
{
	NOP(); // 0.125us
	NOP(); // 0.125us
	NOP(); // 0.125us
	NOP(); // 0.125us
	NOP(); // 0.125us
	NOP(); // 0.125us
	NOP(); // 0.125us
	NOP(); // 0.125us
}

/******************************************************************************
** �������� : delay_ms
** ������� : count
** ������� : ��
** �������� : ������ʱ
** ȫ�ֱ��� : ��
** ��    �� : ��ά��
** ��    �� : 2017-12-11
** ��    �� : ��
** ��    �� : V0.1
*******************************************************************************/
void delay_ms(WORD count)
{
	WORD i, j;
	
	for (i = 0; i < count; i++)
	{
    	for (j = 0; j < 1000; j++)
    	{
    		NOP(); // 0.125us
			NOP(); // 0.125us
			NOP(); // 0.125us
			NOP(); // 0.125us
			NOP(); // 0.125us
			NOP(); // 0.125us
			NOP(); // 0.125us
			NOP(); // 0.125us
    	}
   	}
}