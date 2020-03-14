#include "remote_app.h"
#include "can_task.h"
#include "can_data.h"
#include "gimbal_control.h"
#include "ramp.h"
#include "pid.h"

///////////////////////////////定义部分/////////////////////

RampGen_t GMPitchRamp = PITCH_RAMP_GEN_DAFAULT;
RampGen_t GMYawRamp = PITCH_RAMP_GEN_DAFAULT;


/////////////////////////////控制函数部分//////////////////
/**
@brief 云台主循环定义
*/
void gimbalControl(float pitch_angle_target,float yaw_angle_target){	
//发送pitch轴速度环和位置环pid	
    float pid_position_out;
    float pid_speed_out;
    Motor_Data_t motor;
    motor=GetMotorData(PIT_MOTOR);
    
    pid_position_out=PID_Calc(5,motor.angle * GMPitchRamp.Calc(&GMPitchRamp),pitch_angle_target);
    
    pid_speed_out=PID_Calc(4,0,pid_position_out);
	SetMotorCurrent (PIT_MOTOR,pid_speed_out);
    SendMotorCurrent(PIT_MOTOR);
//发送yaw轴速度环和位置环pid	  
    motor=GetMotorData(YAW_MOTOR);
    
    pid_position_out=PID_Calc(7,motor.angle * GMYawRamp.Calc(&GMYawRamp),yaw_angle_target);
    
    pid_speed_out=PID_Calc(6,0,pid_position_out);
	SetMotorCurrent (YAW_MOTOR,pid_speed_out);
    SendMotorCurrent(YAW_MOTOR);    
}

/**
@brief gimbal停止工作，未适配编码器
*/
//void gimbal_stop(void){            //未适配编码器
//    Gimbal_Target.pitch_angle_target = getPitchAngle();
//}


