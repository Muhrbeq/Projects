/*
 * Battery.h
 *
 *  Created on: 13 Sep 2018
 *      Author: andreas.fredlund
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#include "Global.h"

/* Defines for the battery calculations */
#define VBAT_MAX	 				4200	//Max for Li-battery
#define VBAT_MIN					3400	//Min for Li-battery
#define RESISTOR_1_VDIV				1100
#define RESISTOR_2_VDIV				2200
#define VOLTAGE_DIVIDER 			RESISTOR_2_VDIV/(RESISTOR_1_VDIV + RESISTOR_2_VDIV)
#define ADC_12_BIT					4095	//ADC is 12 bit
#define V3V3MAX						3000	//Max on VBAT-sense

void Battery_init(void);
uint16_t Battery_GetLevel_Raw(void);
uint8_t Battery_GetLevel(void);
void MX_ADC_DeInit(void);

#endif /* BATTERY_H_ */
