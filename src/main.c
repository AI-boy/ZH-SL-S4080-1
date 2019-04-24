/******************************************************************************
**                           志和兴业有限公司
**
**                           
**
**--------------基本文件信息---------------------------------------------------
** 文   件   名	: main.c
** 当前    版本	: V1.0 
** 描        述 : 太阳能路灯控制器主程序
** 编   译   器 : xc8
** 硬        件 : 太阳能路灯控制器
**--------------历史版本信息---------------------------------------------------
** 开始创建日期	: 2017-12-11
** 创   建   人	: 蔡维凯
** 版  	     本 : V1.0
** 描　      述 : 

**-----------------------------------------------------------------------------
*******************************************************************************/

//**--------------头文件-------------------------------------------------------
#include <xc.h>
#include <GenericTypeDefs.h>

#include <stdio.h>
#include <string.h>

#include"userdefine.h"


 




//**--------------配置位(只针对PIC系列单片机)----------------------------------

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



//**--------------全局变量-----------------------------------------------------

//**--------------宏定义-------------------------------------------------------

//**--------------函数申明-----------------------------------------------------

//**--------------函数定义-----------------------------------------------------







/**********************************************************************
*函数名称：init_osc
*描述：系统时钟初配置
*返回类型：void
*输入形参：void
***********************************************************************/
void init_osc(void)
{
	OSCCON = 0x70;				 // 0b0111 0000 振荡器控制寄存器  内部时钟 8MHz/32MHz
	WDTCONbits.WDTPS=0b01110;    //看门狗周期 16s
}






/**********************************************************************
*函数名称：init_port
*描述：端口初始化
*返回类型：void
*输入形参：void
***********************************************************************/
void init_port(void)
{
	ANSELA = 0x00; // 将 RA<5:0>引脚选择为数字功能
	ANSELB = 0x00; // 将 RB<5:0> 引脚选择为数字功能
	
	ANSA0 =	1; //RA0  设置为模拟输入  AN0  LED_I_1	输出电流（第一路80W）
	ANSA1 =	1; //RA1  设置为模拟输入  AN1  TEM		温度检测
	ANSA2 =	1; //RA2  设置为模拟输入  AN2  PV		光伏电压
	ANSA3 =	1; //RA3  设置为模拟输入  AN3  BV		电池电压
	ANSA5 =	1; //RA5  设置为模拟输入  AN4  LED_V_1	输出电压（第一路80W）
	
	ANSB4 = 1; //RB4  设置为模拟输入  AN11 LED_V_2	输出电压（第二路40W）
	ANSB3 = 1; //RB3  设置为模拟输入  AN9  BT_I		充电电流
	ANSB2 = 1; //RB2  设置为模拟输入  AN8  LED_I_2	输出电流（第二路40W）
		
	PERSON_CHECK1_TRIS=1; //红外人体检测1   RA7  设置为输入模式	
	PERSON_CHECK2_TRIS=1; //红外人体检测2   RC1  设置为输入模式
	
	LOAD_OUT1_TRIS=0;     //负载1 LED输出控制 RC3  设置为输出模式
	LOAD_OUT2_TRIS=0;     //负载2 LED输出控制 RC0  设置为输出模式
	
	LOAD_OUT1_DISABLE;   //系统初始阶段关闭负载1输出
	LOAD_OUT2_DISABLE;   //系统初始阶段关闭负载2输出
	
	BUCK_PWM_H_TRIS=1;   //PWM输出端口初始阶段为输入  即禁止PWM端口  充电
	
	BOST_PWM1_TRIS =1;	//PWM输出端口初始阶段为输入  即禁止PWM端口	 第一路放电
	
	BOST_PWM2_TRIS =1;	//PWM输出端口初始阶段为输入  即禁止PWM端口	 第二路放电
	
	DL_1_TRIS =1;
	DL_2_TRIS =1;
	
	POWER_TRIS = 0;
	POWER_ON;
	
}




/**********************************************************************
*函数名称：driver_init
*描述：驱动层初始化
*返回类型：void
*输入形参：void
***********************************************************************/
void driver_init(void)
{
	init_osc();	    	//系统时钟配置
		
	init_port();		//IO端口及外设端口初始化
	
	init_key();			//初始化按键检测端口	
	
	init_timer1();  	//初始化定时器TIMER1
	
	init_uart1();   	//初始化uart
	
	Init_Ird();			//初始化红外接受端口
	
	init_adc();     	//初始化adc
	
	init_pwm();     	//初始化pwm
	
	init_interrupt();	//初始化中断
	
	
}




/**********************************************************************
*函数名称：middle_init
*描述：中间层初始化
*返回类型：void
*输入形参：void
***********************************************************************/
void middle_init(void)
{
	
	auto_system();			//系统电压识别：12V?  24V?  锂电池
	
	read_param_eeprom();  //读取系统参数
}







/**********************************************************************
*函数名称：application_init
*描述：应用层初始化
*返回类型：void
*输入形参：void
***********************************************************************/
void application_init(void)
{
	
	TMR1ON =1;//开启时基定时器 1ms最小周期
	
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
	
	
	
	//恒流充电PID调节
	PID_CC_charge.Proportion = 0.001;    				// 比例常数 Proportional Const
	
	PID_CC_charge.Integral   = 0; 						// 积分常数 Integral Const
																			    
	PID_CC_charge.Derivative = 0; 						// 微分常数 Derivative Const
																				    
	PID_CC_charge.SetPoint   = 5.0*15.0; 				// 设定目标 Desired value  5000mA
											
    
    
	//恒压充电PID调节
	PID_CV_charge.Proportion = 0.001;    				// 比例常数 Proportional Const
											
	PID_CV_charge.Integral   = 0; 						// 积分常数 Integral Const
										    
	PID_CV_charge.Derivative = 0; 						// 微分常数 Derivative Const
										    
	PID_CV_charge.SetPoint   = dianchi_cv_level+1; 		// 设定目标 Desired value  16.21V 
    
    
    //放电恒流PID调节
    PID_CC_discharge.Proportion = 0.001;  				// 比例常数 Proportional Const
    
	PID_CC_discharge.Integral   = 0; 					// 积分常数 Integral Const
										    
	PID_CC_discharge.Derivative = 0; 					// 微分常数 Derivative Const
										    
	PID_CC_discharge.SetPoint   = (load_cc_I/100)*30;   // 设定目标 Desired value 330mA  
	
	//放电恒流PID调节
    PID_CC2_discharge.Proportion = 0.001;  	// 比例常数 Proportional Const
    
	PID_CC2_discharge.Integral   = 0; 		// 积分常数 Integral Const
										    
	PID_CC2_discharge.Derivative = 0; 		// 微分常数 Derivative Const
										    
	PID_CC2_discharge.SetPoint   = (load_cc_I/100)*0.30*step_05_load_percent;    	// 设定目标 Desired value 330mA 
    
    open_discharge_time=0x00;
    
    ms_cnt=0x00;						
	se_cnt=0x00;						
	mi_cnt=0x00;						
	ho_cnt=0x00;		
    

    
}







/*---------------------------------------------------------------------
*函数名称：main
*描述：
*返回类型：int
*输入形参：void
---------------------------------------------------------------------*/
int main(void) // 主程序
{
	driver_init(); 		// 驱动层初始化
	middle_init(); 		// 中间层初始化
	application_init(); // 应用层初始化
	
	
	while(1)
	{
		
		CLRWDT();  //喂狗  看门狗周期16s
		
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
              
              TMR0=0;//定时器清零
			              
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
             
         if(uart1_receive_flag==TRUE)  //红外数据处理
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
