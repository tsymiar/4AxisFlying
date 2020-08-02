#include "PWM_set.h"

void PWM_Config(uint32_t OUT_Pin)
{
	/*PWM initiate*/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE); //TIM8 clock enable    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); //PORTA clock enable	
	
	GPIO_InitStructure.GPIO_Pin = OUT_Pin;              //GPIOA pins
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //Alternate function Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //Push-pull multiplexed output
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //Push-pull
	GPIO_Init(GPIOA,&GPIO_InitStructure);
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_2); //GPIOA3 multi-use TIM8 AF2

	TIM_TimeBaseStructure.TIM_Prescaler=255;           //clock divider
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //TIM_Counter_Mode
	TIM_TimeBaseStructure.TIM_Period=0;   		         //Auto-Reload Register
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //TIM pulse width modulation mode 2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //enable output compare
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //TIM output polarity higher
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //enable TIM8 output polarity on CCR2
  TIM_ARRPreloadConfig(TIM8,ENABLE);
	TIM_Cmd(TIM8, ENABLE);
}

void PWM_Set(int moto1, int moto2, int moto3, int moto4)
{
	/*output*/
	PWM_Config(GPIO_Pin_3);
	TIM_SetCompare2(TIM8,moto1);
	
	PWM_Config(GPIO_Pin_4);
	TIM_SetCompare2(TIM8,moto2);
	
	PWM_Config(GPIO_Pin_5);
	TIM_SetCompare2(TIM8,moto3);
	
	PWM_Config(GPIO_Pin_6);
	TIM_SetCompare2(TIM8,moto4);
}
