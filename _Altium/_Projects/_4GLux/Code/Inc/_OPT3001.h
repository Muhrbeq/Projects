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
	OPT3001_IDLE,
	OPT3001_INITIATE,
	OPT3001_IDWRITE,
	OPT3001_IDREAD,
	OPT3001_REQUESTREADING,
	OPT3001_READINGREADYWRITE,
	OPT3001_READINGREADYREAD,
	OPT3001_GETLUXWRITE,
	OPT3001_GETLUXREAD,
	OPT3001_SHUTDOWN,
	OPT3001_DONE,
	OPT3001_ERROR,
}OPT3001_State;

typedef struct OPT3001
{
	OPT3001_State tState;
	I2C_HandleTypeDef hi2c;
	uint8_t i2cWrite;
	uint8_t i2cRead;

	SensorStatus sStatus;
	uint8_t errorCounter;

	float currentLux;
}OPT3001;

void OPT3001_InitDevice(OPT3001 *device, I2C_HandleTypeDef *hi2c, uint8_t devWriteAddr, uint8_t devReadAddr);
uint8_t OPT3001_GetDeviceID(OPT3001 *device);
void OPT3001_StateMachine(OPT3001 *device);

#endif /* INC_OPT3001_H_ */
