

#ifndef _ZH_PWM_H
#define _ZH_PWM_H 






#define    BUCK_PWM_H_TRIS      TRISCbits.TRISC2

#define    BOST_PWM1_TRIS      	TRISBbits.TRISB5

#define    BOST_PWM2_TRIS      	TRISAbits.TRISA4


#define 	BUCK_PWM_MAX		900  	

#define 	BUCK_PWM_MIN		50	


#define 	BOST_PWM_MAX		488  	

#define 	BOST_PWM_MIN		5	


extern unsigned int pwm_duty_value;

extern unsigned int buck_pwm_value;

extern unsigned int bost_pwm1_value;

extern unsigned int bost_pwm2_value;




extern long int P_prev;  //ǰһ�ι���

extern long int P_last;  //��һ�ι���





extern void init_pwm(void);

extern void Enable_Buck_Pwm(void);

extern void Disable_Buck_Pwm(void);

extern void Set_Buck_Pwm_Duty(unsigned int duty);

extern void Enable_Bost_Pwm1(void);

extern void Disable_Bost_Pwm1(void);

extern void Set_Bost_Pwm1_Duty(unsigned int dduty);

extern void Enable_Bost_Pwm2(void);

extern void Disable_Bost_Pwm2(void);

extern void Set_Bost_Pwm2_Duty(unsigned int dduty);





#endif