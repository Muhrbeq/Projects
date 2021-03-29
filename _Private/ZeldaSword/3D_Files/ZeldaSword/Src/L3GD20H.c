/*
 * L3GD20H.c
 *
 *  Created on: 2 mars 2020
 *      Author: Rasmus.Muhrbeck
 */

/*
 *
 *  MEMS motion sensor:
 *  three-axis digital output gyroscope
 *
 */

#include "L3GD20H.h"
#include "stdbool.h"
#include "i2c.h"

float dpsSensitivity;

/* Return the struct you want to work with depending on i2c channel */
GyroScopeData *L3_GetGyroStruct(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == &hi2c2)
	{
		return &gyroStruct_i2c2;
	}
	else if(hi2c == &hi2c3)
	{
		return &gyroStruct_i2c3;
	}
	else
	{
		return &gyroStruct_i2c4;
	}
}

/* Init struct */
void L3_initStruct(GyroScopeData *gsd)
{
	gsd->x = 0;
	gsd->y = 0;
	gsd->z = 0;
}

/* Write to L3 over I2C */
static uint8_t L3_WriteI2C(I2C_HandleTypeDef *hi2c, uint8_t *addr, unsigned char* data, uint8_t len)
{
	uint32_t timeOutCounter = 0;
	while (HAL_I2C_Master_Transmit(hi2c, L3_WRITE_ADDR, (uint8_t *) data, len, I2C_TIMEOUT_TX) != HAL_OK)
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

/* Read L3 over I2C */
static uint16_t L3_ReadI2C(I2C_HandleTypeDef *hi2c, uint8_t *addr, unsigned char* data, uint8_t len)
{
	uint32_t timeOutCounter = 0;

	/* Transmit over i2c channel, address of unit, and data */
	while (HAL_I2C_Master_Transmit(hi2c, L3_WRITE_ADDR, (uint8_t *) data, 1, I2C_TIMEOUT_TX) != HAL_OK)
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
	while (HAL_I2C_Master_Receive(hi2c, L3_READ_ADDR, (uint8_t *) data, len, I2C_TIMEOUT_RX) != HAL_OK)
	{
		/* If timeout, report error */
		if (timeOutCounter > I2C_TIMEOUT_RX)
		{
//			I2C_Error(LOG_SUB_READTEMP, LOG_TYPE_READ, LOG_STATUS_TIMEOUT);
			return HAL_ERROR;
		}
		timeOutCounter++;
	}
	return HAL_OK;
}

/* Read ID of GyroScope */
uint8_t L3_ReadID(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = L3_WHO_AM_I;

	/* Read ID */
	if(L3_ReadI2C(hi2c, (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		return data[0];
	}

	/* Implement ErrorFlag */
	return 0;
}

static uint8_t L3_SetLowOutputDataRate(I2C_HandleTypeDef *hi2c, uint8_t lowDataRateEnable)
{
	unsigned char data[3] = {0};
	data[0] = L3_LOW_ODR;

	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		/* Modify  */
		data[1] = (lowDataRateEnable << L3_LOW_ODR_MASK) | data[0];
		data[0] = L3_LOW_ODR;

		if(L3_WriteI2C(hi2c, (uint8_t*)L3_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t L3_GetLowOutputDataRate(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = L3_LOW_ODR;

	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		uint8_t outputDataRate = data[0] & 0x01;

		return outputDataRate >> L3_LOW_ODR_MASK;
	}
	return 0;
}

static uint8_t L3_SetOutputDataRate(I2C_HandleTypeDef *hi2c, uint8_t dataRate)
{
	unsigned char data[3] = {0};
	data[0] = L3_CTRL_REG1;

	/* Read current data on CTRL_REG */
	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		/* Modify  */
		data[1] = (dataRate << L3_ODR_MASK) | data[0];
		data[0] = L3_CTRL_REG1;

		if(L3_WriteI2C(hi2c, (uint8_t*)L3_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}

	return HAL_ERROR;
}

static uint8_t L3_GetOutputDataRate(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = L3_CTRL_REG1;

	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		uint8_t outputDataRate = data[0] & 0xF0;

		return outputDataRate >> L3_ODR_MASK;
	}
	return 0;
}

static uint8_t L3_SetMode(I2C_HandleTypeDef *hi2c, uint8_t setMode)
{
	unsigned char data[3] = {0};
	data[0] = L3_CTRL_REG1;

	/* Read current data on CTRL_REG */
	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		/* Modify  */
		data[1] = (setMode << L3_POWER_MODE_MASK) | data[0];
		data[0] = L3_CTRL_REG1;

		if(L3_WriteI2C(hi2c, (uint8_t*)L3_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}

	return HAL_ERROR;
}

static uint8_t L3_GetMode(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = L3_CTRL_REG1;

	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		uint8_t powerMode = data[0] & 0x08;

		return powerMode >> L3_POWER_MODE_MASK;
	}
	return 0;
}

static uint8_t L3_SetFullScale(I2C_HandleTypeDef *hi2c, uint8_t fullScale)
{
	unsigned char data[3] = {0};
	data[0] = L3_CTRL_REG4;

	/* Read current data on CTRL_REG */
	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		/* Modify  */
		data[1] = (fullScale << L3_FULLSCALE_MASK) | data[0];
		data[0] = L3_CTRL_REG4;

		if(L3_WriteI2C(hi2c, (uint8_t*)L3_WRITE_ADDR, data, 2) == HAL_OK)
		{
			if(fullScale == L3_SCALE_245_DPS)
			{
				dpsSensitivity = L3_245_DPS_SENSITIVITY;
			}
			else if(fullScale == L3_SCALE_500_DPS)
			{
				dpsSensitivity = L3_500_DPS_SENSITIVITY;
			}
			else
				dpsSensitivity = L3_2000_DPS_SENSITIVITY;

			return HAL_OK;
		}
	}

	return HAL_ERROR;
}

static uint8_t L3_GetFullScale(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = L3_CTRL_REG4;

	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		uint8_t fullScale = data[0] & 0x30;

		return fullScale >> L3_FULLSCALE_MASK;
	}
	return 0;
}

static uint8_t L3_EnableX(I2C_HandleTypeDef *hi2c, uint8_t enableAxis)
{
	unsigned char data[3] = {0};
	data[0] = L3_CTRL_REG1;

	/* Read current data on CTRL_REG */
	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		/* Modify  */
		data[1] = (enableAxis << L3_X_MASK) | data[0];
		data[0] = L3_CTRL_REG1;

		if(L3_WriteI2C(hi2c, (uint8_t*)L3_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}

	return HAL_ERROR;
}

static uint8_t L3_EnableY(I2C_HandleTypeDef *hi2c, uint8_t enableAxis)
{
	unsigned char data[3] = {0};
	data[0] = L3_CTRL_REG1;

	/* Read current data on CTRL_REG */
	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		/* Modify  */
		data[1] = (enableAxis << L3_Y_MASK) | data[0];
		data[0] = L3_CTRL_REG1;

		if(L3_WriteI2C(hi2c, (uint8_t*)L3_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}

	return HAL_ERROR;
}

static uint8_t L3_EnableZ(I2C_HandleTypeDef *hi2c, uint8_t enableAxis)
{
	unsigned char data[3] = {0};
	data[0] = L3_CTRL_REG1;

	/* Read current data on CTRL_REG */
	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		/* Modify  */
		data[1] = (enableAxis << L3_Z_MASK) | data[0];
		data[0] = L3_CTRL_REG1;

		if(L3_WriteI2C(hi2c, (uint8_t*)L3_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}

	return HAL_ERROR;
}

static uint8_t L3_GetEnabledAxes(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = L3_CTRL_REG1;

	if(L3_ReadI2C((hi2c), (uint8_t*)L3_READ_ADDR, data, 1) == HAL_OK)
	{
		uint8_t axesEnabled = data[0] & L3_ALL_AXES_ENABLED;

		return axesEnabled >> L3_Y_MASK;
	}
	return 0;
}

uint8_t L3_EnableAllAxes(I2C_HandleTypeDef *hi2c, uint8_t enableAxis)
{
	if(L3_EnableX(hi2c, enableAxis) == HAL_OK)
	{
		if(L3_EnableY(hi2c, enableAxis) == HAL_OK)
		{
			if(L3_EnableZ(hi2c, enableAxis) == HAL_OK)
			{
				if(L3_GetEnabledAxes(hi2c) == L3_ALL_AXES_ENABLED)
				{
					return HAL_OK;
				}
			}
		}
	}
	return HAL_ERROR;
}

uint8_t L3_Start(I2C_HandleTypeDef *hi2c, uint8_t lodrEn, uint8_t odr, uint8_t mode, uint8_t scale)
{
	if(L3_ReadID(hi2c) == L3_ID)
	{
		if(L3_SetLowOutputDataRate(hi2c, lodrEn) == HAL_OK)
		{
			if(L3_GetLowOutputDataRate(hi2c) == lodrEn)
			{
				if(L3_SetOutputDataRate(hi2c, odr) == HAL_OK)
				{
					if(L3_GetOutputDataRate(hi2c) == odr)
					{
						if(L3_SetMode(hi2c, mode) == HAL_OK)
						{
							if(L3_GetMode(hi2c) == mode)
							{
								if(L3_SetFullScale(hi2c, scale) == HAL_OK)
								{
									if(L3_GetFullScale(hi2c) == scale)
									{
										debugPrint("Gyroscope initialized!\r\n", 1);
										return HAL_OK;
									}
									else
										debugPrint("Not same full scale as set\r\n", 2);
								}
								else
									debugPrint("Cannot set full scale\r\n", 1);
							}
							else
								debugPrint("Not same mode as set\r\n", 2);
						}
						else
							debugPrint("Cannot set mode\r\n", 1);
					}
					else
						debugPrint("Not same output data rate as set\r\n", 2);
				}
				else
					debugPrint("Cannot set output data rate\r\n", 1);
			}
			else
				debugPrint("LOW ODR bit not same as set\r\n", 2);
		}
		else
			debugPrint("Cannot set LOW ODR\r\n", 1);
	}
	else
		debugPrint("Not correct ID\r\n", 1);

	return HAL_ERROR;
}

static void L3_ReadAxisX(I2C_HandleTypeDef *hi2c, GyroScopeData *gyro)
{
	uint8_t data[2] = {0};
	unsigned char dRead[1] = {0};

	dRead[0] = L3_OUT_X_L;
	L3_ReadI2C(hi2c, (uint8_t*)L3_READ_ADDR, dRead, 1);
	data[0] = dRead[0];

	dRead[0] = L3_OUT_X_H;
	L3_ReadI2C(hi2c, (uint8_t*)L3_READ_ADDR, dRead, 1);
	data[1] = dRead[0];

	gyro->x = ((data[0])  | (data[1] << 8));
}

static void L3_ReadAxisY(I2C_HandleTypeDef *hi2c, GyroScopeData *gyro)
{
	uint8_t data[2] = {0};
	unsigned char dRead[1] = {0};

	dRead[0] = L3_OUT_Y_L;
	L3_ReadI2C(hi2c, (uint8_t*)L3_READ_ADDR, dRead, 1);
	data[0] = dRead[0];

	dRead[0] = L3_OUT_Y_H;
	L3_ReadI2C(hi2c, (uint8_t*)L3_READ_ADDR, dRead, 1);
	data[1] = dRead[0];

	gyro->y = ((data[0])  | (data[1] << 8));
}

static void L3_ReadAxisZ(I2C_HandleTypeDef *hi2c, GyroScopeData *gyro)
{
	uint8_t data[2] = {0};
	unsigned char dRead[1] = {0};

	dRead[0] = L3_OUT_Z_L;
	L3_ReadI2C(hi2c, (uint8_t*)L3_READ_ADDR, dRead, 1);
	data[0] = dRead[0];

	dRead[0] = L3_OUT_Z_H;
	L3_ReadI2C(hi2c, (uint8_t*)L3_READ_ADDR, dRead, 1);
	data[1] = dRead[0];

	gyro->z = ((data[0])  | (data[1] << 8));
}

static void L3_AdjustAxesXYZ(GyroScopeData *gyro)
{
	gyro->x *= dpsSensitivity;
	gyro->y *= dpsSensitivity;
	gyro->z *= dpsSensitivity;
}

void L3_ReadAxesXYZ(I2C_HandleTypeDef *hi2c, GyroScopeData *gyro)
{
	L3_ReadAxisX(hi2c, gyro);
	L3_ReadAxisY(hi2c, gyro);
	L3_ReadAxisZ(hi2c, gyro);
	L3_AdjustAxesXYZ(gyro);

	printf("x: %d, y: %d, z:%d\r\n", gyro->x, gyro->y, gyro->z);
}
