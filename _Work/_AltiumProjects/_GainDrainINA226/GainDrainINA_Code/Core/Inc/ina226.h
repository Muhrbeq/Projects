/*
 * ina226.h
 *
 *  Created on: 27 Mar 2018
 *      Author: daniel.magnusson
 */

#ifndef INCLUDE_INA226_H_
#define INCLUDE_INA226_H_


#ifndef INA226__H_
#define INA226__H_

#include <stdint.h>

/* Resistance value on the ina226 device */
#define INA226_RSENS_BATT_Ohm           0.1
#define INA226_RSENS_CELL_Ohm           0.025



/* Whats the maximum current we can expect on the device (in Apms) */
#define INA226_MAX_CURR_BATT            0.8
#define INA226_MAX_CURR_CELL            2

typedef struct
{
    uint8_t i2c_address; 	//Device address
    int alert_pin; 			//Pin number for alert line
    float current_lsb;
    I2C_HandleTypeDef *hi2c;

    /* Data */
	float voltage;
	float current;
	float power;
	float shunt_voltage;

} ina226;

/**
 * Possible sampling periods.
 */
typedef enum
{
    INA226_PERIOD_140us 	= 0,
    INA226_PERIOD_204us 	= 1,
    INA226_PERIOD_332us 	= 2,
    INA226_PERIOD_588us 	= 3,
    INA226_PERIOD_1100us 	= 4,
    INA226_PERIOD_2116us 	= 5,
    INA226_PERIOD_4156us 	= 6,
    INA226_PERIOD_8244us 	= 7,
} ina226_sampling_period_t;

/** Possible averaging values */
typedef enum
{
    INA226_AVERAGE_1 	= 0,
    INA226_AVERAGE_4 	= 1,
    INA226_AVERAGE_16 	= 2,
    INA226_AVERAGE_64 	= 3,
    INA226_AVERAGE_128 	= 4,
    INA226_AVERAGE_256 	= 5,
    INA226_AVERAGE_512 	= 6,
    INA226_AVERAGE_1024 = 7,
} ina226_averaging_factor_t;

enum
{
    INA226_REG_CONFIGURATION = 0x00,
    INA226_REG_SHUNT_VOLTAGE = 0x01,
    INA226_REG_BUS_VOLTAGE 	 = 0x02,
    INA226_REG_POWER 		 = 0x03,
    INA226_REG_CURRENT 		 = 0x04,
    INA226_REG_CALIBRATION 	 = 0x05,
    INA226_REG_MASK_ENABLE 	 = 0x06,
    INA226_REG_ALERT_LIMIT 	 = 0x07,
    INA226_REG_DIE_ID 		 = 0xFF,
};

enum
{
    INA226_CONFIGURATION__RST = 0x8000,

    INA226_CONFIGURATION__AVG_MASK = 0x0E00,
    INA226_CONFIGURATION__AVG_SHIFT = 9,

    INA226_CONFIGURATION__BUS_CONV_TIME_MASK = 0x01C0,
    INA226_CONFIGURATION__BUS_CONV_TIME_SHIFT = 6,

    INA226_CONFIGURATION__SHUNT_CONV_TIME_MASK = 0x0038,
    INA226_CONFIGURATION__SHUNT_CONV_TIME_SHIFT = 3,

    INA226_CONFIGURATION__MODE_MASK = 0x0007,
    INA226_CONFIGURATION__MODE_SHIFT = 0,
};

enum
{
    INA226_MASK_ENABLE__CVRF = 0x0008,
};

#define INA226_CELL_ADDR       			0x40
#define INA226_BATT_ADDR       			0x40
#define INA226_CELL_ALERT_PIN  			7
#define INA226_BATT_ALERT_PIN  			6

/**
 * Initialize the INA226, specifying its I2C bus and I2C address
 *
 * \param i2c the I2C bus to use
 * \param address the address on the bus
 * \param alert_exti the exti line corresponding to ALERT pin
 */
void INA226_InitDevice(ina226 *device, I2C_HandleTypeDef *hi2c, uint8_t address, int alertpin);

/**
 * Calibrate the INA226.
 *
 * Calibrate the INA226 based on the shunt resistor and the max expected current.
 * This will compute the optimum settings and write them to the chip registers.
 *
 * \param r_shunt the shunt resistor value
 * \param max_current the expected maximum current to flow through the resistor
 */
void INA226_Calibrate(ina226 *device, float r_shunt, float max_current);


/**
 * Configure the INA226 for sampling.
 *
 * This selects the sampling rate and average factor of the chip.
 *
 * \param period the sampling period
 * \param average the averaging factor
 */
void INA226_Configure(ina226 *device, ina226_sampling_period_t period, ina226_averaging_factor_t average);

/**
 * Disable the INA226.
 *
 * This places the INA226 in power down
 */
void INA226_Disable(ina226 *device);

/**
 * @brief Enable the INA226
 *
 */
void INA226_Enable(ina226 *device);


///**
// * Enable the ALERT pin to generate interrupts on each data ready
// *
// * \param handler the handler to be called on data ready
// * \param arg an optional argument to the handler
// */
//void ina226_alert_enable(handler_t handler, handler_arg_t arg);
///**
// * Disable the ALERT pin to generate interrupts on each data ready
// */
//void ina226_alert_disable();

/**
 * Read the sampled values.
 *
 * This reads the values from the INA226 registers and convert them to SI
 * units according to the calibration values.
 *
 * The availability of a new set of values may be checked with \ref
 * ina226_conversion_ready.
 *
 * \param voltage a pointer to store the bus voltage, in V, or NULL if not requested
 * \param current a pointer to store the load current, in A, or NULL if not requested
 * \param power a pointer to store the load power, in W, or NULL if not requested
 * \param shunt_voltage a pointer to store the shunt voltage, in V, or NULL if not requested
 */
void INA226_ReadRaw(ina226 *device, float *voltage, float *current, float *power, float* shunt_voltage);

/**
 * Check if a new set of sample is available.
 *
 * This reads a register of the INA226 to check if a new set of samples is
 * available for reading through \ref ina226_read.
 *
 * \return 1 if a new set is available for reading, 0 if not.
 */
int INA226_ConversionReady(ina226 *device);

void INA226_ResetReadData(ina226 *res);

void INA226_Read(ina226 *device);

void INA226_Init(ina226 *device, I2C_HandleTypeDef *hi2c, uint8_t deviceAddr, int alertPin, float r_shunt, float max_current, ina226_sampling_period_t period, ina226_averaging_factor_t average);

void INA226_PrintData(ina226 *device);


#endif /* INA226__H_ */



#endif /* INCLUDE_INA226_H_ */
