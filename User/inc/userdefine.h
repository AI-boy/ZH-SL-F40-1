 #ifndef _USERDEFINE_H
 #define _USERDEFINE_H
 
 
 
 


//------------------------------------------------------------------------------------------------��������ͷ�ļ�
 	#include "udelay.h"
	#include "interrupt.h"
	#include "led.h"
	#include "timer.h"	
	#include "adc.h"
	#include "pwm.h"
	#include "task.h"
	#include "PID.h"
	#include "ird_remote.h"
	
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------

//#define BIT0  0
//#define BIT1  1
//#define BIT2  2
//#define BIT3  3
//#define BIT4  4
//#define BIT5  5
//#define BIT6  6
//#define BIT7  7




#define SYE_Freq     16000000

#define PERSON_CHECK1			(P13)

#define LOAD_OUT_ENABLE			(set_P14) 		// LED�������
#define LOAD_OUT_DISABLE		(clr_P14) 		// LED���عض�

#define CHARGE_ON				(set_P05)                //���պ�
#define CHARGE_OFF		        (clr_P05)             //���ر�


extern unsigned long sys_clk_freq;
extern unsigned char handle_scan_key_flag;
extern unsigned int  have_person1_time_count;
extern unsigned char have_person1_flag;
extern unsigned char  scan_key_time_count;
extern unsigned char  run_index;

#endif