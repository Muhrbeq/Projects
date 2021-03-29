/*
 * PowerManage.c
 *
 *  Created on: 27 mars 2020
 *      Author: rasmu
 */

#include "main.h"
#include "PowerManage.h"
#include "gpio.h"

void SystemPower_Config(void)
{

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE()
	;

	/* Enable Ultra low power mode */
//	HAL_PWREx_EnableUltraLowPower();
//
//	/* Enable the fast wake up from Ultra low power mode */
//	HAL_PWREx_EnableFastWakeUp();

}

void SystemSleepAndWakeup(void)
{
//	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	HAL_GPIO_WritePin(Enable_Neo_GPIO_Port, Enable_Neo_Pin, GPIO_PIN_RESET);

	HeartBeatOff();

	/* Go to sleep */
	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

//	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
//	__HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
//
//	/* Enable SysTick interrupts */
//
//	/* Stop RTC timer to generate interrupt */
//	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	SystemClock_Config();

	MX_GPIO_Init();

	HeartBeatOff();

	HAL_GPIO_WritePin(Enable_Neo_GPIO_Port, Enable_Neo_Pin, GPIO_PIN_SET);
}

void SystemSHUTDOWN(void)
{
	/* Disable all used wakeup sources: WKUP pin */
	    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1_LOW);

	    /* Clear wake up Flag */
	    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);

	    /* Enable wakeup pin WKUP2 */
	    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_LOW);

	    /* Set RTC back-up register RTC_BKP31R to indicate
	       later on that system has entered shutdown mode  */
	    WRITE_REG( RTC->BKP31R, 0x1 );
	    /* Enter shutdown mode */
	    HAL_PWREx_EnterSHUTDOWNMode();
}

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  Error_Handler();
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_MSI_Enable();

   /* Wait till MSI is ready */
  while(LL_RCC_MSI_IsReady() != 1)
  {

  }
  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
  LL_RCC_MSI_SetCalibTrimming(0);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 20, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_PLLSAI1_ConfigDomain_ADC(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 16, LL_RCC_PLLSAI1R_DIV_2);
  LL_RCC_PLLSAI1_EnableDomain_ADC();
  LL_RCC_PLLSAI1_Enable();

   /* Wait till PLLSAI1 is ready */
  while(LL_RCC_PLLSAI1_IsReady() != 1)
  {

  }
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_SetSystemCoreClock(40000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  };
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C3_CLKSOURCE_PCLK1);
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLLSAI1);
}
