/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "__ExegerGeneric.h"
#include "Uart_Handling.h"
#include "NeoPixel.h"
#include "Battery.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint8_t checkCommando;

extern uint8_t BLE_Flag;
extern uint8_t Setting_Flag;
extern uint8_t Vbat_Flag;

uint32_t rainbowCounter = 0;
uint8_t BLE_Mode = 1;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void SystemPower_Config(void);
void SystemSleepAndWakeup(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

	/* Enable USART2 IRQ */
	LL_USART_ClearFlag_ORE(USART2);
	LL_USART_EnableIT_RXNE(USART2);

	HAL_GPIO_WritePin(Neo_En_GPIO_Port, Neo_En_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BLE_En_GPIO_Port, BLE_En_Pin, GPIO_PIN_SET);

	HAL_Delay(10);

  led_init();
  led_set_color_all(0x00, 0x00, 0x00);
  led_update(1);

  PowerUpSword();

  InitRainbows();
//SystemSleepAndWakeup();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(checkCommando)
	  {
		  checkCommando = 0;

		  NeoStateMachine();
	  }

	if(BLE_Flag)
	{
		BLE_Flag = 0;
	    if(BLE_Mode)
	    {
	    	BLE_Mode = 0;
		  //Enable BLE
	    	while(HAL_GPIO_ReadPin(BLE_EXTI5_GPIO_Port, BLE_EXTI5_Pin) == GPIO_PIN_SET)
	    	{
	    		HAL_Delay(1);
	    	}
	    	EnableBLE();
	    }
	    else
	    {
	    	BLE_Mode = 1;
	    	while(HAL_GPIO_ReadPin(BLE_EXTI5_GPIO_Port, BLE_EXTI5_Pin) == GPIO_PIN_SET)
			{
	    		HAL_Delay(1);
			}
	    	DisableBLE();
	    }

	  }
	  if(Setting_Flag)
	  {
		  //Move up 1 light level
		  while(HAL_GPIO_ReadPin(Light_Setting_EXTI6_GPIO_Port, Light_Setting_EXTI6_Pin) == GPIO_PIN_SET)
			{
			  HAL_Delay(1);
			}
		  UpdateIntensity(GetNeoIntensityState());
		  Setting_Flag = 0;

	  }
	  if(Vbat_Flag)
	  {
		  while(HAL_GPIO_ReadPin(VBat_Level_EXTI7_GPIO_Port, VBat_Level_EXTI7_Pin) == GPIO_PIN_SET)
			{
			  HAL_Delay(1);
			}

		  SetNeoState(NEO_BATTERY);
	  }
	  if(GetNeoState() == NEO_BATTERY && Vbat_Flag == 1)
	  {
		  BatteryMeasure_Init();

		  NeoBatteryLevelLight(Battery_GetLevel());

		  DisableBatterySensePin();

		  Vbat_Flag = 0;
	  }
	  else if(GetNeoState() == NEO_RAINBOWCOLD)
	  {
		  if(rainbowCounter > 20 * GetNeoIntensityState())
		  {
			  UpdateRainbowCold();
			  rainbowCounter = 0;
		  }

		  rainbowCounter++;
	  }
	  else if(GetNeoState() == NEO_RAINBOWWARM)
	  {
		  if(rainbowCounter > 20 * GetNeoIntensityState())
		  {
			  UpdateRainbowWarm();
			  rainbowCounter = 0;
		  }

		  rainbowCounter++;
	  }

	  HAL_Delay(1);

	  HeartBeatToggle();

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

   if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
  Error_Handler();  
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_MSI_Enable();

   /* Wait till MSI is ready */
  while(LL_RCC_MSI_IsReady() != 1)
  {
    
  }
  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
  LL_RCC_MSI_SetCalibTrimming(0);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 20, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_PLLSAI1_ConfigDomain_ADC(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 16, LL_RCC_PLLSAI1R_DIV_4);
  LL_RCC_PLLSAI1_EnableDomain_ADC();
  LL_RCC_PLLSAI1_Enable();

   /* Wait till PLLSAI1 is ready */
  while(LL_RCC_PLLSAI1_IsReady() != 1)
  {
    
  }
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_SetSystemCoreClock(40000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();  
  };
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLLSAI1);
}

/* USER CODE BEGIN 4 */
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

void SystemSleepAndWakeup(void)
{
//	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	/* Go to sleep */
	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

//	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
//	__HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
//
//	/* Enable SysTick interrupts */
//
//	/* Stop RTC timer to generate interrupt */
//	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	SystemClock_Config();

	MX_GPIO_Init();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
