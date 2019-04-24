 
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"





unsigned int temperature_value=0x00;   		//环境温度

unsigned int pv_voltage_value=0x00;		//光伏板电压

unsigned int battery_voltage_value=0x00;	//电池电压

unsigned int load_voltage_value=0x00;		//输出电压

unsigned int charge_current_value=0x00;		//充电电流

unsigned int discharge_current_value=0x00;	//放电电流


unsigned int adc_buffer[64];




/**********************************************************************
*函数名称：init_adc
*描述：初始化AD采样端口
*返回类型：void
*输入形参：void
***********************************************************************/

void init_adc(void)
{
   
    
    P06_Input_Mode;  		   //AIN3---AN1   温度采样
        
    P07_Input_Mode;            //AIN2---AN2   光伏板电压采样
    						           
    P03_Input_Mode;            //AIN6---AN3(AN12)  电池电压采样
        
    P04_Input_Mode;            //AIN5---AN4   输出电压采样
    
    P11_Input_Mode;            //AIN7---AN10  放电电流 采样
    
   	Disable_ADC;
    
    clr_ADCF;
        
}
