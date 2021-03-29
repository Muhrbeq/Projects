/*
 * LowLevel.c
 *
 *  Created on: 25 nov. 2020
 *      Author: Rasmus.Muhrbeck
 */

#include <main.h>
#include <_LowLevel.h>

#include <lptim.h>
#include <adc.h>
#include <crc.h>
#include <fatfs.h>
#include <gpio.h>
#include <i2c.h>
#include <rtc.h>
#include <iwdg.h>
#include <sdmmc.h>
#include <spi.h>
#include <tim.h>
#include <usart.h>

/* File to handle all low level inits and config */

/* ##########################################
 * LED Drivers
 * ##########################################
 *  */

void DualLed_RedOn(void)
{
	HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_SET);
}

void DualLed_RedOff(void)
{
	HAL_GPIO_WritePin(LED_Red_GPIO_Port, LED_Red_Pin, GPIO_PIN_RESET);
}

void DualLed_GreenOn(void)
{
	HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_SET);
}

void DualLed_GreenOff(void)
{
	HAL_GPIO_WritePin(LED_Green_GPIO_Port, LED_Green_Pin, GPIO_PIN_RESET);
}

void DualLed_ToggleRed(void)
{
	HAL_GPIO_TogglePin(LED_Red_GPIO_Port, LED_Red_Pin);
}

void DualLed_ToggleGreen(void)
{
	HAL_GPIO_TogglePin(LED_Green_GPIO_Port, LED_Green_Pin);
}

void DualLed_BlinkGreen(uint8_t amount)
{
	for(uint8_t i = 0; i < amount; i++)
	{
		DualLed_ToggleGreen();
		HAL_Delay(100);
	}
}

void DualLed_BlinkRed(uint8_t amount)
{
	for(uint8_t i = 0; i < amount; i++)
	{
		DualLed_ToggleRed();
		HAL_Delay(100);
	}
}

/* ##########################################
 * LED Drivers ENDING
 * ##########################################
 *  */

/* ##########################################
 * RESET CAUSE
 * ##########################################
 *  */

/* Check what the cause of reset is */
uint8_t CheckResetCause(void)
{
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
	{
		return 1;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
	{
		return 2;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
	{
		//If IWDG resets, can be cause of fatal error
		return 3;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
	{
		return 4;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
	{
		return 5;
	}
	// Needs to come *after* checking the `RCC_FLAG_PORRST` flag in order to ensure first that the reset cause is
	// NOT a POR/PDR reset. See note below.
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
	{
		return 6;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_FWRST))
	{
		return 7;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST))
	{
		return 8;
	}
	else
	{
		return 0;
	}

	__HAL_RCC_CLEAR_RESET_FLAGS();
}

/* ##########################################
 * RESET CAUSE ENDING
 * ##########################################
 *  */




/**
  * @brief System Clock Configuration
  * @retval None
  */
//void SystemClock_Config(void)
//{
//  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//
//  /** Configure LSE Drive Capability
//  */
//  HAL_PWR_EnableBkUpAccess();
//  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
//  /** Initializes the CPU, AHB and APB busses clocks
//  */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE
//                              |RCC_OSCILLATORTYPE_LSE;
//  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
//  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//  RCC_OscInitStruct.PLL.PLLM = 1;
//  RCC_OscInitStruct.PLL.PLLN = 8;
//  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
//  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
//  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Initializes the CPU, AHB and APB busses clocks
//  */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
//                              |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART3
//                              |RCC_PERIPHCLK_LPTIM2|RCC_PERIPHCLK_I2C1
//                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_I2C4
//                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_ADC;
//  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
//  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
//  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
//  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
//  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
//  PeriphClkInit.I2c4ClockSelection = RCC_I2C4CLKSOURCE_PCLK1;
//  PeriphClkInit.Lptim2ClockSelection = RCC_LPTIM2CLKSOURCE_PCLK;
//  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
//  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
//  PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
//  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
//  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
//  PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
//  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
//  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
//  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
//  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK|RCC_PLLSAI1_ADC1CLK;
//  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Configure the main internal regulator output voltage
//  */
//  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//}

///* Inits the peripherals */
//void InitPeripherals(void)
//{
//	//Hal Init
//	HAL_Init();
//
//	//Init system clock
//	SystemClock_Config();
//
//	//Peripherals
//	MX_GPIO_Init();
//	MX_CRC_Init();
//	MX_IWDG_Init();
//	MX_SPI2_Init();
//	MX_USART1_UART_Init();
//	MX_USART2_UART_Init();
//	MX_USART3_UART_Init();
//	MX_ADC1_Init();
//	MX_I2C1_Init();
//	MX_I2C2_Init();
//	MX_I2C4_Init();
//	MX_RTC_Init();
//	MX_SDMMC1_SD_Init();
//	MX_FATFS_Init();
//	MX_LPTIM2_Init();
//	MX_TIM2_Init();
//
//	//Enable Interrupt on USART2 (Sara Com)
//	LL_USART_ClearFlag_ORE(USART2);
//	LL_USART_EnableIT_RXNE(USART2);
//
//	//Enable Interrupt on USART3 (GPS Com)
//	LL_USART_ClearFlag_ORE(USART3);
//	LL_USART_EnableIT_RXNE(USART3);
//}
//
//
//
///
//
////############ FOR SYSTEM POWER SAVING ############//
//
//void SystemPower_Config(void)
//{
//
//	/* Enable Power Control clock */
//	__HAL_RCC_PWR_CLK_ENABLE()
//	;
//
//	/* Enable Ultra low power mode */
////	HAL_PWREx_EnableUltraLowPower();
////
////	/* Enable the fast wake up from Ultra low power mode */
////	HAL_PWREx_EnableFastWakeUp();
//
//}
//
///* Optimal GPIO settings for sleep current  (EXAMPLE)*/
////todo
//static void LowLevel_TotalSleepGPIO(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	__HAL_RCC_GPIOA_CLK_ENABLE()
//	;
//	__HAL_RCC_GPIOB_CLK_ENABLE()
//	;
//	__HAL_RCC_GPIOC_CLK_ENABLE()
//	;
//	__HAL_RCC_GPIOD_CLK_ENABLE()
//	;
//	__HAL_RCC_GPIOH_CLK_ENABLE()
//	;
//	__HAL_RCC_GPIOE_CLK_ENABLE()
//	;
//
//	/* All GPIO A that shouldn't have internal pullup/pulldowns init as nopull */
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//
//	/* All GPIO B that shouldn't have internal pullup/pulldowns init as nopull */
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//	/* All GPIO C that shouldn't have internal pullup/pulldowns init as nopull */
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//	/* All GPIO D that shouldn't have internal pullup/pulldowns init as nopull */
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
//	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
//	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
//
//	/* Disable GPIOs clock */
//	__HAL_RCC_GPIOA_CLK_DISABLE();
//	__HAL_RCC_GPIOB_CLK_DISABLE();
//	__HAL_RCC_GPIOC_CLK_DISABLE();
//	__HAL_RCC_GPIOD_CLK_DISABLE();
//	__HAL_RCC_GPIOH_CLK_DISABLE();
//	__HAL_RCC_GPIOE_CLK_DISABLE();
//}
//
///* Optimal GPIO settings for sleep current  (EXAMPLE)*/
////todo
//static void SystemSleepGPIO(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	__HAL_RCC_GPIOA_CLK_ENABLE()
//	;
//	__HAL_RCC_GPIOB_CLK_ENABLE()
//	;
//	__HAL_RCC_GPIOC_CLK_ENABLE()
//	;
//	__HAL_RCC_GPIOD_CLK_ENABLE()
//	;
//	__HAL_RCC_GPIOH_CLK_ENABLE()
//	;
//	__HAL_RCC_GPIOE_CLK_ENABLE()
//	;
//
//	/* All GPIO A that shouldn't have internal pullup/pulldowns init as nopull */
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_1) & (~GPIO_PIN_2) & (~GPIO_PIN_3)
//			& (~GPIO_PIN_6) & (~GPIO_PIN_7);
//
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	/* All GPIO A that should have internal pullup/pulldowns */
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
//	GPIO_InitStructure.Pin = (GPIO_PIN_2) | (GPIO_PIN_3);
//
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	/* All GPIO B that shouldn't have internal pullup/pulldowns init as nopull */
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//	/* All GPIO C that shouldn't have internal pullup/pulldowns init as nopull */
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//	/* All GPIO C that should have internal pullup/pulldowns */
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
//	GPIO_InitStructure.Pin = GPIO_PIN_7;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//	/* All GPIO D that shouldn't have internal pullup/pulldowns init as nopull */
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
//	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
//	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
//
//	/* Disable GPIOs clock */
//	__HAL_RCC_GPIOA_CLK_DISABLE();
//	__HAL_RCC_GPIOB_CLK_DISABLE();
//	__HAL_RCC_GPIOC_CLK_DISABLE();
//	__HAL_RCC_GPIOD_CLK_DISABLE();
//	__HAL_RCC_GPIOH_CLK_DISABLE();
//	__HAL_RCC_GPIOE_CLK_DISABLE();
//}
//
//void SystemSleepAndWakeUp(void)
//{
//	/* Deinit everything */
//
//
//	///////////////////////
//
//	/* Disable Wakeup Counter */
//	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
//
//	/* Go to sleep */
//	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
//
//	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
//	__HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
//
//	/* Enable SysTick interrupts */
//
//	/* Stop RTC timer to generate interrupt */
//	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
//
//	/* Reinit everything */
//	SystemClock_Config();
//
//	/* Init everything that is needed */
//	///////////////////////
//
//
//}
//
///* Max sleep for 2 sec */
//void SleepMax2s(uint32_t msToSleep)
//{
//	uint32_t msClockCycle = 32;
//
//	uint32_t sleepTime = msClockCycle * msToSleep;
//
//	uint32_t period = 65535;
//
//	// Start interrupt timeout for timer
//	HAL_LPTIM_TimeOut_Start_IT(&hlptim2, period, sleepTime);
//
//	/* Enter in STOP2 mode */
//	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
//
//	SystemClock_Config();
//
//	HAL_LPTIM_TimeOut_Stop_IT(&hlptim2);
//}
//
//// END OF POWER SAVING //


