/**
  ******************************************************************************
  * @file       can_task.c/h
  * @brief      完成can设备完成电机数据读取、设置控制命令、发送控制命令、电机参数配置等四个主要任务
  * @note       该文件不是freeRTOS任务
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
	
#include "can_data.h"
#include "can.h"
#include "main.h"
#include "stm32f4xx.h"
//#include "sys.h"
#include "string.h"

/// @brief CAN通信设备配置配置信息
static Can_Cfg_Info_t 	can_cfg_info[CAN_DEVICE_NUM];
/// @brief CAN通信设备接收数据(ecd_angle取4000防止电机启动时在特殊位置导致cycles++)
static Motor_Data_t 	motor_data[CAN_DEVICE_NUM]={4000,0,0,0,0};
static Motor_Data_t 	motor_last_data[CAN_DEVICE_NUM]={4000,0,0,0,0};

/// @brief CAN通信设备发送数据
static uint8_t data_CAN1_0x200[8]={0};
static uint8_t data_CAN1_0x1FF[8]={0};
static uint8_t data_CAN1_0x2FF[8]={0};
static uint8_t data_CAN2_0x200[8]={0};
static uint8_t data_CAN2_0x1FF[8]={0};
static uint8_t data_CAN2_0x2FF[8]={0};

/// @brief 用于自检
int8_t pitch_Connectionstate;
int8_t yaw_Connectionstate;
int8_t fric1_Connectionstate;
int8_t fric2_Connectionstate;
int8_t rammer_Connectionstate;
int8_t Chassis_Connectionstate;

/**
@brief 电机数据读取
@param device_seq必须是电机的设备序列号
@return 设备对应的Motor_Data_t类型的数据，
	所有电机都有角度信息，但不是都有其他信息
	详细参考Motor_Data_t结构体注释
*/
Motor_Data_t GetMotorData(uint8_t device_seq){
	if(can_cfg_info[device_seq-1].type>Chassis){
		return motor_data[device_seq-1];
	}
	else {
    Motor_Data_t no_data={0};
//		printf("device %d is not a motor\r\n",device_seq);
		return no_data;
	}
}


/**
@brief 设定电机的控制数据(未发送)
控制报文ID与可对应电机
0x200	C610,C620
0x1FF	C610,C620,GM6020
0x2FF	GM6020
@param device_seq必须是电机的设备序列号
@param current是电机的控制电流
*/
void SetMotorCurrent(uint8_t device_seq, int16_t current){
	if(can_cfg_info[device_seq-1].type>Chassis){
		///<限幅
		switch(can_cfg_info[device_seq-1].type){
		case C610:		VAL_LIMIT(current, -10000, 10000);break;
		case C620:		VAL_LIMIT(current, -16384, 16384);break;
		case GM6020:	VAL_LIMIT(current, -30000, 30000);break;
		default:break;
		}
//can1设置
		if(can_cfg_info[device_seq-1].ch==CAN_1){
			switch(can_cfg_info[device_seq-1].id_send){
				case 0x200:{
					data_CAN1_0x200[2*can_cfg_info[device_seq-1].id-2]	=(uint8_t)(current >> 8);
					data_CAN1_0x200[2*can_cfg_info[device_seq-1].id-1]	=(uint8_t)current;
				}break;
				case 0x1FF:{
					if(can_cfg_info[device_seq-1].type==C610||can_cfg_info[device_seq-1].type==C620){
						data_CAN1_0x1FF[2*(can_cfg_info[device_seq-1].id-4)-2]	=(uint8_t)(current >> 8);
						data_CAN1_0x1FF[2*(can_cfg_info[device_seq-1].id-4)-1]	=(uint8_t)current;
					}
					else if(can_cfg_info[device_seq-1].type==GM6020||can_cfg_info[device_seq-1].type==GM6020){
						data_CAN1_0x1FF[2*can_cfg_info[device_seq-1].id-2]	=(uint8_t)(current >> 8);
						data_CAN1_0x1FF[2*can_cfg_info[device_seq-1].id-1]	=(uint8_t)current;
					}
				}break;
				case 0x2FF:{
					if(can_cfg_info[device_seq-1].type==GM6020){
						data_CAN1_0x2FF[2*(can_cfg_info[device_seq-1].id-4)-2]	=(uint8_t)(current >> 8);
						data_CAN1_0x2FF[2*(can_cfg_info[device_seq-1].id-4)-1]	=(uint8_t)current;
					}
				}break;
			}
		}
//can2设置		
		else if(can_cfg_info[device_seq-1].ch==CAN_2){
			switch(can_cfg_info[device_seq-1].id_send){
				case 0x200:{
					data_CAN2_0x200[2*can_cfg_info[device_seq-1].id-2]	=(uint8_t)(current >> 8);
					data_CAN2_0x200[2*can_cfg_info[device_seq-1].id-1]	=(uint8_t)current;
				}break;
				case 0x1FF:{
					if(can_cfg_info[device_seq-1].type==C610||can_cfg_info[device_seq-1].type==C620){
						data_CAN2_0x1FF[2*(can_cfg_info[device_seq-1].id-4)-2]	=(uint8_t)(current >> 8);
						data_CAN2_0x1FF[2*(can_cfg_info[device_seq-1].id-4)-1]	=(uint8_t)current;
					}
					else if(can_cfg_info[device_seq-1].type==GM6020||can_cfg_info[device_seq-1].type==GM6020){
						data_CAN2_0x1FF[2*can_cfg_info[device_seq-1].id-2]	=(uint8_t)(current >> 8);
						data_CAN2_0x1FF[2*can_cfg_info[device_seq-1].id-1]	=(uint8_t)current;
					}
				}break;
				case 0x2FF:{
					if(can_cfg_info[device_seq-1].type==GM6020){
						data_CAN2_0x2FF[2*(can_cfg_info[device_seq-1].id-4)-2]	=(uint8_t)(current >> 8);
						data_CAN2_0x2FF[2*(can_cfg_info[device_seq-1].id-4)-1]	=(uint8_t)current;
					}
				}break;
			}
		}
	}
//	else printf("device %d is not a motor\r\n",device_seq);
}



/**
@brief 发送云台控制数据
@param device_seq必须是电机的设备序列号
*/
void SendMotorCurrent(uint8_t device_seq){
	if(can_cfg_info[device_seq-1].type>Chassis){
		CanTxMsg tx_message;
		tx_message.StdId = can_cfg_info[device_seq-1].id_send;
//		gimbal_yaw_iq = 0;
		tx_message.IDE = CAN_Id_Standard;
		tx_message.RTR = CAN_RTR_Data;
		tx_message.DLC = 0x08;
		switch(can_cfg_info[device_seq-1].id_send){
			case 0x200:{
				if(can_cfg_info[device_seq-1].ch==CAN_1){
					memcpy(tx_message.Data,data_CAN1_0x200,sizeof(tx_message.Data));
					CAN_Transmit(CAN1,&tx_message);
				}
				else{
					memcpy(tx_message.Data,data_CAN2_0x200,sizeof(tx_message.Data));
					CAN_Transmit(CAN2,&tx_message);
				}
			}break;
			case 0x1FF:{
				if(can_cfg_info[device_seq-1].ch==CAN_1){
					memcpy(tx_message.Data,data_CAN1_0x1FF,sizeof(tx_message.Data));
					CAN_Transmit(CAN1,&tx_message);
				}
				else{
					memcpy(tx_message.Data,data_CAN2_0x1FF,sizeof(tx_message.Data));
					CAN_Transmit(CAN2,&tx_message);
				}
			}break;
			case 0x2FF:{
				if(can_cfg_info[device_seq-1].ch==CAN_1){
					memcpy(tx_message.Data,data_CAN1_0x2FF,sizeof(tx_message.Data));
					CAN_Transmit(CAN1,&tx_message);
				}
				else{
					memcpy(tx_message.Data,data_CAN2_0x2FF,sizeof(tx_message.Data));
					CAN_Transmit(CAN2,&tx_message);
				}
			}break;
		}
	}
}

/**
@brief 发送底盘速度
@param forward_back_target	前后速度
@param left_right_target	左右速度
@param rotate_target		自旋速度
@param chasis_heat			底盘热量
*/
void SendChassisSpeed(CAN_TypeDef *CANx, int16_t forward_back_target, int16_t left_right_target, int16_t rotate_target, int16_t chasis_heat){
    CanTxMsg tx_message;
    tx_message.StdId = 0x401;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    tx_message.Data[0] = (uint8_t)(forward_back_target >> 8);
    tx_message.Data[1] = (uint8_t)forward_back_target;
    tx_message.Data[2] = (uint8_t)(left_right_target >> 8);
    tx_message.Data[3] = (uint8_t)left_right_target;
    tx_message.Data[4] = (uint8_t)(rotate_target >> 8);
    tx_message.Data[5] = (uint8_t)rotate_target;
    tx_message.Data[6] = (uint8_t)(chasis_heat >> 8);
    tx_message.Data[7] = (uint8_t)chasis_heat;
    CAN_Transmit(CANx,&tx_message);
}



/**
@brief CAN电机信息配置
@param device_seq	设备序列号,用于区分设备，必须小于CAN_DEVICE_NUM
@param ID			电调拨码卡关ID,若为底盘则为底盘的接收ID
@param device		设备类型，详细参考Motor_Data_t结构体注释
@param CAN_x 		CAN_1或CAN_2
@param bias			码盘角度计算的参考偏移量
*/
void CAN_Motor_Config(uint8_t device_seq,uint32_t ID,Device_Type_e device,Can_Channel_e CAN_x,int16_t bias){
	if(device_seq<=CAN_DEVICE_NUM){
		can_cfg_info[device_seq-1].id=ID;
		can_cfg_info[device_seq-1].ch=CAN_x;
		can_cfg_info[device_seq-1].type=device;
		can_cfg_info[device_seq-1].ecd_bias=bias;
		switch(device){
			case C610:{
				if(ID<=4){
					can_cfg_info[device_seq-1].id_send=0x200;
					can_cfg_info[device_seq-1].id_recieve=0x200+ID;
				}
				else if(ID<=8){
					can_cfg_info[device_seq-1].id_send=0x1FF;
					can_cfg_info[device_seq-1].id_recieve=0x200+ID;
				}
			}break;
			case C620:{
				if(ID<=4){
					can_cfg_info[device_seq-1].id_send=0x200;
					can_cfg_info[device_seq-1].id_recieve=0x200+ID;
				}
				else if(ID<=8){
					can_cfg_info[device_seq-1].id_send=0x1FF;
					can_cfg_info[device_seq-1].id_recieve=0x200+ID;
				}
			}break;
			case GM6020:{
				if(ID>=1&&ID<=4){
					can_cfg_info[device_seq-1].id_send=0x1FF;
					can_cfg_info[device_seq-1].id_recieve=0x204+ID;
				}
				else if(ID<=7){
					can_cfg_info[device_seq-1].id_send=0x2FF;
					can_cfg_info[device_seq-1].id_recieve=0x204+ID;
				}
			}break;
			
			case Chassis:{
				can_cfg_info[device_seq-1].id_send=ID;
			}break;
			
			default:{
//				printf("no such device\r\n");
			}break;
		}
	}
//	else printf("seq必须小于CAN_DEVICE_NUM\r\n");
}
//**
//=================================================我是一条分界线===============================================
//=================================================其他相关函数===============================================

void angle_convert(uint8_t seq){
	motor_data[seq-1].angle=(motor_data[seq-1].ecd_angle-can_cfg_info[seq-1].ecd_bias)*360.00/Full_Ecd_Angle;
	if(motor_data[seq-1].angle>180){
		motor_data[seq-1].angle-=360.0f;
	}
	else if(motor_data[seq-1].angle<-180){
		motor_data[seq-1].angle+=360.0f;
	}
	if((motor_data[seq-1].angle-motor_last_data[seq-1].angle)>300)motor_data[seq-1].cycles--;
	else if((motor_data[seq-1].angle-motor_last_data[seq-1].angle)<-300)motor_data[seq-1].cycles++;
}
/**
@brief 判断是否有重复ID
只能在所有设备配置完后执行
*/
void CAN_ID_CHECK(void){
	int i=0;
	int j;
	for(i=0;i<CAN_DEVICE_NUM;i++){
		int id_i=0;
    id_i=can_cfg_info[i].id;
		for(j=i+1;j<CAN_DEVICE_NUM;j++){
			int id_j=0;
	    id_j=can_cfg_info[j].id;
			if(can_cfg_info[i].ch==can_cfg_info[j].ch){
				if(can_cfg_info[i].id_send==can_cfg_info[j].id_send){
					if(id_i==id_j)
                        ;
//                        printf("序列%d设备发送ID重复\r\n",i+1);
				}
				if(can_cfg_info[i].id_recieve==can_cfg_info[j].id_recieve){
                    ;
//					printf("序列%d设备接收ID重复\r\n",i+1);
				}
			}
		}
	}
}
/**
@brief 打印设备控制ID
只能在所有设备配置完后执行
*/
void CAN_id_send_Print(void){
	int i=0;
	for(i=0;i<CAN_DEVICE_NUM;i++){
//		printf("设备序列:%d/id_send:%#X\r\n",i+1,can_cfg_info[i].id_send);
	}
}

void angle_convert(uint8_t seq);

void can_msg_encode( CanRxMsg * msg, Can_Channel_e CAN_x){
	int i=0;
	for(;i<CAN_DEVICE_NUM;i++){
		if(can_cfg_info[i].ch==CAN_x){
			if(msg->StdId==can_cfg_info[i].id_recieve){
				switch(can_cfg_info[i].type){
					case C610:{
                        if(rammer_Connectionstate<10)
                        {
                            rammer_Connectionstate++;
                        }
						motor_last_data[i]=motor_data[i];
						motor_data[i].ecd_angle	=(msg->Data[0]<<8)|msg->Data[1];
						motor_data[i].speed	=(msg->Data[2]<<8)|msg->Data[3];
						motor_data[i].torque=(msg->Data[4]<<8)|msg->Data[5];
						angle_convert(i+1);//换算角度
					}break;
					
					case C620:{
                        if(i==FRIC1_MOTOR-1)
                        {
                            if(fric1_Connectionstate<10)
                                {
                                    fric1_Connectionstate++;
                                }
                        }
                        else if(i==FRIC2_MOTOR-1)
                        {
                            if(fric2_Connectionstate<10)
                                {
                                    fric2_Connectionstate++;
                                }                            
                        }                        
						motor_last_data[i]=motor_data[i];
						motor_data[i].ecd_angle	=(msg->Data[0]<<8)|msg->Data[1];
						motor_data[i].speed	=(msg->Data[2]<<8)|msg->Data[3];
						motor_data[i].torque=(msg->Data[4]<<8)|msg->Data[5];    //c620此项对应电流
						motor_data[i].temperature=msg->Data[6];
						angle_convert(i+1);
					}break;
					case GM6020:{
                        if(i==PIT_MOTOR-1)
                        {
                            if(pitch_Connectionstate<10)
                                {
                                    pitch_Connectionstate++;
                                }       
                        }
                        else if(i==YAW_MOTOR-1)
                        {
                            if(yaw_Connectionstate<10)
                                {
                                    yaw_Connectionstate++;
                                }     
                        }
						motor_last_data[i]=motor_data[i];
						motor_data[i].ecd_angle	=(msg->Data[0]<<8)|msg->Data[1];
						motor_data[i].speed	=(msg->Data[2]<<8)|msg->Data[3];
						motor_data[i].torque=(msg->Data[4]<<8)|msg->Data[5];
						motor_data[i].temperature=msg->Data[6];
						angle_convert(i+1);
					}break;

					case Chassis:{
                        if(Chassis_Connectionstate<10)
                        {
                            Chassis_Connectionstate++;
                        }
					}break;
					default:break;
				}
				i=CAN_DEVICE_NUM;	///<结束循环
			}
		}
	}
}

/*
    @ author: 张之宇
    @ latest  update:2018年10月14日;
    @ function: 底盘自检，OK返回1，错误返回0，main.c中调用
*/
int8_t chassisSelfCheck(void){
    if(pitch_Connectionstate!=10)
        return 1;
    if(yaw_Connectionstate!=10)
        return 1;
    if(fric1_Connectionstate!=10)
        return 1;
    if(fric2_Connectionstate!=10)
        return 1;
    if(rammer_Connectionstate!=10)
        return 1;
//    if(Chassis_Connectionstate!=10)
//        return 1;
    return 0;
}
