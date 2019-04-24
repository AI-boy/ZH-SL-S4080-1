

#include <xc.h>
#include <GenericTypeDefs.h>

#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>

#include "userdefine.h"


unsigned char buf[50];

unsigned char system_status=0x00;

unsigned char system_check_time=0x00;

unsigned int system_time_count=0x00;

unsigned char work_mode=0x00;


unsigned char pv_voltage_status=0x00;    		//光伏板电压状态

unsigned char battery_voltage_status=0x00;		//电池电压状态

unsigned char load1_voltage_status=0x00;			//负载端状态
unsigned char load2_voltage_status=0x00;			//负载端状态

unsigned char discharge1_index=0x00;  			//放电阶段变量
unsigned char discharge2_index=0x00;  			//放电阶段变量

unsigned char charge_index=0x00;				//充电阶段变量

unsigned char nop_index=0x00;

unsigned int involid_time_count=0x00;
unsigned int cv_level_time_count=0x00;

unsigned int load_short_time_count=0x00;		//输出短路检测变量
unsigned int load2_short_time_count=0x00;		//输出短路检测变量

unsigned int load_open_time_count=0x00;			//输出开路检测变量
unsigned int load2_open_time_count=0x00;		//输出开路检测变量

unsigned int over_load1_time_count=0x00;
unsigned int over_load2_time_count=0x00;

unsigned int battery_low_time_count=0x00;


unsigned long int night_time_count=0x00;
					
unsigned long int  day_time_count=0x00;

unsigned long int pv_time_count=0x00;

unsigned long int light_time_level=0x00;

unsigned int pv_prev=0x00;

unsigned int I_prev=0x00;


float PWM	=	0;

float PWM_I=	0;

float PWM2	=	0;

float PWM_I2=	0;


unsigned char mppt_status=0x00;

unsigned int mppt_time_count=0x00;

unsigned char temper_time_count=0x00;

unsigned char over_temper_time_count=0x00;
					
unsigned char sys_temper_flag=0x01;

unsigned char charge_led_status=0x00;

unsigned char bt_led_status=0x00;

unsigned char discharge_led_status=0x00;

unsigned char bt_time_count=0x00;

unsigned char pv_index=0x00;

unsigned char Adjust_Pout_Flag=0x00;

unsigned char voltage_adjust_index=0x01;

unsigned char limit_power_pid_flag=0x00;
unsigned char limit_power_pid2_flag=0x00;

unsigned char t1=0x00;
unsigned char t2=0x00;
unsigned char t3=0x00;
unsigned char t4=0x00;
unsigned char t5=0x00;

float bt_prev=0.0;

unsigned char mcu_t00_time_count=0x00;
unsigned char mcu_t01_time_count=0x00;
unsigned char mcu_t02_time_count=0x00;
unsigned char mcu_t03_time_count=0x00;
unsigned char mcu_temp_index=0x00;

unsigned char pid_index=0x00;

unsigned int index_01_count=0x00;
unsigned int index_02_count=0x00;
unsigned int index_03_count=0x00;
unsigned int index_04_count=0x00;
unsigned int index_05_count=0x00;
unsigned int index_06_count=0x00;


/**********************************************************************
*函数名称：handle_ad_loop
*描述：AD轮训采样
*返回类型：void
*输入形参：void
***********************************************************************/
void handle_ad_loop(void)
{
		//------------------------------------------------------------------------------------------------------------------------------------------------------
		float data_cc_var;
		
		//------------------------------------------------------------------------------------------------------------------------------------------------------
		if(handle_an1_process_flag==0x01)
		{
		
			temperature_value=start_ad_convert(AN1_CHANNEL);  		//采样通道选择	AN1		环境温度  0℃--90      70℃--950
			
			if((temperature_value>4)&&(temperature_value<605))  	//正常工作温度范围：-30℃---70℃
			{
				temper_time_count++;
				
				over_temper_time_count=0x00;
				
				if(temper_time_count>=10)
				{
					temper_time_count=0x00;
					
					sys_temper_flag=0x01; 						//系统工作温度正常
				}	
			}
			else
			{
				over_temper_time_count++;
				
				temper_time_count=0x00;
				
				if(over_temper_time_count>=10)
				{
					over_temper_time_count=0x00;
					
					sys_temper_flag=0x00;						//超出系统正常工作温度范围
				}
			}
			
			//--------------------------------------------------------------芯片内部温度
			ADCON1bits.ADPREF =0b00;  //VEF---VDD
			
			mcu_value=start_ad_convert(MCU_CHANNEL);  			//采样通道选择  MCU_CHANNEL  现版本该引脚用于短路保护 比较器负端输入 
			
			ADCON1bits.ADPREF =0b11;  //VEF---1,024V  内部参考电压
			
			if(mcu_value<490) //60 温度
			{
				mcu_t00_time_count++;
				mcu_t01_time_count=0x00;
				mcu_t02_time_count=0x00;
				mcu_t03_time_count=0x00;
				
				if(mcu_t00_time_count>=120)
				{
					mcu_t00_time_count=0x00;
					
					mcu_temp_index=0x00;
				}
			}				
			else if(mcu_value>510 && mcu_value<530) //80--100 温度 放电--降载50%
			{
				mcu_t01_time_count++;
				mcu_t00_time_count=0x00;
				mcu_t02_time_count=0x00;
				mcu_t03_time_count=0x00;
				
				if(mcu_t01_time_count>=60)
				{
					mcu_t01_time_count=0x00;
					
					mcu_temp_index=0x01;
				}
			}
			else if(mcu_value>530 && mcu_value<550) //100--120 温度  放电--降载20%
			{
					mcu_t02_time_count++;
					mcu_t00_time_count=0x00;
					mcu_t01_time_count=0x00;
					mcu_t03_time_count=0x00;
				
				if(mcu_t02_time_count>=30)
				{
					mcu_t02_time_count=0x00;
					
					mcu_temp_index=0x02;
				}
			}
			else if(mcu_value>=550)  //120--   过温保护
			{
					mcu_t03_time_count++;
					mcu_t00_time_count=0x00;
					mcu_t01_time_count=0x00;
					mcu_t02_time_count=0x00;
				
				if(mcu_t03_time_count>=10)
				{
					mcu_t03_time_count=0x00;
					
					mcu_temp_index=0x03;
				}
			}
			else
			{
				mcu_t00_time_count=0x00;
				mcu_t01_time_count=0x00;
				mcu_t02_time_count=0x00;
				mcu_t03_time_count=0x00;
			}
			
			handle_an1_process_flag=0x00;
						
		}
		
		
		
		//------------------------------------------------------------------------------------------------------------------------------------------------------
		if(handle_an2_process_flag==0x01)
		{
		
			pv_voltage_value=start_ad_convert(AN2_CHANNEL);  		//采样通道选择	AN2		光伏板电压    分压电阻缩减比例 1:69 
			
			
			if(pv_index==0x00)
			{
				if(pv_voltage_value<=light_on_pv_level) 		//鉴别白天和黑夜   通过光伏板电压检测   1V---12V
				{																								
					night_time_count++;
					
					day_time_count=0x00;
					
					if(night_time_count>light_time_level)  
					{
						night_time_count=0x00;
						
						pv_voltage_status=0x02;			//进入放电模式
						
					}
						
				}
				else if((pv_voltage_value>light_off_pv_level)) 
				{
					day_time_count++;
					
					night_time_count=0x00;
					
					if(day_time_count>light_time_level)
					{
						day_time_count=0x00;
						
						pv_voltage_status=0x00;  //空闲状态
												
					}				
				}
				else
				{
					day_time_count=0x00;
					
					night_time_count=0x00;
				}
			}
			
			if((pv_voltage_value>=light_off_pv_level)&&(pv_voltage_value<1000)) 	//10V---61V  光伏板电压范围
			{																			
				pv_time_count++;
												
				if(pv_time_count>(light_time_level/5))
				{
					pv_time_count=0x00;
							
					pv_voltage_status=0x01; 			//进入充电模式
					
					day_time_count=0x00;  
					
					night_time_count=0x00; 
					
					pv_index=0x01;										
							
				}				
							
			}
			else if(pv_voltage_value>=1000)
			{
				pv_time_count=0x00;
				
				pv_voltage_status=0x00; //空闲状态
			}
			else
			{
				pv_time_count=0x00;
						
			}
			
			if(pv_index==0x01)
			{				 
				if(pv_voltage_value<=light_on_pv_level) 									//鉴别白天和黑夜   通过光伏板电压检测   1V---10V
				{																								
					night_time_count++;
										
					if(night_time_count>light_time_level)  
					{
						night_time_count=0x00;
						
						pv_voltage_status=0x02;												//夜晚状态   即将进入放电模式
						
						pv_index=0x00;
					}
						
				}
				else
				{					
					night_time_count=0x00;
				} 
			
			}
			
		
			
			handle_an2_process_flag=0x00;
						
		}
		
		//------------------------------------------------------------------------------------------------------------------------------------------------------
		if(handle_an9_process_flag==0x01)  								//电流检测范围：0.1A---12A  采样电阻15mΩ
		{
			charge_current_value=0x00;
			
			if(system_status ==	SYSTEM_CHARGE_STATUS)
			{
				charge_current_value=start_ad_convert(AN9_CHANNEL);  		//采样通道选择	AN9		充电电流
			
				
				if((charge_current_value>=charge_i_limit)&&(charge_index>=0x03)) 		//充电限流12A
				{
				
					if(buck_pwm_value>BUCK_PWM_MIN)
					{
						buck_pwm_value =	buck_pwm_value - 1;
					}
					Set_Buck_Pwm_Duty(buck_pwm_value);

				}
			
			}			
			
			handle_an9_process_flag=0x00;
						
		}
		
		
		//------------------------------------------------------------------------------------------------------------------------------------------------------
	
		if(handle_an3_process_flag==0x01)
		{
		
			battery_voltage_value=start_ad_convert(AN3_CHANNEL);  			//采样通道选择	AN3	电池电压    分压电阻缩减比例 1：31.04
			
			//-------------------------------------------------------------------
			b_v=(float)(((float)battery_voltage_value)/10)*31.04;
			
			//-------------------------------------------------------------------
			
			if((battery_voltage_value<(dianchi_guoya_baohu+volt_id*5))&&(charge_index==0x00))					//锂电池电压是否正常？  <14.0V
			{
				bt_time_count++;
				
				if(bt_time_count>=100)
				{
					bt_time_count=0x00;
					
					bt_led_status=0x01;
					
					charge_index=0x01;
					
					battery_voltage_status=0x01;														//电池状态正常
				}					
				
			}
			else if((battery_voltage_value>=(dianchi_guoya_baohu+volt_id*5))&&(charge_index<=0x01))
			{		
					bt_led_status=0x00;
					
					bt_time_count=0x00;
												
					battery_voltage_status=0x00;					//电池状态异常
			}
			
				
			if( (battery_voltage_value<dianchi_guoya_huifu) && (charge_index==0x02) )  		//<电池充电恢复电压
			{
				
				
				charge_index=0x03;
				
				charge_led_status=0x01;
				
				Enable_Buck_Pwm();
			
			}
			else if( (battery_voltage_value>=dianchi_guoya_huifu) && (charge_index==0x02) )
			{
				bt_00_lv = b_v;
				
				charge_led_status=0x00;
				
			}
			else if( (battery_voltage_value<dianchi_guoya_baohu) && (charge_index>=0x03) )
			{
				
				if((charge_current_value<charge_i_limit)&&(charge_index>=0x03))	    				//充电电流：0---12A	
				{
					mppt_time_count++;
					
					if(mppt_time_count>5)
					{
						mppt_time_count=0x00;
						
						Mppt_Charge();       														//MPPT充电模式
						
						I_prev=charge_current_value;
					}					 												
							
				}
				
				involid_time_count=0x00;
				
			}
			else if( (battery_voltage_value>=dianchi_guoya_baohu) && (charge_index>=0x03) )  
			{
					
				involid_time_count++;
				
				if(involid_time_count>200)
				{
					involid_time_count=0x00;
					
					if(charge_index>=0x03)	
					{
						charge_index=0x05;
						
						Disable_Buck_Pwm();
									
						Set_Buck_Pwm_Duty(0);
						
						charge_led_status=0x00;
						
						while(1); //等待系统复位
					}	
				}	
			}
												
							
			 
			P_prev=P_last;
			
			pv_prev=pv_voltage_value;
			
			handle_an3_process_flag=0x00;
						
		}
		
		
		
		
		
		//---------------------------------------------------------------------------------------------------------------------第一路输出电压		
		if(handle_an4_process_flag==0x01)								//分压电阻缩减比例 1:69
		{
			load_voltage_value=0x00;
			
			if(system_status ==	SYSTEM_DISCHARGE_STATUS)
			{
				load_voltage_value=start_ad_convert(AN4_CHANNEL);  			//采样通道选择	AN4		负载输出端电压
				
			
				//---------------------------------------------------------------------------------------------------
					if((load_voltage_value<shuchu_shortout_voltage)&&(discharge1_index==0x02)&&(PID_CC_discharge.SetPoint>0))     	//输出端  <输出短路电压点   可判定：1.输出端短路  2.LED灯珠选用错误
					{
						load_short_time_count++;
						
						if(load_short_time_count>100)
						{
							load_short_time_count=0x00;				
							
							bost_pwm1_value=2;
							
							Set_Bost_Pwm1_Duty(bost_pwm1_value);
							
							discharge_led_status=0x00;
							
							delay_ms(1);			//2018-03-22
							
							Disable_Bost_Pwm1();		//2018-03-22
							
							LOAD_OUT1_DISABLE;		//2018-03-22
							
							while(1); //等待系统复位
						}
						
					}
					else if((load_voltage_value>=shuchu_shortout_voltage)&&(load_voltage_value<(shuchu_shortout_voltage*2))&&(discharge1_index==0x02)&&(PID_CC_discharge.SetPoint>0))     	//
					{
						load_short_time_count++;
						
						if(load_short_time_count>100)
						{
							load_short_time_count=0x00;	
							
							discharge1_index=0x03;			
							
							bost_pwm1_value=2;
							
							Set_Bost_Pwm1_Duty(bost_pwm1_value);
							
							discharge_led_status=0x00;
							
							delay_ms(1);			//2018-03-22
							
							Disable_Bost_Pwm1();		//2018-03-22
							
							LOAD_OUT1_DISABLE;		//2018-03-22
							
							while(1); //等待系统复位
						}
						
					}
					else
					{
						load_short_time_count=0x00;
					}
								
				//---------------------------------------------------------------------------------------------------
					if((load_voltage_value>shuchu_open_voltage)&&(discharge1_index==0x02)) //输出端电压>60V+放电模式=负载端开路过压
					{
						load_open_time_count++;
						
						if(load_open_time_count>20)
						{								
							discharge1_index=0x03;
							
							load_open_time_count=0x00;			
												
							bost_pwm1_value=5;
								
							Set_Bost_Pwm1_Duty(bost_pwm1_value);
								
							discharge_led_status=0x00;
							
							delay_ms(1);			//2018-03-22
								
							Disable_Bost_Pwm1();		//2018-03-22
								
							LOAD_OUT1_DISABLE;		//2018-03-22
							
							while(1); //等待系统复位
						}
					
					}
					else
					{
						load_open_time_count=0x00;
					}	
				//---------------------------------------------------------------------------------------------------
				
			}		
					  			
			handle_an4_process_flag=0x00;
						
		}
		
	
		//-----------------------------------------------------------------------------------------------------------------------第一路输出电流
		if(handle_an0_process_flag==0x01)  								//电流检测范围：
		{
			discharge_current_value=0x00;
			
			if(system_status ==	SYSTEM_DISCHARGE_STATUS)									//系统处于放电状态
			{
				discharge_current_value=start_ad_convert(AN0_CHANNEL);  					//采样通道选择	AN10	放电电流
							
				if((discharge_current_value>discharge_i_baohu)&&(discharge1_index==0x02))  	//过载保护    >4.5A
				{ 									
					over_load1_time_count++;
					
					if(over_load1_time_count>20)												//过载延时保护
					{										
						discharge1_index=0x03;
										
						bost_pwm1_value=2;
						
						Set_Bost_Pwm1_Duty(bost_pwm1_value);
						
						delay_ms(1);
						
						Disable_Bost_Pwm1();
						
						LOAD_OUT1_DISABLE;
						
						over_load1_time_count=0x00;
						
						discharge_led_status=0x00;
						
						while(1); //等待系统复位
					}
					
				}
				else
				{
						over_load1_time_count=0x00;
				}
				
				if((battery_voltage_value>dianchi_qianya_huifu)&&(discharge1_index==0x01))   //    电池欠压恢复点
				{
					discharge1_index=0x02;
					
					discharge_led_status=0x01;
					
					open_discharge_time=0x01;					
					
					LOAD_OUT1_ENABLE;
																																	
					Enable_Bost_Pwm1();					
					
				}
				else if((battery_voltage_value<=dianchi_qianya_huifu)&&(discharge1_index==0x01))
				{
					discharge_led_status=0x00;
					
					open_discharge_time=0x00;
				}
				
				
				 if((battery_voltage_value>=dianchi_qianya_baohu)&&(discharge1_index==0x02))    		//电池电压>欠压保护点
				 {
					 
				 	if((discharge_current_value<=discharge_i_limit)&&(discharge1_index==0x02))  		//PID 恒流输出控制  默认350mA(330mA)   PID 调节   电流限制=4.0A
					{								    

						//----------------------------------------------------------------------------时长节能调节模式
						t1=step_01_time_limit;
						t2=step_01_time_limit+step_02_time_limit;
						t3=step_01_time_limit+step_02_time_limit+step_03_time_limit;
						t4=step_01_time_limit+step_02_time_limit+step_03_time_limit+step_04_time_limit;
						t5=step_01_time_limit+step_02_time_limit+step_03_time_limit+step_04_time_limit+step_05_time_limit;
						
						if((ho_cnt<t1) && (Adjust_Pout_Flag<=0x01))	
						{
							Adjust_Pout_Flag=0x01;	
						}
						else if((ho_cnt>=t1) && (ho_cnt<t2) && (Adjust_Pout_Flag<=0x02))
						{
							Adjust_Pout_Flag=0x02;	
						}
						else if((ho_cnt>=t2) && (ho_cnt<t3) && (Adjust_Pout_Flag<=0x03))
						{
							Adjust_Pout_Flag=0x03;	
						}
						else if((ho_cnt>=t3) && (ho_cnt<t4) && (Adjust_Pout_Flag<=0x04))
						{
							Adjust_Pout_Flag=0x04;	
						}
						else if((ho_cnt>=t4) && (ho_cnt<t5) && (Adjust_Pout_Flag<=0x05))
						{
							Adjust_Pout_Flag=0x05;	
						}
						else
						{
							Adjust_Pout_Flag=0x00;
						}
						//----------------------------------------------------------------------------电量调节模式
						if(battery_voltage_value>bt_volt_80 && voltage_adjust_index<=0x01)
						{
							index_01_count++;
							index_02_count=0x00;
							index_03_count=0x00;
							index_04_count=0x00;
							index_05_count=0x00;
							index_06_count=0x00;
							if(index_01_count>1500)
							{
								voltage_adjust_index=0x01;
								index_01_count=0x00;
							}
							
						}
						else if(battery_voltage_value>bt_volt_60 && battery_voltage_value<=bt_volt_80 && voltage_adjust_index<=0x02)
						{
							index_02_count++;
							index_01_count=0x00;
							index_03_count=0x00;
							index_04_count=0x00;
							index_05_count=0x00;
							index_06_count=0x00;
							if(index_02_count>1500)
							{
								voltage_adjust_index=0x02;
								index_02_count=0x00;
							}
						}
						else if(battery_voltage_value>bt_volt_40 && battery_voltage_value<=bt_volt_60 && voltage_adjust_index<=0x03)
						{
							index_03_count++;
							index_02_count=0x00;
							index_01_count=0x00;
							index_04_count=0x00;
							index_05_count=0x00;
							index_06_count=0x00;
							if(index_03_count>1500)
							{
								voltage_adjust_index=0x03;
								index_03_count=0x00;
							}
						}
						else if(battery_voltage_value>bt_volt_20 && battery_voltage_value<=bt_volt_40 && voltage_adjust_index<=0x04)
						{
							index_04_count++;
							index_02_count=0x00;
							index_03_count=0x00;
							index_01_count=0x00;
							index_05_count=0x00;
							index_06_count=0x00;
							if(index_04_count>1500)
							{
								voltage_adjust_index=0x04;
								index_04_count=0x00;
							}
						}
						else if(battery_voltage_value>bt_volt_00 && battery_voltage_value<=bt_volt_20 && voltage_adjust_index<=0x05)
						{
							index_05_count++;
							index_02_count=0x00;
							index_03_count=0x00;
							index_04_count=0x00;
							index_01_count=0x00;
							index_06_count=0x00;
							if(index_05_count>1500)
							{
								voltage_adjust_index=0x05;
								index_05_count=0x00;
							}
						}
						else if(battery_voltage_value<bt_volt_00 && voltage_adjust_index<=0x06)
						{
							index_06_count++;
							index_02_count=0x00;
							index_03_count=0x00;
							index_04_count=0x00;
							index_05_count=0x00;
							index_01_count=0x00;
							if(index_06_count>1500)
							{
								voltage_adjust_index=0x06;
								index_06_count=0x00;
							}
						}
						else
						{
							index_01_count=0x00;
							index_02_count=0x00;
							index_03_count=0x00;
							index_04_count=0x00;
							index_05_count=0x00;
							index_06_count=0x00;
						}
						//---------------------------------------------------------------------------限定功率输出
						if( ((load_cc_I/100)*30) > (Load_Pmax*30/(load_voltage_value*61/1000)) )	
						{
							PID_CC_discharge.SetPoint=(Load_Pmax*30)/(load_voltage_value*61/1000);
							
							limit_power_pid_flag=0x01;	//------PID限功率调节标记	
							
							data_cc_var = 	PID_CC_discharge.SetPoint;									           
							
							if(Adjust_Pout_Flag==0x01)
							{
								PID_CC_discharge.SetPoint   = (data_cc_var)*step_01_load_percent/100;	//2018-03-22
							}
							else if(Adjust_Pout_Flag==0x02)
							{
								PID_CC_discharge.SetPoint   = (data_cc_var)*step_02_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x03)
							{
								PID_CC_discharge.SetPoint   = (data_cc_var)*step_03_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x04)
							{
								PID_CC_discharge.SetPoint   = (data_cc_var)*step_04_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x05)
							{
								PID_CC_discharge.SetPoint   = (data_cc_var)*step_05_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x00)
							{
								PID_CC_discharge.SetPoint  =0;
							}
							
															
						}
						else
						{
							PID_CC_discharge.SetPoint=(load_cc_I/100)*30;
							
							data_cc_var = 	PID_CC_discharge.SetPoint;
							
							if(Adjust_Pout_Flag==0x01)
							{
								PID_CC_discharge.SetPoint   = (data_cc_var)*step_01_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x02)
							{
								PID_CC_discharge.SetPoint   = (data_cc_var)*step_02_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x03)
							{
								PID_CC_discharge.SetPoint   = (data_cc_var)*step_03_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x04)
							{
								PID_CC_discharge.SetPoint   = (data_cc_var)*step_04_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x05)
							{
								PID_CC_discharge.SetPoint   = (data_cc_var)*step_05_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x00)
							{
								PID_CC_discharge.SetPoint  =0;
							}
							
						}
						//------------------------------------------------------电量节能调节
							if(voltage_adjust_index==0x01)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*1.0;
							}
							else if(voltage_adjust_index==0x02)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.70;
							}
							else if(voltage_adjust_index==0x03)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.45;
							}
							else if(voltage_adjust_index==0x04)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.25;
							}
							else if(voltage_adjust_index==0x05)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.15;
							}
							else if(voltage_adjust_index==0x06)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.05;
							}
						
						//------------------------------------------------------内部过温降载
							if(mcu_temp_index==0x01) //80--100℃  降载50%
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.5;
							}
							else if(mcu_temp_index==0x02) //100--120℃  降载20%
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.2;
							}
							else if(mcu_temp_index==0x03) //120 内部过温保护
							{
								PID_CC_discharge.SetPoint  =0;
							}
						//---------------------------------------------------------亮灯模式
						 if(light_mode==0x00 && work_mode==0x00)//常亮模式
						 {
						 	PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.5; //常亮模式  50%额定功率输出(80W--40W)
						 }
						 else if(light_mode==0x01 && work_mode==0x00)//感应模式
						 {
						 	if(have_person1_flag==0x01 || have_person2_flag==0x01)
						 	{
						 		NOP();		//有感应时 电量调节输出
						 	}
						 	else
						 	{
						 		PID_CC_discharge.SetPoint = data_cc_var*0.05; //无感应时 5%输出
						 	}
						 }
						 else if(light_mode==0x02 && work_mode==0x00)//智能模式
						 {
						 	if(Adjust_Pout_Flag==0x01) //
						 	{
						 		PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.5; //常亮模式
						 	}
						 	else if(Adjust_Pout_Flag>=0x02)
						 	{
						 		if(have_person1_flag==0x01 || have_person2_flag==0x01)
							 	{
							 		NOP();		//有感应时 电量调节输出
							 	}
							 	else
							 	{
							 		PID_CC_discharge.SetPoint = data_cc_var*0.05; //无感应时 5%输出
							 		
							 	}
						 	}
						 }
						//---------------------------------------------------------开关灯控制
						if(led_out_ctrl==0x01 && work_mode==0x00)
						{
							PID_CC_discharge.SetPoint = 0;
						}
			
						//---------------------------------------------------------
							pid_index = 0x01; //恒流放电PID调节标志
							
							PWM_I=pid_calc(&PID_CC_discharge,discharge_current_value);
									
						    PWM	 = PWM+PWM_I;
						           	
						    if( PWM>=BOST_PWM_MAX)
						      {
						           PWM=BOST_PWM_MAX;
						      }			           
						    else if(PWM<=BOST_PWM_MIN) 
						      {
						           PWM=BOST_PWM_MIN;
						       }
						    
						    bost_pwm1_value = (unsigned int)PWM;
						    
						   
						    
						    if(PID_CC_discharge.SetPoint==0)
						    {
						    	bost_pwm1_value = 0;
						    }
						           
						    Set_Bost_Pwm1_Duty(bost_pwm1_value);
						    
						    limit_power_pid_flag=0x00;		//清限定输出功率标志
							
							
					}
					else if((discharge_current_value>discharge_i_limit)&&(discharge1_index==0x02))
					{
						if(bost_pwm1_value>10)
						{
							bost_pwm1_value =	bost_pwm1_value-1;												
						}
						
						Set_Bost_Pwm1_Duty(bost_pwm1_value);
					}
					
					battery_low_time_count=0;
					
				 }
				 else if((battery_voltage_value<dianchi_qianya_baohu)&&(discharge1_index==0x02)) //   欠压保护 
				 {
				 		
				 		battery_low_time_count++;
						
						if(battery_low_time_count>1000)
						{													
							discharge1_index=0x03;
							
							bost_pwm1_value=2;
							
							Set_Bost_Pwm1_Duty(bost_pwm1_value);
							
							battery_low_time_count=0x00;
							
							discharge_led_status=0x00;
							
							while(1); //等待系统复位
						}
				 
				 }
				 else
				 {
				 		battery_low_time_count=0;
				 }
			
			}
						
			handle_an0_process_flag=0x00;
						
		}
		//---------------------------------------------------------------------------------------------------------第二路输出电压 LED_V_2
		if(handle_an11_process_flag==0x01)								//分压电阻缩减比例 1:69
		{
			load_voltage2_value=0x00;
			
			if(system_status ==	SYSTEM_DISCHARGE_STATUS)
			{
				load_voltage2_value=start_ad_convert(AN11_CHANNEL);  			//采样通道选择	AN11		
			
				//---------------------------------------------------------------------------------------------------
					if((load_voltage2_value<shuchu_shortout_voltage)&&(discharge2_index==0x02)&&(PID_CC2_discharge.SetPoint>0))     	//输出端  <输出短路电压点   可判定：1.输出端短路  2.LED灯珠选用错误
					{
						load2_short_time_count++;
						
						if(load2_short_time_count>50)
						{
							load2_short_time_count=0x00;				
							
							bost_pwm2_value=2;
							
							Set_Bost_Pwm2_Duty(bost_pwm2_value);
							
							discharge_led_status=0x00;
							
							delay_ms(1);			//2018-03-22
							
							Disable_Bost_Pwm2();		//2018-03-22
							
							LOAD_OUT2_DISABLE;		//2018-03-22
							
							while(1); //等待系统复位
						}
						
					}
					else if((load_voltage2_value>=shuchu_shortout_voltage)&&(load_voltage2_value<(shuchu_shortout_voltage*2))&&(discharge2_index==0x02)&&(PID_CC2_discharge.SetPoint>0))     	//
					{
						load2_short_time_count++;
						
						if(load2_short_time_count>50)
						{
							load2_short_time_count=0x00;	
							
							discharge2_index=0x03;			
							
							bost_pwm2_value=2;
							
							Set_Bost_Pwm2_Duty(bost_pwm2_value);
							
							discharge_led_status=0x00;
							
							delay_ms(1);			//2018-03-22
							
							Disable_Bost_Pwm2();		//2018-03-22
							
							LOAD_OUT2_DISABLE;		//2018-03-22
							
							while(1); //等待系统复位
						}
						
					}
					else
					{
						load2_short_time_count=0x00;
					}
								
				//---------------------------------------------------------------------------------------------------
					if((load_voltage2_value>shuchu_open_voltage)&&(discharge2_index==0x02)) //输出端电压>60V+放电模式=负载端开路过压
					{
						load2_open_time_count++;
						
						if(load2_open_time_count>20)
						{								
							discharge2_index=0x03;
							
							load2_open_time_count=0x00;			
												
							bost_pwm2_value=5;
								
							Set_Bost_Pwm2_Duty(bost_pwm2_value);
								
							discharge_led_status=0x00;
							
							delay_ms(1);			//2018-03-22
								
							Disable_Bost_Pwm2();		//2018-03-22
								
							LOAD_OUT2_DISABLE;		//2018-03-22
							
							while(1); //等待系统复位
						}
					
					}
					else
					{
						load2_open_time_count=0x00;
					}	
				//---------------------------------------------------------------------------------------------------
				
			}		
					  			
			handle_an11_process_flag=0x00;
						
		}
		//---------------------------------------------------------------------------------------------------------第二路输出电流
		if(handle_an8_process_flag==0x01)  								//电流检测范围：  采样电阻75毫欧    默认330mA输出 
		{
			discharge_current2_value=0x00;
			
			if(system_status ==	SYSTEM_DISCHARGE_STATUS)									//系统处于放电状态
			{
				discharge_current2_value=start_ad_convert(AN8_CHANNEL);  					//采样通道选择	AN8	
							
				if((discharge_current2_value>discharge_i_baohu)&&(discharge2_index==0x02))  	//过载保护    >4.5A
				{ 									
					over_load2_time_count++;
					
					if(over_load2_time_count>20)												//过载延时保护
					{										
						discharge2_index=0x03;
										
						bost_pwm2_value=2;
						
						Set_Bost_Pwm2_Duty(bost_pwm2_value);
						
						delay_ms(1);
						
						Disable_Bost_Pwm2();
						
						LOAD_OUT2_DISABLE;
						
						over_load2_time_count=0x00;
						
						discharge_led_status=0x00;
						
						while(1); //等待系统复位
					}
					
				}
				else
				{
						over_load2_time_count=0x00;
				}
				
				if((battery_voltage_value>dianchi_qianya_huifu)&&(discharge2_index==0x01))   //    电池欠压恢复点
				{
					discharge2_index=0x02;
					
					discharge_led_status=0x01;
					
					open_discharge_time=0x01;					
					
					LOAD_OUT2_ENABLE;
																																	
					Enable_Bost_Pwm2();					
					
				}
				else if((battery_voltage_value<=dianchi_qianya_huifu)&&(discharge2_index==0x01))
				{
					discharge_led_status=0x00;
					
					open_discharge_time=0x00;
				}
				
				
				 if((battery_voltage_value>=dianchi_qianya_baohu)&&(discharge2_index==0x02))    		//电池电压>欠压保护点
				 {
					 
				 	if((discharge_current2_value<=discharge_i_limit)&&(discharge2_index==0x02))  		//PID 恒流输出控制  默认350mA(330mA)   PID 调节   电流限制=4.0A
					{								    

						//----------------------------------------------------------------------------时长节能调节模式
						t1=step_01_time_limit;
						t2=step_01_time_limit+step_02_time_limit;
						t3=step_01_time_limit+step_02_time_limit+step_03_time_limit;
						t4=step_01_time_limit+step_02_time_limit+step_03_time_limit+step_04_time_limit;
						t5=step_01_time_limit+step_02_time_limit+step_03_time_limit+step_04_time_limit+step_05_time_limit;
						
						if((ho_cnt<t1) && (Adjust_Pout_Flag<=0x01))	
						{
							Adjust_Pout_Flag=0x01;	
						}
						else if((ho_cnt>=t1) && (ho_cnt<t2) && (Adjust_Pout_Flag<=0x02))
						{
							Adjust_Pout_Flag=0x02;	
						}
						else if((ho_cnt>=t2) && (ho_cnt<t3) && (Adjust_Pout_Flag<=0x03))
						{
							Adjust_Pout_Flag=0x03;	
						}
						else if((ho_cnt>=t3) && (ho_cnt<t4) && (Adjust_Pout_Flag<=0x04))
						{
							Adjust_Pout_Flag=0x04;	
						}
						else if((ho_cnt>=t4) && (ho_cnt<t5) && (Adjust_Pout_Flag<=0x05))
						{
							Adjust_Pout_Flag=0x05;	
						}
						else
						{
							Adjust_Pout_Flag=0x00;
						}
						
						//---------------------------------------------------------------------------限定功率输出
						if( ((load_cc_I/100)*0.30*step_05_load_percent) > (Load2_Pmax*30/(load_voltage2_value*61/1000)) )	
						{
							PID_CC2_discharge.SetPoint=(Load2_Pmax*30)/(load_voltage2_value*61/1000);
							
							limit_power_pid2_flag=0x01;	//------PID限功率调节标记	
							
							data_cc_var = PID_CC2_discharge.SetPoint;										           
							
							if(Adjust_Pout_Flag==0x01)
							{
								PID_CC2_discharge.SetPoint   = (data_cc_var)*step_01_load_percent/100;	//2018-03-22
							}
							else if(Adjust_Pout_Flag==0x02)
							{
								PID_CC2_discharge.SetPoint   = (data_cc_var)*step_02_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x03)
							{
								PID_CC2_discharge.SetPoint   = (data_cc_var)*step_03_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x04)
							{
								PID_CC2_discharge.SetPoint   = (data_cc_var)*step_04_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x05)
							{
								PID_CC2_discharge.SetPoint   = (data_cc_var)*step_05_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x00)
							{
								PID_CC2_discharge.SetPoint  =0;
							}
														
						}
						else
						{
							PID_CC2_discharge.SetPoint=(load_cc_I/100)*0.30*step_05_load_percent;
							
							data_cc_var = PID_CC2_discharge.SetPoint;
							
							if(Adjust_Pout_Flag==0x01)
							{
								PID_CC2_discharge.SetPoint   = (data_cc_var)*step_01_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x02)
							{
								PID_CC2_discharge.SetPoint   = (data_cc_var)*step_02_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x03)
							{
								PID_CC2_discharge.SetPoint   = (data_cc_var)*step_03_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x04)
							{
								PID_CC2_discharge.SetPoint   = (data_cc_var)*step_04_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x05)
							{
								PID_CC2_discharge.SetPoint   = (data_cc_var)*step_05_load_percent/100;
							}
							else if(Adjust_Pout_Flag==0x00)
							{
								PID_CC2_discharge.SetPoint  =0;
							}													
							
						}
							//------------------------------------------------------电量节能调节
							if(voltage_adjust_index==0x01)
							{
								PID_CC2_discharge.SetPoint = PID_CC2_discharge.SetPoint*1.0;
							}
							else if(voltage_adjust_index==0x02)
							{
								PID_CC2_discharge.SetPoint = PID_CC2_discharge.SetPoint*0.70;
							}
							else if(voltage_adjust_index==0x03)
							{
								PID_CC2_discharge.SetPoint = PID_CC2_discharge.SetPoint*0.45;
							}
							else if(voltage_adjust_index==0x04)
							{
								PID_CC2_discharge.SetPoint = PID_CC2_discharge.SetPoint*0.25;
							}
							else if(voltage_adjust_index==0x05)
							{
								PID_CC2_discharge.SetPoint = PID_CC2_discharge.SetPoint*0.15;
							}
							else if(voltage_adjust_index==0x06)
							{
								PID_CC2_discharge.SetPoint = PID_CC2_discharge.SetPoint*0.05;
							}
							//------------------------------------------------------内部过温降载
							if(mcu_temp_index==0x01) //80--100℃  降载50%
							{
								PID_CC2_discharge.SetPoint = PID_CC2_discharge.SetPoint*0.5;
							}
							else if(mcu_temp_index==0x02) //100--120℃  降载20%
							{
								PID_CC2_discharge.SetPoint = PID_CC2_discharge.SetPoint*0.2;
							}
							else if(mcu_temp_index==0x03) //120 内部过温保护
							{
								PID_CC2_discharge.SetPoint  =0;
							}
							//---------------------------------------------------------亮灯模式
							 if(light2_mode==0x00 && work_mode==0x00)//----------------常亮模式
							 {
							 	PID_CC2_discharge.SetPoint = PID_CC2_discharge.SetPoint*0.5;  
							 }
							 else if(light2_mode==0x01 && work_mode==0x00)//-------------感应模式
							 {
							 	if(have_person1_flag==0x01 || have_person2_flag==0x01)
							 	{
							 		NOP();		//有感应时 电量调节输出
							 	}
							 	else
							 	{
							 		PID_CC2_discharge.SetPoint = data_cc_var*0.05; //无感应时 5%输出
							 	}
							 }
							 else if(light2_mode==0x02 && work_mode==0x00)//智能模式
							 {
							 	if(Adjust_Pout_Flag==0x01) //
							 	{
							 		PID_CC2_discharge.SetPoint = PID_CC2_discharge.SetPoint*0.5; //常亮模式
							 	}
							 	else if(Adjust_Pout_Flag>=0x02)
							 	{
							 		if(have_person1_flag==0x01 || have_person2_flag==0x01)
								 	{
								 		NOP();		//有感应时 电量调节输出
								 	}
								 	else
								 	{
								 		PID_CC2_discharge.SetPoint = data_cc_var*0.05; //无感应时 5%输出
								 	}
							 	}
							 }
							//---------------------------------------------------------开关灯控制
							if(led_out_ctrl==0x01 && work_mode==0x00)
							{
								PID_CC2_discharge.SetPoint = 0;
							}
							//---------------------------------------------------------
							pid_index = 0x01; //恒流放电PID调节标志
							
							PWM_I2=pid_calc_2(&PID_CC2_discharge,discharge_current2_value);
									
						    PWM2	 = PWM2+PWM_I2;
						           	
						    if( PWM2>=BOST_PWM_MAX)
						      {
						           PWM2=BOST_PWM_MAX;
						      }			           
						    else if(PWM2<=BOST_PWM_MIN) 
						      {
						           PWM2=BOST_PWM_MIN;
						       }
						    
						    bost_pwm2_value = (unsigned int)PWM2;
						    
						    if(PID_CC2_discharge.SetPoint==0)
						    {
						    	bost_pwm2_value = 0;
						    }
						           
						    Set_Bost_Pwm2_Duty(bost_pwm2_value);
						    
						    limit_power_pid2_flag=0x00;		//清限定输出功率标志
							
							
					}
					else if((discharge_current2_value>discharge_i_limit)&&(discharge2_index==0x02))
					{
						if(bost_pwm2_value>10)
						{
							bost_pwm2_value =	bost_pwm2_value-1;												
						}
						
						Set_Bost_Pwm2_Duty(bost_pwm2_value);
					}
					
					battery_low_time_count=0;
					
				 }
				 else if((battery_voltage_value<dianchi_qianya_baohu)&&(discharge2_index==0x02)) //   欠压保护 
				 {
				 		
				 		battery_low_time_count++;
						
						if(battery_low_time_count>1000)
						{													
							discharge2_index=0x03;
							
							bost_pwm2_value=2;
							
							Set_Bost_Pwm2_Duty(bost_pwm2_value);
							
							battery_low_time_count=0x00;
							
							discharge_led_status=0x00;
							
							while(1); //等待系统复位
						}
				 
				 }
				 else
				 {
				 		battery_low_time_count=0;
				 }
			
			}
						
			handle_an8_process_flag=0x00;
						
		}
		


}



/**********************************************************************
*函数名称：start_ad_convert
*描述：启动AD采样转换
*返回类型：unsigned int
*输入形参：unsigned char
***********************************************************************/
unsigned int start_ad_convert(unsigned char ch)
{
	
			unsigned char i,j,k;
		
			unsigned int ad_h,ad_l;		
			
		
			unsigned int temp;
			
			unsigned char ad_ch;
			
			ad_ch=0x00;
			
			ad_ch=ch;
	
			ADCON0bits.CHS	  =ch;  			//采样通道选择
	
    		ADCON0bits.ADON   =1;				//使能AD外设
    		
    		if((ad_ch==AN0_CHANNEL)||(ad_ch==AN8_CHANNEL)||(ad_ch==AN9_CHANNEL))   //放电电流----充电电流
    		{
    			for(i=0;i<51;i++)
				{
					__delay_us(5);//等待采样时间		
			
					ADCON0bits.GO=1; //启动采样
			
					while(ADCON0bits.nDONE);
			
					ad_h=0b00000011&ADRESH;
			
					ad_l=ADRESL;
			
					adc_buffer[i]=(ad_h<<8)|ad_l;
				}
    			for(j=0;j<50;j++)
				{
					for(i=0;i<50-j;i++)
					{
						if(adc_buffer[i]>adc_buffer[i+1])
						{
							temp=adc_buffer[i];
							
							adc_buffer[i]=adc_buffer[i+1];
			
							adc_buffer[i+1]=temp;
						}			
			
					}
				}
			
				ADCON0bits.ADON   =0;
				
				return((adc_buffer[24]+adc_buffer[26]+
						adc_buffer[22]+adc_buffer[28]+
						adc_buffer[20]+adc_buffer[30]+
						adc_buffer[18]+adc_buffer[32]+
						adc_buffer[16]+adc_buffer[34]+
						adc_buffer[14]+adc_buffer[36]+
						adc_buffer[12]+adc_buffer[38]+
						adc_buffer[10]+adc_buffer[40]+
						adc_buffer[8]+adc_buffer[42]+
						adc_buffer[6]+adc_buffer[44]+
						adc_buffer[4]+adc_buffer[46]+
						adc_buffer[2]+adc_buffer[48]+
						adc_buffer[0]+adc_buffer[50])/26);
    		}
    		
    		
    		
    		for(i=0;i<N;i++)
			{
				__delay_us(10);//等待采样时间		
		
				ADCON0bits.GO=1; //启动采样
		
				while(ADCON0bits.nDONE);
		
				ad_h=0b00000011&ADRESH;
		
				ad_l=ADRESL;
		
				adc_buffer[i]=(ad_h<<8)|ad_l;
			}
			
			for(j=0;j<N-1;j++)
			{
				for(i=0;i<N-j;i++)
				{
					if(adc_buffer[i]>adc_buffer[i+1])
					{
						temp=adc_buffer[i];
						
						adc_buffer[i]=adc_buffer[i+1];
		
						adc_buffer[i+1]=temp;
					}			
		
				}
			}
			
			ADCON0bits.ADON   =0;
			
			return((adc_buffer[((N-1)/2)-1]+adc_buffer[((N-1)/2)+1])/2);
			
					
}
		



/**********************************************************************
*函数名称：handle_task_process
*描述：系统任务调度处理
*返回类型：void
*输入形参：void
***********************************************************************/		
void handle_task_process(void)
{
	
	
	if(handle_short1_flag==0x01)  //第一路短路保护
	{
		over_load1_time_count=0x00;
						
		load1_voltage_status=0x01;
		
		handle_short1_flag=0x00;
		
		discharge_led_status=0x00;
						
		while(1); //等待系统复位
	}
	
	if(handle_short2_flag==0x01)  //第二路短路保护
	{
		over_load2_time_count=0x00;
						
		load2_voltage_status=0x01;
		
		handle_short2_flag=0x00;
		
		discharge_led_status=0x00;
						
		while(1); //等待系统复位
	}
	
	if(work_mode==0x00)//-----------------正常工作模式（自动模式）
	{
		if((pv_voltage_status==0x01)&&(battery_voltage_status==0x01)&&(sys_temper_flag==0x01)&&(mcu_temp_index<0x03))
		{
										
			system_status =	SYSTEM_CHARGE_STATUS;		
								
		}
		else if((pv_voltage_status==0x02)&&(battery_voltage_status==0x01)&&(sys_temper_flag==0x01)&&(mcu_temp_index<0x03))
		{
		
			system_status =	SYSTEM_DISCHARGE_STATUS;
			
		}
		else
		{
			system_status = SYSTEM_NOP_STATUS;
		}	
	}
	else if(work_mode==0x01)  //---------------手动充电模式
	{
		system_status =	SYSTEM_CHARGE_STATUS;
	}
	else if(work_mode==0x02)  //---------------手动放电模式
	{
		system_status =	SYSTEM_DISCHARGE_STATUS;
	}
	else
	{
		system_status = SYSTEM_NOP_STATUS;
	}
	
	
	switch(system_status)
	{
		case SYSTEM_NOP_STATUS:      
									if(nop_index==0x00)
									{
										bt_led_status=0x00;
										
										charge_led_status=0x00;
										
										discharge_led_status=0x00;																			
										
										Disable_Buck_Pwm();
								
										Set_Buck_Pwm_Duty(0);	
								
										Disable_Bost_Pwm1();
										Disable_Bost_Pwm2();
								
										Set_Bost_Pwm1_Duty(0);
										Set_Bost_Pwm2_Duty(0);
										
										LOAD_OUT1_DISABLE;
										LOAD_OUT2_DISABLE;
										
										P_last=0;
										
										P_prev=0;
										
										pv_prev=0;
										
										buck_pwm_value=0;
										
										bost_pwm1_value=0;
										bost_pwm2_value=0;
										
										PWM		=50;
										PWM2	=50;
										
										pid_init ( &PID_CC_charge);
										
										pid_init ( &PID_CV_charge);
	
										pid_init ( &PID_CC_discharge);
										pid_init ( &PID_CC2_discharge);
										
										
										//恒流充电PID调节
										PID_CC_charge.Proportion = 0.001;    			// 比例常数 Proportional Const
										
										PID_CC_charge.Integral   = 0; 					// 积分常数 Integral Const
																												    
										PID_CC_charge.Derivative = 0; 					// 微分常数 Derivative Const
																													    
										PID_CC_charge.SetPoint   = 5.0*15.0;    		// 设定目标 Desired value  
																				
									    
									    
										//恒压充电PID调节
										PID_CV_charge.Proportion = 0.001;    			// 比例常数 Proportional Const
																				
										PID_CV_charge.Integral   = 0; 					// 积分常数 Integral Const
																			    
										PID_CV_charge.Derivative = 0; 					// 微分常数 Derivative Const
																			    
										PID_CV_charge.SetPoint   = dianchi_cv_level+1;    		
									    
									    
									    //放电恒流PID调节
									    PID_CC_discharge.Proportion = 0.001;   			 	// 比例常数 Proportional Const
									    
										PID_CC_discharge.Integral   = 0; 					// 积分常数 Integral Const
																			    
										PID_CC_discharge.Derivative = 0; 					// 微分常数 Derivative Const
																			    
										PID_CC_discharge.SetPoint   = (load_cc_I/100)*30;   // 设定目标 Desired value 330mA 
										
										PID_CC2_discharge.Proportion = 0.001;    			// 比例常数 Proportional Const
									    
										PID_CC2_discharge.Integral   = 0; 					// 积分常数 Integral Const
																			    
										PID_CC2_discharge.Derivative = 0; 					// 微分常数 Derivative Const
																			    
										PID_CC2_discharge.SetPoint   = (load_cc_I/100)*0.30*step_05_load_percent;    	// 设定目标 Desired value 330mA   
										
										open_discharge_time=0x00;
										
										ms_cnt=0x00;						
										se_cnt=0x00;						
										mi_cnt=0x00;						
										ho_cnt=0x00;
										limit_power_pid_flag=0x00;
										Adjust_Pout_Flag=0x00;
										voltage_adjust_index=0x01;
										
										nop_index=0x01;
											
										discharge1_index=0x00;
										discharge2_index=0x00;
											
										charge_index=0x00;
									}
									
									if(device_status==0x00 && nop_index==0x01)
									{
										
										Disable_Buck_Pwm();								
										Set_Buck_Pwm_Duty(0);
										Disable_Bost_Pwm1();
										LOAD_OUT1_DISABLE;
										Set_Bost_Pwm1_Duty(0);
										Disable_Bost_Pwm2();
										LOAD_OUT2_DISABLE;
										Set_Bost_Pwm2_Duty(0);
										
										LOAD_OUT1_ENABLE;																																	
										Enable_Bost_Pwm1();											
										LOAD_OUT2_ENABLE;																																	
										Enable_Bost_Pwm2();
										
										Set_Bost_Pwm1_Duty(200);
										Set_Bost_Pwm2_Duty(200);
										
										delay_ms(200);
										
										Set_Bost_Pwm1_Duty(0);
										Set_Bost_Pwm2_Duty(0);
										
										delay_ms(200);
										
										Set_Bost_Pwm1_Duty(200);
										Set_Bost_Pwm2_Duty(200);
										
										delay_ms(200);
										
										Set_Bost_Pwm1_Duty(0);
										Set_Bost_Pwm2_Duty(0);
										
										delay_ms(200);
										
										Set_Bost_Pwm1_Duty(200);
										Set_Bost_Pwm2_Duty(200);
										
										delay_ms(200);
										
										Set_Bost_Pwm1_Duty(0);
										Set_Bost_Pwm2_Duty(0);
										
										Disable_Bost_Pwm1();
										LOAD_OUT1_DISABLE;
										Set_Bost_Pwm1_Duty(0);
										Disable_Bost_Pwm2();
										LOAD_OUT2_DISABLE;
										Set_Bost_Pwm2_Duty(0);
										
										nop_index=0x02;
										
										key1_index=0x00;
										
										POWER_OFF;  //进入低功耗模式
										
										open_sleep_time = 0x01;
										
										SLEEP();										
										
										NOP();
										NOP();
										NOP();
									}
									
									
								
									break;
		
		case SYSTEM_CHARGE_STATUS:	
										if(charge_index==0x01)
										{
											
											discharge_led_status=0x01;
											
																						
											Disable_Bost_Pwm1();
											Disable_Bost_Pwm2();
											
											Set_Bost_Pwm1_Duty(0);
											Set_Bost_Pwm2_Duty(0);
											
											LOAD_OUT1_DISABLE;
											LOAD_OUT2_DISABLE;											
											
											Disable_Buck_Pwm();
											
											PWM	=BUCK_PWM_MIN;
											
											buck_pwm_value=BUCK_PWM_MIN;
											
											Set_Buck_Pwm_Duty(buck_pwm_value);		//扰动跟踪 初始占空比
											
											#if defined(TEST_CHARGE)
											
											Enable_Buck_Pwm();
											
											buck_pwm_value=900;
											
											Set_Buck_Pwm_Duty(buck_pwm_value);		
											
											while(1)
											{
												CLRWDT();
											}
											
											#endif
										
											
											pid_init ( &PID_CC_charge);				//恒流PID初始化
											
											pid_init ( &PID_CV_charge);  			//恒压PID参数初始化
											
											
											PID_CC_charge.Proportion = 0.001;    	// 比例常数 Proportional Const
	
											PID_CC_charge.Integral   = 0; 	// 积分常数 Integral Const
																			    
											PID_CC_charge.Derivative = 0; 		// 微分常数 Derivative Const
																				    
											PID_CC_charge.SetPoint   = 5.0*15.0;   		// 设定目标 Desired value  1000mA
											
											
											
											PID_CV_charge.Proportion = 0.001;    	// 比例常数 Proportional Const
											
										    PID_CV_charge.Integral   = 0; 	// 积分常数 Integral Const
										    
										    PID_CV_charge.Derivative = 0; 		// 微分常数 Derivative Const
										    
										    PID_CV_charge.SetPoint   = dianchi_cv_level+1;    		// 设定目标 Desired value    --恒压设定13.85V
										    
											
											P_prev=0;
											
											P_last=0;
											
											open_discharge_time=0x00;
											
											ms_cnt=0x00;						
											se_cnt=0x00;						
											mi_cnt=0x00;						
											ho_cnt=0x00;
											limit_power_pid_flag=0x00;
											Adjust_Pout_Flag=0x00;
											voltage_adjust_index=0x01;
											nop_index=0x00;
											
											discharge1_index=0x00;
											discharge2_index=0x00;
											
											charge_index=0x02;
											
											//--------------------------
											today_index = today_index+1;
											 
											today_index = today_index%8;
											
											eeprom_write(0x11,(unsigned char)today_index);
										
											bt_00_lv = b_v;
											//--------------------------
										}
										break;
		
		case SYSTEM_DISCHARGE_STATUS:
																		
										if(discharge1_index==0)
										{
											
											charge_led_status=0x01;											
																														
											Disable_Buck_Pwm();
								
											Set_Buck_Pwm_Duty(0);
											
											Set_Bost_Pwm1_Duty(0);
											
											LOAD_OUT1_DISABLE;
											
											Disable_Bost_Pwm1();
											
											PWM	=50;
											
											bost_pwm1_value=50;
										
											Set_Bost_Pwm1_Duty(bost_pwm1_value); 
											
											 		
											
											#if defined(TEST_DISCHARGE)
											
											LOAD_OUT1_ENABLE;
											
											Enable_Bost_Pwm1();
											
											bost_pwm1_value=480;
										
											Set_Bost_Pwm1_Duty(bost_pwm1_value);  		
											
											while(1)
											{
												CLRWDT();
											}
											
											#endif
											
											
											pid_init ( &PID_CC_discharge);			//初始化PID控制参数
											
											
											PID_CC_discharge.Proportion = 0.001;  	// 比例常数 Proportional Const
    
										    PID_CC_discharge.Integral   = 0; 		// 积分常数 Integral Const
										    
										    PID_CC_discharge.Derivative = 0; 		// 微分常数 Derivative Const
										    
										    PID_CC_discharge.SetPoint   = (load_cc_I/100)*30;   	// 设定目标 Desired value 默认330mA  
										    
										    open_discharge_time=0x00;
										    
										    ms_cnt=0x00;						
											se_cnt=0x00;						
											mi_cnt=0x00;						
											ho_cnt=0x00;
										    limit_power_pid_flag=0x00;
											Adjust_Pout_Flag=0x00;
											voltage_adjust_index=0x01;										
											nop_index=0x00;
											
											discharge1_index=0x01;
											
											charge_index=0x00;
											
											//------------------------------------保存今天电池电压记录
											bt_00_hv = b_v;
											
											eeprom_write((0x12+4*( (today_index+8-0)%8 )+0),(unsigned char)(bt_00_hv/256));
			
											eeprom_write((0x12+4*( (today_index+8-0)%8 )+1),(unsigned char)bt_00_hv);
											
											eeprom_write((0x12+4*( (today_index+8-0)%8 )+2),(unsigned char)(bt_00_lv/256));
			
											eeprom_write((0x12+4*( (today_index+8-0)%8 )+3),(unsigned char)bt_00_lv);
											
											
											//-----------------------------------
										}
										//--------------------------------------------------
										if(discharge2_index==0)
										{
											
											charge_led_status=0x01;
											
																														
											Disable_Buck_Pwm();
								
											Set_Buck_Pwm_Duty(0);
											
											Set_Bost_Pwm2_Duty(0);
											
											LOAD_OUT2_DISABLE;
											
											Disable_Bost_Pwm2();
											
											PWM2	=50;
											
											bost_pwm2_value=50;
										
											Set_Bost_Pwm2_Duty(bost_pwm2_value); 
											
											 		
											
											#if defined(TEST_DISCHARGE)
											
											LOAD_OUT2_ENABLE;
											
											Enable_Bost_Pwm2();
											
											bost_pwm2_value=480;
										
											Set_Bost_Pwm2_Duty(bost_pwm2_value);  		
											
											while(1)
											{
												CLRWDT();
											}
											
											#endif
											
											
											pid_init ( &PID_CC2_discharge);			//初始化PID控制参数
											
											
											PID_CC2_discharge.Proportion = 0.001;  	// 比例常数 Proportional Const
    
										    PID_CC2_discharge.Integral   = 0; 		// 积分常数 Integral Const
										    
										    PID_CC2_discharge.Derivative = 0; 		// 微分常数 Derivative Const
										    
										    PID_CC2_discharge.SetPoint   = (load_cc_I/100)*0.30*step_05_load_percent;   	// 设定目标 Desired value 默认330mA  
										    
										    open_discharge_time=0x00;
										    
										    ms_cnt=0x00;						
											se_cnt=0x00;						
											mi_cnt=0x00;						
											ho_cnt=0x00;
										    limit_power_pid_flag=0x00;
											Adjust_Pout_Flag=0x00;
											voltage_adjust_index=0x01;										
											nop_index=0x00;
											
											discharge2_index=0x01;
											
											charge_index=0x00;
											
											//------------------------------------保存今天电池电压记录
											bt_00_hv = b_v;
											
											eeprom_write((0x12+4*( (today_index+8-0)%8 )+0),(unsigned char)(bt_00_hv/256));
			
											eeprom_write((0x12+4*( (today_index+8-0)%8 )+1),(unsigned char)bt_00_hv);
											
											eeprom_write((0x12+4*( (today_index+8-0)%8 )+2),(unsigned char)(bt_00_lv/256));
			
											eeprom_write((0x12+4*( (today_index+8-0)%8 )+3),(unsigned char)bt_00_lv);
											
											
											//-----------------------------------
										}
		
										break;
		
		default:break;
	
	}
	

}


void Mppt_Charge(void)
{
	
					if((battery_voltage_value<dianchi_guoya_huifu)&&(charge_index==0x03))  //MPPT跟踪充电      -----12.35 进入 MPPT充电  362
					{
						
						if(mppt_status==0x00)
						{
								if (charge_current_value>=I_prev)  
								{
							
										buck_pwm_value=buck_pwm_value +  200 * 0.025;//400 * 0.025;
										
											if(buck_pwm_value>=BUCK_PWM_MAX)
											{
												buck_pwm_value=BUCK_PWM_MAX;
											}
									
										Set_Buck_Pwm_Duty(buck_pwm_value);
									
								}
								else 
								{						
										mppt_status=1;
		
										buck_pwm_value=buck_pwm_value - 200 * 0.025;//400 * 0.025;
										
										if(buck_pwm_value<=BUCK_PWM_MIN)
											{
												buck_pwm_value=BUCK_PWM_MIN;
												
												mppt_status=0;//-----2018-1-17
											}
									
										Set_Buck_Pwm_Duty(buck_pwm_value);
		
								}
						}
						else
						{
								if (charge_current_value>=I_prev)  
								{
							
										buck_pwm_value=buck_pwm_value-200 * 0.025;//400 * 0.025;
										
										if(buck_pwm_value<=BUCK_PWM_MIN)
											{
												buck_pwm_value=BUCK_PWM_MIN;
												
												mppt_status=0;//-----2018-1-17
											}
									
										Set_Buck_Pwm_Duty(buck_pwm_value);
										
										
									
								}
								else 
								{						
										mppt_status=0;
		
										buck_pwm_value=buck_pwm_value+  200 * 0.025;//400 * 0.025;
										
											if(buck_pwm_value>=BUCK_PWM_MAX)
											{
												buck_pwm_value=BUCK_PWM_MAX;
											}
									
										Set_Buck_Pwm_Duty(buck_pwm_value);
		
								}

								

						}
					
						PWM	= buck_pwm_value;//------------caiwk-2018-06-23	
										
						cv_level_time_count=0x00;	
					
					}
					else if((battery_voltage_value>=dianchi_cv_level)&&(battery_voltage_value<dianchi_guoya_baohu)&&(charge_index==0x03)) //CV恒压充电  12.4--12.6
					{
							cv_level_time_count++;
							
							if(cv_level_time_count>=100)
							{
								cv_level_time_count=0x00;
								
								charge_index=0x04;
							
								PWM	= buck_pwm_value;//------------caiwk-2018-06-23
							}
							
					}
					else if( (battery_voltage_value<dianchi_guoya_baohu) && (charge_index==0x04) ) //CV恒压充电  12.4--12.6
					{
							pid_index = 0x02;
							
							PWM_I=pid_calc(&PID_CV_charge,battery_voltage_value);							
							
				           	PWM	 = PWM+PWM_I;
				           	
				           if( PWM>=BUCK_PWM_MAX)
				           {
				           		PWM=BUCK_PWM_MAX;
				           }			           
				           else if(PWM<=BUCK_PWM_MIN) 
				           {
				           		PWM=BUCK_PWM_MIN;
				           }
				           buck_pwm_value = (unsigned int)PWM;
				           
				           Set_Buck_Pwm_Duty(buck_pwm_value);
					}
					else
					{
						cv_level_time_count=0x00;
					}

}



void CC_CV_Charge(void)
{

					if((battery_voltage_value<=dianchi_guoya_huifu)&&(charge_index==0x03))     //CC恒流充电   7.5--12.3  
					{
							PWM_I=pid_calc(&PID_CC_charge,charge_current_value);														
							
				           	PWM	 = PWM+PWM_I;
				           	
				           if( PWM>=BUCK_PWM_MAX)
				           {
				           		PWM=BUCK_PWM_MAX;
				           }			           
				           else if(PWM<=BUCK_PWM_MIN) 
				           {
				           		PWM=BUCK_PWM_MIN;
				           }
				           
				           #if defined(PRINT_PWM)
			
							sprintf(buf, "PWM=%3.2f.....AN3_CHANNEL!\n", PWM);
						
							uart1_send_nbyte(buf, strlen(buf));		
							
							#endif
							
				           buck_pwm_value = (unsigned int)PWM;	
				           
				         	           
				           Set_Buck_Pwm_Duty(buck_pwm_value);
				           
				           
					}
					else if((battery_voltage_value>=dianchi_cv_level)&&(battery_voltage_value<dianchi_guoya_baohu)&&(charge_index==0x03)) //CV恒压充电  12.4--12.6
					{
							charge_index=0x04;
							
							PWM	= buck_pwm_value;//------------caiwk-2018-06-23
					}
					else if((battery_voltage_value>=dianchi_guoya_huifu)&&(battery_voltage_value<dianchi_guoya_baohu)&&(charge_index==0x04)) //CV恒压充电  12.4--12.6
					{
							PWM_I=pid_calc(&PID_CV_charge,battery_voltage_value);
							
				           	PWM	 = PWM+PWM_I;
				           	
				           if( PWM>=BUCK_PWM_MAX)
				           {
				           		PWM=BUCK_PWM_MAX;
				           }			           
				           else if(PWM<=BUCK_PWM_MIN) 
				           {
				           		PWM=BUCK_PWM_MIN;
				           }
				           buck_pwm_value = PWM;
				           
				           Set_Buck_Pwm_Duty(buck_pwm_value);
					}
					else NOP();
					
					
					
					
					
					
}
