#ifndef __HC05_SET_H
#define __HC05_SET_H

#include "func_utils.h"

u8 HC05_Init(void);
u8 HC05_Get_Role(void);
u8 HC05_Set_Cmd(u8* atstr);
void HC05_Debug(u8 *str);

#endif /* __HC05_SET_H */
