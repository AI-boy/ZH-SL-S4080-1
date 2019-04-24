

#include <xc.h>
#include <GenericTypeDefs.h>
#include"userdefine.h"



unsigned char   open_discharge_time=0x00;			//放电时长定时器
unsigned int    ms_cnt=0x00;						//毫秒
unsigned int	se_cnt=0x00;						//秒
unsigned int	mi_cnt=0x00;						//分
unsigned int	ho_cnt=0x00;						//时

unsigned char 	open_an0_time=0x00;  				//AN0 通道采样周期定时器
unsigned int   	an0_time_count=0x00;				
unsigned char  	handle_an0_process_flag=0x00;


unsigned char 	open_an1_time=0x00; 				//AN1 通道采样周期定时器
unsigned int   	an1_time_count=0x00;				
unsigned char  	handle_an1_process_flag=0x00;


unsigned char 	open_an2_time=0x00; 				//AN2 通道采样周期定时器
unsigned int   	an2_time_count=0x00;				
unsigned char  	handle_an2_process_flag=0x00;


unsigned char 	open_an3_time=0x00; 				//AN3 通道采样周期定时器
unsigned int   	an3_time_count=0x00;				
unsigned char  	handle_an3_process_flag=0x00;


unsigned char 	open_an4_time=0x00; 				//AN4 通道采样周期定时器
unsigned int   	an4_time_count=0x00;				
unsigned char  	handle_an4_process_flag=0x00;


unsigned char 	open_an9_time=0x00; 				//AN9 通道采样周期定时器
unsigned int   	an9_time_count=0x00;				
unsigned char  	handle_an9_process_flag=0x00;


unsigned char 	open_an8_time=0x00; 				//AN10 通道采样周期定时器
unsigned int   	an8_time_count=0x00;				
unsigned char  	handle_an8_process_flag=0x00;

unsigned char 	open_an11_time=0x00; 				//AN10 通道采样周期定时器
unsigned int   	an11_time_count=0x00;				
unsigned char  	handle_an11_process_flag=0x00;

unsigned char handle_short1_flag=0x00;
unsigned char handle_short2_flag=0x00;

unsigned char set_1HZ_flag=0x00;

unsigned char open_sleep_time=0x00;
unsigned int sleep_time_count=0x00;

/**********************************************************************
*函数名称：interrupt isr
*描述：中断服务程序
*返回类型：void
*输入形参：void
***********************************************************************/
void interrupt isr(void)
{
	unsigned char r1_data,r2_data;
	
	//---------------------------------------------------------------------------------------------------------------------
	if (TMR1IE && TMR1IF) // 定时器1中断 1ms间隔
	{
					TMR1IF = 0x00;
					TMR1H = 0xE0;
					TMR1L = 0xC0; // 定时1ms   
					
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
						
						if(scan_key_time_count>=40)    //50ms扫描按键
						{
							scan_key_time_count=0x00;
							
							handle_scan_key_flag=0x01;
						}
						//----------------------------------------
					}
					//------------------------------------------------------------放电阶段时长控制
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
								se_cnt++;		//----1秒
							}
							else
							{
								se_cnt=0x00;
								
								if(mi_cnt<60)
								{
									mi_cnt++;  //-----1分钟
								}
								else
								{
									mi_cnt=0x00;
									
									if(ho_cnt<24)
									{
										ho_cnt++; //1小时
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
					if(open_an0_time==0x01) //AN0 通道采样周期定时器  2ms  第一路负载电流
					{
						an0_time_count++;
						
						if(an0_time_count>=2)
						{
							an0_time_count=0x00;
							
							handle_an0_process_flag=0x01;
						}
						
					}
					if(open_an1_time==0x01)	//AN1 通道采样周期定时器   1s 温度检测
					{
						an1_time_count++;
						
						if(an1_time_count>=1000)
						{
							an1_time_count=0x00;
							
							handle_an1_process_flag=0x01;
						}
						
					}
					if(open_an2_time==0x01) //AN2 通道采样周期定时器  5ms  光伏电压
					{
						an2_time_count++;
						
						if(an2_time_count>=5)
						{
							an2_time_count=0x00;
							
							handle_an2_process_flag=0x01;
						}
						
					}
					
					if(open_an3_time==0x01) //AN3 通道采样周期定时器  5ms  电池电压检测
					{
						an3_time_count++;
						
						if(an3_time_count>=5)
						{
							an3_time_count=0x00;
							
							handle_an3_process_flag=0x01;
						}
						
					}
									
					
					if(open_an9_time==0x01) //AN9 通道采样周期定时器   5ms 充电电流  
					{
						an9_time_count++;
						
						if(an9_time_count>=5)
						{
							an9_time_count=0x00;
																			
							handle_an9_process_flag=0x01;
							
						
						}
						
					}
					
					if(open_an4_time==0x01) //AN4 通道采样周期定时器  5ms 第一路输出端电压 
					{
						an4_time_count++;
						
						if(an4_time_count>=2)
						{
							an4_time_count=0x00;
							
							handle_an4_process_flag=0x01;
						}
						
					}
					
					if(open_an8_time==0x01) //AN8 通道采样周期定时器  2ms 第二路负载电流
					{
						an8_time_count++;
						
						if(an8_time_count>=2)
						{
							an8_time_count=0x00;
							
							handle_an8_process_flag=0x01;
						}
						
					}
					
					if(open_an11_time==0x01) //AN11 通道采样周期定时器  5ms 第二路输出电压
					{
						an11_time_count++;
						
						if(an11_time_count>=2)
						{
							an11_time_count=0x00;
							
							handle_an11_process_flag=0x01;
						}
						
					}
					//------------------------------------------------------
		
					if (uart1_time_count > 0x00)   //UART1接受结束鉴别
					{
						uart1_time_count--;
						
						if(uart1_time_count==0)
						{
							if((uart1_receive_byte_count==5)&&(uart1_rx_buf[1]==0xA1)&&(uart1_rx_buf[4]==0xAA))
							{
								rx_frame_kind=0x01;				//运行数据请求
								
								uart1_receive_flag=TRUE;
							}
							else if((uart1_receive_byte_count==5)&&(uart1_rx_buf[1]==0xA2)&&(uart1_rx_buf[4]==0xAA))
							{
								rx_frame_kind=0x02;				//参数请求
								
								uart1_receive_flag=TRUE;
							}
							else if((uart1_receive_byte_count==21)&&(uart1_rx_buf[1]==0xA3)&&(uart1_rx_buf[20]==0xAA))
							{
								rx_frame_kind=0x03;				//参数保存
								
								uart1_receive_flag=TRUE;
							}
							else if((uart1_receive_byte_count==5)&&(uart1_rx_buf[1]==0xA4)&&(uart1_rx_buf[4]==0xAA))
							{
								rx_frame_kind=0x04;				//手动充电模式
								
								uart1_receive_flag=TRUE;
							}
							else if((uart1_receive_byte_count==5)&&(uart1_rx_buf[1]==0xA5)&&(uart1_rx_buf[4]==0xAA))
							{
								rx_frame_kind=0x05;				//手动放电模式
								
								uart1_receive_flag=TRUE;
							}
							else if((uart1_receive_byte_count==5)&&(uart1_rx_buf[1]==0xA6)&&(uart1_rx_buf[4]==0xAA))
							{
								rx_frame_kind=0x06;				//自动模式
								
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
			    		
		
					if (uart_send_delay_timer > 0x00)   //延时发送
					{
						uart_send_delay_timer--;
						
						if (uart_send_delay_timer == 0x00)
						{							
							TXIE = 0x01; // 使能UART1发送中断
						}
						
					}
					
					if (uart_send_end_flag == 0x01)    //鉴别数据真正发送完毕
			       	{
				       	if (TXSTAbits.TRMT) 			// 务必注意
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
		              
		              TMR0=0;//定时器清零
			              
			          TMR0IF = 0;            
		              
		          }
		      }
		   }
		
			      	
			      	
	}


	//---------------------------------------------------------------------------------------------------------------------	
	if (TXIE && TXIF) // 发送中断 
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
                  
             uart1_time_count=0x0A;//UART1接收数据 相隔5ms即表示一帧数据的结束  9600--0x05   2400--0x0A

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
	     
	     	if(IOCBF7)//----------------------PRB7(PGD)电平变化中断 下降沿触发  IrD
	     	{
		     		IOCBF7 = 0;
		     		
		     		ird_receive_time_count=150;//100ms超时时间
		    
				    if((receive_bit_count<32)&&(ird_receive_flag==0x00))
				    {
				          if(ird_status==0x00)
				          {
				              ird_status=0x01;
				              
				              TMR0=0;//定时器清零   CLK--0.032ms
				              
				              TMR0IF = 0;
				              
				          }
				          else if(ird_status==0x01)
				          {
				                time_cnt=TMR0;		               
				                
				                if(TMR0IF==1)    //13.5ms
				                {
				                    receive_bit_count=0x00; //数据计数清零
				                      
				                    ird_data=0x00;
				                    
				                    TMR0=0;//定时器清零
				              
				              		TMR0IF = 0;
				              		
				              		
				                }
				                else if((time_cnt>20)&&(time_cnt<50)) //数据 ‘0’  1.12ms
				                {
				                    receive_bit_count++;
				                    
				                    ird_data=ird_data<<1;
				                    
				                    TMR0=0;//定时器清零
				              
				              		TMR0IF = 0;
				                }
				                else if((time_cnt>50)&&(time_cnt<90)) //数据 ‘1’ 2.25ms
				                {
				                    receive_bit_count++;
				                    
				                    ird_data=(ird_data<<1)+1;
				                    
				                     TMR0=0;//定时器清零
				              
				              		 TMR0IF = 0;
				                }
				          }
				    }
			         		    		
	     	}
     	
     	//------------------------------------------------------
	     	if(IOCBF1)  //---------DL_1 第一路短路保护中断触发
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
	     	if(IOCBF0)  //---------DL_2 第二路短路保护中断触发
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
** 函数名称 : init_interrupt
** 输入参数 : 无
** 输出参数 : 无
** 功能描述 : 初始化中断
			  使能外设中断
			  使能全局中断
** 全局变量 : 无
** 作    者 : caiwk
** 日    期 : 2014-10-16
** 修    改 : 无
** 版    本 : V1.0
*******************************************************************************/
void init_interrupt(void)
{
	
	TRISBbits.TRISB0 = 1;  //RB0 电平变化中断 上升沿触发  DL_2  第二路短路中断触发保护
	
	TRISBbits.TRISB1 = 1;  //RB1 电平变化中断 上升沿触发  DL_1  第一路短路中断触发保护
	
	IOCBPbits.IOCBP0 = 1; //RB0 使能上升沿触发中断
	
	IOCBFbits.IOCBF0 = 0; //清RB0中断标志  
	
	IOCBPbits.IOCBP1 = 1; //RB1 使能上升沿触发中断
	
	IOCBFbits.IOCBF1 = 0; //清RB1中断标志  
	
	IOCIF = 0;			//清电平变化中断标志
	
	IOCIE = 1;			//使能端口电平变化中断
	
	PEIE = 0x01; // 使能外设中断
	GIE = 0x01; // 使能全局中断	   
}
