#ifndef __GIMBAL_CONTROL_H__
#define __GIMBAL_CONTROL_H__
#include "sys.h"
#include "pid.h"
#include "main.h"

typedef struct {
    float pitch_angle_target;   //×ÜÊÇ±àÂëÆ÷
    float yaw_angle_target_imu;
    float yaw_angle_target_ecd;
    float pitch_angle_target_imu;
    float pitch_angle_target_ecd;
}Gimbal_Target_t;


typedef enum {
    YAW_ENCODER,
    YAW_IMU
}YAW_MODE;

void gimbal_stop(void);
void gimbalControl(float pitch_angle_target,float yaw_angle_target);


#endif
