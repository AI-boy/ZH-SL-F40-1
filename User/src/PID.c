
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"





// PID PID_CC_charge;       //控制调节恒流充电
//
// PID PID_CV_charge;     //控制调节恒压充电
 
 PID PID_CC_discharge;  //控制调节恒流输出
 




float pid_calc( PID *pp, int NowPoint )
{
	float PWM_var;
	
	float dError, Error; 
	
	float dk;
	
	dk=5.0; //---caiwk-2018-5-17---!!!
	
	Error = pp->SetPoint - NowPoint;            // 先计算偏差
	
	if(limit_power_pid_flag==0x01)
	{
		if((Error<=2)&&(Error>=-2))		
			{
				dk=200.0;
			}
			else if(Error>=4)
			{
				dk=200.0; 
			}
			else if(Error<=-4)
			{
				dk=200.0;
			}
	}
	else
	{
		if(pid_index==0x01)
		{  
						
			if((Error<=1)&&(Error>=-1))		
			{
				Error=0;
			}
			else if(Error>=2)
			{
				dk=200.0; 
			}
			else if(Error<=-2)
			{
				dk=200.0;
			}
			else
			{
				dk=200.0;
			}
		
		}
		else if(pid_index==0x02)
		{
			if((Error<=1)&&(Error>=-1))		
			{
				Error=0;
			}
			else if((Error>=2)||(Error<=-2))
			{
				dk=100.0; 
			}
		}
			
	}
		
	
	pp->SumError += Error;                      // 在对偏差进行积分


     
    if((pp->SumError)>=2)     pp->SumError=  2;
    else if((pp->SumError)<=-2)  pp->SumError= -2;

	dError = pp->LastError - pp->PrevError;     // 对偏差微分
	
	pp->PrevError = pp->LastError;
	
	pp->LastError = Error;
					// -------比例项-----------------积分项----------------------------微分项---
	PWM_var=(pp->Proportion * Error*dk  + pp->Integral * pp->SumError  + pp->Derivative * dError  ) ;              


		if((Error>=3 && Error<=5)||(Error<=-3 && Error>=-5))
		{
			if(light_mode==0x00)
			{
				if((ho_cnt==0) && (mi_cnt==0) && (se_cnt<30))
				{
					if(PWM_var>=2)PWM_var           =  2;
					else  if(PWM_var <= -2)PWM_var  = -2;
				}
				else
				{
					if(PWM_var>=0.1)PWM_var           =  0.1;
					else  if(PWM_var <= -0.1)PWM_var  = -0.1;
				}
			}
			else
			{
				if(PWM_var>=2)PWM_var           =  2;
				else  if(PWM_var <= -2)PWM_var  = -2; 
			}
			
		}
		else if((Error>5)||(Error<-5))
		{
			if(light_mode==0x00)
			{
				if((ho_cnt==0) && (mi_cnt==0) && (se_cnt<30))
				{
					if(PWM_var>=5)PWM_var           =  5;
					else  if(PWM_var <= -5)PWM_var  = -5;
				}
				else
				{
					if(PWM_var>=0.1)PWM_var           =  0.1;
					else  if(PWM_var <= -0.1)PWM_var  = -0.1;
				}
			}
			else
			{
				if(PWM_var>=5)PWM_var           =  5;
				else  if(PWM_var <= -5)PWM_var  = -5; 
			} 
		}
		else
		{
			if(have_person1_flag)
			{
				if(light_mode==0x00)
				{
					if(PWM_var>=0.1)PWM_var           =  0.1;
					else  if(PWM_var <= -0.1)PWM_var  = -0.1;
				}
				else
				{
					if(PWM_var>=0.5)PWM_var           =  0.5;
					else  if(PWM_var <= -0.5)PWM_var  = -0.5;
				}
				
			}
			else
			{
				if(light_mode==0x00)
				{
					if(PWM_var>=0.1)PWM_var           =  0.1;
					else  if(PWM_var <= -0.1)PWM_var  = -0.1;
				}
				else
				{
					if(PWM_var>=0.5)PWM_var           =  0.5;
					else  if(PWM_var <= -0.5)PWM_var  = -0.5;
				}
			}
			 
		}		

    
    return (PWM_var );
}




/*====================================================================================================
　　　　 Initialize PID Structure
=====================================================================================================*/
void pid_init ( PID *pp)
{
	memset (pp,0,sizeof(PID));
}


