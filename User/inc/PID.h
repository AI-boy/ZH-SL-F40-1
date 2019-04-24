#ifndef __PID_H
#define __PID_H


//-------------------------------------
//
//
//PID结构体
//
//
//
//---------------------------------------

typedef struct PID_VAR{

     float SetPoint;   // 设定目标 Desired value
     float Proportion; // 比例常数 Proportional Const
     float Integral;   // 积分常数 Integral Const
     float Derivative; // 微分常数 Derivative Const
     float LastError;  // Error[-1]
     float PrevError;  // Error[-2]
     float SumError;   // Sums of Errors
} PID;




//extern	PID PID_CC_charge;       //控制调节恒流充电
//
//extern  PID PID_CV_charge;     //控制调节恒压充电
 
extern  PID PID_CC_discharge;  //控制调节恒流输出

extern  void pid_init (PID *pp);

extern  float  pid_calc( PID *pp, int NowPoint );

#endif
