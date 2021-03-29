/*
 * LowLevel.h
 *
 *  Created on: 25 nov. 2020
 *      Author: Rasmus.Muhrbeck
 */

#ifndef LOWLEVEL_H_
#define LOWLEVEL_H_

#include "main.h"
#include "__ExegerGeneric.h"
#include "rtc.h"

//void SystemClock_Config(void);
void InitPeripherals(void);
uint8_t CheckResetCause(void);
void SystemPower_Config(void);
void SystemSleepAndWakeUp(void);
void SleepMax2s(uint32_t msToSleep);

#endif /* LOWLEVEL_H_ */
