/*				|								  |
*				| __ExegerGeneric.c				  |
*				|								  |
*				|  Created on: 8 jan. 2020		  |
*				|      Author: Rasmus.Muhrbeck	  |
*			    |_________________________________|
 */

/* @info
 *
 * General C file for Exeger prototype team
 * used for generic functions.
 * Based on GeneralLogger pinout
 */

/*
 *
 * @ include files
 *
 */

/* Which functions do you want? */
#define USE_USART_FUNCTIONS
#define USE_GPIO_FUNCTIONS
#define USE_RTC_FUNCTIONS
#define USE_IWDG_FUNCTIONS

/* To include necessary global variables like USART1 etc... */
#include "main.h"

#ifdef USE_USART_FUNCTIONS
#include <usart.h> /* If "Generate peripheral initialization as a pair of '.c/.h'files per peripheral"
 	 	 	 	 	 not checked in CubeMX, comment out #include */
#endif

#ifdef USE_RTC_FUNCTIONS
#ifdef HAL_RTC_MODULE_ENABLED
#include <rtc.h> /* If "Generate peripheral initialization as a pair of '.c/.h'files per peripheral"
 	 	 	 	 	 not checked in CubeMX, comment out #include */
#endif
#endif

#ifdef USE_IWDG_FUNCTIONS
#ifdef HAL_IWDG_MODULE_ENABLED
#include <iwdg.h> /* If "Generate peripheral initialization as a pair of '.c/.h'files per peripheral"
 	 	 	 	 	 not checked in CubeMX, comment out #include */
#endif
#endif

#ifdef HAL_I2C_MODULE_ENABLED

#endif

/* Linker to header file */
#include "__ExegerGeneric.h"


/* @info
 * Name Heartbeat and debugLed in CubeMX
 *
 * In case no LEDs have been initialized in CubeMX
 *
 *  */
#ifdef USE_GPIO_FUNCTIONS
#ifndef HeartBeat_Pin
#ifdef HAL_GPIO_MODULE_ENABLED
#define HeartBeat_Pin GPIO_PIN_11
#else
#define HeartBeat_Pin LL_GPIO_PIN_13
#endif
#define HeartBeat_GPIO_Port GPIOA
#endif

#ifndef debugLed_Pin
#ifdef HAL_GPIO_MODULE_ENABLED
#define debugLed_Pin GPIO_PIN_12
#else
#define debugLed_Pin LL_GPIO_PIN_7
#endif
#define debugLed_GPIO_Port GPIOA
#endif
#endif

/* In case that the Leds used does not have the layout as
 * shown below, change variable LEDS_3V_TO_MCU to 0.
 *
 * If the Leds (HeartBeat and debugLed) is powered from 3v plane
 * and goes to MCU_Gnd to light up
 *
 * 			   [LED]
 * 		3v --- -|>- --- [Resistor] --- MCU
 *
 *  */
#define LEDS_3V_TO_MCU 1

#if(LEDS_3V_TO_MCU == 1)
#define LED_ON 0
#define LED_OFF 1
#else
#define LED_ON 1
#define LED_OFF 0
#endif


/* Variable for the printf function which UART-port it should
 * print to, nicer to just change on one place than everywhere.
 * */
#ifdef USE_USART_FUNCTIONS
#ifndef HAL_UART_MODULE_ENABLED
USART_TypeDef* USART_Generic = USART1;
#else
UART_HandleTypeDef* huart_Generic = &huart2;
#endif
#endif

/*______________________________________________________________*/
/*
 *					Functions in document
 *
 * 						printf
 * 						directPrint (dPrint)
 * 						debugPrint
 * 						HeartBeatLed-functions
 * 						debugLed-functions
 * 						Bit checking functions
 * 						InfoLog functions
 * 						WatchDog
 *
 */
/*______________________________________________________________*/


#ifdef USE_USART_FUNCTIONS
#ifndef HAL_UART_MODULE_ENABLED /* (LL_DRIVERS) */

/*-------------------------------------------------------------------------------------------------
  Name         : printf()
  Description  : The printf functions are hard coded to one specific USART port,
 	 	 	 	 change to the port you are using.
  Argument(s)  : Use as standard printf, MUST add \r\n to end!
				 printf("string\r\n");
				 or
				 printf("Variable 1: %d\r\n, var1); to get the variable printed
--------------------------------------------------------------------------------------------------*/
int _write(int file, char *data, int len)
{
	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
	{
		errno = EBADF;
		return -1;
	}
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		while (!LL_USART_IsActiveFlag_TXE(USART_Generic))
		{
		}

		LL_USART_TransmitData8(USART_Generic, *data++);
	}
	/* Wait for TC flag to be raised for last char */
//	while (!LL_USART_IsActiveFlag_TC(LPUART1))
//	{
//	}

	return 0;
}

/*-------------------------------------------------------------------------------------------------
  Name         :  dPrint (directPrint)
  Description  :  Print to computer (HAL) without the need of \r\n
  Argument(s)  :  first arg defines which uart port to use,
  	  	  	  	  second arg is the string you want to print
--------------------------------------------------------------------------------------------------*/
void dPrint(USART_TypeDef *USARTx, char* _out)
{
	uint8_t len = strlen(_out);

	for(int i = 0; i < len; i++)
	{
		while (!LL_USART_IsActiveFlag_TXE(USART_Generic))
		{
		}
		LL_USART_TransmitData8(USART_Generic, *_out++);
	}

}

#else /* (HAL DRIVERS) */


/*-------------------------------------------------------------------------------------------------
  Name         : printf()
  Description  : The printf functions are hard coded to one specific USART port,
 	 	 	 	 change to the port you are using.
  Argument(s)  : Use as standard printf, MUST add \r\n to end!
				 printf("string\r\n");
				 or
				 printf("Variable 1: %d\r\n, var1); to get the variable printed
--------------------------------------------------------------------------------------------------*/
int _write(int file, char *data, int len)
{
	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
	{
		errno = EBADF;
		return -1;
	}
	// arbitrary timeout 1000
	HAL_StatusTypeDef status = HAL_UART_Transmit(huart_Generic, (uint8_t*) data, len, 1000);

	// return # of bytes written - as best we can tell
	return (status == HAL_OK ? len : 0);
}

/*-------------------------------------------------------------------------------------------------
  Name         :  dPrint (directPrint)
  Description  :  Print to computer (HAL) without the need of \r\n
  Argument(s)  :  first arg defines which uart port to use,
  	  	  	  	  second arg is the string you want to print
--------------------------------------------------------------------------------------------------*/
void dPrint(UART_HandleTypeDef *huart, char _out[])
{
	HAL_UART_Transmit(huart, (uint8_t*) _out, strlen(_out), 10);
}

#endif

/*-------------------------------------------------------------------------------------------------
  Name         :  debugPrint
  Description  :  Print to computer and add a debug message infront,
  	  	  	  	  Writes to same UART port that printf defines
  Argument(s)  :  First arg is string to be printed, second arg is debugLevel
  	  	  	  	  See enum for info about second argument
--------------------------------------------------------------------------------------------------*/
void debugPrint(char* out, uint8_t debugLevel)
{
	if(debugLevel == debugLevel_Print)
	{
		printf("--%s\r\n", out);
	}
	else if(debugLevel == debugLevel_Info)
	{
		printf("[Info] - %s\r\n", out);
	}
	else if(debugLevel == debugLevel_Warning)
	{
		printf("[Warning] - %s\r\n", out);
	}
	else if(debugLevel == debugLevel_Error)
	{
		printf("[Error] -- %s\r\n", out);
	}
	else
	{
		printf("__ No debugLevel given __\r\n");
	}
}

//const char *DbgTraceGetFileName(const char *fullpath)
//{
//  const char *ret = fullpath;
//
//  if (strrchr(fullpath, '\\') != NULL)
//  {
//    ret = strrchr(fullpath, '\\') + 1;
//  }
//  else if (strrchr(fullpath, '/') != NULL)
//  {
//    ret = strrchr(fullpath, '/') + 1;
//  }
//
//  return ret;
//}

#endif

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * End of print functions
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/



/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * 						Led functions
 *
 * 						Led Blinky
 * 						Led Toggle
 * 						Led On
 * 						Led Off
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * */

#ifdef USE_GPIO_FUNCTIONS
#ifndef HAL_GPIO_MODULE_ENABLED

/*-------------------------------------------------------------------------------------------------
  Name         :  Blinks Heartbeat Led
  Description  :  Blinks the Heartbeat Led rapidly for 1 sec
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void HeartBeatLedBlinky(void)
{
	for(int i = 0; i < 20; i++)
	{
		LL_GPIO_TogglePin(HeartBeat_GPIO_Port, HeartBeat_Pin);
		LL_mDelay(50);
	}
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Toggles Heartbeat Led
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void HeartBeatToggle(void)
{
	LL_GPIO_TogglePin(HeartBeat_GPIO_Port, HeartBeat_Pin);
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Turns Heartbeat Led on
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void HeartBeatOn(void)
{
	if(LED_ON == 0)
	{
		LL_GPIO_ResetOutputPin(HeartBeat_GPIO_Port, HeartBeat_Pin);
	}
	else
	{
		LL_GPIO_SetOutputPin(HeartBeat_GPIO_Port, HeartBeat_Pin);
	}

}

/*-------------------------------------------------------------------------------------------------
  Name         :  Turns Heartbeat Led off
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void HeartBeatOff(void)
{
	if(LED_OFF == 1)
	{
		LL_GPIO_SetOutputPin(HeartBeat_GPIO_Port, HeartBeat_Pin);
	}
	else
	{
		LL_GPIO_ResetOutputPin(HeartBeat_GPIO_Port, HeartBeat_Pin);
	}

}

/*-------------------------------------------------------------------------------------------------
  Name         :  Toggles debugLed Led
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void debugLedToggle(void)
{
	LL_GPIO_TogglePin(debugLed_GPIO_Port, debugLed_Pin);
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Turns debugLed Led on
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void debugLedOn(void)
{
	if(LED_ON == 0)
	{
		LL_GPIO_ResetOutputPin(debugLed_GPIO_Port, debugLed_Pin);
	}
	else
	{
		LL_GPIO_SetOutputPin(debugLed_GPIO_Port, debugLed_Pin);
	}

}

/*-------------------------------------------------------------------------------------------------
  Name         :  Turns debugLed Led off
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void debugLedOff(void)
{
	if(LED_OFF == 1)
	{
		LL_GPIO_SetOutputPin(debugLed_GPIO_Port, debugLed_Pin);
	}
	else
	{
		LL_GPIO_ResetOutputPin(debugLed_GPIO_Port, debugLed_Pin);
	}

}
#else

/*-------------------------------------------------------------------------------------------------
  Name         :  Blinks Heartbeat Led
  Description  :  Blinks the Heartbeat Led rapidly for 1 sec
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void HeartBeatLedBlinky(void)
{
	for(int i = 0; i < 20; i++)
	{
		HAL_GPIO_TogglePin(HeartBeat_GPIO_Port, HeartBeat_Pin);
		HAL_Delay(50);
	}
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Toggles Heartbeat Led
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void HeartBeatToggle(void)
{
	HAL_GPIO_TogglePin(HeartBeat_GPIO_Port, HeartBeat_Pin);
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Turns Heartbeat Led on
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void HeartBeatOn(void)
{
	HAL_GPIO_WritePin(HeartBeat_GPIO_Port, HeartBeat_Pin, LED_ON);
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Turns Heartbeat Led off
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void HeartBeatOff(void)
{
	HAL_GPIO_WritePin(HeartBeat_GPIO_Port, HeartBeat_Pin, LED_OFF);
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Toggles debugLed Led
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void debugLedToggle(void)
{
	HAL_GPIO_TogglePin(debugLed_GPIO_Port, debugLed_Pin);
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Turns debugLed Led on
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void debugLedOn(void)
{
	HAL_GPIO_WritePin(HeartBeat_GPIO_Port, HeartBeat_Pin, LED_ON);
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Turns debugLed Led off
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void debugLedOff(void)
{
	HAL_GPIO_WritePin(HeartBeat_GPIO_Port, HeartBeat_Pin, LED_OFF);
}

#endif
#endif

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * End of LED functions
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 *					RTC-functions
 *
 *					Set Rtc Time
 *					Get Rtc Time Compressed
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * */

#ifdef USE_RTC_FUNCTIONS
#ifdef HAL_RTC_MODULE_ENABLED

/*-------------------------------------------------------------------------------------------------
  Name         :  Set RTC time
  Description  :  Set Rtc of MCU
  Argument(s)  :  Time_Compressed is a 32 bit compressed time bit array
--------------------------------------------------------------------------------------------------*/
uint8_t SetRtcTime(uint32_t Time_Compressed)
{
	RTC_DateTypeDef sDate1;
	RTC_TimeTypeDef sTime1;

	sDate1.Year =    (0b11111100000000000000000000000000 & Time_Compressed) >> 26;   /// 6bits (0-63), Year has 2000 in offset.
	sDate1.Month =   (0b00000011110000000000000000000000 & Time_Compressed) >> 22;      /// 4bits (0-12)
	sDate1.Date =    (0b00000000001111100000000000000000 & Time_Compressed) >> 17;          /// 5bits (0-31)
	sTime1.Hours =   (0b00000000000000011111000000000000 & Time_Compressed) >> 12;        /// 5bits (0-23)
	sTime1.Minutes = (0b00000000000000000000111111000000 & Time_Compressed) >> 6;         /// 6bits (0-59)
	sTime1.Seconds = (0b00000000000000000000000000111111 & Time_Compressed);
	HAL_RTC_SetTime(&hrtc, &sTime1, FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate1, FORMAT_BIN);

	return HAL_OK;
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Gets RTC time
  Description  :  Gets RTC time of MCU in compressed format
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
uint32_t RTC_Get_DateTime_Comp()
{
	RTC_DateTypeDef sD;
	RTC_TimeTypeDef tS;

	uint32_t dateTime = 0;

	HAL_RTC_GetTime(&hrtc, &tS, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sD, RTC_FORMAT_BIN);

	dateTime = (uint32_t) (0b111111 & sD.Year) << 26;
	dateTime |= (uint32_t) (0b1111 & sD.Month) << 22;
	dateTime |= (uint32_t) (0b11111 & sD.Date) << 17;
	dateTime |= (uint32_t) (0b11111 & tS.Hours) << 12;
	dateTime |= (uint32_t) (0b111111 & tS.Minutes) << 6;
	dateTime |= (uint32_t) (0b111111 & tS.Seconds);

	return dateTime;
}
#endif
#endif

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * End of RTC functions
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * 			Bit checking functions
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * */
/*-------------------------------------------------------------------------------------------------
  Name         :  Set bit with byte size check
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void Set_bit_bc(uint32_t *x, uint8_t bitNum, uint8_t VariableSize)
{
	if(bitNum < VariableSize)
	{
		*x |= (1L << bitNum);
	}
}
/*-------------------------------------------------------------------------------------------------
  Name         : Set bit, no check
  Description  : without bit length check
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void Set_bit(uint16_t *x, uint8_t bitNum)
{
	*x |= (1L << bitNum);
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Clear bit
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void Clear_bit_bc(uint32_t *x, uint8_t bitNum, uint8_t VariableSize)
{
	if(bitNum < VariableSize)
	{
		*x &= ~(1UL << bitNum);
	}
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Clear bit
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void Clear_bit(uint32_t *x, uint8_t bitNum)
{
	*x &= ~(1UL << bitNum);
}


/*-------------------------------------------------------------------------------------------------
  Name         :  Check bit
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
uint8_t Check_bit(uint16_t *x, uint8_t bitNum)
{
	// To check a bit, shift the number n to the right, then bitwise AND it:
	return (*x >> bitNum) & 1UL;

}
/**-------------------------------------------------------------------------------------------------
  Name         :

  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
uint8_t IsCharHex(char Data)
{
	if (('A' <= Data && Data <= 'F') || ('a' <= Data && Data <= 'f') || ('0' <= Data && Data <= '9'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * End of Bit checking functions
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/



/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * 					InfoLog functions
 *
 * 					GetInfo
 * 					Init
 * 					Reset
 * 					AddLog
 * 					GetSize
 * 					Print
 * 					CopyToAnother
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *  */

/*-------------------------------------------------------------------------------------------------
  Name         :  Get info struct
  Description  :  To be used with InfoAddLog
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
Info *GetInfoStruct(void)
{
	return &globalInfo;
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Get info struct
  Description  :  To be used with InfoAddLog
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
Info *GetTempInfoStruct(void)
{
	return &tempInfo;
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Init info struct
  Description  :
  Argument(s)  :  Info struct that you want to init
--------------------------------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------------------------------
  Name         :  Reset info struct
  Description  :
  Argument(s)  :  Info struct that you want to reset
--------------------------------------------------------------------------------------------------*/
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

/*-------------------------------------------------------------------------------------------------
  Name         :  Add info to Log
  Description  :
  Argument(s)  :  errorLevel is the severity of error
  	  	  	  	  function - where error occurs
  	  	  	  	  subfunction - where in function error occurs
  	  	  	  	  type - read/write error
  	  	  	  	  status - passed/failed
  	  	  	  	  valueA/B/C - if a value should be logged, different sizes
  	  	  	  	  iL - infoLog, use GetInfoStruct() as input
--------------------------------------------------------------------------------------------------*/
void InfoLogAdd(ErrorLevel errorLevel, uint8_t function, uint8_t subFunction, uint8_t type,
		uint8_t status, uint8_t valueA, uint16_t valueB, uint32_t valueC, Info *iL)
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

/*-------------------------------------------------------------------------------------------------
  Name         :  Get Size of info struct
  Description  :  Can be used for example with SD-card writing
  Argument(s)  :  use GetInfoStruct() as input
--------------------------------------------------------------------------------------------------*/
uint32_t InfoGetSize(Info *in)
{
	uint32_t size = in->infoCounter * sizeof(InfoLog);
	return size;
}

/*-------------------------------------------------------------------------------------------------
  Name         :  Print info log with printf
  Description  :
  Argument(s)  :  use GetInfoStruct() as input
--------------------------------------------------------------------------------------------------*/
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

#ifdef USE_USART_FUNCTIONS
/*-------------------------------------------------------------------------------------------------
  Name         :  Copies from temp struct to global info struct
  Description  :  if one uses temp struct for each iteration of main loops,
  	  	  	  	  save everything in temp struct first and then copy it to a global struct
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
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
#endif

/*-------------------------------------------------------------------------------------------------
  Name         :  Checks for error in input info struct
  Description  :
  Argument(s)  : use GetInfoStruct() as input
--------------------------------------------------------------------------------------------------*/
uint8_t CheckForErrorInInfoStruct(Info *iL)
{
	if(iL->highestError >= ERRORLEVEL_WARNING)
	{
		return 1;
	}
	else
		return 0;
}

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * End of Info Log functions
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifdef USE_IWDG_FUNCTIONS
#ifdef HAL_IWDG_MODULE_ENABLED
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * 								Watchdog Function
 *
 * 								Reset watchdog
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/*-------------------------------------------------------------------------------------------------
  Name         :  Refresh Watchdog counter so that MCU doesnt restart
  Description  :
  Argument(s)  :
--------------------------------------------------------------------------------------------------*/
void RefreshWatchDogCounter(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}
#endif
#endif

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 *
 * End of Watchdog functions
 *
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
