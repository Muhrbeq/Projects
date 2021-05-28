/**
  ******************************************************************************
  * @file    iwdg.c
  * @brief   This file provides code for the configuration
  *          of the IWDG instances.
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

/* Includes ------------------------------------------------------------------*/
#include "iwdg.h"

/* USER CODE BEGIN 0 */
#include "__ExegerGeneric.h"
/* USER CODE END 0 */

IWDG_HandleTypeDef hiwdg;

/* IWDG init function */
void MX_IWDG_Init(void)
{

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USER CODE BEGIN 1 */
void ResetIWDG(void)
{
	IWDG->KR = 0x0000AAAA;
}

void Program_IWDG_Freeze_Shutdown(void)
{
	FLASH_OBProgramInitTypeDef myFlashOB_Program;
	myFlashOB_Program.OptionType = OPTIONBYTE_USER;
	myFlashOB_Program.USERType = OB_USER_IWDG_STOP;
	myFlashOB_Program.USERConfig = 0;
	myFlashOB_Program.RDPLevel = 0xAA;

	printf("----------** FLASH Program **----------\r\n");

	if (HAL_FLASH_Unlock() == HAL_OK)
	{
		printf("FLASH Unlocked\r\n");
	}

	if (HAL_FLASH_OB_Unlock() == HAL_OK)
	{
		printf("OB_Bits unlocked\r\n");
	}

	if (HAL_FLASHEx_OBProgram(&myFlashOB_Program) == HAL_OK)
	{
		printf("OB_Bit programmed\r\n");
	}

	HAL_FLASH_Lock();

	printf("FLASH Locked\r\n");
	printf("-------------------------\r\n");
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
