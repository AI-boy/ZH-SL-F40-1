
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"



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


unsigned char 	open_an10_time=0x00; 				//AN10 通道采样周期定时器
unsigned int   	an10_time_count=0x00;				
unsigned char  	handle_an10_process_flag=0x00;

unsigned char 	handle_short_flag=0x00;

unsigned  int   time_cnt=0x00;


/**********************************************************************
*函数名称：PinInterrupt_ISR
*描述：管脚中断
*返回类型：void
*输入形参：void
***********************************************************************/
void PinInterrupt_ISR(void) interrupt 7 //管脚中断 中断号：7
{
	if(PIF!=0x00)	 //端口引脚中断
	{
		if(PIF&0x02) //过流保护
		{
			handle_short_flag=0x01;
     		    		     	
	     	discharge_index=0x03;
	     		
	     	LOAD_OUT_DISABLE;
											
			bost_pwm_value=2;
								
			Set_Bost_Pwm_Duty(bost_pwm_value);
								
			Disable_Bost_Pwm();
				
			clr_PIF1;
		}

		if(PIF&0x04) //遥控器红外接收
		{
			//unsigned   int   time_cnt;
  
		    ird_receive_time_count=150;//100ms超时时间
		    
		    if((receive_bit_count<32)&&(ird_receive_flag==0x00))
		    {
		          if(ird_status==0x00)
		          {
		              ird_status=0x01;
		              
		              clr_TR1;   //停止计数 
					               
			          TL1=0x00;
			  		  TH1=0x00;		
					                
		              set_TR1;                         
		              
		              system_time_count=0x00;             
		              
		          }
		          else if(ird_status==0x01)
		          {
				  		clr_TR1;   //停止计数
		                time_cnt=(unsigned int)(TH1<<8)+TL1;
		                TL1=0x00;
			  		  	TH1=0x00;

		                if((time_cnt>10000)&&(time_cnt<30000))	//13.5ms 通信起始周期
		                {
		                    receive_bit_count=0x00; //数据计数清零
		                      
		                    ird_data=0x00;

							clr_TR1;   //停止计数
		                    TL1=0x00;
			  		  		TH1=0x00;
							set_TR1;
							clr_TF1;
		                 
		                }
		                else if((time_cnt>500)&&(time_cnt<=2000)) //数据 ‘0’  0.5ms--1.5ms
		                {
		                    receive_bit_count++;
		                    
		                    ird_data=ird_data<<1;

							clr_TR1;   //停止计数
		                    TL1=0x00;
			  		  		TH1=0x00;
							set_TR1;
							clr_TF1;
		                    
		                }
		                else if((time_cnt>2000)&&(time_cnt<4000)) //数据 ‘1’	1.5ms--3ms
		                {
		                    receive_bit_count++;
		                    
		                    ird_data=(ird_data<<1)+1;

							clr_TR1;   //停止计数
		                    TL1=0x00;
			  		  		TH1=0x00;
							set_TR1;
							clr_TF1;
		                    
		                }
						
		          }
		    }

			clr_PIF2;
		}

		PIF=0x00;  //----!!!
	}

}

/**********************************************************************
*函数名称：Timer3_ISR
*描述：系统时基中断	1ms
*返回类型：void
*输入形参：void
***********************************************************************/
void Timer3_ISR (void) interrupt 16 
{
	clr_TF3;

	//------------------------------------------------------------	
    if(system_check_time==0x01)
    {
        system_time_count++;
        
        if(system_time_count>800)
        {
            system_time_count=0x00;           
            
            if(have_person1_flag==0x01)
            {
                have_person1_time_count++;
                                                                  
                if(have_person1_time_count>20)
                {
                    have_person1_time_count=0x00;
                                                                          
                    have_person1_flag=0x00;
                }
                
            }
            
         } 
 
        scan_key_time_count++;
        
        if(scan_key_time_count>=40)    //50ms扫描按键
        {
            scan_key_time_count=0x00;
                                                          
            handle_scan_key_flag=0x01;
        }
 
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
				se_cnt++;       //----1秒
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
						ho_cnt++; //----1小时
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
    if(open_an0_time==0x01) //AN0 通道采样周期定时器  1s  内部温度检测
    {
		an0_time_count++;
						
		if(an0_time_count>=1000)
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
					
    if(open_an4_time==0x01) //AN4 通道采样周期定时器  5ms 输出端电压 
    {
		an4_time_count++;
						
		if(an4_time_count>=5)
		{
            an4_time_count=0x00;
							
            handle_an4_process_flag=0x01;
		}
						
    }
					
    if(open_an10_time==0x01) //AN10 通道采样周期定时器  5ms  放电电流
    {
		an10_time_count++;
						
		if(an10_time_count>=2)
		{
            an10_time_count=0x00;
							
            handle_an10_process_flag=0x01;
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
		              
				clr_TR1;   //停止计数
                
	            TL1=0x00;
	  			TH1=0x00;//定时器清零    
	                                        
	            clr_TF1;                  
		              
            }
        }
    }

	    
}


