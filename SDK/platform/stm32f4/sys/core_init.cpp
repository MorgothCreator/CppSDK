/*
 * core_init.c
 *
 * Created: 2/8/2013 12:30:34 AM
 *  Author: XxXx
 */

#include <stdbool.h>
#include <string.h>
#include <board.h>
#include "core_init.h"
#include "system_stm32f4xx.h"
#include "sys/system_stm32f4xx.h"
#include "driver/stm32f4xx_hal.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_rcc_ex.h"
#include "driver/stm32f4xx_hal_pwr_ex.h"
#include "driver/stm32f4xx_hal_flash_ex.h"
#include "driver/stm32f4xx_hal_rtc.h"
#include "driver/stm32f4xx_hal_rtc_ex.h"
//#include "driver/core.h"

unsigned long FCPU = CORE_CLOCK_DEFAULT;
unsigned long PLL_REF_CLK = HSE_VALUE;
#if (defined(STM32F469xx) || defined(STM32F479xx)) && defined(STM32F469I_DISCO) && (USE_EXT_RAM == 1)
#include "driver/stm32469i_discovery_sdram.h"
sdRam_Stm32469IDisco *sdram;
#elif defined(STM32F429xx) && defined(STM32F429I_DISCO) && (USE_EXT_RAM == 1)
#include "driver/stm32f429i_discovery_sdram.h"
sdRam_Stm32429IDisco *sdram;
#endif

#define RTC_ASYNCH_PREDIV    0x7F
#define RTC_SYNCH_PREDIV     0x0130

RCC_ClkInitTypeDef RCC_ClkInitStruct;
RCC_OscInitTypeDef RCC_OscInitStruct;
RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
RTC_HandleTypeDef RTCHandle;

unsigned long actualFcpu;

void setVoltageScale(unsigned long core_freq)
{
#if defined(PWR_REGULATOR_VOLTAGE_SCALE3)
	HAL_StatusTypeDef ret = HAL_OK;
	if(core_freq > 168000000)
	{
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
		ret = HAL_PWREx_EnableOverDrive();
		if(ret != HAL_OK)
		{
			while(1)
			{	;}
		}
	}
	else if(core_freq > 144000000)
	{
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
		ret = HAL_PWREx_DisableOverDrive();
		if(ret != HAL_OK)
		{
			while(1)
			{	;}
		}
	}
	else if(core_freq > 120000000)
	{
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
		ret = HAL_PWREx_DisableOverDrive();
		if(ret != HAL_OK)
		{
			while(1)
			{	;}
		}
	}
	else
	{
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
		ret = HAL_PWREx_DisableOverDrive();
		if(ret != HAL_OK)
		{
			while(1)
			{	;}
		}
	}
#else
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F446xx) ||\
    defined(STM32F469xx) || defined(STM32F479xx)
	HAL_StatusTypeDef ret = HAL_OK;
#endif
	if(core_freq > 144000000)
		{
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F446xx) ||\
    defined(STM32F469xx) || defined(STM32F479xx)
			ret = HAL_PWREx_DisableOverDrive();
			if(ret != HAL_OK)
			{
				while(1)
				{	;}
			}
#endif
		}
		else
		{
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F446xx) ||\
    defined(STM32F469xx) || defined(STM32F479xx)
			ret = HAL_PWREx_DisableOverDrive();
			if(ret != HAL_OK)
			{
				while(1)
				{	;}
			}
#endif
		}
#endif
}

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
static void SystemClock_Config(unsigned long int_osc_freq,
		unsigned long ext_osc_freq, unsigned long core_freq)
{
	HAL_StatusTypeDef ret = HAL_OK;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE()
	;


	/* Enable HSE Oscillator and activate PLL with HSE as source */
	if (ext_osc_freq == 0)
	{
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
		RCC_OscInitStruct.HSIState = RCC_HSI_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
		RCC_OscInitStruct.PLL.PLLM = int_osc_freq / 1000000;
	}
	else
	{
		RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
		RCC_OscInitStruct.HSEState = RCC_HSE_ON;
		RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if defined(USE_STM32469I_DISCO_REVA)
		RCC_OscInitStruct.PLL.PLLM = 25;
#else
		RCC_OscInitStruct.PLL.PLLM = ext_osc_freq / 1000000;
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
	if (ret != HAL_OK)
	{
		while (1)
		{
			;
		}
	}

#if defined(STM32F469xx) || defined(STM32F479xx)
	/* Activate the OverDrive to reach the 180 MHz Frequency */
	ret = HAL_PWREx_EnableOverDrive();
	if(ret != HAL_OK)
	{
		while(1)
		{	;}
	}
#endif

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	if(core_freq > actualFcpu)
	{
		setVoltageScale(core_freq);
	}
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, core_freq / 36000000);
	if (ret != HAL_OK)
	{
		while (1)
		{
			;
		}
	}
	PLL_REF_CLK = RCC_OscInitStruct.PLL.PLLM * 1000000;
#if (defined(STM32F469xx) || defined(STM32F479xx)) && defined(STM32F469I_DISCO)
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
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
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_16;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

#endif
	if(core_freq < actualFcpu)
	{
		setVoltageScale(core_freq);
	}
	actualFcpu = core_freq;
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
	memset(&RCC_ClkInitStruct, 0, sizeof(RCC_ClkInitTypeDef));
	memset(&RCC_OscInitStruct, 0, sizeof(RCC_OscInitTypeDef));
	memset(&PeriphClkInitStruct, 0, sizeof(RCC_PeriphCLKInitTypeDef));
	HAL_Init();
	SystemClock_Config(HSI_VALUE, HSE_VALUE, fCpu);
#if (defined(STM32F469xx) || defined(STM32F479xx)) && defined(STM32F469I_DISCO) && (USE_EXT_RAM == 1)
	sdram = new sdRam_Stm32469IDisco();
#elif defined(STM32F429xx) && defined(STM32F429I_DISCO) && (USE_EXT_RAM == 1)
	sdram = new sdRam_Stm32429IDisco();
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

void GI::Sys::Clock::changeCoreClk(unsigned long fCpu)
{
	/* -1- Select HSI as system clock source to allow modification of the PLL configuration */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		while (1)
		{
			;
		}
	}

	HAL_StatusTypeDef ret = HAL_OK;
#if defined(STM32F469xx) || defined(STM32F479xx)
	RCC_OscInitStruct.PLL.PLLR = 2;
#endif
	/* -2- Enable HSI Oscillator, select it as PLL source and finally activate the PLL */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	if(fCpu > 16000000)
	{
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	}
	else
	{
		RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
	}
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = (fCpu * 2) / 1000000;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	RCC_OscInitStruct.HSICalibrationValue = 0x10;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		while (1)
		{
			;
		}
	}

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F446xx) ||\
    defined(STM32F469xx) || defined(STM32F479xx)
	if (fCpu > 180000000)
	{
		if (HAL_PWREx_EnableOverDrive() != HAL_OK)
		{
			/* Initialization Error */
			while (1)
			{
				;
			}
		}
	}
	else
	{
		if (HAL_PWREx_DisableOverDrive() != HAL_OK)
		{
			/* Initialization Error */
			while (1)
			{
				;
			}
		}
	}
#endif
	if(fCpu > 16000000)
	{
		/* The voltage scaling allows optimizing the power consumption when the device is
		 clocked below the maximum system frequency, to update the voltage scaling value
		 regarding system frequency refer to product datasheet.  */
		if(fCpu > actualFcpu)
		{
			setVoltageScale(fCpu);
		}
		/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
		RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
		RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
		RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

		ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, fCpu / 36000000);
		if (ret != HAL_OK)
		{
			while (1)
			{
				;
			}
		}
		PLL_REF_CLK = RCC_OscInitStruct.PLL.PLLM * 1000000;
	}
	if(fCpu < actualFcpu)
	{
		setVoltageScale(fCpu);
	}
	actualFcpu = fCpu;
}



void GI::Sys::Clock::sleep()
{
	  /* Request to enter SLEEP mode */
	  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}



#ifdef __cplusplus
extern "C"
{
#endif
/**
  * @brief  This function handles RTC Auto wake-up interrupt request.
  * @param  None
  * @retval None
  */
void RTC_WKUP_IRQHandler(void)
{
  HAL_RTCEx_WakeUpTimerIRQHandler(&RTCHandle);
}
#ifdef __cplusplus
}
#endif
