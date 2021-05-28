/*
 * Global.h
 *
 *  Created on: 17 Oct 2018
 *      Author: andreas.fredlund
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "main.h"
#include "adc.h"
#include "fatfs.h"
#include "i2c.h"
#include "rtc.h"
#include "lptim.h"
//#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "iwdg.h"

#include "bsp_driver_sd.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"
#include <stm32l4xx_hal_i2c.h>
#include "stm32l4xx_hal_flash.h"
#include "stm32l4xx_hal_flash_ex.h"
#include "stm32l452xx.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"
#include  <errno.h>
#include  <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO
#include <string.h>
#include "time.h"
#include <stdbool.h>
#include "crc.h"
//#include "_write.h"
#include "sdmmc.h"

/*
 * Defines for the error log
 */
#define ERROR_LOGG_ARRAY_SIZE		30   //TODO to be decided. If we need to write more to the buffer. what to do? >30 errors. right now the program hangs


//	8 bit
#define LOG_FUNC_FLASH				0x11
#define LOG_FUNC_POWERSAVE			0x12
#define LOG_FUNC_BATTERY			0x13
#define LOG_FUNC_AS726X				0x14
#define LOG_FUNC_SD					0x15
#define LOG_FUNC_TMP112				0x16
#define LOG_FUNC_ERROR				0x17
#define LOG_FUNC_HDC1080			0x18
#define LOG_FUNC_TSL2561			0x19
#define LOG_FUNC_MAIN				0x1A
#define LOG_FUNC_STATE				0x1B
#define LOG_FUNC_SETTING			0x1C
#define LOG_FUNC_VEML7700			0x1D

//8 bit
#define LOG_SUB_NONE				0x11
#define LOG_SUB_DATA 				0x12
#define LOG_SUB_SECTOR				0x13
#define LOG_SUB_VIR_READ            0x14
#define LOG_SUB_READ				0x15
#define LOG_SUB_VIR_WRITE	        0x16
#define LOG_SUB_WRITE				0x17
#define LOG_SUB_FAT_MOUNT			0x18
#define LOG_SUB_SD_FILE_EXIST		0x19
#define LOG_SUB_SD_GETNUM_FILES		0x1A
#define LOG_SUB_FAT_LINK			0x1B
#define LOG_SUB_WAIT_ONESHOT		0x1C
#define LOG_SUB_READTEMP			0x1D
#define LOG_SUB_ADD					0x1E
#define LOG_SUB_WRITE_BUFFER		0x1F
#define LOG_SUB_WRITE_ERROR			0x20
#define LOG_SUB_RESET				0x21
#define LOG_SUB_SETADDR				0x22
#define LOG_SUB_NORM				0x23
#define LOG_SUB_MEASURE				0x24
#define LOG_SUB_MEASURE_RAW			0x25
#define LOG_SUB_VERIFY				0x26
#define LOG_SUB_INIT				0x27
#define LOG_SUB_ERASE				0x28
#define LOG_SUB_NETWORK				0x29
#define LOG_SUB_RSSI				0x31
#define LOG_SUB_CLOCK				0x32
#define LOG_SUB_SERVER				0x33
#define LOG_SUB_PARSE				0x34
#define LOG_SUB_JSMN				0x35
#define LOG_SUB_DAILY				0x36
#define LOG_SUB_CRITICAL			0x37
#define LOG_SUB_AWAKE				0x38
#define LOG_SUB_SENSOR				0x39


/// 8 bits
#define LOG_TYPE_NONE               0x11
#define LOG_TYPE_ERASE              0x12
#define LOG_TYPE_ID					0x13
#define LOG_TYPE_READ               0x14
#define LOG_TYPE_WRITE              0x15
#define LOG_TYPE_START              0x16
#define LOG_TYPE_SET				0x17
#define LOG_TYPE_OPEN				0x18
#define LOG_TYPE_SEEK				0x19
#define LOG_TYPE_TIMESTAMP			0x1A
#define LOG_TYPE_START_ONESHOT		0x1B
#define LOG_TYPE_WAIT_ONESHOT		0x1C
#define LOG_TYPE_GET_TEMP			0x1D
#define LOG_TYPE_BUFFER_FULL		0x1E
#define LOG_TYPE_GET_HUM			0x1F
#define LOG_TYPE_INIT				0x20
#define LOG_TYPE_CALC				0x21
#define LOG_TYPE_ADDRESS			0x22
#define LOG_TYPE_MOUNT				0x23
#define LOG_TYPE_CALIBRATION        0x24
#define LOG_TYPE_COMPARE	        0x25
#define LOG_TYPE_CONNECT			0x26
#define LOG_TYPE_SUBSCRIBE			0x27
#define LOG_TYPE_FWRITE				0x28
#define LOG_TYPE_SHUTDOWN			0x29
#define LOG_TYPE_GAIN				0X30
#define LOG_TYPE_INT_TIME			0x31
#define LOG_TYPE_PERSIST			0X32
#define LOG_TYPE_POWMODE			0X33
#define LOG_TYPE_SETPOWSAVE			0X34
#define LOG_TYPE_INTERRUPT			0x35
#define LOG_TYPE_SETPOW 			0X36
#define LOG_TYPE_SETALSH 			0X37
#define LOG_TYPE_SETALSL 			0X38
#define LOG_TYPE_AWAKE				0x39

/// 8 bits
#define LOG_STATUS_FAIL             0x11
#define LOG_STATUS_PASSED           0x12
#define LOG_STATUS_TIMEOUT			0x13
#define LOG_STATUS_MODE             0x14
#define LOG_STATUS_MAX				0x15
#define LOG_STATUS_MIN				0x16
/*
 * Defines for the sensor buffer
 *
 * Struct of spectrum (6*16 bit) + lux (16 bit) + merged temp and humidity (16 bit). 16 bytes per measurement.  --> 4 registers in flash
 * Have also 4 bytes timestamp and 1 byte Battery value.  --> 2 registers in flash
 * n measurements + timestamp + battery takes up 4*n + 2 registers in flash. (4*(4n + 2) bytes)
 *
 */
#define SENSOR_STRUCT_ARRAY_SIZE	1440					// measures once every 2 minutes -> 720 measures a day.
#define INFO_LOGG_ARRAY_SIZE		500

#define SYSTICK_IWDG 				25000

typedef struct Spectrum
{
	uint16_t violet;
	uint16_t blue;
	uint16_t green;
	uint16_t yellow;
	uint16_t orange;
	uint16_t red;

} Spectrum;

typedef struct SensorData
{
	uint32_t tempHumLux; 			// temperature and humidity
	//uint16_t lux;				// todo foo. lux -> 17 bitar
	Spectrum spectrum;
} SensorData;

typedef struct ReadingData
{
	uint32_t timeStamp;
	uint8_t batteryLevel;
	uint8_t foo;			//fill out to get even 4 byte when starting saving the sensordata
	uint16_t fooo;
	SensorData sensData[SENSOR_STRUCT_ARRAY_SIZE];
} ReadingData;

typedef struct Data
{
	ReadingData readData;
	uint16_t sensCounter;
	uint8_t dayCounter;
} Data;

struct Globals
{
	uint16_t Head;
	uint16_t Tail;
};

typedef struct Time
{
	uint8_t Year;
	uint8_t Month;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Minute;
	uint8_t Second;
} Time;

typedef struct errorTimer
{
	Time oneHourError;
} errorTimer;

/* All sorts of different flags that the radio cna encounter */
typedef struct radioFlags
{
	uint8_t errorHourFlag;
	uint8_t errorDayFlag;
	uint8_t radioEnableFlag;
	uint8_t errorHourCounter;
	uint8_t errorDayCounter;
	uint8_t testCounterGetSignal;
	uint8_t testCounterConnectServer;
	uint8_t testCounterTime;
	uint8_t publishError;
	uint8_t subscribeError;
	uint8_t updateError;
	uint8_t noNewSettings;
	uint8_t serverDo;
	uint8_t startUpError;
	uint8_t AtInit;
	uint8_t setServerError;
	uint8_t dataPublishedFlag;
	uint8_t adjustedHours;
	uint8_t adjusted;
	uint8_t firstRun;
	uint8_t errorStart;
	uint8_t everythingOK;
	uint8_t firstErrorOfDay;
	uint16_t counterMessage;
	uint8_t alreadyConnected;
	uint8_t triedToSendCount;
	uint8_t trySubscribeAgain;
	uint8_t subscribeErrorCounter;
	uint8_t radioRTC;
	uint8_t radioPublishCounter;
} radioFlags;

/* Max 255 "pages", 255*1000 = 255000 chars */
struct communicationServer
{
	uint8_t prevMessageReceived;
	uint8_t messageReceived;
	uint8_t messageTotal;
	uint8_t dataSentToServerFlag;
	uint8_t serverFwFlag;
	uint8_t newSettingsServer;
};

typedef struct SaraConfig
{
	char id[32];
	char ip[32];
	int port;
	char revision[8];
	int serverToDo;
	char connectSubTopic[16];
	char connectPubTopic[16];
	char connectComTopic[16];
	int updateFrequency;
	uint8_t hourToSend;
	char webPage[32];
	int firstMessageToReSend;
	int lastMessageToReSend;

} SaraConfig;




typedef struct ErrorLog
{
	uint32_t timestamp;
	uint8_t function;
	uint8_t subFunction;
	uint8_t type;
	uint8_t status;
	uint32_t valueA;
	uint32_t valueB;
	uint8_t logLevel;
} ErrorLog;

typedef struct Error
{
	ErrorLog errorLog[ERROR_LOGG_ARRAY_SIZE];
	uint16_t errorCounter;
} Error;

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
	ERRORLEVEL_SEVERE = 0x15,
	ERRORLEVEL_HIGH = 0x16,
	ERRORLEVEL_FATAL = 0x17,
} ErrorLevel;

typedef enum TimeStructs
{
	SaraLastConnectedVal,
	mcuClockTimeVal,
	networkStartVal,
	networkEndVal,
	startTimeToSendVal,
	endTimeToSendVal,
	turnOffStartVal,
	turnOffEndVal,
} TimeStructs;

typedef enum ErrorStartEnd
{
	startTimerEnum,
	endTimerEnum,
} ErrorStartEnd;

/*
 * Struct Globals
 */
extern Data data_global;
extern Error error_global;
extern uint16_t luxCounter;


Data *GetDataStruct(void);
Error *GetErrorStruct(void);
Time *GetTimeStruct(TimeStructs retStruct);
Info *GetInfoStruct(void);
Info *GetTempInfoStruct(void);

SaraConfig *GetSaraConfigurationStruct();

radioFlags *GetErrorFlagsStruct();

errorTimer *GetErrorTimerStruct(ErrorStartEnd state);

#endif /* GLOBAL_H_ */
