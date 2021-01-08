/*
 * _Global.h
 *
 *  Created on: 5 jan. 2021
 *      Author: rasmu
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "main.h"
#include "__ExegerGeneric.h"

#define SENSOR_STRUCT_ARRAY_SIZE	1440					// measures once every 2 minutes -> 720 measures a day.
#define TX_BUFF_SIZE				256
#define DEFAULT_TIMEOUT				1000
#define LOW_TIMEOUT 				10
#define POWEROFF_TIMEOUT			5
#define MAXIMUM_WAIT_FOR_NETWORK	300

/*############ Structs that will be sent #############*/

typedef struct Spectrum
{
	uint16_t violet;
	uint16_t blue;
	uint16_t green;
	uint16_t yellow;
	uint16_t orange;
	uint16_t red;
} Spectrum;

typedef struct GPS
{
	uint32_t longitudeLatitude;
} GPS;

typedef struct SensorData
{
	uint32_t tempHumLux;
	Spectrum spectrum;
} SensorData;

typedef struct ReadingData
{
	uint32_t timeStamp;
	uint8_t batteryLevel;
	SensorData sData[SENSOR_STRUCT_ARRAY_SIZE];
} ReadingData;

typedef struct Data
{
	ReadingData rData;
	uint16_t sensCounter;
} Data;

typedef struct Accelerometer
{
	uint32_t xyz;
} Accelerometer;

typedef struct TransmitData
{
	Data data;
	GPS gps;
	Accelerometer acc;
} TransmitData;

/*####################################################*/

typedef struct UART
{
	uint16_t Head;
	uint16_t Tail;
} UART;

typedef struct Time
{
	uint8_t Year;
	uint8_t Month;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Minute;
	uint8_t Second;
} Time;

typedef struct RadioFlags
{
	/* Some errors? */
} RadioFlags;

typedef struct SensorFlags
{
	/* Some errors? */
} SensorFlags;

typedef struct SaraConfig
{
	char id[32];
	char ip[32];
	int port;
	int revision;
	int updateFrequency;
} SaraConfig;

extern uint16_t luxCounter;

Data *GetDataStruct(void);
SaraConfig *GetSaraConfigurationStruct();
RadioFlags *GetRadioFlagStruct();
SensorFlags *GetSensorFlagStruct();
void DataStruct_init(Data *data);
void PrintsDatasorData(Data *data);


RadioFlags rFlags;
SensorFlags sFlags;
SaraConfig sConfig;
InfoLog iLog;
Data dataGlobal;


#endif /* GLOBAL_H_ */
