/**
  ******************************************************************************
  * @file    main.c
  * @modify  tsymiar
  * @version V1.0.0
  * @date    21-May-2015
  * @brief   Hardware Configuration & Setup.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "main.h"

__IO uint32_t UserButtonPressed = 0;
__IO uint8_t DataReady = 0;

/* Private variables ---------------------------------------------------------*/
float gyroVal[3] = {0.0f};
u8  MAIN_BUF[32];

/**
  * @brief  Main program.
  * @param  None 
  * @retval None
  */
int main(void)
{  
  Init_User_Config();
   
  /* Reset UserButton_Pressed variable */
  UserButtonPressed = 0x00; 
	
  /* Infinite loop */
  while (1)
  {   
    /* LEDs Off */
		LEDs_Off();
    
    /* Waiting User Button is pressed */
    while (UserButtonPressed == 0x00)
    {
			MarqueeToggle();
    }
    
    DataReady = 0x00;
    
    /* All LEDs Off */
		LEDs_Off();
    
    /* Demo Gyroscope */
    My_GyroConfig();
    /* Waiting User Button is pressed */
    while (UserButtonPressed == 0x01)
    {
			Lamp_CatchSignal(Orange, LED_ON);
      /* Wait for data ready */
      while(DataReady != 0x05)
      {}
      DataReady = 0x00;
      
      /* LEDs Off */
      LEDs_Off();
         
      GyroToggle(gyroVal);
				
			if (gyroVal[2]!=0)
			{
					sprintf((char*)MAIN_BUF, "Zval=%f\t", (float)gyroVal[2]);
					HC05_Set_Cmd(MAIN_BUF);
			}
    }
        
    DataReady = 0x00;
    
    /* LEDs Off */
    LEDs_Off();
    
    /* My Compass */
    My_CompassConfig();
    
    /* Waiting User Button is pressed */
    while (UserButtonPressed == 0x02)
    {
			Lamp_CatchSignal(Orange, LED_ON);
      /* Wait for data ready */
      while(DataReady !=0x05)
      {}
      DataReady = 0x00;
			CompassToggle();
		} 
  }
}
