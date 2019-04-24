 
 
 
#include <xc.h>
#include <GenericTypeDefs.h>

#include "userdefine.h"



unsigned int pwm_duty_value=0x0000;

unsigned int buck_pwm_value=0x00;

unsigned int bost_pwm1_value=0x00;

unsigned int bost_pwm2_value=0x00;




long int P_prev=0;  //ǰһ�ι���
long int P_last=0;  //��һ�ι���




/**********************************************************************
*�������ƣ�init_pwm
*������CPP1 CCP2 CCP3 CCP4 CCP5   Time2/Time4/Time6/    BUCK�������ѡ��Time6  BOST���PWMѡ��Time4
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void init_pwm(void)
{

	BUCK_PWM_H_TRIS=1;  //PWM����˿ڳ�ʼ�׶�Ϊ����  ����ֹPWM�˿�
	
	BOST_PWM1_TRIS =1;	//BOOST ��ѹ�����һ·
	
	BOST_PWM2_TRIS =1; //BOOST ��ѹ����ڶ�·
	
	APFCONbits.CCP3SEL=1;  //CCP3/P3A����λ��RB5/AN13/CPS5/CCP3/P3A/T1G/COM1 ������
	
	
	
	//----------------------------BUCK��ѹPWM�����  CCP1  TIME6
	
	PR6=0xFF;//0x63;  		   				//PWM��������	Fosc=32MHz  PWM���Ƶ��=31.4Khz  �ֱ���10λ
	
	CCP1CONbits.CCP1M=0b1100;
	
	CCP1CONbits.P1M=0b00;			//��PWM���ģʽ
	
	CCPTMRS0bits.C1TSEL=0b10;      	//CCP1 ѡ��TIME6 
														
	CCPR1L=0x00;					//ռ�ձ�����  ������
	
	CCP1CONbits.DC1B=0b00;
	
	TMR6IF=0;
	
	T6CONbits.T6OUTPS=0b0000;
	
	TMR6ON=0;
	
	T6CONbits.T6CKPS=0b00;    	  //Ԥ��ƵֵΪ1
	

	
	//----------------------------��һ·BOST��ѹPWM1���   CCP3  TIME4
	
	PR4=0x9F;  		   				//PWM��������	Fosc=32MHz  PWM���Ƶ��=50Khz  �ֱ���9λ
	
	CCP3CONbits.CCP3M=0b1100;
	
	CCP3CONbits.P3M=0b00;
	
	CCPTMRS0bits.C3TSEL=0b01;      	//CCP3 ѡ��TIME4 
														
	CCPR3L=0x00;					//ռ�ձ�����  ������
	
	CCP3CONbits.DC3B=0b00;
	
	TMR4IF=0;
	
	T4CONbits.T4OUTPS=0b0000;
	
	TMR4ON=0;
	
	T4CONbits.T4CKPS=0b00;    	  //Ԥ��ƵֵΪ1
	
	//----------------------------�ڶ�·BOST��ѹPWM2���   CCP5  TIME2
	
	PR2=0x9F;  		   				//PWM��������	Fosc=32MHz  PWM���Ƶ��=50Khz  �ֱ���9λ
	
	CCP5CONbits.CCP5M=0b1100;
	
	CCP5CONbits.P5M=0b00;
	
	CCPTMRS1bits.C5TSEL=0b00;      					//CCP5 ѡ��TIME2 
														
	CCPR5L=0x00;				
	
	CCP5CONbits.DC5B=0b00;	
	
	TMR2IF=0;
	
	T2CONbits.T2OUTPS=0b0000;
	
	TMR2ON=0;
	
	T2CONbits.T2CKPS=0b00;    	  //Ԥ��ƵֵΪ1
	
}



/**********************************************************************
*�������ƣ�enable_buck_pwm
*������ʹ��BUCK PWM
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Enable_Buck_Pwm(void)
{
	BUCK_PWM_H_TRIS=0;  //ʹ��PWM�˿����
	
	TMR6ON=1;			//����CCP1 PWM  Time6��ʱ��

}



/**********************************************************************
*�������ƣ�disable_buck_pwm
*�������ر�BUCK PWM
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Disable_Buck_Pwm(void)
{
	BUCK_PWM_H_TRIS=1;  //����PWM�˿����  P1A
	
	TMR6ON=0;			//�ر�CCP1 PWM  Time6��ʱ��

}



/**********************************************************************
*�������ƣ�set_buck_pwm_duty
*����������BUCK PWMռ�ձ�
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Set_Buck_Pwm_Duty(unsigned int duty)     //D=duty/(4*(PR6+1))
{
	buck_pwm_value=duty;
	
	if(duty<=BUCK_PWM_MAX)
	{
		CCPR1L=(unsigned char)(duty>>2);					//ռ�ձ�����  ������
	
		CCP1CONbits.DC1B=(unsigned char)(duty&0b11);
	}
	else
	{
		duty=BUCK_PWM_MAX;
		
		CCPR1L=(unsigned char)(duty>>2);					
	
		CCP1CONbits.DC1B=(unsigned char)(duty&0b11);
	}

}



/**********************************************************************
*�������ƣ�enable_bost_pwm1
*����������BOST PWM1
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Enable_Bost_Pwm1(void)
{
	BOST_PWM1_TRIS =0;	//ʹ��PWM�˿����  CCP3   RB5
			
	TMR4ON=1;
}

/**********************************************************************
*�������ƣ�enable_bost_pwm2
*����������BOST PWM2
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Enable_Bost_Pwm2(void)
{
	BOST_PWM2_TRIS =0;	//ʹ��PWM�˿����  CCP5   RA4
			
	TMR2ON=1;
}



/**********************************************************************
*�������ƣ�disable_bost_pwm1
*����:�ر�BOST PWM1
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Disable_Bost_Pwm1(void)
{
	BOST_PWM1_TRIS =1;	//����PWM�˿����  CCP3   RB5
	
	TMR4ON=0;			//�رն�ʱ��4
	
}

/**********************************************************************
*�������ƣ�disable_bost_pwm2
*����:�ر�BOST PWM2
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Disable_Bost_Pwm2(void)
{
	BOST_PWM2_TRIS =1;	//����PWM�˿����  CCP5   RA4
	
	TMR2ON=0;			//�رն�ʱ��2
	
}

/**********************************************************************
*�������ƣ�set_bost_pwm_duty1
*����������BOST PWMռ�ձ�
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Set_Bost_Pwm1_Duty(unsigned int dduty)     //D=duty/(4*(PR4+1))
{
	
	bost_pwm1_value=dduty;
	
	if(dduty<=BOST_PWM_MAX)  
	{
		CCPR3L=(unsigned char)(dduty>>2);					//ռ�ձ�����  ������
	
		CCP3CONbits.DC3B=(unsigned char)(dduty&0b11);
	}
	else
	{
		dduty=BOST_PWM_MAX;
		
		CCPR3L=(unsigned char)(dduty>>2);					//ռ�ձ�����  ������
	
		CCP3CONbits.DC3B=(unsigned char)(dduty&0b11);
	}

}


/**********************************************************************
*�������ƣ�set_bost_pwm_duty2
*����������BOST PWMռ�ձ�
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Set_Bost_Pwm2_Duty(unsigned int dduty)     //D=duty/(4*(PR2+1))
{
	
	bost_pwm2_value=dduty;
	
	if(dduty<=BOST_PWM_MAX)  
	{
		CCPR5L=(unsigned char)(dduty>>2);					//ռ�ձ�����  ������
	
		CCP5CONbits.DC5B=(unsigned char)(dduty&0b11);
	}
	else
	{
		dduty=BOST_PWM_MAX;
		
		CCPR5L=(unsigned char)(dduty>>2);					//ռ�ձ�����  ������
	
		CCP5CONbits.DC5B=(unsigned char)(dduty&0b11);
	}

}




