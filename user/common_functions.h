#ifndef __MY_FUNCTIONS_H
#define __MY_FUNCTIONS_H

#include "common_include"

// bt
u8 HC05_Init(void);
u8 HC05_Get_Role(void);
void BT_Usart2_Config(uint32_t rate, uint32_t addr, uint8_t len);
// pwm_set
void PWM_Init(uint32_t OUT_Pin);
void PWM_Set(int moto1, int moto2, int moto3, int moto4);

int fputc(int ch, FILE *f);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
void Joystick_Send(uint8_t Keys, USART_TypeDef * USARTn);
void Lamp_CatchSignal(int color, int state);
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, uint32_t Addr, u8 len);

void My_USBConfig(void);
void My_GyroConfig(void);
void My_CompassConfig(void);
void My_GyroReadAngRate(float* pfData);
void My_CompassReadMag(float* pfData);
void My_CompassReadAcc(float* pfData);

#endif /* __MY_FUNCTIONS_H */
