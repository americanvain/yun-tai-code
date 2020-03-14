#ifndef _CAN_BUS_TASK_H_
#define _CAN_BUS_TASK_H_

#include "sys.h"
#include "can_data.h"

void CAN_Device_Init(void);
void CanMsgPrc(void);

void SetMotorCurrent(uint8_t device_seq, int16_t current);
void SendMotorCurrent(uint8_t device_seq);
void SendChassisSpeed(CAN_TypeDef *CANx, int16_t forward_back_target, int16_t left_right_target, int16_t rotate_target, int16_t chasis_heat);
//Motor_Data_t GetMotorData(uint8_t device_seq);

#endif

