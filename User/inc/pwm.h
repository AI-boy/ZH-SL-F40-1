

#ifndef _ZH_PWM_H
#define _ZH_PWM_H 







//-----BOOST PWM<90%*1024=920
#define 	BOST_PWM_MAX		920

#define 	BOST_PWM_MIN		2	




extern unsigned int bost_pwm_value;




extern void init_pwm(void);

extern void Enable_Bost_Pwm(void);

extern void Disable_Bost_Pwm(void);

extern void Set_Bost_Pwm_Duty(unsigned int dduty);





#endif