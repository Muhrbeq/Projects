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

void SetState(OPT3001 *device, OPT3001_State oState)
{
	device->tState = oState;
}

static uint8_t OPT3001_Startup(OPT3001 *device)
{
	return HAL_OK;
}

static uint8_t OPT3001_Init(OPT3001 *device)
{
	return HAL_OK;
}

static uint8_t OPT3001_Shutdown(OPT3001 *device)
{
	return HAL_OK;
}

static uint8_t OPT3001_ReadingReady(OPT3001 *device)
{
	return HAL_OK;
}

static uint8_t OPT3001_GetLux(OPT3001 *device)
{
	return HAL_OK;
}
