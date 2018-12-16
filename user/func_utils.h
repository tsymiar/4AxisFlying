#ifndef __FUNC_UTILS_H
#define __FUNC_UTILS_H

#include "comm_include"

void u2_printf(char* fmt,...);
int fputc(int ch, FILE *f);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
void Joystick_Send(uint8_t Keys, USART_TypeDef * USARTn);
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, uint32_t Addr, u8 len);

void My_USBConfig(void);
void My_GyroConfig(void);
void My_CompassConfig(void);
void My_GyroReadAngRate(float* pfData);
void My_CompassReadMag(float* pfData);
void My_CompassReadAcc(float* pfData);

#endif /* __FUNC_UTILS_H */
