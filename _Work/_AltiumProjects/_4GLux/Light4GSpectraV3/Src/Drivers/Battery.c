/*
 * Battery.c
 *
 *  Created on: 13 Sep 2018
 *      Author: andreas.fredlund
 */

#include "Battery.h"
#include "Error.h"
#include "Global.h"

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

/* Reads the ADC */
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
		InfoLogAdd(
				ERRORLEVEL_SEVERE,
				LOG_FUNC_BATTERY,
				LOG_SUB_DATA,
				LOG_TYPE_NONE,
				LOG_STATUS_FAIL,
				0,
				0,
				0,
				GetTempInfoStruct());
		
		ErrorAddLog(LOG_FUNC_BATTERY, LOG_SUB_DATA, LOG_TYPE_NONE, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
	}

	return adc_value;
}

/*
 * Initialization of the adc.
 */
void Battery_init(void)
{
	DisEnableBatteryPin(1);

	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK)
	{
		InfoLogAdd(
				ERRORLEVEL_SEVERE,
				LOG_FUNC_BATTERY,
				LOG_SUB_INIT,
				LOG_TYPE_CALIBRATION,
				LOG_STATUS_FAIL,
				0,
				0,
				0,
				GetTempInfoStruct());

		ErrorAddLog(LOG_FUNC_BATTERY, LOG_SUB_INIT, LOG_TYPE_CALIBRATION, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
	}
	else
	{
		if (HAL_ADC_Start(&hadc1) != HAL_OK)
		{
			InfoLogAdd(
					ERRORLEVEL_SEVERE,
					LOG_FUNC_BATTERY,
					LOG_SUB_INIT,
					LOG_TYPE_START,
					LOG_STATUS_FAIL,
					0,
					0,
					0,
					GetTempInfoStruct());

			ErrorAddLog(LOG_FUNC_BATTERY, LOG_SUB_INIT, LOG_TYPE_START, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
		}
	}
}

void MX_ADC_DeInit(void)
{
	HAL_ADC_DeInit(&hadc1);
}


/* ________________________________NOT USED________________________________ */

/*
 *	100% -> 2606
 *	0% -> 2099
 *	todo . More measurements of the battery later when the Sara module is there and when we have the new pcb. Fix a more proper function
 *
 */
uint8_t Battery_GetLevel2(void)
{

	float Vbat_max = 4200;
	float Vbat_min = 3400;
	float ADC_max = 4095 * Vbat_max / 3.3;
	float ADC_min = 4095 * Vbat_min / 3.3;
	float ratioBat = 3300.0 / 4095.0;
	float bat_raw = (float) Battery_GetLevel_Raw();
	float ADCmax = Vbat_max * 0.75;
	float ADCmin = Vbat_min * 0.75;
	float milliV = bat_raw * ratioBat;
	float Percentz;
//	milliV = milliV * 4 / 3;

	printf("%d\r\n", (int) milliV);

	float batteryLevel = 100 * milliV / 3300.0;

	printf("ADCmin: %d, ADCmax: %d\r\n", (int) ADCmin, (int) ADCmax);

	printf("Percent: %d\r\n", (int) batteryLevel);

	if (milliV <= ADCmin)
	{
		Percentz = 0;
	}
	else
	{
		Percentz = (100 * (milliV - ADCmin) / (ADCmax - ADCmin));
	}

	printf("Total: %d\r\n", (int) Percentz);

	if (bat_raw < ADC_min)
	{
		bat_raw = ADC_min;
	}

	else if (bat_raw > ADC_max)
	{
		bat_raw = ADC_max;
	}

	return (uint8_t) (100 * (bat_raw - ADC_min) / (ADC_max - ADC_min));
}
