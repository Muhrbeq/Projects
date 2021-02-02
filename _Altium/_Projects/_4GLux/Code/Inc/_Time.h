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
void Time_RTCSetDateTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
uint8_t Time_RTCSetAlarmA(uint32_t seconds);
uint8_t Time_RTCSetAlarmB(uint32_t seconds);
void GetTime(Time *s);
void printTime(Time *s);


#endif /* TIME_H_ */
