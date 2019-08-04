#include "LED_set.h"

/* Private variables ---------------------------------------------------------*/
__IO float HeadingValue = 0.0f;  

			/*campass*/						/*acceleration*/				/*gyroscope*/
float MagBuffer[3] = {0.0f}, AccBuffer[3] = {0.0f}, Buffer[3] = {0.0f};
uint8_t Xval, Yval, Zval= 0x00;
	
float fNormAcc,fSinRoll,fSinPitch,fSinYaw,adCosRoll,adCosPitch,adCosYaw = 0.0f, RollAng = 0.0f, PitchAng = 0.0f, YawAng = 0.0f;
float fTiltedX,fTiltedY = 0.0f;

struct LED_COLOR {
		Led_TypeDef LED;
    u8 COLOR; 
};

const struct LED_COLOR leds_color[] = {
		{LED3, Red},
		{LED4, Blue},
		{LED5, Orange},
		{LED6, Green},
		{LED7, Green},
		{LED8, Orange},
		{LED9, Blue},
		{LED10, Red}
};

/* Public functions ---------------------------------------------------------*/
void LED_Init()
{  
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
}

void LEDs_Off()
{
    /* All LEDs Off */
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED6);
    STM_EVAL_LEDOff(LED7);
    STM_EVAL_LEDOff(LED4);
    STM_EVAL_LEDOff(LED10);
    STM_EVAL_LEDOff(LED8);
    STM_EVAL_LEDOff(LED9);
    STM_EVAL_LEDOff(LED5); 
}

void LEDx_Blink(Led_TypeDef LED)
{
		LEDs_Off();
    STM_EVAL_LEDOn(LED);
}

void LEDs_Toggle()
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
}

void MarqueeToggle()
{
    /* Toggle LD3 */
      STM_EVAL_LEDToggle(LED3);
      /* Insert 50 ms delay */
      Delay_us(5);
      /* Toggle LD5 */
      STM_EVAL_LEDToggle(LED5);
      /* Insert 50 ms delay */
      Delay_us(5);
      /* Toggle LD7 */
      STM_EVAL_LEDToggle(LED7);
      /* Insert 50 ms delay */
      Delay_us(5);
      /* Toggle LD9 */
      STM_EVAL_LEDToggle(LED9);
      /* Insert 50 ms delay */
      Delay_us(5);
      /* Toggle LD10 */
      STM_EVAL_LEDToggle(LED10);
      /* Insert 50 ms delay */
      Delay_us(5);
      /* Toggle LD8 */
      STM_EVAL_LEDToggle(LED8);
      /* Insert 50 ms delay */
      Delay_us(5); 
      /* Toggle LD6 */
      STM_EVAL_LEDToggle(LED6);
      /* Insert 50 ms delay */
      Delay_us(5);
      /* Toggle LD4 */
      STM_EVAL_LEDToggle(LED4);
      /* Insert 50 ms delay */
      Delay_us(5);
}

void GyroToggle(float* gyroVal)
{
			/* Read Gyro Angular data */
      My_GyroReadAngRate(Buffer);
         
      /* Update autoreload and capture compare registers value*/
			Xval = ABS((int8_t)(Buffer[0]));
			Yval = ABS((int8_t)(Buffer[1])); 
			Zval = ABS((int8_t)(Buffer[2]));
      gyroVal[0] = ABS(Buffer[0]);
      gyroVal[1] = ABS(Buffer[0]);
			gyroVal[2] = ABS(Buffer[0]);
	
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

void CompassToggle()
{
			uint8_t i = 0;
      /* Read Compass data */
      My_CompassReadMag(MagBuffer);
      My_CompassReadAcc(AccBuffer);
      for(i = 0; i < 3; i++)
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
         RollAng = acos(adCosRoll)*180/_PI_;
       }
       else
       {
         RollAng = acos(adCosRoll)*180/_PI_ + 180;
       }
     }
     else
     {
       if (adCosRoll>0)
       {
         RollAng = acos(adCosRoll)*180/_PI_ + 360;
       }
       else
       {
         RollAng = acos(adCosRoll)*180/_PI_ + 180;
       }
     }
     
      if (fSinPitch >0)
     {
       if (adCosPitch>0)
       {
            PitchAng = acos(adCosPitch)*180/_PI_;
       }
       else
       {
          PitchAng = acos(adCosPitch)*180/_PI_ + 180;
       }
     }
     else
     {
       if (adCosPitch>0)
       {
            PitchAng = acos(adCosPitch)*180/_PI_ + 360;
       }
       else
       {
          PitchAng = acos(adCosPitch)*180/_PI_ + 180;
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
      
      HeadingValue = (float) ((atan2f((float)fTiltedY,(float)fTiltedX))*180)/_PI_;
			
      if (HeadingValue < 0)
      {
        HeadingValue = HeadingValue + 360;    
      }
      
      if ((RollAng <= 40.0f) && (PitchAng <= 40.0f))
      {
        if (((HeadingValue < 25.0f)&&(HeadingValue >= 0.0f))||((HeadingValue >=340.0f)&&(HeadingValue <= 360.0f)))
        {
					LEDx_Blink(LED10);
        }
        else  if ((HeadingValue <70.0f)&&(HeadingValue >= 25.0f))
        {
					LEDx_Blink(LED9);
        } 
        else  if ((HeadingValue < 115.0f)&&(HeadingValue >= 70.0f))
        {
					LEDx_Blink(LED7);
        }
        else  if ((HeadingValue <160.0f)&&(HeadingValue >= 115.0f))
        {
					LEDx_Blink(LED5);
        } 
        else  if ((HeadingValue <205.0f)&&(HeadingValue >= 160.0f))
        {
					LEDx_Blink(LED3);
        } 
        else  if ((HeadingValue <250.0f)&&(HeadingValue >= 205.0f))
        {
					LEDx_Blink(LED4);
        } 
        else  if ((HeadingValue < 295.0f)&&(HeadingValue >= 250.0f))
        {
					LEDx_Blink(LED6);
        }        
        else  if ((HeadingValue < 340.0f)&&(HeadingValue >= 295.0f))
        {
					LEDx_Blink(LED8);
        }
      }
      else
      {
        /* Delay 50ms */
        Delay_us(5);
      }
}

void LED_Error_Handler()
{
	LED_CatchSignal(Red, LED_ON);
}
