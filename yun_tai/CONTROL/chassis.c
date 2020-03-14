#include "chassis.h"
#include "can_task.h"
#include "ramp.h"
#include "motor_control.h"

ChassisSpeed_Target_t ChassisSpeed_Target;
RampGen_t chassisRamp = CHASSIS_RAMP_GEN_DAFAULT;

void CMControlLoop(char rotate,int16_t FB,int16_t LR,int16_t RO){
	
    ChassisSpeed_Target.forward_back_target =  FB;
    ChassisSpeed_Target.left_right_target = LR;
    ChassisSpeed_Target.rotate_target =RO;
    
	switch (rotate)
    {
        case CHASSISMOVE:
        case CM360rotateStop:
         {
             SendChassisSpeed(CAN2, ChassisSpeed_Target.forward_back_target * 70,
			 ChassisSpeed_Target.left_right_target * 70, ChassisSpeed_Target.rotate_target* 70, 0);						
         }break;

        case CM360rotate:
		{
             ChassisSpeed_Target.rotate_target360 =12000*chassisRamp.Calc(&chassisRamp);    //12000
             SendChassisSpeed(CAN2, ChassisSpeed_Target.forward_back_target * 70,
             ChassisSpeed_Target.left_right_target * 70, ChassisSpeed_Target.rotate_target360, 0);						
		}break;
    }

}
