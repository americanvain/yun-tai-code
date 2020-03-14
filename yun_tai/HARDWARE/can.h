#ifndef _CAN1_DATA_H_
#define _CAN1_DATA_H_
#include "main.h"
#define CAN1_NVIC 4
#define CAN2_NVIC 4

void CAN1_Init(uint8_t tsjw, uint8_t tbs2, uint8_t tbs1, uint16_t brp, uint8_t mode);
void CAN2_Init(uint8_t tsjw, uint8_t tbs2, uint8_t tbs1, uint16_t brp, uint8_t mode);

#endif 

