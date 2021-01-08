/*
 * _Global.c
 *
 *  Created on: 5 jan. 2021
 *      Author: rasmu
 */

#include "_Global.h"
#include "__ExegerGeneric.h"

volatile char sd_fileName[13];

uint16_t sensorCounter = 0;


Data *GetDataStruct(void)
{
	return &dataGlobal;
}


SaraConfig *GetSaraConfigurationStruct()
{
	return &sConfig;
}

RadioFlags *GetRadioFlagStruct()
{
	return &rFlags;
}

SensorFlags *GetSensorFlagStruct()
{
	return &sFlags;
}


void DataStruct_init(Data *data)
{
	data->rData.timeStamp = 0;
	data->rData.batteryLevel = 0;

	for (int i = 0; i > SENSOR_STRUCT_ARRAY_SIZE; i++)
	{
		data->rData.sData[i].tempHumLux = 0;
		data->rData.sData[i].spectrum.violet = 0;
		data->rData.sData[i].spectrum.blue = 0;
		data->rData.sData[i].spectrum.green = 0;
		data->rData.sData[i].spectrum.yellow = 0;
		data->rData.sData[i].spectrum.orange = 0;
		data->rData.sData[i].spectrum.red = 0;
	}

}

/*
 * Prints the collected sDatasorData
 */
void PrintsDatasorData(Data *data)
{
	printf(
			"\t Time[0x%lx] Battery[%d] \r\n ",
			data->rData.timeStamp,
			data->rData.batteryLevel);

	uint16_t i;

	for (i = 0; i < data->sensCounter; i++)
	{
		printf("\t Temp[%lu] Humidity[%lu] \t Lux[%lu] Violet[%u] Blue[%u] Green[%u] Yellow[%u] Orange[%u] Red[%u]  \r\n ", ((data->rData.sData[i].tempHumLux >> 7) & 0b11111111) / 5,
				(data->rData.sData[i].tempHumLux & 0b1111111), data->rData.sData[i].tempHumLux >> 15, data->rData.sData[i].spectrum.violet,
				data->rData.sData[i].spectrum.blue,
				data->rData.sData[i].spectrum.green, data->rData.sData[i].spectrum.yellow, data->rData.sData[i].spectrum.orange, data->rData.sData[i].spectrum.red);
	}
	printf("\r\n\r\n");
}
