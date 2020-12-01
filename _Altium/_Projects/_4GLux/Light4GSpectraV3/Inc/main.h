/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_dma.h"

#include "stm32l4xx_ll_exti.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define Heartbeat_Pin GPIO_PIN_6
#define Heartbeat_GPIO_Port GPIOC
#define DebugPin12_Pin GPIO_PIN_2
#define DebugPin12_GPIO_Port GPIOC
//#define DebugPin14_Pin GPIO_PIN_3
//#define DebugPin14_GPIO_Port GPIOC
#define USART2_CTS_Pin GPIO_PIN_0
#define USART2_CTS_GPIO_Port GPIOA
#define USART2_TX_Pin GPIO_PIN_2
#define USART2_TX_GPIO_Port GPIOA
#define USART2_RX_Pin GPIO_PIN_3
#define USART2_RX_GPIO_Port GPIOA
#define USART2_CK_Pin GPIO_PIN_4
#define USART2_CK_GPIO_Port GPIOA
#define Radio_EN_Pin GPIO_PIN_1
#define Radio_EN_GPIO_Port GPIOA
#define Sara_Power_On_Pin GPIO_PIN_6
#define Sara_Power_On_GPIO_Port GPIOA
#define Sara_Reset_Pin GPIO_PIN_7
#define Sara_Reset_GPIO_Port GPIOA
#define VINT_Sense_Pin GPIO_PIN_4
#define VINT_Sense_GPIO_Port GPIOC
#define DebugPin16_Pin GPIO_PIN_5
#define DebugPin16_GPIO_Port GPIOC
#define VBAT_Sense_Pin GPIO_PIN_3
#define VBAT_Sense_GPIO_Port GPIOC
#define PG_Pin GPIO_PIN_1
#define PG_GPIO_Port GPIOB
#define VBAT_Sense_EN_Pin GPIO_PIN_2
#define VBAT_Sense_EN_GPIO_Port GPIOC
#define INT1_MCU_Pin GPIO_PIN_12
#define INT1_MCU_GPIO_Port GPIOB
#define INT2_MCU_Pin GPIO_PIN_13
#define INT2_MCU_GPIO_Port GPIOB
#define Temperature_EN_Pin GPIO_PIN_6
#define Temperature_EN_GPIO_Port GPIOB
#define Spectrum_EN_Pin GPIO_PIN_5
#define Spectrum_EN_GPIO_Port GPIOB
//#define DebugPin18_Pin GPIO_PIN_6
//#define DebugPin18_GPIO_Port GPIOC
#define SD_Power_EN_Pin GPIO_PIN_7
#define SD_Power_EN_GPIO_Port GPIOC
#define DebugPin9_Pin GPIO_PIN_8
#define DebugPin9_GPIO_Port GPIOA
#define VBUS_Detect_Pin GPIO_PIN_9
#define VBUS_Detect_GPIO_Port GPIOA
#define DebugPin11_Pin GPIO_PIN_10
#define DebugPin11_GPIO_Port GPIOA
#define DebugPin13_Pin GPIO_PIN_15
#define DebugPin13_GPIO_Port GPIOA
#define DebugPin17_Pin GPIO_PIN_4
#define DebugPin17_GPIO_Port GPIOB
#define DebugPin19_Pin GPIO_PIN_5
#define DebugPin19_GPIO_Port GPIOB
//#define DebugPin21_Pin GPIO_PIN_6
//#define DebugPin21_GPIO_Port GPIOB
#define DebugPin22_Pin GPIO_PIN_7
#define DebugPin22_GPIO_Port GPIOB
#define PCB_Ver_Sense_Pin GPIO_PIN_1
#define PCB_Ver_Sense_GPIO_Port GPIOB
#define PCB_Ver_EN_Pin GPIO_PIN_2
#define PCB_Ver_EN_GPIO_Port GPIOB
#define Charge_Detect_Pin GPIO_PIN_15
#define Charge_Detect_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

void LED_Blink(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
