#include "stm32f3xx_hal_def.h"

/** @defgroup RCC_AHB_Clock_Enable_Disable RCC AHB Clock Enable Disable
  * @brief  Enable or disable the AHB peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{  
  */
#define __GPIOA_CLK_ENABLE()         (RCC->AHBENR |= (RCC_AHBENR_GPIOAEN))
#define __GPIOB_CLK_ENABLE()         (RCC->AHBENR |= (RCC_AHBENR_GPIOBEN))
#define __GPIOC_CLK_ENABLE()         (RCC->AHBENR |= (RCC_AHBENR_GPIOCEN))
#define __GPIOD_CLK_ENABLE()         (RCC->AHBENR |= (RCC_AHBENR_GPIODEN))
#define __GPIOF_CLK_ENABLE()         (RCC->AHBENR |= (RCC_AHBENR_GPIOFEN))
#define __CRC_CLK_ENABLE()           (RCC->AHBENR |= (RCC_AHBENR_CRCEN))
#define __DMA1_CLK_ENABLE()          (RCC->AHBENR |= (RCC_AHBENR_DMA1EN))
#define __SRAM_CLK_ENABLE()          (RCC->AHBENR |= (RCC_AHBENR_SRAMEN))
#define __FLITF_CLK_ENABLE()         (RCC->AHBENR |= (RCC_AHBENR_FLITFEN))
#define __TSC_CLK_ENABLE()           (RCC->AHBENR |= (RCC_AHBENR_TSCEN))

#define __GPIOA_CLK_DISABLE()        (RCC->AHBENR &= ~(RCC_AHBENR_GPIOAEN))
#define __GPIOB_CLK_DISABLE()        (RCC->AHBENR &= ~(RCC_AHBENR_GPIOBEN))
#define __GPIOC_CLK_DISABLE()        (RCC->AHBENR &= ~(RCC_AHBENR_GPIOCEN))
#define __GPIOD_CLK_DISABLE()        (RCC->AHBENR &= ~(RCC_AHBENR_GPIODEN))
#define __GPIOF_CLK_DISABLE()        (RCC->AHBENR &= ~(RCC_AHBENR_GPIOFEN))
#define __CRC_CLK_DISABLE()          (RCC->AHBENR &= ~(RCC_AHBENR_CRCEN))
#define __DMA1_CLK_DISABLE()         (RCC->AHBENR &= ~(RCC_AHBENR_DMA1EN))
#define __SRAM_CLK_DISABLE()         (RCC->AHBENR &= ~(RCC_AHBENR_SRAMEN))
#define __FLITF_CLK_DISABLE()        (RCC->AHBENR &= ~(RCC_AHBENR_FLITFEN))
#define __TSC_CLK_DISABLE()          (RCC->AHBENR &= ~(RCC_AHBENR_TSCEN))
/**
  * @}
  */
/** @defgroup RCCEx_AHB_Clock_Enable_Disable RCC Extended AHB Clock Enable Disable
  * @brief  Enable or disable the AHB peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{   
  */

#define __DMA2_CLK_ENABLE()          (RCC->AHBENR |= (RCC_AHBENR_DMA2EN))
#define __GPIOE_CLK_ENABLE()         (RCC->AHBENR |= (RCC_AHBENR_GPIOEEN))
#define __ADC12_CLK_ENABLE()         (RCC->AHBENR |= (RCC_AHBENR_ADC12EN))
/* Aliases for STM32 F3 compatibility */
#define __ADC1_CLK_ENABLE()          __ADC12_CLK_ENABLE()
#define __ADC2_CLK_ENABLE()          __ADC12_CLK_ENABLE()

#define __DMA2_CLK_DISABLE()         (RCC->AHBENR &= ~(RCC_AHBENR_DMA2EN))
#define __GPIOE_CLK_DISABLE()        (RCC->AHBENR &= ~(RCC_AHBENR_GPIOEEN))
#define __ADC12_CLK_DISABLE()        (RCC->AHBENR &= ~(RCC_AHBENR_ADC12EN))
/* Aliases for STM32 F3 compatibility */
#define __ADC1_CLK_DISABLE()          __ADC12_CLK_DISABLE()
#define __ADC2_CLK_DISABLE()          __ADC12_CLK_DISABLE()

#define __ADC34_CLK_ENABLE()         (RCC->AHBENR |= (RCC_AHBENR_ADC34EN))

#define __ADC34_CLK_DISABLE()        (RCC->AHBENR &= ~(RCC_AHBENR_ADC34EN))

/**
  * @}
  */
	
/**
  * @brief  RCC System, AHB and APB busses clock configuration structure definition
  */
typedef struct
{
  uint32_t ClockType;            /*!< The clock to be configured.
                                      This parameter can be a value of @ref RCC_System_Clock_Type */

  uint32_t SYSCLKSource;         /*!< The clock source (SYSCLKS) used as system clock.
                                      This parameter can be a value of @ref RCC_System_Clock_Source */

  uint32_t AHBCLKDivider;        /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                      This parameter can be a value of @ref RCC_AHB_Clock_Source */

  uint32_t APB1CLKDivider;       /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                      This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

  uint32_t APB2CLKDivider;       /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK).
                                      This parameter can be a value of @ref RCC_APB1_APB2_Clock_Source */

}RCC_ClkInitTypeDef;

/**
  * @brief  RCC PLL configuration structure definition
  */
typedef struct
{
  uint32_t PLLState;   /*!< PLLState: The new state of the PLL.
                            This parameter can be a value of @ref RCC_PLL_Config */

  uint32_t PLLSource;  /*!< PLLSource: PLL entry clock source.
                            This parameter must be a value of @ref RCCEx_PLL_Clock_Source */

  uint32_t PLLMUL;     /*!< PLLMUL: Multiplication factor for PLL VCO input clock
                            This parameter must be a value of @ref RCC_PLL_Multiplication_Factor*/

}RCC_PLLInitTypeDef;

/**
  * @brief  RCC Internal/External Oscillator (HSE, HSI, LSE and LSI) configuration structure definition
  */
typedef struct
{
  uint32_t OscillatorType;       /*!< The oscillators to be configured.
                                      This parameter can be a value of @ref RCC_Oscillator_Type */

  uint32_t HSEState;             /*!< The new state of the HSE.
                                      This parameter can be a value of @ref RCC_HSE_Config */

  uint32_t HSEPredivValue;       /*!<  The HSE predivision factor value.
                                       This parameter can be a value of @ref RCCEx_HSE_Predivision_Factor */

  uint32_t LSEState;             /*!<  The new state of the LSE.
                                       This parameter can be a value of @ref RCC_LSE_Config */

  uint32_t HSIState;             /*!< The new state of the HSI.
                                      This parameter can be a value of @ref RCC_HSI_Config */

  uint32_t HSICalibrationValue;  /*!< The calibration trimming value.
                                      This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F */

  uint32_t LSIState;             /*!<  The new state of the LSI.
                                       This parameter can be a value of @ref RCC_LSI_Config */

  RCC_PLLInitTypeDef PLL;        /*!< PLL structure parameters */

}RCC_OscInitTypeDef;

/** @defgroup RCC_Oscillator_Type RCC Oscillator Type
  * @{
  */
#define RCC_OSCILLATORTYPE_NONE            ((uint32_t)0x00000000)
#define RCC_OSCILLATORTYPE_HSE             ((uint32_t)0x00000001)
#define RCC_OSCILLATORTYPE_HSI             ((uint32_t)0x00000002)
#define RCC_OSCILLATORTYPE_LSE             ((uint32_t)0x00000004)
#define RCC_OSCILLATORTYPE_LSI             ((uint32_t)0x00000008)

#define IS_RCC_OSCILLATORTYPE(OSCILLATOR) (((OSCILLATOR) == RCC_OSCILLATORTYPE_NONE)                           || \
                                           (((OSCILLATOR) & RCC_OSCILLATORTYPE_HSE) == RCC_OSCILLATORTYPE_HSE) || \
                                           (((OSCILLATOR) & RCC_OSCILLATORTYPE_HSI) == RCC_OSCILLATORTYPE_HSI) || \
                                           (((OSCILLATOR) & RCC_OSCILLATORTYPE_LSI) == RCC_OSCILLATORTYPE_LSI) || \
                                           (((OSCILLATOR) & RCC_OSCILLATORTYPE_LSE) == RCC_OSCILLATORTYPE_LSE))
/**
  * @}
  */
/** @defgroup RCCEx_PLL_Clock_Source RCC Extended PLL Clock Source
  * @{
  */
#define RCC_PLLSOURCE_HSI                RCC_CFGR_PLLSRC_HSI_DIV2
#define RCC_PLLSOURCE_HSE                RCC_CFGR_PLLSRC_HSE_PREDIV

#define IS_RCC_PLLSOURCE(SOURCE) (((SOURCE) == RCC_PLLSOURCE_HSI) || \
                                  ((SOURCE) == RCC_PLLSOURCE_HSE))
/**
  * @}
  */

/** @defgroup RCCEx_HSE_Predivision_Factor RCC Extended HSE Predivision Factor
  * @{
  */

#define RCC_HSE_PREDIV_DIV1              RCC_CFGR2_PREDIV_DIV1
#define RCC_HSE_PREDIV_DIV2              RCC_CFGR2_PREDIV_DIV2
#define RCC_HSE_PREDIV_DIV3              RCC_CFGR2_PREDIV_DIV3
#define RCC_HSE_PREDIV_DIV4              RCC_CFGR2_PREDIV_DIV4
#define RCC_HSE_PREDIV_DIV5              RCC_CFGR2_PREDIV_DIV5
#define RCC_HSE_PREDIV_DIV6              RCC_CFGR2_PREDIV_DIV6
#define RCC_HSE_PREDIV_DIV7              RCC_CFGR2_PREDIV_DIV7
#define RCC_HSE_PREDIV_DIV8              RCC_CFGR2_PREDIV_DIV8
#define RCC_HSE_PREDIV_DIV9              RCC_CFGR2_PREDIV_DIV9
#define RCC_HSE_PREDIV_DIV10             RCC_CFGR2_PREDIV_DIV10
#define RCC_HSE_PREDIV_DIV11             RCC_CFGR2_PREDIV_DIV11
#define RCC_HSE_PREDIV_DIV12             RCC_CFGR2_PREDIV_DIV12
#define RCC_HSE_PREDIV_DIV13             RCC_CFGR2_PREDIV_DIV13
#define RCC_HSE_PREDIV_DIV14             RCC_CFGR2_PREDIV_DIV14
#define RCC_HSE_PREDIV_DIV15             RCC_CFGR2_PREDIV_DIV15
#define RCC_HSE_PREDIV_DIV16             RCC_CFGR2_PREDIV_DIV16

#define IS_RCC_HSE_PREDIV(DIV) (((DIV) == RCC_HSE_PREDIV_DIV1)  || ((DIV) == RCC_HSE_PREDIV_DIV2)  || \
                                ((DIV) == RCC_HSE_PREDIV_DIV3)  || ((DIV) == RCC_HSE_PREDIV_DIV4)  || \
                                ((DIV) == RCC_HSE_PREDIV_DIV5)  || ((DIV) == RCC_HSE_PREDIV_DIV6)  || \
                                ((DIV) == RCC_HSE_PREDIV_DIV7)  || ((DIV) == RCC_HSE_PREDIV_DIV8)  || \
                                ((DIV) == RCC_HSE_PREDIV_DIV9)  || ((DIV) == RCC_HSE_PREDIV_DIV10) || \
                                ((DIV) == RCC_HSE_PREDIV_DIV11) || ((DIV) == RCC_HSE_PREDIV_DIV12) || \
                                ((DIV) == RCC_HSE_PREDIV_DIV13) || ((DIV) == RCC_HSE_PREDIV_DIV14) || \
                                ((DIV) == RCC_HSE_PREDIV_DIV15) || ((DIV) == RCC_HSE_PREDIV_DIV16))
/**
  * @}
  */
/** @defgroup FLASH_Latency FLASH Latency
	* @{
	*/ 
#define FLASH_LATENCY_0            ((uint8_t)0x0000)    /*!< FLASH Zero Latency cycle */
#define FLASH_LATENCY_1            FLASH_ACR_LATENCY_0  /*!< FLASH One Latency cycle */
#define FLASH_LATENCY_2            FLASH_ACR_LATENCY_1  /*!< FLASH Two Latency cycles */

/** @defgroup RCC_System_Clock_Type RCC System Clock Type
  * @{
  */
#define RCC_CLOCKTYPE_SYSCLK             ((uint32_t)0x00000001)
#define RCC_CLOCKTYPE_HCLK               ((uint32_t)0x00000002)
#define RCC_CLOCKTYPE_PCLK1              ((uint32_t)0x00000004)
#define RCC_CLOCKTYPE_PCLK2              ((uint32_t)0x00000008)

#define IS_RCC_CLOCKTYPE(CLK) ((((CLK) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK) || \
                               (((CLK) & RCC_CLOCKTYPE_HCLK)   == RCC_CLOCKTYPE_HCLK)   || \
                               (((CLK) & RCC_CLOCKTYPE_PCLK1)  == RCC_CLOCKTYPE_PCLK1)  || \
                               (((CLK) & RCC_CLOCKTYPE_PCLK2)  == RCC_CLOCKTYPE_PCLK2))
/**
  * @}
  */

/** @defgroup RCC_System_Clock_Source RCC System Clock Source
  * @{
  */
#define RCC_SYSCLKSOURCE_HSI             RCC_CFGR_SW_HSI
#define RCC_SYSCLKSOURCE_HSE             RCC_CFGR_SW_HSE
#define RCC_SYSCLKSOURCE_PLLCLK          RCC_CFGR_SW_PLL

#define IS_RCC_SYSCLKSOURCE(SOURCE)  (((SOURCE) == RCC_SYSCLKSOURCE_HSI) || \
                                      ((SOURCE) == RCC_SYSCLKSOURCE_HSE) || \
                                      ((SOURCE) == RCC_SYSCLKSOURCE_PLLCLK))
/**
  * @}
  */
	

/** @defgroup RCC_AHB_Clock_Source RCC AHB Clock Source
  * @{
  */
#define RCC_SYSCLK_DIV1                  RCC_CFGR_HPRE_DIV1
#define RCC_SYSCLK_DIV2                  RCC_CFGR_HPRE_DIV2
#define RCC_SYSCLK_DIV4                  RCC_CFGR_HPRE_DIV4
#define RCC_SYSCLK_DIV8                  RCC_CFGR_HPRE_DIV8
#define RCC_SYSCLK_DIV16                 RCC_CFGR_HPRE_DIV16
#define RCC_SYSCLK_DIV64                 RCC_CFGR_HPRE_DIV64
#define RCC_SYSCLK_DIV128                RCC_CFGR_HPRE_DIV128
#define RCC_SYSCLK_DIV256                RCC_CFGR_HPRE_DIV256
#define RCC_SYSCLK_DIV512                RCC_CFGR_HPRE_DIV512

#define IS_RCC_SYSCLK_DIV(DIV) (((DIV) == RCC_SYSCLK_DIV1)   || ((DIV) == RCC_SYSCLK_DIV2) || \
                                ((DIV) == RCC_SYSCLK_DIV4)   || ((DIV) == RCC_SYSCLK_DIV8) || \
                                ((DIV) == RCC_SYSCLK_DIV16)  || ((DIV) == RCC_SYSCLK_DIV64) || \
                                ((DIV) == RCC_SYSCLK_DIV128) || ((DIV) == RCC_SYSCLK_DIV256) || \
                                ((DIV) == RCC_SYSCLK_DIV512))
/**
  * @}
  */

/** @defgroup RCC_APB1_APB2_Clock_Source RCC APB1 APB2 Clock Source
  * @{
  */
#define RCC_HCLK_DIV1                    RCC_CFGR_PPRE1_DIV1
#define RCC_HCLK_DIV2                    RCC_CFGR_PPRE1_DIV2
#define RCC_HCLK_DIV4                    RCC_CFGR_PPRE1_DIV4
#define RCC_HCLK_DIV8                    RCC_CFGR_PPRE1_DIV8
#define RCC_HCLK_DIV16                   RCC_CFGR_PPRE1_DIV16

#define IS_RCC_HCLK_DIV(DIV) (((DIV) == RCC_HCLK_DIV1) || ((DIV) == RCC_HCLK_DIV2) || \
                              ((DIV) == RCC_HCLK_DIV4) || ((DIV) == RCC_HCLK_DIV8) || \
                              ((DIV) == RCC_HCLK_DIV16))
/**
  * @}
  */


/** @defgroup RCC_HSI_Config RCC HSI Config
  * @{
  */
#define RCC_HSI_OFF                      ((uint32_t)0x00000000)
#define RCC_HSI_ON                       ((uint32_t)0x00000001)

#define IS_RCC_HSI(HSI) (((HSI) == RCC_HSI_OFF) || ((HSI) == RCC_HSI_ON))

#define RCC_HSICALIBRATION_DEFAULT ((uint32_t)0x10) /* Default HSI calibration trimming value */

#define IS_RCC_CALIBRATION_VALUE(__VALUE__) ((__VALUE__) <= 0x1F)
/**
  * @}
  */

/** @defgroup RCC_LSI_Config RCC LSI Config
  * @{
  */
#define RCC_LSI_OFF                      ((uint32_t)0x00000000)
#define RCC_LSI_ON                       ((uint32_t)0x00000001)

#define IS_RCC_LSI(LSI) (((LSI) == RCC_LSI_OFF) || ((LSI) == RCC_LSI_ON))
/**
  * @}
  */

/** @defgroup RCC_PLL_Config RCC PLL Config
  * @{
  */
#define RCC_PLL_NONE                     ((uint32_t)0x00000000)
#define RCC_PLL_OFF                      ((uint32_t)0x00000001)
#define RCC_PLL_ON                       ((uint32_t)0x00000002)

#define IS_RCC_PLL(PLL) (((PLL) == RCC_PLL_NONE) ||((PLL) == RCC_PLL_OFF) || ((PLL) == RCC_PLL_ON))
/**
  * @}
  */
