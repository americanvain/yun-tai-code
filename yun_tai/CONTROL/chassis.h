#ifndef CHASSIS_CONTROL_H
#define CHASSIS_CONTROL_H

#include "main.h"
#include "sys.h"
#include "math.h"

//s[1]通道控制
#define CHASSISMOVE 1
#define CM360rotate 2
#define CM360rotateStop 3


///	@brief	底盘目标速度结构体
typedef __packed struct{
	int16_t forward_back_target;
	int16_t left_right_target;
	int16_t rotate_target;
	int16_t rotate_target360;
}ChassisSpeed_Target_t;

void CMControlLoop(char rotate,int16_t FB,int16_t LR,int16_t RO);

#endif


