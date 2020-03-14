#include "remote_app.h"
#include "can_task.h"
#include "can_data.h"
#include "gimbal_control.h"
#include "ramp.h"
#include "pid.h"

///////////////////////////////���岿��/////////////////////

RampGen_t GMPitchRamp = PITCH_RAMP_GEN_DAFAULT;
RampGen_t GMYawRamp = PITCH_RAMP_GEN_DAFAULT;


/////////////////////////////���ƺ�������//////////////////
/**
@brief ��̨��ѭ������
*/
void gimbalControl(float pitch_angle_target,float yaw_angle_target){	
//����pitch���ٶȻ���λ�û�pid	
    float pid_position_out;
    float pid_speed_out;
    Motor_Data_t motor;
    motor=GetMotorData(PIT_MOTOR);
    
    pid_position_out=PID_Calc(5,motor.angle * GMPitchRamp.Calc(&GMPitchRamp),pitch_angle_target);
    
    pid_speed_out=PID_Calc(4,0,pid_position_out);
	SetMotorCurrent (PIT_MOTOR,pid_speed_out);
    SendMotorCurrent(PIT_MOTOR);
//����yaw���ٶȻ���λ�û�pid	  
    motor=GetMotorData(YAW_MOTOR);
    
    pid_position_out=PID_Calc(7,motor.angle * GMYawRamp.Calc(&GMYawRamp),yaw_angle_target);
    
    pid_speed_out=PID_Calc(6,0,pid_position_out);
	SetMotorCurrent (YAW_MOTOR,pid_speed_out);
    SendMotorCurrent(YAW_MOTOR);    
}

/**
@brief gimbalֹͣ������δ���������
*/
//void gimbal_stop(void){            //δ���������
//    Gimbal_Target.pitch_angle_target = getPitchAngle();
//}


