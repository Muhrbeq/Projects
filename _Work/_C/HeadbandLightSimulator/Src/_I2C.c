/*
 * _I2C.c
 *
 *  Created on: 2 feb. 2021
 *      Author: Rasmus.Muhrbeck
 */

#include "_I2C.h"
#include "__ExegerGeneric.h"

/* Write to I2C with given address
 *
 * Input: I2C handle, address to write, pointer to data to write, lenght of data
 *
 * */
uint8_t I2C_WriteRegister(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t* data, uint8_t len)
{
	uint32_t timeOutCounter = 0;
	while (HAL_I2C_Master_Transmit(hi2c, addr, data,
			len, 1) != HAL_OK)
	{
		/* Check if timeout error */
		if (timeOutCounter > I2C_TIMEOUT_TX_END)
		{
			/* Return error to add to info */
			return HAL_ERROR;
		}
		/* Increment counter */
		timeOutCounter++;
	}
	return HAL_OK;
}

/* Read I2C with given address */
uint16_t I2C_ReadRegister(I2C_HandleTypeDef *hi2c, uint16_t writeAddr, uint16_t readAddr, uint8_t* data, uint8_t len)
{
	uint32_t timeOutCounter = 0;

	while (HAL_I2C_Master_Transmit(hi2c, writeAddr, data,
			1, 1) != HAL_OK)
	{
		if (timeOutCounter > I2C_TIMEOUT_TX_END)
		{
			return HAL_ERROR;
		}
		timeOutCounter++;
	}

	timeOutCounter = 0;
	while (HAL_I2C_Master_Receive(hi2c, readAddr, data,
			len, 1) != HAL_OK)
	{
		if (timeOutCounter > I2C_TIMEOUT_RX_END)
		{
			return HAL_ERROR;
		}
		timeOutCounter++;
	}
	return HAL_OK;
}

uint8_t I2C_Write(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t* data, uint8_t len)
{
	if(HAL_I2C_Master_Transmit(hi2c, addr, data,
			len, 100) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

uint8_t I2C_ReadWrite(I2C_HandleTypeDef *hi2c, uint16_t writeAddr, uint8_t* data, uint8_t len)
{
	if(HAL_I2C_Master_Transmit(hi2c, writeAddr, data,
				len, 100) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

uint16_t I2C_ReadRead(I2C_HandleTypeDef *hi2c, uint16_t readAddr, uint8_t* data, uint8_t len)
{
	if(HAL_I2C_Master_Receive(hi2c, readAddr, data,
			len, 100) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

