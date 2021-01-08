/*
 * _sDatasorControl.c
 *
 *  Created on: Jan 7, 2021
 *      Author: rasmu
 */

#include "__ExegerGeneric.h"
#include <_SensorControl.h>
#include <_Global.h>

#include <_AS726x.h>
#include <_Battery.h>
#include <_HDC1080.h>
#include <_LIS2DW12.h>
#include <_Tmp112.h>

#include <i2c.h>

void EnableSpectrum(uint8_t On)
{
	if(On != 0)
	{
		LL_GPIO_SetOutputPin(Spectrum_Enable_GPIO_Port, Spectrum_Enable_Pin);
		return;
	}
	LL_GPIO_ResetOutputPin(Spectrum_Enable_GPIO_Port, Spectrum_Enable_Pin);
}

void EnablesDatasors(uint8_t On)
{
	if(On != 0)
		{
			LL_GPIO_SetOutputPin(Sensor_Enable_GPIO_Port, Sensor_Enable_Pin);
			return;
		}
	LL_GPIO_ResetOutputPin(Sensor_Enable_GPIO_Port, Sensor_Enable_Pin);
}

void EnableAccelerometer(uint8_t On)
{
	if(On != 0)
		{
			LL_GPIO_SetOutputPin(Acceleromater_Enable_GPIO_Port, Acceleromater_Enable_Pin);
			return;
		}
	LL_GPIO_ResetOutputPin(Acceleromater_Enable_GPIO_Port, Acceleromater_Enable_Pin);
}

void EnableGPS(uint8_t On)
{
	if(On != 0)
	{
		LL_GPIO_SetOutputPin(GPS_Enable_GPIO_Port, GPS_Enable_Pin);
		return;
	}
	LL_GPIO_ResetOutputPin(GPS_Enable_GPIO_Port, GPS_Enable_Pin);
}

void SensorTakeMeasurement(SensorData *sD)
{
	float lux = 0;
	//Code from ChEn OPT3001
	int tmp = TMP112_ReadTemp(hi2c2);
	uint8_t tempSign = 1;
	uint8_t humidity = HDC1080_Get_Humidity(hi2c2);
	humidity = humidity / 4;

	if(tmp < 0)
	{
		tempSign = 0;
		tmp = tmp * -1;
	}

	sD->tempHumLux = (((0b1 & tempSign << 9) | (0xFF & tmp) << 8) | (0b11111 & humidity)
			| (0b11111111111111111 & (int) lux << 15));

	AS726X_takeMeasurements(AS7262_GAIN_3_7,  &(sD->spectrum));
}

