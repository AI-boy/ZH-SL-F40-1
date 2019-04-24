

 #include "N76E003.h"
 #include "SFR_Macro.h"
 #include "Function_define.h"
 #include "Common.h"
 #include "stdio.h"
 #include "string.h"


#include "userdefine.h"








//参数设置

float light_pv=6.0;				//光控电压   设定范围:1--12V

float load_cc_I=1.0;				//LED灯工作电流  负载设定值:42W  1.2A


unsigned int  step_01_time_limit=5;		//输出时长 5 h

unsigned int  step_02_time_limit=5;		//输出时长 5 h

unsigned int  step_03_time_limit=5;		//输出时长 5 h

unsigned int  step_04_time_limit=5;		//输出时长 5 h

unsigned int  step_05_time_limit=5;		//输出时长 5 h

unsigned int light_time= 0x00;			//光控时间 (单位：min)


float bt_volt_100 = 12.30; 			//100%电量时

float bt_volt_80  = 11.64; 			//80%电量

float bt_volt_60  = 10.98; 			//60%电量

float bt_volt_40  = 10.32; 			//40%电量

float bt_volt_20  = 9.66; 			//20%电量

float bt_volt_00  = 9.00; 			//0%电量



float dianchi_guoya_baohu=12.30;   	//100%  电池过压保护点

float dianchi_guoya_huifu=12.15;	//95%   电池过压恢复点

float dianchi_qianya_baohu=9.00;	//0%	电池欠压保护点

float dianchi_qianya_huifu=9.50;	//15%	电池欠压恢复点

float dianchi_cv_level=12.15;		// 进入恒压充电方式

float charge_i_limit=5.00;		//充电电流限定值5A

float shuchu_shortout_voltage=7.5;      //输出短路电压

float shuchu_open_voltage=63.00;	//输出开路电压

float discharge_i_baohu=4.50;		//放电电流保护点  过流保护  4.50A

float discharge_i_limit=4.00;		//放电电流限定值 4.00A

float light_on_pv_level=6.00;		//亮灯光伏电压门限值 默认6V

float light_off_pv_level=8.00;		//关灯光伏电压门限值 默认8V


float charge_r_value=15.0;	   	//充电电流采样电阻  毫欧

float discharge_r_value=150.0;		//放电电流采样电阻  毫欧

float Load_Pmax=42.00;			//输出最大功率


//--------------------------------红外遥控器数据接收
unsigned int   ird_status=0x00;

unsigned int   receive_bit_count=0x00;

unsigned int   ird_receive_flag=0x00;

unsigned int   ird_receive_time_count=0x00;

unsigned long   ird_data=0x00;//0x10ED40BF;

unsigned char   light_mode=0x00;

unsigned char   lr_cnt =0x00;

unsigned char   led_out_ctrl=0x00;

//---------------------------------EEPROM系统参数

volatile unsigned char xdata page_buffer[128];


UINT8 Read_APROM_BYTE(UINT16 code *u16_addr)
{
	UINT8 rdata;
	rdata = *u16_addr>>8;
	return rdata;
}

/*****************************************************************************************************************
write_DATAFLASH_BYTE :
user can copy all this subroutine into project, then call this function in main.
******************************************************************************************************************/		
void Write_DATAFLASH_BYTE(unsigned int u16EPAddr,unsigned char u8EPData)
{
	unsigned char looptmp=0;
	unsigned int u16_addrl_r;
	unsigned int RAMtmp;
	
//Check page start address
	u16_addrl_r=(u16EPAddr/128)*128;
//Save APROM data to XRAM0
	for(looptmp=0;looptmp<0x80;looptmp++)
	{
		RAMtmp = Read_APROM_BYTE((unsigned int code *)(u16_addrl_r+looptmp));
		page_buffer[looptmp]=RAMtmp;
	}
// Modify customer data in XRAM
		page_buffer[u16EPAddr&0x7f] = u8EPData;
	
//Erase APROM DATAFLASH page
		IAPAL = u16_addrl_r&0xff;
		IAPAH = (u16_addrl_r>>8)&0xff;
		IAPFD = 0xFF;
	  set_IAPEN; 
		set_APUEN;
    IAPCN = 0x22; 		
 		set_IAPGO; 
		
//Save changed RAM data to APROM DATAFLASH
		set_IAPEN; 
		set_APUEN;
	  IAPCN = 0x21;
		for(looptmp=0;looptmp<0x80;looptmp++)
		{
			IAPAL = (u16_addrl_r&0xff)+looptmp;
      IAPAH = (u16_addrl_r>>8)&0xff;
			IAPFD = page_buffer[looptmp];
			set_IAPGO;			
		}
		clr_APUEN;
		clr_IAPEN;
}	

/**********************************************************************
*函数名称：read_param_eeprom
*描述：获取系统参数
*返回类型：void
*输入形参：void
***********************************************************************/
void read_param_eeprom(void)
{
    unsigned char byte_l;

/*	APROM 0x3800~0x38FF demo as dataflash 				      				     	*/
/* 	Please use Memory window key in C:0x3800 to check earse result					*/	  
    
    byte_l = Read_APROM_BYTE(0x4000); 

	if(byte_l!=0x55) //程序初始下载标志
	{
		Write_DATAFLASH_BYTE(0x4000,0x55);

		Write_DATAFLASH_BYTE(0x4001,0x00);	 //亮灯模式 0：常亮  1：感应  2:智能模式   默认常亮模式

		Write_DATAFLASH_BYTE(0x4002,0x00);  //开灯状态 0：开灯  1：关灯  默认开灯状态
	}
      
    byte_l=Read_APROM_BYTE(0x4001);      	//亮灯模式
								
    light_mode=byte_l;		        		//默认值：0x00:常亮模式  0x01:感应模式   0x02:智能模式   
	
    byte_l=Read_APROM_BYTE(0x4002);      	//开灯状态     
								
    led_out_ctrl=byte_l;					//0：开灯  1：关灯
	
    light_time_level = 1200;             	//光控检测时间	1200*5ms=6000ms=6s
    
    
	
	//9--12.3V  磷酸铁锂电池	 5V---4096   1V---819.2
	
	bt_volt_100 = (12.35/31.04)*819.2; 			//100%电量时   819.2=4096/5V

	bt_volt_80  = (11.80/31.04)*819.2; 			//80%电量
		
	bt_volt_60  = (11.30/31.04)*819.2; 			//60%电量
		
	bt_volt_40  = (10.80/31.04)*819.2; 			//40%电量
		
	bt_volt_20  = (10.30/31.04)*819.2; 			//20%电量
		
	bt_volt_00  = (9.00/31.04)*819.2; 			//0%电量
		
		
	dianchi_guoya_baohu = (12.35/31.04)*819.2;   	//100%  电池过压保护点
	
	dianchi_guoya_huifu = (12.15/31.04)*819.2;		//95%   电池过压恢复点
		
	dianchi_qianya_baohu = (9.00/31.04)*819.2;		//0%	电池欠压保护点
		
	dianchi_qianya_huifu = (9.50/31.04)*819.2;		//15%	电池欠压恢复点		 
		
	dianchi_cv_level = (12.25/31.04)*819.2;			//12.15时 进入恒压充电方式
		
	charge_i_limit = ((12.00*15.0)/1000)*819.2;		//充电电流限定值
		
	shuchu_shortout_voltage = (7.50/61)*819.2; 	    //输出短路电压
		
	shuchu_open_voltage = (61.00/61)*819.2;		    //输出开路电压
		
	discharge_i_baohu = ((4.50*150)/1000)*819.2;			//放电电流保护点  过流保护  4.5A
		
	discharge_i_limit = ((4.00*150)/1000)*819.2;			//放电电流限定值 4.0A
		
	light_on_pv_level =  (5.0/61)*819.2;		        //亮灯光伏电压门限值 5V
		
	light_off_pv_level = (8.0/61)*819.2;		        //关灯光伏电压门限值 10V
		
	Load_Pmax=42.00;
			

}




/**********************************************************************
*函数名称：Init_Ird
*描述：初始化红外接收端口 
*返回类型：void
*输入形参：void
***********************************************************************/
void Init_Ird(void)
{
	P12_Input_Mode;				//P1.2
	Enable_INT_Port1;			//管脚中断通道2 端口01--P1.2
	Enable_BIT2_FallEdge_Trig;  //P1.2   IRD_IN  红外遥控器
	clr_PIF2;
}


//0x4ECA807F   其中4ECA为用户码 也可区分遥控器设备码 MAC 白色遥控器
//0x00FF807F   其中00FF为用户码 也可区分遥控器设备码 MAC 黑色遥控器
//0x10ED807F   其中10ED为用户码 也可区分遥控器设备码 MAC 此版本

void Key_Menu(unsigned long key_value)
{
    unsigned char usr_code;
    
    unsigned int  remote_mac;
    
    usr_code=(unsigned char)(key_value>>8);
									    
    remote_mac=key_value>>16;			//0x10ED  设备码
    
    if(remote_mac!=ZH_MAC)   			
    {
        //return;
    }
  	
	
    switch(usr_code)
    {
    	case HAN_MODE:
   				nop();
    			break;  
    	case STA_MODE://---------------------------------------------------标准模式(夜间常亮)	
    					
    			light_mode =0x00;
    			Write_DATAFLASH_BYTE(0x4001,light_mode); 
    			led_out_ctrl=0x00;
				Write_DATAFLASH_BYTE(0x4002,led_out_ctrl);  													
				nop_index=0x00;
				discharge_index=0x00;											
				charge_index=0x00;	
							
    			CHARGE_OFF;								
			
				Disable_Bost_Pwm();
				LOAD_OUT_DISABLE;
				Set_Bost_Pwm_Duty(0);																		
				LOAD_OUT_ENABLE;
																																				
				Enable_Bost_Pwm();												
																			
				Set_Bost_Pwm_Duty(25);																		
				delay_ms(1000);
								
				Set_Bost_Pwm_Duty(0);
	
				Disable_Bost_Pwm();
				LOAD_OUT_DISABLE;
				Set_Bost_Pwm_Duty(0);									
							
				pv_voltage_status=0x00;  //空闲状态
								
				day_time_count=0x00;
							
                night_time_count=0x00;
							
				pv_time_count=0x00;
    					
    			break;  
    	case IRD_MODE://---------------------------------------------------人体感应模式(夜间有人亮灯)
    					
    					
    			light_mode =0x01;
    			Write_DATAFLASH_BYTE(0x4001,light_mode); 
    			led_out_ctrl=0x00;
				Write_DATAFLASH_BYTE(0x4002,led_out_ctrl);
    						
							
				nop_index=0x00;
				discharge_index=0x00;											
				charge_index=0x00;
							
    			CHARGE_OFF;								
			
				Disable_Bost_Pwm();
				LOAD_OUT_DISABLE;
				Set_Bost_Pwm_Duty(0);
							
													
				LOAD_OUT_ENABLE;																																			
				Enable_Bost_Pwm();																														
				Set_Bost_Pwm_Duty(25);																		
				delay_ms(1000);
								
				Set_Bost_Pwm_Duty(0);
	
				Disable_Bost_Pwm();
				LOAD_OUT_DISABLE;													
				Set_Bost_Pwm_Duty(0);				
				pv_voltage_status=0x00;  //空闲状态
								
				day_time_count=0x00;
							
                night_time_count=0x00;
							
				pv_time_count=0x00;
    					
    			break;  
    	case AUT_MODE://--------------------------------------------------智能模式
    					   					
    			light_mode =0x02;
    			Write_DATAFLASH_BYTE(0x4001,light_mode);  
    			led_out_ctrl=0x00;
				Write_DATAFLASH_BYTE(0x4002,led_out_ctrl);			
							
				nop_index=0x00;
				discharge_index=0x00;											
				charge_index=0x00;
							
    			CHARGE_OFF;								
			
				Disable_Bost_Pwm();
				LOAD_OUT_DISABLE;
				Set_Bost_Pwm_Duty(0);
																			
				LOAD_OUT_ENABLE;
																																				
				Enable_Bost_Pwm();												
																			
				Set_Bost_Pwm_Duty(25);						
													
				delay_ms(1000);
								
				Set_Bost_Pwm_Duty(0);
	
				Disable_Bost_Pwm();
				LOAD_OUT_DISABLE;													
				Set_Bost_Pwm_Duty(0);				
				pv_voltage_status=0x00;  //空闲状态
								
				day_time_count=0x00;
								
				night_time_count=0x00;
								
				pv_time_count=0x00;
    					
                break; 
    	case ONF_MODE://-----------------------------------------------手动开关灯模式
    					
    					
    			led_out_ctrl=led_out_ctrl+1;
    						
    			led_out_ctrl=led_out_ctrl%2;

				//Write_DATAFLASH_BYTE(0x4001,light_mode);
    						
    			Write_DATAFLASH_BYTE(0x4002,led_out_ctrl);
    					
    			if(led_out_ctrl==0x00 )
    			{														
                	nop_index=0x00;
                    discharge_index=0x00;											
                    charge_index=0x00;	
							
	                CHARGE_OFF;								
	                        
	                Disable_Bost_Pwm();
                    LOAD_OUT_DISABLE;
                    Set_Bost_Pwm_Duty(0);
																
                    LOAD_OUT_ENABLE;
																																	
                    Enable_Bost_Pwm();												
																
                    Set_Bost_Pwm_Duty(25);						
										
                    delay_ms(1000);
					
                    Set_Bost_Pwm_Duty(0);

                    Disable_Bost_Pwm();
                    LOAD_OUT_DISABLE;
					Set_Bost_Pwm_Duty(0);								
					
                    pv_voltage_status=0x00;  //空闲状态
					
                    day_time_count=0x00;
					
                    night_time_count=0x00;
					
                    pv_time_count=0x00;
    			}
                else if(led_out_ctrl==0x01 )
    			{   													
                    nop_index=0x00;
                    discharge_index=0x00;											
                    charge_index=0x00;
					
                   CHARGE_OFF;								
                    					
                    Disable_Bost_Pwm();
                    //LOAD_OUT_DISABLE;
                    Set_Bost_Pwm_Duty(0);
                    delay_ms(200);
                    LOAD_OUT_ENABLE;																																	
                    Enable_Bost_Pwm();																	
								
                    Set_Bost_Pwm_Duty(35);
				
                    delay_ms(500);
								
                    Set_Bost_Pwm_Duty(0);
						
                    delay_ms(500);
								
                    Set_Bost_Pwm_Duty(35);
								
                    delay_ms(500);
								
                    Set_Bost_Pwm_Duty(0);
								
                    delay_ms(500);
								
                    Set_Bost_Pwm_Duty(35);
								
                    delay_ms(500);
								
                    Set_Bost_Pwm_Duty(0);	
                    Disable_Bost_Pwm();
                    LOAD_OUT_DISABLE;
					Set_Bost_Pwm_Duty(0);
					delay_ms(500);
					
                    pv_voltage_status=0x00;  //空闲状态
					
                    day_time_count=0x00;
					
                    night_time_count=0x00;
					
                    pv_time_count=0x00;
    			}
    			break;  
    	default:break;
    }
        
} 