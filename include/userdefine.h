 #ifndef _USERDEFINE_H
 #define _USERDEFINE_H
 
 
 
 


//------------------------------------------------------------------------------------------------��������ͷ�ļ�
 	#include"delay.h"
	#include"interrupt.h"
	#include"led.h"
	#include"timer.h"
	#include"uart.h"
	#include"adc.h"
	#include"pwm.h"
	#include"task.h"
	#include "PID.h"
	#include"Modbus.h"
	#include"ird_remote.h"
	#include"key.h"
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------

#define BIT0  0
#define BIT1  1
#define BIT2  2
#define BIT3  3
#define BIT4  4
#define BIT5  5
#define BIT6  6
#define BIT7  7



#define MAC_ADDR   0x55         //����ң���豸��ַ


#define _XTAL_FREQ	32000000





#define PERSON_CHECK1_TRIS        TRISAbits.TRISA7
#define PERSON_CHECK2_TRIS        TRISCbits.TRISC1

#define PERSON_CHECK1			 RA7
#define PERSON_CHECK2			 RC1

#define LOAD_OUT1_TRIS			 TRISCbits.TRISC3
#define LOAD_OUT2_TRIS			 TRISCbits.TRISC0

#define LOAD_OUT1_ENABLE		(LATCbits.LATC3 = 0x01) 		// LED����1���
#define LOAD_OUT1_DISABLE		(LATCbits.LATC3 = 0x00) 		// LED����1�ض�

#define LOAD_OUT2_ENABLE		(LATCbits.LATC0 = 0x01) 		// LED����2���
#define LOAD_OUT2_DISABLE		(LATCbits.LATC0 = 0x00) 		// LED����2�ض�

#define DL_1_TRIS			TRISBbits.TRISB1				//��һ·��·�жϼ��
#define DL_2_TRIS			TRISBbits.TRISB0				//�ڶ�·��·�жϼ��



#define POWER_TRIS         TRISAbits.TRISA6
#define POWER_ON		  (LATAbits.LATA6 = 0x01)
#define POWER_OFF		  (LATAbits.LATA6 = 0x00)


#endif