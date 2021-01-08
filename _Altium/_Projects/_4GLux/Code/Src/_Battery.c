/*
 * Battery.c
 *
 *  Created on: 31 aug. 2020
 *      Author: Rasmus.Muhrbeck
 */

#include <_Battery.h>
#include <adc.h>

void EnableBatterySensePin(void)
{
	HAL_GPIO_WritePin(VBat_Sense_Enable_GPIO_Port, VBat_Sense_Enable_Pin, GPIO_PIN_SET);
}

void DisableBatterySensePin(void)
{
	HAL_GPIO_WritePin(VBat_Sense_Enable_GPIO_Port, VBat_Sense_Enable_Pin, GPIO_PIN_RESET);
}

uint8_t BatteryMeasure_Init(void)
{
	//Enable the GPIO pin
	EnableBatterySensePin();

	HAL_Delay(10);

	if(HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) == HAL_OK)
	{
		if(HAL_ADC_Start(&hadc1) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

uint16_t Battery_GetLevel_Raw(void)
{
	__IO uint16_t adc_value = 0;

	HAL_ADC_PollForConversion(&hadc1, 10);

	/* Check if the continuous conversion of regular channel is finished */
	if ((HAL_ADC_GetState(&hadc1) & HAL_ADC_STATE_REG_EOC)
			== HAL_ADC_STATE_REG_EOC)
	{
		/* Get the converted value of regular channel */
		adc_value = HAL_ADC_GetValue(&hadc1);
	}
	else
	{
		return 0;
	}

	return adc_value;
}

/* Get level of battery */
uint8_t Battery_GetLevel(void)
{
	/* How much is one ADC bit worth in Volt? */
	float ratioBat = V3V3MAX / (ADC_12_BIT * 1.0);

	/* Get Raw value from ADC */
	float bat_raw = (float) Battery_GetLevel_Raw();

	/* Min/Max values + how many milliV that is calculated */
	float ADCmax = VBAT_MAX * VOLTAGE_DIVIDER;
	float ADCmin = VBAT_MIN * VOLTAGE_DIVIDER;
	float milliV = bat_raw * ratioBat;

	/* Safety check */
	if (bat_raw < ADCmin)
	{
		bat_raw = ADCmin;
	}

	/* Stafety check */
	else if (bat_raw > ADCmax)
	{
		bat_raw = ADCmax;
	}


	uint8_t vBatPercent = (100 * (milliV - ADCmin) / (ADCmax - ADCmin));

	/* Overflow safety, can max be 100% */
	if ((vBatPercent >= 100) && (milliV > ADCmin))
	{
		vBatPercent = 100;
	}

	/* Underflow safety, can max be 0 % */
	if (milliV < ADCmin)
	{
		vBatPercent = 0;
	}



	return vBatPercent;
}
