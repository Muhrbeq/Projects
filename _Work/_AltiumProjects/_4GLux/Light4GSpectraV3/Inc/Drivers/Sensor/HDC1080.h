/*
 * HDC1080.h
 *
 *  Created on: 27 Nov 2018
 *      Author: andreas.fredlund
 */

#ifndef HDC1080_H_
#define HDC1080_H_

#include "Global.h"

/*
 *  Defines for Humidity sensor HDC1080
 */
#define HDC1080_ADDRESS   			0x40    //7-bit unshifted default I2C Address
#define HDC1080_ADDR_WRITE  		0x80
#define HDC1080_ADDR_READ   		0x81

//Registers
#define HDC1080_TEMP_REG			0x00	//	Temperature measurement output
#define HDC1080_HUM_REG				0x01	//	Relative Humidity measurement output
#define HDC1080_CONF_REG			0X02	//	HDC1080 configuration and status
#define HDC1080_ID_REG_FIRST		0xFB	//	Serial ID device dependent First 2 bytes of the serial ID of the part
#define HDC1080_ID_REG_MID			0xFC	// 	Serial ID device dependent Mid 2 bytes of the serial ID of the part
#define HDC1080_ID_REG_LAST			0xFD	//	Serial ID device dependent Last byte bit of the serial ID of the part
#define HDC1080_MAN_ID_REG			0xFE	//	ID REG of the device
#define HDC1080_DEV_ID_REG			0xFF	//  ID REG of Texas Instruments

#define HDC1080_DEVICE_ID			0x1050  //	ID of the device
#define HDC1080_MAN_ID				0x5449	//  ID of Texas Instruments

#define HDC1080_TIMEOUT_TX_END		100 	// Counts in transmit (write) loop
#define HDC1080_TIMEOUT_RX_END		100		// Counts in receive loop
#define HDC1080_ERROR_VALUE			0xDEAD

uint8_t HDC1080_readRegister(unsigned char* data, uint8_t len, uint8_t throw);
uint8_t HDC1080_writeRegister(unsigned char* data, uint8_t len, uint8_t throw);
uint16_t HDC1080_Get_Device_ID(void);
uint16_t HDC1080_Get_Temp(void);
uint8_t HDC1080_Get_Humidity(void);
uint16_t HDC1080_Get_Temp_raw(void);
uint16_t HDC1080_Get_Humidity_raw(void);
void HDC1080_Init(void);

#endif /* HDC1080_H_ */