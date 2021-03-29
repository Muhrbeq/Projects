/*
 * i2cGeneric.h
 *
 *  Created on: 26 feb. 2020
 *      Author: Rasmus.Muhrbeck
 */

#ifndef I2CGENERIC_H_
#define I2CGENERIC_H_

#include "main.h"

#define I2C_TIMEOUT_TX			100
#define I2C_TIMEOUT_RX			100

uint8_t WriteI2C(I2C_HandleTypeDef *hi2c, uint8_t *addr, unsigned char* data, uint8_t len);

uint16_t ReadI2C(I2C_HandleTypeDef *hi2c, uint8_t *addr, unsigned char* data, uint8_t len);

#endif /* I2CGENERIC_H_ */
