#include "signal_transmit.h"

/* UART handler declaration */
USART_InitTypeDef UartHandle;
__IO ITStatus UartReady = RESET;

/* Buffer used for transmission */
uint8_t aTxBuffer[] = " ############ ";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];

 /* STM32F3xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
			 */
void com_DMA()
{

  HAL_Init();
	
	  /* Configure the system clock to 168 Mhz */
  SystemClock_Config();
	  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */

  UartHandle.USART_BaudRate   = 38400;
  UartHandle.USART_WordLength = USART_WordLength_8b;
  UartHandle.USART_StopBits   = USART_StopBits_1;
  UartHandle.USART_Parity     = USART_Parity_No;
  UartHandle.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
  UartHandle.USART_Mode       = USART_Mode_Rx|USART_Mode_Tx;
  
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  } 
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
#ifdef TRANSMITTER_BOARD

  /* Configure User push-button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
  
  /* Wait for User push-button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) == RESET)
  {
  }
  
  /* The board sends the message and expects to receive it back */
  
  /*##-2- Start the transmission process #####################################*/  
  /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
  {
    Error_Handler();
  }
  
  /*##-3- Wait for the end of the transfer ###################################*/  
  while (UartReady != SET)
  {
  }
  
  /* Reset transmission flag */
  UartReady = RESET;
  
  /*##-4- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  } 
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  if(HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }

#else
  
  /* The board receives the message and sends it back */

  /*##-2- Put UART peripheral in reception process ###########################*/  
  if(HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
  {
    Error_Handler();
  }
  
  /*##-3- Wait for the end of the transfer ###################################*/  
  while (UartReady != SET)
  {
  }

  /* Reset transmission flag */
  UartReady = RESET;
  
  /*##-4- Start the transmission process #####################################*/  
  /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  } 
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }     
  if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE)!= HAL_OK)
  {
    Error_Handler();
  }
  
#endif /* TRANSMITTER_BOARD */
  
  /*##-5- Wait for the end of the transfer ###################################*/  
  while (UartReady != SET)
  {
  }

  /* Reset transmission flag */
  UartReady = RESET;

  /*##-6- Compare the sent and received buffers ##############################*/
  if(Buffercmp((uint8_t*)aTxBuffer,(uint8_t*)aRxBuffer,RXBUFFERSIZE))
  {
    Error_Handler();
  }
   
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV                     = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
#ifdef USE_FULL_ASSERT
  uint32_t signal = HAL_OK;
#endif /* USE_FULL_ASSERT */
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  
#ifdef USE_FULL_ASSERT
  signal = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(signal != HAL_OK)
  {
    assert_failed((uint8_t *)__FILE__, __LINE__);
  }
#else
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
#endif /* USE_FULL_ASSERT */
    	
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

#ifdef USE_FULL_ASSERT
  signal = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
  if(signal != HAL_OK)
  {
    assert_failed((uint8_t *)__FILE__, __LINE__);
  }
#else
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
#endif /* USE_FULL_ASSERT */
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  To set and config blutooth then send data to the register.
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
			Handle_SignalLamp(2,1); 
			while (!DMA_GetFlagStatus(USART_FLAG_TC)){};
}

void DMA_Re(DMA_Channel_TypeDef* DMA_CHx,u8 len)
{
	DMA_CHx->CCR&=~(1<<0);    //close DMA
	DMA_CHx->CNDTR=len;       //DMA1, quantity you sent. 
	DMA_CHx->CCR|=1<<0;       //ensure DMA
}	
