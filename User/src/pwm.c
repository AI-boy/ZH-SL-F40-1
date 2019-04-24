 
 
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"




unsigned int bost_pwm_value=0x00;






/**********************************************************************
*�������ƣ�init_pwm
*������ CH3   PA3  BOST_PWM
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void init_pwm(void)
{
    P15_Input_Mode;  			//P1.5  ��ʼ������Ϊ����״̬ ��ֹPWM���

	PWM_CLOCK_FSYS; 			//PWMʱ��ԴΪϵͳʱ��FSYS 16MHz

	PWM5_P15_OUTPUT_ENABLE;		// PWM5 �������ѡ��P15

	PWM_IMDEPENDENT_MODE;		//PWM �������ģʽ

	PWM5_OUTPUT_NORMAL;		   	//PWM ��������������

	PWM_EDGE_TYPE;				//PWM ����ģʽ ���ض���

	PWMPH = 0x03;				//PWM �������� PWMʱ��Ƶ��Ĭ��ϵͳƵ��Fsys=16MHz
	PWMPL = 0xFF;	 			//Fpwm=16000000/(0x3ff+1)=15.625 khz  10λ�ֱ���

	set_CLRPWM;					//���16λPWM������

	set_SFRPAGE;			    //PWM4 and PWM5 duty seting is in SFP page 1	
	PWM5H = 0x00;				//SFR ���⹦�ܼĴ�����ҳ0 ҳ1		
	PWM5L = 0x00;
	clr_SFRPAGE;	

	set_LOAD;   				//����PWM���ڼ�ռ�ձ��趨ֵ

	clr_PWMF;		
}

/**********************************************************************
*�������ƣ�enable_bost_pwm
*����������BOST PWM
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Enable_Bost_Pwm(void)
{
    P15_PushPull_Mode;

	clr_P15;
  
    set_PWMRUN; //����PWM 
}



/**********************************************************************
*�������ƣ�disable_bost_pwm
*����:�ر�BOST PWM
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Disable_Bost_Pwm(void)
{
    P15_Input_Mode;
	
    clr_PWMRUN;

	P15_Input_Mode;

	//P15=0;
}


/**********************************************************************
*�������ƣ�set_bost_pwm_duty
*����������BOST PWMռ�ձ�
*�������ͣ�void
*�����βΣ�void
***********************************************************************/
void Set_Bost_Pwm_Duty(unsigned int dduty)     
{	
    bost_pwm_value=dduty;

	
    if(dduty<=BOST_PWM_MAX)  
    {
        set_SFRPAGE;			    				//PWM4 and PWM5 duty seting is in SFP page 1	
		PWM5H = (unsigned char)((dduty>>8)&0x03);	//PWM ռ�ձȸ�2λ						
		PWM5L = (unsigned char)dduty;				//PWM ռ�ձȵ�8λ
		clr_SFRPAGE; 		
    }
    else
    {
        dduty=BOST_PWM_MAX;
            
        set_SFRPAGE;			    	
		PWM5H = (unsigned char)((dduty>>8)&0x03);					
		PWM5L = (unsigned char)dduty;
		clr_SFRPAGE; 		
    }

	set_LOAD;		//����PWMռ�ձ���ֵ

}


		