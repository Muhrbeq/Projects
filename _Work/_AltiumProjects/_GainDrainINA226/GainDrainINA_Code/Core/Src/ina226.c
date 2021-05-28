/*
 * ina226.c
 *
 *  Created on: 27 Mar 2018
 *      Author: daniel.magnusson
 */

/*
 * This file is part of HiKoB Openlab.
 *
 * HiKoB Openlab is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, version 3.
 *
 * HiKoB Openlab is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with HiKoB Openlab. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2011,2012 HiKoB.
 */

/*
 * ina226.c
 *
 *  Created on: Jul 12, 2012
 *      Author: Clément Burin des Roziers <clement.burin-des-roziers.at.hikob.com>
 */

#include "main.h"
#include <math.h>
#include <stdio.h>
#include "ina226.h"
//#include "twi_master.h"
//#include "lowlevel.h"

ina226 ina_battery;
ina226 ina_cell;


static uint16_t INA226_ReadReg(ina226 *device, uint8_t regaddr)
{
	uint8_t pData[3] = {0};

	/* Select configuration register */
	pData[0] = regaddr;

	uint32_t timeOutCounter = 0;
	while(HAL_I2C_Master_Transmit(device->hi2c, device->i2c_address << 1, pData, 1, 1000) != HAL_OK)
	{
		if (timeOutCounter > 100)
		{
			return HAL_ERROR;
		}
		timeOutCounter++;
	}
	timeOutCounter = 0;
	/* Read register */
	while(HAL_I2C_Master_Receive(device->hi2c, device->i2c_address << 1 | 0x01, pData, 2, 1000) != HAL_OK)
	{
		if (timeOutCounter > 100)
		{
			return HAL_ERROR;
		}
		timeOutCounter++;
	}

	return (pData[0] << 8) + pData[1];
}

static uint8_t INA226_WriteReg(ina226 *device, uint8_t regaddr, uint16_t value)
{
    /* Prepare the 3 byte buffer */
    uint8_t buf[3];
    buf[0] = regaddr;
    buf[1] = value >> 8;
    buf[2] = value & 0xFF;

    uint32_t timeOutCounter = 0;
	/* Write register */
    while(HAL_I2C_Master_Transmit(device->hi2c, device->i2c_address << 1, buf, 3, 1000) != HAL_OK)
    {
    	if (timeOutCounter > 100) //todo 100 timeout too long?
		{
			return HAL_ERROR;
		}

		timeOutCounter++;
    }
    return HAL_OK;
//	twi_master_transfer(device->i2c_address << 1, buf, 3, TWI_ISSUE_STOP);
}

void INA226_InitDevice(ina226 *device, I2C_HandleTypeDef *hi2c, uint8_t address, int alertpin)
{
    /* Store the parameters */
	device->hi2c = hi2c;
	device->i2c_address = address;
	device->alert_pin = alertpin;

	/* Input no Pull-up */
    //nrf_gpio_cfg_input(device->alert_pin, NRF_GPIO_PIN_NOPULL);

    /* Reset the chip */
	INA226_WriteReg(device, INA226_REG_CONFIGURATION, INA226_CONFIGURATION__RST);
}

void INA226_Disable(ina226 *device)
{
    /* Clear the MODE bits of the configuration registerv */
    uint16_t reg;
    reg = INA226_ReadReg(device, INA226_REG_CONFIGURATION);
    reg &= ~INA226_CONFIGURATION__MODE_MASK;

    /* Write the configuration value */
    INA226_WriteReg(device, INA226_REG_CONFIGURATION, reg);
}

void INA226_Enable(ina226 *device)
{
    /* Set the MODE bits of the configuration registerv */
    uint16_t reg;
    reg = INA226_ReadReg(device, INA226_REG_CONFIGURATION);
    reg |= INA226_CONFIGURATION__MODE_MASK;

    /* Write the configuration value */
    INA226_WriteReg(device, INA226_REG_CONFIGURATION, reg);
}

void INA226_Calibrate(ina226 *device, float r_shunt, float max_current)
{
    /* Compute the current LSB as max_expected_current/2**15 */
    float current_lsb = max_current / (1 << 15); // Max current / 32768

    /* Compute calibration register as 0.00512 / (current_lsb * r_shunt) */
    float calib = 0.00512 / (current_lsb * r_shunt);

    /* Register is a 16bit unsigned integer, thus convert and round above */
    uint16_t calib_reg = (uint16_t) floorf(calib);

    /* Re-compute and store real current LSB */
    device->current_lsb = 0.00512 / (r_shunt * calib_reg);

    /* Write calibration */
    INA226_WriteReg(device, INA226_REG_CALIBRATION, calib_reg);
}

void INA226_Configure(ina226 *device, ina226_sampling_period_t period, ina226_averaging_factor_t average)
{
	// Configuration register
	//
	//   15   14  13  12    11     10     9       8       7       6      5      4      3      2     1     0
	//  _____ ___ ___ ___ ______ ______ ______ _______ _______ _______ ______ ______ ______ _____ _____ _____
	// |     |   |   |   |      |      |      |       |       |       |      |      |      |     |     |     |
	// | RST | - | - | - | AVG2 | AVG1 | AVG0 |VBUSCT2|VBUSCT1|VBUSCT0|VSHCT2|VSHCT1|VSHCT0|MODE3|MODE2|MODE1|
	// |_____|__ |___|___|______|______|______|_______|_______|_______|______|______|______|_____|_____|_____|

    /* Prepare register value; */
    uint16_t reg = 0;

    /* Bus Voltage Conversion Time = Sets the conversion time for the bus voltage measurement */
    reg = (period << INA226_CONFIGURATION__BUS_CONV_TIME_SHIFT) & INA226_CONFIGURATION__BUS_CONV_TIME_MASK;

    /* Shunt Voltage Conversion Time = Sets the conversion time for the shunt voltage measurement */
    reg |= (period << INA226_CONFIGURATION__SHUNT_CONV_TIME_SHIFT) & INA226_CONFIGURATION__SHUNT_CONV_TIME_MASK;

    /* Averaging Mode = Determines the number of samples that are collected and averaged */
    reg |= (average << INA226_CONFIGURATION__AVG_SHIFT) & INA226_CONFIGURATION__AVG_MASK;

    /* Operating Mode  = 0b0000000000000111 = Shunt and Bus, Continuous */
    reg |= INA226_CONFIGURATION__MODE_MASK;

    /* Write the configuration value */
    INA226_WriteReg(device, INA226_REG_CONFIGURATION, reg);
}

int INA226_ConversionReady(ina226 *device)
{
	/*
	Although the device can be read at any time, and the data from the last conversion remain available, the
	Conversion Ready flag bit (Mask/Enable Register, CVRF bit) is provided to help coordinate one-shot or triggered
	conversions. The Conversion Ready flag (CVRF) bit is set after all conversions, averaging, and multiplication
	operations are complete.
	The Conversion Ready flag (CVRF) bit clears under these conditions:
	• Writing to the Configuration Register (00h), except when configuring the MODE bits for power-down mode; or
	• Reading the Mask/Enable Register (06h)
	*/
    return (INA226_ReadReg(device, INA226_REG_MASK_ENABLE) & INA226_MASK_ENABLE__CVRF) != 0;
}

void INA226_ReadRaw(ina226 *device, float *voltage, float *current, float *power, float* shunt_voltage)
{
    uint16_t voltage_reg;
    int16_t current_reg;
    int16_t shunt_voltage_reg;
    int16_t power_reg;

    /* Read BUS voltage register */
    voltage_reg = INA226_ReadReg(device, INA226_REG_BUS_VOLTAGE);

    /* Read current register */
    current_reg = (int16_t) INA226_ReadReg(device, INA226_REG_CURRENT);

    /* Read POWER register */
    power_reg = (int16_t) INA226_ReadReg(device, INA226_REG_POWER);

    /* Read POWER register */
    shunt_voltage_reg = (int16_t) INA226_ReadReg(device, INA226_REG_SHUNT_VOLTAGE);

    /* Read the mask/enable register to clear it */
    (void) INA226_ReadReg(device, INA226_REG_MASK_ENABLE);

//	printf("ReadRAW:\n\r\tVoltage = %X\n\r\tCurrent = %X\n\r\tPower = %X\n\r\tShunt Voltage = %X\n\r",
//			voltage_reg,
//			current_reg,
//			power_reg,
//			shunt_voltage_reg);

    /* Check for the requested measures and compute their values */
    if (voltage)
    {
        /* Convert to Volts voltage_reg */
        //*voltage = (float) voltage_reg * 1.25e-3; //in volts
        *voltage = (float) voltage_reg * 1.25; 		//in mV
    }

    if (current)
    {
        /* Convert to Amperes
         * The value of the Current Register is calculated by multiplying the decimal value
         * in the Shunt Voltage Register with the decimal value of the Calibration Register
         */
        *current = (float) current_reg * device->current_lsb * 1000000; // in uA
    }

    if (power)
    {
        /* Convert to Watts
    	 * The Power Register LSB is internally programmed to equal 25 times the programmed value of the Current_LSB
    	 */
        *power = (float) power_reg * 25.0 * device->current_lsb; //in Watts
    }

    if (shunt_voltage)
    {
        /* Convert to Volts */
        //*shunt_voltage = (float) shunt_voltage_reg * 2.5e-6;  //in volts
    	//*shunt_voltage = (float) shunt_voltage_reg * 2.5e-3;  //in mV
        *shunt_voltage = (float) shunt_voltage_reg * 2.5; 		//in uV
    }
}

void INA226_Read(ina226 *device)
{
	/* Check that conversion is ready before reading the value */
	if(INA226_ConversionReady(device))
	{
//		printf("Device Conversion Done \n");
		INA226_ReadRaw(device, &device->voltage, &device->current, &device->power, &device->shunt_voltage);

	}
	else
	{
//		printf("Device Conversion NOT ready\n");
	}
}

void INA226_PrintData(ina226 *device)
{
//	printf("\r\nINA226 Device %X\n\r\tData:Read:\n\r\tVoltage = %f mV\n\r\tCurrent = %f mA\n\r\tPower = %f mW\n\r\tShunt Voltage = %f uV\n\r",
//				device->i2c_address,
//				device->voltage,
//				device->current * 1000.0,	//current in A, convert to mA
//				device->power * 1000.0, 	//power in W, convert to mW
//				device->shunt_voltage);
}

void INA226_ResetReadData(ina226 *res)
{
	res->current = 0;
	res->power = 0;
	res->shunt_voltage = 0;
	res->voltage = 0;
}

void INA226_Init(ina226 *device, I2C_HandleTypeDef *hi2c, uint8_t deviceAddr, int alertPin, float r_shunt, float max_current, ina226_sampling_period_t period, ina226_averaging_factor_t average)
{
	uint16_t die_id = 0;

	/* Set device information */
	INA226_InitDevice(device, hi2c, deviceAddr, alertPin);

	/* Verify that the device is connected by reading the die id 0x2260 (fixed value) */
	die_id = INA226_ReadReg(device, INA226_REG_DIE_ID);

	if(die_id == 0x2260)
	{
//		printf("Found INA226 (address 0x%X) Die ID = 0x%X", device->i2c_address, die_id);
	}
	else
	{
//		printf("Invalid INA226 (address 0x%X) Die ID = 0x%X! Expected 0x2260", device->i2c_address, die_id);
		return;
	}

	/* Set up the device */
	INA226_Calibrate(device, r_shunt, max_current);
	INA226_Configure(device, period, average);

	/* Clear variables */
	INA226_ResetReadData(device);
}




//void INA226_Test(void)
//{
//	INA226_ResetData(&ina_cell);
//
//	NRF_LOG_INFO("Testing INA226 for Cell\n");
//	if(INA226_ConversionReady(&ina_cell))
//	{
//		NRF_LOG_INFO("Reading..\n");
//		INA226_Read(&ina_cell);
//
//		NRF_LOG_INFO("Read:\n\r\tVoltage = %d\n\r\tCurrent = %d\n\r\tPower = %d\n\r\tShunt Voltage = %d\n\r",
//				ina_cell.voltage*100.0,
//				ina_cell.current*100.0,
//				ina_cell.power*100.0,
//				ina_cell.shunt_voltage*100.0);
//		NRF_LOG_INFO("Done\n");
//	}
//	else
//	{
//		NRF_LOG_INFO("Device Conversion NOT ready\n");
//	}
//
//	//Reset Values
//	INA226_ResetData(&ina_battery);
//
//	NRF_LOG_INFO("Testing INA226 for Battery\n");
//	if(INA226_ConversionReady(&ina_battery))
//	{
//		NRF_LOG_INFO("Reading..\n");
//		INA226_Read(&ina_battery);
//
//		NRF_LOG_INFO("Read:\n\r\tVoltage = %d\n\r\tCurrent = %d\n\r\tPower = %d\n\r\tShunt Voltage = %d\n\r",
//				ina_battery.voltage*100.0,
//				ina_battery.current*100.0,
//				ina_battery.power*100.0,
//				ina_battery.shunt_voltage*100.0);
//		NRF_LOG_INFO("Done\n");
//	}
//	else
//	{
//		NRF_LOG_INFO("Device Conversion NOT ready\n");
//	}
//}
//
//
//void INA226_AlertEnable(ina226 *device, handler_t handler, handler_arg_t arg)
//{
//    // Enable DATA READY alert source
//    INA226_WriteReg(device, INA226_REG_MASK_ENABLE, 1 << 10);
//
//    // Enable the alert line
//    exti_set_handler(ina.alert_line, handler, arg);
//    exti_enable_interrupt_line(ina.alert_line, EXTI_TRIGGER_FALLING);
//}
//
//void INA226_AlertDisable(ina226 *device)
//{
//    // Disable all alert sources
//    INA226_WriteReg(device, INA226_REG_MASK_ENABLE, 0);
//
//    // Disable the alert line
//    exti_disable_interrupt_line(ina.alert_line);
//    exti_set_handler(ina.alert_line, NULL, NULL);
//}
