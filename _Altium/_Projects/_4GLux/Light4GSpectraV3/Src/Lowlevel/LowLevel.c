/*
 * LowLevel.c
 *
 *  Created on: 15 mars 2019
 *      Author: Rasmus.Muhrbeck
 */
#include "LowLevel.h"

#include "Global.h"
#include "TimeFunc.h"
#include "utils.h"
#include "Battery.h"

#include "SDCard.h"

//extern struct radioFlags radioErrors;
extern uint8_t checkResetCause;

void SystemPower_Config(void)
{

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE()
	;

	/* Enable Ultra low power mode */
//	HAL_PWREx_EnableUltraLowPower();
//
//	/* Enable the fast wake up from Ultra low power mode */
//	HAL_PWREx_EnableFastWakeUp();

}

/* Optimal GPIO settings for sleep current */
static void SystemSleepGPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;
	__HAL_RCC_GPIOH_CLK_ENABLE()
	;
	__HAL_RCC_GPIOE_CLK_ENABLE()
	;

	/* All GPIO A that shouldn't have internal pullup/pulldowns init as nopull */
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_1) & (~GPIO_PIN_2) & (~GPIO_PIN_3)
			& (~GPIO_PIN_6) & (~GPIO_PIN_7);

	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* All GPIO A that should have internal pullup/pulldowns */
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Pin = Radio_EN_Pin | (GPIO_PIN_2) | (GPIO_PIN_3) | Sara_Power_On_Pin
			| Sara_Reset_Pin;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* All GPIO B that shouldn't have internal pullup/pulldowns init as nopull */
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~Spectrum_EN_Pin) & (~Temperature_EN_Pin);

	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* All GPIO B that should have internal pullup/pulldowns */
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Pin = Spectrum_EN_Pin | Temperature_EN_Pin;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Turns off SD Card, otherwise strange behavior */
	DisEnableSDPin(0);

	/* All GPIO C that shouldn't have internal pullup/pulldowns init as nopull */
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_7) & (~VBAT_Sense_EN_Pin);
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* All GPIO C that should have internal pullup/pulldowns */
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Pin = GPIO_PIN_7 | VBAT_Sense_EN_Pin;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* All GPIO D that shouldn't have internal pullup/pulldowns init as nopull */
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Disable GPIOs clock */
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
	__HAL_RCC_GPIOH_CLK_DISABLE();
	__HAL_RCC_GPIOE_CLK_DISABLE();
}


/* Sleep function that  */
void SystemSleepAndWakeUp(void)
{
	/* Deinit everything */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	SystemSleepGPIO();
	MX_I2C1_DeInit();
	MX_I2C2_DeInit();
	MX_LPUART1_UART_DeInit();
	MX_ADC_DeInit();

	/* Disable Wakeup Counter */
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	/* Go to sleep */
	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	__HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);

	/* Enable SysTick interrupts */

	/* Stop RTC timer to generate interrupt */
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	/* Reinit everything */
	SystemClock_Config();

	/* Init everything that is needed */
	MX_GPIO_Init();
	MX_LPUART1_UART_Init();
	MX_USART2_UART_Init();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}

/* Set every GPIO to Analog except for Radio adn sensor pins */
static void GPIOSetAnalogExceptRadio(void)  //TODO  not even used right now
{

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clock. todo fix this ugly ; ; ; ;  */
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;
	__HAL_RCC_GPIOH_CLK_ENABLE()
	;
	__HAL_RCC_GPIOE_CLK_ENABLE()
	;

	/* 	Configure all GPIO port pins in Analog Input mode (floating input trigger OFF)
	 *	Note: Debug using ST-Link is not possible during the execution of this
	 *	example because communication between ST-link and the device
	 *	under test is done through UART. All GPIO pins are disabled (set to analog input mode) including  UART I/O pins.
	 */
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_8 & GPIO_PIN_9 & GPIO_PIN_10 & GPIO_PIN_11 & GPIO_PIN_12
			& GPIO_PIN_13 & GPIO_PIN_14 & GPIO_PIN_15;

	//& (~GPIO_PIN_13) & (~GPIO_PIN_14); set only the pins except the sdwio amd swclk (a13 and a14)

	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	GPIO_InitStructure.Pin = GPIO_PIN_All;
//
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Turns off SD Card, otherwise strange behavior */
	DisEnableSDPin(0);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_7);
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Disable GPIOs clock */
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
	__HAL_RCC_GPIOH_CLK_DISABLE();
	__HAL_RCC_GPIOE_CLK_DISABLE();

}

/* Turn off SD card for power consumption */
void DisableSD_Card(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Turn off SD power EN pin */
	DisEnableSDPin(0);

	/* Reinit GPIO pins */
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/* Set GPIOs to analog except sensor and radio pins */
void GPIOSetAnalogSensor(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clock. todo fix this ugly ; ; ; ;  */
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;
	__HAL_RCC_GPIOH_CLK_ENABLE()
	;
	__HAL_RCC_GPIOE_CLK_ENABLE()
	;

	/* 	Configure all GPIO port pins in Analog Input mode (floating input trigger OFF)
	 *	Note: Debug using ST-Link is not possible during the execution of this
	 *	example because communication between ST-link and the device
	 *	under test is done through UART. All GPIO pins are disabled (set to analog input mode) including  UART I/O pins.
	 */
//			GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//			GPIO_InitStructure.Pull = GPIO_NOPULL;
//			GPIO_InitStructure.Pin = GPIO_PIN_8 & GPIO_PIN_9 & GPIO_PIN_10 & GPIO_PIN_11 & GPIO_PIN_12
//					& GPIO_PIN_13 & GPIO_PIN_14 & GPIO_PIN_15;
//
//			//& (~GPIO_PIN_13) & (~GPIO_PIN_14); set only the pins except the sdwio amd swclk (a13 and a14)
//
//			HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
			| GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Turns off SD Card, otherwise strange behavior */
	DisEnableSDPin(0);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_7);
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Disable GPIOs clock */
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
	__HAL_RCC_GPIOH_CLK_DISABLE();
	__HAL_RCC_GPIOE_CLK_DISABLE();
}

/* Sleep when searching for network */
void EnterStopRadioNetwork(void)
{
	printf("Radio searching for network... \t going to sleep, leaving radio on\r\n");

	/* Set alarm so that the MCU wakes up after 1 sec */
	RTC_SetAlarmB(1);

	/* DeInit */
	GPIOSetAnalogExceptRadio();
	MX_LPUART1_UART_DeInit();

	/* Disable Wakeup Counter */
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	/* Enter Stop mode 2 for power saving */
	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	__HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);

	/* Enable SysTick interrupts */

	/* Stop RTC timer to generate interrupt */
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	/* Init everything */
	SystemClock_Config();
	GPIOInitRadio();
	MX_LPUART1_UART_Init();
	MX_USART2_UART_Init();
	MX_LPTIM1_Init();

	printf("Start\r\n");
}

//void EnterStopMode(void)
//{
//
//	HAL_GPIO_WritePin(GPIOC, Heartbeat_Pin, GPIO_PIN_RESET);
//
//	printf("Sleep\r\n");
//
//	HAL_GPIO_WritePin(GPIOC, Heartbeat_Pin, GPIO_PIN_SET);
//
//	GPIO_Set_Analog();
//
//	MX_I2C1_DeInit();
//
//	MX_I2C2_DeInit();
//
//	MX_LPUART1_UART_DeInit();
//
//	MX_ADC_DeInit();
//
//	/* Disable Wakeup Counter */
//	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
//
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
//	SystemClock_Config();
//
//	MX_GPIO_Init();
//
//	HAL_GPIO_WritePin(GPIOC, Heartbeat_Pin, GPIO_PIN_RESET);
//
//	MX_LPUART1_UART_Init();
//
//	MX_USART2_UART_Init();
//
//	MX_ADC1_Init();
//
//	MX_LPTIM1_Init();
//
//	MX_FATFS_Init();
//
//	HAL_SD_MspInit(&hsd1);
//
//	printf("Start\r\n");
//
//	MX_I2C1_Init();
//
//	MX_I2C2_Init();
//
//	HAL_GPIO_WritePin(GPIOC, Heartbeat_Pin, GPIO_PIN_SET);
//
//}

/* Max sleep for 2 sec */
void SleepForApproxMsSpectra(uint32_t mS)
{
	uint32_t msClockCycle = 32;

	uint32_t sleepTime = msClockCycle * mS;

	uint32_t period = 65535;

	HAL_LPTIM_TimeOut_Start_IT(&hlptim1, period, sleepTime);
	
	/* Enter in STOP2 mode */
	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

	SystemClock_Config();

	HAL_LPTIM_TimeOut_Stop_IT(&hlptim1);
}


/* Max sleep for 2 sec */
void SleepForApproxMs(uint32_t mS)
{
	uint32_t msClockCycle = 32;

	uint32_t sleepTime = msClockCycle * mS;

	uint32_t period = 65535;

	HAL_LPTIM_TimeOut_Start_IT(&hlptim1, period, sleepTime);
	
	/* Enter in STOP2 mode */
	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

	SystemClock_Config();

	HAL_LPTIM_TimeOut_Stop_IT(&hlptim1);
}

/* Check what the cause of reset is */
void CheckResetCause(void)
{
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
	{
		checkResetCause = 1;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
	{
		checkResetCause = 2;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
	{
		checkResetCause = 3;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
	{
		checkResetCause = 4;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
	{
		checkResetCause = 5;
	}
	// Needs to come *after* checking the `RCC_FLAG_PORRST` flag in order to ensure first that the reset cause is
	// NOT a POR/PDR reset. See note below.
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
	{
		checkResetCause = 6;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_FWRST))
	{
		checkResetCause = 7;
	}
	else if (__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST))
	{
		checkResetCause = 8;
	}
	else
	{
		checkResetCause = 0;
	}

	__HAL_RCC_CLEAR_RESET_FLAGS();
}

/* Initialize everything (from auto generated cubeMX) */
void InitAll(void)
{
	HAL_Init();

	SystemClock_Config();

	CheckResetCause();
	
	MX_GPIO_Init();


	MX_CRC_Init();

	MX_I2C2_Init();

	MX_RTC_Init();

	MX_LPUART1_UART_Init();

	MX_USART2_UART_Init();

	MX_I2C1_Init();



	MX_ADC1_Init();

	MX_LPTIM1_Init();

	/* Enable USART2 IRQ */
	LL_USART_ClearFlag_ORE(USART2);
	LL_USART_EnableIT_RXNE(USART2);

	MX_FATFS_Init();

	MX_SDMMC1_SD_Init();

	HAL_SD_MspInit(&hsd1);

	MX_IWDG_Init();
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	/**Configure LSE Drive Capability 
	 */
	HAL_PWR_EnableBkUpAccess();

	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

	/**Initializes the CPU, AHB and APB busses clocks 
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 8;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Initializes the CPU, AHB and APB busses clocks 
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
			| RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_USART2
			| RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_I2C2
			| RCC_PERIPHCLK_SDMMC1
			| RCC_PERIPHCLK_ADC;
	PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
	PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
	PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
	PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
	PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
	PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
	PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
	PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
	PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
	PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
	PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK | RCC_PLLSAI1_ADC1CLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure the main internal regulator output voltage 
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure the Systick interrupt time 
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick 
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}



/* NOT USED */

void GPIO_Set_Analog(void)  //TODO  not even used right now
{

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clock. todo fix this ugly ; ; ; ;  */
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;
	__HAL_RCC_GPIOH_CLK_ENABLE()
	;
	__HAL_RCC_GPIOE_CLK_ENABLE()
	;

	/* 	Configure all GPIO port pins in Analog Input mode (floating input trigger OFF)
	 *	Note: Debug using ST-Link is not possible during the execution of this
	 *	example because communication between ST-link and the device
	 *	under test is done through UART. All GPIO pins are disabled (set to analog input mode) including  UART I/O pins.
	 */
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All; //& (~GPIO_PIN_13) & (~GPIO_PIN_14); set only the pins except the sdwio amd swclk (a13 and a14)

	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Turns off SD Card, otherwise strange behavior */
	DisEnableSDPin(0);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_7);
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_All;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Disable GPIOs clock */
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
	__HAL_RCC_GPIOH_CLK_DISABLE();
	__HAL_RCC_GPIOE_CLK_DISABLE();

}
