/*
 * L3GD20H.h
 *
 *  Created on: 2 mars 2020
 *      Author: Rasmus.Muhrbeck
 */

#ifndef L3GD20H_H_
#define L3GD20H_H_


#include "main.h"
#include "i2cGeneric.h"
#include "__ExegerGeneric.h"


#define L3_ID					0xD7

#define L3_READ_ADDR			0xD7
#define L3_WRITE_ADDR			0xD6

#define L3_WHO_AM_I				0x0F

#define L3_CTRL_REG1			0x20
#define L3_CTRL_REG2			0x21
#define L3_CTRL_REG3			0x22
#define L3_CTRL_REG4			0x23
#define L3_CTRL_REG5			0x24

#define L3_REFERENCE			0x25

#define L3_OUT_TEMP				0x26

#define L3_STATUS				0x27

#define L3_OUT_X_L				0x28
#define L3_OUT_X_H				0x29

#define L3_OUT_Y_L				0x2A
#define L3_OUT_Y_H				0x2B

#define L3_OUT_Z_L				0x2C
#define L3_OUT_Z_H				0x2D

#define L3_FIFO_CTRL			0x2E
#define L3_FIFO_SRC				0x2F

#define L3_IG_CFG				0x30
#define L3_IG_SRC				0x31
#define L3_IG_THS_XH			0x32
#define L3_IG_THS_XL			0x33
#define L3_IG_THS_YH			0x34
#define L3_IG_THS_YL			0x35
#define L3_IG_THS_ZH			0x36
#define L3_IG_THS_ZL			0x37
#define L3_IG_DURATION			0x38

#define L3_LOW_ODR				0x39

#define L3_AXIS_ENABLE			0x1
#define L3_AXIS_DISABLE			0x0

#define L3_X_MASK				0x1

#define L3_Y_MASK				0x0

#define L3_Z_MASK				0x2

#define L3_ALL_AXES_ENABLED		0x7

#define L3_POWER_MODE_MASK		0x3
#define L3_POWER_MODE_NORMAL	0x1
#define L3_POWER_MODE_POWERDOWN	0x0

#define L3_ODR_MASK				0x4

#define L3_LODR_12HZ_5			0x0
#define L3_LODR_25HZ			0x4
#define L3_LODR_50HZ			0x8

#define L3_ODR_100HZ			0x0
#define L3_ODR_200HZ			0x4
#define L3_ODR_400HZ			0x8
#define L3_ODR_800HZ			0xC



#define L3_FULLSCALE_MASK		0x4
#define L3_SCALE_245_DPS		0x0
#define L3_SCALE_500_DPS		0x1
#define L3_SCALE_2000_DPS		0x2


#define L3_FIFO_ENABLE_MASK		0x6
#define L3_FIFO_ENABLE			0x1

#define L3_FIFO_MASK				0x5

#define L3_FIFO_BYPASS				0x0
#define L3_FIFO_MODE				0x1
#define L3_FIFO_STREAM_MODE			0x2
#define L3_FIFO_STREAM_TO_FIFO		0x3
#define L3_FIFO_BYP_TO_STREAM		0x4
#define L3_FIFO_DYNAMIC_STREAM		0x6
#define L3_FIFO_BYPASS_TO_FIFO		0x7


#define L3_DRDY_MASK				0x5
#define L3_DRDY_HIGH				0x0
#define L3_DRDY_LOW					0x1

#define L3_LOW_ODR_MASK				0x0
#define L3_LOW_ODR_ENABLE			0x1
#define L3_LOW_ODR_DISABLE			0x0

#define L3_245_DPS_SENSITIVITY		0.00875f
#define L3_500_DPS_SENSITIVITY		0.01750
#define L3_2000_DPS_SENSITIVITY		0.07f

typedef struct GyroscopeData
{
	int16_t x;
	int16_t y;
	int16_t z;
}GyroScopeData;

GyroScopeData gyroStruct_i2c2;
GyroScopeData gyroStruct_i2c3;
GyroScopeData gyroStruct_i2c4;

uint8_t L3_ReadID(I2C_HandleTypeDef *hi2c);
uint8_t L3_EnableAllAxes(I2C_HandleTypeDef *hi2c, uint8_t enableAxis);
uint8_t L3_Start(I2C_HandleTypeDef *hi2c, uint8_t lodrEn, uint8_t odr, uint8_t mode, uint8_t scale);
void L3_ReadAxesXYZ(I2C_HandleTypeDef *hi2c, GyroScopeData *gyro);
GyroScopeData *L3_GetGyroStruct(I2C_HandleTypeDef *hi2c);
void L3_initStruct(GyroScopeData *gsd);

#endif /* L3GD20H_H_ */
