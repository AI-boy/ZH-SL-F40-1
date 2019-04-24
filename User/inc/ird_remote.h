 #ifndef _IRD_REMOTE_H
 #define _IRD_REMOTE_H

//-----------------------------------------------------------------
//��������
extern float charge_r_value;	   //������������������ֵ **��ŷķ

extern float discharge_r_value;		//�ŵ������������







//��������

extern float light_pv;			//��ص�ѹ   �趨��Χ:5--10V(500--1000)

extern float load_cc_I;			//LED�ƹ�������  �����趨ֵ:0.33--1.2A(33--1200)


extern unsigned int  step_01_time_limit;		//���ʱ�� 5 h

extern unsigned int  step_02_time_limit;		//���ʱ�� 5 h

extern unsigned int  step_03_time_limit;		//���ʱ�� 1 h

extern unsigned int  step_04_time_limit;		//���ʱ�� 1 h

extern unsigned int  step_05_time_limit;		//���ʱ�� 1 h

extern unsigned int light_time;					//���ʱ�� (��λ��min)

extern float bt_volt_100 ; 			//100%����ʱ

extern float bt_volt_80  ; 			//80%����

extern float bt_volt_60  ; 			//60%����

extern float bt_volt_40  ; 			//40%����

extern float bt_volt_20  ; 			//20%����

extern float bt_volt_00  ; 			//0%����


extern float dianchi_guoya_baohu;   	//100%  ��ع�ѹ������

extern float dianchi_guoya_huifu;		//95%   ��ع�ѹ�ָ���

extern float dianchi_qianya_baohu;		//0%	���Ƿѹ������

extern float dianchi_qianya_huifu;		//20%	���Ƿѹ�ָ���

extern float dianchi_cv_level;			// �����ѹ��緽ʽ

extern float charge_i_limit;			//�������޶�ֵ

extern float shuchu_shortout_voltage; 	//�����·��ѹ

extern float shuchu_open_voltage;		//�����·��ѹ

extern float discharge_i_baohu;			//�ŵ����������  ��������  2.4A

extern float discharge_i_limit;			//�ŵ�����޶�ֵ 2.0A

extern float light_on_pv_level;			//���ƹ����ѹ����ֵ Ĭ��6V

extern float light_off_pv_level;		//�صƹ����ѹ����ֵ Ĭ��8V


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
