/******************************************************************************
**                           ־����ҵ���޹�˾
**
**                           
**
**--------------�����ļ���Ϣ---------------------------------------------------
** ��   ��   ��	: main.c
** ��ǰ    �汾	: V1.0 
** ��        �� : ̫���ܷ���ƿ�����
** ��   ��   �� : xc8
** Ӳ        �� : ̫���ܷ���ƿ�����
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ��ʼ��������	: 2018-09-20
** ��   ��   ��	: ��ά��
** ��  	     �� : V1.0
** �衡      �� : 

**-----------------------------------------------------------------------------
*******************************************************************************/
/*
���ļ�¼����    20181206
����ԭ��ȡ��������һ�µ�����	 ��262���Ժ��λ��
/* Includes ------------------------------------------------------------------*/
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"

#include "stdio.h"
#include "string.h"


#include "userdefine.h"

//**--------------ȫ�ֱ���-----------------------------------------------------
unsigned long sys_clk_freq=0 ;   //ϵͳʱ��Ƶ��
unsigned char handle_scan_key_flag=0;
unsigned int  have_person1_time_count=0;
unsigned char have_person1_flag=0;
unsigned char scan_key_time_count=0;

unsigned char run_index=0x00;
//**--------------�궨��-------------------------------------------------------

//**--------------��������-----------------------------------------------------

//**--------------��������-----------------------------------------------------





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
*�������ƣ�init_osc
*������
*1.ϵͳʱ�����ã�16MHz
*2.���Ź����� ��1.02��
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void init_osc(void)
{
  //---------------------------HIRC enable part                
  set_HIRCEN;												//step1: enable HIRC clock source run
  while((CKSWT&SET_BIT5)==0);								//step2: check ready
  clr_OSC1;													//step3: switching system clock source if needed
  clr_OSC0;
  while((CKEN&SET_BIT0)==1);								//step4: check system clock switching OK or NG
  
  //---------------------------ʹ�ܿ��Ź� 1.638s
  Check_WDT_Reset_Config();
	
  TA=0xAA;TA=0x55;WDCON|=0x07;						    //Setting WDT prescale 
  set_WDCLR;											//Clear WDT timer
  while((WDCON|~SET_BIT6)==0xFF);				        //confirm WDT clear is ok before into power down mode
 
		
  IE	=	0x00;			 		
  EIE	=	0x00;
  EIE1	=	0x00;
  TCON	=	0x00;
  
  delay_ms(100);//�ȴ�ϵͳ��Դ�ȶ�
}






/**********************************************************************
*�������ƣ�init_port
*�������˿ڳ�ʼ��
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void init_port(void)
{
        Set_All_GPIO_Quasi_Mode;

		P05_PushPull_Mode;clr_P05;  //P0.5  CH_EN  	 ���MOS�ܿ��ƶ˿�
		CHARGE_ON;
		P06_Input_Mode;	    		//AIN3--AN1  �¶Ȳ���
		P07_Input_Mode;				//AIN2--AN2	 ������ѹ����

		P15_PushPull_Mode;	   		//P15 PWM    LED��ѹ����  ��������͵�ƽ
		clr_P15; 
		
		P04_Input_Mode;				//AIN5--AN4	  �����ѹ
		P03_Input_Mode;         	//AIN6--AN12  ��ص�ѹ

		P01_Input_Mode;				//P0.1  ���ر���  �½����ж�  DLBH
		
		Enable_INT_Port0;			//�ܽ��ж�ͨ��1 �˿�00--P0.1
		Enable_BIT1_FallEdge_Trig;  //�½��ش����ж�
		

		P11_Input_Mode;				//P1.1--AIN7--AN10  LED���ص�������

		P12_Input_Mode;				//P1.2
		 
		Enable_INT_Port1;			//�ܽ��ж�ͨ��2 �˿�01--P1.2
		Enable_BIT2_FallEdge_Trig;  //P1.2   IRD_IN  ����ң����
		       

        P13_Input_Mode;				//P1.3  ������

		P14_PushPull_Mode;			//P1.4  LED_OUT  �ŵ�MOS�ܿ���
        clr_P14;
        
}




/**********************************************************************
*�������ƣ�driver_init
*�������������ʼ��
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void driver_init(void)
{
    init_osc();	    	//ϵͳʱ������
		
    init_port();        //IO�˿ڼ�����˿ڳ�ʼ��
	
    init_timer3();  	//��ʼ����ʱ��TIMER3  ϵͳʱ����ʱ��  1ms
	
    Init_Time1_IRD();   //��ʼ����ʱ��TIME1  ����ң����
        
    Init_Ird();			//��ʼ��������ܶ˿�
	
    init_adc();     	//��ʼ��adc
	
    init_pwm();     	//��ʼ��pwm
			
}




/**********************************************************************
*�������ƣ�middle_init
*�������м���ʼ��
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void middle_init(void)
{		
    read_param_eeprom();  //��ȡϵͳ����   
	
}


/**********************************************************************
*�������ƣ�application_init
*������Ӧ�ò��ʼ��
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void application_init(void)
{
	
	system_check_time=0x01;
	
	//open_an0_time=0x01;
	
	open_an1_time=0x01;                     //�����¶ȼ��
	
	open_an2_time=0x01;                     //̫���ܵ�ذ��ѹ���
	
	open_an3_time=0x01;                     //��ص�ѹ���
	
	open_an4_time=0x01;                     //�����ѹ���
	
	//open_an9_time=0x01;                   //���������
	
	open_an10_time=0x01;                    //�ŵ�������
				
    
    //�ŵ����PID����
    pid_init ( &PID_CC_discharge);
        
    PID_CC_discharge.Proportion = 0.001;  		 // �������� Proportional Const
    
	PID_CC_discharge.Integral   = 0; 		     // ���ֳ��� Integral Const
										    
	PID_CC_discharge.Derivative = 0; 		     // ΢�ֳ��� Derivative Const
										    
	PID_CC_discharge.SetPoint   = ((load_cc_I*150)/1000)*819.2;               // �趨Ŀ�� Desired value 330mA  
    
    open_discharge_time=0x00;
    
    ms_cnt=0x00;						
	se_cnt=0x00;						
	mi_cnt=0x00;						
	ho_cnt=0x00;
	//--------------------------------------------
	run_index=0x00;
	//----------------------����������ѭ��ǰ�Ĳ���		
    set_WDTR;			   //�������Ź�
    set_TR3;              //����ʱ����ʱ��Timer3 Tsys=1ms    
    set_EPI;	          //�ⲿ�ܽ��ж�  
	clr_PIF1;			  
	PIF=0x00;			  
	clr_T1M;
    EA=1;;               //ʹ��CPUȫ���ж�

//	if(run_index==0x00)
//	{
//		Disable_Bost_Pwm();		   	//��ֹ��ѹ����
//		LOAD_OUT_DISABLE;			  // LED���عض�
//
//		battery_voltage_value=start_ad_convert(AN3_CHANNEL);		//��ص�ѹ����  ��ʼ���֮ǰ�ж��Ƿ��е�ؽ��룬���û�л��ߵ�ѹ����һ��ֱֵ���������� 2018.12.06
//
//		if(battery_voltage_value>((10.60/31.04)*819.2))
//		{
//			CHARGE_ON;				   //���պ�
//
//			Set_Bost_Pwm_Duty(0);																		
//			LOAD_OUT_ENABLE;		  // LED���ؿ���
//																																			
//			Enable_Bost_Pwm();																																				
//			Set_Bost_Pwm_Duty(3);	
//			delay_ms(800);	
//			battery_voltage_value=start_ad_convert(AN3_CHANNEL);		//��ص�ѹ����
//			if(battery_voltage_value>((10.60/31.04)*819.2))	
//			{
//				Set_Bost_Pwm_Duty(0);																		
//				LOAD_OUT_ENABLE;		  // LED���ؿ���
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
////			battery_voltage_value=start_ad_convert(AN3_CHANNEL);			//��ص�ѹ����																
////			Disable_Bost_Pwm();		   	//��ֹ��ѹ����
////			LOAD_OUT_DISABLE;			  // LED���عض�
////			if(battery_voltage_value>((10.60/31.04)*819.2))
////			{
////			    Set_Bost_Pwm_Duty(0);																		
////				LOAD_OUT_ENABLE;		  // LED���ؿ���
////																																				
////				Enable_Bost_Pwm();																																				
////				Set_Bost_Pwm_Duty(25);	
////				delay_ms(3500);														
////				Set_Bost_Pwm_Duty(0);
////			}
////			else
////			{
////				Set_Bost_Pwm_Duty(0);																		
////				LOAD_OUT_ENABLE;		  // LED���ؿ���
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
*�������ƣ�main
*������
*�������ͣ�int
*�����βΣ�void
---------------------------------------------------------------------*/
void main(void) 
{
    driver_init(); 		        // �������ʼ��
    
    middle_init(); 		        // �м���ʼ��
    
    application_init();         // Ӧ�ò��ʼ��


                  
    while(1)
    {
        set_WDCLR;           	// ι��  
                  
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

			clr_TR1;   //ֹͣ����
                
            TL1=0x00;
  			TH1=0x00;//��ʱ������    
                                        
            clr_TF1;            
                
            ird_receive_flag=0x00;									   
         }
                             
                  
          handle_ad_loop();
                  
          handle_task_process();
                  
    }
	
}
