/*
 * HDC1080.c
 *
 *  Created on: 27 Nov 2018
 *      Author: andreas.fredlund
 */

#include "HDC1080.h"
#include "Error.h"
#include "Global.h"

/*
 * Writes to a certain register. Input: data with register and data, the length of the array, error type message.
 * Return hal_ok if success, else hal_error
 */
uint8_t HDC1080_writeRegister(unsigned char* data, uint8_t len, uint8_t throw)
{
	uint8_t timeOutCounter = 0;

	while (HAL_I2C_Master_Transmit(&hi2c2, HDC1080_ADDR_WRITE, (uint8_t *) data,
			len, 1) != HAL_OK)
	{
		if (timeOutCounter > HDC1080_TIMEOUT_TX_END)
		{


			ErrorAddLog(LOG_FUNC_HDC1080, LOG_SUB_WRITE, throw, LOG_STATUS_TIMEOUT, data[0], data[1], 0, GetErrorStruct());
			return HAL_ERROR;
		}
		timeOutCounter++;
	}
	return HAL_OK;
}



uint8_t HDC1080_readRegister(unsigned char* data, uint8_t len, uint8_t throw)
{
	uint8_t timeOutCounter = 0;

	while (HAL_I2C_Master_Transmit(&hi2c2, HDC1080_ADDR_WRITE, (uint8_t *) data,
			1, 1) != HAL_OK)
	{
		if (timeOutCounter > HDC1080_TIMEOUT_TX_END)
		{
			ErrorAddLog(LOG_FUNC_HDC1080, LOG_SUB_READ, throw, LOG_STATUS_TIMEOUT, data[0], 1, 0, GetErrorStruct());
			return HAL_ERROR;
		}
		timeOutCounter++;
	}

	HAL_Delay(20);	// todo dont know if needed

	timeOutCounter = 0;
	while (HAL_I2C_Master_Receive(&hi2c2, HDC1080_ADDR_READ, (uint8_t *) data,
			len, 1) != HAL_OK)
	{
		if (timeOutCounter > HDC1080_TIMEOUT_RX_END)
		{
			ErrorAddLog(LOG_FUNC_HDC1080, LOG_SUB_READ, throw, LOG_STATUS_TIMEOUT, data[0], 2, 0, GetErrorStruct());
			return HAL_ERROR;
		}
		timeOutCounter++;
	}
	return HAL_OK;
}

void HDC1080_Init(void)
{
	unsigned char data[4] = { 0 };
	data[0] = HDC1080_CONF_REG;

	if (HDC1080_Get_Device_ID() != HDC1080_DEVICE_ID)
	{
		ErrorAddLog(LOG_FUNC_HDC1080, LOG_SUB_INIT, LOG_TYPE_ID, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

		InfoLogAdd(ERRORLEVEL_SEVERE,
		LOG_FUNC_HDC1080, LOG_SUB_INIT, LOG_TYPE_ID, LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());
	}
	else
	{

		if (HDC1080_readRegister(data, 2, LOG_TYPE_SET) != HAL_OK)
		{
			ErrorAddLog(LOG_FUNC_HDC1080, LOG_SUB_INIT, LOG_TYPE_READ, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			InfoLogAdd(
					ERRORLEVEL_SEVERE,
					LOG_FUNC_HDC1080,
					LOG_SUB_INIT,
					LOG_TYPE_READ,
					LOG_STATUS_FAIL,
					0,
					0,
					0,
					GetTempInfoStruct());

		}
		else
		{

			uint16_t confReg = (data[0] << 8) | data[1];

			confReg |= 1 << 13;		//	heater disabled 0 ; enable 1
			confReg |= 0 << 12;		//	Temperature or Humidity is acquired 0 ; Temperature and Humidity is acquired 1.
			confReg |= 0 << 10;		//	Temp Resolution. 0 = 14 bit	;	1 = 11 bit
			confReg |= 0 << 9;		//	Hum Resolution. bit [9:8]. 00 = 14 bit ; 01 = 11 bit ; 10 = 8 bit
			confReg |= 0 << 8;

			data[0] = HDC1080_CONF_REG;
			data[1] = (uint8_t) ((confReg >> 8) & 0xFF);  	//	msb
			data[2] = (uint8_t) (confReg & 0xFF);  			//	lsb

			if (HDC1080_writeRegister(data, 3, LOG_TYPE_SET) != HAL_OK)
			{
				ErrorAddLog(LOG_FUNC_HDC1080, LOG_SUB_INIT, LOG_TYPE_WRITE, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

				InfoLogAdd(
						ERRORLEVEL_SEVERE,
						LOG_FUNC_HDC1080,
						LOG_SUB_INIT,
						LOG_TYPE_WRITE,
						LOG_STATUS_FAIL,
						0,
						0,
						0,
						GetTempInfoStruct());
			}
			else
			{
				data[0] = HDC1080_CONF_REG;
				HDC1080_readRegister(data, 2, LOG_TYPE_SET);
			}
		}
	}
}


uint16_t HDC1080_Get_Device_ID(void)
{
	unsigned char data[2] = { 0 };
	data[0] = HDC1080_DEV_ID_REG; 					 // address of the register

	if (HDC1080_readRegister(data, 2, LOG_TYPE_ID) == HAL_OK)  // data, length, LOG_TYPE error
	{
		return (data[0] << 8) | data[1];
	}
	else
	{
		return HDC1080_ERROR_VALUE;
	}
}

uint16_t HDC1080_Get_Temp_raw(void)
{
	unsigned char data[2] = { 0 };
	data[0] = HDC1080_TEMP_REG;

	if (HDC1080_readRegister(data, 2, LOG_TYPE_GET_TEMP) == HAL_OK)
	{
		return (data[0] << 8) | data[1];
	}
	else
	{
		return HDC1080_ERROR_VALUE;
	}
}


uint16_t HDC1080_Get_Humidity_raw(void)
{
	unsigned char data[2] = { 0 };
	data[0] = HDC1080_HUM_REG;
	if (HDC1080_readRegister(data, 2, LOG_TYPE_GET_HUM) == HAL_OK)
	{
		return (data[0] << 8) | data[1];
	}
	else
	{
		InfoLogAdd(ERRORLEVEL_WARNING,
		LOG_FUNC_HDC1080, LOG_SUB_SENSOR,
		LOG_TYPE_READ,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		return HDC1080_ERROR_VALUE;
	}
}


/*
 * Calc from datasheet
 * Mult by 100 to get 4 digit number (2 decimals).
 */
uint16_t HDC1080_Get_Temp(void)
{
	float temp = (float) HDC1080_Get_Temp_raw();
	temp = 165 * temp / 65536.0 - 40.0;
	return (uint16_t) (temp * 100);
}


/*
 * Calc from datasheet.
 * Gives Relative Humidity in %
 */
uint8_t HDC1080_Get_Humidity(void)
{
	float temp = 100.0 * (float) HDC1080_Get_Humidity_raw() / 65536.0;

	if (temp > 100)
	{
		temp = 100;
	}
	else if (temp < 0)
	{
		temp = 0;
	}

	return (uint8_t) temp;
}





