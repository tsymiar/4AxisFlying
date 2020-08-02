#ifndef __FUNC_UTILS_H
#define __FUNC_UTILS_H

#include "comm_include"

void u2printf(char* fmt,...);
int fputc(int ch, FILE *f);

void System_Clock_Config(void);
void UART1_Config(void);
void USB_Config(void);
void TimingDelay_Decrement(void);
void Delay_us(__IO uint32_t nTime);

void My_GyroConfig(void);
void My_CompassConfig(void);
void My_GyroReadAngRate(float* pfData);
void My_CompassReadMag(float* pfData);
void My_CompassReadAcc(float* pfData);

#endif /* __FUNC_UTILS_H */
