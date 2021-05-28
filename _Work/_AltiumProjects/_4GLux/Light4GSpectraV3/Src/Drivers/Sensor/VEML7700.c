/*
 * VEML7700.C
 *
 *  Created on: 31 Jan 2019
 *      Author: andreas.fredlund
 */

#include "VEML7700.h"
#include "Global.h"
#include "LowLevel.h"
#include "Utils.h"
#include "Error.h"

/* 													RESOLUTION AND MAXIMUM DETECTION RANGE
 *						TYPICAL RESOLUTION											MAXIMUM POSSIBLE ILLUMINATION
 * 				GAIN 2	|	GAIN 1	|	GAIN 1/4	|	GAIN 1/8	||||	GAIN 2	|	GAIN 1	|	GAIN 1/4	|	GAIN 1/8
 * IT (ms)  |														||||
 * 800		|	0.0036 	|	0.0072 	|	0.0288 		|	0.0576 		||||	236 	|	472 	|	1887 		|	3775
 * 400		|	0.0072 	|	0.0144  |	0.0576 		|	0.1152 		||||	472 	|	944 	|	3775 		|	7550
 * 200		|	0.0144 	|	0.0288 	|	0.1152 		|	0.2304 		||||	944 	|	1887 	|	7550 		|	15 099
 * 100		|	0.0288 	|	0.0576 	|	0.2304 		|	0.4608 		||||	1887 	|	3775 	|	15 099 		|	30 199
 * 50		|	0.0576 	|	0.1152 	|	0.4608 		|	0.9216 		||||	3775 	|	7550 	|	30 199 		|	60 398
 * 25		|	0.1152 	|	0.2304 	|	0.9216 		|	1.8432 		||||	7550 	|	15 099 	|	60 398 		|	120 796
 */

//https://github.com/tedyapo/arduino-VEML7700

uint16_t register_cache[4];

#define VEML7700_ADDR_WRITE  		VEML7700_ADDRESS << 1
#define VEML7700_ADDR_READ   		(VEML7700_ADDRESS << 1) +1

static uint8_t VEML7700_writeRegister(unsigned char* data, uint8_t len)
{
	uint8_t timeOutCounter = 0;
	/* First place in array data is the register. Then is the data itself */
	while (HAL_I2C_Master_Transmit(&hi2c2, VEML7700_ADDR_WRITE, (uint8_t *) data, len, 10) != HAL_OK)
	{
		if (timeOutCounter > VEML7700_TIMEOUT_TX_END)
		{
//			ErrorAddLog(
//					LOG_FUNC_VEML7700,
//					LOG_SUB_WRITE,
//					throw,
//					LOG_STATUS_TIMEOUT,
//					data[0],
//					0,
//					0,
//					GetErrorStruct());
			return HAL_ERROR;
		}
		timeOutCounter++;
	}
	return HAL_OK;
}

static uint8_t VEML7700_readRegister(uint8_t reg, unsigned char* data, uint8_t len)
{
	uint8_t timeOutCounter = 0;
	while (HAL_I2C_Mem_Read(&hi2c2, VEML7700_ADDR_READ, reg, 1, data, len, 10) != HAL_OK)
	{
		if (timeOutCounter > VEML7700_TIMEOUT_RX_END)
		{
//			ErrorAddLog(
//					LOG_FUNC_VEML7700,
//					LOG_SUB_READ,
//					throw,
//					LOG_STATUS_TIMEOUT,
//					data[0],
//					0,
//					0,
//					GetErrorStruct());
			return HAL_ERROR;
		}
		timeOutCounter++;
	}
	return HAL_OK;
}

static uint8_t VEML7700_ReadData(uint8_t reg, uint16_t *data)
{
	unsigned char data_r[3] = { 0 };

	if (VEML7700_readRegister(reg, data_r, 2) == HAL_OK)
	{
		*data = (data_r[0] + data_r[1] * 256);
		return STATUS_OK;
	}
	return STATUS_ERROR;

//	uint8_t rData[3] = { 0 };
//
////	TWID_Read(GetTWIDriver(), VEML7700_ADDRESS, reg, 1, rData, 2, 0);
//
//	uint8_t timeOutCounter = 0;
//	while (HAL_I2C_Mem_Read(&hi2c2, VEML7700_ADDRESS, reg, 1, rData, 2, 10) != HAL_OK)
//	{
//		if (timeOutCounter > VEML7700_TIMEOUT_RX_END)
//		{
//			ErrorAddLog(
//					LOG_FUNC_VEML7700,
//					LOG_SUB_READ,
//					0,
//					LOG_STATUS_TIMEOUT,
//					data[0],
//					0,
//					0,
//					GetErrorStruct());
//			return STATUS_ERROR;
//		}
//		timeOutCounter++;
//	}
//
//	*data = (rData[0] | (rData[1] << 8)); //LSB first
//
//	return STATUS_OK;
}

static uint8_t VEML7700_SendData(uint8_t reg, uint16_t data)
{
	/* Array for the register and data */
	unsigned char data_t[4] = { 0 };

	uint8_t dataLen = sizeof(data_t);
	/* Adding 1 for the register */
	dataLen = dataLen + 1;

	/* Sets upp the array */
	data_t[0] = reg;
	data_t[1] = (data & 0xff);
	data_t[2] = (data >> 8);

	//printf("ReadDataLen: %d\r\n", dataLen);

	if (VEML7700_writeRegister(data_t, 3) == HAL_OK)
	{
		return STATUS_OK;
	}
	return STATUS_ERROR;

//	uint8_t tmp[3] = { 0 };
//
//	tmp[0] = (data) & 0xFF;		 //LSB First
//	tmp[1] = (data >> 8) & 0xFF; //MSB Last
//
//	uint8_t timeOutCounter = 0;
////	TWID_Write(GetTWIDriver(), VEML7700_ADDRESS, reg, 1, tmp, 2, 0);
//	while (HAL_I2C_Master_Transmit(&hi2c2, VEML7700_ADDRESS, tmp, 2, 10) != HAL_OK)
//	{
//		if (timeOutCounter > VEML7700_TIMEOUT_TX_END)
//		{
//			ErrorAddLog(
//			LOG_FUNC_VEML7700,
//			LOG_SUB_WRITE, 0,
//			LOG_STATUS_TIMEOUT, tmp[0], 0, 0, GetErrorStruct());
//			return STATUS_ERROR;
//		}
//		timeOutCounter++;
//	}
//
//	return STATUS_OK;
}

void VEML7700_Initialize(void)
{
	// write initial state to VEML7700
	register_cache[0] = (((uint16_t) (ALS_GAIN_x2) << ALS_SM_SHIFT)
			| ((uint16_t) (ALS_INTEGRATION_100ms) << ALS_IT_SHIFT)
			| ((uint16_t) (ALS_PERSISTENCE_1) << ALS_PERS_SHIFT)
			| ((uint16_t) (0) << ALS_INT_EN_SHIFT) | ((uint16_t) (0) << ALS_SD_SHIFT));

	register_cache[1] = 0x0000;

	register_cache[2] = 0xffff;

	register_cache[3] = (((uint16_t) (ALS_POWER_MODE_3) << PSM_SHIFT)
			| ((uint16_t) (0) << PSM_EN_SHIFT));

	for (uint8_t i = 0; i < 4; i++)
	{
		VEML7700_SendData(i, register_cache[i]);
	}

	// wait at least 2.5ms as per datasheet
	HAL_Delay(3);
}

uint8_t VEML7700_SetGain(als_gain_t gain)
{
	uint16_t reg = ((register_cache[COMMAND_ALS_SM] & ~ALS_SM_MASK)
			| (((uint16_t) (gain) << ALS_SM_SHIFT) & ALS_SM_MASK));

	register_cache[COMMAND_ALS_SM] = reg;

	return VEML7700_SendData(COMMAND_ALS_SM, reg);
}

uint8_t VEML7700_GetGain(als_gain_t *gain)
{
	*gain = (als_gain_t) ((register_cache[COMMAND_ALS_SM] & ALS_SM_MASK) >> ALS_SM_SHIFT);

	return STATUS_OK;
}

uint8_t VEML7700_SetIntegrationTime(als_itime_t itime)
{
	uint16_t reg = ((register_cache[COMMAND_ALS_IT] & ~ALS_IT_MASK)
			| (((uint16_t) (itime) << ALS_IT_SHIFT) & ALS_IT_MASK));

	register_cache[COMMAND_ALS_IT] = reg;

	return VEML7700_SendData(COMMAND_ALS_IT, reg);
}

uint8_t VEML7700_GetIntegrationTime(als_itime_t *itime)
{
	*itime = (als_itime_t) ((register_cache[COMMAND_ALS_IT] & ALS_IT_MASK) >> ALS_IT_SHIFT);

	return STATUS_OK;
}

uint8_t VEML7700_SetPersistence(als_persist_t persist)
{
	uint16_t reg = ((register_cache[COMMAND_ALS_PERS] & ~ALS_PERS_MASK)
			| (((uint16_t) (persist) << ALS_PERS_SHIFT) & ALS_PERS_MASK));

	register_cache[COMMAND_ALS_PERS] = reg;

	return VEML7700_SendData(COMMAND_ALS_PERS, reg);
}

uint8_t VEML7700_SetPowerSavingMode(als_powmode_t powmode)
{
	uint16_t reg = ((register_cache[COMMAND_PSM] & ~PSM_MASK)
			| (((uint16_t) (powmode) << PSM_SHIFT) & PSM_MASK));

	register_cache[COMMAND_PSM] = reg;

	return VEML7700_SendData(COMMAND_PSM, reg);
}

uint8_t VEML7700_SetPowerSaving(uint8_t enabled)
{
	uint16_t reg = ((register_cache[COMMAND_PSM_EN] & ~PSM_EN_MASK)
			| (((uint16_t) (enabled) << PSM_EN_SHIFT) & PSM_EN_MASK));

	register_cache[COMMAND_PSM_EN] = reg;

	return VEML7700_SendData(COMMAND_PSM_EN, reg);
}

uint8_t VEML7700_SetInterrupts(uint8_t enabled)
{
	uint16_t reg = ((register_cache[COMMAND_ALS_INT_EN] & ~ALS_INT_EN_MASK)
			| (((uint16_t) (enabled) << ALS_INT_EN_SHIFT) & ALS_INT_EN_MASK));

	register_cache[COMMAND_ALS_INT_EN] = reg;

	return VEML7700_SendData(COMMAND_ALS_INT_EN, reg);
}

uint8_t VEML7700_SetPower(uint8_t on)
{
	uint16_t reg = ((register_cache[COMMAND_ALS_SD] & ~ALS_SD_MASK)
			| (((uint16_t) (~on) << ALS_SD_SHIFT) & ALS_SD_MASK));

	register_cache[COMMAND_ALS_SD] = reg;

	uint8_t status = VEML7700_SendData(COMMAND_ALS_SD, reg);

	if (on)
	{
		HAL_Delay(3); // minimu 2.5us delay per datasheet
	}

	return status;
}

uint8_t VEML7700_SetALSHighThreshold(uint16_t thresh)
{
	return VEML7700_SendData(COMMAND_ALS_WH, thresh);
}

uint8_t VEML7700_SetALSLowThreshold(uint16_t thresh)
{
	return VEML7700_SendData(COMMAND_ALS_WL, thresh);
}

uint8_t VEML7700_GetALS(uint16_t *als)
{
	return VEML7700_ReadData(COMMAND_ALS, als);
}

uint8_t VEML7700_GetWhite(uint16_t *white)
{
	return VEML7700_ReadData(COMMAND_WHITE, white);
}

uint8_t VEML7700_GetHighThresholdEvent(uint8_t *event)
{
	uint16_t reg;
	uint8_t status = VEML7700_ReadData(COMMAND_ALS_IF_H, &reg);

	*event = (reg & ALS_IF_H_MASK) >> ALS_IF_H_SHIFT;

	return status;
}

uint8_t VEML7700_GetLowThresholdEvent(uint8_t *event)
{
	uint16_t reg;
	uint8_t status = VEML7700_ReadData(COMMAND_ALS_IF_L, &reg);

	*event = (reg & ALS_IF_L_MASK) >> ALS_IF_L_SHIFT;

	return status;
}

void VEML7700_ScaleLux(uint16_t raw_counts, float *lux)
{
	als_gain_t gain;
	als_itime_t itime;

	VEML7700_GetGain(&gain);
	VEML7700_GetIntegrationTime(&itime);

	float factor1, factor2;

	switch (gain & 0x3)
	{
	case ALS_GAIN_x1:
		factor1 = 1.f;
		break;

	case ALS_GAIN_x2:
		factor1 = 0.5f;
		break;

	case ALS_GAIN_d8:
		factor1 = 8.f;
		break;

	case ALS_GAIN_d4:
		factor1 = 4.f;
		break;

	default:
		factor1 = 1.f;
		break;
	}

	switch (itime)
	{
	case ALS_INTEGRATION_25ms:
		factor2 = 0.2304f;
		break;

	case ALS_INTEGRATION_50ms:
		factor2 = 0.1152f;
		break;

	case ALS_INTEGRATION_100ms:
		factor2 = 0.0576f;
		break;

	case ALS_INTEGRATION_200ms:
		factor2 = 0.0288f;
		break;

	case ALS_INTEGRATION_400ms:
		factor2 = 0.0144f;
		break;

	case ALS_INTEGRATION_800ms:
		factor2 = 0.0072f;
		break;

	default:
		factor2 = 0.2304f;
		break;
	}

	*lux = raw_counts * factor1 * factor2;

	/* Apply correction from App. Note for all readings using Horner's method */
	*lux = *lux * (1.0023f + *lux * (8.1488e-5f + *lux * (-9.3924e-9f + *lux * 6.0135e-13f)));
}

uint8_t VEML7700_GetALSLux(float *lux)
{
	uint16_t raw_counts;
	uint8_t status = VEML7700_GetALS(&raw_counts);

	VEML7700_ScaleLux(raw_counts, lux);

	return status;
}

uint8_t VEML7700_GetWhiteLux(float *lux)
{
	uint16_t raw_counts;
	uint8_t status = VEML7700_GetWhite(&raw_counts);

	VEML7700_ScaleLux(raw_counts, lux);

	return status;
}

uint8_t VEML7700_GetAutoXLux(float *lux, GetRawCountsFunc counts_func, als_gain_t *auto_gain,
		als_itime_t *auto_itime, uint16_t *raw_counts)
{
	als_gain_t gains[5] = { ALS_GAIN_d8, ALS_GAIN_d4, ALS_GAIN_x1, ALS_GAIN_x2, 0 };

	als_itime_t itimes[7] = { ALS_INTEGRATION_25ms, ALS_INTEGRATION_50ms, ALS_INTEGRATION_100ms,
			ALS_INTEGRATION_200ms, ALS_INTEGRATION_400ms, ALS_INTEGRATION_800ms, 0 };

	uint16_t counts_threshold = 200;

	int8_t itime_idx;
	uint8_t gain_idx;

	if (VEML7700_SetPower(0))
	{
		InfoLogAdd(ERRORLEVEL_SEVERE,
		LOG_FUNC_VEML7700, LOG_SUB_NONE,
		LOG_TYPE_SETPOW,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		return STATUS_ERROR;
	}

	for (itime_idx = 2; itime_idx < 6; itime_idx++)
	{
		if (VEML7700_SetIntegrationTime(itimes[itime_idx]))
		{
			InfoLogAdd(ERRORLEVEL_SEVERE,
			LOG_FUNC_VEML7700, LOG_SUB_NONE,
			LOG_TYPE_INT_TIME,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

			return STATUS_ERROR;
		}

		for (gain_idx = 0; gain_idx < 4; gain_idx++)
		{
			if (VEML7700_SetGain(gains[gain_idx]))
			{
				InfoLogAdd(ERRORLEVEL_SEVERE,
				LOG_FUNC_VEML7700, LOG_SUB_NONE,
				LOG_TYPE_GAIN,
				LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

				return STATUS_ERROR;
			}

			if (VEML7700_SetPower(1))
			{
				InfoLogAdd(ERRORLEVEL_SEVERE,
				LOG_FUNC_VEML7700, LOG_SUB_DATA,
				LOG_TYPE_SETPOW,
				LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

				return STATUS_ERROR;
			}

			VEML7700_SampleDelay();

			if (counts_func(raw_counts))
			{
				InfoLogAdd(ERRORLEVEL_SEVERE,
				LOG_FUNC_VEML7700, LOG_SUB_NONE,
				LOG_TYPE_PERSIST,
				LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

				return STATUS_ERROR;
			}

			if (*raw_counts > counts_threshold)
			{
				do
				{
					if (*raw_counts < 10000)
					{
						VEML7700_ScaleLux(*raw_counts, lux);

						*auto_gain = gains[gain_idx];
						*auto_itime = itimes[itime_idx];

						return STATUS_OK;
					}

					if (VEML7700_SetPower(0))
					{
						InfoLogAdd(ERRORLEVEL_SEVERE,
						LOG_FUNC_VEML7700, LOG_SUB_NONE,
						LOG_TYPE_SETPOW,
						LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

						return STATUS_ERROR;
					}

					itime_idx--;

					if (VEML7700_SetIntegrationTime(itimes[itime_idx]))
					{
						InfoLogAdd(ERRORLEVEL_SEVERE,
						LOG_FUNC_VEML7700, LOG_SUB_NONE,
						LOG_TYPE_INT_TIME,
						LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

						return STATUS_ERROR;
					}

					if (VEML7700_SetPower(1))
					{
						InfoLogAdd(ERRORLEVEL_SEVERE,
						LOG_FUNC_VEML7700, LOG_SUB_DATA,
						LOG_TYPE_SETPOW,
						LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

						return STATUS_ERROR;
					}

					VEML7700_SampleDelay();

					if (counts_func(raw_counts))
					{
						InfoLogAdd(ERRORLEVEL_SEVERE,
						LOG_FUNC_VEML7700, LOG_SUB_NONE,
						LOG_TYPE_PERSIST,
						LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

						return STATUS_ERROR;
					}

				} while (itime_idx > 0);

				VEML7700_ScaleLux(*raw_counts, lux);

				*auto_gain = gains[gain_idx];
				*auto_itime = itimes[itime_idx];

				return STATUS_OK;
			}

			if (VEML7700_SetPower(0))
			{
				InfoLogAdd(ERRORLEVEL_SEVERE,
				LOG_FUNC_VEML7700, LOG_SUB_NONE,
				LOG_TYPE_SETPOW,
				LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

				return STATUS_ERROR;
			}
		}
	}

	VEML7700_ScaleLux(*raw_counts, lux);

	*auto_gain = gains[gain_idx];
	*auto_itime = itimes[itime_idx];

	return STATUS_OK;
}

uint8_t VEML7700_GetAutoALSLux(float *lux)
{
	als_gain_t auto_gain;
	als_itime_t auto_itime;
	uint16_t raw_counts;
	uint8_t status = VEML7700_GetAutoXLux(
			lux,
			&VEML7700_GetALS,
			&auto_gain,
			&auto_itime,
			&raw_counts);
	if (*lux > 120000)
	{
		*lux = 120000;
	}
	printf(
			"Gain = %s, iTime = %s, Count = %d\r\n",
			(auto_gain == 0) ? "1" : (auto_gain == 1) ? "2" : (auto_gain == 2) ? "1/8" : "1/4",
			(auto_itime == 0xc) ? "25ms" : (auto_itime == 0x8) ? "50ms" :
			(auto_itime == 0) ? "100ms" : (auto_itime == 1) ? "200ms" :
			(auto_itime == 2) ? "400ms" : "800ms",
			raw_counts);

	return status;
}

uint8_t VEML7700_GetAutoWhiteLux(float *lux)
{
	als_gain_t auto_gain;
	als_itime_t auto_itime;
	uint16_t raw_counts;

	return VEML7700_GetAutoXLux(lux, &VEML7700_GetWhite, &auto_gain, &auto_itime, &raw_counts);
}

//uint8_t VEML7700_GetAutoALSLux(float *lux,
//              als_gain_t *auto_gain,
//              als_itime_t *auto_itime,
//              uint16_t *raw_counts)
//{
//	return VEML7700_GetAutoXLux(lux,
//					 VEML7700_GetALS,
//					 auto_gain,
//					 auto_itime,
//					 raw_counts);
//}
//
//uint8_t VEML7700_GetAutoWhiteLux(float *lux, als_gain_t *auto_gain, als_itime_t *auto_itime, uint16_t *raw_counts)
//{
//	return VEML7700_GetAutoXLux(lux,
//					 VEML7700_GetWhite,
//					 auto_gain,
//					 auto_itime,
//					 raw_counts);
//}

uint8_t VEML7700_SampleDelay(void)
{
	als_itime_t itime;
	uint8_t status = VEML7700_GetIntegrationTime(&itime);

	switch (itime) {
	case ALS_INTEGRATION_25ms:
		EXTENDEN_DELAY(25);
		break;

	case ALS_INTEGRATION_50ms:
		EXTENDEN_DELAY(50);
		break;

	case ALS_INTEGRATION_100ms:
		EXTENDEN_DELAY(100);
		break;

	case ALS_INTEGRATION_200ms:
		EXTENDEN_DELAY(200);
		break;

	case ALS_INTEGRATION_400ms:
		EXTENDEN_DELAY(400);
		break;

	case ALS_INTEGRATION_800ms:
		EXTENDEN_DELAY(800);
		break;

	default:
		EXTENDEN_DELAY(100);
		break;
	}

	return status;
}
