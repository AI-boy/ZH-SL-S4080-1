/******************************************************************************
**                           ־����ҵ���޹�˾
**
**                           
**
**--------------�����ļ���Ϣ---------------------------------------------------
** ��   ��   ��	: main.c
** ��ǰ    �汾	: V1.0 
** ��        �� : ̫����·�ƿ�����������
** ��   ��   �� : xc8
** Ӳ        �� : ̫����·�ƿ�����
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

#include <stdio.h>
#include <string.h>

#include"userdefine.h"


 




//**--------------����λ(ֻ���PICϵ�е�Ƭ��)----------------------------------

//Register: CONFIG1 @ 0x8007

#pragma config   CPD =    OFF		 //Data Memory Code Protection 
									 //OFF Data memory code protection is disabled 
									 //ON Data memory code protection is enabled 
									
#pragma config   BOREN = ON
									//Brown-out Reset Enable 
									//OFF Brown-out Reset disabled 
									//ON Brown-out Reset enabled 
									//NSLEEP Brown-out Reset enabled while running and disabled in Sleep 
									//SBODEN Brown-out Reset controlled by the SBOREN bit in the BORCON register 

#pragma config   IESO = OFF 
									//Internal/External Switchover 
									//OFF Internal/External Switchover mode is disabled 
									//ON Internal/External Switchover mode is enabled 

#pragma config   FOSC = INTOSC
									//Oscillator Selection 
									//ECM ECM, External Clock, Medium Power Mode (0.5-4 MHz): device clock supplied to CLKIN pin 
									//XT XT Oscillator, Crystal/resonator connected between OSC1 and OSC2 pins 
									//LP LP Oscillator, Low-power crystal connected between OSC1 and OSC2 pins 
									//EXTRC EXTRC oscillator: External RC circuit connected to CLKIN pin 
									//ECH ECH, External Clock, High Power Mode (4-32 MHz): device clock supplied to CLKIN pin 
									//ECL ECL, External Clock, Low Power Mode (0-0.5 MHz): device clock supplied to CLKIN pin 
									//INTOSC INTOSC oscillator: I/O function on CLKIN pin 
									//HS HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins 

#pragma config   FCMEN = OFF
									//Fail-Safe Clock Monitor Enable 
									//OFF Fail-Safe Clock Monitor is disabled 
									//ON Fail-Safe Clock Monitor is enabled 

#pragma config   MCLRE = ON
									//MCLR Pin Function Select 
									//OFF MCLR/VPP pin function is digital input 
									//ON MCLR/VPP pin function is MCLR 

#pragma config   WDTE = NSLEEP
									//Watchdog Timer Enable 
									//OFF WDT disabled 
									//ON WDT enabled 
									//NSLEEP WDT enabled while running and disabled in Sleep 
									//SWDTEN WDT controlled by the SWDTEN bit in the WDTCON register 

#pragma config   CP = OFF
									//Flash Program Memory Code Protection 
									//OFF Program memory code protection is disabled 
									//ON Program memory code protection is enabled 

#pragma config   PWRTE = OFF
									//Power-up Timer Enable 
									//OFF PWRT disabled 
									//ON PWRT enabled 

#pragma config   CLKOUTEN = OFF
									//Clock Out Enable 
									//OFF CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin 
									//ON CLKOUT function is enabled on the CLKOUT pin 

//----------------------------------Register: CONFIG2 @ 0x8008

#pragma config   PLLEN = ON
									//PLL Enable 
									//OFF 4x PLL disabled 
									//ON 4x PLL enabled 

#pragma config   WRT = OFF
									//Flash Memory Self-Write Protection 
									//OFF Write protection off 
									//BOOT 000h to 1FFh write protected, 200h to 3FFFh may be modified by EECON control 
									//HALF 000h to 1FFFh write protected, 2000h to 3FFFh may be modified by EECON control 
									//ALL 000h to 3FFFh write protected, no addresses may be modified by EECON control 

#pragma config   STVREN = ON
									//Stack Overflow/Underflow Reset Enable 
									//OFF Stack Overflow or Underflow will not cause a Reset 
									//ON Stack Overflow or Underflow will cause a Reset 

#pragma config   BORV = LO
									//Brown-out Reset Voltage Selection 
									//LO Brown-out Reset Voltage (Vbor), low trip point selected. 
									//HI Brown-out Reset Voltage (Vbor), high trip point selected. 

#pragma config   VCAPEN = OFF
									//Voltage Regulator Capacitor Enable 
									//RA5 VCAP functionality is enabled on RA5 
									//OFF All VCAP pin functionality is disabled 
									//RA6 VCAP functionality is enabled on RA6 
									//RA0 VCAP functionality is enabled on RA0 

#pragma config   LVP = ON
									//Low-Voltage Programming Enable 
									//OFF High-voltage on MCLR/VPP must be used for programming 
									//ON Low-voltage programming enabled 			



//**--------------ȫ�ֱ���-----------------------------------------------------

//**--------------�궨��-------------------------------------------------------

//**--------------��������-----------------------------------------------------

//**--------------��������-----------------------------------------------------







/**********************************************************************
*�������ƣ�init_osc
*������ϵͳʱ�ӳ�����
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void init_osc(void)
{
	OSCCON = 0x70;				 // 0b0111 0000 �������ƼĴ���  �ڲ�ʱ�� 8MHz/32MHz
	WDTCONbits.WDTPS=0b01110;    //���Ź����� 16s
}






/**********************************************************************
*�������ƣ�init_port
*�������˿ڳ�ʼ��
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void init_port(void)
{
	ANSELA = 0x00; // �� RA<5:0>����ѡ��Ϊ���ֹ���
	ANSELB = 0x00; // �� RB<5:0> ����ѡ��Ϊ���ֹ���
	
	ANSA0 =	1; //RA0  ����Ϊģ������  AN0  LED_I_1	�����������һ·80W��
	ANSA1 =	1; //RA1  ����Ϊģ������  AN1  TEM		�¶ȼ��
	ANSA2 =	1; //RA2  ����Ϊģ������  AN2  PV		�����ѹ
	ANSA3 =	1; //RA3  ����Ϊģ������  AN3  BV		��ص�ѹ
	ANSA5 =	1; //RA5  ����Ϊģ������  AN4  LED_V_1	�����ѹ����һ·80W��
	
	ANSB4 = 1; //RB4  ����Ϊģ������  AN11 LED_V_2	�����ѹ���ڶ�·40W��
	ANSB3 = 1; //RB3  ����Ϊģ������  AN9  BT_I		������
	ANSB2 = 1; //RB2  ����Ϊģ������  AN8  LED_I_2	����������ڶ�·40W��
		
	PERSON_CHECK1_TRIS=1; //����������1   RA7  ����Ϊ����ģʽ	
	PERSON_CHECK2_TRIS=1; //����������2   RC1  ����Ϊ����ģʽ
	
	LOAD_OUT1_TRIS=0;     //����1 LED������� RC3  ����Ϊ���ģʽ
	LOAD_OUT2_TRIS=0;     //����2 LED������� RC0  ����Ϊ���ģʽ
	
	LOAD_OUT1_DISABLE;   //ϵͳ��ʼ�׶ιرո���1���
	LOAD_OUT2_DISABLE;   //ϵͳ��ʼ�׶ιرո���2���
	
	BUCK_PWM_H_TRIS=1;   //PWM����˿ڳ�ʼ�׶�Ϊ����  ����ֹPWM�˿�  ���
	
	BOST_PWM1_TRIS =1;	//PWM����˿ڳ�ʼ�׶�Ϊ����  ����ֹPWM�˿�	 ��һ·�ŵ�
	
	BOST_PWM2_TRIS =1;	//PWM����˿ڳ�ʼ�׶�Ϊ����  ����ֹPWM�˿�	 �ڶ�·�ŵ�
	
	DL_1_TRIS =1;
	DL_2_TRIS =1;
	
	POWER_TRIS = 0;
	POWER_ON;
	
}




/**********************************************************************
*�������ƣ�driver_init
*�������������ʼ��
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void driver_init(void)
{
	init_osc();	    	//ϵͳʱ������
		
	init_port();		//IO�˿ڼ�����˿ڳ�ʼ��
	
	init_key();			//��ʼ���������˿�	
	
	init_timer1();  	//��ʼ����ʱ��TIMER1
	
	init_uart1();   	//��ʼ��uart
	
	Init_Ird();			//��ʼ��������ܶ˿�
	
	init_adc();     	//��ʼ��adc
	
	init_pwm();     	//��ʼ��pwm
	
	init_interrupt();	//��ʼ���ж�
	
	
}




/**********************************************************************
*�������ƣ�middle_init
*�������м���ʼ��
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void middle_init(void)
{
	
	auto_system();			//ϵͳ��ѹʶ��12V?  24V?  ﮵��
	
	read_param_eeprom();  //��ȡϵͳ����
}







/**********************************************************************
*�������ƣ�application_init
*������Ӧ�ò��ʼ��
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void application_init(void)
{
	
	TMR1ON =1;//����ʱ����ʱ�� 1ms��С����
	
	system_check_time=0x01;
	
	open_an0_time=0x01;
	
	open_an1_time=0x01;
	
	open_an2_time=0x01;
	
	open_an3_time=0x01; 
	
	open_an4_time=0x01;
	
	open_an9_time=0x01;
	
	open_an8_time=0x01;
	
	open_an11_time=0x01;
	
	
	
	pid_init ( &PID_CC_charge);
	
	pid_init ( &PID_CV_charge);
	
	pid_init ( &PID_CC_discharge);
	
	pid_init ( &PID_CC2_discharge);
	
	
	
	//�������PID����
	PID_CC_charge.Proportion = 0.001;    				// �������� Proportional Const
	
	PID_CC_charge.Integral   = 0; 						// ���ֳ��� Integral Const
																			    
	PID_CC_charge.Derivative = 0; 						// ΢�ֳ��� Derivative Const
																				    
	PID_CC_charge.SetPoint   = 5.0*15.0; 				// �趨Ŀ�� Desired value  5000mA
											
    
    
	//��ѹ���PID����
	PID_CV_charge.Proportion = 0.001;    				// �������� Proportional Const
											
	PID_CV_charge.Integral   = 0; 						// ���ֳ��� Integral Const
										    
	PID_CV_charge.Derivative = 0; 						// ΢�ֳ��� Derivative Const
										    
	PID_CV_charge.SetPoint   = dianchi_cv_level+1; 		// �趨Ŀ�� Desired value  16.21V 
    
    
    //�ŵ����PID����
    PID_CC_discharge.Proportion = 0.001;  				// �������� Proportional Const
    
	PID_CC_discharge.Integral   = 0; 					// ���ֳ��� Integral Const
										    
	PID_CC_discharge.Derivative = 0; 					// ΢�ֳ��� Derivative Const
										    
	PID_CC_discharge.SetPoint   = (load_cc_I/100)*30;   // �趨Ŀ�� Desired value 330mA  
	
	//�ŵ����PID����
    PID_CC2_discharge.Proportion = 0.001;  	// �������� Proportional Const
    
	PID_CC2_discharge.Integral   = 0; 		// ���ֳ��� Integral Const
										    
	PID_CC2_discharge.Derivative = 0; 		// ΢�ֳ��� Derivative Const
										    
	PID_CC2_discharge.SetPoint   = (load_cc_I/100)*0.30*step_05_load_percent;    	// �趨Ŀ�� Desired value 330mA 
    
    open_discharge_time=0x00;
    
    ms_cnt=0x00;						
	se_cnt=0x00;						
	mi_cnt=0x00;						
	ho_cnt=0x00;		
    

    
}







/*---------------------------------------------------------------------
*�������ƣ�main
*������
*�������ͣ�int
*�����βΣ�void
---------------------------------------------------------------------*/
int main(void) // ������
{
	driver_init(); 		// �������ʼ��
	middle_init(); 		// �м���ʼ��
	application_init(); // Ӧ�ò��ʼ��
	
	
	while(1)
	{
		
		CLRWDT();  //ι��  ���Ź�����16s
		
		if(handle_scan_key_flag==0x01)
		{
			scan_key_value();
			
			handle_scan_key_flag=0x00;
		}
		
		if(ird_receive_flag==0x01)
        {          
              Key_Menu(ird_data);
          
              receive_bit_count=0x00;
              
              ird_status=0x00;
              
              ird_data=0x00;
              
              TMR0=0;//��ʱ������
			              
			  TMR0IF = 0;             
              
              ird_receive_flag=0x00;
                           
        }
		
		 handle_ad_loop();
		
		 handle_task_process();
		
		if (RCSTAbits.FERR || RCSTAbits.OERR)  //uart1 
			{
						RCSTAbits.CREN = 0x00;
						RCSTAbits.SPEN = 0x00;
						NOP();
						NOP();
						NOP();
						RCSTAbits.CREN = 0x01;
						RCSTAbits.SPEN = 0x01;
			}				 
             
         if(uart1_receive_flag==TRUE)  //�������ݴ���
             {                				
                handle_uart_rx_buf();
                
				if (RCSTAbits.FERR || RCSTAbits.OERR)  //uart1 
				{
							RCSTAbits.CREN = 0x00;
							RCSTAbits.SPEN = 0x00;
							NOP();
							NOP();
							NOP();
							RCSTAbits.CREN = 0x01;
							RCSTAbits.SPEN = 0x01;
				}
	 
                uart1_receive_byte_count=0x00;  //!!!!!!!!!

                uart1_receive_flag=FALSE;       //!!!!!!!!!

             } 
		
		
	}
	
}
