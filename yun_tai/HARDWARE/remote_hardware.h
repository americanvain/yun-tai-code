#ifndef _REOMTE_HARDWARE_H_
#define _REOMTE_HARDWARE_H_

#include "main.h"

#define RC_NVIC 4

#define SBUS_RX_BUF_NUM 36u


void RC_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
void RC_unable(void);
void RC_restart(uint16_t dma_buf_num);


#endif
