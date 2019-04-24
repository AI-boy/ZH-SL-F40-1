

#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"





unsigned char system_status=0x00;

unsigned char system_check_time=0x00;

unsigned int system_time_count=0x00;

unsigned char work_mode=0x00;


unsigned char pv_voltage_status=0x00;    		//光伏板电压状态

unsigned char battery_voltage_status=0x00;		//电池电压状态

unsigned char load_voltage_status=0x00;			//负载端状态

unsigned char discharge_index=0x00;  			//放电阶段变量

unsigned char charge_index=0x00;				//充电阶段变量

unsigned char nop_index=0x00;

unsigned int involid_time_count=0x00;

unsigned int cv_level_time_count=0x00;

unsigned int load_short_time_count=0x00;		//输出短路检测变量

unsigned int load_open_time_count=0x00;			//输出开路检测变量

unsigned int over_load_time_count=0x00;

unsigned int battery_low_time_count=0x00;


unsigned long int night_time_count=0x00;
					
unsigned long int  day_time_count=0x00;

unsigned long int pv_time_count=0x00;

unsigned long int light_time_level=0x00;




float PWM	=	0;

float PWM_I=	0;

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

float  x_v=0.0;

float  y_v=0.0;


/**********************************************************************
*函数名称：handle_ad_loop
*描述：AD轮训采样
*返回类型：void
*输入形参：void
***********************************************************************/
void handle_ad_loop(void)
{
	float data_cc_var;
		//------------------------------------------------------------------------------------------------------------------------------------------------------
		if(handle_an0_process_flag==0x01)
		{								
			handle_an0_process_flag=0x00;						
		}		
		//------------------------------------------------------------------------------------------------------------------------------------------------------
		if(handle_an1_process_flag==0x01)
		{
		
			temperature_value=start_ad_convert(AN1_CHANNEL);  		//采样通道选择	AN1		环境温度  -30℃--3      70℃--600
			
			if((temperature_value>3)&&(temperature_value<492))  	//正常工作温度范围：-30℃---70℃
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
			
			handle_an1_process_flag=0x00;
						
		}
		//---------------------------------------------------------------------------------------------------------------------- 
		if(handle_an2_process_flag==0x01)
		{
			pv_voltage_value=start_ad_convert(AN2_CHANNEL);  //采样通道选择	AN2光伏板电压  分压电阻缩减比例 1:61 
			battery_voltage_value=start_ad_convert(AN3_CHANNEL); 
			x_v = (float)pv_voltage_value/819.2;
			y_v = ((float)battery_voltage_value/819.2)*31.04;
			if(y_v>x_v)
			{
				y_v = ((float)battery_voltage_value/819.2)*31.04-x_v;
				y_v =  y_v/100.0;	//支路电流1
				x_v =  x_v/24.9;	//支路电流2
				if(y_v>=x_v)
				{
					x_v =  y_v-x_v;	 //支路电流3
					
					x_v =  x_v*100.0;
					
					y_v = (float)pv_voltage_value/819.2;
					
					x_v = y_v-x_v; 	 //TYNB-	!!!!!!
					
					y_v = ((float)battery_voltage_value/819.2)*31.04;
					
					y_v = y_v-x_v; //(YNB+)-(YNB-)=PV
					if(y_v<0)
					{
						y_v=0  ;
					}
				}
				else
				{
					x_v = x_v-y_v;
					
					x_v =  x_v*100.0;

					y_v = (float)pv_voltage_value/819.2;
					
					x_v = y_v+x_v; 	 //TYNB-	 !!!!!!
					
					y_v = ((float)battery_voltage_value/819.2)*31.04;
					
					y_v = y_v-x_v; //(YNB+)-(YNB-)=PV
					
					if(y_v<0)
					{
						y_v=0;
					}	
				}				
	
				pv_voltage_value = (y_v/61.0)*819.2;
			}
			else
			{
				pv_voltage_value = (61.0/61.0)*819.2;	
			}
			


			
			if(pv_index==0x00)
			{
				if(pv_voltage_value<=light_on_pv_level)   //鉴别白天和黑夜   通过光伏板电压检测   1V---12V
				{																								
					night_time_count++;
					
					day_time_count=0x00;
					
					if(night_time_count>light_time_level)  
					{
						night_time_count=0x00;
						
						pv_voltage_status=0x02;			//进入放电模式
						
					}
						
				}
				else if((pv_voltage_value>(light_off_pv_level))) 
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
			
			if((pv_voltage_value>=light_off_pv_level)&&(pv_voltage_value<402)) 	//18V 1.8A  光伏板电压范围<30V
			{																			
				pv_time_count++;
												
				if(pv_time_count>light_time_level)
				{
					pv_time_count=0x00;
							
					pv_voltage_status=0x01; 			//进入充电模式
					
					day_time_count=0x00;  
					
					night_time_count=0x00; 
					
					pv_index=0x01;										
							
				}				
							
			}
			else if(pv_voltage_value>=402)
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
				if(pv_voltage_value<=light_on_pv_level) 									//鉴别白天和黑夜   通过光伏板电压检测   5V---10V
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
		if(handle_an9_process_flag==0x01)  		//电流检测范围：0.15A---5A  采样电阻15毫欧
		{
			charge_current_value=0x00;
			
			if(system_status ==SYSTEM_CHARGE_STATUS)
			{				
                nop();
			}			
			
			handle_an9_process_flag=0x00;
						
		}
		//------------------------------------------------------------------------------------------------------------------------------------------------------
		if(handle_an3_process_flag==0x01)
		{		
			battery_voltage_value=start_ad_convert(AN3_CHANNEL);  			//采样通道选择	AN3	电池电压   分压电阻缩减比例 1:31.04
			//-------------------------------------------------------------------			
			if((battery_voltage_value<((12.70/31.04)*819.2))&&(charge_index==0x00))	//锂电池电压是否正常？  <14.0V
			{
				bt_time_count++;
				
				if(bt_time_count>=100)
				{
					bt_time_count=0x00;
					
					bt_led_status=0x01;
					
					charge_index=0x01;
					
					battery_voltage_status=0x01;	//电池状态正常
				}					
				
			}
			else if((battery_voltage_value>=((12.70/31.04)*819.2))&&(charge_index<=0x01))
			{		
					bt_led_status=0x00;
					
					bt_time_count=0x00;
												
					battery_voltage_status=0x00;					//电池状态异常
			}
			//--------------------------------------------------------------------------------------------------
			if( (battery_voltage_value<dianchi_guoya_huifu) && (charge_index==0x02) )  		//<电池充电恢复电压
			{
								
				charge_index=0x03;
				
				charge_led_status=0x01;
                                
                CHARGE_ON;
			
			}
			else if( (battery_voltage_value>=dianchi_guoya_huifu) && (charge_index==0x02) )
			{
				
				charge_led_status=0x00;
                                
               CHARGE_OFF;
				
			}
			else if( (battery_voltage_value<dianchi_guoya_baohu) && (charge_index>=0x03) )
			{
				
				if((charge_current_value<charge_i_limit)&&(charge_index>=0x03))	    //充电电流：0---12A	
				{
					mppt_time_count++;
					
					if(mppt_time_count>5)
					{
						mppt_time_count=0x00;
						
						//Mppt_Charge();       		//MPPT充电模式
						
						//I_prev=charge_current_value;
					}					 												
							
				}
				
				involid_time_count=0x00;
				
			}
			else if( (battery_voltage_value>=(dianchi_guoya_baohu)) && (charge_index>=0x03) )  
			{
				involid_time_count++;
				
				if(involid_time_count>1000)
				{
					involid_time_count=0x00;
					
					if(charge_index>=0x03)	
					{
						charge_index=0x05;
						
						CHARGE_OFF;					 //修改过20181120	  已改回

//						CHARGE_ON;
						
						charge_led_status=0x00;
						
						while(1); //等待系统复位
					}	
				}	
			}
												
			handle_an3_process_flag=0x00;
						
		}
		//------------------------------------------------------------------------------------------------------------------------------------------------------		
		if(handle_an4_process_flag==0x01)					//分压电阻缩减比例 1:61
		{
			load_voltage_value=0x00;
			
			if(system_status ==SYSTEM_DISCHARGE_STATUS)
			{
				load_voltage_value=start_ad_convert(AN4_CHANNEL);  	//采样通道选择	AN4 负载输出端电压
			
				//---------------------------------------------------------------------------------------------------
				if((load_voltage_value<shuchu_shortout_voltage)&&(discharge_index==0x02)&&(PID_CC_discharge.SetPoint>0))     	//输出端  <输出短路电压点   可判定：1.输出端短路  2.LED灯珠选用错误
				{
                    load_short_time_count++;
						
	                if(load_short_time_count>1000)
	                {
						load_short_time_count=0x00;				
								
						bost_pwm_value=0;
								
						Set_Bost_Pwm_Duty(bost_pwm_value);
								
						discharge_led_status=0x00;
								
						delay_ms(1);			//2018-03-22
								
						Disable_Bost_Pwm();		//2018-03-22
								
						LOAD_OUT_DISABLE;		//2018-03-22
								
						while(1); //等待系统复位
	                }
						
                }
				else if((load_voltage_value>=shuchu_shortout_voltage)&&(load_voltage_value<(shuchu_shortout_voltage*2))&&(discharge_index==0x02)&&(PID_CC_discharge.SetPoint>0))     	
				{
	              	load_short_time_count++;
		
	                if(load_short_time_count>1000)
	                {
						load_short_time_count=0x00;	
								
						discharge_index=0x03;			
								
						bost_pwm_value=0;
								
						Set_Bost_Pwm_Duty(bost_pwm_value);
								
						discharge_led_status=0x00;
								
						delay_ms(1);			
								
						Disable_Bost_Pwm();		
								
						LOAD_OUT_DISABLE;		
								
						while(1); //等待系统复位
	                }
						
				}
				else
				{
                    load_short_time_count=0x00;
				}								
				//---------------------------------------------------------------------------------------------------
				if((load_voltage_value>shuchu_open_voltage)&&(discharge_index==0x02)) //输出端电压>60V+放电模式=负载端开路过压
				{
                    load_open_time_count++;
		
                    if(load_open_time_count>1000)
                    {								
                        discharge_index=0x03;
							
						load_open_time_count=0x00;			
													
						bost_pwm_value=0;
									
						Set_Bost_Pwm_Duty(bost_pwm_value);
									
						discharge_led_status=0x00;
								
						delay_ms(1);			
									
						Disable_Bost_Pwm();		
									
						LOAD_OUT_DISABLE;		
								
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
		//-----------------------------------------------------------------------------------------输出电流
		if(handle_an10_process_flag==0x01)  			//LED负载电流   采样电阻=160毫欧
		{
			discharge_current_value=0x00;
			
			if(system_status == SYSTEM_DISCHARGE_STATUS)				//系统处于放电状态
			{
				discharge_current_value=start_ad_convert(AN10_CHANNEL);  		//采样通道选择	AN10	放电电流
							
				if((discharge_current_value>discharge_i_baohu)&&(discharge_index==0x02))  	//过载保护    >2.4A
				{ 									
					over_load_time_count++;
					
					if(over_load_time_count>20)												//过载延时保护
					{										
						discharge_index=0x03;
										
						bost_pwm_value=0;
						
						Set_Bost_Pwm_Duty(bost_pwm_value);
						
						delay_ms(1);
						
						Disable_Bost_Pwm();
						
						LOAD_OUT_DISABLE;
						
						over_load_time_count=0x00;
						
						discharge_led_status=0x00;
						
						while(1); //等待系统复位
					}
					
				}
				else
				{
						over_load_time_count=0x00;
				}
				
				if((battery_voltage_value>dianchi_qianya_huifu)&&(discharge_index==0x01))   //    电池欠压恢复点
				{
					discharge_index=0x02;
					
					discharge_led_status=0x01;
					
					open_discharge_time=0x01;					
					
					LOAD_OUT_ENABLE;
																																	
					Enable_Bost_Pwm();					
					
				}
				else if((battery_voltage_value<=dianchi_qianya_huifu)&&(discharge_index==0x01))
				{
					discharge_led_status=0x00;
					
					open_discharge_time=0x00;
				}
				
				if(discharge_index==0x02)
				{
					battery_voltage_value = start_ad_convert(AN3_CHANNEL);
					
					battery_voltage_value = battery_voltage_value + (0.4/31.04)*819.2*(discharge_current_value/(((load_cc_I*150)/1000)*819.2));	
				}
				
				 if((battery_voltage_value>=dianchi_qianya_baohu)&&(discharge_index==0x02))    		//电池电压>欠压保护点
				 {
					 
				 	if((discharge_current_value<=discharge_i_limit)&&(discharge_index==0x02))  		//PID 恒流输出控制  默认350mA(330mA)   PID 调节   电流限制<2.1A
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
						if((battery_voltage_value>bt_volt_80) && (voltage_adjust_index<=0x01))
						{
							index_01_count++;
							index_02_count=0x00;
							index_03_count=0x00;
							index_04_count=0x00;
							index_05_count=0x00;
							index_06_count=0x00;
							if(index_01_count>100)
							{
								voltage_adjust_index=0x01;
								index_01_count=0x00;
							}
							
						}
						else if((battery_voltage_value>bt_volt_60) && (battery_voltage_value<=bt_volt_80) && (voltage_adjust_index<=0x02))
						{
							index_02_count++;
							index_01_count=0x00;
							index_03_count=0x00;
							index_04_count=0x00;
							index_05_count=0x00;
							index_06_count=0x00;
							if(index_02_count>100)
							{
								voltage_adjust_index=0x02;
								index_02_count=0x00;
							}
						}
						else if((battery_voltage_value>bt_volt_40) && (battery_voltage_value<=bt_volt_60) && (voltage_adjust_index<=0x03))
						{
							index_03_count++;
							index_02_count=0x00;
							index_01_count=0x00;
							index_04_count=0x00;
							index_05_count=0x00;
							index_06_count=0x00;
							if(index_03_count>100)
							{
								voltage_adjust_index=0x03;
								index_03_count=0x00;
							}
						}
						else if((battery_voltage_value>bt_volt_20) && (battery_voltage_value<=bt_volt_40) && (voltage_adjust_index<=0x04))
						{
							index_04_count++;
							index_02_count=0x00;
							index_03_count=0x00;
							index_01_count=0x00;
							index_05_count=0x00;
							index_06_count=0x00;
							if(index_04_count>100)
							{
								voltage_adjust_index=0x04;
								index_04_count=0x00;
							}
						}
						else if((battery_voltage_value>bt_volt_00) && (battery_voltage_value<=bt_volt_20) && (voltage_adjust_index<=0x05))
						{
							index_05_count++;
							index_02_count=0x00;
							index_03_count=0x00;
							index_04_count=0x00;
							index_01_count=0x00;
							index_06_count=0x00;
							if(index_05_count>100)
							{
								voltage_adjust_index=0x05;
								index_05_count=0x00;
							}
						}
						else if((battery_voltage_value<bt_volt_00) && (voltage_adjust_index<=0x06))
						{
							index_06_count++;
							index_02_count=0x00;
							index_03_count=0x00;
							index_04_count=0x00;
							index_05_count=0x00;
							index_01_count=0x00;
							if(index_06_count>100)
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
						
							PID_CC_discharge.SetPoint=((load_cc_I*150)/1000)*819.2;
							
							data_cc_var = PID_CC_discharge.SetPoint;							
													
					
						//------------------------------------------------------电量节能调节
							if(voltage_adjust_index==0x01)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*1.0;
							}
							else if(voltage_adjust_index==0x02)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.60;
							}
							else if(voltage_adjust_index==0x03)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.35;
							}
							else if(voltage_adjust_index==0x04)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.20;
							}
							else if(voltage_adjust_index==0x05)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.10;
							}
							else if(voltage_adjust_index==0x06)
							{
								PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.03;			   
							}
							
							
						//---------------------------------------------------------亮灯模式
						 if((light_mode==0x00) && (work_mode==0x00))//常亮模式
						 {
						 	if((ho_cnt==0) && (mi_cnt<30))	  			//改之前ho_cnt==0 && mi_cnt<30
						 	{
								if(voltage_adjust_index==0x05)
								{
								  	PID_CC_discharge.SetPoint = data_cc_var*0.05;		 //修改前0.01
								}
								else
								{
								   PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.5; //常亮模式
								}
						 		 
						 	}
							else
						 	{
						 		if(have_person1_flag==0x01)
							 	{
							 		if(voltage_adjust_index==0x05)
									{
									  	PID_CC_discharge.SetPoint = data_cc_var*0.05;	  //修改前0.01
									}
									else
									{
									   PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.5; //常亮模式
									} 
							 	}
							 	else
							 	{
							 		if(voltage_adjust_index==0x05)
									{
									  	PID_CC_discharge.SetPoint = data_cc_var*0.05;
									}
									else
									{
									   PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.5*0.7; //常亮模式
									} 
							 	}
						 	}
						 	
						 }
						 else if((light_mode==0x01) && (work_mode==0x00))//感应模式
						 {
						 	if(have_person1_flag==0x01)
						 	{
						 		nop();		//有感应时 电量调节输出						 		
						 	}
						 	else
						 	{
						 		PID_CC_discharge.SetPoint = data_cc_var*0.05; //无感应时 5%输出	 //改之前0.015
						 	}
						 }
						 else if((light_mode==0x02) && (work_mode==0x00))//智能模式
						 {
						 	if((Adjust_Pout_Flag==0x01) && (voltage_adjust_index<0x04)) //2018-11-7-caiweikai  voltage>10.7V  前5小时
						 	{
						 		if((ho_cnt==0) && (mi_cnt<30))
							 	{
									if(voltage_adjust_index==0x05)
									{
									  	PID_CC_discharge.SetPoint = data_cc_var*0.05;		 //0.01
									}
									else
									{
									   PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.5; //常亮模式
									}
							 		 
							 	}
							 	else
							 	{
							 		if(have_person1_flag==0x01)
								 	{
								 		if(voltage_adjust_index==0x05)
										{
										  	PID_CC_discharge.SetPoint = data_cc_var*0.05;		 //0.01
										}
										else
										{
										   PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.5; //常亮模式
										} 
								 	}
								 	else
								 	{
								 		if(voltage_adjust_index==0x05)
										{
										  	PID_CC_discharge.SetPoint = data_cc_var*0.05;
										}
										else
										{
										   PID_CC_discharge.SetPoint = PID_CC_discharge.SetPoint*0.5*0.7; //常亮模式 0.01
										} 
								 	}
							 	}
						 	}
						 	else if((Adjust_Pout_Flag>=0x02) || (voltage_adjust_index>=0x04))
						 	{
						 		if(have_person1_flag==0x01)
							 	{
							 		nop();		//有感应时 电量调节输出
							 	}
							 	else
							 	{
							 		PID_CC_discharge.SetPoint = data_cc_var*0.05; //无感应时 5%输出 0.015
							 	}
						 	}
						 }
						//---------------------------------------------------------开关灯控制
							if((led_out_ctrl==0x01) && (work_mode==0x00))
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
						    
						    bost_pwm_value = (unsigned int)PWM;
						    
						    if(PID_CC_discharge.SetPoint==0)
						    {
						    	bost_pwm_value = 0;
						    }
						    else if((PID_CC_discharge.SetPoint<=1) && (PID_CC_discharge.SetPoint>0))
						    {
						    	bost_pwm_value = 25;
						    }
						    
						    Set_Bost_Pwm_Duty(bost_pwm_value);
						    
						    limit_power_pid_flag=0x00;		//清限定输出功率标志
							
							
					}
					else if((discharge_current_value>discharge_i_limit)&&(discharge_index==0x02))
					{
						if(bost_pwm_value>0)
						{
							bost_pwm_value = bost_pwm_value-1;												
						}
						
						Set_Bost_Pwm_Duty(bost_pwm_value);
					}
					
					battery_low_time_count=0;
					
				 }
				 else if((battery_voltage_value<dianchi_qianya_baohu)&&(discharge_index==0x02)) //   欠压保护 
				 {
				 		
				 		battery_low_time_count++;
					
						if(battery_low_time_count>1000)
						{													
							discharge_index=0x03;
							
							bost_pwm_value=0;
							
							Set_Bost_Pwm_Duty(bost_pwm_value);
							
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
						
			handle_an10_process_flag=0x00;
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
	
    unsigned char i,j;		
			
    unsigned int temp;
			
    unsigned char ad_ch;
			
    ad_ch=0x00;
			
    ad_ch=ch;
	
    //-------------------------------------------采样通道选择
    clr_ADCEN; //关闭ADC外设
    
    if(ch==AN1_CHANNEL)
    {
      Enable_ADC_AIN3;
    }
    else if(ch==AN2_CHANNEL)
    {
      Enable_ADC_AIN2;
    }
    else if(ch==AN3_CHANNEL)
    {
      Enable_ADC_AIN6;
    }
    else if(ch==AN4_CHANNEL)
    {
      Enable_ADC_AIN5;
    }
    else if(ch==AN10_CHANNEL)
    {
      Enable_ADC_AIN7;
    }
	
    set_ADCEN;	//使能AD外设
    
    
    //---------------------------------------------------------------------------------------		
	if((ad_ch==AN10_CHANNEL)||(ad_ch==AN9_CHANNEL))   //放电电流----充电电流
	{
		for(i=0;i<51;i++)
		{
            set_ADCEN;
                                
			delay_1us();  //等待采样时间
					
			clr_ADCF;

			set_ADCS;  //启动AD转换
	
			while(ADCF==0);
			clr_ADCF;
	
			adc_buffer[i]=(unsigned int)(ADCRH<<4)+(ADCRL&0x0f);      //读取转换结果
            
            clr_ADCEN;
		}
		//------------冒泡排序算法
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
	
		clr_ADCEN;
                                                				
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
    //---------------------------------------------------------------------------------------		
	for(i=0;i<N;i++)
	{
            set_ADCEN;
                                
			delay_1us();  //等待采样时间
			delay_1us();  //等待采样时间
			delay_1us();  //等待采样时间
			delay_1us();  //等待采样时间
			delay_1us();  //等待采样时间
			delay_1us();  //等待采样时间
			delay_1us();  //等待采样时间
			delay_1us();  //等待采样时间
			delay_1us();  //等待采样时间
					
			clr_ADCF;

			set_ADCS;  //启动AD转换
	
			while(ADCF==0);
			clr_ADCF;
	
			adc_buffer[i]=(unsigned int)(ADCRH<<4)+(ADCRL&0x0f);      //读取转换结果
            
            clr_ADCEN;
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
	
	clr_ADCEN;
	
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
	
	
	if(handle_short_flag==0x01)  //短路保护
	{
		over_load_time_count=0x00;
						
		load_voltage_status=0x01;
		
		handle_short_flag=0x00;
		
		discharge_led_status=0x00;
						
		while(1); //等待系统复位
	}
	
	if(work_mode==0x00)//-----------------正常工作模式（自动模式）
	{
		if((pv_voltage_status==0x01)&&(battery_voltage_status==0x01)&&(sys_temper_flag==0x01))
		{
										
			system_status =	SYSTEM_CHARGE_STATUS;		
								
		}
		else if((pv_voltage_status==0x02)&&(battery_voltage_status==0x01)&&(sys_temper_flag==0x01))
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
																			
//									battery_voltage_value=start_ad_convert(AN3_CHANNEL);	   //20181206 修改过 邢
//									if(battery_voltage_value>=((12.70/31.04)*819.2))		   //	  这段
//									{														  //
//										delay_ms(1000);										  //
//										battery_voltage_value=start_ad_convert(AN3_CHANNEL);  //
//										if(battery_voltage_value>=((12.70/31.04)*819.2))	 //
//										{													//
// 	                                 	  CHARGE_OFF;									   //
//										}												 //
//
//									}													 //20181206 修改过 邢					
//                                    CHARGE_OFF;
									CHARGE_OFF;                                        						
                                    Disable_Bost_Pwm();
						
                                    Set_Bost_Pwm_Duty(0);
								
                                    LOAD_OUT_DISABLE;																													
								
                                    bost_pwm_value=0;
								
                                    PWM	=10;

								
                                    pid_init ( &PID_CC_discharge);
															    
                                    //放电恒流PID调节
                                    PID_CC_discharge.Proportion = 0.001;    // 比例常数 Proportional Const
                                                                        
                                    PID_CC_discharge.Integral   = 0.; 	// 积分常数 Integral Const
                                                                                                                                                        
                                    PID_CC_discharge.Derivative = 0; 	// 微分常数 Derivative Const
                                                                                                                                                        
                                    PID_CC_discharge.SetPoint   = ((load_cc_I*150)/1000)*819.2;    	// 设定目标 Desired value  
                                                                            
                                    open_discharge_time=0x00;
								
                                    ms_cnt=0x00;						
                                    se_cnt=0x00;						
                                    mi_cnt=0x00;						
                                    ho_cnt=0x00;
                                    limit_power_pid_flag=0x00;
                                    Adjust_Pout_Flag=0x00;
                                    voltage_adjust_index=0x01;
                                                                            
                                    nop_index=0x01;
                                                                                    
                                    discharge_index=0x00;
                                                                                    
                                    charge_index=0x00;
                                }
																																					
                            break;
		
		case SYSTEM_CHARGE_STATUS:	
                                if(charge_index==0x01)
                                {
								
									discharge_led_status=0x01;																																	
									Disable_Bost_Pwm();											
									Set_Bost_Pwm_Duty(0);											
									LOAD_OUT_DISABLE;																						
									CHARGE_OFF;																	
									open_discharge_time=0x00;											
									ms_cnt=0x00;						
									se_cnt=0x00;						
									mi_cnt=0x00;						
									ho_cnt=0x00;
									limit_power_pid_flag=0x00;
									Adjust_Pout_Flag=0x00;
									voltage_adjust_index=0x01;											
									nop_index=0x00;											
									discharge_index=0x00;											
									charge_index=0x02;																						
								
                                }
                                break;
		
		case SYSTEM_DISCHARGE_STATUS:
																		
								if(discharge_index==0)
								{
											
                                    charge_led_status=0x01;
							
                                    CHARGE_OFF;																							
																		
                                    LOAD_OUT_DISABLE;
							
                                    Disable_Bost_Pwm();
                                                                            
                                    PWM	=10;
                                                                            
                                    bost_pwm_value=10;
                                                                    
                                    Set_Bost_Pwm_Duty(bost_pwm_value); 
						
                                    pid_init ( &PID_CC_discharge);	        //初始化PID控制参数
							
							
                                    PID_CC_discharge.Proportion = 0.001;  	// 比例常数 Proportional Const

                                    PID_CC_discharge.Integral   = 0; 		// 积分常数 Integral Const
                                                                        
                                    PID_CC_discharge.Derivative = 0; 		// 微分常数 Derivative Const
                                                                        
                                    PID_CC_discharge.SetPoint   = ((load_cc_I*150)/1000)*819.2;   	// 设定目标 Desired value 
                                                                        
                                    open_discharge_time=0x00;
                                                                        
                                    ms_cnt=0x00;						
                                    se_cnt=0x00;						
                                    mi_cnt=0x00;						
                                    ho_cnt=0x00;
                                                                        
                                    limit_power_pid_flag=0x00;
                                    Adjust_Pout_Flag=0x00;
                                    voltage_adjust_index=0x01;		
                                                                                                                                                            
                                    nop_index=0x00;
                                                                            
                                    discharge_index=0x01;
                                                                            
                                    charge_index=0x00;
												
								}
				
								break;
		
		default:break;
	
	}
	

}


