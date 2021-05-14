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
#include "rng.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "__ExegerGeneric.h"
#include <_LowLevel.h>
#include "_Time.h"
#include "_RevisionControl.h"
#include "_SaraR.h"
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
  MX_CRC_Init();
  //MX_IWDG_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C4_Init();
  MX_RTC_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_LPTIM2_Init();
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */
//  Time_RTCGetDateTime();
//  Time_RTCSetDateTime(21, 2, 2, 12, 1, 2);
//  Time_RTCGetDateTime();

  /*
   * Before main:
   *
   *  - Check reset cause	x
   *  - Check Revision		x
   *  - Check status of all peripherals (ID of everyone)
   *  -
   *
   *
   *
   * */

  uint8_t ResetCause = CheckResetCause();
  TRACE("[ResetCheck] - PASSED\r\n");

  LL_USART_ClearFlag_ORE(USART2);
  LL_USART_EnableIT_RXNE(USART2);

//  while(RevisionControl_GetState() != REVISIONSTATE_DONE &&
//		  RevisionControl_GetState() != REVISIONSTATE_ERROR)
//  {
//	  RevisionControl_StateMachine();
//  }

  /* DISPLAY ALL INIT */

  TRACE("[Main] - Entering While-loop\r\n");
  DualLed_GreenOff();
  DualLed_RedOff();

  StartUpSara() ? DualLed_BlinkGreen(11) : DualLed_BlinkRed(21);

  HAL_Delay(1000);
  SaraInitAtCommands() ? DualLed_BlinkGreen(11) : DualLed_BlinkRed(21);

  HAL_Delay(1000);

  SaraCheckCommand("AT+GMR\r\n", "+CEREG: 0,1OK.....................................................", LOW_TIMEOUT);
  SaraCheckCommand("AT+CGDCONT?\r\n", "+CEREG: 0,1OK.....................................................", LOW_TIMEOUT);
  HAL_Delay(10000);
//  SaraCheckCommand("AT+COPS=0\r\n", "OK", LOW_TIMEOUT);
//  SaraCheckCommand("AT+URAT=7,8\r\n", "OK", LOW_TIMEOUT);

 // SaraCheckCommand("AT+CFUN?\r\n", "+CFUN: 1OK", LOW_TIMEOUT) ? DualLed_BlinkGreen(11) : DualLed_BlinkRed(21);
  //HAL_Delay(1000);
 // SaraCheckCommand("AT+CEREG=1\r\n", "OK", LOW_TIMEOUT) ? DualLed_BlinkGreen(11) : DualLed_BlinkRed(21);
//  HAL_GPIO_WritePin(Radio_Enable_GPIO_Port, Radio_Enable_Pin, GPIO_PIN_SET);

//  HAL_GPIO_WritePin(Sara_Reset_GPIO_Port, Sara_Reset_Pin, GPIO_PIN_SET);
//  HAL_GPIO_WritePin(Sara_Power_On_GPIO_Port, Sara_Power_On_Pin, GPIO_PIN_SET);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(SaraCheckCommand("AT+CEREG?\r\n", "+CEREG: 0,1OK", LOW_TIMEOUT))
	  {
		  DualLed_ToggleGreen() ;
	  }
	  else
	  {
		  DualLed_ToggleRed();
	  }
//	  HAL_Delay(300);
//	  DualLed_ToggleGreen();

	  /* TODO:
	   *
	   * Order BOM of 4GLL
	   * 	Farnell/digikey order x (no sara)
	   *
	   * Test generate random number		x
	   *
	   * Basic functions for MCU
	   * 	Generate CRC on data
	   * 	LED - Use for debugging
	   * 	Button - IRQ
	   * 	RTC alarm
	   * 	Low power solution
	   * 	ADC for revision control 		x
	   * 	USB communication - HID?
	   * 	SD-card read/write				x
	   * 	Redefine printf function with define
	   * 	Window circuit for the LED
	   * 	SPI communication
	   * 	GPIOs
	   * 	Sleep with different IOs
	   * 	Implement returnCodes for all error states
	   *
	   *
	   * States
	   * 	States of all actions
	   * 		Have peripherals as states?
	   *
	   * Basic functions for peripherals
	   * 	Code for TMP112	(OOP)		x
	   * 	Code for HDC1080 (OOP)		x
	   * 	Code for OPT (OOP)
	   * 	Code for Spectrum sensor (OOP)
	   * 	Code for Accelerometer (OOP)
	   *	ID of each peripheral
	   *	Rewrite all code
	   *	Test Screen functionality
	   *	Comment for readability
	   *
	   * Structure the UARTs for input from comp + Sara with IRQ
	   * 	Arrays for each UART to store data
	   * 	Read arrays when \r / \n received
	   * 	Write test scenarios for Computer communication
	   *
	   * Structure the output data sent
	   * 	How am I going to send the data?
	   *
	   * Structure startup sequence
	   * 	Startup-cause
	   *
	   */


	  /* Add State machine for sensors and radio/GPS */

//	  uint32_t rngN = GenerateRandomNumber();
//	  printf("%lu\r\n", rngN);
//	  TRACE("%lu\r\n", rngN);



	  //TRACE("_________\r\n");

//	  printf("This is a test printf\r\n");
//
//	  debugPrint("Testing DebugPrint - Print", debugLevel_Print);
//	  debugPrint("Testing DebugPrint - Info", debugLevel_Info);
//	  debugPrint("Testing DebugPrint - Warning", debugLevel_Warning);
//	  debugPrint("Testing DebugPrint - Error", debugLevel_Error);

	  HAL_Delay(500);
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_LPTIM2|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_I2C4
                              |RCC_PERIPHCLK_USB|RCC_PERIPHCLK_SDMMC1
                              |RCC_PERIPHCLK_RNG|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c4ClockSelection = RCC_I2C4CLKSOURCE_PCLK1;
  PeriphClkInit.Lptim2ClockSelection = RCC_LPTIM2CLKSOURCE_PCLK;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_PLLSAI1;
  PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 12;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK|RCC_PLLSAI1_ADC1CLK;
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
