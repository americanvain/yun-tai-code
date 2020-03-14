#include "stm32f4xx.h"
#include "pid.h" 
#include "main.h"

const float Kp = 0.9,Ki = 0.10,Kd = 0;
Inc_PID_t gunPID_1 = {Kp,Ki,Kd,0,0,0,0};
Inc_PID_t gunPID_2 = {Kp,Ki,Kd,0,0,0,0};

PID_Regulator_t rammer_speed_PID={0,0,{0,0,0},{0,0,0},RAMMER_SPEED_KP,RAMMER_SPEED_KI,RAMMER_SPEED_KD,0,0,0,1000,0,5000};

PID_Regulator_t GMPSpeedPID={0,0,{0,0,0},{0,0,0},PITCH_SPEED_KP,PITCH_SPEED_KI,PITCH_SPEED_KD,0,0,0,1000,0,25000};

PID_Regulator_t GMPPositionPID={0,0,{0,0,0},{0,0,0},PITCH_POSITION_KP,PITCH_POSITION_KI,PITCH_POSITION_KD,0,0,0,1000,0,25000};

PID_Regulator_t GMYSpeedPID={0,0,{0,0,0},{0,0,0},YAW_SPEED_KP,YAW_SPEED_KI,YAW_SPEED_KD,0,0,0,1000,0,25000};

PID_Regulator_t GMYPositionPID={0,0,{0,0,0},{0,0,0},YAW_POSITION_KP,YAW_POSITION_KI,YAW_POSITION_KD,0,0,0,1000,0,25000};

int16_t IncPID(uint8_t PIDNum, int16_t current_velocity,int16_t target_velocity)
{
    Inc_PID_t* PID;
    if(PIDNum==1)
    {
        PID =&gunPID_1;
    }
    else if(PIDNum==2)
    {
        PID=&gunPID_2;
    }
    PID->error_last_last = PID->error_last;
    PID->error_last = PID->error_now;
    PID->error_now = target_velocity - current_velocity;
    
    PID->pid_out =  PID->Kp * (PID->error_now - PID->error_last)
		              + PID->Ki * PID->error_now
		              + PID->Kd *( PID->error_now - 2 * PID->error_last + PID->error_last_last);
    return PID->pid_out;
}


float abs_limit(float a, float ABS_MAX)
{
    if (a > ABS_MAX)
        a = ABS_MAX;
    if (a < -ABS_MAX)
        a = -ABS_MAX;
    return a;
}

float PID_Calc(uint8_t PIDNum,int16_t Speednow,int16_t TargetSpeed) {
    PID_Regulator_t* pid;
    if(PIDNum==3)
    {
        pid =&rammer_speed_PID;
    }
    else if (PIDNum==4)     //pitch 的速度位置PID
    {
        pid =&GMPSpeedPID;
    }
    else if (PIDNum==5)     //pitch 的位置PID
    {
        pid=&GMPPositionPID;
    }
    else if (PIDNum==6)     //yaw 的速度PID
    {
        pid =&GMYSpeedPID;
    }
    else if (PIDNum==7)     //yaw 的位置PID
    {
        pid=&GMYPositionPID;
    }
    pid->ref=TargetSpeed;
    pid->fdb=Speednow;
    pid->err[2] = pid->err[1];
    pid->err[1] = pid->err[0];
    pid->err[0] = pid->ref - pid->fdb;

    pid->Pout = pid->kp * pid->err[0];
    pid->Iout += pid->ki * pid->err[0];
    pid->Dbuf[2] = pid->Dbuf[1];
    pid->Dbuf[1] = pid->Dbuf[0];
    pid->Dbuf[0] = (pid->err[0] - pid->err[1]);
    pid->Dout = pid->kd * pid->Dbuf[0];
    pid->Iout=abs_limit(pid->Iout, pid->IoutMax);
    pid->out = pid->Pout + pid->Iout + pid->Dout;
    pid->out=abs_limit(pid->out, pid->outMax);
    return pid->out;
}
