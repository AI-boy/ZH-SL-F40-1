

//**--------------ͷ�ļ�-------------------------------------------------------
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_define.h"
#include "Common.h"
#include "stdio.h"
#include "string.h"


#include "userdefine.h"




void delay_1us(void) // ��ʱ1us����Ϊϵͳʱ����16MHZ��һ��ָ��ʱ����1/16M * 16 = 1 us
{
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
	nop(); // 0.125us
}



void delay_ms(unsigned short count)
{
	unsigned short i, j;
	
	for (i = 0; i < count; i++)
	{
    	for (j = 0; j < 2000; j++)
    	{
	    	nop(); // 0.125us
			nop(); // 0.125us
			nop(); // 0.125us
			nop(); // 0.125us
			nop(); // 0.125us
			nop(); // 0.125us
			nop(); // 0.125us
			nop(); // 0.125us           
    	}
		set_WDCLR;// ι�� 
   	}
}
