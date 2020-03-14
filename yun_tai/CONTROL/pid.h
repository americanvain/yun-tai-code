#ifndef _PID_H_
#define _PID_H_

#include "sys.h"

typedef struct{
    float Kp;				//比例
    float Ki;				//积分
    float Kd;				//微分
    float error_now;		//本次误差
    float error_last;		//上次误差
    float error_last_last;	//上上次误差
    int16_t pid_out;		//PID输出
}Inc_PID_t;				//增量式PID

typedef struct PID_Regulator_t {
    float ref;
    float fdb;
    float err[3];
    float Dbuf[3];//差分缓存，暂未使用
    float kp;
    float ki;
    float kd;
    float Pout;
    float Iout;
    float Dout;
    float IoutMax;
    float out;
    float outMax;
}PID_Regulator_t;


#define YAW_POSITION_KP     45   //30
#define YAW_POSITION_KI     0
#define YAW_POSITION_KD     20

#define YAW_SPEED_KP        20
#define YAW_SPEED_KI        0
#define YAW_SPEED_KD        40

#define PITCH_POSITION_KP   85
#define PITCH_POSITION_KI   0
#define PITCH_POSITION_KD   18

//#define PITCH_POSITION_KP   15
//#define PITCH_POSITION_KI   0
//#define PITCH_POSITION_KD   5

#define PITCH_SPEED_KP      30
#define PITCH_SPEED_KI      0
#define PITCH_SPEED_KD      38

//#define PITCH_SPEED_KP      15
//#define PITCH_SPEED_KI      0
//#define PITCH_SPEED_KD      5


#define RAMMER_SPEED_KP     15
#define RAMMER_SPEED_KI     0
#define RAMMER_SPEED_KD     5


float PID_Calc(uint8_t PIDNum,int16_t Speednow,int16_t TargetSpeed);
int16_t IncPID(uint8_t PIDNum, int16_t current_velocity,int16_t target_velocity);

#endif


