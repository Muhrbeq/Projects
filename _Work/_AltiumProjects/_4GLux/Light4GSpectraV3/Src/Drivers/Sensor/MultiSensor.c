/*
 * MultiSensor.c
 *
 *  Created on: 5 Dec 2018
 *      Author: andreas.fredlund
 */

#include "MultiSensor.h"
#include "AS726x.h"
#include "HDC1080.h"
#include "Tmp112.h"
#include "Global.h"
#include "VEML7700.h"

extern uint16_t testCounter;

/*
 * Todo Organize and change name to this file
 *
 */

void MultiSensor_takeMeasurements(SensorData *sd)				// todo try to make this simultaneously to reduce sensor reading time
{
	testCounter++;

	float lux;
	VEML7700_GetAutoALSLux(&lux);
	sd->tempHumLux = (((0b11111111 & TMP112_ReadTemp()) << 7) | (0b1111111 & HDC1080_Get_Humidity())
			| ((0b11111111111111111 & (int) lux) << 15));

	HAL_GPIO_WritePin(Temperature_EN_GPIO_Port, Temperature_EN_Pin, GPIO_PIN_RESET);

	AS726X_takeMeasurements(AS7262_GAIN_3_7, &(sd->spectrum));
}

void MultiSensor_takeMeasurementsRaw(SensorData *sd)
{
	AS726X_takeMeasurementsRaw(AS7262_GAIN_3_7, &(sd->spectrum));
//	sd->lux = TSL2561_getLux();
//	sd->tempHum = ((0b111111111 & TMP112_ReadTemp()) << 7) | (0b1111111 & HDC1080_Get_Humidity());
}


void DataStruct_init(Data *data)
{
	data->sensCounter = 0;
	data->dayCounter = 0;
	data->readData.timeStamp = 0;
	data->readData.batteryLevel = 0;
	data->readData.foo = 0;
	data->readData.fooo = 0;

	for (int i = 0; i > SENSOR_STRUCT_ARRAY_SIZE; i++)
	{
		data->readData.sensData[i].tempHumLux = 0;
		data->readData.sensData[i].spectrum.violet = 0;
		data->readData.sensData[i].spectrum.blue = 0;
		data->readData.sensData[i].spectrum.green = 0;
		data->readData.sensData[i].spectrum.yellow = 0;
		data->readData.sensData[i].spectrum.orange = 0;
		data->readData.sensData[i].spectrum.red = 0;
	}

}


/*
 *  Gets the size in bytes of the data struct that has been written to it. The whole struct with zeros has size
 *  sizeof(Data) but a big part of if could be zeros.
 *
 *  This size increments in every loop as d-sensCounter increases.
 */
uint32_t GetDataSize(Data *d)
{
	uint32_t size = sizeof(d->readData.timeStamp);
	size += sizeof(d->readData.batteryLevel);
	size += sizeof(d->readData.foo);
	size += sizeof(d->readData.fooo);
	size += d->sensCounter * sizeof(SensorData);

	return size;
}

/*
 * Prints the collected sensorData
 */
void PrintSensorData(Data *data)
{
	printf(
			"\t Time[0x%lx] Battery[%d] \r\n ",
			data->readData.timeStamp,
			data->readData.batteryLevel);

	uint16_t i;

	for (i = 0; i < data->sensCounter; i++)
	{
		printf("\t Temp[%lu] Humidity[%lu] \t Lux[%lu] Violet[%u] Blue[%u] Green[%u] Yellow[%u] Orange[%u] Red[%u]  \r\n ", ((data->readData.sensData[i].tempHumLux >> 7) & 0b11111111) / 5,
				(data->readData.sensData[i].tempHumLux & 0b1111111), data->readData.sensData[i].tempHumLux >> 15, data->readData.sensData[i].spectrum.violet,
				data->readData.sensData[i].spectrum.blue,
				data->readData.sensData[i].spectrum.green, data->readData.sensData[i].spectrum.yellow, data->readData.sensData[i].spectrum.orange, data->readData.sensData[i].spectrum.red);
	}
	printf("\r\n\r\n");
}
