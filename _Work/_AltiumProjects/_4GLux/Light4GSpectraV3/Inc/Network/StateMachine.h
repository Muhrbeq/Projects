/*
 * StateMachine.h
 *
 *  Created on: 18 jan. 2019
 *      Author: Rasmus.Muhrbeck
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "Global.h"



typedef enum RadioStates
{
	RADIO_IDLE = 0,
	RADIO_AWAKE,
	RADIO_STARTUP,
	RADIO_INIT,
	RADIO_NETWORK,
	RADIO_RSSI,
	RADIO_CLOCK,
	RADIO_SETSERVER,
	RADIO_CONNECT,
	RADIO_PUSHDATA,
	RADIO_SUBSCRIBE,
	RADIO_SERVERDO,
	RADIO_TODO,
	RADIO_NOTADJUSTED,
	RADIO_POWEROFF,
	RADIO_SENDFLASH,
	RADIO_CHECKPOWER,
	RADIO_DISCONNECT,


} RadioStates;

/* Radio functions */
void RadioResetState(Error *er, Data *data);
RadioStates SetRadioState(RadioStates rState);
RadioStates GetRadioState(void);
void RadioFirstRunFinished(void);
void RadioStateMachine(Data *data, Error *er);

typedef enum SensorStates
{
	SENSOR_IDLE = 0,
	SENSOR_AWAKE,
	SENSOR_INIT,
	SENSOR_FIRST,
	SENSOR_MEASURE,
	SENSOR_SLEEP,
	SENSOR_DATAPUSHED,
	SENSOR_DATAPUSHFAILED,
	SENSOR_ERROR,
	SENSOR_DAYSPASSED,


} SensorStates;

/* Sensor functions */
void SensorResetState(void);
SensorStates SetSensorState(SensorStates sState);
SensorStates GetSensorState(void);
void SensorStateMachine(Data *data, Error *er);

/* Sleep state machine */
void SleepStateMachine();

#endif /* STATEMACHINE_H_ */
