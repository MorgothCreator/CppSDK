/*
 * core_init.c
 *
 * Created: 2/8/2013 12:30:34 AM
 *  Author: XxXx
 */

#include <stdbool.h>
#include <string.h>
#include "core_init.h"
#include "system_stm32f4xx.h"
#include "sys/system_stm32f4xx.h"
#include "driver/stm32f4xx_hal.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_rcc_ex.h"
#include "driver/stm32f4xx_hal_pwr_ex.h"
#include "driver/stm32f4xx_hal_flash_ex.h"
//#include "driver/core.h"

unsigned long FCPU = CORE_CLOCK_DEFAULT;
unsigned long PLL_REF_CLK = HSE_VALUE;
#if (defined(STM32F469xx) || defined(STM32F479xx)) && defined(STM32F469I_DISCO)
#include "driver/stm32469i_discovery_sdram.h"
sdRam_Stm32469IDisco *sdram;
#endif

  /**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE or HSI)
  *            SYSCLK(Hz)                     = XPLL
  *            HCLK(Hz)                       = XPLL
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = XHSI
  *            PLL_M                          = 8
  *            PLL_N                          = XPLL * 2
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(unsigned long int_osc_freq, unsigned long ext_osc_freq, unsigned long core_freq)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  memset(&RCC_ClkInitStruct, 0, sizeof(RCC_ClkInitTypeDef));
  RCC_OscInitTypeDef RCC_OscInitStruct;
  memset(&RCC_OscInitStruct, 0, sizeof(RCC_OscInitTypeDef));
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
	if(ext_osc_freq == 0)
	{
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
		RCC_OscInitStruct.HSIState = RCC_HSI_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
		RCC_OscInitStruct.PLL.PLLM = int_osc_freq /1000000;
	}
	else
	{
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		RCC_OscInitStruct.HSEState = RCC_HSE_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if defined(USE_STM32469I_DISCO_REVA)
		RCC_OscInitStruct.PLL.PLLM = 25;
#else
		RCC_OscInitStruct.PLL.PLLM = ext_osc_freq /1000000;
#endif /* USE_STM32469I_DISCO_REVA */
	}
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLN = (core_freq * 2) / 1000000;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
#if defined(STM32F469xx) || defined(STM32F479xx)
  RCC_OscInitStruct.PLL.PLLR = 2;
#endif

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

#if defined(STM32F469xx) || defined(STM32F479xx)
  /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
#endif

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  PLL_REF_CLK =	RCC_OscInitStruct.PLL.PLLM * 1000000;
#if (defined(STM32F469xx) || defined(STM32F479xx)) && defined(STM32F469I_DISCO)
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  memset(&PeriphClkInitStruct, 0, sizeof(RCC_PeriphCLKInitTypeDef));
  PeriphClkInitStruct.PeriphClockSelection = PLL_REF_CLK / 1000000;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 360;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 7;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  //PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CK48;
  //PeriphClkInitStruct.Clk48ClockSelection = RCC_CK48CLKSOURCE_PLLSAIP;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
#elif defined(STM32F429xx) || defined(STM32F439xx)|| defined(STM32F469xx) || defined(STM32F479xx)
  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/4 = 48 Mhz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_8 = 48/4 = 6Mhz */
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

#endif
}

GI::Sys::Clock::Clock()
{
	setClk(FCPU);
}

GI::Sys::Clock::~Clock()
{
	SystemClock_Config(HSI_VALUE, 0, 0);
}

void GI::Sys::Clock::setClk(unsigned long fCpu)
{
	//SystemInit();
	//SystemCoreClockUpdate();
	SystemClock_Config(HSI_VALUE, HSE_VALUE, FCPU);
	HAL_Init();
#if (defined(STM32F469xx) || defined(STM32F479xx)) && defined(STM32F469I_DISCO)
	sdram = new sdRam_Stm32469IDisco();
#endif
	//SystemCoreClockUpdate();
	//RCC_PCLK1Config(RCC_HCLK_Div4);
	//RCC_PCLK2Config(RCC_HCLK_Div2);
#ifdef __HAL_RCC_GPIOA_CLK_ENABLE
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOB_CLK_ENABLE
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOC_CLK_ENABLE
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOD_CLK_ENABLE
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOE_CLK_ENABLE
	__HAL_RCC_GPIOE_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOF_CLK_ENABLE
	__HAL_RCC_GPIOF_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOG_CLK_ENABLE
	__HAL_RCC_GPIOG_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOH_CLK_ENABLE
	__HAL_RCC_GPIOH_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOI_CLK_ENABLE
	__HAL_RCC_GPIOI_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOJ_CLK_ENABLE
	__HAL_RCC_GPIOJ_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOK_CLK_ENABLE
	__HAL_RCC_GPIOK_CLK_ENABLE()
	;
#endif
#ifdef __HAL_RCC_GPIOL_CLK_ENABLE
	__HAL_RCC_GPIOK_CLK_ENABLE();
#endif
}
