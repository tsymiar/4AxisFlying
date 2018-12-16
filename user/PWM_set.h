#ifndef __PWM_SET_H
#define __PWM_SET_H

#include "comm_include"

void PWM_Init(uint32_t OUT_Pin);
void PWM_Set(int moto1, int moto2, int moto3, int moto4);

#endif /* __PWM_SET_H */
