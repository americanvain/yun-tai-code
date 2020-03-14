#ifndef _CAN_DATA1_H_
#define _CAN_DATA1_H_

#include "main.h"
#include "stm32f4xx.h"
#define Full_Ecd_Angle (8192.00)

#define CAN_DEVICE_NUM 5
//Yaw轴电机
#define YAW_MOTOR		1
#define	YAW_MOTOR_ID	4
#define YAW_MOTOR_TYPE	GM6020
#define YAW_MOTOR_CH	CAN_1
#define YAW_MOTOR_BIAS	2008
//Pitch轴电机
#define PIT_MOTOR		2
#define	PIT_MOTOR_ID	1
#define PIT_MOTOR_TYPE	GM6020
#define PIT_MOTOR_CH	CAN_1
#define PIT_MOTOR_BIAS	2598
//拨弹电机
#define RAMMER_MOTOR		3
#define	RAMMER_MOTOR_ID		3
#define RAMMER_MOTOR_TYPE	C610
#define RAMMER_MOTOR_CH		CAN_1
#define RAMMER_MOTOR_BIAS	0

//水平摩擦轮
#define FRIC1_MOTOR			4
#define	FRIC1_MOTOR_ID		1
#define FRIC1_MOTOR_TYPE	C620
#define FRIC1_MOTOR_CH		CAN_1
#define FRIC1_MOTOR_BIAS	0

#define FRIC2_MOTOR			5
#define	FRIC2_MOTOR_ID		2
#define FRIC2_MOTOR_TYPE	C620
#define FRIC2_MOTOR_CH		CAN_1
#define FRIC2_MOTOR_BIAS	0

#define VAL_LIMIT(val, min, max)\
if(val<=min)\
{\
	val = min;\
}\
else if(val>=max)\
{\
	val = max;\
}\
else {val = val;}\

//------------------------------------------------------------
//枚举类型与数据结构体定义
//------------------------------------------------------------
/**
@brief CAN通道枚举
*/
typedef enum
{
	CAN_1	= 1,
	CAN_2	= 2,
}Can_Channel_e;

/**
@brief CAN设备枚举
*/
typedef enum
{
	Chassis	= 0,
	C610    = 1,///<C610电调,主要驱动M2006
	C620	= 2,///<C620电调，主要驱动M3510
	GM6020	= 3,
}Device_Type_e;
/**
@brief CAN设备配置信息
*/
typedef struct
{
	Can_Channel_e	ch;				///<CAN设备通道
	uint8_t			id;				///<CAN电调拨码开关ID
	uint32_t		id_send;		///<CAN控制ID
	uint32_t 		id_recieve;		///<CAN反馈ID
	Device_Type_e	type;			///<CAN设备类型
	int16_t 		ecd_bias;		///<码盘角度计算的参考偏移量
}Can_Cfg_Info_t;
/**
@brief 电机数据结构体(*为有这个数据)
type	angle	speed	torque	temperature
C610		*		*		*
C620		*		*		*		*
GM6020	*		*		*		*
RM6623	*				*		
GM3510	*				*
RM820R	*		*
Chassis	(Not a motor)
*/
typedef struct
{
	int16_t  ecd_angle;
	int16_t  cycles;
	float    angle;
	int16_t  speed;
	int16_t  torque;
	int8_t  temperature;		
}Motor_Data_t;


Motor_Data_t GetMotorData(uint8_t device_seq);
//void SetMotorCurrent(uint8_t device_seq, int16_t current);
//void SendMotorCurrent(uint8_t device_seq);
//void SendChassisSpeed(CAN_TypeDef *CANx, int16_t forward_back_target, int16_t left_right_target, int16_t rotate_target, int16_t chasis_heat);
void CAN_ID_CHECK(void);
void CAN_id_send_Print(void);
void can_msg_encode(CanRxMsg * msg, Can_Channel_e CAN_x);
void CAN_Motor_Config(uint8_t device_seq,uint32_t ID,Device_Type_e device,Can_Channel_e CAN_x,int16_t bias);

int8_t chassisSelfCheck(void);
#endif 


