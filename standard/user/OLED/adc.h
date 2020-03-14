#ifndef ADC_H
#define ADC_H

#include <stdint.h>

extern void OLED_Button_ADC_init(void);
uint16_t get_ADC(uint8_t ch);
#endif

