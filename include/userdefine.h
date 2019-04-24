 #ifndef _USERDEFINE_H
 #define _USERDEFINE_H
 
 
 
 


//------------------------------------------------------------------------------------------------包含工程头文件
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



#define MAC_ADDR   0x55         //红外遥控设备地址


#define _XTAL_FREQ	32000000





#define PERSON_CHECK1_TRIS        TRISAbits.TRISA7
#define PERSON_CHECK2_TRIS        TRISCbits.TRISC1

#define PERSON_CHECK1			 RA7
#define PERSON_CHECK2			 RC1

#define LOAD_OUT1_TRIS			 TRISCbits.TRISC3
#define LOAD_OUT2_TRIS			 TRISCbits.TRISC0

#define LOAD_OUT1_ENABLE		(LATCbits.LATC3 = 0x01) 		// LED负载1输出
#define LOAD_OUT1_DISABLE		(LATCbits.LATC3 = 0x00) 		// LED负载1关断

#define LOAD_OUT2_ENABLE		(LATCbits.LATC0 = 0x01) 		// LED负载2输出
#define LOAD_OUT2_DISABLE		(LATCbits.LATC0 = 0x00) 		// LED负载2关断

#define DL_1_TRIS			TRISBbits.TRISB1				//第一路短路中断检测
#define DL_2_TRIS			TRISBbits.TRISB0				//第二路短路中断检测



#define POWER_TRIS         TRISAbits.TRISA6
#define POWER_ON		  (LATAbits.LATA6 = 0x01)
#define POWER_OFF		  (LATAbits.LATA6 = 0x00)


#endif