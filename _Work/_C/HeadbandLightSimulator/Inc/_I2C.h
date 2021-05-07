/*
 * _I2C.h
 *
 *  Created on: 2 feb. 2021
 *      Author: Rasmus.Muhrbeck
 */

#ifndef I2C_H_
#define I2C_H_

#include "main.h"

/* Generic I2C file */

#define I2C_TIMEOUT_TX_END	1000
#define I2C_TIMEOUT_RX_END	1000

uint8_t I2C_WriteRegister(I2C_HandleTypeDef *hi2c, uint16_t addr,
		unsigned char* data, uint8_t len);
uint16_t I2C_ReadRegister(I2C_HandleTypeDef *hi2c,
		uint16_t writeAddr, uint16_t readAddr,
		unsigned char* data, uint8_t len);
uint8_t I2C_Write(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t* data, uint8_t len);
uint8_t I2C_ReadWrite(I2C_HandleTypeDef *hi2c,
		uint16_t writeAddr, uint8_t* data, uint8_t len);
uint16_t I2C_ReadRead(I2C_HandleTypeDef *hi2c,
		uint16_t readAddr, uint8_t* data, uint8_t len);

#endif /* I2C_H_ */
