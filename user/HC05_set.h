#ifndef __HC05_DEBUG_H
#define __HC05_DEBUG_H

#include "common_functions.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#define HC05_LED  	BIT_ADDR(GPIOC_BASE+8,5)		
#define HC05_KEY  	BIT_ADDR(GPIOC_BASE+12,4)		

#endif /* __HC_05_H */
