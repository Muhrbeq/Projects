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

void DualLed_RedOn(void);
void DualLed_RedOff(void);
void DualLed_GreenOn(void);
void DualLed_GreenOff(void);
void DualLed_ToggleRed(void);
void DualLed_ToggleGreen(void);
void DualLed_BlinkGreen(uint8_t amount);
void DualLed_BlinkRed(uint8_t amount);

#endif /* LOWLEVEL_H_ */
