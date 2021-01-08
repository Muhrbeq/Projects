/*
 * AS726X.c
 *
 *  Created on: 14 Sep 2018
 *      Author: andreas.fredlund
 */

#include "_AS726x.h"
#include "_LowLevel.h"

#include "i2c.h"


uint8_t AS726X_begin(uint8_t gain, uint8_t measurementMode)
{

	if (AS726X_Get_HW_No() != SENSORTYPE_AS7262 && AS726X_Get_HW_No() != SENSORTYPE_AS7263)	//HW version for AS7262&AS7263
	{
		InfoLogAdd(ERRORLEVEL_WARNING, LOG_FUNC_SPECTRUM_SENS, LOG_SUB_INIT, LOG_TYPE_ID, LOG_STATUS_FAIL, 0, 0, 0, GetInfoStruct());
		return HAL_ERROR;
	}
	else
	{

		AS726X_setBulbCurrent(0b00); 		//Set to 12.5mA (minimum)

		AS726X_disableBulb(); 				//Turn off to avoid heating the sensor

		AS726X_disableIndicator();			//Turn off lights to save power

		AS726X_setIntegrationTime(AS726X_INTEGRATION_TIME);

		AS726X_setGain(gain); 						//Set gain

		AS726X_setMeasurementMode(measurementMode);  //One-shot reading of VBGYOR

		return HAL_OK;
	}
}

/*
 * Gets the hardware number of the device
 */
uint8_t AS726X_Get_HW_No(void)
{
	return AS726X_virtualReadRegister(AS726x_HW_VERSION);
}

/*
 * Sets the measurement mode. Either 0, 1, 2 or one shot according to data sheet
 * Read, mask/set, write
 */
void AS726X_setMeasurementMode(uint8_t mode)
{
	if (mode > 0b11)
	{
		mode = 0b11;
	}

	uint8_t value = AS726X_virtualReadRegister(AS726x_CONTROL_SETUP); //Read
	value &= 0b11110011; //Clear BANK bits
	value |= (mode << 2); //Set BANK bits with user's choice
	AS726X_virtualWriteRegister(AS726x_CONTROL_SETUP, value); //Write
}

/*
 * Sets the gain value. Gain 0: 1x (power-on default). Gain 1: 3.7x. Gain 2: 16x. Gain 3: 64x
 * Read, mask/set, write
 */
void AS726X_setGain(uint8_t gain)
{
	if (gain > 0b11)
	{
		gain = 0b11;
	}

	uint8_t value = AS726X_virtualReadRegister(AS726x_CONTROL_SETUP); //Read
	value &= 0b11001111; //Clear GAIN bits
	value |= (gain << 4); //Set GAIN bits with user's choice
	AS726X_virtualWriteRegister(AS726x_CONTROL_SETUP, value); //Write
}

/*
 *  Sets the integration value
 *	Give this function a byte from 0 to 255.
 *	Time will be 2.8ms * [integration value]
 */
void AS726X_setIntegrationTime(uint8_t integrationValue)
{
	AS726X_virtualWriteRegister(AS726x_INT_T, integrationValue); //Write
}

/*
 * Enables the interrupt pin.
 * Read, mask/set, write
 */
void AS726X_enableInterrupt(void)
{
	uint8_t value = AS726X_virtualReadRegister(AS726x_CONTROL_SETUP); //Read
	value |= 0b01000000; //Set INT bit
	AS726X_virtualWriteRegister(AS726x_CONTROL_SETUP, value); //Write
}

/*
 * Disables the interrupt pin.
 * Read, mask/set, write
 */
void AS726X_disableInterrupt(void)
{
	uint8_t value = AS726X_virtualReadRegister(AS726x_CONTROL_SETUP); //Read
	value &= 0b10111111; //Clear INT bit
	AS726X_virtualWriteRegister(AS726x_CONTROL_SETUP, value); //Write
}

uint8_t AS726X_virtualReadRegister(uint8_t virtualAddr)
{
	uint8_t status;
	uint32_t timeOutCounter = 0;

	//Do a prelim check of the read register

	status = AS726X_readRegister(AS72XX_SLAVE_STATUS_REG);

	if ((status & AS72XX_SLAVE_RX_VALID) != 0) //There is data to be read
	{
		uint8_t incoming = AS726X_readRegister(AS72XX_SLAVE_READ_REG); //Read the byte but do nothing with it
		UNUSED(incoming);
	}

	//Wait for WRITE flag to clear

	while (timeOutCounter < AS72XX_TIMEOUT_RX_END)
	{
		status = AS726X_readRegister(AS72XX_SLAVE_STATUS_REG);

		if ((status & AS72XX_SLAVE_TX_VALID) == 0)
		{
			break; // If TX bit is clear, it is ok to write
		}

		HAL_Delay(1);
		timeOutCounter++;
	}


	if (timeOutCounter >= AS72XX_TIMEOUT_RX_END)
	{
		InfoLogAdd(
				ERRORLEVEL_WARNING,
				LOG_FUNC_SPECTRUM_SENS,
				LOG_SUB_READ,
				LOG_TYPE_READ,
				LOG_STATUS_TIMEOUT,
				virtualAddr,
				1,
				0,
				GetTempInfoStruct());
	}

	// Send the virtual register address (bit 7 should be 0 to indicate we are reading a register).
	AS726X_writeRegister(AS72XX_SLAVE_WRITE_REG, virtualAddr);

	//Wait for READ flag to be set
	timeOutCounter = 0;

	while (timeOutCounter < AS72XX_TIMEOUT_RX_END)
	{
		status = AS726X_readRegister(AS72XX_SLAVE_STATUS_REG);
		if ((status & AS72XX_SLAVE_RX_VALID) != 0)
		{
			break; // Read data is ready.
		}

		//SleepForApproxMsSpectra(10);

		timeOutCounter++;
	}

	if (timeOutCounter >= AS72XX_TIMEOUT_RX_END)
	{
		InfoLogAdd(
				ERRORLEVEL_WARNING,
				LOG_FUNC_SPECTRUM_SENS,
				LOG_SUB_READ,
				LOG_TYPE_READ,
				LOG_STATUS_TIMEOUT,
				virtualAddr,
				2,
				0,
				GetTempInfoStruct());
	}


	uint8_t incoming = AS726X_readRegister(AS72XX_SLAVE_READ_REG);


	return (incoming);
}

uint8_t AS726X_readRegister(uint8_t addr)
{
	uint8_t data_t[2];

	data_t[0] = addr;

	// first set the register pointer to the register wanted to be read
	// note the & operator which gives us the address of the register_pointer variable
	HAL_I2C_Master_Transmit(&hi2c2, (uint8_t) AS726X_ADDR_Write, (uint8_t *) data_t, 1, 1);

	// receive the 2 x 8bit data into the receive buffer
	HAL_I2C_Master_Receive(&hi2c2, (uint8_t) AS726X_ADDR_Read, (uint8_t *) data_t, 1, 1);

	return data_t[0];
}

//Write to a virtual register in the AS726x
void AS726X_virtualWriteRegister(uint8_t virtualAddr, uint8_t dataToWrite)
{
	uint8_t status;
	uint32_t timeOutCounter = 0;

	//Wait for WRITE register to be empty
	while (timeOutCounter < AS72XX_TIMEOUT_TX_END)
	{
		status = AS726X_readRegister(AS72XX_SLAVE_STATUS_REG);
		if ((status & AS72XX_SLAVE_TX_VALID) == 0)
		{
			break; // No inbound TX pending at slave. Okay to write now.
		}
		HAL_Delay(POLLING_DELAY);
		timeOutCounter++;
	}

	if (timeOutCounter >= AS72XX_TIMEOUT_TX_END)
	{
		InfoLogAdd(ERRORLEVEL_WARNING, LOG_FUNC_SPECTRUM_SENS, LOG_SUB_WRITE, LOG_TYPE_WRITE, LOG_STATUS_TIMEOUT, virtualAddr, 1, 0, GetInfoStruct());
	}

	timeOutCounter = 0;

	// Send the virtual register address (setting bit 7 to indicate we are writing to a register).
	AS726X_writeRegister(AS72XX_SLAVE_WRITE_REG, (virtualAddr | 0x80));

	//Wait for WRITE register to be empty
	while (timeOutCounter < AS72XX_TIMEOUT_TX_END)
	{
		status = AS726X_readRegister(AS72XX_SLAVE_STATUS_REG);
		if ((status & AS72XX_SLAVE_TX_VALID) == 0)
		{
			break; // No inbound TX pending at slave. Okay to write now.
		}
		HAL_Delay(POLLING_DELAY);
		timeOutCounter++;
	}

	if (timeOutCounter >= AS72XX_TIMEOUT_TX_END)
	{
		InfoLogAdd(ERRORLEVEL_WARNING, LOG_FUNC_SPECTRUM_SENS, LOG_SUB_WRITE, LOG_TYPE_WRITE, LOG_STATUS_TIMEOUT, 0, 0, 0, GetInfoStruct());
	}

	// Send the data to complete the operation.
	AS726X_writeRegister(AS72XX_SLAVE_WRITE_REG, dataToWrite);
}

//Write a value to a spot in the AS726x
void AS726X_writeRegister(uint8_t addr, uint8_t val)
{
	uint8_t data_t[3];
	data_t[0] = addr;
	data_t[1] = val;
	uint32_t timeOutCounter = 0;

	while (HAL_I2C_Master_Transmit(&hi2c2, AS726X_ADDR_Write, (uint8_t *) data_t, 2, 1) != HAL_OK)
	{

		if (timeOutCounter > AS72XX_TIMEOUT_TX_END)
		{
			InfoLogAdd(ERRORLEVEL_WARNING, LOG_FUNC_SPECTRUM_SENS, LOG_SUB_WRITE, LOG_TYPE_WRITE, LOG_STATUS_TIMEOUT, val, addr, 0, GetInfoStruct());
			break;
		}
		timeOutCounter++;
	}
}

void AS726X_takeMeasurements(uint8_t gain, Spectrum *sp)
{



	if (AS726X_begin(gain, AS726X_MODE_ONE_SHOT) != HAL_OK) //Gain and One-shot mode
	{
		InfoLogAdd(
				ERRORLEVEL_WARNING,
				LOG_FUNC_SPECTRUM_SENS,
				LOG_SUB_MEASURE,
				LOG_TYPE_INIT,
				LOG_STATUS_FAIL,
				0,
				0,
				0,
				GetTempInfoStruct());

	}

	else
	{


		AS726X_clearDataAvailable(); //Clear DATA_RDY flag when using Mode 3
//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		//Wait for data to be ready
		while (AS726X_dataAvailable() == 0)  //todo can get stuck here?
		{
//			HAL_Delay(1);
		}

//		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

		//Readings can now be accessed via getViolet(), getBlue(), etc
		AS726X_getSpectrum(sp);
		//AS726X_getSpectrum_norm(gain);   //normalized
	}
}

void AS726X_takeMeasurementsRaw(uint8_t gain, Spectrum *sp)
{
	if (AS726X_begin(gain, AS726X_MODE_ONE_SHOT) != HAL_OK) //Gain and One-shot mode
	{
		InfoLogAdd(ERRORLEVEL_WARNING, LOG_FUNC_SPECTRUM_SENS, LOG_SUB_MEASURE, LOG_TYPE_READ, LOG_STATUS_FAIL, 0, 0, 0, GetInfoStruct());
	}

	else
	{
		AS726X_clearDataAvailable(); //Clear DATA_RDY flag when using Mode 3

		//Wait for data to be ready
		while (AS726X_dataAvailable() == 0)
		{
			HAL_Delay(POLLING_DELAY);
		}

		//Readings can now be accessed via getViolet(), getBlue(), etc
		AS726X_getSpectrumRaw(sp);
		//AS726X_getSpectrumRaw_norm(gain);   //normalized
	}
}

void AS726X_getSpectrum(Spectrum *sp)
{
	sp->violet = (uint16_t) AS726X_getCalibratedViolet();
	sp->blue = (uint16_t) AS726X_getCalibratedBlue();
	sp->green = (uint16_t) AS726X_getCalibratedGreen();
	sp->yellow = (uint16_t) AS726X_getCalibratedYellow();
	sp->orange = (uint16_t) AS726X_getCalibratedOrange();
	sp->red = (uint16_t) AS726X_getCalibratedRed();
}

void AS726X_getSpectrum_norm(uint8_t gain, Spectrum *sp)
{
	sp->violet = AS726X_get_Normalized_Value((uint16_t) AS726X_getCalibratedViolet(), gain);
	sp->blue = AS726X_get_Normalized_Value((uint16_t) AS726X_getCalibratedBlue(), gain);
	sp->green = AS726X_get_Normalized_Value((uint16_t) AS726X_getCalibratedGreen(), gain);
	sp->yellow = AS726X_get_Normalized_Value((uint16_t) AS726X_getCalibratedYellow(), gain);
	sp->orange = AS726X_get_Normalized_Value((uint16_t) AS726X_getCalibratedOrange(), gain);
	sp->red = AS726X_get_Normalized_Value((uint16_t) AS726X_getCalibratedRed(), gain);
}

void AS726X_getSpectrumRaw(Spectrum *sp)
{
	sp->violet = AS726X_getViolet();
	sp->blue = AS726X_getBlue();
	sp->green = AS726X_getGreen();
	sp->yellow = AS726X_getYellow();
	sp->orange = AS726X_getOrange();
	sp->red = AS726X_getRed();
}

void AS726X_getSpectrumRaw_norm(uint8_t gain, Spectrum *sp)
{
	sp->violet = AS726X_get_Normalized_Value(AS726X_getViolet(), gain);
	sp->blue = AS726X_get_Normalized_Value(AS726X_getBlue(), gain);
	sp->green = AS726X_get_Normalized_Value(AS726X_getGreen(), gain);
	sp->yellow = AS726X_get_Normalized_Value(AS726X_getYellow(), gain);
	sp->orange = AS726X_get_Normalized_Value(AS726X_getOrange(), gain);
	sp->red = AS726X_get_Normalized_Value(AS726X_getRed(), gain);
}

//Checks to see if DRDY flag is set in the control setup register
uint8_t AS726X_dataAvailable(void)
{
	uint8_t value = AS726X_virtualReadRegister(AS726x_CONTROL_SETUP);
	return (value & (1 << 1)); //Bit 1 is DATA_RDY
}

//Clears the DRDY flag
//Normally this should clear when data registers are read
uint8_t AS726X_clearDataAvailable(void)
{
	uint8_t value = AS726X_virtualReadRegister(AS726x_CONTROL_SETUP);
	value &= ~(1 << 1); //Set the DATA_RDY bit
	AS726X_virtualWriteRegister(AS726x_CONTROL_SETUP, value);

	return 1;
}

//Get the various color readings
uint16_t AS726X_getViolet(void)
{
	return (AS726X_getChannel(AS7262_V));
}
uint16_t AS726X_getBlue(void)
{
	return (AS726X_getChannel(AS7262_B));
}
uint16_t AS726X_getGreen(void)
{
	return (AS726X_getChannel(AS7262_G));
}
uint16_t AS726X_getYellow(void)
{
	return (AS726X_getChannel(AS7262_Y));
}
uint16_t AS726X_getOrange(void)
{
	return (AS726X_getChannel(AS7262_O));
}
uint16_t AS726X_getRed(void)
{
	return (AS726X_getChannel(AS7262_R));
}

//A the 16-bit value stored in a given channel registerReturns
uint16_t AS726X_getChannel(uint8_t channelRegister)
{
	int colorData = AS726X_virtualReadRegister(channelRegister) << 8; //High byte
	colorData |= AS726X_virtualReadRegister(channelRegister + 1); //Low byte
	return (colorData);
}

//Returns the various calibration data
float AS726X_getCalibratedViolet(void)
{
	return (AS726X_getCalibratedValue(AS7262_V_CAL));
}
float AS726X_getCalibratedBlue(void)
{
	return (AS726X_getCalibratedValue(AS7262_B_CAL));
}
float AS726X_getCalibratedGreen(void)
{
	return (AS726X_getCalibratedValue(AS7262_G_CAL));
}
float AS726X_getCalibratedYellow(void)
{
	return (AS726X_getCalibratedValue(AS7262_Y_CAL));
}
float AS726X_getCalibratedOrange(void)
{
	return (AS726X_getCalibratedValue(AS7262_O_CAL));
}
float AS726X_getCalibratedRed(void)
{
	return (AS726X_getCalibratedValue(AS7262_R_CAL));
}

//Given an address, read four bytes and return the floating point calibrated value
float AS726X_getCalibratedValue(uint8_t calAddress)
{

	uint8_t b0, b1, b2, b3;
	b0 = AS726X_virtualReadRegister(calAddress + 0);
	b1 = AS726X_virtualReadRegister(calAddress + 1);
	b2 = AS726X_virtualReadRegister(calAddress + 2);
	b3 = AS726X_virtualReadRegister(calAddress + 3);

	//Channel calibrated values are stored big-endian
	uint32_t calBytes = 0;
	calBytes |= ((uint32_t) b0 << (8 * 3));
	calBytes |= ((uint32_t) b1 << (8 * 2));
	calBytes |= ((uint32_t) b2 << (8 * 1));
	calBytes |= ((uint32_t) b3 << (8 * 0));

	return (AS726X_convertBytesToFloat(calBytes));
}

//Given 4 bytes returns the floating point value
float AS726X_convertBytesToFloat(uint32_t myLong)
{
	float myFloat = 0;
	memcpy(&myFloat, &myLong, 4); //Copy bytes into a float
	return (myFloat);
}

/*
 * Enable the onboard indicator LED
 * Read, mask/set, write
 *
 */
void AS726X_enableIndicator(void)
{
	uint8_t value = AS726X_virtualReadRegister(AS726x_LED_CONTROL);
	value |= (1 << 0); //Set the bit
	AS726X_virtualWriteRegister(AS726x_LED_CONTROL, value);
}

/*
 * Disable the onboard indicator LED
 * Read, mask/set, write
 */
void AS726X_disableIndicator(void)
{
	uint8_t value = AS726X_virtualReadRegister(AS726x_LED_CONTROL);
	value &= ~(1 << 0); //Clear the bit
	AS726X_virtualWriteRegister(AS726x_LED_CONTROL, value);
}

/*
 * Set the current limit of onboard LED. Default is max 8mA = 0b11.
 * Read, mask/set, write
 */
//

void AS726X_setIndicatorCurrent(uint8_t current)
{
	if (current > 0b11)
	{
		current = 0b11;
	}
	uint8_t value = AS726X_virtualReadRegister(AS726x_LED_CONTROL); //Read
	value &= 0b11111001; //Clear ICL_IND bits
	value |= (current << 1); //Set ICL_IND bits with user's choice
	AS726X_virtualWriteRegister(AS726x_LED_CONTROL, value); //Write
}

/*
 * Enable the onboard 5700k or external incandescent bulb
 * Read, mask/set, write
 */
void AS726X_enableBulb(void)
{
	uint8_t value = AS726X_virtualReadRegister(AS726x_LED_CONTROL);
	value |= (1 << 3); //Set the bit
	AS726X_virtualWriteRegister(AS726x_LED_CONTROL, value);
}

/*
 * Disable the onboard 5700k or external incandescent bulb
 * Read, mask/set, write
 */
void AS726X_disableBulb()
{
	uint8_t value = AS726X_virtualReadRegister(AS726x_LED_CONTROL);
	value &= ~(1 << 3); //Clear the bit
	AS726X_virtualWriteRegister(AS726x_LED_CONTROL, value);
}

/*
 * Set the current limit of bulb/LED. Current 0: 12.5mA, Current 1: 25mA, Current 2: 50mA, Current 3: 100mA
 * Read, mask/set, write
 */
void AS726X_setBulbCurrent(uint8_t current)
{
	if (current > 0b11)
	{
		current = 0b11; //Limit to two bits
	}
	uint8_t value = AS726X_virtualReadRegister(AS726x_LED_CONTROL); //Read
	value &= 0b11001111; //Clear ICL_DRV bits
	value |= (current << 4); //Set ICL_DRV bits with user's choice
	AS726X_virtualWriteRegister(AS726x_LED_CONTROL, value); //Write
}

/*
 * Returns the temperature in C. Pretty inaccurate: +/-8.5C
 */
uint8_t AS726X_getTemperature(void)
{
	return (AS726X_virtualReadRegister(AS726x_DEVICE_TEMP));
}

/*
 * Convert to F if needed
 */
float AS726X_getTemperatureF(void)
{
	float temperatureF = AS726X_getTemperature();
	temperatureF = temperatureF * 1.8 + 32.0;
	return (temperatureF);
}

/*
 * Does a soft reset
 * Give sensor at least 1000ms to reset
 * Read, mask/set, write
 */
void AS726X_softReset(void)
{
	uint8_t value = AS726X_virtualReadRegister(AS726x_CONTROL_SETUP); //Read
	value |= (1 << 7); //Set RST bit
	AS726X_virtualWriteRegister(AS726x_CONTROL_SETUP, value); //Write
}


/*
 *  Normalizes the value in times of integration time and gain so we always get a legit value.
 *  If we have a longer integration time on one device we will get a higher value from the sensor.
 *  If we have a higher gain we will get a higher value.
 * 	So a normalization is needed to be able to compare the intensity of different devices.
 * 	Default: integration time = 255, Gain = 1
 * 	TODO check if it works with 16 bit
 */
uint16_t AS726X_get_Normalized_Value(uint32_t intensity, uint8_t gain)
{

	if (intensity >= 0xFFFFFFFF)
	{
		InfoLogAdd(ERRORLEVEL_INFO, LOG_FUNC_SPECTRUM_SENS, LOG_SUB_MEASURE, LOG_TYPE_NONE, LOG_STATUS_MAX, AS726X_INTEGRATION_TIME, gain, 0, GetInfoStruct());
	}

	float gained_intensity = (float) intensity;

	if (gain == AS7262_GAIN_3_7)
	{
		gained_intensity = gained_intensity / 3.7;
	}
	else if (gain == AS7262_GAIN_16)
	{
		gained_intensity = gained_intensity / 16.0;
	}

	else if (gain == AS7262_GAIN_64)
	{
		gained_intensity = gained_intensity / 64.0;
	}

	gained_intensity = gained_intensity * (float) AS726X_MAX_INTEGRATION_TIME / (float) AS726X_INTEGRATION_TIME;

	return (uint16_t) gained_intensity;
}



