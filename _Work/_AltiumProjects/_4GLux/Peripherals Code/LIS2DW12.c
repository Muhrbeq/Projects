/*
 * LIS2DW12.c
 *
 *  Created on: 26 feb. 2020
 *      Author: Rasmus.Muhrbeck
 */

#include <LIS2DW12.h>
#include <i2c.h>
#include "__ExegerGeneric.h"

float accSensitivity;

/* Return the struct you want to work with depending on i2c channel */
Acceleration *LIS_GetAccelerationStruct(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == &hi2c1)
	{
		return &accStruct_i2c3;
	}
	else
	{
		return &accStruct_i2c4;
	}
}

/* Init struct */
void LIS_initStruct(Acceleration *acc)
{
	acc->x = 0;
	acc->y = 0;
	acc->z = 0;
}

/* Write to LIS over I2C */
static uint8_t LIS_WriteI2C(I2C_HandleTypeDef *hi2c, uint8_t *addr, unsigned char* data, uint8_t len)
{
	uint32_t timeOutCounter = 0;
	while (HAL_I2C_Master_Transmit(hi2c, LIS_WRITE_ADDR, (uint8_t *) data, len, I2C_TIMEOUT_TX) != HAL_OK)
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

/* Read LIS over I2C */
static uint16_t LIS_ReadI2C(I2C_HandleTypeDef *hi2c, uint8_t *addr, unsigned char* data, uint8_t len)
{
	uint32_t timeOutCounter = 0;

	/* Transmit over i2c channel, address of unit, and data */
	while (HAL_I2C_Master_Transmit(hi2c, LIS_WRITE_ADDR, (uint8_t *) data, 1, I2C_TIMEOUT_TX) != HAL_OK)
	{
		/* If timeout, report error */
		if (timeOutCounter > I2C_TIMEOUT_TX)
		{
			HeartBeatToggle();
//			I2C_Error(LOG_SUB_READTEMP, LOG_TYPE_WRITE, LOG_STATUS_TIMEOUT);
			return HAL_ERROR;
		}
		timeOutCounter++;
	}

	timeOutCounter = 0;
	/* Receive on same i2c channel, same unit address */
	while (HAL_I2C_Master_Receive(hi2c, LIS_READ_ADDR, (uint8_t *) data, len, I2C_TIMEOUT_RX) != HAL_OK)
	{
		/* If timeout, report error */
		if (timeOutCounter > I2C_TIMEOUT_RX)
		{
			HeartBeatToggle();
//			I2C_Error(LOG_SUB_READTEMP, LOG_TYPE_READ, LOG_STATUS_TIMEOUT);
			return HAL_ERROR;
		}
		timeOutCounter++;
	}
	return HAL_OK;
}

/* Read ID of Accelerometer */
uint8_t LIS_ReadID(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = LIS_WHO_AM_I;

	/* Read ID */
	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		return data[0];
	}

	/* Implement ErrorFlag */
	return 0;
}

/* Set output data rate */
static uint8_t LIS_SetOutputDataRate(I2C_HandleTypeDef *hi2c, uint8_t dataRate)
{
	unsigned char data[3] = {0};
	data[0] = LIS_CTRL_REG1;

	/* Read current data on CTRL_REG */
	if(LIS_ReadI2C((hi2c), (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		/* Modify  */
		data[1] = (dataRate << LIS_ODR_MASK) | data[0];
		data[0] = LIS_CTRL_REG1;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}

	return HAL_ERROR;
}

static uint8_t LIS_GetOutputDataRate(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG1;

	if(LIS_ReadI2C((hi2c), (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		uint8_t outputDataRate = data[0] & 0xF0;

		return outputDataRate >> LIS_ODR_MASK;
	}
	return 0;
}

static uint8_t LIS_SetMode(I2C_HandleTypeDef *hi2c, uint8_t modeSelect)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG1;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (modeSelect << LIS_MODE_MASK) | data[0];
		data[0] = LIS_CTRL_REG1;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_GetMode(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG1;

	if(LIS_ReadI2C((hi2c), (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		uint8_t modeSelected = data[0] & 0x0C;

		return modeSelected >> LIS_MODE_MASK;
	}
	return 0;
}

static uint8_t LIS_SetFifoMode(I2C_HandleTypeDef *hi2c, uint8_t fifoMode)
{
	unsigned char data[2] = {0};
	data[0] = LIS_FIFO_CTRL;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (fifoMode << LIS_FIFO_MASK) | data[0];
		data[0] = LIS_FIFO_CTRL;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_GetFifoMode(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = LIS_FIFO_CTRL;

	if(LIS_ReadI2C((hi2c), (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		uint8_t fifoMode = data[0] & 0xE0;

		return fifoMode >> LIS_FIFO_MASK;
	}
	return 0;
}

static uint8_t LIS_SetFullScale(I2C_HandleTypeDef *hi2c, uint8_t fullScale)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG6;


	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (fullScale << LIS_SCALE_MASK) | data[0];
		data[0] = LIS_CTRL_REG6;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			if(fullScale == LIS_SCALE_2G)
			{
				accSensitivity = LIS_SENSITIVITY_2G;
			}
			else if(fullScale == LIS_SCALE_4G)
			{
				accSensitivity = LIS_SENSITIVITY_4G;
			}
			else if(fullScale == LIS_SCALE_8G)
			{
				accSensitivity = LIS_SENSITIVITY_8G;
			}
			else
				accSensitivity = LIS_SENSITIVITY_16G;

			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_GetFullScale(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG6;

	if(LIS_ReadI2C((hi2c), (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		uint8_t fullScale = data[0] & 0x30;

		return fullScale >> LIS_SCALE_MASK;
	}
	return 0;
}

static uint8_t LIS_SetSingleTapEnable(I2C_HandleTypeDef *hi2c, uint8_t enable)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG4_INT1;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (enable << LIS_SINGLE_TAP_MASK) | data[0];
		data[0] = LIS_CTRL_REG4_INT1;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_SetDoubleTapEnable(I2C_HandleTypeDef *hi2c, uint8_t enable)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG4_INT1;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (enable << LIS_DOUBLE_TAP_MASK) | data[0];
		data[0] = LIS_CTRL_REG4_INT1;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_SetThresholdX_Tap(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = LIS_TAP_THS_X;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (LIS_TAP_THSX_20 << LIS_TAP_THSX_MASK) | data[0];
		data[0] = LIS_TAP_THS_X;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_EnableTapX(I2C_HandleTypeDef *hi2c, uint8_t enable)
{
	unsigned char data[2] = {0};
	data[0] = LIS_TAP_THS_Z;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (enable << LIS_TAP_X_EN_MASK) | data[0];
		data[0] = LIS_TAP_THS_Z;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}


static uint8_t LIS_SetLatchedIRQ(I2C_HandleTypeDef *hi2c, uint8_t enable)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG3;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (enable << LIS_LATCHED_IRQ_MASK) | data[0];
		data[0] = LIS_CTRL_REG3;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_EnableInterrupts(I2C_HandleTypeDef *hi2c, uint8_t enable)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG7;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (enable << LIS_IRQ_EN_MASK) | data[0];
		data[0] = LIS_CTRL_REG7;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_EnableLowNoise(I2C_HandleTypeDef *hi2c, uint8_t enable)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG6;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (enable << LIS_LOW_NOISE_MASK) | data[0];
		data[0] = LIS_CTRL_REG6;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_SetLatencyDoubleTap(I2C_HandleTypeDef *hi2c, uint8_t latency)
{
	unsigned char data[2] = {0};
	data[0] = LIS_INT_DUR;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (latency << LIS_LATENCY_MASK) | data[0];
		data[0] = LIS_INT_DUR;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_SetQuietTime(I2C_HandleTypeDef *hi2c, uint8_t quietTime)
{
	unsigned char data[2] = {0};
	data[0] = LIS_INT_DUR;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (quietTime << LIS_QUIET_MASK) | data[0];
		data[0] = LIS_INT_DUR;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_SetShockTime(I2C_HandleTypeDef *hi2c, uint8_t shock)
{
	unsigned char data[2] = {0};
	data[0] = LIS_INT_DUR;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (shock << LIS_SHOCK_MASK) | data[0];
		data[0] = LIS_INT_DUR;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

static uint8_t LIS_EnableSingleDoubleTap(I2C_HandleTypeDef *hi2c, uint8_t enable)
{
	unsigned char data[2] = {0};
	data[0] = LIS_WAKE_UP_THS;

	if(LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		data[1] = (enable << LIS_SINGLE_DOUBLE_MASK) | data[0];
		data[0] = LIS_WAKE_UP_THS;

		if(LIS_WriteI2C(hi2c, (uint8_t*)LIS_WRITE_ADDR, data, 2) == HAL_OK)
		{
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}

uint8_t LIS_ReadTapSource(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = LIS_TAP_SRC;

	if(LIS_ReadI2C((hi2c), (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		uint8_t fullScale = data[0] & 0xFF;

		return fullScale;
	}
	return 0;
}

uint8_t LIS_CTRL1_Read(I2C_HandleTypeDef *hi2c)
{
	unsigned char data[2] = {0};
	data[0] = LIS_CTRL_REG1;

	if(LIS_ReadI2C((hi2c), (uint8_t*)LIS_READ_ADDR, data, 1) == HAL_OK)
	{
		return data[0];
	}
	return 0;
}

uint8_t LIS_Start(I2C_HandleTypeDef *hi2c, uint8_t odr, uint8_t mode, uint8_t fifo, uint8_t scale)
{
	if(LIS_ReadID(hi2c) == LIS_ADDRESS)
	{
		if(LIS_SetOutputDataRate(hi2c, odr) == HAL_OK)
		{
			debugPrint("Set output data rate\r\n", 1);
			if(LIS_GetOutputDataRate(hi2c) == odr)
			{
				printf("ODR: %x\r\n", LIS_GetOutputDataRate(hi2c));
				if(LIS_SetMode(hi2c, mode) == HAL_OK)
				{
					debugPrint("Set Mode\r\n", 1);
					if(LIS_GetMode(hi2c) == mode)
					{
						debugPrint("Get Mode\r\n", 1);
						if(LIS_SetFifoMode(hi2c, fifo) == HAL_OK)
						{
							debugPrint("Set Fifo\r\n", 1);
							if(LIS_GetFifoMode(hi2c) == fifo)
							{
								if(LIS_SetFullScale(hi2c, scale) == HAL_OK)
								{
									debugPrint("Set full scale\r\n", 1);
									if(LIS_GetFullScale(hi2c) == scale)
									{
										if(LIS_EnableLowNoise(hi2c, 1) == HAL_OK)
										{
											debugPrint("Low noise enabled\r\n", 1);
										}
										if(LIS_EnableSingleDoubleTap(hi2c,1) == HAL_OK)
										{
											debugPrint("Single/Double Tap enable", 1);
										}
										if(LIS_SetDoubleTapEnable(hi2c, 1) == HAL_OK)
										{
											debugPrint("DOUBLE TAP ENABLE!\r\n", 3);

											if(LIS_SetThresholdX_Tap(hi2c) == HAL_OK)
											{
												debugPrint("SingleTap X threshold set to 0x14\r\n", 1);

												if(LIS_SetQuietTime(hi2c, LIS_QUIET_TEST1) == HAL_OK)
												{
													debugPrint("Quiet time set\r\n", 1);
												}
												if(LIS_SetShockTime(hi2c, LIS_SHOCK_TEST1) == HAL_OK)
												{
													debugPrint("Shock Time set \r\n", 1);
												}

												if(LIS_EnableTapX(hi2c, 1) == HAL_OK)
												{
													debugPrint("Single Tap X axis enable!\r\n", 0);
												}
												if(LIS_SetLatencyDoubleTap(hi2c, LIS_LATENCY_TEST1) == HAL_OK)
												{
													debugPrint("Latency set\r\n", 1);
												}
												if(LIS_EnableInterrupts(hi2c, 1) == HAL_OK)
												{
													debugPrint("IRQs Enable!\r\n", 1);
												}
											}
										}
										debugPrint("Accelerometer initialized!\r\n", 1);
										return HAL_OK;
									}
									else
									{
										debugPrint("Full scale not same as set\r\n", 3);
									}
								}
								else
								{
									debugPrint("Not able to set full scale\r\n", 2);
								}
							}
							else
							{
								debugPrint("Fifo mode not same as set\r\n", 3);
							}
						}
						else
						{
							debugPrint("Not able to set Fifo mode\r\n", 2);
						}
					}
					else
					{
						debugPrint("Mode not same as set mode\r\n", 3);
					}
				}
				else
				{
					debugPrint("Not able to set Mode\r\n", 2);
				}
			}
			else
			{
				debugPrint("ODR not same as set\r\n", 3);
			}
		}
		else
		{
			debugPrint("Not able to set Output data rate\r\n", 2);
		}
	}
	else
	{
		debugPrint("Not able to recognize accelerometer\r\n", 1);
	}
	return HAL_ERROR;
}

static void LIS_ReadAxisX(I2C_HandleTypeDef *hi2c, Acceleration *acc)
{
	uint8_t data[2] = {0};
	unsigned char dRead[1] = {0};

	dRead[0] = LIS_OUT_X_L;
	LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, dRead, 1);
	data[0] = dRead[0];

	dRead[0] = LIS_OUT_X_H;
	LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, dRead, 1);
	data[1] = dRead[0];

	acc->x = ((data[0])  | (data[1] << 8));
}

static void LIS_ReadAxisY(I2C_HandleTypeDef *hi2c, Acceleration *acc)
{
	uint8_t data[2] = {0};
	unsigned char dRead[1] = {0};

	dRead[0] = LIS_OUT_Y_L;
	LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, dRead, 1);
	data[0] = dRead[0];

	dRead[0] = LIS_OUT_Y_H;
	LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, dRead, 1);
	data[1] = dRead[0];

	acc->y = (data[0]) | (data[1] << 8);
}

static void LIS_ReadAxisZ(I2C_HandleTypeDef *hi2c, Acceleration *acc)
{
	uint8_t data[2] = {0};
	unsigned char dRead[1] = {0};

	dRead[0] = LIS_OUT_Z_L;
	LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, dRead, 1);
	data[0] = dRead[0];

	dRead[0] = LIS_OUT_Z_H;
	LIS_ReadI2C(hi2c, (uint8_t*)LIS_READ_ADDR, dRead, 1);
	data[1] = dRead[0];

	acc->z = (data[0]) | (data[1] << 8);
}

static void LIS_AdjustAxesXYZ(Acceleration *acc)
{
	acc->x *= accSensitivity;
	acc->y *= accSensitivity;
	acc->z *= accSensitivity;
}

void LIS_ReadAxesXYZ(I2C_HandleTypeDef *hi2c, Acceleration *acc)
{
	LIS_ReadAxisX(hi2c, acc);
	LIS_ReadAxisY(hi2c, acc);
	LIS_ReadAxisZ(hi2c, acc);
	LIS_AdjustAxesXYZ(acc);

	//printf("A: x: %d, y: %d, z:%d\r\n", acc->x, acc->y, acc->z);
}
