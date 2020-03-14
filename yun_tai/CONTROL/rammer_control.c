#include "stm32f4xx.h"
#include "rammer_control.h" 
#include "can_data.h"
#include "can_task.h"
#include "pid.h"

Motor_Data_t rammer_data;
//const int16_t target[8]={0,1023,2046,3069,4092,5115,6138,7161};


#define torque_limit 1500

int16_t time_invert=0;

void RammerControlLoop(char shoot_state) {
    int16_t rammer_speed_target;
    rammer_data=GetMotorData(RAMMER_MOTOR);
//    char shoot_circuit=shoot_state;
    Motor_Data_t fric_data1=GetMotorData(FRIC1_MOTOR);
    Motor_Data_t fric_data2=GetMotorData(FRIC2_MOTOR);
    
//    if((fric_data1.speed+fric_data2.speed)/2<6000)      //摩擦轮速度还没有起来
//    {
//        shoot_state=0;
//    }
    
    switch (shoot_state)
    {
        case NONE_SHOOT:
        {
            rammer_speed_target=0;       
        }break;
        case SINGLE_SHOOT:
        case SERIES_SHOOT:
        {
            rammer_speed_target=1500;
        }break;      
    }
//    if((rammer_data.torque>torque_limit)||time_invert>0)//堵转
//    {
//        if(time_invert==0)
//        {
//            time_invert=50;
//        }
//        rammer_speed_target=-1500;  
//        time_invert--;
//    }
    SetMotorCurrent(RAMMER_MOTOR, PID_Calc(3,rammer_data.speed,rammer_speed_target));
    SendMotorCurrent(RAMMER_MOTOR);
    
}
