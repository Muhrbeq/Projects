/*
 * Error.c
 *
 *  Created on: 17 Oct 2018
 *      Author: andreas.fredlund
 */


#include "Global.h"
#include "Error.h"
#include "TimeFunc.h"
#include "StateMachine.h"
#include "sara.h"

#include "SDCard.h"

/*
 * Initialize the error struct
 */
void ErrorStruct_init(Error *er)
{
	er->errorCounter = 0;

	for (int i = 0; i < ERROR_LOGG_ARRAY_SIZE; i++)
	{
		er->errorLog[0].function = 0;
		er->errorLog[0].subFunction = 0;
		er->errorLog[0].type = 0;
		er->errorLog[0].status = 0;
		er->errorLog[0].valueA = 0;
		er->errorLog[0].valueB = 0;
		er->errorLog[0].timestamp = 0;
		er->errorLog[0].logLevel = 0;
	}
}

/*
 * Adds an error the the errorlog
 * Todo implement a system for the logLevel
 */
void ErrorAddLog(uint8_t function, uint8_t subFunction, uint8_t type, uint8_t status, uint32_t valueA, uint32_t valueB, uint8_t logLevel, Error *er)
{

	/* If the buffer is full this error should be written */
	if (er->errorCounter >= ERROR_LOGG_ARRAY_SIZE)
	{
		er->errorCounter = ERROR_LOGG_ARRAY_SIZE - 1;
		er->errorLog[er->errorCounter].function = LOG_FUNC_ERROR;
		er->errorLog[er->errorCounter].subFunction = LOG_SUB_ADD;
		er->errorLog[er->errorCounter].type = LOG_TYPE_BUFFER_FULL;
		er->errorLog[er->errorCounter].status = LOG_STATUS_FAIL;
		er->errorLog[er->errorCounter].valueA = 0;
		er->errorLog[er->errorCounter].valueB = 0;
		er->errorLog[er->errorCounter].logLevel = 0;
		er->errorLog[er->errorCounter].timestamp = RTC_Get_DateTime_Comp();
	}
	else
	{
		er->errorLog[er->errorCounter].function = function;
		er->errorLog[er->errorCounter].subFunction = subFunction;
		er->errorLog[er->errorCounter].type = type;
		er->errorLog[er->errorCounter].status = status;
		er->errorLog[er->errorCounter].valueA = valueA;
		er->errorLog[er->errorCounter].valueB = valueB;
		er->errorLog[er->errorCounter].timestamp = RTC_Get_DateTime_Comp();
		er->errorLog[er->errorCounter].logLevel = logLevel;

		er->errorCounter++;
	}
}

/*
 * Initialize the info struct
 */
void InitInfoStruct(Info *iL)
{
	iL->infoCounter = 0;
	iL->highestError = 0;

	for (int i = 0; i < INFO_LOGG_ARRAY_SIZE; i++)
	{
		iL->infoLog[i].function = 0;
		iL->infoLog[i].subFunction = 0;
		iL->infoLog[i].type = 0;
		iL->infoLog[i].status = 0;
		iL->infoLog[i].timestamp = 0;
		iL->infoLog[i].errorLevel = 0;
		iL->infoLog[i].valueA = 0;
		iL->infoLog[i].valueB = 0;
		iL->infoLog[i].valueC = 0;
	}
}

/*
 * Re-Initialize/Reset the error struct
 */
void ResetInfoStruct(Info *iL)
{
	iL->infoCounter = 0;
	iL->highestError = 0;

	for (int i = 0; i < INFO_LOGG_ARRAY_SIZE; i++)
	{
		iL->infoLog[i].function = 0;
		iL->infoLog[i].subFunction = 0;
		iL->infoLog[i].type = 0;
		iL->infoLog[i].status = 0;
		iL->infoLog[i].timestamp = 0;
		iL->infoLog[i].errorLevel = 0;
		iL->infoLog[i].valueA = 0;
		iL->infoLog[i].valueB = 0;
		iL->infoLog[i].valueC = 0;
	}
}

/* Debug messages */
void InfoLogAdd(ErrorLevel errorLevel, uint8_t function, uint8_t subFunction, uint8_t type,
		uint8_t status, uint8_t valueA, uint16_t valueB, uint32_t valueC,
		Info *iL)
{
	/* If the buffer is full this error should be written */
	if (iL->infoCounter >= INFO_LOGG_ARRAY_SIZE)
	{
		iL->infoCounter = INFO_LOGG_ARRAY_SIZE - 1;
		iL->infoLog[iL->infoCounter].function = LOG_FUNC_ERROR;
		iL->infoLog[iL->infoCounter].subFunction = LOG_SUB_ADD;
		iL->infoLog[iL->infoCounter].type = LOG_TYPE_BUFFER_FULL;
		iL->infoLog[iL->infoCounter].status = LOG_STATUS_FAIL;
		iL->infoLog[iL->infoCounter].errorLevel = 0;
		iL->infoLog[iL->infoCounter].valueA = 0;
		iL->infoLog[iL->infoCounter].valueB = 0;
		iL->infoLog[iL->infoCounter].valueC = 0;
		iL->infoLog[iL->infoCounter].timestamp = RTC_Get_DateTime_Comp();
	}
	else
	{
		iL->infoLog[iL->infoCounter].function = function;
		iL->infoLog[iL->infoCounter].subFunction = subFunction;
		iL->infoLog[iL->infoCounter].type = type;
		iL->infoLog[iL->infoCounter].status = status;
		iL->infoLog[iL->infoCounter].timestamp = RTC_Get_DateTime_Comp();
		iL->infoLog[iL->infoCounter].errorLevel = errorLevel;
		iL->infoLog[iL->infoCounter].valueA = valueA;
		iL->infoLog[iL->infoCounter].valueB = valueB;
		iL->infoLog[iL->infoCounter].valueC = valueC;

		iL->infoCounter++;

		/* Updates the highest errorlevel this iteration */
		if (errorLevel > iL->highestError)
		{
			iL->highestError = errorLevel;
		}
	}
}

/* Gets size of current Info-struct */
uint32_t InfoGetSize(Info *in)
{
	uint32_t size = in->infoCounter * sizeof(InfoLog);
	return size;
}

void InfoPrintLog(Info *in)
{
	for (int i = 0; i < in->infoCounter; i++)
	{
		printf(
				"\t Info: %lx - %02x - %02x%02x%02x%02x - %02x - %02x - %02lx\r\n",
				in->infoLog[i].timestamp,
				in->infoLog[i].errorLevel,
				in->infoLog[i].function,
				in->infoLog[i].subFunction,
				in->infoLog[i].type,
				in->infoLog[i].status,
				in->infoLog[i].valueA,
				in->infoLog[i].valueB,
				in->infoLog[i].valueC);
	}
}

/* Copies all data from a temp struct to a permanent one */
void CopyTempToInfo(void)
{
	for (int i = 0; i < GetTempInfoStruct()->infoCounter; i++)
	{

		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].startOfInfo = 0xABBABABE;

		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].timestamp =
				GetTempInfoStruct()->infoLog[i].timestamp;
		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].errorLevel =
				GetTempInfoStruct()->infoLog[i].errorLevel;
		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].function =
				GetTempInfoStruct()->infoLog[i].function;
		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].subFunction =
				GetTempInfoStruct()->infoLog[i].subFunction;
		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].type =
				GetTempInfoStruct()->infoLog[i].type;
		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].status =
				GetTempInfoStruct()->infoLog[i].status;
		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].valueA =
				GetTempInfoStruct()->infoLog[i].valueA;
		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].valueB =
				GetTempInfoStruct()->infoLog[i].valueB;
		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].valueC =
				GetTempInfoStruct()->infoLog[i].valueC;

		GetInfoStruct()->infoLog[GetInfoStruct()->infoCounter].endOfInfo = 0xDEADBEEF;

		GetInfoStruct()->infoCounter++;
	}
}

/*
 * Gets the current size of the errorLog
 */
uint32_t ErrorGetSize(Error *er)
{
	uint32_t size = er->errorCounter * sizeof(ErrorLog);
	return size;
}

/*
 * Prints the errorlog
 */
void ErrorPrintLog(Error *er)
{
	for (int i = 0; i < er->errorCounter; i++)
	{
		printf(
				"\t Error: %lx - %02x%02x%02x%02x - %lx - %lx - %x \r\n",
				er->errorLog[i].timestamp,
				er->errorLog[i].status,
				er->errorLog[i].type,
				er->errorLog[i].subFunction,
				er->errorLog[i].function,
				er->errorLog[i].valueA,
				er->errorLog[i].valueB,
				er->errorLog[i].logLevel);
	}
}





/* Radio Errors */

void ErrorTimerHourly(errorTimer *sTimer, radioFlags *rFlags)
{
	RTC_DateTypeDef sDate;
	RTC_TimeTypeDef timeStruct;

	/* Get current time of MCU */
	HAL_RTC_GetTime(&hrtc, &timeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	/* Assign the values to startTimer */
	sTimer->oneHourError.Year = sDate.Year;
	sTimer->oneHourError.Month = sDate.Month;
	sTimer->oneHourError.Day = sDate.Date;
	sTimer->oneHourError.Hour = timeStruct.Hours;
	sTimer->oneHourError.Minute = timeStruct.Minutes;
	sTimer->oneHourError.Second = timeStruct.Seconds;

	/* Disable radio unit and enable error handler */
	rFlags->radioEnableFlag = 0;
	rFlags->errorHourFlag = 1;
}

void ErrorTimerHourlyCheck(errorTimer *sTimer, errorTimer *eTimer,
		struct radioFlags *rFlags)
{
	/* Try again in next time its scheduled (next day) */

	RTC_DateTypeDef sDate;
	RTC_TimeTypeDef timeStruct;
	/* Get current time of MCU */
	HAL_RTC_GetTime(&hrtc, &timeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	/* Assign current time to endTimer */
	eTimer->oneHourError.Year = sDate.Year;
	eTimer->oneHourError.Month = sDate.Month;
	eTimer->oneHourError.Day = sDate.Date;
	eTimer->oneHourError.Hour = timeStruct.Hours;
	eTimer->oneHourError.Minute = timeStruct.Minutes;
	eTimer->oneHourError.Second = timeStruct.Seconds;

	printf("ErrorHourlyCheck\r\n");
	//HAL_Delay(1000);

	/* Calculate the difference in seconds between startTimer and endTimer */
	int32_t diffSeconds = RTC_GetElapsedSeconds(&sTimer->oneHourError, &eTimer->oneHourError);

	/* If the clocks differ more than TIMEOUT_BEFORE_START_AGAIN, it has waited enough time */
	if (diffSeconds > TIMEOUT_BEFORE_START_AGAIN)
	{
		/* Time to try the radio module again */
		rFlags->errorHourFlag = 0;
		rFlags->radioEnableFlag = 1;

		/* Hour-error incremented */
		rFlags->errorHourCounter++;

		MX_USART2_UART_Init();

		MX_LPTIM1_Init();

		/* Wake up and try again */
		SetRadioState(RADIO_STARTUP); //TODO
	}

	/* If the module has failed more than 3 times this day, go to sleep til next time its
	 * time to try sending data */
	if (rFlags->errorHourCounter >= MAX_FAILED_ATTEMPTS_HOURS)
	{
		/* Daily error incremented, and hourly error reset */
		rFlags->errorDayCounter++;
		rFlags->errorHourCounter = 0;

//		DisEnableSDPin(0);

		/* Sleep until next day */
		SetRadioState(RADIO_IDLE); //TODO

		InfoLogAdd(ERRORLEVEL_SEVERE,
		LOG_FUNC_ERROR, LOG_SUB_DAILY,
		LOG_TYPE_SET,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		/* Add to error log */
		ErrorAddLog(
		LOG_FUNC_ERROR, LOG_SUB_DAILY,
		LOG_TYPE_SET,
		LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

		/* If the radio module has failed more than X days in a row, CRITICAL ERROR */
		if (rFlags->errorDayCounter == MAX_FAILED_ATTEMPTS_DAYS)
		{
			/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
			/*@													@*/
			/*@ Critical error, everybody evacuate the building @*/
			/*@													@*/
			/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

			int n = 10;
			while (n-- > 0)
			{
				printf("Sara exploding in %d seconds\r\n", n);
				HAL_Delay(1000);
				/* Lite kul får man ha */
			}

			InfoLogAdd(ERRORLEVEL_FATAL,
			LOG_FUNC_ERROR, LOG_SUB_CRITICAL,
			LOG_TYPE_SET,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

			/* Add to error log */
			ErrorAddLog(
			LOG_FUNC_ERROR, LOG_SUB_CRITICAL,
			LOG_TYPE_SET,
			LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			rFlags->errorDayCounter = 0;
		}
	}
}
