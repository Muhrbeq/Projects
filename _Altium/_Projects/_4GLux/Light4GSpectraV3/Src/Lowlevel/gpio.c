/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
        * Free pins are configured automatically as Analog (this feature is enabled through 
        * the Code Generation settings)
     PA11   ------> USB_DM
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(
			GPIOC,
			VBAT_Sense_EN_Pin | Heartbeat_Pin | DebugPin12_Pin | DebugPin16_Pin

	| SD_Power_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(
			GPIOA,
			USART2_CTS_Pin | USART2_CK_Pin | Radio_EN_Pin
                          |Sara_Power_On_Pin|Sara_Reset_Pin|DebugPin9_Pin|DebugPin11_Pin 
                          |DebugPin13_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(
			GPIOB,
			Temperature_EN_Pin | PCB_Ver_EN_Pin | PCB_Ver_Sense_Pin | Spectrum_EN_Pin
					| DebugPin17_Pin | DebugPin19_Pin
					| DebugPin22_Pin,
			GPIO_PIN_RESET);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin 
                           PCPin PCPin */
	GPIO_InitStruct.Pin = Heartbeat_Pin | DebugPin12_Pin | DebugPin16_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = SD_Power_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin PAPin 
                           PAPin */
	GPIO_InitStruct.Pin = USART2_CTS_Pin | USART2_CK_Pin | Radio_EN_Pin
                          |Sara_Power_On_Pin|Sara_Reset_Pin|DebugPin9_Pin|DebugPin11_Pin 
                          |DebugPin13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
	GPIO_InitStruct.Pin = INT1_MCU_Pin | INT2_MCU_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = VINT_Sense_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin */
	GPIO_InitStruct.Pin = VBAT_Sense_EN_Pin | PCB_Ver_Sense_Pin
	| PCB_Ver_EN_Pin
	| Temperature_EN_Pin | Spectrum_EN_Pin | DebugPin17_Pin
                          | DebugPin19_Pin
			| DebugPin22_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PC9 PC10 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = VBUS_Detect_Pin | Charge_Detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_Detect_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PH3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
void GPIOInitRadio(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOH_CLK_ENABLE()
	;
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC,
			Heartbeat_Pin | VBAT_Sense_EN_Pin | DebugPin12_Pin | DebugPin16_Pin

					| SD_Power_EN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, DebugPin9_Pin | DebugPin11_Pin | DebugPin13_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
			Temperature_EN_Pin | Spectrum_EN_Pin | DebugPin17_Pin
					| DebugPin19_Pin | DebugPin22_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pins : PCPin PCPin PCPin PCPin
	 PCPin PCPin */
	GPIO_InitStruct.Pin = Heartbeat_Pin | DebugPin12_Pin | DebugPin16_Pin

			| SD_Power_EN_Pin | VBAT_Sense_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PAPin PAPin PAPin PAPin
	 PAPin PAPin PAPin PAPin
	 PAPin */
	GPIO_InitStruct.Pin = DebugPin9_Pin | DebugPin11_Pin | DebugPin13_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PBPin PBPin PBPin */
	GPIO_InitStruct.Pin = INT1_MCU_Pin | INT2_MCU_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = VINT_Sense_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PBPin PBPin PBPin PBPin
	 PBPin PBPin PBPin */
	GPIO_InitStruct.Pin = Temperature_EN_Pin | Spectrum_EN_Pin | DebugPin17_Pin
			| DebugPin19_Pin
			| DebugPin22_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PC9 PC10 PC11 */
//	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
//	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = VBUS_Detect_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(VBUS_Detect_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PA11 */
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_USB_FS;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PA12 */
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PH3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);


}

void GPIOInitSensor(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE()
	;
	__HAL_RCC_GPIOH_CLK_ENABLE()
	;
	__HAL_RCC_GPIOA_CLK_ENABLE()
	;
	__HAL_RCC_GPIOB_CLK_ENABLE()
	;
	__HAL_RCC_GPIOD_CLK_ENABLE()
	;

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC,
	VBAT_Sense_EN_Pin | DebugPin12_Pin | DebugPin16_Pin

	| SD_Power_EN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, DebugPin9_Pin | DebugPin11_Pin | DebugPin13_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */

	/*Configure GPIO pins : PCPin PCPin PCPin PCPin
	 PCPin PCPin */
	GPIO_InitStruct.Pin = DebugPin12_Pin | DebugPin16_Pin

	| SD_Power_EN_Pin | VBAT_Sense_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PAPin PAPin PAPin PAPin
	 PAPin PAPin PAPin PAPin
	 PAPin */
	GPIO_InitStruct.Pin = DebugPin9_Pin | DebugPin11_Pin | DebugPin13_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PBPin PBPin PBPin */
	GPIO_InitStruct.Pin = INT1_MCU_Pin | INT2_MCU_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = VINT_Sense_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PBPin PBPin PBPin PBPin
	 PBPin PBPin PBPin */

	/*Configure GPIO pins : PC9 PC10 PC11 */
	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = VBUS_Detect_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(VBUS_Detect_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PA11 */
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_USB_FS;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PA12 */
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PH3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}


/* bool = 1 will enable pins, bool = 0 will disable */
void DisEnableSensorPins(uint8_t bool)
{
	if (bool)
	{
		HAL_GPIO_WritePin(Temperature_EN_GPIO_Port, Temperature_EN_Pin,
				GPIO_PIN_SET);

		HAL_GPIO_WritePin(Spectrum_EN_GPIO_Port, Spectrum_EN_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(VBAT_Sense_EN_GPIO_Port, VBAT_Sense_EN_Pin,
				GPIO_PIN_SET);

		HAL_Delay(10);
	}
	else
	{
		HAL_GPIO_WritePin(Temperature_EN_GPIO_Port, Temperature_EN_Pin,
				GPIO_PIN_RESET);

		HAL_GPIO_WritePin(Spectrum_EN_GPIO_Port, Spectrum_EN_Pin,
				GPIO_PIN_RESET);

		HAL_GPIO_WritePin(VBAT_Sense_EN_GPIO_Port, VBAT_Sense_EN_Pin,
				GPIO_PIN_RESET);

		HAL_Delay(10);
	}
}

void DisEnableBatteryPin(uint8_t bool)
{
	if (bool)
	{
		HAL_GPIO_WritePin(Spectrum_EN_GPIO_Port, Spectrum_EN_Pin, GPIO_PIN_SET);

		HAL_Delay(10);
	}
	else
	{
		HAL_GPIO_WritePin(Spectrum_EN_GPIO_Port, Spectrum_EN_Pin, GPIO_PIN_RESET);

		HAL_Delay(10);
	}
}

void DisEnableRadioPin(uint8_t bool)
{
	if (bool)
	{
		HAL_GPIO_WritePin(Radio_EN_GPIO_Port, Radio_EN_Pin, GPIO_PIN_SET);

		HAL_Delay(10);
	}
	else
	{
		HAL_GPIO_WritePin(Radio_EN_GPIO_Port, Radio_EN_Pin, GPIO_PIN_RESET);

		HAL_Delay(10);
	}
}

void DisEnableSDPin(uint8_t bool)
{
	if (bool)
	{
		GPIO_InitTypeDef GPIO_InitStruct;

		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOC_CLK_ENABLE()
		;

		GPIO_InitStruct.Pin = SD_Power_EN_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		HAL_GPIO_WritePin(SD_Power_EN_GPIO_Port, SD_Power_EN_Pin, GPIO_PIN_SET);

		HAL_Delay(10);
	}
	else
	{
		HAL_GPIO_WritePin(SD_Power_EN_GPIO_Port, SD_Power_EN_Pin,
				GPIO_PIN_RESET);

		HAL_Delay(10);
	}
}

void DisEnableSaraPin(uint8_t bool)
{
	if (bool)
	{
		HAL_GPIO_WritePin(Sara_Power_On_GPIO_Port, Sara_Power_On_Pin, GPIO_PIN_SET);

		HAL_Delay(10);
	}
	else
	{
		HAL_GPIO_WritePin(Sara_Power_On_GPIO_Port, Sara_Power_On_Pin, GPIO_PIN_RESET);

		HAL_Delay(10);
	}
}


/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
