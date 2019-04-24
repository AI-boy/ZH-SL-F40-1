/******************************************************************************
**                           志和兴业有限公司
**
**                           
**
**--------------基本文件信息---------------------------------------------------
** 文   件   名	: main.c
** 当前    版本	: V1.0 
** 描        述 : 太阳能泛光灯控制器
** 编   译   器 : xc8
** 硬        件 : 太阳能泛光灯控制器
**--------------历史版本信息---------------------------------------------------
** 开始创建日期	: 2018-09-20
** 创   建   人	: 蔡维凯
** 版  	     本 : V1.0
** 描　      述 : 

**-----------------------------------------------------------------------------
*******************************************************************************/
/*
更改记录：邢    20181206
更改原因：取消开灯闪一下的问题	 在262行以后的位置
/* Includes ------------------------------------------------------------------*/
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"

#include "stdio.h"
#include "string.h"


#include "userdefine.h"

//**--------------全局变量-----------------------------------------------------
unsigned long sys_clk_freq=0 ;   //系统时钟频率
unsigned char handle_scan_key_flag=0;
unsigned int  have_person1_time_count=0;
unsigned char have_person1_flag=0;
unsigned char scan_key_time_count=0;

unsigned char run_index=0x00;
//**--------------宏定义-------------------------------------------------------

//**--------------函数申明-----------------------------------------------------

//**--------------函数定义-----------------------------------------------------





/***********************************************************************
	WDT CONFIG enable 
	warning : this macro is only when ICP not enable CONFIG WDT function
	copy this marco code to you code to enable WDT reset.
************************************************************************/
void Check_WDT_Reset_Config(void)
{
		set_IAPEN;
		IAPAL = 0x04;
		IAPAH = 0x00;	
		IAPFD = 0xFF;
		IAPCN = 0xC0;						//Read config command
		set_IAPGO;  
		if ((IAPFD&0xF0)==0xF0)
		{
			IAPFD = 0x0F;
			IAPCN = 0xE1;
    		set_IAPGO;                         						//trigger IAP
			while((CHPCON&SET_BIT6)==SET_BIT6);          //check IAPFF (CHPCON.6)
			clr_CFUEN;
			clr_IAPEN;
			EA = 0;
			TA = 0xAA;
			TA = 0x55;
			CHPCON &= 0xFD;
			TA = 0xAA;
			TA = 0x55;
			CHPCON |= 0x80;
		}
		clr_IAPEN;
}

/**********************************************************************
*函数名称：init_osc
*描述：
*1.系统时钟配置：16MHz
*2.看门狗设置 ：1.02秒
*返回类型：void
*输入形参：void
***********************************************************************/
void init_osc(void)
{
  //---------------------------HIRC enable part                
  set_HIRCEN;												//step1: enable HIRC clock source run
  while((CKSWT&SET_BIT5)==0);								//step2: check ready
  clr_OSC1;													//step3: switching system clock source if needed
  clr_OSC0;
  while((CKEN&SET_BIT0)==1);								//step4: check system clock switching OK or NG
  
  //---------------------------使能看门狗 1.638s
  Check_WDT_Reset_Config();
	
  TA=0xAA;TA=0x55;WDCON|=0x07;						    //Setting WDT prescale 
  set_WDCLR;											//Clear WDT timer
  while((WDCON|~SET_BIT6)==0xFF);				        //confirm WDT clear is ok before into power down mode
 
		
  IE	=	0x00;			 		
  EIE	=	0x00;
  EIE1	=	0x00;
  TCON	=	0x00;
  
  delay_ms(100);//等待系统电源稳定
}






/**********************************************************************
*函数名称：init_port
*描述：端口初始化
*返回类型：void
*输入形参：void
***********************************************************************/
void init_port(void)
{
        Set_All_GPIO_Quasi_Mode;

		P05_PushPull_Mode;clr_P05;  //P0.5  CH_EN  	 充电MOS管控制端口
		CHARGE_ON;
		P06_Input_Mode;	    		//AIN3--AN1  温度采样
		P07_Input_Mode;				//AIN2--AN2	 光伏板电压采样

		P15_PushPull_Mode;	   		//P15 PWM    LED升压驱动  推挽输出低电平
		clr_P15; 
		
		P04_Input_Mode;				//AIN5--AN4	  输出电压
		P03_Input_Mode;         	//AIN6--AN12  电池电压

		P01_Input_Mode;				//P0.1  过载保护  下降沿中断  DLBH
		
		Enable_INT_Port0;			//管脚中断通道1 端口00--P0.1
		Enable_BIT1_FallEdge_Trig;  //下降沿触发中断
		

		P11_Input_Mode;				//P1.1--AIN7--AN10  LED负载电流采样

		P12_Input_Mode;				//P1.2
		 
		Enable_INT_Port1;			//管脚中断通道2 端口01--P1.2
		Enable_BIT2_FallEdge_Trig;  //P1.2   IRD_IN  红外遥控器
		       

        P13_Input_Mode;				//P1.3  人体检测

		P14_PushPull_Mode;			//P1.4  LED_OUT  放电MOS管控制
        clr_P14;
        
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
		
    init_port();        //IO端口及外设端口初始化
	
    init_timer3();  	//初始化定时器TIMER3  系统时基定时器  1ms
	
    Init_Time1_IRD();   //初始化定时器TIME1  红外遥控器
        
    Init_Ird();			//初始化红外接受端口
	
    init_adc();     	//初始化adc
	
    init_pwm();     	//初始化pwm
			
}




/**********************************************************************
*函数名称：middle_init
*描述：中间层初始化
*返回类型：void
*输入形参：void
***********************************************************************/
void middle_init(void)
{		
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
	
	system_check_time=0x01;
	
	//open_an0_time=0x01;
	
	open_an1_time=0x01;                     //环境温度检测
	
	open_an2_time=0x01;                     //太阳能电池板电压检测
	
	open_an3_time=0x01;                     //电池电压检测
	
	open_an4_time=0x01;                     //输出电压检测
	
	//open_an9_time=0x01;                   //充电电流检测
	
	open_an10_time=0x01;                    //放电电流检测
				
    
    //放电恒流PID调节
    pid_init ( &PID_CC_discharge);
        
    PID_CC_discharge.Proportion = 0.001;  		 // 比例常数 Proportional Const
    
	PID_CC_discharge.Integral   = 0; 		     // 积分常数 Integral Const
										    
	PID_CC_discharge.Derivative = 0; 		     // 微分常数 Derivative Const
										    
	PID_CC_discharge.SetPoint   = ((load_cc_I*150)/1000)*819.2;               // 设定目标 Desired value 330mA  
    
    open_discharge_time=0x00;
    
    ms_cnt=0x00;						
	se_cnt=0x00;						
	mi_cnt=0x00;						
	ho_cnt=0x00;
	//--------------------------------------------
	run_index=0x00;
	//----------------------即将进入主循环前的操作		
    set_WDTR;			   //开启开门狗
    set_TR3;              //启用时基定时器Timer3 Tsys=1ms    
    set_EPI;	          //外部管脚中断  
	clr_PIF1;			  
	PIF=0x00;			  
	clr_T1M;
    EA=1;;               //使能CPU全局中断

//	if(run_index==0x00)
//	{
//		Disable_Bost_Pwm();		   	//禁止升压启动
//		LOAD_OUT_DISABLE;			  // LED负载关断
//
//		battery_voltage_value=start_ad_convert(AN3_CHANNEL);		//电池电压采样  开始充电之前判断是否有电池接入，如果没有或者电压低于一定值直接跳过闪灯 2018.12.06
//
//		if(battery_voltage_value>((10.60/31.04)*819.2))
//		{
//			CHARGE_ON;				   //充电闭合
//
//			Set_Bost_Pwm_Duty(0);																		
//			LOAD_OUT_ENABLE;		  // LED负载开启
//																																			
//			Enable_Bost_Pwm();																																				
//			Set_Bost_Pwm_Duty(3);	
//			delay_ms(800);	
//			battery_voltage_value=start_ad_convert(AN3_CHANNEL);		//电池电压采样
//			if(battery_voltage_value>((10.60/31.04)*819.2))	
//			{
//				Set_Bost_Pwm_Duty(0);																		
//				LOAD_OUT_ENABLE;		  // LED负载开启
//																																				
//				Enable_Bost_Pwm();																																				
//				Set_Bost_Pwm_Duty(15);	
//				delay_ms(2000);														
//				Set_Bost_Pwm_Duty(0);
//			}
//			else
//			 {
//				delay_ms(2000);
//				Set_Bost_Pwm_Duty(0);
//			 }
//	
////			battery_voltage_value=start_ad_convert(AN3_CHANNEL);			//电池电压采样																
////			Disable_Bost_Pwm();		   	//禁止升压启动
////			LOAD_OUT_DISABLE;			  // LED负载关断
////			if(battery_voltage_value>((10.60/31.04)*819.2))
////			{
////			    Set_Bost_Pwm_Duty(0);																		
////				LOAD_OUT_ENABLE;		  // LED负载开启
////																																				
////				Enable_Bost_Pwm();																																				
////				Set_Bost_Pwm_Duty(25);	
////				delay_ms(3500);														
////				Set_Bost_Pwm_Duty(0);
////			}
////			else
////			{
////				Set_Bost_Pwm_Duty(0);																		
////				LOAD_OUT_ENABLE;		  // LED负载开启
////																																				
////				Enable_Bost_Pwm();																																				
////				Set_Bost_Pwm_Duty(5);	
////				delay_ms(2000);														
////				Set_Bost_Pwm_Duty(0);
////			}
//		}
//				
//		Disable_Bost_Pwm();
//		LOAD_OUT_DISABLE;
//		Set_Bost_Pwm_Duty(0); 
//		set_WDCLR;
//		run_index=0x01;
//	}		

 
}

/*---------------------------------------------------------------------
*函数名称：main
*描述：
*返回类型：int
*输入形参：void
---------------------------------------------------------------------*/
void main(void) 
{
    driver_init(); 		        // 驱动层初始化
    
    middle_init(); 		        // 中间层初始化
    
    application_init();         // 应用层初始化


                  
    while(1)
    {
        set_WDCLR;           	// 喂狗  
                  
        if(handle_scan_key_flag==0x01)
        {			
            if(PERSON_CHECK1)
            {
              have_person1_time_count=0x00;
                              
              have_person1_flag=0x01;
            }
                              
            handle_scan_key_flag=0x00;
        }
                  
        if(ird_receive_flag==0x01)
        {          
            Key_Menu(ird_data);
            
            receive_bit_count=0x00;
                
            ird_status=0x00;
                
            ird_data=0x00;

			clr_TR1;   //停止计数
                
            TL1=0x00;
  			TH1=0x00;//定时器清零    
                                        
            clr_TF1;            
                
            ird_receive_flag=0x00;									   
         }
                             
                  
          handle_ad_loop();
                  
          handle_task_process();
                  
    }
	
}
