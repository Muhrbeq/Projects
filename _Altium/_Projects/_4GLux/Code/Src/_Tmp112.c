/*
 * Tmp112.c
 *
 *  Created on: 22 Nov 2018
 *      Author: andreas.fredlund
 */

//#include "Global.h"
#include <_Tmp112.h>
#include <i2c.h>
#include "__ExegerGeneric.h"
#include "_I2C.h"

void TMP112_SetState(TMP112 *tSense, TMP112_State tState)
{
	tSense->tState = tState;
}

TMP112_State TMP112_GetState(TMP112 *tSense)
{
	return tSense->tState;
}

void TMP112_InitDevice(TMP112 *device, I2C_HandleTypeDef *hi2c, uint8_t devWriteAddr, uint8_t devReadAddr)
{
	device->hi2c = *hi2c;
	device->i2cWrite = devWriteAddr;
	device->i2cRead = devReadAddr;
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

void TMP112_StateMachine(TMP112 *device)
{
	switch(device->tState)
	{
	case TMP112_INIT:
		if(TMP112_Init(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_STARTONESHOT);
			TRACE_DEBUG("TMP112 Init passed\r\n");
		}
		break;
	case TMP112_STARTONESHOT:
		if(TMP112_StartOneShot(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_WAITFORONESHOTWRITE);
			TRACE_DEBUG("TMP112_STARTONESHOT passed\r\n");
		}
		break;
	case TMP112_WAITFORONESHOTWRITE:
		if(TMP112_WaitForOneShotWrite(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_WAITFORONESHOTREAD);
			TRACE_DEBUG("TMP112_WAITFORONESHOTWRITE passed\r\n");
		}
		break;
	case TMP112_WAITFORONESHOTREAD:
		if(TMP112_WaitForOneShotRead(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_GETTEMPERATURERAWWRITE);
			TRACE_DEBUG("TMP112_WAITFORONESHOTREAD passed\r\n");
		}
		break;
	case TMP112_GETTEMPERATURERAWWRITE:
		if(TMP112_GetTemperatureRawWrite(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_GETTEMPERATURERAWREAD);
			TRACE_DEBUG("TMP112_GETTEMPERATURERAWWRITE passed\r\n");
		}
		break;
	case TMP112_GETTEMPERATURERAWREAD:
		if(TMP112_GetTemperatureRawRead(device) == HAL_OK)
		{
			TMP112_SetState(device, TMP112_CALCULATETEMPERATURE);
			TRACE_DEBUG("TMP112_GETTEMPERATURERAWREAD passed\r\n");
		}
		break;
	case TMP112_CALCULATETEMPERATURE:
		device->temperature = TMP112_CalculateTemperature(device->temperatureRaw);
		TMP112_SetState(device, TMP112_DONE);
		TRACE_DEBUG("TMP112_CALCULATETEMPERATURE passed\r\n");
		break;
	case TMP112_DONE:
		TRACE("[TMP112] - PASSED\r\n");
		break;
	default:
		TRACE("Shouldn't get here\r\n");
		break;
	}
}

/* ################ OLD FUNCTIONS ################# */

//void TMP112_Init(I2C_HandleTypeDef hi2c)
//{
//	unsigned char data_t[4] = { 0 };
//
//	/* Set TMP112 into Shutdown mode */
//	data_t[0] = TMP112_CONF_REG;
//	data_t[1] = 0x61; //0x61 = SD enable
//	data_t[2] = 0xA0; //Default values of byte 2 in configuration register
//
//	TMP112_writeRegister(hi2c, data_t, 3);
//}
//
///*
// *  Sets up one short setting
// */
//uint8_t TMP112_StartOneShot(I2C_HandleTypeDef hi2c)
//{
//	unsigned char data_t[4] = { 0 };
//
//	/* Set TMP112 into OneShot mode */
//	data_t[0] = TMP112_CONF_REG;
//	data_t[1] = 0xE1; //0xE1 = SD enable & OS enable
//	data_t[2] = 0xA0; //Default values of byte 2 in configuration register
//
//	/* Send data to sensor */
//	return TMP112_writeRegister(hi2c, data_t, 3);
//}
//
//uint8_t TMP112_WaitForOneShot(I2C_HandleTypeDef hi2c)
//{
//	unsigned char data[2] = { 0 };
//	data[0] = TMP112_CONF_REG;
//	uint8_t timeOutCounter = 25; // wait total of max 100ms (4*25)
//	uint8_t ret = 0;
//
//	while ((data[0] & 0x80) == 0 && timeOutCounter--)
//	{
//		ret = TMP112_readRegister(hi2c, data, 2);
//
//		/* If not valid ready yet, wait */
//		if ((data[0] & 0x80) == 0)
//		{
//			HAL_Delay(4);
//		}
//	}
//
//	/* SUCCESS only if everything went OK */
//	return (ret == HAL_OK && timeOutCounter > 0) ? HAL_OK : HAL_ERROR;
//}
//
///*
// * Gets the raw value from the temp register
// */
//uint8_t TMP112_GetTemperature_raw(I2C_HandleTypeDef hi2c, uint16_t *get)
//{
//	unsigned char data[2] = { 0 };
//
//	uint8_t ret = HAL_OK;
//	data[0] = TMP112_TEMP_REG;
//
//	ret = TMP112_readRegister(hi2c, data, 2);
//
//	/* 12 bit temperature */
//	*get = (((data[0] << 8) | data[1]) >> 4);
//
//	return ret;
//}
//
///*
// * Reads the raw value from the temp register.
// * Returns a 12 bit two's complement value. range: (-2047) - (2047)
// */
//uint16_t TMP112_ReadTemp_raw(I2C_HandleTypeDef hi2c)
//{
//	uint8_t ret = HAL_OK;
//	uint16_t temp = 0;
//
//	/* Set one shot */
//	ret = TMP112_StartOneShot(hi2c);
//	if (ret != HAL_OK)
//	{
////		InfoLogAdd(
////				ERRORLEVEL_SEVERE,
////				LOG_FUNC_TMP112,
////				LOG_SUB_READTEMP,
////				LOG_TYPE_WAIT_ONESHOT,
////				LOG_STATUS_FAIL,
////				0,
////				0,
////				0,
////				GetTempInfoStruct());
////
////		ErrorAddLog(LOG_FUNC_TMP112, LOG_SUB_READTEMP, LOG_TYPE_WAIT_ONESHOT, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
//	}
//	else
//	{
//		ret = TMP112_WaitForOneShot(hi2c);
//		if (ret != HAL_OK)
//		{
////			InfoLogAdd(
////					ERRORLEVEL_SEVERE,
////					LOG_FUNC_TMP112,
////					LOG_SUB_READTEMP,
////					LOG_TYPE_START_ONESHOT,
////					LOG_STATUS_FAIL,
////					0,
////					0,
////					0,
////					GetTempInfoStruct());
////
////			ErrorAddLog(LOG_FUNC_TMP112, LOG_SUB_READTEMP, LOG_TYPE_START_ONESHOT, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
//		}
//		else
//		{
//			ret = TMP112_GetTemperature_raw(hi2c, &temp);
//		}
//	}
//
//	/* If get temperature fails, send back Invalid temperature representation */
//	if (ret != HAL_OK)
//	{
//		temp = TMP112_INVALID_TEMP;
//
////		InfoLogAdd(
////				ERRORLEVEL_SEVERE,
////				LOG_FUNC_TMP112,
////				LOG_SUB_READTEMP,
////				LOG_TYPE_GET_TEMP,
////				LOG_STATUS_FAIL,
////				0,
////				temp,
////				0,
////				GetTempInfoStruct());
////
////		ErrorAddLog(LOG_FUNC_TMP112, LOG_SUB_READTEMP, LOG_TYPE_GET_TEMP, LOG_STATUS_FAIL, temp, 0, 0, GetErrorStruct());
//	}
//	return temp;
//}
//
//
//
///*
// * Reads and calculates the temperature. Multiplies by 10 means that we get a 1 decimal number.
// * Example: returns 241 means 24,1 celsius.
// * Can use 9 bits -> max value is then 51,1 degrees.
// */
//int32_t TMP112_ReadTemp(I2C_HandleTypeDef hi2c)
//{
//
//	int8_t signOfTemp = 1;
//	/* Calculate temp */
//	uint16_t raw = TMP112_ReadTemp_raw(hi2c);
//
////	if (raw == TMP112_INVALID_TEMP)
////	{
////		return TMP112_INVALID_TEMP;
////	}
//
//	uint32_t temp;
//	double tempCalc = (double) (raw & 0xFFF);
//
//	/* Check if number is negative (bit 11 == 1, raw >= 0x800). Can't represent negative values */
//	if (BitTst(raw, 11))
//	{
//
////		InfoLogAdd(
////				ERRORLEVEL_WARNING,
////				LOG_FUNC_TMP112,
////				LOG_SUB_READTEMP,
////				LOG_TYPE_GET_TEMP,
////				LOG_STATUS_MIN,
////				0,
////				raw,
////				0,
////				GetTempInfoStruct());
////
////		ErrorAddLog(LOG_FUNC_TMP112, LOG_SUB_READTEMP, LOG_TYPE_GET_TEMP, LOG_STATUS_MIN, 0, 0, 0, GetErrorStruct());
//
//		signOfTemp = -1;
//
//		int16_t tempComplement = (raw ^ 0x0FFF) + 1;
//
//		temp = (1000 * tempComplement * 0.0625);
////		return TMP112_INVALID_TEMP;
//	}
//	else
//	{
//		temp = (1000 * tempCalc * 0.0625);
//	}
//
//	/* calculated value (same as 100 * tempCalc * 0.0625)*/
////	uint8_t temp = (uint8_t) (5 * tempCalc / 16.0);
//
//	/* want to use 9 bits to represent the temp. Max of 9 bits is 511 */
//	if (temp > TMP112_MAX_TEMP)
//	{
//		temp = TMP112_MAX_TEMP;
//	}
//
//	return signOfTemp * temp;
//
//}
//
//
//
//
