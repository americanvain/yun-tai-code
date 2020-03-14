#ifndef _SPI_H_
#define _SPI_H_

#include "sys.h"

#define	SPI_CS PFout(6)

void SPI5_Init(void);//MPU6020
void SPI1_Init(void);//OLED

u8 SPI_ReadWriteByte(u8 TxData);
u8 SPI1_WriteByte(u8 TxData);

#endif

