
#ifndef _ZH_INTERRUPT_H
#define _ZH_INTERRUPT_H

//**--------------包含文件-----------------------------------------------------

//**--------------宏定义-------------------------------------------------------

//**--------------数据结构-----------------------------------------------------

//**--------------函数申明-----------------------------------------------------

	
//**--------------全局变量-----------------------------------------------------

extern unsigned char    open_discharge_time;			//放电时长定时器
extern unsigned int     ms_cnt;					//毫秒
extern unsigned int     se_cnt;					//秒
extern unsigned int     mi_cnt;					//分
extern unsigned int	ho_cnt;				        //时

extern unsigned char 	open_an0_time;  			//AN0 通道采样周期定时器
extern unsigned int   	an0_time_count;				
extern unsigned char  	handle_an0_process_flag;


extern unsigned char 	open_an1_time; 				//AN1 通道采样周期定时器
extern unsigned int   	an1_time_count;				
extern unsigned char  	handle_an1_process_flag;


extern unsigned char 	open_an2_time; 				//AN2 通道采样周期定时器
extern unsigned int   	an2_time_count;				
extern unsigned char  	handle_an2_process_flag;


extern unsigned char 	open_an3_time; 				//AN3 通道采样周期定时器
extern unsigned int   	an3_time_count;				
extern unsigned char  	handle_an3_process_flag;


extern unsigned char 	open_an4_time; 				//AN4 通道采样周期定时器
extern unsigned int   	an4_time_count;				
extern unsigned char  	handle_an4_process_flag;


extern unsigned char 	open_an9_time; 				//AN9 通道采样周期定时器
extern unsigned int   	an9_time_count;				
extern unsigned char  	handle_an9_process_flag;


extern unsigned char 	open_an10_time; 				//AN10 通道采样周期定时器
extern unsigned int   	an10_time_count;				
extern unsigned char  	handle_an10_process_flag;


extern unsigned char handle_short_flag;

extern unsigned  int   time_cnt;



















#endif 
