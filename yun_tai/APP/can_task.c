/**
  ******************************************************************************
  * @file       can_task.c/h
  * @brief      完成can设备完成
  * @note       该文件参与freeRTOS系统
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2020/1/10       young              
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ******************************************************************************
  */
	
#include "can.h"
#include "can_task.h"
#include "can_data.h"
#include "stm32f4xx.h"

void CAN_Device_Init(){
	CAN_Motor_Config(YAW_MOTOR,YAW_MOTOR_ID,YAW_MOTOR_TYPE,YAW_MOTOR_CH,YAW_MOTOR_BIAS);
	CAN_Motor_Config(PIT_MOTOR,PIT_MOTOR_ID,PIT_MOTOR_TYPE,PIT_MOTOR_CH,PIT_MOTOR_BIAS);
	CAN_Motor_Config(RAMMER_MOTOR,RAMMER_MOTOR_ID,RAMMER_MOTOR_TYPE,RAMMER_MOTOR_CH,RAMMER_MOTOR_BIAS);
	CAN_Motor_Config(FRIC1_MOTOR,FRIC1_MOTOR_ID,FRIC1_MOTOR_TYPE,FRIC1_MOTOR_CH,FRIC1_MOTOR_BIAS);
	CAN_Motor_Config(FRIC2_MOTOR,FRIC2_MOTOR_ID,FRIC2_MOTOR_TYPE,FRIC2_MOTOR_CH,FRIC2_MOTOR_BIAS);
	CAN_ID_CHECK();
//	CAN_id_send_Print();
	CAN1_Init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
	CAN2_Init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
}


void CanMsgPrc(){
	SendMotorCurrent(YAW_MOTOR);
}
	
