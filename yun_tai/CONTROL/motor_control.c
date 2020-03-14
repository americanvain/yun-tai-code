#include "stm32f4xx.h"
#include "motor_control.h" 
#include "main.h"

#include "friction_control.h"
#include "rammer_control.h"
#include "remote_app.h"
#include "gimbal_control.h"
#include "chassis.h"
#include "led.h"

const RC_ctrl_t *rc_ctrl_now;
char shoot_state=0;
char rotate_state=0;
float pitch_target=0;  //2029
float yaw_target=0;

int16_t FBtarget=0;
int16_t LRtarget=0;
int16_t ROtarget=0;

#define STICK_TO_CHASSIS_SPEED_REF_FACT     0.45f

#define STICK_TO_PITCH_ANGLE_INC_FACT       0.0004f
#define STICK_TO_YAW_ANGLE_INC_FACT         0.0010f

#define PITCH_MAX 0.0f  //2580.0f 11.77f 
#define PITCH_MIN -58.0f   //1250.0f -46.4f 
#define YAW_MAX 33.5f
#define YAW_MIN -33.5f

void process_remote(void)
{
    rc_ctrl_now=get_remote_control_point();
    rotate_state= rc_ctrl_now->rc.s[1];
    shoot_state=rc_ctrl_now->rc.s[0];
    ////////////////////////////////
        pitch_target-=(rc_ctrl_now->rc.ch[1])* STICK_TO_PITCH_ANGLE_INC_FACT;
        if(pitch_target>PITCH_MAX)
            pitch_target=PITCH_MAX;
        if(pitch_target<PITCH_MIN)
            pitch_target=PITCH_MIN;
        
        yaw_target+=(-rc_ctrl_now->rc.ch[0])* STICK_TO_YAW_ANGLE_INC_FACT;
        if(yaw_target>YAW_MAX)
            yaw_target=YAW_MAX;
        if(yaw_target<YAW_MIN)
            yaw_target=YAW_MIN;
    /////////////////////////////////
    FBtarget=(-rc_ctrl_now->rc.ch[3])* STICK_TO_CHASSIS_SPEED_REF_FACT;
    LRtarget=(-rc_ctrl_now->rc.ch[2])* STICK_TO_CHASSIS_SPEED_REF_FACT;
    ROtarget=(rc_ctrl_now->rc.ch[0])* STICK_TO_CHASSIS_SPEED_REF_FACT;
    ;
}
void Control_Task(void){
    process_remote();
    gimbalControl(pitch_target,yaw_target);
    Fric_controlLoop(shoot_state);     //Ä¦²ÁÂÖ
    CMControlLoop(rotate_state,FBtarget,LRtarget,ROtarget);
//        gimbalControlLoop();
    RammerControlLoop(shoot_state);    //²¦µ¯
}
