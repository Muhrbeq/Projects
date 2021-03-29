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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "fatfs.h"
#include "i2c.h"
#include "iwdg.h"
#include "lptim.h"
#include "rtc.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "__ExegerGeneric.h"
#include "LIS2DW12.h"
#include "L3GD20H.h"
#include "PowerManage.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t accelerometerDetect = 0;
uint8_t gyroscopeDetect = 0;
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
//  MX_GPIO_Init();
//  MX_ADC1_Init();
//  MX_I2C2_Init();
//  MX_I2C3_Init();
//  MX_I2C4_Init();
//  MX_USART1_UART_Init();
//  MX_RTC_Init();
//  MX_SDMMC1_SD_Init();
//  MX_USB_PCD_Init();
//  MX_FATFS_Init();
//  MX_CRC_Init();
//  MX_LPTIM1_Init();
////  MX_IWDG_Init();
//  MX_TIM1_Init();
//  MX_TIM3_Init();
//  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
// LL_GPIO_SetOutputPin(GPIOB, GPIO_PIN_12);
// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
// HAL_GPIO_WritePin(Sens2_Led_GPIO_Port, Sens2_Led_Pin, GPIO_PIN_SET);
//
//
//// L3_Start(&hi2c3, L3_LOW_ODR_DISABLE, L3_ODR_200HZ, L3_POWER_MODE_NORMAL, L3_SCALE_245_DPS);
//
//
// if(LIS_Start(&hi2c3, LIS_ODR_200_HZ, LIS_MODE_PERFORMANCE, LIS_FIFO_CONTINOUS, LIS_SCALE_2G) == HAL_OK)
// {
//	 accelerometerDetect = 1;
// }
// if(L3_Start(&hi2c3, L3_LOW_ODR_DISABLE, L3_ODR_200HZ, L3_POWER_MODE_NORMAL, L3_SCALE_245_DPS) == HAL_OK)
// {
//	 gyroscopeDetect = 1;
// }
// LIS_initStruct(LIS_GetAccelerationStruct(&hi2c3));
// L3_initStruct(L3_GetGyroStruct(&hi2c3));

 HAL_Delay(1000);

SystemPower_Config();
SystemSleepAndWakeup();
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(accelerometerDetect)
	  {
		  LIS_ReadAxesXYZ(&hi2c3, LIS_GetAccelerationStruct(&hi2c3));
	  }
	  if(gyroscopeDetect)
	  {
		  L3_ReadAxesXYZ(&hi2c3, L3_GetGyroStruct(&hi2c3));
	  }
	  HeartBeatToggle();
//
	  LL_mDelay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
