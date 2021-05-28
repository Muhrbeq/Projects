/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_dma.h"

#include "stm32l4xx_ll_exti.h"

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
#define Spectrum_Enable_Pin GPIO_PIN_13
#define Spectrum_Enable_GPIO_Port GPIOC
#define LED_Green_Pin GPIO_PIN_0
#define LED_Green_GPIO_Port GPIOC
#define LED_Red_Pin GPIO_PIN_1
#define LED_Red_GPIO_Port GPIOC
#define Revision_Enable_Pin GPIO_PIN_2
#define Revision_Enable_GPIO_Port GPIOC
#define ADC_Revision_Pin GPIO_PIN_3
#define ADC_Revision_GPIO_Port GPIOC
#define SendData_EXTI0_Pin GPIO_PIN_0
#define SendData_EXTI0_GPIO_Port GPIOA
#define SendData_EXTI0_EXTI_IRQn EXTI0_IRQn
#define Radio_NetStatus_Pin GPIO_PIN_1
#define Radio_NetStatus_GPIO_Port GPIOA
#define Radio_Enable_Pin GPIO_PIN_4
#define Radio_Enable_GPIO_Port GPIOA
#define Sara_Reset_Pin GPIO_PIN_5
#define Sara_Reset_GPIO_Port GPIOA
#define Sara_Power_On_Pin GPIO_PIN_6
#define Sara_Power_On_GPIO_Port GPIOA
#define SD_Detect_Pin GPIO_PIN_7
#define SD_Detect_GPIO_Port GPIOA
#define Screen_Enable_Pin GPIO_PIN_1
#define Screen_Enable_GPIO_Port GPIOB
#define Acceleromater_Enable_Pin GPIO_PIN_2
#define Acceleromater_Enable_GPIO_Port GPIOB
#define SPI_CS_Pin GPIO_PIN_12
#define SPI_CS_GPIO_Port GPIOB
#define Screen_DC_Pin GPIO_PIN_6
#define Screen_DC_GPIO_Port GPIOC
#define Screen_RST_Pin GPIO_PIN_7
#define Screen_RST_GPIO_Port GPIOC
#define Screen_Busy_Pin GPIO_PIN_8
#define Screen_Busy_GPIO_Port GPIOA
#define USART1_TX_PC_Pin GPIO_PIN_9
#define USART1_TX_PC_GPIO_Port GPIOA
#define USART1_RX_PC_Pin GPIO_PIN_10
#define USART1_RX_PC_GPIO_Port GPIOA
#define SD_Enable_Pin GPIO_PIN_15
#define SD_Enable_GPIO_Port GPIOA
#define VBUS_Detect_EXTI4_Pin GPIO_PIN_4
#define VBUS_Detect_EXTI4_GPIO_Port GPIOB
#define VBUS_Detect_EXTI4_EXTI_IRQn EXTI4_IRQn
#define Sensor_Enable_Pin GPIO_PIN_5
#define Sensor_Enable_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
