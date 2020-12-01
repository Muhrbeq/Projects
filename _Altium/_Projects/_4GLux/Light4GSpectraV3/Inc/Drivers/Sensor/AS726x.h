/*
 * AS726c.h
 *
 *  Created on: 14 Sep 2018
 *      Author: andreas.fredlund
 */

#ifndef AS726X_H_
#define AS726X_H_

#include "Global.h"


/*
 * Defines for AS726X
 *  Default values:
 * integration time: 0xff = 255
 * gain = 0 -> gain = 1x
 * bank mode = 10 = 2
 */
#define AS726X_MAX_INTEGRATION_TIME 255
#define AS726X_INTEGRATION_TIME     210
#define AS726X_MODE_0				0
#define AS726X_MODE_1				1
#define AS726X_MODE_2				2
#define AS726X_MODE_ONE_SHOT		3
#define AS7262_GAIN_1				0
#define AS7262_GAIN_3_7				1
#define AS7262_GAIN_16				2
#define AS7262_GAIN_64				3

#define AS726X_ADDR_Write 			0x92	//0x49, 7-bit unshifted default I2C Address
#define AS726X_ADDR_Read 			0x93

#define SENSORTYPE_AS7262 			0x3E
#define SENSORTYPE_AS7263 			0x3F

//Register addresses
#define AS726x_DEVICE_TYPE 			0x00
#define AS726x_HW_VERSION 			0x01
#define AS726x_CONTROL_SETUP 		0x04
#define AS726x_INT_T 				0x05
#define AS726x_DEVICE_TEMP 			0x06
#define AS726x_LED_CONTROL 			0x07

#define AS72XX_SLAVE_STATUS_REG 	0x00
#define AS72XX_SLAVE_WRITE_REG 		0x01
#define AS72XX_SLAVE_READ_REG 		0x02

//AS7262 Registers
#define AS7262_V 					0x08
#define AS7262_B 					0x0A
#define AS7262_G 					0x0C
#define AS7262_Y 					0x0E
#define AS7262_O 					0x10
#define AS7262_R 					0x12
#define AS7262_V_CAL 				0x14
#define AS7262_B_CAL 				0x18
#define AS7262_G_CAL 				0x1C
#define AS7262_Y_CAL 				0x20
#define AS7262_O_CAL 				0x24
#define AS7262_R_CAL 				0x28

#define AS72XX_SLAVE_TX_VALID 		0x02
#define AS72XX_SLAVE_RX_VALID 		0x01

#define POLLING_DELAY 				5 	// Amount of ms to wait between checking for virtual register changes
#define AS72XX_TIMEOUT_TX_END		100 // todo 100*POLLING_DELAY
#define AS72XX_TIMEOUT_RX_END		100 // 100*POLLING_DELAY

// my functions
uint8_t AS726X_Get_HW_No(void);
void AS726X_takeMeasurements(uint8_t gain, Spectrum *sp);
void AS726X_takeMeasurementsRaw(uint8_t gain, Spectrum *sp);
void AS726X_getSpectrum(Spectrum *sp);
void AS726X_getSpectrum_norm(uint8_t gain, Spectrum *sp);
void AS726X_getSpectrumRaw(Spectrum *sp);
void AS726X_getSpectrumRaw_norm(uint8_t gain, Spectrum *sp);
uint16_t AS726X_get_Normalized_Value(uint32_t intensity, uint8_t gain); //todo

uint8_t AS726X_begin(uint8_t gain, uint8_t measurementMode);
uint8_t AS726X_virtualReadRegister(uint8_t virtualAddr);
uint8_t AS726X_readRegister(uint8_t addr);
uint8_t AS726X_getTemperature(void);
float AS726X_getTemperatureF(void);
void AS726X_setMeasurementMode(uint8_t mode);
void AS726X_setGain(uint8_t gain);
void AS726X_setIntegrationTime(uint8_t integrationValue);
void AS726X_enableInterrupt(void);
uint8_t AS726X_dataAvailable(void);
void AS726X_enableIndicator(void);
void AS726X_disableIndicator(void);
void AS726X_setIndicatorCurrent(uint8_t current);
void AS726X_enableBulb(void);
void AS726X_disableBulb(void);
void AS726X_setBulbCurrent(uint8_t current);
void AS726X_softReset(void);
void AS726X_setGain(uint8_t gain);
void AS726X_setIntegrationTime(uint8_t integrationValue);
void AS726X_enableInterrupt(void);
void AS726X_disableInterrupt(void);

//Get the various color readings
uint16_t AS726X_getViolet(void);
uint16_t AS726X_getBlue(void);
uint16_t AS726X_getGreen(void);
uint16_t AS726X_getYellow(void);
uint16_t AS726X_getOrange(void);
uint16_t AS726X_getRed(void);

//Returns the various calibration data
float AS726X_getCalibratedViolet(void);
float AS726X_getCalibratedBlue(void);
float AS726X_getCalibratedGreen(void);
float AS726X_getCalibratedYellow(void);
float AS726X_getCalibratedOrange(void);
float AS726X_getCalibratedRed(void);

uint16_t AS726X_getChannel(uint8_t channelRegister);
float AS726X_getCalibratedValue(uint8_t calAddress);
float AS726X_convertBytesToFloat(uint32_t myLong);
uint8_t AS726X_clearDataAvailable(void);
uint8_t AS726X_virtualReadRegister(uint8_t virtualAddr);
void AS726X_virtualWriteRegister(uint8_t virtualAddr, uint8_t dataToWrite);
void AS726X_writeRegister(uint8_t addr, uint8_t val);
uint8_t AS726X_readRegister(uint8_t addr);

#endif /* AS726X_H_ */
