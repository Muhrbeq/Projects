/*
 * TimeFunc.h
 *
 *  Created on: 12 feb. 2019
 *      Author: Rasmus.Muhrbeck
 */

#ifndef UTILITIES_TIMEFUNC_H_
#define UTILITIES_TIMEFUNC_H_

int32_t RTC_GetElapsedSeconds(Time *startTime, Time *endTime);
void GetTime(Time *s);
uint16_t CalculateAverage(uint16_t *get, uint8_t lenBuff);
uint8_t EvaluatePeaks(uint16_t *get, uint8_t lenBuff, uint16_t minDiff);
uint8_t AnalyzeData();
uint8_t ConfigureSendTime(int year, int month, int day, int hour, int minute,
		int second, int adjustHour, Time *tS);
uint8_t TimeToSend(Time *sT, Time *eT, Data *data);
void printTime(Time *s);

#endif /* UTILITIES_TIMEFUNC_H_ */
