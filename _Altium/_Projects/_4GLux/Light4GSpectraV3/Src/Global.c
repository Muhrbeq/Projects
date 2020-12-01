/*
 * Global.c
 *
 *  Created on: 17 Oct 2018
 *      Author: andreas.fredlund
 */


#include "Global.h"


/*
 * SD Globals
 */
volatile char sd_fileName[13];
volatile uint16_t testCounter = 0;
/*
 * Struct Globals
 */
RTC_DateTypeDef sDateGT;
RTC_TimeTypeDef timeStructGT;

Data data_global;
Error error_global;
Info info_global;
Info tempInfo;
Time SaraLastConnected;
Time mcuClockTime;
Time networkStart;
Time networkEnd;
Time startTimeToSend;
Time turnOffStart;
Time turnOffEnd;

radioFlags radioErrors;

SaraConfig SaraConfiguration;

errorTimer startTimer;
errorTimer endTimer;

uint16_t luxCounter = 0;

/* Fuctions that gets structs */
Data *GetDataStruct(void)
{
	return &data_global;
}

Error *GetErrorStruct(void)
{
	return &error_global;
}

Info *GetInfoStruct(void)
{
	return &info_global;
}

Info *GetTempInfoStruct(void)
{
	return &tempInfo;
}

Time *GetTimeStruct(TimeStructs retStruct)
{
	if (retStruct == SaraLastConnectedVal)
	{
		return &SaraLastConnected;
	}
	else if (retStruct == mcuClockTimeVal)
	{
		return &mcuClockTime;
	}
	else if (retStruct == networkStartVal)
	{
		return &networkStart;
	}
	else if (retStruct == networkEndVal)
	{
		return &networkEnd;
	}
	else if (retStruct == startTimeToSendVal)
	{
		return &startTimeToSend;
	}
	else if (retStruct == turnOffStartVal)
	{
		return &turnOffStart;
	}
	else
	{
		return &turnOffEnd;
	}

}

SaraConfig *GetSaraConfigurationStruct()
{
	return &SaraConfiguration;
}

radioFlags *GetErrorFlagsStruct()
{
	return &radioErrors;
}

errorTimer *GetErrorTimerStruct(ErrorStartEnd state)
{
	if (state == startTimerEnum)
	{
		return &startTimer;
	}
	else
	{
		return &endTimer;
	}
}
