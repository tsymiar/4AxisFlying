/**
  ******************************************************************************
  * @file    main.c
  * @author  TsyQi
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
/* Private define ------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x   // get the absolute value.
/* Private variables ---------------------------------------------------------*/
  RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t UserButtonPressed = 0;
__IO float HeadingValue = 0.0f;  

__IO uint8_t DataReady = 0;

			/*campass*/						/*acceleration*/				/*gyroscope*/
float MagBuffer[3] = {0.0f}, AccBuffer[3] = {0.0f}, Buffer[3] = {0.0f};
//static uint8_t Val[50]={0.0f};
uint8_t Xval, Yval, Zval= 0x00;
	
float fNormAcc,fSinRoll,fSinPitch,fSinYaw,adCosRoll,adCosPitch,adCosYaw = 0.0f, RollAng = 0.0f, PitchAng = 0.0f, YawAng = 0.0f;
float fTiltedX,fTiltedY = 0.0f;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None 
  * @retval None
  */
int main(void)
{  
  uint8_t i = 0;
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  
  /* Initialize LEDs and User Button available on STM32F3-Discovery board */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED7);
  STM_EVAL_LEDInit(LED8);
  STM_EVAL_LEDInit(LED9);
  STM_EVAL_LEDInit(LED10);
  
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI); 

  /* Configure the USB */
  My_USBConfig();
  
  /* Reset UserButton_Pressed variable */
  UserButtonPressed = 0x00; 
   
  /* Infinite loop */
  while (1)
  {   
    /* LEDs Off */
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED6);
    STM_EVAL_LEDOff(LED7);
    STM_EVAL_LEDOff(LED4);
    STM_EVAL_LEDOff(LED10);
    STM_EVAL_LEDOff(LED8);
    STM_EVAL_LEDOff(LED9);
    STM_EVAL_LEDOff(LED5);
    
    /* Waiting User Button is pressed */
    while (UserButtonPressed == 0x00)
    {
      /* Toggle LD3 */
      STM_EVAL_LEDToggle(LED3);
      /* Insert 50 ms delay */
      Delay(5);
      /* Toggle LD5 */
      STM_EVAL_LEDToggle(LED5);
      /* Insert 50 ms delay */
      Delay(5);
      /* Toggle LD7 */
      STM_EVAL_LEDToggle(LED7);
      /* Insert 50 ms delay */
      Delay(5);
      /* Toggle LD9 */
      STM_EVAL_LEDToggle(LED9);
      /* Insert 50 ms delay */
      Delay(5);
      /* Toggle LD10 */
      STM_EVAL_LEDToggle(LED10);
      /* Insert 50 ms delay */
      Delay(5);
      /* Toggle LD8 */
      STM_EVAL_LEDToggle(LED8);
      /* Insert 50 ms delay */
      Delay(5); 
      /* Toggle LD6 */
      STM_EVAL_LEDToggle(LED6);
      /* Insert 50 ms delay */
      Delay(5);
      /* Toggle LD4 */
      STM_EVAL_LEDToggle(LED4);
      /* Insert 50 ms delay */
      Delay(5);
    }
    
    DataReady = 0x00;
    
    /* All LEDs Off */
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED6);
    STM_EVAL_LEDOff(LED7);
    STM_EVAL_LEDOff(LED4);
    STM_EVAL_LEDOff(LED10);
    STM_EVAL_LEDOff(LED8);
    STM_EVAL_LEDOff(LED9);
    STM_EVAL_LEDOff(LED5); 
    
    /* Demo Gyroscope */
    My_GyroConfig();
		My_CompassConfig();
    /* Waiting User Button is pressed */
    while (UserButtonPressed == 0x01)
    {
      /* Wait for data ready */
      while(DataReady != 0x05)
      {}
      DataReady = 0x00;
      
      /* LEDs Off */
			Lamp_CatchSignal(2,0);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED6);
      STM_EVAL_LEDOff(LED7);
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOff(LED10);
      STM_EVAL_LEDOff(LED8);
      STM_EVAL_LEDOff(LED9);
      STM_EVAL_LEDOff(LED5);
      
      /* Read Gyro Angular data */
      My_GyroReadAngRate(Buffer);
         
      /* Update autoreload and capture compare registers value*/
      Xval = ABS((int8_t)(Buffer[0]));
      Yval = ABS((int8_t)(Buffer[1])); 
			Zval = ABS((int8_t)(Buffer[2]));
//////////////////////////////////////
			if (Zval!=0)
			{
					BT_Usart2_Config(38400, Zval, 2);
					printf("Zval=%f\t", (float)Zval);
			}
///////////////////////////////////////
      if ( Xval>Yval && Xval>Zval )		// when the pitching angle is maximum of these three.
      {
        if ((int8_t)Buffer[0] > 5.0f)
        { 
          /* LD10 On */
          STM_EVAL_LEDOn(LED10);
        }
        if ((int8_t)Buffer[0] < -5.0f)
        { 
          /* LD3 On */
          STM_EVAL_LEDOn(LED3);
        }
      }
      else if(Yval>Xval && Yval>Zval )	// also when the Rolling angle is the largest.
      {
        if ((int8_t)Buffer[1] < -5.0f)
        {
          /* LD6 on */
          STM_EVAL_LEDOn(LED6);
        }
        if ((int8_t)Buffer[1] > 5.0f)
        {
          /* LD7 On */
          STM_EVAL_LEDOn(LED7);
        } 
      }
    }
        
    DataReady = 0x00;
    
    /* LEDs Off */
		Lamp_CatchSignal(2,0);
    STM_EVAL_LEDOff(LED4);
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED6);
    STM_EVAL_LEDOff(LED7);
    STM_EVAL_LEDOff(LED10);
    STM_EVAL_LEDOff(LED8);
    STM_EVAL_LEDOff(LED9);
    STM_EVAL_LEDOff(LED5);
    
    /* My Compass */
    My_CompassConfig();
    
    /* Waiting User Button is pressed */
    while (UserButtonPressed == 0x02)
    {
      /* Wait for data ready */
      while(DataReady !=0x05)
      {}
      DataReady = 0x00;
      
      /* Read Compass data */
      My_CompassReadMag(MagBuffer);
      My_CompassReadAcc(AccBuffer);
      
      for(i=0;i<3;i++)
        AccBuffer[i] /= 100.0f;
      
      fNormAcc = sqrt((AccBuffer[0]*AccBuffer[0])+(AccBuffer[1]*AccBuffer[1])+(AccBuffer[2]*AccBuffer[2]));
      
      fSinRoll = -AccBuffer[1]/fNormAcc;			//???????????
      arm_sqrt_f32(1.0f-(fSinRoll * fSinRoll), &adCosRoll);
      fSinPitch = AccBuffer[0]/fNormAcc;
      arm_sqrt_f32(1.0f-(fSinPitch * fSinPitch), &adCosPitch);
				
     if ( fSinRoll >0)
     {
       if (adCosRoll>0)
       {
         RollAng = acos(adCosRoll)*180/PI;
       }
       else
       {
         RollAng = acos(adCosRoll)*180/PI + 180;
       }
     }
     else
     {
       if (adCosRoll>0)
       {
         RollAng = acos(adCosRoll)*180/PI + 360;
       }
       else
       {
         RollAng = acos(adCosRoll)*180/PI + 180;
       }
     }
     
      if (fSinPitch >0)
     {
       if (adCosPitch>0)
       {
            PitchAng = acos(adCosPitch)*180/PI;
       }
       else
       {
          PitchAng = acos(adCosPitch)*180/PI + 180;
       }
     }
     else
     {
       if (adCosPitch>0)
       {
            PitchAng = acos(adCosPitch)*180/PI + 360;
       }
       else
       {
          PitchAng = acos(adCosPitch)*180/PI + 180;
       }
     }

      if (RollAng >=360)
      {
        RollAng = RollAng - 360;
      }
      
      if (PitchAng >=360)
      {
        PitchAng = PitchAng - 360;
      }
      
      fTiltedX = MagBuffer[0]*adCosPitch+MagBuffer[2]*fSinPitch;
      fTiltedY = MagBuffer[0]*fSinRoll*fSinPitch+MagBuffer[1]*adCosRoll-MagBuffer[1]*fSinRoll*adCosPitch;
      
      HeadingValue = (float) ((atan2f((float)fTiltedY,(float)fTiltedX))*180)/PI;
			
      if (HeadingValue < 0)
      {
        HeadingValue = HeadingValue + 360;    
      }
      
      if ((RollAng <= 40.0f) && (PitchAng <= 40.0f))
      {
        if (((HeadingValue < 25.0f)&&(HeadingValue >= 0.0f))||((HeadingValue >=340.0f)&&(HeadingValue <= 360.0f)))
        {
          STM_EVAL_LEDOn(LED10);
          STM_EVAL_LEDOff(LED3);
          STM_EVAL_LEDOff(LED6);
          STM_EVAL_LEDOff(LED7);
          STM_EVAL_LEDOff(LED4);
          STM_EVAL_LEDOff(LED8);
          STM_EVAL_LEDOff(LED9);
          STM_EVAL_LEDOff(LED5);
        }
        else  if ((HeadingValue <70.0f)&&(HeadingValue >= 25.0f))
        {
          STM_EVAL_LEDOn(LED9);
          STM_EVAL_LEDOff(LED6);
          STM_EVAL_LEDOff(LED10);
          STM_EVAL_LEDOff(LED3);
          STM_EVAL_LEDOff(LED8);
          STM_EVAL_LEDOff(LED5);
          STM_EVAL_LEDOff(LED4);
          STM_EVAL_LEDOff(LED7);
        } 
        else  if ((HeadingValue < 115.0f)&&(HeadingValue >= 70.0f))
        {
          STM_EVAL_LEDOn(LED7);
          STM_EVAL_LEDOff(LED3);
          STM_EVAL_LEDOff(LED4);
          STM_EVAL_LEDOff(LED9);
          STM_EVAL_LEDOff(LED10);
          STM_EVAL_LEDOff(LED8);
          STM_EVAL_LEDOff(LED6);
          STM_EVAL_LEDOff(LED5);
        }
        else  if ((HeadingValue <160.0f)&&(HeadingValue >= 115.0f))
        {
          STM_EVAL_LEDOn(LED5);
          STM_EVAL_LEDOff(LED6);
          STM_EVAL_LEDOff(LED10);
          STM_EVAL_LEDOff(LED8);
          STM_EVAL_LEDOff(LED9);
          STM_EVAL_LEDOff(LED7);
          STM_EVAL_LEDOff(LED4);
          STM_EVAL_LEDOff(LED3);
        } 
        else  if ((HeadingValue <205.0f)&&(HeadingValue >= 160.0f))
        {
          STM_EVAL_LEDOn(LED3);
          STM_EVAL_LEDOff(LED6);
          STM_EVAL_LEDOff(LED4);
          STM_EVAL_LEDOff(LED8);
          STM_EVAL_LEDOff(LED9);
          STM_EVAL_LEDOff(LED5);
          STM_EVAL_LEDOff(LED10);
          STM_EVAL_LEDOff(LED7);
        } 
        else  if ((HeadingValue <250.0f)&&(HeadingValue >= 205.0f))
        {
          STM_EVAL_LEDOn(LED4);
          STM_EVAL_LEDOff(LED6);
          STM_EVAL_LEDOff(LED10);
          STM_EVAL_LEDOff(LED8);
          STM_EVAL_LEDOff(LED9);
          STM_EVAL_LEDOff(LED5);
          STM_EVAL_LEDOff(LED3);
          STM_EVAL_LEDOff(LED7);
        } 
        else  if ((HeadingValue < 295.0f)&&(HeadingValue >= 250.0f))
        {
          STM_EVAL_LEDOn(LED6);
          STM_EVAL_LEDOff(LED9);
          STM_EVAL_LEDOff(LED10);
          STM_EVAL_LEDOff(LED8);
          STM_EVAL_LEDOff(LED3);
          STM_EVAL_LEDOff(LED5);
          STM_EVAL_LEDOff(LED4);
          STM_EVAL_LEDOff(LED7);
        }        
        else  if ((HeadingValue < 340.0f)&&(HeadingValue >= 295.0f))
        {
          STM_EVAL_LEDOn(LED8);
          STM_EVAL_LEDOff(LED6);
          STM_EVAL_LEDOff(LED10);
          STM_EVAL_LEDOff(LED7);
          STM_EVAL_LEDOff(LED9);
          STM_EVAL_LEDOff(LED3);
          STM_EVAL_LEDOff(LED4);
          STM_EVAL_LEDOff(LED5);
        }
      }
      else
      {
        /* Toggle All LEDs */
        STM_EVAL_LEDToggle(LED7);
        STM_EVAL_LEDToggle(LED6);
        STM_EVAL_LEDToggle(LED10);
        STM_EVAL_LEDToggle(LED8);
        STM_EVAL_LEDToggle(LED9);
        STM_EVAL_LEDToggle(LED3);
        STM_EVAL_LEDToggle(LED4);
        STM_EVAL_LEDToggle(LED5);
        /* Delay 50ms */
        Delay(5);
      }
    }
  }
}
