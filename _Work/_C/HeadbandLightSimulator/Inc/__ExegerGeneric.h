/*
 * __ExegerGeneric.h
 *
 *  Created on: 8 jan. 2020
 *      Author: Rasmus.Muhrbeck
 */

#ifndef EXEGERGENERIC_H_
#define EXEGERGENERIC_H_

#include "main.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "errno.h"
#include  <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO

#define true                        1
#define false                       0

#define PASSED                      1
#define FAILED                      0

#define DEBUG_PRINT					printf

//#define DEBUG_MSG(...)     do{printf(__VA_ARGS__);}while(0);

#define TRACE_DEBUG(...)//	printf(__VA_ARGS__)
//#define TRACE_DEBUG(...)
#define TRACE(...)		//printf(__VA_ARGS__)
//#define TRACE(...)

#define INFO_LOGG_ARRAY_SIZE		500

/* 8 bit
   #define LOG_FUNC_FILENAME */
#define LOG_FUNC_MAIN				0x11
#define LOG_FUNC_POWERSAVE			0x12
#define LOG_FUNC_STATE				0x13
#define LOG_FUNC_SETTINGS			0x14

#define LOG_FUNC_LUX_SENS			0x21
#define LOG_FUNC_TEMPERATURE_SENS	0x22
#define LOG_FUNC_HUMIDITY_SENS		0x23
#define LOG_FUNC_SPECTRUM_SENS		0x24

#define LOG_FUNC_SD					0x31
#define LOG_FUNC_RADIO				0x32

#define LOG_FUNC_BATTERY			0x41

#define LOG_FUNC_ERROR				0x99


/* 8bit
 * #define LOG_SUB_FUNCTIONNAME */
#define LOG_SUB_NONE				0x11
#define LOG_SUB_INIT				0x12
#define LOG_SUB_ADD					0x13
#define LOG_SUB_ERASE				0x14
#define LOG_SUB_WRITE				0x15
#define LOG_SUB_READ				0x16
#define LOG_SUB_MOUNT				0x17
#define LOG_SUB_MEASURE				0x18
#define LOG_SUB_VERIFY				0x19

#define LOG_SUB_DATA				0x1A
#define LOG_SUB_MEASURE_RAW			0x1B
#define LOG_SUB_TEMP_READ			0x1C
#define LOG_SUB_TEMP_READ_RAW		0x1D
#define LOG_SUB_TEMP_ONESHOT		0x1E

#define LOG_SUB_HUM_READ_RAW		0x21
#define LOG_SUB_HUM_ID				0x22

#define LOG_SUB_SD_FOLDER			0x23
#define LOG_SUB_TIMESTAMP			0x24


/* 8 bits
 * #define LOG_TYPE_WhatFunctionIsDoingNAME */
#define LOG_TYPE_NONE               0x11
#define LOG_TYPE_READ               0x12
#define LOG_TYPE_WRITE              0x13
#define LOG_TYPE_BUFFER_FULL		0x14
#define LOG_TYPE_MOUNT				0x15
#define LOG_TYPE_ID					0x16
#define LOG_TYPE_INIT				0x17
#define LOG_TYPE_OPEN				0x18
#define LOG_TYPE_SEEK				0x19
#define LOG_TYPE_COMPARE			0x1A

/* 8 bits
 * #define LOG_STATUS_ResultNAME */
#define LOG_STATUS_FAIL             0x11
#define LOG_STATUS_PASSED           0x12
#define LOG_STATUS_TIMEOUT			0x13
#define LOG_STATUS_MODE             0x14
#define LOG_STATUS_MAX				0x15
#define LOG_STATUS_MIN				0x16

typedef struct InfoLog
{
	uint32_t startOfInfo;
	uint32_t timestamp;
	uint8_t errorLevel;
	uint8_t function;
	uint8_t subFunction;
	uint8_t type;
	uint8_t status;
	uint8_t valueA;
	uint16_t valueB;
	uint32_t valueC;
	uint32_t endOfInfo;
} InfoLog;

typedef struct Info
{
	InfoLog infoLog[INFO_LOGG_ARRAY_SIZE];
	uint16_t infoCounter;
	uint8_t highestError;
} Info;

typedef enum ErrorLevel
{
	ERRORLEVEL_NONE = 0x11,
	ERRORLEVEL_INFO = 0x12,
	ERRORLEVEL_PASS = 0x13,
	ERRORLEVEL_WARNING = 0x14,
	ERRORLEVEL_FATAL = 0x15,
} ErrorLevel;


enum
{
	debugLevel_Print,
	debugLevel_Info,
	debugLevel_Warning,
	debugLevel_Error
};


#ifndef HAL_UART_MODULE_ENABLED
void dPrint(USART_TypeDef *USARTx, char* _out);
#else
void dPrint(UART_HandleTypeDef *huart, char _out[]);
#endif

void debugPrint(char* out, uint8_t debugLevel);
void HeartBeatLedBlinky(void);
void HeartBeatToggle(void);
void HeartBeatOn(void);
void HeartBeatOff(void);
void debugLedToggle(void);
void debugLedOn(void);
void debugLedOff(void);

#define BitSet(arg , posn) ((arg) |= (1L << (posn)))
#define BitClr(arg , posn) ((arg) &= ~(1L << (posn)))
#define BitTst(arg , posn) BOOL((arg) & (1L << (posn)))
#define BitFlp(arg , posn) ((arg) ^= (1L << (posn)))
#define BOOL(x) (!(!(x)))

void Set_bit_bc(uint32_t *x, uint8_t bitNum, uint8_t VariableSize);
void Set_bit(uint16_t *x, uint8_t bitNum);
void Clear_bit_bc(uint32_t *x, uint8_t bitNum, uint8_t VariableSize);
void Clear_bit(uint32_t *x, uint8_t bitNum);
uint8_t Check_bit(uint16_t *x, uint8_t bitNum);
uint8_t IsCharHex(char Data);

Info *GetInfoStruct(void);
Info *GetTempInfoStruct(void);

void InitInfoStruct(Info *iL);
void ResetInfoStruct(Info *iL);
void InfoLogAdd(ErrorLevel errorLevel, uint8_t function, uint8_t subFunction, uint8_t type,
		uint8_t status, uint8_t valueA, uint16_t valueB, uint32_t valueC, Info *iL);
uint32_t InfoGetSize(Info *in);
void InfoPrintLog(Info *in);
void CopyTempToInfo(void);
uint8_t CheckForErrorInInfoStruct(Info *iL);

void RefreshWatchDogCounter(void);

const char *DbgTraceGetFileName( const char *fullpath );


Info globalInfo;
Info tempInfo;

#endif /* EXEGERGENERIC_H_ */
