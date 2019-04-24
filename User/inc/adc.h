 
#ifndef _ZH_ADC_H
#define _ZH_ADC_H






#define       N          11

#define      AN1_CHANNEL   0x01
#define      AN2_CHANNEL   0x02
#define      AN3_CHANNEL   0x03
#define      AN4_CHANNEL   0x04
#define      AN9_CHANNEL   0x09
#define      AN10_CHANNEL  0x0a




extern unsigned int temperature_value;   		//�����¶�

extern unsigned int pv_voltage_value;			//������ѹ

extern unsigned int battery_voltage_value;		//��ص�ѹ

extern unsigned int load_voltage_value;			//�����ѹ

extern unsigned int charge_current_value;		//������

extern unsigned int discharge_current_value;	//�ŵ����

extern unsigned int adc_buffer[64];

extern void init_adc(void);




#endif