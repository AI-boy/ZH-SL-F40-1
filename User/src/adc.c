 
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"





unsigned int temperature_value=0x00;   		//�����¶�

unsigned int pv_voltage_value=0x00;		//������ѹ

unsigned int battery_voltage_value=0x00;	//��ص�ѹ

unsigned int load_voltage_value=0x00;		//�����ѹ

unsigned int charge_current_value=0x00;		//������

unsigned int discharge_current_value=0x00;	//�ŵ����


unsigned int adc_buffer[64];




/**********************************************************************
*�������ƣ�init_adc
*��������ʼ��AD�����˿�
*�������ͣ�void
*�����βΣ�void
***********************************************************************/

void init_adc(void)
{
   
    
    P06_Input_Mode;  		   //AIN3---AN1   �¶Ȳ���
        
    P07_Input_Mode;            //AIN2---AN2   ������ѹ����
    						           
    P03_Input_Mode;            //AIN6---AN3(AN12)  ��ص�ѹ����
        
    P04_Input_Mode;            //AIN5---AN4   �����ѹ����
    
    P11_Input_Mode;            //AIN7---AN10  �ŵ���� ����
    
   	Disable_ADC;
    
    clr_ADCF;
        
}
