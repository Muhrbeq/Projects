/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "__ExegerGeneric.h"
#include "_OPT3001.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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

OPT3001 opt1;
OPT3001 opt2;
OPT3001 opt3;
OPT3001 opt4;
OPT3001 opt5;
OPT3001 opt6;
OPT3001 opt7;
OPT3001 opt8;
OPT3001 opt9;
OPT3001 opt10;

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
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_USART1_UART_Init();
  MX_I2C4_Init();
  /* USER CODE BEGIN 2 */



OPT3001_InitDevice(&opt1, &hi2c1, 0b1000111, 0b1000111);
OPT3001_InitDevice(&opt2, &hi2c1, 0b1000110, 0b1000110);
OPT3001_InitDevice(&opt3, &hi2c1, 0b1000100, 0b1000100);
OPT3001_InitDevice(&opt4, &hi2c1, 0b1000101, 0b1000101);
OPT3001_InitDevice(&opt5, &hi2c4, 0b1000100, 0b1000100);
OPT3001_InitDevice(&opt6, &hi2c4, 0b1000111 , 0b1000111);
OPT3001_InitDevice(&opt7, &hi2c3, 0b1000111, 0b1000111);
OPT3001_InitDevice(&opt8, &hi2c3, 0b1000110, 0b1000110);
OPT3001_InitDevice(&opt9, &hi2c3, 0b1000100, 0b1000100);
OPT3001_InitDevice(&opt10, &hi2c3, 0b1000101, 0b1000101);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
OPT3001_StateMachine(&opt1);
OPT3001_StateMachine(&opt2);
OPT3001_StateMachine(&opt3);
OPT3001_StateMachine(&opt4);
OPT3001_StateMachine(&opt5);
OPT3001_StateMachine(&opt6);
OPT3001_StateMachine(&opt7);
OPT3001_StateMachine(&opt8);
OPT3001_StateMachine(&opt9);
OPT3001_StateMachine(&opt10);

if(opt1.tState == OPT3001_DONE)
{
	printf("OPT1:%lf\n", opt1.currentLux);
	opt1.sStatus = OPT3001_INITIATE;
}
if(opt2.tState == OPT3001_DONE)
{
	printf("OPT2:%lf\n", opt2.currentLux);
	opt2.sStatus = OPT3001_INITIATE;
}
if(opt3.tState == OPT3001_DONE)
{
	printf("OPT3:%lf\n", opt3.currentLux);
	opt3.sStatus = OPT3001_INITIATE;
}
if(opt4.tState == OPT3001_DONE)
{
	printf("OPT4:%lf\n", opt4.currentLux);
	opt4.sStatus = OPT3001_INITIATE;
}
if(opt5.tState == OPT3001_DONE)
{
	printf("OPT5:%lf\n", opt5.currentLux);
	opt5.sStatus = OPT3001_INITIATE;
}
if(opt6.tState == OPT3001_DONE)
{
	printf("OPT6:%lf\n", opt6.currentLux);
	opt6.sStatus = OPT3001_INITIATE;
}
if(opt7.tState == OPT3001_DONE)
{
	printf("OPT7:%lf\n", opt7.currentLux);
	opt7.sStatus = OPT3001_INITIATE;
}
if(opt8.tState == OPT3001_DONE)
{
	printf("OPT8:%lf\n", opt8.currentLux);
	opt8.sStatus = OPT3001_INITIATE;
}
if(opt9.tState == OPT3001_DONE)
{
	printf("OPT9:%lf\n", opt9.currentLux);
	opt9.sStatus = OPT3001_INITIATE;
}
if(opt10.tState == OPT3001_DONE)
{
	printf("OPT10:%lf\n", opt10.currentLux);
	opt10.sStatus = OPT3001_INITIATE;
}


    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C4|RCC_PERIPHCLK_I2C3;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  PeriphClkInit.I2c4ClockSelection = RCC_I2C4CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

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
