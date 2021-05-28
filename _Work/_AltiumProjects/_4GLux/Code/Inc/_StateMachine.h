/*
 * _StateMachine.h
 *
 *  Created on: 5 jan. 2021
 *      Author: rasmu
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "_Global.h"

typedef enum RadioStates
{
	RADIO_IDLE,
	RADIO_AWAKE,
	RADIO_STARTUP,
	RADIO_INIT,
	RADIO_NETWORK,
	RADIO_NETWORK_STRENGTH,
	RADIO_CLOCK,
} RadioStates;

/* Radio functions */
void RadioResetState(Error *er, Data *data);
RadioStates SetRadioState(RadioStates rState);
RadioStates GetRadioState(void);
void RadioFirstRunFinished(void);
void RadioStateMachine(Data *data, Error *er);

typedef enum SensorStates
{
	SENSOR_IDLE,
	SENSOR_AWAKE,
	SENSOR_INIT,
	SENSOR_FIRST_MEASURE,
	SENSOR_MEASURE,
	SENSOR_SLEEP,
} SensorStates;

/* Sensor functions */
void SensorResetState(void);
SensorStates SetSensorState(SensorStates sState);
SensorStates GetSensorState(void);
void SensorStateMachine(Data *data, Error *er);



#endif /* STATEMACHINE_H_ */
