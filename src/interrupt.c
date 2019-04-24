

#include <xc.h>
#include <GenericTypeDefs.h>
#include"userdefine.h"



unsigned char   open_discharge_time=0x00;			//�ŵ�ʱ����ʱ��
unsigned int    ms_cnt=0x00;						//����
unsigned int	se_cnt=0x00;						//��
unsigned int	mi_cnt=0x00;						//��
unsigned int	ho_cnt=0x00;						//ʱ

unsigned char 	open_an0_time=0x00;  				//AN0 ͨ���������ڶ�ʱ��
unsigned int   	an0_time_count=0x00;				
unsigned char  	handle_an0_process_flag=0x00;


unsigned char 	open_an1_time=0x00; 				//AN1 ͨ���������ڶ�ʱ��
unsigned int   	an1_time_count=0x00;				
unsigned char  	handle_an1_process_flag=0x00;


unsigned char 	open_an2_time=0x00; 				//AN2 ͨ���������ڶ�ʱ��
unsigned int   	an2_time_count=0x00;				
unsigned char  	handle_an2_process_flag=0x00;


unsigned char 	open_an3_time=0x00; 				//AN3 ͨ���������ڶ�ʱ��
unsigned int   	an3_time_count=0x00;				
unsigned char  	handle_an3_process_flag=0x00;


unsigned char 	open_an4_time=0x00; 				//AN4 ͨ���������ڶ�ʱ��
unsigned int   	an4_time_count=0x00;				
unsigned char  	handle_an4_process_flag=0x00;


unsigned char 	open_an9_time=0x00; 				//AN9 ͨ���������ڶ�ʱ��
unsigned int   	an9_time_count=0x00;				
unsigned char  	handle_an9_process_flag=0x00;


unsigned char 	open_an8_time=0x00; 				//AN10 ͨ���������ڶ�ʱ��
unsigned int   	an8_time_count=0x00;				
unsigned char  	handle_an8_process_flag=0x00;

unsigned char 	open_an11_time=0x00; 				//AN10 ͨ���������ڶ�ʱ��
unsigned int   	an11_time_count=0x00;				
unsigned char  	handle_an11_process_flag=0x00;

unsigned char handle_short1_flag=0x00;
unsigned char handle_short2_flag=0x00;

unsigned char set_1HZ_flag=0x00;

unsigned char open_sleep_time=0x00;
unsigned int sleep_time_count=0x00;

/**********************************************************************
*�������ƣ�interrupt isr
*�������жϷ������
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void interrupt isr(void)
{
	unsigned char r1_data,r2_data;
	
	//---------------------------------------------------------------------------------------------------------------------
	if (TMR1IE && TMR1IF) // ��ʱ��1�ж� 1ms���
	{
					TMR1IF = 0x00;
					TMR1H = 0xE0;
					TMR1L = 0xC0; // ��ʱ1ms   
					
					//------------------------------------------------------------	
					if(system_check_time==0x01)
					{
						system_time_count++;
					
						if(system_time_count>800)
						{
							system_time_count=0x00;	
							
							set_1HZ_flag^=0x01;	
							
							if(have_person1_flag==0x01)
							{
								have_person1_time_count++;
								
								if(have_person1_time_count>20)
								{
									have_person1_time_count=0x00;
									
									have_person1_flag=0x00;
								}
							}
							
							if(have_person2_flag==0x01)
							{
								have_person2_time_count++;
								
								if(have_person2_time_count>20)
								{
									have_person2_time_count=0x00;
									
									have_person2_flag=0x00;
								}
							}															
							
							if(open_sleep_time==0x01 && device_status==0x00)
							{
								sleep_time_count++;
								
								if(sleep_time_count>=20)
								{
									sleep_time_count=0x00;
									
									SLEEP();										
										
									NOP();
									NOP();
									NOP();
								}
							}
						}
						//---------------------------------------
						scan_key_time_count++;
						
						if(scan_key_time_count>=40)    //50msɨ�谴��
						{
							scan_key_time_count=0x00;
							
							handle_scan_key_flag=0x01;
						}
						//----------------------------------------
					}
					//------------------------------------------------------------�ŵ�׶�ʱ������
					if(open_discharge_time==0x01)
					{
						if(ms_cnt<1000)
						{
							ms_cnt++;
						}
						else
						{
							ms_cnt=0x00;
							
							if(se_cnt<60)
							{
								se_cnt++;		//----1��
							}
							else
							{
								se_cnt=0x00;
								
								if(mi_cnt<60)
								{
									mi_cnt++;  //-----1����
								}
								else
								{
									mi_cnt=0x00;
									
									if(ho_cnt<24)
									{
										ho_cnt++; //1Сʱ
									}
									else
									{
										ho_cnt=0x00;
									}
								}
							}
						}
					}
					//------------------------------------------------------------
					if(open_an0_time==0x01) //AN0 ͨ���������ڶ�ʱ��  2ms  ��һ·���ص���
					{
						an0_time_count++;
						
						if(an0_time_count>=2)
						{
							an0_time_count=0x00;
							
							handle_an0_process_flag=0x01;
						}
						
					}
					if(open_an1_time==0x01)	//AN1 ͨ���������ڶ�ʱ��   1s �¶ȼ��
					{
						an1_time_count++;
						
						if(an1_time_count>=1000)
						{
							an1_time_count=0x00;
							
							handle_an1_process_flag=0x01;
						}
						
					}
					if(open_an2_time==0x01) //AN2 ͨ���������ڶ�ʱ��  5ms  �����ѹ
					{
						an2_time_count++;
						
						if(an2_time_count>=5)
						{
							an2_time_count=0x00;
							
							handle_an2_process_flag=0x01;
						}
						
					}
					
					if(open_an3_time==0x01) //AN3 ͨ���������ڶ�ʱ��  5ms  ��ص�ѹ���
					{
						an3_time_count++;
						
						if(an3_time_count>=5)
						{
							an3_time_count=0x00;
							
							handle_an3_process_flag=0x01;
						}
						
					}
									
					
					if(open_an9_time==0x01) //AN9 ͨ���������ڶ�ʱ��   5ms ������  
					{
						an9_time_count++;
						
						if(an9_time_count>=5)
						{
							an9_time_count=0x00;
																			
							handle_an9_process_flag=0x01;
							
						
						}
						
					}
					
					if(open_an4_time==0x01) //AN4 ͨ���������ڶ�ʱ��  5ms ��һ·����˵�ѹ 
					{
						an4_time_count++;
						
						if(an4_time_count>=2)
						{
							an4_time_count=0x00;
							
							handle_an4_process_flag=0x01;
						}
						
					}
					
					if(open_an8_time==0x01) //AN8 ͨ���������ڶ�ʱ��  2ms �ڶ�·���ص���
					{
						an8_time_count++;
						
						if(an8_time_count>=2)
						{
							an8_time_count=0x00;
							
							handle_an8_process_flag=0x01;
						}
						
					}
					
					if(open_an11_time==0x01) //AN11 ͨ���������ڶ�ʱ��  5ms �ڶ�·�����ѹ
					{
						an11_time_count++;
						
						if(an11_time_count>=2)
						{
							an11_time_count=0x00;
							
							handle_an11_process_flag=0x01;
						}
						
					}
					//------------------------------------------------------
		
					if (uart1_time_count > 0x00)   //UART1���ܽ�������
					{
						uart1_time_count--;
						
						if(uart1_time_count==0)
						{
							if((uart1_receive_byte_count==5)&&(uart1_rx_buf[1]==0xA1)&&(uart1_rx_buf[4]==0xAA))
							{
								rx_frame_kind=0x01;				//������������
								
								uart1_receive_flag=TRUE;
							}
							else if((uart1_receive_byte_count==5)&&(uart1_rx_buf[1]==0xA2)&&(uart1_rx_buf[4]==0xAA))
							{
								rx_frame_kind=0x02;				//��������
								
								uart1_receive_flag=TRUE;
							}
							else if((uart1_receive_byte_count==21)&&(uart1_rx_buf[1]==0xA3)&&(uart1_rx_buf[20]==0xAA))
							{
								rx_frame_kind=0x03;				//��������
								
								uart1_receive_flag=TRUE;
							}
							else if((uart1_receive_byte_count==5)&&(uart1_rx_buf[1]==0xA4)&&(uart1_rx_buf[4]==0xAA))
							{
								rx_frame_kind=0x04;				//�ֶ����ģʽ
								
								uart1_receive_flag=TRUE;
							}
							else if((uart1_receive_byte_count==5)&&(uart1_rx_buf[1]==0xA5)&&(uart1_rx_buf[4]==0xAA))
							{
								rx_frame_kind=0x05;				//�ֶ��ŵ�ģʽ
								
								uart1_receive_flag=TRUE;
							}
							else if((uart1_receive_byte_count==5)&&(uart1_rx_buf[1]==0xA6)&&(uart1_rx_buf[4]==0xAA))
							{
								rx_frame_kind=0x06;				//�Զ�ģʽ
								
								uart1_receive_flag=TRUE;
							}
							else
							{
									if (RCSTAbits.FERR || RCSTAbits.OERR)  //uart2 RS485
										{
											RCSTAbits.CREN = 0x00;
											RCSTAbits.SPEN = 0x00;
											NOP();
											NOP();
											NOP();
											RCSTAbits.CREN = 0x01;
											RCSTAbits.SPEN = 0x01;				
										}
			
										uart1_receive_byte_count=0x00;
							}
						}
						
					}				    		
			    		
		
					if (uart_send_delay_timer > 0x00)   //��ʱ����
					{
						uart_send_delay_timer--;
						
						if (uart_send_delay_timer == 0x00)
						{							
							TXIE = 0x01; // ʹ��UART1�����ж�
						}
						
					}
					
					if (uart_send_end_flag == 0x01)    //�������������������
			       	{
				       	if (TXSTAbits.TRMT) 			// ���ע��
			           	{
			            				
							uart_send_end_flag = 0x00;  
							
			          	}
			          	
			      	}
			      	
		
		//-----------------------------------
		 if(ird_receive_time_count>0)
		   {
		      ird_receive_time_count--;
		      
		      if(ird_receive_time_count==0x00)
		      {
		          if(receive_bit_count==32)
		          {
		              ird_receive_flag=0x01;
		              
		              receive_bit_count=0x00;
		          }
		          else
		          {
		              receive_bit_count=0x00;
		              
		              ird_status=0x00;
		              
		              TMR0=0;//��ʱ������
			              
			          TMR0IF = 0;            
		              
		          }
		      }
		   }
		
			      	
			      	
	}


	//---------------------------------------------------------------------------------------------------------------------	
	if (TXIE && TXIF) // �����ж� 
	{
		if (uart_send_byte_count > 0x00)
       	{
          	uart_send_byte_count--;
           	TXREG = *uart_send_point;
           	uart_send_point++;
      	}
       	else if (uart_send_byte_count == 0x00)
      	{
          	uart_send_end_flag = 0x01;
          	TXIE = 0x00;
      	}
		else
		{
			NOP();
		}	
	}
	
	if(RCIF && RCIE)    //uart1 
     {
           
            r1_data=RCREG;
                  
             uart1_time_count=0x0A;//UART1�������� ���5ms����ʾһ֡���ݵĽ���  9600--0x05   2400--0x0A

            if( (uart1_receive_byte_count<21)&&(uart1_receive_flag==FALSE) )
              {
                 uart1_rx_buf[uart1_receive_byte_count]=r1_data;
          
                  if(uart1_rx_buf[0]==MAC_ADDR)
                    {
                       uart1_receive_byte_count++;
                    } 
               }     
          
                                    
                
     }
   
     //---------------------------------------------
     if(IOCIE && IOCIF)
     {
	     unsigned   int   time_cnt;
	     
	     if(IOCBF6)
	     {
	     	IOCBF6=0;
	     }
	     
	     	if(IOCBF7)//----------------------PRB7(PGD)��ƽ�仯�ж� �½��ش���  IrD
	     	{
		     		IOCBF7 = 0;
		     		
		     		ird_receive_time_count=150;//100ms��ʱʱ��
		    
				    if((receive_bit_count<32)&&(ird_receive_flag==0x00))
				    {
				          if(ird_status==0x00)
				          {
				              ird_status=0x01;
				              
				              TMR0=0;//��ʱ������   CLK--0.032ms
				              
				              TMR0IF = 0;
				              
				          }
				          else if(ird_status==0x01)
				          {
				                time_cnt=TMR0;		               
				                
				                if(TMR0IF==1)    //13.5ms
				                {
				                    receive_bit_count=0x00; //���ݼ�������
				                      
				                    ird_data=0x00;
				                    
				                    TMR0=0;//��ʱ������
				              
				              		TMR0IF = 0;
				              		
				              		
				                }
				                else if((time_cnt>20)&&(time_cnt<50)) //���� ��0��  1.12ms
				                {
				                    receive_bit_count++;
				                    
				                    ird_data=ird_data<<1;
				                    
				                    TMR0=0;//��ʱ������
				              
				              		TMR0IF = 0;
				                }
				                else if((time_cnt>50)&&(time_cnt<90)) //���� ��1�� 2.25ms
				                {
				                    receive_bit_count++;
				                    
				                    ird_data=(ird_data<<1)+1;
				                    
				                     TMR0=0;//��ʱ������
				              
				              		 TMR0IF = 0;
				                }
				          }
				    }
			         		    		
	     	}
     	
     	//------------------------------------------------------
	     	if(IOCBF1)  //---------DL_1 ��һ·��·�����жϴ���
	     	{
	     		IOCBF1 = 0;
	     		
	     		handle_short1_flag=0x01;
	     		   	
	     		discharge1_index=0x03;
	     		
	     		LOAD_OUT1_DISABLE;
											
				bost_pwm1_value=2;
							
				Set_Bost_Pwm1_Duty(bost_pwm1_value);
							
				Disable_Bost_Pwm1();		
	     	}
	    //------------------------------------------------------
	     	if(IOCBF0)  //---------DL_2 �ڶ�·��·�����жϴ���
	     	{
	     		IOCBF0 = 0;
	     		
	     		handle_short2_flag=0x01;
	     		   	
	     		discharge2_index=0x03;
	     		
	     		LOAD_OUT2_DISABLE;
											
				bost_pwm2_value=2;
							
				Set_Bost_Pwm2_Duty(bost_pwm2_value);
							
				Disable_Bost_Pwm2();		
	     	}
     	
     }
     
		
}

/******************************************************************************
** �������� : init_interrupt
** ������� : ��
** ������� : ��
** �������� : ��ʼ���ж�
			  ʹ�������ж�
			  ʹ��ȫ���ж�
** ȫ�ֱ��� : ��
** ��    �� : caiwk
** ��    �� : 2014-10-16
** ��    �� : ��
** ��    �� : V1.0
*******************************************************************************/
void init_interrupt(void)
{
	
	TRISBbits.TRISB0 = 1;  //RB0 ��ƽ�仯�ж� �����ش���  DL_2  �ڶ�·��·�жϴ�������
	
	TRISBbits.TRISB1 = 1;  //RB1 ��ƽ�仯�ж� �����ش���  DL_1  ��һ·��·�жϴ�������
	
	IOCBPbits.IOCBP0 = 1; //RB0 ʹ�������ش����ж�
	
	IOCBFbits.IOCBF0 = 0; //��RB0�жϱ�־  
	
	IOCBPbits.IOCBP1 = 1; //RB1 ʹ�������ش����ж�
	
	IOCBFbits.IOCBF1 = 0; //��RB1�жϱ�־  
	
	IOCIF = 0;			//���ƽ�仯�жϱ�־
	
	IOCIE = 1;			//ʹ�ܶ˿ڵ�ƽ�仯�ж�
	
	PEIE = 0x01; // ʹ�������ж�
	GIE = 0x01; // ʹ��ȫ���ж�	   
}
