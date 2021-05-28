/*
 * MultiSensor.h
 *
 *  Created on: 5 Dec 2018
 *      Author: andreas.fredlund
 */

#ifndef MULTISENSOR_H_
#define MULTISENSOR_H_

#include "Global.h"

void MultiSensor_takeMeasurements(SensorData *sd);
void MultiSensor_takeMeasurementsRaw(SensorData *sd);


uint32_t GetDataSize(Data *d);
void DataStruct_init(Data *data);
void PrintSensorData(Data *data);

#endif /* MULTISENSOR_H_ */
