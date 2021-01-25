/*
 * _Time.h
 *
 *  Created on: 25 jan. 2021
 *      Author: rasmu
 */

#ifndef TIME_H_
#define TIME_H_

#include "__ExegerGeneric.h"
#include "_Global.h"

uint32_t Time_RTCGetDateTime(void);
uint8_t Time_RTCSetAlarmA(uint32_t seconds);
uint8_t Time_RTCSetAlarmB(uint32_t seconds);
void GetTime(Time *s);
void printTime(Time *s);


#endif /* TIME_H_ */
