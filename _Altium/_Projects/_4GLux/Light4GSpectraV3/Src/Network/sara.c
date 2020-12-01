/*
 * sara.c
 *
 *  Created on: 26 nov. 2018
 *      Author: Rasmus.Muhrbeck
 */
#include "sara.h"
#include "Global.h"
#include "TimeFunc.h"
#include "Error.h"

struct Globals headTail;
extern RTC_DateTypeDef sDateGT;
extern RTC_TimeTypeDef timeStructGT;

/* UART buffer init */
volatile uint8_t uart_Buffer[TX_BUFF_SIZE] = { 0 };

/* Usart1 IRQ handler */
void USART2_Callback(void)
{

	/* Check if RXNE flag high */
	if ((USART2->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
	{
		/* Read Data Register on USART1 (Sara module) */
		uint8_t data = USART2->RDR;

		/* Disable \r and \n from buffer */
		if (data != '\r')
		{
			if (data != '\n')
			{
				/* Add data to Buffer */
				uart_Buffer[headTail.Head] = data;
				headTail.Head++;
			}
		}

		/* Overflow Safety */
		if (headTail.Head > TX_BUFF_SIZE - 1)
		{
			headTail.Head = 0;
		}
	}
}

/* See if Sara is alive */
uint8_t SaraIsAlive()
{
	/* Check if sara is alive */
	return SaraCheckCommand("AT\r\n", "OK", LOW_TIMEOUT);
}

uint8_t StartUpSara()
{
	/* Toggle LED to show init of startup sequence */
	LED_Blink();

	/*-____________________________________________________-*/
	/* Set Radio Pin high to enable power to Radio module */

	DisEnableRadioPin(1);
	HAL_Delay(1000);

	/*-____________________________________________________-*/

	/* Check if Sara module already ON */
	if (SaraCheckCommand("AT\r\n", "OK", LOW_TIMEOUT))
	{
		/* Sara is on */
		return true;
	}

	/* Set PIN_B14 (Power_Sara) high */
	DisEnableSaraPin(1);

	/* 500-1500 ms to power on Sara */
	HAL_Delay(1050);

	/* Reset PIN_B14 (Sara is now ON) */
	DisEnableSaraPin(0);

	/* Wait for 5 sec (minimum 4.5s) for Sara to be fuctional*/
	HAL_Delay(5000);

//Heartbeat at end of cycle to show startup done
	LED_Blink();

	/* Check if something is in the buffer, remove it */
	LL_USART_ReceiveData8(USART2);

	/* Init start of Head */
	headTail.Head = 0;

	/* Init start of Tail */
	headTail.Tail = 0;

	/* Check if the module is ON */
	if (SaraCheckCommand("AT\r\n", "ATOK", LOW_TIMEOUT))
	{
		/* Sara is on */
		return true;
	}

	/* Sara is not on */
	return false;
}

/* Init commands for Sara module */
uint8_t SaraInitAtCommands()
{
	/* Disables Echo mode */
	if (!SaraDisableEchoMode())
	{
		return false;
	}

	/* Activates the internet-LED to get visual confirmation when network is found */
	if (!SaraCheckCommand("AT+UGPIOC=16,2\r\n", "OK", LOW_TIMEOUT))
	{
		return false;
	}

	/* Checks if SIM is ready */
	/* This does not work for SIM cards with PIN code */
	if (!SaraCheckSimStatus())
	{
		return false;
	}

	return true;
}

/* Powers down Sara module */
uint8_t SaraPowerDown(void)
{
	return SaraCheckCommand("AT+CPWROFF\r\n", "OK", DEFAULT_TIMEOUT);
}

/* Disables Echo mode on responses from Sara */
uint8_t SaraDisableEchoMode()
{
	if (SaraCheckCommand("ATE0\r\n", "ATE0OK", LOW_TIMEOUT))
	{
		return true;
	}
	else if (SaraCheckCommand("ATE0\r\n", "OK", LOW_TIMEOUT))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* Checks if the SIM card is ready or not */
uint8_t SaraCheckSimStatus()
{
	return SaraCheckCommand(
			"AT+CPIN?\r\n",
			"+CPIN: READYOK",
			DEFAULT_TIMEOUT);
}

/* Set function of Sara module (Never used) */
uint8_t SaraSetCFUN(uint8_t mode)
{
	char txBuff[20] = { 0 };

	sprintf(txBuff, "AT+CFUN=%d\r\n", mode);

	return SaraCheckCommand(txBuff, "OK", 2 * DEFAULT_TIMEOUT);
}

/* Reboot Sara (never used) */
uint8_t SaraReboot()
{
	return SaraCheckCommand("AT+CFUN=15\r\n", "OK", DEFAULT_TIMEOUT);
}

/* Wait for response from sara module after AT command */
uint16_t SaraWaitForResponse(uint8_t *get, uint16_t count, uint32_t timeout_ms)
{
	uint32_t loop = 0;
	uint16_t u_counter = 0;

	/* Loop for 'timeout' specified time */
	while (loop <= timeout_ms)
	{
		if (headTail.Head < headTail.Tail)
		{
			u_counter = TX_BUFF_SIZE - (headTail.Tail - headTail.Head);
		}

		else
		{
			u_counter = headTail.Head - headTail.Tail;
		}

		/* When uart buffer has specified number of bytes, copy them */
		if (count == u_counter)
		{
			/* Fetch the data back to call */
			for (int i = 0; i < count; i++)
			{
				/* Assign received buffer the values of UART buffer */
				get[i] = uart_Buffer[headTail.Tail];

				/* Tail jumps one position */
				headTail.Tail++;

				/* If tail gets bigger than buffer */
				if (headTail.Tail > TX_BUFF_SIZE - 1)
				{
					/* Jump back to first position */
					headTail.Tail = 0;
				}
			}
			/* Reset counter for next command */
			headTail.Tail = headTail.Head;

			/* Return number of bytes copied */
			printf("count == counter\r\n");
			return u_counter;
		}
		/* Delay wait for more data */
		HAL_Delay(1);

		/* (1 ms increments) */
		loop += 1;
	}

	/* Just copy the volatile counter locally */
	if (headTail.Head < headTail.Tail)
	{
		/* See above explanation */
		u_counter = TX_BUFF_SIZE - (headTail.Tail - headTail.Head);
	}
	else
	{
		/* See above explanation */
		u_counter = headTail.Head - headTail.Tail;
	}

	/* If the uart counter is bigger than the count specified */
	if (u_counter > count)
	{
		/* Reassign ucounter = count */
		u_counter = count;
	}

	/* Loop through the uart buffer */
	for (int i = 0; i < u_counter; i++)
	{
		/* Received buffer = uart buffer */
		get[i] = uart_Buffer[headTail.Tail];

		/* Tail jumps one position */
		headTail.Tail++;

		/* If tail gets bigger than buffer */
		if (headTail.Tail > TX_BUFF_SIZE - 1)
		{
			/* Jump back to first position */
			headTail.Tail = 0;
		}
	}
	/* Make sure that the tail = head so that they start at same place for new command */
	headTail.Tail = headTail.Head;

	/* Return number of bytes copied */
	printf("count != counter\r\n");
	return u_counter;
}

/* Send Command to sara module */
void SaraSendCommand(const char *cmd)
{
	uint8_t len = strlen(cmd);
	uint8_t *pchar = (uint8_t*) cmd;

	/* Send characters one per one, until last char to be sent */
	for (uint8_t i = 0; i < len; i++)
	{
		/* Wait for TXE flag to be raised */
		while (!LL_USART_IsActiveFlag_TXE(USART2))
		{
		}

		/* Write character in Transmit Data register.
		 TXE flag is cleared by writing data in TDR register */
		LL_USART_TransmitData8(USART2, *pchar++);
	}

	/* Wait for TC flag to be raised for last char */
	while (!LL_USART_IsActiveFlag_TC(USART2))
	{
	}

}

/* Send command and check if the received command is as expected */
uint8_t SaraCheckCommand(const char *sendCommand, const char *receiveCommand, uint32_t timeout_ms)
{
	/* Get length of expected response */
	uint8_t len = strlen(receiveCommand);

	/* Init local receiveBuffer to compare against expected response */
	uint8_t receiveBuff[TX_BUFF_SIZE] = { 0 };

	/* Send desired command to Sara module */
	SaraSendCommand(sendCommand);

	/* Wait for response from Sara module with the given length of expected response */
	SaraWaitForResponse(receiveBuff, len, timeout_ms);

	/* Debug */
	printf("Received data: %s, Expected Data: %s\r\n", receiveBuff, receiveCommand);

	/* Loop through both Buffers to see if they match */
	for (int i = 0; i < len; i++)
	{
		if (receiveBuff[i] != receiveCommand[i])
		{
			/* If an element does not match, print the response and the expected response */
			return false;
		}
	}

	/* If all element do match, print the response and the expected response */
	return true;

}

/* Gets the internet clock from Sara */
void SARA_getRealTimeClock(char *time)
{
	int i = 0;
	char *p;
	uint8_t tmp[64] = { '\0' };

	/* Command to Sara to send the internet clock */
	SaraSendCommand("AT+CCLK?\r");

	/* Wait for Response with the clock */
	SaraWaitForResponse(tmp, sizeof tmp - 1, 100);

	/* See if there is a "+CCLK:" in the answer */
	if (NULL != (p = strstr((char*) tmp, "+CCLK:")))
	{
		i = 8;
		while (*(p + i) != '\"' && *(p + i) != '\0')
		{
			/* Assign clock to Time */
			*(time++) = *(p + i);
			i++;
		}
	}
	else
	{
		/* Read Real Time Clock Failed */
		return;
	}
}

/* Calculate the time after GMT conversion and assign it to timestruct when sara last was connected */
uint8_t GMTcalculator(int *year, int *month, int *day, int *hour, int *minute, int *second,
		int *GMT, Time *timeSaraLastConnected)
{
	struct tm timeGMT;

	int multiplier = *GMT;

	timeGMT.tm_year = 100 + *year;
	timeGMT.tm_mon = *month - 1;
	timeGMT.tm_mday = *day;
	timeGMT.tm_hour = *hour;
	timeGMT.tm_min = *minute + multiplier * 15;
	timeGMT.tm_sec = *second;
	timeGMT.tm_isdst = 0;
	timeGMT.tm_wday = 0;
	timeGMT.tm_yday = 0;

//printf("The current date/time is: %s", asctime(&timeGMT));

	mktime(&timeGMT);

	timeSaraLastConnected->Year = (uint8_t) ((timeGMT.tm_year - 100) & 0xFF);
	timeSaraLastConnected->Month = (uint8_t) ((timeGMT.tm_mon + 1) & 0xFF);
	timeSaraLastConnected->Day = (uint8_t) (timeGMT.tm_mday & 0xFF);
	timeSaraLastConnected->Hour = (uint8_t) (timeGMT.tm_hour & 0xFF);
	timeSaraLastConnected->Minute = (uint8_t) (timeGMT.tm_min & 0xFF);
	timeSaraLastConnected->Second = (uint8_t) (timeGMT.tm_sec & 0xFF);

	printf("The current date/time is: %s", asctime(&timeGMT));

	return true;
}

/* Compare MCU and network clock */
uint8_t SaraCompareRealTimeClock(char *time, Time *timeSaraLastConnected, Time *mcuClock)
{

	/* Compares the time that Sara gets from the internet against the time on the MCU
	 *
	 * If they are not the same, update the time on the MCU */

	int year, month, day, hour, minute, second;
	int GMT = 0;

	/* Extracts year, month, day, hour, minute, second, NOT SURE GMT WORKS */
	if (6
			<= (sscanf(
					time,
					"%d/%d/%d,%d:%d:%d+%d",
					(int*) &year,
					(int*) &month,
					(int*) &day,
					(int*) &hour,
					(int*) &minute,
					(int*) &second,
					&GMT)))
	{
		/* Sometimes the Internet clock reads wrong (from start of time) this is
		 * fail-safe, this means that the code needs to be updated before 2059 */
		if (year > 60)
		{
			return false;
		}

		// Convert Time zone aquired by GMT (each GMT is "worth" 15min)
		GMTcalculator(&year, &month, &day, &hour, &minute, &second, &GMT, timeSaraLastConnected);
	}
	/* If 7 pairs can't be paired, then the timeBuffer is corrupt, obtain a new */
	else
	{
		return false;
	}

	/* Gets time on MCU */
	GetTime(mcuClock);

	printTime(mcuClock);

	/* Compares the two clocks */
	int32_t differenceSeconds = RTC_GetElapsedSeconds(timeSaraLastConnected, mcuClock);

	/* If the difference in seconds are too big (15 sec now, can be re-defined)
	 * change the MCU clock to internet clock */
	if (abs(differenceSeconds) > MAX_SECOND_DIFFERENCE)
	{
		/* Change the MCU clock to the network clock */
		sDateGT.Year = timeSaraLastConnected->Year;
		sDateGT.Month = timeSaraLastConnected->Month;
		sDateGT.Date = timeSaraLastConnected->Day;
		timeStructGT.Hours = timeSaraLastConnected->Hour;
		timeStructGT.Minutes = timeSaraLastConnected->Minute;
		timeStructGT.Seconds = timeSaraLastConnected->Second;

		/* Set the time of MCU */
		HAL_RTC_SetTime(&hrtc, &timeStructGT, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &sDateGT, RTC_FORMAT_BIN);
	}

	return true;
}

/* Get RSSI and quality of signal */
uint8_t SaraGetRSSI()
{
	char rxBuff[1024] = { 0 };

	/* Clearing Buffer and sending command to SARA and reads reply */
	SaraClearBuffer();
	SaraSendCommand("AT+CESQ\r\n");
	SaraWaitForResponse((uint8_t*) rxBuff, 1024, LOW_TIMEOUT);

	/* Init variables */
	int32_t rxLev, BER, RSCP, ecn0, RSRQ, RSRP;

	/* Extracts rxLev, BER, RSCP, ecn0, RSRQ, RSRP. RSRQ and RSRP are the only ones that matters for LTE */
	if (6
			== (sscanf(
					rxBuff,
					"+CESQ: %d,%d,%d,%d,%d,%dOK",
					(int*) &rxLev,
					(int*) &BER,
					(int*) &RSCP,
					(int*) &ecn0,
					(int*) &RSRQ,
					(int*) &RSRP)))
	{
		if (RSRP == 255)
		{
			/* Ref signal received power undefined, try again */
			return false;
		}
		return true;
	}
	return false;
}

/* Gets signal strength indication */
uint8_t SaraGetSignalStrength()
{

	uint8_t rxBuff[1024] = { 0 };
	/* Sending command to SARA and reads reply */
	SaraSendCommand("AT+CSQ\r\n");
	SaraWaitForResponse(rxBuff, 1024, LOW_TIMEOUT);

//printf("%s\r\n", (char*) rxBuff);

	/* Define where in the answer the CSQ will be */
	const char* startPattern = ": ";
	const char* endPattern = ",";

	/* Checks if the startPattern appears, otherwise the reply hasn't arrived */
	const char *iStartPattern = strstr((char*) rxBuff, startPattern);
	if (iStartPattern != NULL)
	{
		/* Jump to the start of message */
		const size_t placeStart = strlen(startPattern);

		/* Check if there is an endPattern, otherwise return false */
		const char *iEndPattern = strstr(iStartPattern + placeStart, endPattern);

		/* If iEndPattern is in the array (endPattern occurs), enter loop */
		if (iEndPattern != NULL)
		{
			/* Define message length (End - Start) */
			const size_t messageLen = iEndPattern - (iStartPattern + placeStart);

			/* Assign an array that matches that length */
			char *ret[messageLen];
			if (ret != NULL)

			{
				/* Assign a signal strength variable */
				uint16_t sigSTR;
				memcpy(ret, iStartPattern + placeStart, messageLen);
				ret[messageLen] = '\0';

				/* Signal Strength is equal to the processed message */
				sigSTR = atoi((char*) ret);

				/* Add to InfoLog so that one can see the RSSI in Info */
				InfoLogAdd(ERRORLEVEL_WARNING,
				LOG_FUNC_STATE,
				LOG_SUB_RSSI,
				LOG_TYPE_READ,
				LOG_STATUS_PASSED, 0, sigSTR, 0, GetTempInfoStruct());

				printf("Signal Strength: __%d__\r\n", sigSTR);

				/* If the RSSI is below a certain threshold, don't proceed */
				if (sigSTR < 5 || sigSTR == 99)
				{
					/* Signal strength not good enough to proceed */
					return false;
				}
				/* Signal Strength is good enough to proceed with Radio process */
				return true;
			}
		}
	}
	return false;
}

/* Never Used */
void SaraClearBuffer()
{
	for (int i = TX_BUFF_SIZE - 1; i >= 0; i--)
	{
		uart_Buffer[i] = '\0';
		headTail.Head = 0;
		headTail.Tail = 0;
	}
}

/* Init values in error struct */
void InitErrorStruct(struct radioFlags *rF)
{
	rF->radioEnableFlag = 1;
	rF->firstRun = 1;
	rF->firstErrorOfDay = 1;
	rF->counterMessage = 1;

	rF->errorDayCounter = 0;
	rF->errorDayFlag = 0;
	rF->errorHourCounter = 0;
	rF->errorHourFlag = 0;
	rF->noNewSettings = 0;
	rF->publishError = 0;
	rF->serverDo = 0;
	rF->startUpError = 0;
	rF->subscribeError = 0;
	rF->testCounterConnectServer = 0;
	rF->testCounterGetSignal = 0;
	rF->testCounterTime = 0;
	rF->updateError = 0;
	rF->AtInit = 0;
	rF->setServerError = 0;
	rF->dataPublishedFlag = 0;
	rF->adjustedHours = 0;
	rF->adjusted = 0;
	rF->errorStart = 0;
	rF->everythingOK = 0;
	rF->alreadyConnected = 0;
	rF->triedToSendCount = 0;
	rF->trySubscribeAgain = 0;
	rF->subscribeErrorCounter = 0;
	rF->radioRTC = 0;
	rF->radioPublishCounter = 0;

}

/* Init values to Sara configuration */
void InitSaraStruct(SaraConfig *s)
{
	/* Init the SaraConfiguration to "Start Settings" */
	strcpy(s->id, "anfr");
	strcpy(s->ip, "83.182.202.21");
	strcpy(s->revision, "1.01");
	strcpy(s->connectSubTopic, "SaraSub"); //Where server publishes
	strcpy(s->connectPubTopic, "SaraPub");    //Where Sara publishes
	strcpy(s->connectComTopic, "SaraCom");    //Where Sara publishes
	s->port = 1883;
	s->serverToDo = 0;
	s->updateFrequency = 120; //This is in seconds
	s->hourToSend = 3;
	strcpy(s->webPage, "mbqtest.ddns.net");
	s->firstMessageToReSend = 1;
	s->lastMessageToReSend = 1;
}

/* Init timestruct to current MCU time */
void InitTimeStruct(Time *tS)
{
	Time iT;
	GetTime(&iT);

	tS->Year = iT.Year;
	tS->Month = iT.Month;
	tS->Day = iT.Day;
	tS->Hour = iT.Hour;
	tS->Minute = iT.Minute;
	tS->Second = iT.Second;
}

//
//else if (*stageSara == 9)
//  {
//    uint8_t rxBuff[TX_BUFF_SIZE] =
//	{ 0 };
//    uint8_t JSONBuff[TX_BUFF_SIZE] =
//	{ 0 };
//    uint32_t fCrc;
//    SaraSendCommand ("AT+UMQTTC=6\r\n");
//    SaraWaitForResponse (rxBuff, TX_BUFF_SIZE - 1, DEFAULT_TIMEOUT);
//
//    if (MqttCheckRxData (rxBuff))
//	{
//	  MqttParseJSONMessage (rxBuff, "{\"", "\"}", JSONBuff);
//	  printf ("JSONMessage: %s\r\n", (char*) JSONBuff);
//	  MqttParseJSONCRC (rxBuff, &fCrc, "\"}");
//	  VerifyFile (JSONBuff, &fCrc);
//	  ParseSettingsString ((char*) JSONBuff, &SaraConfiguration);
//	  PrintSettings (&SaraConfiguration);
////			if (MqttParseMessage(rxBuff, "$$", "##"))
////			{
//	  MqttParseMessage (rxBuff, "$$", "##");
//	  MqttParseID (rxBuff, "!!", "@@");
//	  if (MqttReturnPageRead (rxBuff, "@@", "$$", "PageOF"))
//	    {
//	      printf ("Im in the Parse MSG looop \r\n");
//	      *stageSara = 0;
//	      while (!SaraPowerDown ())
//		{
//		};
//	      HAL_Delay (10000);
//	}
//	}
//  }

/* ------------------------------ NEVER USED ------------------------------ */

//uint8_t SaraWaitForNetworkRegistered(uint32_t timeout_ms)
//{
//	uint8_t pass = false;
//	uint32_t timeStart = 0;
//
//// check network registration
//	do
//	{
//		pass = (SaraCheckCommand("AT+CGATT?\r\n", "+CGATT: 1", 10))
//				&& (SaraCheckCommand("AT+CEREG?\r\n", "+CEREG: 0,1", 10));
//
//		if (timeStart > timeout_ms)
//	{
//			return false;
//		}
//		HAL_Delay(10);
//		timeStart += 10;
//		printf(".\r\n");
//	} while (!pass);
//
//	return true;
//}

//uint8_t GMTcalculator(int *year, int *month, int *day, int *hour, int *minute, int *second, int *GMT)
//{
//	if (*GMT > 0)
//	{
//		for (int i = *GMT; i > 0; i--)
//		{
//			*minute = *minute + 15;
//			if (*minute > 60)
//			{
//				*minute = *minute - 60;
//				*hour = *hour + 1;
//			}
//			if (*hour > 24)
//			{
//				*hour = *hour - 24;
//				*day = *day + 1;
//			}
//			if (*month == 1 || *month == 3 || *month == 5 || *month == 7 || *month == 8 || *month == 10)
//			{
//				if (*day > 31)
//				{
//					*month = *month + 1;
//					*day = 1;
//				}
//			}
//			else if (*month == 4 || *month == 6 || *month == 9 || *month == 11)
//			{
//				if (*day > 30)
//				{
//					*month = *month + 1;
//					*day = 1;
//				}
//			}
//			else if (*month == 12)
//			{
//				if (*day > 31)
//				{
//					*day = 1;
//					*month = 1;
//					*year = *year + 1;
//				}
//			}
//			else
//			{
//				if (*year != 2020 || *year != 2024 || *year != 2028)
//				{
//					if (*day > 29)
//					{
//						*month = *month + 1;
//						*day = 1;
//					}
//					else if (*day > 28)
//					{
//						*month = *month + 1;
//						*day = 1;
//					}
//						}
//					}
//
//				}
//			}
//	else if (*GMT < 0)
//	{
//		for (int i = *GMT; i < 0; i++)
//		{
//			*minute = *minute - 15;
//			if (*minute < 0)
//			{
//				*minute = *minute + 60;
//				*hour = *hour - 1;
//			}
//			if (*hour < 0)
//			{
//				*hour = *hour + 24;
//				*day = *day - 1;
//			}
//			if (*month == 1)
//			{
//				if (*day < 1)
//				{
//					*day = 31;
//					*month = 12;
//					*year = *year - 1;
//				}
//			}
//			else if (*month == 5 || *month == 7 || *month == 8 || *month == 10 || *month == 12)
//			{
//				if (*day < 1)
//				{
//					*month = *month - 1;
//					*day = 30;
//				}
//			}
//			else if (*month == 2 || *month == 4 || *month == 6 || *month == 9 || *month == 11)
//			{
//				if (*day < 1)
//				{
//					*month = *month - 1;
//					*day = 31;
//				}
//			}
//			else
//			{
//				if (*year != 2020 || *year != 2024 || *year != 2028)
//				{
//					if (*day < 1)
//					{
//						*month = *month - 1;
//						*day = 28;
//					}
//				}
//				else if (*day < 1)
//				{
//					*month = *month - 1;
//					*day = 29;
//				}
//			}
//
//		}
//	}
//
//	return true;
//}

///* Function that stops everything else until the Sara module has found internet (not used) */
//uint8_t SaraWaitNetwork(uint32_t timeoutMinutes)
//{
//	uint32_t timeout = timeoutMinutes * 60 * 1000;
//	uint32_t tick_t = 0;
//	uint8_t retVal = false;
//
//	/* Check with module if it has found internet */
//	do
//	{
//		retVal = (SaraCheckCommand("AT+CGATT?\r\n", "+CGATT: 1OK", LOW_TIMEOUT))
//				&& (SaraCheckCommand("AT+CEREG?\r\n", "+CEREG: 0,1OK",
//				LOW_TIMEOUT));
//		if (tick_t > timeout)
//	{
//			return false;
//		}
//		HAL_Delay(10);
//		tick_t += 10;
//
//	} while (!retVal);
//
//	return true;
//}

//uint8_t
//SaraParseCgdcont (void)
//{
//	char *p;
//  uint8_t recvBuffer[128] =
//    { '\0' };
//  int a0, a1, a2, a3;
//
//  // Get IP address, AT+CGDCONT?
//  // +CGDCONT: 1,"IP","CMNBIOT1","100.112.210.15",0,0,0,0
//  // OK
//  SaraClearBuffer (recvBuffer, sizeof(recvBuffer));
//  SaraSendCommand ("AT+CGDCONT?\r\n");
//  SaraWaitForResponse (recvBuffer, 127, DEFAULT_TIMEOUT);
//
//  printf ("%s \n\r", (char*) recvBuffer);
//
//  if (NULL != (p = strstr ((char*) recvBuffer, "+CGDCONT:")))
//    {
//      if (5
//	  == (sscanf (
//	      p,
//	      "+CGDCONT: %*d,\"IP\",\"%[^\"]\",\"%d.%d.%d.%d\",%*d,%*d,%*d,%*d",
//	      _apn, &a0, &a1, &a2, &a3)))
//	{
//	  _u32ip = TUPLE_TO_U32IP(a0, a1, a2, a3);
//	  sprintf (_str_ip, IP_FORMAT, a0, a1, a2, a3);
//	}
//    }
//  else
//    {
//      return false;
//    }
//
//  return true;
//}

//void SaraStartUpSequence(uint8_t *stageOfStartUp)
//{
//	if (*stageOfStartUp == 0)
//	{
//		/* Starts by throwing AT command to SARA */
//		SaraSendCommand("AT\r\n");
//		printf("Im Heeeere\r\n");
//		*(stageOfStartUp) = 1;
//	}
//
//	else if (*stageOfStartUp == 1)
//	{
//		/* Sends "Echo off", expects answer OK */
//		if (SaraCheckCommand("ATE0\r\n", "OK", LOW_TIMEOUT))
//		{
//			*stageOfStartUp = 2;
//			printf("Echo Turned OFF\r\n");
//			printf(".\r\n");
//		}
//	}
//
//	else if (*stageOfStartUp == 2)
//	{
//		/* Checks if Sara is connected to internet (0,1) */
//		if (SaraCheckCommand("AT+CEREG?\r\n", "+CEREG: 0,1OK", LOW_TIMEOUT))
//		{
//			/* Network found */
//			char timeBuffer[64] =
//			{ 0 };
//			*stageOfStartUp = 3;
//			printf("Network found!\r\n");
//
//			/* Check status of COPS (operator etc) and CIND (see quality of signal etc) */
//			SaraCheckCommand("AT+COPS?\r\n",
//					"+COPS: 0,0,\"Telia Telia\",9OK",
//					LOW_TIMEOUT);
//			SaraCheckCommand("AT+CIND?\r\n",
//					"++CIND: 5,4,1,0,0,1,0,0,1,0,0,2OK",
//					LOW_TIMEOUT);
//			SARA_getRealTimeClock(timeBuffer);
//			printf("Time: %s\r\n", timeBuffer);
//			printf(".\r\n");
//		}
//	}
//
//	else if (*stageOfStartUp == 3)
//	{
//		/* Create TCP socket */
//		if (SaraCreateSocket(TCP))
//		{
//			/* Socket Created! */
//			printf("Socket Created!\r\n");
//			*stageOfStartUp = 4;
//		}
//	}
//
//	else if (*stageOfStartUp == 4)
//	{
//		/* Connect to TCP server */
//		if (SaraSocketConnect(0, IP, 7799))
//		{
//			printf("Connected to server\r\n");
//			*stageOfStartUp = 5;
//		}
//	}
//
//	else if (*stageOfStartUp == 5)
//	{
//		/* Writing to TCP "5" to TCP server */
//		if (SaraSocketWriteData(0, "5", 1))
//		{
//			*stageOfStartUp = 6;
//			HAL_Delay(100);
//		}
//	}
//
//	else if (*stageOfStartUp == 6)
//	{
//		if (SaraSocketCheckRxData())
//		{
//			uint8_t rxBuffer[256] =
//			{ 0 };
//			SaraSendCommand("AT+USORD=0,128\r\n");
//			SaraWaitForResponse(rxBuffer, 128, LOW_TIMEOUT);
//			*stageOfStartUp = 10;
//		}
//	}
//
//	else if (*stageOfStartUp == 10)
//	{
//		/* Writing to TCP "-1" to TCP server and the closes the connection */
//		SaraSocketWriteData(0, "-1", 2);
//		*stageOfStartUp = 3;
//		HAL_Delay(5000);
//		SaraClearBuffer();
//	}
//
//}

/* No need to comment, will be removed,
 * This function is to test the send and receive command of MCU-Sara communication */
//
//void TestSara()
//{
//
//	uint8_t minBuffer[64] = { 0 };
//
//	SaraSendCommand("AT\r\n");
//
//	uint16_t readLen = SaraWaitForResponse(minBuffer, 60, DEFAULT_TIMEOUT);
//
//	if (readLen == 60)
//		printf("Got = %s\r\n", (char*) minBuffer);
//	else
//		printf("ERROR but Got (len %d) = %s\r\n", readLen, (char*) minBuffer);
//
//}
//
///* Everything with Sockets can be removed, now using MQTT protocol instead of TCP */
//
//uint8_t SaraSocketConnect(uint8_t sockID, char *hostIP, uint16_t port)
//{
//
//	char txBuff[64];
//
//	/* Check if socketID is possible */
//	if (sockID > 6 || sockID < 0)
//	{
//		printf("Sockets only available 0-6\r\n");
//		return false;
//	}
//	printf("HostIp: %s, PortNr: %d\r\n", hostIP, port);
//
//	/* Copy command with sockID, hostIP and port to txBuffer */
//	sprintf(txBuff, "AT+USOCO=%d,\"%s\",%d\r\n", sockID, hostIP, port);
//
//	/* Check if Sara connects, wait for 5s */
//	printf("Trying to connect.....\r\n");
//	if (SaraCheckCommand(txBuff, "OK", 10 * DEFAULT_TIMEOUT))
//	{
//		return true;
//	}
//
//	return false;
//}
//
//uint8_t SaraSocketClose(uint8_t sockID)
//{
//
//	char txBuff[32];
//
//	sprintf(txBuff, "AT+USOCL=%d\r\n", sockID);
//
//	if (SaraCheckCommand(txBuff, "OK",
//	DEFAULT_TIMEOUT))
//	{
//		printf("TCP socket %d is now closed\r\n", sockID);
//		return true;
//	}
//
//	return false;
//
//}
//
//uint8_t SaraSocketWriteData(uint8_t sockID, char *data, uint16_t dataSize)
//{
//	char txBuff[64];
//	char rxBuff[64];
//
//	sprintf(txBuff, "AT+USOWR=%d,%d,\"%s\"\r\n", sockID, dataSize, data);
//	sprintf(rxBuff, "+USOWR: %d,%dOK", sockID, dataSize);
//
//	return SaraCheckCommand(txBuff, rxBuff, 10 * DEFAULT_TIMEOUT);
//
//}
//
//uint8_t SaraSocketCheckRxData()
//{
//	uint8_t processedBuffUart[64] = { 0 };
//	char *p;
//
//	printf("BufferRead: %s\r\n", (char*) processedBuffUart);
//
//	SaraWaitForResponse(processedBuffUart, 63, LOW_TIMEOUT);
//
//	p = strstr((char*) processedBuffUart, "+UUSORD:");
//	if (p != NULL)
//	{
//		return true;
//	}
//	return false;
//}
//
//uint8_t SaraCreateSocket(Socket_type sockType)
//{
//	/* Unsure that port in function is needed */
//
//	char txBuff[32];
////char rxBuff[32];
//
//	/* Expect only one TCP connection at a time to Sara */
//	sprintf(txBuff, "AT+USOCR=%d\r\n", sockType);
//
//	/* Return True if socket created */
//	return SaraCheckCommand(txBuff, "+USOCR: 0OK", DEFAULT_TIMEOUT);
//
//}
