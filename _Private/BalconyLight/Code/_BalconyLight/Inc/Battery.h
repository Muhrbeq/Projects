/*
 * Battery.h
 *
 *  Created on: 31 aug. 2020
 *      Author: Rasmus.Muhrbeck
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#include "main.h"

#define VBAT_MAX	 				4200	//Max for Li-battery
#define VBAT_MIN					3400	//Min for Li-battery
#define RESISTOR_1_VDIV				1100
#define RESISTOR_2_VDIV				2200
#define VOLTAGE_DIVIDER 			RESISTOR_2_VDIV/(RESISTOR_1_VDIV + RESISTOR_2_VDIV)
#define ADC_12_BIT					4095	//ADC is 12 bit
#define V3V3MAX						3000	//Max on VBAT-sense

void EnableBatterySensePin(void);
void DisableBatterySensePin(void);
uint8_t BatteryMeasure_Init(void);
uint16_t Battery_GetLevel_Raw(void);
/* Get level of battery */
uint8_t Battery_GetLevel(void);

#endif /* BATTERY_H_ */
