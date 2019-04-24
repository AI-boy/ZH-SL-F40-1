

//**--------------头文件-------------------------------------------------------
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"


/**********************************************************************
*函数名称：init_timer1
*描述：红外接收 定时器
*返回类型：void
*输入形参：void
***********************************************************************/
void Init_Time1_IRD(void)
{  
  TIMER1_MODE1_ENABLE;  //TIMER1 模式1 16位定时器
  clr_T1M; 				//系统时钟1/12 分频 16M/12
  clr_TF1;				//清标志

  clr_TR1;	 			//停止计数之后再读/写定时器1-----！！！
  TL1=0x00;
  TH1=0x00;   
}

/**********************************************************************
*函数名称：init_timer3
*描述：TIME4初始化 系统时基 定时1ms
*返回类型：void
*输入形参：void
***********************************************************************/
void init_timer3(void) 
{
    /**********TIME3系统时基定时器*************/
    
   //预分频 64  自动重装载值 250 :250*64/16=1000us=1ms 
   clr_TR3;									   //!!!!!
   RH3 = 0xff;//RELOAD_VALUE_H;                       //initial counter values 
   RL3 = 0x06;//RELOAD_VALUE_L;  
     
   T3CON &= 0xE0;
   T3CON |= 0x06;
   clr_TF3;			 //清除中断标志
   set_ET3;  		//使能TIMER3 定时器中断

    
}
