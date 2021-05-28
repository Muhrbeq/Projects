/*
 * MAX31855K.c
 *
 *  Created on: 27 okt. 2020
 *      Author: Rasmus.Muhrbeck
 */


#include "MAX31855K.h"
#include "gpio.h"

static void MAX31855_EnableCS(void)
{
	HAL_GPIO_WritePin(_CS_GPIO_Port, _CS_Pin, GPIO_PIN_RESET);
}

static void MAX31855_DisableCS(void)
{
	HAL_GPIO_WritePin(_CS_GPIO_Port, _CS_Pin, GPIO_PIN_SET);
}

static HAL_StatusTypeDef MAX31855_ReadRegister(uint8_t buffer[], uint8_t len)
{
	uint16_t timeoutCounter = 0;
	MAX31855_EnableCS();
	HAL_Delay(5);

	while(HAL_SPI_Receive(&hspi2, buffer, len, 1000) != HAL_OK)
	{
		if(timeoutCounter > 1000)
		{
			return HAL_ERROR;
		}
		timeoutCounter += 10;
	}
	HAL_Delay(5);

	MAX31855_DisableCS();
	return HAL_OK;
}


uint32_t readBytes()
{
	uint8_t buffer[4] = {'\0'};

	MAX31855_ReadRegister(buffer, 4);

	int32_t ret = buffer[0];
	ret <<= 8;
	ret |= buffer[1];
	ret <<= 8;
	ret |= buffer[2];
	ret <<= 8;
	ret |= buffer[3];


	return ret;
}

int16_t readTemperature()
{
	uint32_t rawValue;

	/* Read SPI */
	rawValue = readBytes();

	/* Check if negative */
	if(rawValue & (1 << 31))
	{
		return (0xC000 | ((rawValue >> 18) & 0x3FFF));
	}
	else
	{
		return rawValue >> 18;
	}
}

int16_t readCJT()
{
	uint32_t rawBytes;

	/* Read SPI */
	rawBytes = readBytes();

	/* Check if negative */
	if(rawBytes & (1 << 15))
	{
		return (0xF000 | ((rawBytes >> 4) & 0xFFF));
	}
	else
	{
		return (rawBytes >> 4) & 0xFFF;
	}
}
