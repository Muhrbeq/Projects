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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "__ExegerGeneric.h"
#include "PowerManage.h"
#include "NeoPixel.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint8_t buttonPressed;
extern uint32_t amountButtonPressed;
uint8_t isSleeping;
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
  MX_I2C3_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(Enable_Neo_GPIO_Port, Enable_Neo_Pin, GPIO_PIN_SET);
  HeartBeatOff();

  HAL_Delay(100);

  led_init();
  led_set_color_all(0x00, 0x00, 0x00);
  led_update(1);

  PowerUpSword();

  uint32_t RainbowCounter = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	  while(1)
//	  {
//		  HAL_Delay(100);
//		  HeartBeatToggle();
//	  }

	  if(buttonPressed)
	  	  {
	  		  while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
	  		  {
	  			  amountButtonPressed++;
	  			  HAL_Delay(1);
	  		  }
	  		  /* Check if been asleep or not! */
	  		HeartBeatOff();
	  		  uint8_t counter = 100;
	  		  if(amountButtonPressed > 1000)
	  		  {
	  //			  TurnAllBlue();
	  //			  while(counter > 0)
	  //			  {
	  //				  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	  //				  HAL_Delay(20);
	  //				  counter--;
	  //
	  //			  }
	  			  isSleeping = 1;
	  			  PowerDownSword();
	  			  SystemSleepAndWakeup();


	  			  /* Go to sleeeeep */
	  		  }
	  		  else
	  		  {
	  			  if(isSleeping)
	  			  {
	  				HeartBeatOff();
	  				PowerUpSword();
	  			  }
	  			  else
	  			  {
	  				HeartBeatOff();
	  				NeoStateMachine(GetNeoState());
	  			  }
	  			  isSleeping = 0;
	  		  }

	  		  amountButtonPressed = 0;
	  		  buttonPressed = 0;
	  	  }

	  if(GetNeoState() == NEO_RAINBOW)
	  {
		  HeartBeatOff();
		  RainBow(RainbowCounter);
		  led_update(1);
		  RainbowCounter++;
		  HAL_Delay(50);
		  if(RainbowCounter > 1000)
		  {
			  RainbowCounter = 0;
		  }
	  }

//	  	  if(isSleeping == 1)
//	  	  {
//
//	  		  isSleeping = 0;
//	  		  NeoStateMachine(GetNeoState());
//	  	  }

	  	  HAL_Delay(1);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
//void SystemClock_Config(void)
//{
//  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
//
//  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
//  {
//  Error_Handler();
//  }
//  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
//  LL_RCC_MSI_Enable();
//
//   /* Wait till MSI is ready */
//  while(LL_RCC_MSI_IsReady() != 1)
//  {
//
//  }
//  LL_RCC_MSI_EnableRangeSelection();
//  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
//  LL_RCC_MSI_SetCalibTrimming(0);
//  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 16, LL_RCC_PLLR_DIV_2);
//  LL_RCC_PLL_EnableDomain_SYS();
//  LL_RCC_PLL_Enable();
//
//   /* Wait till PLL is ready */
//  while(LL_RCC_PLL_IsReady() != 1)
//  {
//
//  }
//  LL_RCC_PLLSAI1_ConfigDomain_ADC(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 16, LL_RCC_PLLSAI1R_DIV_2);
//  LL_RCC_PLLSAI1_EnableDomain_ADC();
//  LL_RCC_PLLSAI1_Enable();
//
//   /* Wait till PLLSAI1 is ready */
//  while(LL_RCC_PLLSAI1_IsReady() != 1)
//  {
//
//  }
//  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
//
//   /* Wait till System clock is ready */
//  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
//  {
//
//  }
//  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
//  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
//  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
//  LL_SetSystemCoreClock(32000000);
//
//   /* Update the time base */
//  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
//  {
//    Error_Handler();
//  };
//  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
//  LL_RCC_SetI2CClockSource(LL_RCC_I2C3_CLKSOURCE_PCLK1);
//  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLLSAI1);
//}

/* USER CODE BEGIN 4 */

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
