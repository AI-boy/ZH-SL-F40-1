 
 
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"




unsigned int bost_pwm_value=0x00;






/**********************************************************************
*函数名称：init_pwm
*描述： CH3   PA3  BOST_PWM
*返回类型：void
*输入形参：void
***********************************************************************/
void init_pwm(void)
{
    P15_Input_Mode;  			//P1.5  初始化设置为输入状态 禁止PWM输出

	PWM_CLOCK_FSYS; 			//PWM时钟源为系统时钟FSYS 16MHz

	PWM5_P15_OUTPUT_ENABLE;		// PWM5 输出引脚选择P15

	PWM_IMDEPENDENT_MODE;		//PWM 独立输出模式

	PWM5_OUTPUT_NORMAL;		   	//PWM 输出极性正常输出

	PWM_EDGE_TYPE;				//PWM 对齐模式 边沿对齐

	PWMPH = 0x03;				//PWM 周期设置 PWM时钟频率默认系统频率Fsys=16MHz
	PWMPL = 0xFF;	 			//Fpwm=16000000/(0x3ff+1)=15.625 khz  10位分辨率

	set_CLRPWM;					//清除16位PWM计数器

	set_SFRPAGE;			    //PWM4 and PWM5 duty seting is in SFP page 1	
	PWM5H = 0x00;				//SFR 特殊功能寄存器在页0 页1		
	PWM5L = 0x00;
	clr_SFRPAGE;	

	set_LOAD;   				//更新PWM周期及占空比设定值

	clr_PWMF;		
}

/**********************************************************************
*函数名称：enable_bost_pwm
*描述：开启BOST PWM
*返回类型：void
*输入形参：void
***********************************************************************/
void Enable_Bost_Pwm(void)
{
    P15_PushPull_Mode;

	clr_P15;
  
    set_PWMRUN; //开启PWM 
}



/**********************************************************************
*函数名称：disable_bost_pwm
*描述:关闭BOST PWM
*返回类型：void
*输入形参：void
***********************************************************************/
void Disable_Bost_Pwm(void)
{
    P15_Input_Mode;
	
    clr_PWMRUN;

	P15_Input_Mode;

	//P15=0;
}


/**********************************************************************
*函数名称：set_bost_pwm_duty
*描述：设置BOST PWM占空比
*返回类型：void
*输入形参：void
***********************************************************************/
void Set_Bost_Pwm_Duty(unsigned int dduty)     
{	
    bost_pwm_value=dduty;

	
    if(dduty<=BOST_PWM_MAX)  
    {
        set_SFRPAGE;			    				//PWM4 and PWM5 duty seting is in SFP page 1	
		PWM5H = (unsigned char)((dduty>>8)&0x03);	//PWM 占空比高2位						
		PWM5L = (unsigned char)dduty;				//PWM 占空比低8位
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

	set_LOAD;		//更新PWM占空比数值

}


		