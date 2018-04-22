#include "HC05_set.h"

u8  TX_BUF[256];
u8  RX_BUF[256];

u16 RX_STA = 0;

/*GPIOA, USART2, DMA1, Channel7, PA4-TX, PA5-RX*/

#if 1
#pragma import(__use_no_semihosting)             
//necssary functions standard library needed.
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef¡¯ d in stdio.h. */ 
FILE __stdout;       
//define function _sys_exit() to avert half-host mode.
void _sys_exit(int x) 
{ 
	x = x; 
} 
#endif

/**
  * @brief  serial port 2, Override function printf.
  * @param  fmt,...ensure every time data sent in length of 200.
  * @retval None
  */
void u2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)TX_BUF,fmt,ap);
	va_end(ap);
	while(DMA1_Channel7->CNDTR!=0);	//waiting if channel 7 finished transportion.   
	DMA_Config(DMA1_Channel7, (uint32_t)TX_BUF, strlen((const char*)TX_BUF)); 	//via DMA.
}

u8 HC05_Init(void)
{
	u8 retry=10,t;
	u8 temp=1;
	USART_InitTypeDef Init_USART2;
	RCC->APB2ENR |= 1<<4;    	//enable PORT clock 	
 	GPIOA->PUPDR &= 0xFF00FFFF;		/*????????????????*/
	GPIOA->PUPDR |= 0x00830000; 
	GPIOA->IDR |= 1<<5; 		//PA5 pull-up, output
	Init_USART2.USART_BaudRate = 38400;		//baud rate my hc-05 supported.
	Init_USART2.USART_WordLength = USART_WordLength_8b;
	Init_USART2.USART_StopBits = USART_StopBits_1;
	Init_USART2.USART_Parity = USART_Parity_No;
	Init_USART2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Init_USART2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &Init_USART2);
	USART_Cmd(USART2, ENABLE);			//USART2 Init 38400 baud rate.
	while(retry--)
	{
		HC05_KEY=1;					//set KEY high to enable AT mode.
		Delay(10);
		u2_printf("AT\r\n");		//send AT test command.
		HC05_KEY=0;					//pull down KEY pin to exit AT mode.
		for(t=0;t<10;t++) 			//wait 50ms at langest to recieve response form HC-05. 
		{
			if(RX_STA&0x8000)
			break;
			Delay(5);
		}		
		if(RX_STA&0x8000)	//has recived data for once.
		{
			temp=RX_STA&0x7FFF;	//get the lenth of the data recieved.
			RX_STA=0;			 
			if(temp==4&&RX_BUF[0]=='O'&&RX_BUF[1]=='K')
			{
				temp=0;//recieved OK.
				break;
			}
		}			    		
	}		    
	if(retry==0)temp=1;	//checkout failed
	return temp;	 
}	

/**
  * @brief  get role of ATK-HC05 module.
	* @param  none.
  * @retval 0, slave; 1, host; 0xFF failed.
  */				  
u8 HC05_Get_Role(void)
{	 		    
	u8 retry=0x0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//set KEY pin heigh, enable AT mode.
		Delay(10);
		u2_printf("AT+ROLE?\r\n");	//checkout the module role.
		for(t=0;t<20;t++) 			//wait for at longest 200ms, to get responses of HC05
		{
			Delay(10);
			if(RX_STA&0x8000)
			break;
		}		
		HC05_KEY=0;						//set KEY pin low, to quit AT mode.
		if(RX_STA&0x8000)	  	//if you recieved some numbers at onece.  
		{
			temp=RX_STA&0x7FFF;	//then get the length of these numbers.
			RX_STA=0;			 
			if(temp==13&&RX_BUF[0]=='+')//if you recieved the correct response.
			{
				temp=RX_BUF[6]-'0';//get values of host and slave mode.
				break;
			}
		}		
	}
	if(retry==0)temp=0xFF;//failed to chekout.
	return temp;
}

/**
  * @brief  set commands of HC-05, suit for and only for the AT cmd who respondes OK.
	* @param  atstr:AT cmds, such as "AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0".
  * @retval 0, success; others failed.
  */						  
u8 HC05_Set_Cmd(u8* atstr)
{	 		    
	u8 retry=0x0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;
		Delay(10);
		u2_printf("%s\r\n",atstr);
		HC05_KEY=0;
		for(t=0;t<20;t++)
		{
			if(RX_STA&0x8000)break;
			Delay(5);
		}		
		if(RX_STA&0x8000)
		{
			temp=RX_STA&0x7FFF;
			RX_STA=0;			 
			if(temp==4&&RX_BUF[0]=='O')
			{			
				temp=0;
				break;			 
			}
		}		
	}
	if(retry==0)temp=0xFF;
	return temp;
} 

/**
  * @brief  using USMART to debugging HC-05 connected with serial port 2.
	* @param  str, command string(<ENTER> is unnecessary).
  * @retval none
  */
void HC05_debug(u8 *str)
{					  
	u8 temp;
	u8 t;		  
	HC05_KEY=1;
	Delay(10);
	u2_printf("%s\r\n",(char*)str);
	for(t=0;t<50;t++)
	{
		if(RX_STA&0X8000)break;
		Delay(10);
	}									    
	HC05_KEY=0;
	if(RX_STA&0X8000)
	{
		temp=RX_STA&0x7FFF;
		RX_STA=0;
		RX_BUF[temp]=0;		//add ending flag.		 
		printf("\r\n%s",RX_BUF);//send response to serial port 1.
	} 				 
}

/**
  * @brief  To set and config bluetooth then send data to the register.
	* @param  rate, baudrate has setted on AT mode; addr,  the memory base address; leng, the length of data you want to send.
  * @retval None.
  */
void BT_Usart2_Config(uint32_t rate, uint32_t addr, uint8_t leng)
{
			USART_InitTypeDef USART_InitStructure;
			GPIO_InitTypeDef GPIO_InitStructure;
			DMA_InitTypeDef DMA_InitStructure;
	
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
			GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_2/*TX, PA2*/ | GPIO_Pin_3/*RX, PA3*/);
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);
	
			USART_InitStructure.USART_BaudRate = rate;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_DeInit(USART2);
			USART_Init(USART2, &USART_InitStructure);
			USART_Cmd(USART2, ENABLE);

			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
			DMA_StructInit(&DMA_InitStructure);
			DMA_DeInit(DMA1_Channel7); 
			DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)USART2 + 0x28;		//////
			DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&addr;
			DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
			DMA_InitStructure.DMA_BufferSize = leng;
			DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//address register is incremented or not
			DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord ;
			DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord ;
			DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
			DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
			DMA_Init(DMA1_Channel7,&DMA_InitStructure);

			USART_ClearFlag(USART2,USART_FLAG_TC);
			DMA_ClearFlag(DMA1_FLAG_TC7);
			USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
			DMA_Cmd(DMA1_Channel7,ENABLE);
			Lamp_CatchSignal(2,1); 
			while (!DMA_GetFlagStatus(USART_FLAG_TC)){};
}
