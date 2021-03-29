/*
 * LIS2DW12.h
 *
 *  Created on: 26 feb. 2020
 *      Author: Rasmus.Muhrbeck
 */

#ifndef LIS2DW12_H_
#define LIS2DW12_H_

#include "main.h"

/* Address to registers */
#define LIS_ADDRESS				0x44

#define LIS_READ_ADDR			0x33
#define LIS_WRITE_ADDR			0x32

#define LIS_OUT_T_L					0x0D
#define LIS_OUT_T_H					0x0E
#define LIS_WHO_AM_I				0x0F

#define LIS_CTRL_REG1				0x20
#define LIS_CTRL_REG2				0x21
#define LIS_CTRL_REG3				0x22
#define LIS_CTRL_REG4_INT1			0x23
#define LIS_CTRL_REG5_INT2			0x24
#define LIS_CTRL_REG6				0x25

#define LIS_OUT_T					0x26

#define LIS_STATUS					0x27

#define LIS_OUT_X_L					0x28
#define LIS_OUT_X_H					0x29

#define LIS_OUT_Y_L					0x2A
#define LIS_OUT_Y_H					0x2B

#define LIS_OUT_Z_L					0x2C
#define LIS_OUT_Z_H					0x2D

#define LIS_FIFO_CTRL				0x2E
#define LIS_FIFO_SAMPLES			0x2F

#define LIS_TAP_THS_X				0x30
#define LIS_TAP_THS_Y				0x31
#define LIS_TAP_THS_Z				0x32

#define LIS_INT_DUR					0x33

#define LIS_WAKE_UP_THS				0x34
#define LIS_WAKE_UP_DUR				0x35

#define LIS_FREE_FALL				0x36
#define LIS_STATUS_DUP				0x37

#define LIS_WAKE_UP_SRC				0x38
#define LIS_TAP_SRC					0x39
#define LIS_SIXD_SRC				0x3A
#define LIS_ALL_INT_SRC				0x3B

#define LIS_X_OFS_USR				0x3C
#define LIS_Y_OFS_USR				0x3D
#define LIS_Z_OFS_USR				0x3E

#define LIS_CTRL_REG7				0x3F

/* CTRL_REG1 */
/* Data rate configuration */

#define LIS_ODR_MASK				4

#define LIS_ODR_POWER_DOWN			0x0
#define LIS_ODR_1_HZ				0x1
#define LIS_ODR_12_HZ				0x2
#define LIS_ODR_25_HZ				0x3
#define LIS_ODR_50_HZ				0x4
#define LIS_ODR_100_HZ				0x5
#define LIS_ODR_200_HZ				0x6
#define LIS_ODR_400_HZ				0x7
#define LIS_ODR_800_HZ				0x8
#define LIS_ODR_1600_HZ				0x9

/* Mode selection */

#define LIS_MODE_MASK				2

#define LIS_MODE_LOW_POWER			0x0
#define LIS_MODE_PERFORMANCE		0x1
#define LIS_MODE_SINGLE_DATA		0x2

/* Low power mode selection */

#define LIS_LP_MASK					0

#define LIS_LP_MODE_1				0x0
#define LIS_LP_MODE_2				0x1
#define LIS_LP_MODE_3				0x2
#define LIS_LP_MODE_4				0x3

/* FIFO_CTRL */

#define LIS_FIFO_MASK				5

#define LIS_FIFO_BYPASS				0x0
#define LIS_FIFO_STOP_WHEN_FULL		0x1
#define LIS_FIFO_CONTINOUS_TO_FIFO	0x3
#define LIS_FIFO_BYP_TO_CONT		0x4
#define LIS_FIFO_CONTINOUS			0x6

/* CTRL_REG6 */
/* Full Scale selection */

#define LIS_SCALE_MASK				4

#define LIS_SCALE_2G				0x0
#define LIS_SCALE_4G				0x1
#define LIS_SCALE_8G				0x2
#define LIS_SCALE_16G				0x3

#define LIS_SENSITIVITY_2G			0.061f
#define LIS_SENSITIVITY_4G			0.122f
#define LIS_SENSITIVITY_8G			0.244f
#define LIS_SENSITIVITY_16G			0.488f

typedef struct Acceleration
{
	int16_t x;
	int16_t y;
	int16_t z;
}Acceleration;

Acceleration accStruct_i2c2;
Acceleration accStruct_i2c3;
Acceleration accStruct_i2c4;

Acceleration *LIS_GetAccelerationStruct(I2C_HandleTypeDef *hi2c);
void LIS_initStruct(Acceleration *acc);
uint8_t LIS_ReadID(I2C_HandleTypeDef *hi2c);
uint8_t LIS_Start(I2C_HandleTypeDef *hi2c, uint8_t odr, uint8_t mode, uint8_t fifo, uint8_t scale);
void LIS_ReadAxesXYZ(I2C_HandleTypeDef *hi2c, Acceleration *acc);
uint8_t LIS_CTRL1_Read(I2C_HandleTypeDef *hi2c);
#endif /* LIS2DW12_H_ */
