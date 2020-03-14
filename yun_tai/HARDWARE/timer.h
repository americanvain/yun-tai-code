#ifndef TIMER_H
#define TIMER_H
#include "main.h"

#define TIM3_NVIC 5
#define TIM6_NVIC 4

extern void TIM1_Init(uint16_t arr, uint16_t psc);
extern void TIM3_Init(uint16_t arr, uint16_t psc);
extern void TIM6_Init(void);
void TIM6_Start(void);
extern void TIM12_Init(uint16_t arr, uint16_t psc);
#endif
