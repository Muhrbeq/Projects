/*
 * _OPT3001.c
 *
 *  Created on: 14 apr. 2021
 *      Author: Rasmus.Muhrbeck
 */


#include "stm32l4xx_hal.h"
#include <math.h>
#include <stdbool.h>

#include "__ExegerGeneric.h"

#include "_OPT3001.h"
#include "_I2C.h"

void OPT3001_SetState(OPT3001 *device, OPT3001_State oState)
{
	device->tState = oState;
}

/* Init device with read/write addr and i2c channel */
void OPT3001_InitDevice(OPT3001 *device, I2C_HandleTypeDef *hi2c, uint8_t devWriteAddr, uint8_t devReadAddr)
{
	device->hi2c = hi2c;
	device->i2cWrite = devWriteAddr;
	device->i2cRead = devReadAddr;
	device->errorCounter = 0;
	device->sStatus = SENSORSTATUS_FAIL;
	device->tState = OPT3001_IDWRITE;
}

/* Send the initial commands */
static uint8_t OPT3001_Init(OPT3001 *device)
{
	unsigned char buffer[3];
	buffer[0]=0x01; //buffer
	buffer[1]=0xC2; //MSB byte
	buffer[2]=0x10; //LSB byte

	if(I2C_Write(device->hi2c, device->i2cWrite, buffer, 3) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

uint8_t OPT3001_GetDeviceIDWrite(OPT3001 *device)
{
	uint8_t id_reg = 0x7F;

	if(I2C_ReadWrite(device->hi2c, device->i2cWrite, &id_reg, 3) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

uint8_t OPT3001_GetDeviceIDRead(OPT3001 *device)
{
	unsigned char buffer[3] = {0};

	if(I2C_ReadRead(device->hi2c, device->i2cRead, buffer, 3) == HAL_OK)
	{
		uint16_t idResult = (((uint16_t)buffer[0]) << 8) | buffer[1];
		if(idResult == 0x3001)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

/* Shut the sensor down to low power mode */
static uint8_t OPT3001_Shutdown(OPT3001 *device)
{
	unsigned char buffer[3];
	buffer[0]=0x01; //buffer
	buffer[1]=0xCA; //MSB byte
	buffer[2]=0x10; //LSB byte

	if(I2C_Write(device->hi2c, device->i2cWrite, buffer, 3) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

static uint8_t OPT3001_RequestReading(OPT3001 *device)
{
	uint8_t id_reg = 0x00;

	if(I2C_Write(device->hi2c, device->i2cWrite, &id_reg, 1) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

/* Check if reading is ready at device */
static uint8_t OPT3001_ReadingReadyWrite(OPT3001 *device)
{
	uint8_t id_reg = 0x01;
	if(I2C_ReadWrite(device->hi2c, device->i2cWrite, &id_reg, 1) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

static uint8_t OPT3001_ReadingReadyRead(OPT3001 *device)
{
	uint16_t rawL;
	uint8_t buffer[2] = {0,0};

	if(I2C_ReadRead(device->hi2c, device->i2cRead, buffer, 2) == HAL_OK)
	{
		rawL = (((uint16_t)buffer[0]) << 8) | buffer[1];
		if ((rawL & 0x80) == 128)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

/* Get the current lux from the sensor */
static uint8_t OPT3001_GetLuxWrite(OPT3001 *device)
{
	uint8_t id_reg = 0x00;
	if(I2C_Write(device->hi2c, device->i2cWrite, &id_reg, 1) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

/* Get the current lux from the sensor */
static uint8_t OPT3001_GetLuxRead(OPT3001 *device)
{
	uint16_t response;
	uint8_t buffer[2] = {0,0};

	if(I2C_ReadRead(device->hi2c, device->i2cRead, buffer, 2) == HAL_OK)
	{
		response = (((uint16_t)buffer[0]) << 8) | buffer[1];
		uint16_t iExponent, iMantissa;
		iMantissa = response & 0x0FFF;                 // Extract Mantissa
		iExponent = (response & 0xF000) >> 12;         // Extract Exponent
		device->currentLux = iMantissa * (0.01 * pow(2, iExponent));
		return HAL_OK;

	}
	return HAL_ERROR;
}

void OPT3001_StateMachine(OPT3001 *device)
{
	if(device->errorCounter > 100)
	{
		/* Error occur */
		TRACE("[OPT3001] - FAILED\r\n");
		device->errorCounter = 0;
		device->sStatus = SENSORSTATUS_FAIL;
		OPT3001_SetState(device, OPT3001_ERROR);
	}

	switch(device->tState)
	{
	case OPT3001_INITIATE:
	{
		if(OPT3001_Init(device) == HAL_OK)
		{
			TRACE_DEBUG("[OPT3001] - Init Success\r\n");
			OPT3001_SetState(device, OPT3001_REQUESTREADING);
		}
		else
		{
			device->errorCounter++;
			TRACE_DEBUG("[OPT3001] -  OPT3001_INITIATE ERROR\r\n");
		}
		break;
	}
	case OPT3001_IDWRITE:
	{
		if(OPT3001_GetDeviceIDWrite(device) == HAL_OK)
		{
			TRACE_DEBUG("[OPT3001] - ID Write Success\r\n");
			OPT3001_SetState(device, OPT3001_IDREAD);
		}
		else
		{
			TRACE_DEBUG("[OPT3001] -  OPT3001_IDWRITE ERROR\r\n");
			device->errorCounter++;
		}
		break;
	}
	case OPT3001_IDREAD:
	{
		if(OPT3001_GetDeviceIDRead(device) == HAL_OK)
		{
			TRACE_DEBUG("[OPT3001] - ID Read Success\r\n");
			OPT3001_SetState(device, OPT3001_DONE);
		}
		else
		{
			TRACE_DEBUG("[OPT3001] -  OPT3001_IDREAD ERROR\r\n");
			device->errorCounter++;
		}
		break;
	}
	case OPT3001_REQUESTREADING:
	{
		if(OPT3001_RequestReading(device) == HAL_OK)
		{
			TRACE_DEBUG("[OPT3001] - Request Reading Success\r\n");
			OPT3001_SetState(device, OPT3001_READINGREADYWRITE);
		}
		else
		{
			TRACE_DEBUG("[OPT3001] -  OPT3001_REQUESTREADING ERROR\r\n");
			device->errorCounter++;
		}
		break;
	}
	case OPT3001_READINGREADYWRITE:
	{
		if(OPT3001_ReadingReadyWrite(device) == HAL_OK)
		{
			TRACE_DEBUG("[OPT3001] - Reading Ready Write Success\r\n");
			OPT3001_SetState(device, OPT3001_READINGREADYREAD);
		}
		else
		{
			TRACE_DEBUG("[OPT3001] -  OPT3001_READINGREADYWRITE ERROR\r\n");
			device->errorCounter++;
		}
		break;
	}
	case OPT3001_READINGREADYREAD:
	{
		if(OPT3001_ReadingReadyRead(device) == HAL_OK)
		{
			TRACE_DEBUG("[OPT3001] - Reading Ready Read Success\r\n");
			OPT3001_SetState(device, OPT3001_GETLUXWRITE);
		}
		else
		{
			TRACE_DEBUG("[OPT3001] -  OPT3001_READINGREADYREAD ERROR\r\n");
			device->errorCounter++;
		}
		break;
	}
	case OPT3001_GETLUXWRITE:
	{
		if(OPT3001_GetLuxWrite(device) == HAL_OK)
		{
			TRACE_DEBUG("[OPT3001] - Get Lux Write Success\r\n");
			OPT3001_SetState(device, OPT3001_GETLUXREAD);
		}
		else
		{
			TRACE_DEBUG("[OPT3001] -  OPT3001_GETLUXWRITE ERROR\r\n");
			device->errorCounter++;
		}
		break;
	}
	case OPT3001_GETLUXREAD:
	{
		if(OPT3001_GetLuxRead(device) == HAL_OK)
		{
			TRACE_DEBUG("[OPT3001] - Get Lux Read Success\r\n");
			OPT3001_SetState(device, OPT3001_SHUTDOWN);
		}
		else
		{
			TRACE_DEBUG("[OPT3001] -  OPT3001_GETLUXREAD ERROR\r\n");
			device->errorCounter++;
		}
		break;
	}
	case OPT3001_SHUTDOWN:
	{
		if(OPT3001_Shutdown(device) == HAL_OK)
		{
			TRACE_DEBUG("[OPT3001] - Shutdown Success\r\n");
			OPT3001_SetState(device, OPT3001_DONE);
		}
		else
		{
			device->errorCounter++;
		}
		break;
	}
	case OPT3001_DONE:
	{
		TRACE("[OPT3001] - Passed\r\n");
		device->errorCounter = 0;
		device->sStatus = SENSORSTATUS_PASS;
		OPT3001_SetState(device, OPT3001_INITIATE);
		break;
	}
	default:
		break;
	}
}
