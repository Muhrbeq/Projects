/*
 * TimeFunc.c
 *
 *  Created on: 12 feb. 2019
 *      Author: Rasmus.Muhrbeck
 */
#include "Global.h"
#include "TimeFunc.h"
#include "StateMachine.h"

#include "sara.h"

#include "LowLevel.h"

#include "MultiSensor.h"

#include "SDCard.h"

//extern struct radioFlags radioErrors;
//extern SaraConfig SaraConfiguration;
//RTC_DateTypeDef sDate;
//RTC_TimeTypeDef timeStruct;

#define lenBuffAD 48 //2 Days worth of data (Not used now)

uint8_t firstRun = 1;

/* Gets amount of seconds between the two structs */
int32_t RTC_GetElapsedSeconds(Time *startTime, Time *endTime)
{
	int32_t startSeconds = 0;
	int32_t endSeconds = 0;
	int32_t totalSeconds = 0;

	printf("-----------------\r\n");
	printTime(startTime);
	printf("-----------------\r\n");
	printTime(endTime);
	printf("-----------------\r\n");

	/* Calculating seconds of first struct (StartStruct) */
	startSeconds = (int32_t) (startTime->Year) * 365 * 24 * 60 * 60;
	startSeconds += (int32_t) startTime->Month * 30 * 24 * 60 * 60;
	startSeconds += (int32_t) startTime->Day * 24 * 60 * 60;
	startSeconds += (int32_t) startTime->Hour * 60 * 60;
	startSeconds += (int32_t) startTime->Minute * 60;
	startSeconds += (int32_t) startTime->Second;

	/* Calculating seconds of Last struct (EndStruct) */
	endSeconds = (int32_t) (endTime->Year) * 365 * 24 * 60 * 60;
	endSeconds += (int32_t) endTime->Month * 30 * 24 * 60 * 60;
	endSeconds += (int32_t) endTime->Day * 24 * 60 * 60;
	endSeconds += (int32_t) endTime->Hour * 60 * 60;
	endSeconds += (int32_t) endTime->Minute * 60;
	endSeconds += (int32_t) endTime->Second;

	/* Calculate the difference in seconds */
	totalSeconds = endSeconds - startSeconds;

	return totalSeconds;
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

/* Configures when it is time to activate the Radio */
uint8_t TimeToSend(Time *sT, Time *eT, Data *data)
{
	SaraConfig *sC = GetSaraConfigurationStruct();

	/* Get MCU clock */
	GetTime(eT);

	/* Calculate elapsed seconds between last connected and now */
	int32_t elapsedSec = RTC_GetElapsedSeconds(sT, eT);

//	/* If the adjustment from LuxAnalysis wants to shift > 2 hours */
//	/* This is not used at the moment due to the function now being optimized */
//	if (abs(24 - rE->adjustedHours) > 2 && rE->adjustedHours > 2)
//	{
//
//		/* Configure the send time by adjusting the MCU clock */
//		ConfigureSendTime((int) eT->Year, (int) eT->Month, (int) eT->Day, (int) eT->Hour,
//				(int) eT->Minute,
//				(int) eT->Second,
//				(int) rE->adjustedHours,
//				eT);
//
//		/* If adjusted MCU clock is equal to hour to send */
//		if (eT->Hour == sC->hourToSend
//				&& abs(elapsedSec > TIMEOUT_BEFORE_START_AGAIN))
//		{
//			debugPrint(&hlpuart1, "lC");
//
////			DisEnableSDPin(1);
//
//
//			/* Start Radio */
//			return SetRadioState(RADIO_STARTUP);
//		}
//	}
//	/* Lux analysis is within 2 hours */


		/* If MCU clock is equal to hour to send */
	if (eT->Hour == sC->hourToSend && abs(elapsedSec > 3600))
//		if (abs(elapsedSec > 30))
	{
		debugPrint(&hlpuart1, "nC");

		/* Start radio up */
		return SetRadioState(RADIO_AWAKE);
	}
	/* If its not time to send */
	return false;
}

/* Copy one time struct to another */
uint8_t CopyTime(Time *to, Time *from)
{
	if (!to || !from)
		return false;

	/* Copy structs */
	memcpy(to, from, sizeof(Time));
	return SUCCESS;
}

/*
 *
 * Below functions are NOT USED at the moment.
 *
 * */


/* Not used at the moment (Used with AnalyzeDataWithLux) */
uint16_t CalculateAverage(uint16_t *get, uint8_t lenBuff)
{
	/* Init storage for sum of data */
	uint32_t sumData = 0;

	/* Loop through *get buffer */
	for (int i = 0; i < lenBuff; i++)
	{
		/* Gather the sum of all data */
		sumData = sumData + get[i];
	}

	/* Return uint16_t average */
	return sumData / lenBuff;
}

/* Not used at the moment (Used with AnalyzeDataWithLux) */
uint8_t EvaluatePeaks(uint16_t *get, uint8_t lenBuff, uint16_t minDiff)
{
	/* Initial lowest point */
	uint16_t lowestPoint = 20000;

	/* Initial highest value */
	uint16_t highestPoint = 0;

	/* Loop through buffer */
	for (uint8_t i = 0; i < lenBuff; i++)
	{
		/* The lowest point is lower than previous measurements */
		if (get[i] < lowestPoint)
		{
			/* Store lowest point */
			lowestPoint = get[i];
		}

		/* The highest point is higher than previous measurements */
		if (get[i] > highestPoint)
		{
			/* Store highest point */
			highestPoint = get[i];
		}
	}

	/* If we have similar lux data during the whole day, discard */
	if (highestPoint - lowestPoint < minDiff)
	{
		/* Trust networkClock instead */
		return false;
	}

	/* Peaks differ enough to compute */
	return true;
}

/* AnalyzeDataWithLux */
//
//uint8_t AnalyzeData()
//{
//	/* Store the data in an array */
//	uint16_t analyzeData[lenBuffAD] = { 0 };
//
//	/* Average of data in analyzeData */
//	uint16_t averageData;
//
//	/* Counter of index in analyzeData */
//	uint32_t counterIndex = 0;
//
//	/* Which day are we on? */
//	uint8_t day = 0;
//
//	/* Look for the day that are going to be written on now */
//	for (uint8_t i = 1; i < 9; i++)
//	{
//		if (Flash_GetAddressDayX(i)
//				== Flash_GetNextAddress(FLASH_DATA_SETUP_ADDR))
//		{
//			/* We want the last 2 days */
//			day = i - 2;
//
//			/* Don't want to step outside */
//			if (day == -1)
//			{
//				day = 6;
//			}
//
//			/* Same here (day 0 is FLASH_DATA_SETUP_ADDR) */
//			else if (day == 0)
//			{
//				day = 7;
//			}
//			break;
//		}
//	}
//
//	for (uint8_t i = 0; i < 2; i++)
//	{
//		/* Address of day to start analyze */
//		uint32_t Address = Flash_GetAddressDayX(day);
//
//		/* Get data struct size */
//		uint32_t size = GetDataSize(GetDataStruct()); // size in bytes of the collected data from sensors.
//
//		/* How much is not filled with zero? */
//		uint32_t sizeOfDataToRead = Flash_EOF(Address, size);
//
//		/* Read Lux values and put them in analyzeData */
//		FlashReadLux(Address, Address + 4 * sizeOfDataToRead, analyzeData,
//				&counterIndex, lenBuffAD);
//
//		/* Step forward to next day */
//		day++;
//
//		/* Dont step outside */
//		if (day > 7)
//		{
//			day = 1;
//		}
//	}
//
//	if (counterIndex < 36)
//	{
//		return false;
//	}
//
//	/* Calculate average of the analyzeData array of lenBuff long */
//	averageData = CalculateAverage(analyzeData, lenBuffAD);
//
//	/* Evaluate if the peaks are far enough apart */
//	if (!EvaluatePeaks(analyzeData, lenBuffAD, 1500))
//	{
//		return false;
//	}
//
//	/* Array to store when the data goes over the average value */
//	uint8_t crossMed[16] = { 0 };
//
//	/* Loop through the analyzeData array */
//	for (uint8_t i = 1, j = 0; i < lenBuffAD; i++)
//	{
//		/* Check if the values have passed over the average line */
//		if ((analyzeData[i - 1] < averageData && analyzeData[i] > averageData)
//				|| (analyzeData[i - 1] > averageData
//						&& analyzeData[i] < averageData))
//		{
//
//			/*  Representation of "crossMed"
//			 *
//			 * Lux
//			 * ^
//			 * |                  ___________ Lux values
//			 * |    -----        /
//			 * |   /     \      /
//			 * |--/-------\----/-------------  Average
//			 * | /|       |---- |
//			 * |/ |       |     |
//			 * |--|-------|-----|------------> Time
//			 * crossMed[0]|  crossMed[2]
//			 *         crossMed[1]
//			 *
//			 *
//			 *   crossMed[0] and [2] is highCross
//			 *    while crossMed[2] is lowCross*/
//
//			/* Add hour of crossing average in crossMed */
//			crossMed[j] = i;
////			printf("crossMed[%d] = %d\r\n", j, i);
//
//			/* J + 1 */
//			j++;
//		}
//	}
//
//	/* Variables for when the lux values passes average on the way up (highCross) */
//	uint8_t highCrossAvg = 0;
//
//	/* or down (lowCross)  */
//	uint8_t lowCrossAvg = 0;
//
//	/* If crossMed[0] is bigger than [1], -> every even number is a high period */
//	if (analyzeData[crossMed[0]] > analyzeData[crossMed[1]])
//	{
//		for (int i = 16; i > 0; i = i - 2)
//		{
//			/* If the measured points are 24h +- 3h, calculate as a day */
//			if (crossMed[i] - crossMed[i - 2] > 20
//					&& crossMed[i] - crossMed[i - 2] < 28)
//			{
//				/* Acquire the latest lowCross */
//				lowCrossAvg = crossMed[i - 1];
//
//				/* Acquire the latest highCross */
//				highCrossAvg = crossMed[i];
//
//				/* Variable to store when its suggested to send */
//				uint8_t sendTime;
//
//				/* Calculate where a low is */
//				sendTime = lowCrossAvg + (highCrossAvg - lowCrossAvg) / 2;
//
//				/* Now - sendTime will give the suggested hour to send */
//				radioErrors.adjustedHours = lenBuffAD - sendTime;
//
//				/* Keep it within 24 hours */
//				if (radioErrors.adjustedHours > 24)
//				{
//					radioErrors.adjustedHours = radioErrors.adjustedHours - 24;
//				}
//
//				return true;
//			}
//		}
//	}
//
//	else
//	{
//		for (int i = 16; i > 0; i = i - 2)
//		{
//			/* If the measured points are 24h +- 3h, calculate as a day */
//			if (crossMed[i] - crossMed[i - 2] > 20
//					&& crossMed[i] - crossMed[i - 2] < 28)
//			{
//				/* Acquire the latest lowCross */
//				lowCrossAvg = crossMed[i];
//
//				/* Acquire the latest highCross */
//				highCrossAvg = crossMed[i - 1];
//
//				/* used to calculate how long a night is */
//				uint8_t firstCross = crossMed[i - 2];
//
//				/* Variable to store when its suggested to send */
//				uint8_t sendTime;
//
//				/* Calculate where a low is */
//				sendTime = lowCrossAvg + (highCrossAvg - firstCross) / 2;
//
//				/* Now - sendTime will give the suggested hour to send */
//				radioErrors.adjustedHours = abs(lenBuffAD - sendTime);
//
//				/* Keep it within 24 hours */
//				if (radioErrors.adjustedHours > 24)
//				{
//					radioErrors.adjustedHours = radioErrors.adjustedHours - 24;
//				}
//
//				return true;
//			}
//		}
//	}
//
//	return false;
//}

/* Not used at the moment (Used with AnalyzeDataWithLux) */
uint8_t ConfigureSendTime(int year, int month, int day, int hour, int minute,
		int second, int adjustHour, Time *tS)
{
	/* Init tm struct (to use mktime) */
	struct tm timeSendData;

	/* Write adjustHour to local variable */
	int addHour = adjustHour;

	/* If the adjustHour is bigger than 12 hours, add instead of subtract */
	if (addHour > 12)
	{
		timeSendData.tm_year = 100 + year;
		timeSendData.tm_mon = month - 1;
		timeSendData.tm_mday = day;
		timeSendData.tm_hour = hour + 12 - (addHour - 12);
		timeSendData.tm_min = minute;
		timeSendData.tm_sec = second;
		timeSendData.tm_isdst = 0;
		timeSendData.tm_wday = 0;
		timeSendData.tm_yday = 0;
	}
	/* Else, subtract the hour */
	else
	{
		timeSendData.tm_year = 100 + year;
		timeSendData.tm_mon = month - 1;
		timeSendData.tm_mday = day;
		timeSendData.tm_hour = hour - addHour;
		timeSendData.tm_min = minute;
		timeSendData.tm_sec = second;
		timeSendData.tm_isdst = 0;
		timeSendData.tm_wday = 0;
		timeSendData.tm_yday = 0;
	}

	/* Adjust time based on adjustHour */
	mktime(&timeSendData);

	/* Set timestruct based on adjusted values */
	tS->Year = (uint8_t) ((timeSendData.tm_year - 100) & 0xFF);
	tS->Month = (uint8_t) ((timeSendData.tm_mon + 1) & 0xFF);
	tS->Day = (uint8_t) (timeSendData.tm_mday & 0xFF);
	tS->Hour = (uint8_t) (timeSendData.tm_hour & 0xFF);
	tS->Minute = (uint8_t) (timeSendData.tm_min & 0xFF);
	tS->Second = (uint8_t) (timeSendData.tm_sec & 0xFF);

	return true;
}



