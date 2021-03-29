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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32l4xx_hal.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_dma.h"

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
#define HeartBeat_Pin LL_GPIO_PIN_13
#define HeartBeat_GPIO_Port GPIOC
#define VBAT_Sens_Enable_Pin LL_GPIO_PIN_2
#define VBAT_Sens_Enable_GPIO_Port GPIOC
#define VBAT_Sens_ADC_Pin LL_GPIO_PIN_3
#define VBAT_Sens_ADC_GPIO_Port GPIOC
#define PowerGood_Pin LL_GPIO_PIN_0
#define PowerGood_GPIO_Port GPIOA
#define Solar_Detect_Pin LL_GPIO_PIN_1
#define Solar_Detect_GPIO_Port GPIOA
#define Bluetooth_Button_Pin LL_GPIO_PIN_4
#define Bluetooth_Button_GPIO_Port GPIOA
#define Bluetooth_Button_EXTI_IRQn EXTI4_IRQn
#define ChangeMode_Button_Pin LL_GPIO_PIN_5
#define ChangeMode_Button_GPIO_Port GPIOA
#define ChangeMode_Button_EXTI_IRQn EXTI9_5_IRQn
#define Action_Button_Pin LL_GPIO_PIN_6
#define Action_Button_GPIO_Port GPIOA
#define Action_Button_EXTI_IRQn EXTI9_5_IRQn
#define LowPowerLedTIM_Pin LL_GPIO_PIN_7
#define LowPowerLedTIM_GPIO_Port GPIOA
#define INT2_Gyro_Pin LL_GPIO_PIN_4
#define INT2_Gyro_GPIO_Port GPIOC
#define INT_Gyro_Pin LL_GPIO_PIN_5
#define INT_Gyro_GPIO_Port GPIOC
#define INT2_ACC_Pin LL_GPIO_PIN_0
#define INT2_ACC_GPIO_Port GPIOB
#define INT_ACC_Pin LL_GPIO_PIN_1
#define INT_ACC_GPIO_Port GPIOB
#define Sens2_Enable_Pin LL_GPIO_PIN_2
#define Sens2_Enable_GPIO_Port GPIOB
#define Sens2_Led_Pin LL_GPIO_PIN_13
#define Sens2_Led_GPIO_Port GPIOB
#define USB_Led_Pin LL_GPIO_PIN_14
#define USB_Led_GPIO_Port GPIOB
#define Bluetooth_Led_Pin LL_GPIO_PIN_15
#define Bluetooth_Led_GPIO_Port GPIOB
#define SD_Led_Pin LL_GPIO_PIN_6
#define SD_Led_GPIO_Port GPIOC
#define debugLed_Pin LL_GPIO_PIN_7
#define debugLed_GPIO_Port GPIOC
#define Bluetooth_Enable_Pin LL_GPIO_PIN_8
#define Bluetooth_Enable_GPIO_Port GPIOA
#define SD_Enable_Pin LL_GPIO_PIN_15
#define SD_Enable_GPIO_Port GPIOA
#define SD_Detect_Pin LL_GPIO_PIN_4
#define SD_Detect_GPIO_Port GPIOB
#define Sens1_Enable_Pin LL_GPIO_PIN_5
#define Sens1_Enable_GPIO_Port GPIOB
#define Sens1_Led_Pin LL_GPIO_PIN_8
#define Sens1_Led_GPIO_Port GPIOB
#define Blink_Sequence_EXTI_Pin LL_GPIO_PIN_9
#define Blink_Sequence_EXTI_GPIO_Port GPIOB
#define Blink_Sequence_EXTI_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
