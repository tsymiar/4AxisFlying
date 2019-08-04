#ifndef __PWM_SIG_H
#define __PWM_SIG_H

#include "comm_include"

void PWM_Config(uint32_t OUT_Pin);
void PWM_Set(int moto1, int moto2, int moto3, int moto4);

#endif /* __PWM_SIG_H */
