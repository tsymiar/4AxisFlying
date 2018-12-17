#include "PWM_sig.h"

void PWM_Set(int moto1, int moto2, int moto3, int moto4)
{
	PWM_Init(GPIO_Pin_3);
	/*output*/
	TIM_SetCompare2(TIM8,moto1);
	
	PWM_Init(GPIO_Pin_4);
	TIM_SetCompare2(TIM8,moto2);
	
	PWM_Init(GPIO_Pin_5);
	TIM_SetCompare2(TIM8,moto3);
	
	PWM_Init(GPIO_Pin_6);
	TIM_SetCompare2(TIM8,moto4);
}

void PWM_Init(uint32_t OUT_Pin)
{
	/*PWM initiate*/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8时钟使能    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); //使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = OUT_Pin; //GPIOA pins
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA3
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_2); //GPIOA3复用位定时器8 AF3

	TIM_TimeBaseStructure.TIM_Prescaler=255;  //*********定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=0;   		//*********自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//初始化定时器8

	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 OC2

	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
}
