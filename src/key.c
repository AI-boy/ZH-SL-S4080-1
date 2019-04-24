  
//**--------------ͷ�ļ�-------------------------------------------------------
#include <xc.h>
#include <GenericTypeDefs.h>

#include "userdefine.h"

//**--------------ȫ�ֱ���-----------------------------------------------------

//**--------------��������-----------------------------------------------------

//**--------------��������-----------------------------------------------------


KEY_STATE_NU  KEY_STATE =KEY_UP;

KEY_VALUE_NU  KEY_VALUE =KNOP;


unsigned char scan_key_time_count=0x00;

unsigned char handle_scan_key_flag=0x00;

unsigned char have_person1_flag=0x00;

unsigned char have_person1_time_count=0x00;

unsigned char have_person2_flag=0x00;

unsigned char have_person2_time_count=0x00;

unsigned char device_status=0x00;

unsigned char key1_time_count=0x00;

unsigned char key1_index=0x00;


/**********************************************************************
*�������ƣ�init_key
*��������ʼ������KEY�˿�  ����Ϊ����ģʽ
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void init_key(void)
{
  	K1_TRIS  = 1; 
	K2_TRIS  = 1;
 	K3_TRIS  = 1; 
  	K4_TRIS  = 1; 
  

}

/**********************************************************************
*�������ƣ�scan_key_value
*������ɨ�谴��״̬
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void scan_key_value(void)
{
	unsigned int key_temp;
	
	key_temp=0x00;
	
	key_temp=(key_temp<<1)|K1_GET;
	
	key_temp=(key_temp<<1)|K2_GET;
	
	key_temp=(key_temp<<1)|K3_GET;
	
	key_temp=(key_temp<<1)|K4_GET;
	
	
	
	switch(key_temp)
	  {
	     case 0b1111: KEY_STATE =KEY_UP;break;
	     
	     case 0b0111: KEY_VALUE =K1;KEY_STATE =KEY_DOWN;break;  //K1 ����   
	
	     case 0b1011: KEY_VALUE =K2;KEY_STATE =KEY_DOWN;break;  //K2 ����   
	 
	     case 0b1101: KEY_VALUE =K3;KEY_STATE =KEY_DOWN;break;  //K3 ����	
	
	     case 0b1110: KEY_VALUE =K4;KEY_STATE =KEY_DOWN;break;  //K4 ����	
	
	     default:  KEY_STATE =KEY_DOWN;break;
	    
	  }
	  
	  	  
	  if(KEY_STATE==KEY_UP)    //������������Ч
	    {
	      if(KEY_VALUE!=KNOP)  //�ͷŰ���
	       {
		        switch(KEY_VALUE)
		        {
			        	case K1:
			        			NOP();	
			        			key1_index=0x00;				        	
			        			break;
			        	case K2://--------------------------------------����ģʽ
				        		if(device_status==0x01) 
		    					{
		    						light_mode =0x00;
		    						eeprom_write(0x36,(unsigned char)(light_mode));
		    						
		    						light2_mode =0x00;
		    						eeprom_write(0x37,(unsigned char)(light2_mode));
		    						
		    						led_out_ctrl=0x00;
									eeprom_write(0x39,(unsigned char)(led_out_ctrl));
		    						
		    						work_mode =0x00;
							
									lr_cnt = 0x00;
									
									nop_index=0x00;
									discharge1_index=0x00;
									discharge2_index=0x00;											
									charge_index=0x00;							
									
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
														
									delay_ms(1000);
									
									Set_Bost_Pwm1_Duty(0);
									Set_Bost_Pwm2_Duty(0);
									
									Disable_Bost_Pwm1();
									LOAD_OUT1_DISABLE;
									Set_Bost_Pwm1_Duty(0);
									Disable_Bost_Pwm2();
									LOAD_OUT2_DISABLE;
									Set_Bost_Pwm2_Duty(0);
									
									
									pv_voltage_status=0x00;  //����״̬
									
									day_time_count=0x00;
									
									night_time_count=0x00;
									
									pv_time_count=0x00;    					
		    					}
		    					break;
			        	case K3:
			        			if(device_status==0x01) 
		    					{
		    						light_mode =0x01;
		    						eeprom_write(0x36,(unsigned char)(light_mode));
		    						
		    						light2_mode =0x01;
		    						eeprom_write(0x37,(unsigned char)(light2_mode));
		    						
		    						led_out_ctrl=0x00;
									eeprom_write(0x39,(unsigned char)(led_out_ctrl));
		    						
		    						work_mode =0x00;
							
									lr_cnt = 0x00;
									
									nop_index=0x00;
									discharge1_index=0x00;
									discharge2_index=0x00;											
									charge_index=0x00;							
									
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
														
									delay_ms(1000);
									
									Set_Bost_Pwm1_Duty(0);
									Set_Bost_Pwm2_Duty(0);
									
									Disable_Bost_Pwm1();
									LOAD_OUT1_DISABLE;
									Set_Bost_Pwm1_Duty(0);
									Disable_Bost_Pwm2();
									LOAD_OUT2_DISABLE;
									Set_Bost_Pwm2_Duty(0);
									
									
									pv_voltage_status=0x00;  //����״̬
									
									day_time_count=0x00;
									
									night_time_count=0x00;
									
									pv_time_count=0x00;    					
		    					}
			        			break;
			        	case K4:
			        			if(device_status==0x01) 
		    					{
		    						light_mode =0x02;
		    						eeprom_write(0x36,(unsigned char)(light_mode));
		    						
		    						light2_mode =0x02;
		    						eeprom_write(0x37,(unsigned char)(light2_mode));
		    						
		    						led_out_ctrl=0x00;
									eeprom_write(0x39,(unsigned char)(led_out_ctrl));
		    						
		    						work_mode =0x00;
							
									lr_cnt = 0x00;
									
									nop_index=0x00;
									discharge1_index=0x00;
									discharge2_index=0x00;											
									charge_index=0x00;							
									
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
														
									delay_ms(1000);
									
									Set_Bost_Pwm1_Duty(0);
									Set_Bost_Pwm2_Duty(0);
									
									Disable_Bost_Pwm1();
									LOAD_OUT1_DISABLE;
									Set_Bost_Pwm1_Duty(0);
									Disable_Bost_Pwm2();
									LOAD_OUT2_DISABLE;
									Set_Bost_Pwm2_Duty(0);
									
									
									pv_voltage_status=0x00;  //����״̬
									
									day_time_count=0x00;
									
									night_time_count=0x00;
									
									pv_time_count=0x00;    					
		    					}
			        			break;
			        	default:break;
		        }
				
	         KEY_VALUE=KNOP;
	         
	       }
	
	    }
	    
	    if((KEY_VALUE ==K1)&&(KEY_STATE == KEY_DOWN)&&(key1_index==0x00))     //	��������������
	    {
	    	if(key1_time_count<80)
			{
				 key1_time_count++;
							
			}
			else
			{
				key1_time_count=0x00;
				
				device_status^=0x01;
				
				eeprom_write(0x38,(unsigned char)(device_status));	
				
				key1_index=0x01;
				
				if(device_status==0x00)
				{
					work_mode =0x04;					
					
					lr_cnt = 0x00;
					
					led_out_ctrl=0x00;
					eeprom_write(0x39,(unsigned char)(led_out_ctrl));
					
					nop_index=0x00;
					discharge1_index=0x00;
					discharge2_index=0x00;											
					charge_index=0x00;
				}
				else
				{
					work_mode =0x00;
					
					POWER_ON;  
					
					open_sleep_time=0x00;
					sleep_time_count=0x00;
					lr_cnt = 0x00;
					
					nop_index=0x00;
					discharge1_index=0x00;
					discharge2_index=0x00;											
					charge_index=0x00;
					
					led_out_ctrl=0x00;
					eeprom_write(0x39,(unsigned char)(led_out_ctrl));
					
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
										
					delay_ms(1000);
					
					Set_Bost_Pwm1_Duty(0);
					Set_Bost_Pwm2_Duty(0);
					
					Disable_Bost_Pwm1();
					LOAD_OUT1_DISABLE;
					Set_Bost_Pwm1_Duty(0);
					Disable_Bost_Pwm2();
					LOAD_OUT2_DISABLE;
					Set_Bost_Pwm2_Duty(0);
					
					
					pv_voltage_status=0x00;  //����״̬
					
					day_time_count=0x00;
					
					night_time_count=0x00;
					
					pv_time_count=0x00;
				}
				
			}
	    }
	    else
	    {
	    	key1_time_count=0x00;
	    }
	    
	    
	    if(PERSON_CHECK1)
	    {
	    		    	
	    	have_person1_time_count=0x00;
	    	
	    	have_person1_flag=0x01;
	    }
	    
	    if(PERSON_CHECK2)
	    {
	    	have_person2_time_count=0x00;
	    	
	    	//have_person2_flag=0x01;	    //�����Ӧʱ ���ε��ڶ�·������		    	
	    }
	  
}





