/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"

/* USER CODE BEGIN 0 */
#include "iwdg.h"
#include "StateMachine.h"
#include "sara.h"
#include "TimeFunc.h"
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;

uint16_t sysCounter = 0;
/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */


  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles USART2 global interrupt.
*/
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	USART2_Callback();
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
* @brief This function handles RTC alarm interrupt through EXTI line 18.
*/
void RTC_Alarm_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_Alarm_IRQn 0 */

  /* USER CODE END RTC_Alarm_IRQn 0 */
  HAL_RTC_AlarmIRQHandler(&hrtc);
  /* USER CODE BEGIN RTC_Alarm_IRQn 1 */
//	SaraConfig *sC = GetSaraConfigurationStruct();
//
//	radioFlags *rE = GetErrorFlagsStruct();
//
//	if (rE->radioRTC == 0)
//	{
//		RTC_SetAlarm(sC->updateFrequency);
//		SetSensorState(SENSOR_INIT);
//	}
//	else
//	{
//		rE->radioRTC = 0;
//	}
  /* USER CODE END RTC_Alarm_IRQn 1 */
}

/* USER CODE BEGIN 1 */
/**
 * @brief This function handles LPTIM1 global interrupt.
 */
void LPTIM1_IRQHandler(void)
{
	/* USER CODE BEGIN LPTIM1_IRQn 0 */
//	HAL_GPIO_WritePin(HeartBeat_GPIO_Port, HeartBeat_Pin, GPIO_PIN_SET);
//	HAL_GPIO_TogglePin(HeartBeat_GPIO_Port, HeartBeat_Pin);
//	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	/* USER CODE END LPTIM1_IRQn 0 */
	HAL_LPTIM_IRQHandler(&hlptim1);
	/* USER CODE BEGIN LPTIM1_IRQn 1 */

	/* USER CODE END LPTIM1_IRQn 1 */
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	SaraConfig *sC = GetSaraConfigurationStruct();
	RTC_SetAlarm(sC->updateFrequency);
	SetSensorState(SENSOR_AWAKE);
}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{

}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
