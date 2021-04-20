/*
 * _HDC1080.c
 *
 *  Created on: Apr 1, 2021
 *      Author: Rasmus.Muhrbeck
 */

#include "__ExegerGeneric.h"
#include "_HDC1080.h"
#include "_I2C.h"

/* Sets state of sensor */
static void HDC1080_SetState(HDC1080 *sSense, HDC1080_State sState)
{
	sSense->sState = sState;
}

/* Init device with read/write addr and i2c channel */
void HDC1080_InitDevice(HDC1080 *device, I2C_HandleTypeDef *hi2c, uint8_t devWriteAddr, uint8_t devReadAddr)
{
	device->hi2c = *hi2c;
	device->i2cWrite = devWriteAddr;
	device->i2cRead = devReadAddr;
	device->errorCounter = 0;
	device->sStatus = SENSORSTATUS_FAIL;
}

/* Sends write command for init */
static HAL_StatusTypeDef HDC1080_InitWrite(HDC1080 *device)
{
	unsigned char data[4] = { 0 };
	data[0] = HDC1080_CONF_REG;
	return I2C_ReadWrite(device->hi2c, device->i2cWrite, data, 2);
}

/* Reads back the Init command from sensor */
static HAL_StatusTypeDef HDC1080_InitRead(HDC1080 *device)
{
	unsigned char data[2] = { 0 };
	data[0] = HDC1080_CONF_REG;

	if(I2C_ReadRead(device->hi2c, device->i2cRead, data, 2) == HAL_OK)
	{
		device->confReg = (data[0] << 8) | data[1];
		return HAL_OK;
	}
	return HAL_ERROR;
}

/* Configure register for sensor */
static HAL_StatusTypeDef HDC1080_ConfigureRegister(HDC1080 *device)
{
	unsigned char data[4] = { 0 };
	device->confReg |= 1 << 13;		//	heater disabled 0 ; enable 1
	device->confReg |= 0 << 12;		//	Temperature or Humidity is acquired 0 ; Temperature and Humidity is acquired 1.
	device->confReg |= 0 << 10;		//	Temp Resolution. 0 = 14 bit	;	1 = 11 bit
	device->confReg |= 0 << 9;		//	Hum Resolution. bit [9:8]. 00 = 14 bit ; 01 = 11 bit ; 10 = 8 bit
	device->confReg |= 0 << 8;

	data[0] = HDC1080_CONF_REG;
	data[1] = (uint8_t) ((device->confReg >> 8) & 0xFF);  	//	msb
	data[2] = (uint8_t) (device->confReg & 0xFF);  			//	lsb

	return  I2C_ReadWrite(device->hi2c, device->i2cWrite, data, 3);
}

/* Write to sensor to aquire ID */
static HAL_StatusTypeDef HDC1080_GetDeviceIDWrite(HDC1080 *device)
{
	unsigned char data[2] = { 0 };
	data[0] = HDC1080_DEV_ID_REG;
	return I2C_ReadWrite(device->hi2c, device->i2cWrite, data, 2);
}

/* Read ID from sensor */
static HAL_StatusTypeDef HDC1080_GetDeviceIDRead(HDC1080 *device)
{
	unsigned char data[2] = { 0 };
	data[0] = HDC1080_DEV_ID_REG;

	if(I2C_ReadRead(device->hi2c, device->i2cRead, data, 2) == HAL_OK)
	{
		device->deviceID = (data[0] << 8) | data[1];
		return HAL_OK;
	}
	return HAL_ERROR;
}

static HAL_StatusTypeDef HDC1080_GetHumidityRawWrite(HDC1080 *device)
{
	unsigned char data[2] = { 0 };
	data[0] = HDC1080_HUM_REG;
	return I2C_ReadWrite(device->hi2c, device->i2cWrite, data, 2);
}

static HAL_StatusTypeDef HDC1080_GetHumidityRawRead(HDC1080 *device)
{
	unsigned char data[2] = { 0 };
	data[0] = HDC1080_HUM_REG;

	if(I2C_ReadRead(device->hi2c, device->i2cRead, data, 2) == HAL_OK)
	{
		device->humidityRaw = (data[0] << 8) | data[1];
		return HAL_OK;
	}
	return HAL_ERROR;
}

static uint8_t HDC1080_CalculateHumidity(HDC1080 *device)
{
	float hum = 100.0 * (float) device->humidityRaw / 65536.0;

	if (hum > 100)
	{
		hum = 100;
	}
	else if (hum < 0)
	{
		hum = 0;
	}

	return (uint8_t) hum;
}

/* State machine to optimize time in each sector */
void HDC1080_StateMachine(HDC1080 *device)
{

	if(device->errorCounter > 100)
	{
		/* Error occur */
		TRACE("[HDC1080] - FAILED\r\n");
		device->errorCounter = 0;
		device->sStatus = SENSORSTATUS_FAIL;
		HDC1080_SetState(device, HDC1080_ERROR);
	}

	switch(device->sState)
	{
	case HDC1080_GETIDWRITE:
		if(HDC1080_GetDeviceIDWrite(device) == HAL_OK)
		{
			HDC1080_SetState(device, HDC1080_GETIDREAD);
			TRACE_DEBUG("DevID: write - Passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case HDC1080_GETIDREAD:
		if(HDC1080_GetDeviceIDRead(device) == HAL_OK)
		{
			HDC1080_SetState(device, HDC1080_DONE);
			TRACE_DEBUG("DevID: Read - Passed\r\n");D
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case HDC1080_INITWRITE:
		if(HDC1080_InitWrite(device) == HAL_OK)
		{
			HDC1080_SetState(device, HDC1080_INITREAD);
			TRACE_DEBUG("Init: write - Passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case HDC1080_INITREAD:
		if(HDC1080_InitRead(device) == HAL_OK)
		{
			HDC1080_SetState(device, HDC1080_CONFIGUREREGISTER);
			TRACE_DEBUG("Init: Read - Passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case HDC1080_CONFIGUREREGISTER:
		if(HDC1080_ConfigureRegister(device) == HAL_OK)
		{
			HDC1080_SetState(device, HDC1080_GETHUMIDITYRAWWRITE);
			TRACE_DEBUG("Configure Register - Passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case HDC1080_GETHUMIDITYRAWWRITE:
		if(HDC1080_GetHumidityRawWrite(device) == HAL_OK)
		{
			HDC1080_SetState(device, HDC1080_GETHUMIDITYRAWREAD);
			TRACE_DEBUG("Get Humidity raw: write - Passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case HDC1080_GETHUMIDITYRAWREAD:
		if(HDC1080_GetHumidityRawRead(device) == HAL_OK)
		{
			HDC1080_SetState(device, HDC1080_CALCULATEHUMIDITY);
			TRACE_DEBUG("Get humidity raw: Read - Passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case HDC1080_CALCULATEHUMIDITY:
		if(HDC1080_CalculateHumidity(device) == HAL_OK)
		{
			HDC1080_SetState(device, HDC1080_DONE);
			device->sStatus = SENSORSTATUS_PASS;
			TRACE_DEBUG("Calculate Humidity - Passed\r\n");
		}
		else
		{
			device->errorCounter++;
		}
		break;
	case HDC1080_DONE:
			device->errorCounter = 0;
			device->sStatus = SENSORSTATUS_PASS;
			TRACE("[HDC1080] - PASSED\r\n");
			HDC1080_SetState(device, HDC1080_IDLE);
		break;
	default:
			device->errorCounter = 0;
			TRACE("[HDC1080] - ERROR\r\n");
		break;
	}
}

