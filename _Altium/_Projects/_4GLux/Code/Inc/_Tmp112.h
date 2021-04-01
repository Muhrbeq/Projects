/*
 * Tmp112.h
 *
 *  Created on: 22 Nov 2018
 *      Author: andreas.fredlund
 */

#ifndef TMP112_H_
#define TMP112_H_

#include "main.h"

/*
 * Defines for TMP112
 */
#define TMP112_ADDRESS   			0x48    //todo 7-bit unshifted default I2C Address
#define TMP112_ADDR_WRITE  			0x90
#define TMP112_ADDR_READ   			0x91

/** Internal register within TMP112 */
#define TMP112_TEMP_REG       		0x00
#define TMP112_CONF_REG        		0x01
#define TMP112_TLOW_REG       		0x02
#define TMP112_THIGH_REG      		0x03

/* Data to return if its an invalid temperature */
#define TMP112_INVALID_TEMP 		0x0000				//(0x9C0) -100C
#define TMP112_TIMEOUT_TX_END		50 			// todo Counts in transmit (write) loop
#define TMP112_TIMEOUT_RX_END		50			// Counts in receive loop
#define TMP112_MAX_TEMP				100000			// 100 degrees

typedef enum TMP112_State
{
	TMP112_INIT,
	TMP112_STARTONESHOT,
	TMP112_WAITFORONESHOTWRITE,
	TMP112_WAITFORONESHOTREAD,
	TMP112_GETTEMPERATURERAWWRITE,
	TMP112_GETTEMPERATURERAWREAD,
	TMP112_CALCULATETEMPERATURE,
	TMP112_DONE,
	TMP112_ERROR

}TMP112_State;

typedef struct TMP112
{
	TMP112_State tState;
	I2C_HandleTypeDef hi2c;
	uint8_t i2cWrite;
	uint8_t i2cRead;

	uint16_t temperatureRaw;
	int32_t temperature;
}TMP112;

TMP112_State TMP112_GetState();

//uint8_t TMP112_writeRegister(I2C_HandleTypeDef hi2c, unsigned char* data, uint8_t len);
//uint16_t TMP112_readRegister(I2C_HandleTypeDef hi2c, unsigned char* data, uint8_t len);
//uint8_t TMP112_GetTemperature_raw(I2C_HandleTypeDef hi2c, uint16_t *get);
//uint8_t TMP112_StartOneShot(I2C_HandleTypeDef hi2c);
//uint8_t TMP112_WaitForOneShot(I2C_HandleTypeDef hi2c);
//
//uint16_t TMP112_ReadTemp_raw(I2C_HandleTypeDef hi2c);
//void TMP112_Init(I2C_HandleTypeDef hi2c);
//void TMP112_ShutDown(I2C_HandleTypeDef hi2c);
//int32_t TMP112_ReadTemp(I2C_HandleTypeDef hi2c);


#endif /* TMP112_H_ */
