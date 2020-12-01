
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
//#include "adc.h"
//#include "crc.h"
//#include "fatfs.h"
//#include "i2c.h"
//#include "usart.h"
//#include "rtc.h"
//#include "sdmmc.h"
//#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "Global.h"
#include "sara.h"
#include "mqtt.h"
#include "Settings.h"
#include "utils.h"
#include "StateMachine.h"
#include "Battery.h"
#include "HDC1080.h"
#include "Tmp112.h"
#include "VEML7700.h"


#include "Error.h"
#include "MultiSensor.h"
#include "LowLevel.h"

#include "SDCard.h"

//extern struct radioFlags radioErrors;
//extern SaraConfig SaraConfiguration;
//extern errorTimer startTimer;
//extern errorTimer endTimer;
uint8_t checkResetCause = 0;

/* USER CODE END Includes */

void SystemClock_Config(void);

void Start_blink(void);
void Start_blink(void)
{
	uint8_t i;

	for (i = 0; i < 20; i++)
	{
		HAL_GPIO_TogglePin(GPIOC, Heartbeat_Pin);
		HAL_Delay(100);
	}
}

/* Blinks Heartbeat LED */
void LED_Blink(void)
{
	HAL_GPIO_TogglePin(Heartbeat_GPIO_Port, Heartbeat_Pin);
}




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
	/* Init peripherals */
	InitAll();

	/* Init every struct that is needed in the run */
	InitSaraStruct(GetSaraConfigurationStruct());
	InitErrorStruct(GetErrorFlagsStruct());

	/* Reason for the two InfoStructs, one is for each iteration (temp), if an error occurs,
	 * it will be written over to the other, which will be written to the SD card when its time */
	InitInfoStruct(GetInfoStruct());
	InitInfoStruct(GetTempInfoStruct());

	Data *data = GetDataStruct();
	Error *er = GetErrorStruct();
	radioFlags *rE = GetErrorFlagsStruct();
	DataStruct_init(data);
	ErrorStruct_init(er);
	
	/* Program Option bits so that IWDG freezes in shutdown mode */
	Program_IWDG_Freeze_Shutdown();


	printf("\r\n\r\n\t ** STARTaaa! ** \r\n\r\n");

	/* Turn SD Power pin high */
	DisEnableSDPin(1);

	/* Wait 1 sec for the SD card to be initialized */
	HAL_Delay(1000);

	/* Create folders on SD card that are used for storage */
	InitFoldersSD();

	/* Turn SD Power pin low */
	DisEnableSDPin(0);

	/* Debug, check the reason for reset */
	printf("Reason for reset: %d\r\n", checkResetCause);
	printf("---------------------\r\n");

	/* Add to InfoLog the reason for reset */
	InfoLogAdd(ERRORLEVEL_FATAL,
	LOG_FUNC_MAIN, LOG_SUB_READ,
	LOG_TYPE_START,
	LOG_STATUS_PASSED, checkResetCause, 0, 0, GetTempInfoStruct());



  while (1)
  {

		/* Resets/kicks the WatchDog so that the module can be on for another 32 sec */
		ResetIWDG();

		/* If Radio is enabled (no errors in radio) */
		if (rE->radioEnableFlag == 1)
		{
			/* Go into radio state machine */
			RadioStateMachine(GetDataStruct(), GetErrorStruct());
		}
		/* If error signal flag has been flagged, go into error timer */
		else
		{
			/* Check if X seconds have passed between last connected */
			ErrorTimerHourlyCheck(
					GetErrorTimerStruct(startTimerEnum),
					GetErrorTimerStruct(endTimerEnum),
					GetErrorFlagsStruct());
		}

		/* Enter Sensor state machine */
		SensorStateMachine(GetDataStruct(), GetErrorStruct());

		/* Enter sleep state machine */
		SleepStateMachine();
  }

}



/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
