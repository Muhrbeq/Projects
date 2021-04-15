/*
 * OPT3001.h
 *
 *  Created on: Oct 28, 2019
 *      Author: tomas.forsen
 */

#ifndef INC_OPT3001_H_
#define INC_OPT3001_H_

#include "main.h"
#include "_Global.h"

#define OPT_3001_OK 100
#define OPT_3001_DATA_READY 128
#define OPT_3001_Error 200
#define OPT_3001_Wait 220
#define OPT_3001_REQUEST_SENT 254
#define OPT_3001_NO_NEW_DATA 255

typedef enum OPT3001_State
{
	OPT3001_STARTUP,
	OPT3001_INITIATE,
	OPT3001_SHUTDOWN,
}OPT3001_State;

typedef struct OPT3001
{
	OPT3001_State tState;
	I2C_HandleTypeDef hi2c;
	uint8_t i2cWrite;
	uint8_t i2cRead;

	SensorStatus sStatus;
	uint8_t errorCounter;

	uint16_t iMantissa;
	uint16_t iExponent;
	float currentLux;
}OPT3001;

void OPT_3001_shutdown(void);
uint8_t OPT_3001_init_for_single_shot(void);
uint8_t OPT_3001_request_reading(void);
uint8_t OPT_3001_reading_ready(void);
uint8_t OPT_3001_get_reading(float* lux_ptr);
uint8_t all_sensors_have_data_ready(void);
#endif /* INC_OPT3001_H_ */
