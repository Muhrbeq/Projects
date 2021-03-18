/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_crs.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HeartBeat_Pin GPIO_PIN_13
#define HeartBeat_GPIO_Port GPIOC
#define VBat_ADC_Pin GPIO_PIN_0
#define VBat_ADC_GPIO_Port GPIOC
#define BLE_En_Pin GPIO_PIN_3
#define BLE_En_GPIO_Port GPIOC
#define WKUP_EXTI0_Pin GPIO_PIN_0
#define WKUP_EXTI0_GPIO_Port GPIOA
#define WKUP_EXTI0_EXTI_IRQn EXTI0_IRQn
#define BLE_TX_Pin GPIO_PIN_2
#define BLE_TX_GPIO_Port GPIOA
#define BLE_RX_Pin GPIO_PIN_3
#define BLE_RX_GPIO_Port GPIOA
#define BLE_EXTI5_Pin GPIO_PIN_5
#define BLE_EXTI5_GPIO_Port GPIOA
#define BLE_EXTI5_EXTI_IRQn EXTI9_5_IRQn
#define Light_Setting_EXTI6_Pin GPIO_PIN_6
#define Light_Setting_EXTI6_GPIO_Port GPIOA
#define Light_Setting_EXTI6_EXTI_IRQn EXTI9_5_IRQn
#define VBat_Level_EXTI7_Pin GPIO_PIN_7
#define VBat_Level_EXTI7_GPIO_Port GPIOA
#define VBat_Level_EXTI7_EXTI_IRQn EXTI9_5_IRQn
#define PC_TX_Pin GPIO_PIN_9
#define PC_TX_GPIO_Port GPIOA
#define PC_RX_Pin GPIO_PIN_10
#define PC_RX_GPIO_Port GPIOA
#define Neo_Tim2_Ch1_Pin GPIO_PIN_15
#define Neo_Tim2_Ch1_GPIO_Port GPIOA
#define Neo_En_Pin GPIO_PIN_5
#define Neo_En_GPIO_Port GPIOB
#define VBat_Sense_En_Pin GPIO_PIN_7
#define VBat_Sense_En_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
