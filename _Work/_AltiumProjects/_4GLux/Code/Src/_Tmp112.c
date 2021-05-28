/*
 * _Tmp112.c
 *
 *  Created on: Apr 1, 2021
 *      Author: rasmus.muhrbeck
 */

//#include "Global.h"
#include <_Tmp112.h>
#include <i2c.h>
#include "__ExegerGeneric.h"
#include "_I2C.h"

static void TMP112_SetState(TMP112 *tSense, TMP112_State tState)
{
	tSense->tState = tState;
}


void TMP112_InitDevice(TMP112 *device, I2C_HandleTypeDef *hi2c, uint8_t devWriteAddr, uint8_t devReadAddr)
{
	device->hi2c = hi2c;
	device->i2cWrite = devWriteAddr;
	device->i2cRead = devReadAddr;
	device->errorCounter = 0;
	device->sStatus = SENSORSTATUS_FAIL;
}

static HAL_StatusTypeDef TMP112_Init(TMP112 *device)
{
	unsigned char data_t[4] = { 0 };

	/* Set TMP112 into Shutdown mode */
	data_t[0] = TMP112_CONF_REG;
	data_t[1] = 0x61; //0x61 = SD enable
	data_t[2] = 0xA0; //Default values of byte 2 in configuration register

	return I2C_Write(device->hi2c, device->i2cWrite, data_t, 3);
}

static HAL_StatusTypeDef TMP112_StartOneShot(TMP112 *device)
{
	unsigned char data_t[4] = { 0 };

	/* Set TMP112 into OneShot mode */
	data_t[0] = TMP112_CONF_REG;
	data_t[1] = 0xE1; //0xE1 = SD enable & OS enable
	data_t[2] = 0xA0; //Default values of byte 2 in configuration register

	/* Send data to sensor */
	return I2C_Write(device->hi2c, device->i2cWrite, data_t, 3);
}

static HAL_StatusTypeDef TMP112_WaitForOneShotWrite(TMP112 *device)
{
	unsigned char data[2] = { 0 };
	data[0] = TMP112_CONF_REG;
	return I2C_ReadWrite(device->hi2c, device->i2cWrite, data, 2);
}

static HAL_StatusTypeDef TMP112_WaitForOneShotRead(TMP112 *device)
{
	unsigned char data[2] = { 0 };
	data[0] = TMP112_CONF_REG;
	if(I2C_ReadRead(device->hi2c, device->i2cRead, data, 2) == HAL_OK)
	{
		if ((data[0] & 0x80) != 0)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static HAL_StatusTypeDef TMP112_GetTemperatureRawWrite(TMP112 *device)
{
	unsigned char data[2] = { 0 };

	data[0] = TMP112_TEMP_REG;

	return I2C_ReadWrite(device->hi2c, device->i2cWrite, data, 2);
}

static HAL_StatusTypeDef TMP112_GetTemperatureRawRead(TMP112 *device)
{
	unsigned char data[2] = { 0 };

	data[0] = TMP112_TEMP_REG;

	if(I2C_ReadRead(device->hi2c, device->i2cRead, data, 2) == HAL_OK)
	{
		device->temperatureRaw = (((data[0] << 8) | data[1]) >> 4);
		return HAL_OK;
	}
	return HAL_ERROR;
}

static int32_t TMP112_CalculateTemperature(uint16_t raw)
{
	int8_t signOfTemp = 1;

	uint32_t temp;
	double tempCalc = (double) (raw & 0xFFF);

	/* Check if number is negative (bit 11 == 1, raw >= 0x800). */
	if (BitTst(raw, 11))
	{
		signOfTemp = -1;

		int16_t tempComplement = (raw ^ 0x0FFF) + 1;

		temp = (1000 * tempComplement * 0.0625);
	}
	else
	{
		temp = (1000 * tempCalc * 0.0625);
	}

	/* want to use 9 bits to represent the temp. Max of 9 bits is 511 */
	if (temp > TMP112_MAX_TEMP)
	{
		temp = TMP112_MAX_TEMP;
	}

	return signOfTemp * temp;
}
/*
 * Add errorhandling to statemachine
 * */
void TMP112_StateMachine(TMP112 *device)
{
	if(device->errorCounter > 100)
	{
		/* Error occur */
		TRACE("[TMP112] - FAILED\r\n");
		device->errorCounter = 0;
		device->sStatus = SENSORSTATUS_FAIL;
		TMP112_SetState(device, TMP112_ERROR);
	}

	switch(device->tState)
	{
	case TMP112_INIT:
		if(TMP112_Init(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_STARTONESHOT);
			TRACE_DEBUG("TMP112 Init passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case TMP112_STARTONESHOT:
		if(TMP112_StartOneShot(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_WAITFORONESHOTWRITE);
			TRACE_DEBUG("TMP112_STARTONESHOT passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case TMP112_WAITFORONESHOTWRITE:
		if(TMP112_WaitForOneShotWrite(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_WAITFORONESHOTREAD);
			TRACE_DEBUG("TMP112_WAITFORONESHOTWRITE passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case TMP112_WAITFORONESHOTREAD:
		if(TMP112_WaitForOneShotRead(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_GETTEMPERATURERAWWRITE);
			TRACE_DEBUG("TMP112_WAITFORONESHOTREAD passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case TMP112_GETTEMPERATURERAWWRITE:
		if(TMP112_GetTemperatureRawWrite(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_GETTEMPERATURERAWREAD);
			TRACE_DEBUG("TMP112_GETTEMPERATURERAWWRITE passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case TMP112_GETTEMPERATURERAWREAD:
		if(TMP112_GetTemperatureRawRead(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_CALCULATETEMPERATURE);
			TRACE_DEBUG("TMP112_GETTEMPERATURERAWREAD passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case TMP112_CALCULATETEMPERATURE:
		device->temperature = TMP112_CalculateTemperature(device->temperatureRaw);
		TMP112_SetState(device, TMP112_DONE);
		TRACE_DEBUG("TMP112_CALCULATETEMPERATURE passed\r\n");
		break;
	case TMP112_DONE:
		device->errorCounter = 0;
		device->sStatus = SENSORSTATUS_PASS;
		TMP112_SetState(device, TMP112_IDLE);
		TRACE("[TMP112] - PASSED\r\n");
		break;
	default:
		TRACE("Shouldn't get here\r\n");
		device->errorCounter = 0;
		break;
	}
}
