 #ifndef _IRD_REMOTE_H
 #define _IRD_REMOTE_H

//-----------------------------------------------------------------
//采样电阻
extern float charge_r_value;	   //充电采样电流采样电阻值 **毫欧姆

extern float discharge_r_value;		//放电电流采样电阻







//参数设置

extern float light_pv;			//光控电压   设定范围:5--10V(500--1000)

extern float load_cc_I;			//LED灯工作电流  负载设定值:0.33--1.2A(33--1200)


extern unsigned int  step_01_time_limit;		//输出时长 5 h

extern unsigned int  step_02_time_limit;		//输出时长 5 h

extern unsigned int  step_03_time_limit;		//输出时长 1 h

extern unsigned int  step_04_time_limit;		//输出时长 1 h

extern unsigned int  step_05_time_limit;		//输出时长 1 h

extern unsigned int light_time;					//光控时间 (单位：min)

extern float bt_volt_100 ; 			//100%电量时

extern float bt_volt_80  ; 			//80%电量

extern float bt_volt_60  ; 			//60%电量

extern float bt_volt_40  ; 			//40%电量

extern float bt_volt_20  ; 			//20%电量

extern float bt_volt_00  ; 			//0%电量


extern float dianchi_guoya_baohu;   	//100%  电池过压保护点

extern float dianchi_guoya_huifu;		//95%   电池过压恢复点

extern float dianchi_qianya_baohu;		//0%	电池欠压保护点

extern float dianchi_qianya_huifu;		//20%	电池欠压恢复点

extern float dianchi_cv_level;			// 进入恒压充电方式

extern float charge_i_limit;			//充电电流限定值

extern float shuchu_shortout_voltage; 	//输出短路电压

extern float shuchu_open_voltage;		//输出开路电压

extern float discharge_i_baohu;			//放电电流保护点  过流保护  2.4A

extern float discharge_i_limit;			//放电电流限定值 2.0A

extern float light_on_pv_level;			//亮灯光伏电压门限值 默认6V

extern float light_off_pv_level;		//关灯光伏电压门限值 默认8V


extern float Load_Pmax;	



//----------------------------------------------
extern unsigned int  ird_status;
				  
extern  unsigned int   receive_bit_count;

extern unsigned int   ird_receive_flag;

extern unsigned int   ird_receive_time_count;

extern  unsigned long   ird_data;

extern unsigned char light_mode;


extern unsigned char lr_cnt;

extern unsigned char led_out_ctrl;


#define  ZH_MAC                         0x10ED

#define  HAN_MODE                       0x00
        
#define  STA_MODE                       0x40        
        
#define  IRD_MODE                      	0x10
        
#define  AUT_MODE                       0x50

#define  ONF_MODE						0x88



//------------------------------------------------
extern volatile unsigned char xdata page_buffer[128];

extern UINT8 Read_APROM_BYTE(UINT16 code *u16_addr);

extern void Write_DATAFLASH_BYTE(unsigned int u16EPAddr,unsigned char u8EPData);

extern void read_param_eeprom(void);

extern void Init_Ird(void);

extern void Key_Menu(unsigned long key_value);



#endif
