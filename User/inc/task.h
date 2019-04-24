 
#ifndef _ZH_TASK_H
#define _ZH_TASK_H







#define    SYSTEM_NOP_STATUS					0x00

#define    SYSTEM_CHARGE_STATUS					0x01

#define    SYSTEM_DISCHARGE_STATUS				0x02  






extern unsigned char system_status;

extern unsigned char system_check_time;

extern unsigned int system_time_count;

extern unsigned char work_mode;

extern unsigned char pv_voltage_status;    			//光伏板电压状态

extern unsigned char battery_voltage_status;		//电池电压状态

extern unsigned char load_voltage_status;			//负载端状态

extern unsigned char discharge_index;  				//放电阶段变量

extern unsigned char charge_index;					//充电阶段变量

extern unsigned char nop_index;

extern unsigned int involid_time_count;

extern unsigned int cv_level_time_count;

extern unsigned int load_short_time_count;

extern unsigned int load_open_time_count;

extern unsigned int over_load_time_count;

extern unsigned int battery_low_time_count;




extern unsigned long int night_time_count;
					
extern unsigned long int day_time_count;

extern unsigned long int pv_time_count;

extern unsigned long int light_time_level;



extern float PWM;

extern float PWM_I;

extern float bt_prev;

extern unsigned char mppt_status;

extern unsigned int mppt_time_count;

extern unsigned char temper_time_count;

extern unsigned char over_temper_time_count;
					
extern unsigned char sys_temper_flag;



extern unsigned char charge_led_status;

extern unsigned char bt_led_status;

extern unsigned char discharge_led_status;

extern unsigned char bt_time_count;

extern unsigned char pv_index;

extern unsigned char Adjust_Pout_Flag;

extern unsigned char voltage_adjust_index;

extern unsigned char limit_power_pid_flag;



extern unsigned char mcu_t00_time_count;
extern unsigned char mcu_t01_time_count;
extern unsigned char mcu_t02_time_count;
extern unsigned char mcu_t03_time_count;
extern unsigned char mcu_temp_index;

extern unsigned char pid_index;
extern unsigned int index_01_count;
extern unsigned int index_02_count;
extern unsigned int index_03_count;
extern unsigned int index_04_count;
extern unsigned int index_05_count;
extern unsigned int index_06_count;

extern float  x_v;

extern float  y_v;


extern void handle_ad_loop(void);

extern void handle_task_process(void);

extern	unsigned int start_ad_convert(unsigned char ch);






#endif//-----------------------------