/*
 * LowLevel.h
 *
 *  Created on: 15 mars 2019
 *      Author: Rasmus.Muhrbeck
 */

#ifndef UTILITIES_LOWLEVEL_H_
#define UTILITIES_LOWLEVEL_H_

#include "Global.h"

#include "main.h"

void SystemPower_Config(void);
void GPIO_Set_Analog(void);
void EnterStopMode(void);
void EnterStopRadioNetwork(void);

void InitAll(void);
void SystemClock_Config(void);
void GPIOSetAnalogSensor(void);
void SleepForApproxMs(uint32_t mS);
void SleepForApproxMsSpectra(uint32_t mS);
void SystemSleepAndWakeUp(void);
void DisableSD_Card(void);



#endif /* UTILITIES_LOWLEVEL_H_ */
