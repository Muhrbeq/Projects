/*
 * _Time.c
 *
 *  Created on: 25 jan. 2021
 *      Author: rasmu
 */

#include "time.h"
#include "_Time.h"
#include "rtc.h"


/* Returns a compressed date time */
uint32_t Time_RTCGetDateTime(void)
{
	RTC_DateTypeDef dT;
	RTC_TimeTypeDef tT;

	/* return variable */
	uint32_t dateTime = 0;

	/* Get time from RTC */
	HAL_RTC_GetTime(&hrtc, &tT, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &dT, RTC_FORMAT_BIN);

	/* Bit-shift */
	dateTime = (uint32_t) (0b111111 & dT.Year) << 26;
	dateTime |= (uint32_t) (0b1111 & dT.Month) << 22;
	dateTime |= (uint32_t) (0b11111 & dT.Date) << 17;
	dateTime |= (uint32_t) (0b11111 & tT.Hours) << 12;
	dateTime |= (uint32_t) (0b111111 & tT.Minutes) << 6;
	dateTime |= (uint32_t) (0b111111 & tT.Seconds);

	return dateTime;
}

/* Set alarm A */
uint8_t Time_RTCSetAlarmA(uint32_t seconds)
{
	RTC_AlarmTypeDef sAlarm;
	struct tm time;
	RTC_DateTypeDef ds;
	RTC_TimeTypeDef ts;

	/* Fetch time from rtc */
	HAL_RTC_GetTime(&hrtc, &ts, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &ds, RTC_FORMAT_BIN);

	/* Assign to time struct */
	time.tm_year = ds.Year + 100;
	time.tm_mon = ds.Month - 1;
	time.tm_mday = ds.Date;
	time.tm_hour = ts.Hours;
	time.tm_min = ts.Minutes;
	time.tm_sec = ts.Seconds + seconds;

	/* make the time correct */
	mktime(&time);

	/*
	 *	The date, month and year doesn't matter. The alarm should go off every day no matter what day it is...
	 */
	sAlarm.AlarmTime.Hours = time.tm_hour;
	sAlarm.AlarmTime.Minutes = time.tm_min;
	sAlarm.AlarmTime.Seconds = time.tm_sec;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = time.tm_mday;
	/* Alarm A */
	sAlarm.Alarm = RTC_ALARM_A;

	/* Check if Setalarm works */
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

/* Set alarm B*/
uint8_t Time_RTCSetAlarmB(uint32_t seconds)
{
	RTC_AlarmTypeDef sAlarm;
	struct tm time;
	RTC_DateTypeDef ds;
	RTC_TimeTypeDef ts;

	/* Fetch time from rtc */
	HAL_RTC_GetTime(&hrtc, &ts, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &ds, RTC_FORMAT_BIN);

	/* Assign to time struct */
	time.tm_year = ds.Year + 100;
	time.tm_mon = ds.Month - 1;
	time.tm_mday = ds.Date;
	time.tm_hour = ts.Hours;
	time.tm_min = ts.Minutes;
	time.tm_sec = ts.Seconds + seconds;

	/* make the time correct */
	mktime(&time);

	/*
	 *	The date, month and year doesn't matter. The alarm should go off every day no matter what day it is...
	 */
	sAlarm.AlarmTime.Hours = time.tm_hour;
	sAlarm.AlarmTime.Minutes = time.tm_min;
	sAlarm.AlarmTime.Seconds = time.tm_sec;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = time.tm_mday;
	/* Alarm B */
	sAlarm.Alarm = RTC_ALARM_B;

	/* Check if Setalarm works */
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

/* Get current MCU time */
void GetTime(Time *s)
{

	RTC_DateTypeDef sD;
	RTC_TimeTypeDef tS;
	/* Get MCU time */
	HAL_RTC_GetTime(&hrtc, &tS, RTC_FORMAT_BIN);

	/* Get MCU date*/
	HAL_RTC_GetDate(&hrtc, &sD, RTC_FORMAT_BIN);

	/* Set year of struct */
	s->Year = sD.Year;

	/* Set month of struct */
	s->Month = sD.Month;

	/* Set day of struct */
	s->Day = sD.Date;

	/* Set hour of struct */
	s->Hour = tS.Hours;

	/* Set minute of struct */
	s->Minute = tS.Minutes;

	/* Set second of struct */
	s->Second = tS.Seconds;

	printTime(s);
}

/* */
void printTime(Time *s)
{
	printf("%d/%d/%d - %d:%d:%d\r\n", s->Year, s->Month, s->Day, s->Hour, s->Minute, s->Second);
}
