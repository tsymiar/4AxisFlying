#ifndef __ESP8266_SET_H
#define __ESP8266_SET_H

#include "func_utils.h"

#define _WIFI_PASSTHROUGH_MODE    1
#define _WIFI_NORMAL_MODE         0

void ESP8266_Init(u32 bound);
u8 ESP8266_Test(void);
u8 Is_Connect_AP(void);
u8 Auto_Connect_Adapter(void);
u8 Disconnect(void);
u8 Close_Trans(void);
u8 Get_Work_Mode(void);
u8 Send_Cmd_Wait(const u8 *cmd, u8* ack, u32 waittime);

#endif /* __ESP8266_SET_H */
