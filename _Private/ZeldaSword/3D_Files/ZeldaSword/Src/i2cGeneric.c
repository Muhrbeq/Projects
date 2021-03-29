/*
 * i2cGeneric.c
 *
 *  Created on: 26 feb. 2020
 *      Author: Rasmus.Muhrbeck
 */
#include "__ExegerGeneric.h"
#include "i2cGeneric.h"

uint8_t WriteI2C(I2C_HandleTypeDef *hi2c, uint8_t *addr, unsigned char* data, uint8_t len)
{
	uint32_t timeOutCounter = 0;
//TMP117_ADDR_WRITE
	while (HAL_I2C_Master_Transmit(hi2c, *addr, (uint8_t *) data, len, I2C_TIMEOUT_TX) != HAL_OK)
	{
		/* If timeout, report error */
		if (timeOutCounter > I2C_TIMEOUT_TX)
		{
//			I2C_Error(LOG_SUB_READTEMP, LOG_TYPE_WRITE, LOG_STATUS_TIMEOUT);
			return HAL_ERROR;
		}

		timeOutCounter++;
	}
	return HAL_OK;
}

uint16_t ReadI2C(I2C_HandleTypeDef *hi2c, uint8_t *addr, unsigned char* data, uint8_t len)
{
	uint32_t timeOutCounter = 0;

	/* Transmit over i2c channel, address of unit, and data */
	while (HAL_I2C_Master_Transmit(hi2c, *addr, (uint8_t *) data, 1, I2C_TIMEOUT_TX) != HAL_OK)
	{
		/* If timeout, report error */
		if (timeOutCounter > I2C_TIMEOUT_TX)
		{
//			I2C_Error(LOG_SUB_READTEMP, LOG_TYPE_WRITE, LOG_STATUS_TIMEOUT);
			return HAL_ERROR;
		}
		timeOutCounter++;
	}

	timeOutCounter = 0;
	/* Receive on same i2c channel, same unit address */
	while (HAL_I2C_Master_Receive(hi2c, *addr, (uint8_t *) data, len, I2C_TIMEOUT_RX) != HAL_OK)
	{
		/* If timeout, report error */
		if (timeOutCounter > I2C_TIMEOUT_RX)
		{
//			I2C_Error(LOG_SUB_READTEMP, LOG_TYPE_READ, LOG_STATUS_TIMEOUT);
			return HAL_ERROR;
		}
		timeOutCounter++;
	}
	return 0;
}

/**-------------------------------------------------------------------------------------------------
  Name         :
  Description  :
  Argument(s)  :
  Return 	   :
--------------------------------------------------------------------------------------------------*/
//static void TMP117_Error(uint8_t Sub_type, uint8_t Error_type, uint8_t Status_type)
//{
//	InfoLogAdd(ERRORLEVEL_FATAL, LOG_FUNC_TMP, Sub_type, Error_type,
//			Status_type, 0, 0, 0, GetTempInfoStruct());
//}
