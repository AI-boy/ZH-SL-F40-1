
#ifndef _ZH_LED_H
#define _ZH_LED_H


//**--------------包含文件-----------------------------------------------------

//**--------------宏定义-------------------------------------------------------



////充电指示灯
//#define CHARGE_LED_TRIS			(TRISCbits.TRISC4) 				// RC4
//#define CHARGE_LED_OFF			(LATCbits.LATC4 = 0x00) 		// 输出低电平
//#define CHARGE_LED_ON			(LATCbits.LATC4 = 0x01) 		// 输出高电平
//#define CHARGE_LED_TOG			(LATCbits.LATC4 ^= 0x01) 		// 输出反电平
//
//
//
////放电指示灯
//#define DISCHARGE_LED_TRIS		(TRISCbits.TRISC5) 				// RC5
//#define DISCHARGE_LED_OFF		(LATCbits.LATC5 = 0x00) 		// 输出低电平
//#define DISCHARGE_LED_ON		(LATCbits.LATC5 = 0x01) 		// 输出高电平
//#define DISCHARGE_LED_TOG		(LATCbits.LATC5 ^= 0x01) 		// 输出反电平
//
//
////电池指示灯
//#define BATRGE_LED_TRIS			(TRISBbits.TRISB4) 				// RB4
//#define BATRGE_LED_OFF			(LATBbits.LATB4 = 0x00) 		// 输出低电平
//#define BATRGE_LED_ON			(LATBbits.LATB4 = 0x01) 		// 输出高电平
//#define BATRGE_LED_TOG			(LATBbits.LATB4 ^= 0x01) 		// 输出反电平






//**--------------数据结构-----------------------------------------------------

//**--------------函数申明-----------------------------------------------------


extern void init_led(void);
	


#endif 
