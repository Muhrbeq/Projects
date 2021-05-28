/*
 * OPT3001.c
 *
 *  Created on: Oct 28, 2019
 *      Author: tomas.forsen
 */

#include "stm32l4xx_hal.h"
#include <math.h>
#include <stdbool.h>

#include "debugPrint.h"

uint8_t send_read_cmd(uint8_t);
uint8_t read_data(uint8_t*);
static float SensorOpt3001_convert(uint16_t);

extern I2C_HandleTypeDef hi2c1;

uint8_t send_read_cmd(uint8_t id_reg)
{
    // first set the register pointer to the register wanted to be read
    if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)0x44<<1, &id_reg, 1, 100) == HAL_OK)
    {
    	return HAL_OK;
    }
    else
    {
    	return HAL_ERROR;
    }
}

uint8_t read_data(uint8_t *receive_buffer)
{
	// receive the 2 x 8bit data into the receive buffer
	if(HAL_I2C_Master_Receive(&hi2c1, 0x44<<1, receive_buffer, 2, 100) == HAL_OK)
	{
		if (HAL_I2C_IsDeviceReady(&hi2c1, 0x44<<1, 1, 10) == HAL_OK)
		{
			return HAL_OK;
		}
		else
		{
			debug_printf("read_data I2C ready fail\n\r");
			return HAL_ERROR;
		}
	}
	else
	{
		debug_printf("read_data Receive fail\n\r");
		return HAL_ERROR;
	}
}

static float SensorOpt3001_convert(uint16_t iRawData)
{
  uint16_t iExponent, iMantissa;
  iMantissa = iRawData & 0x0FFF;                 // Extract Mantissa
  iExponent = (iRawData & 0xF000) >> 12;         // Extract Exponent
  return iMantissa * (0.01 * pow(2, iExponent)); // Calculate final LUX
}

uint8_t OPT_3001_init_for_single_shot(void)
{
	unsigned char buffer[3];

	//For	measurement time 800 ms, continuous mode, automatic gain
	//   	buffer[1]=0xCE;
  //		buffer[2]=0x10;

	//For	measurement time 100 ms, continuous mode, automatic gain
	//   	buffer[1]=0xC6;
	//		buffer[2]=0x10;

	//For	measurement time 100 ms, single shot mode, automatic gain
	//   	buffer[1]=0xC2;
	//		buffer[2]=0x10;

	//TODO: Utilize the "01 - Single shot" mode?
	buffer[0]=0x01; //buffer
  buffer[1]=0xC2; //MSB byte
  buffer[2]=0x10; //LSB byte

  //device address 0x44 in the datasheet is shifted 1-bit to the left.
  //3 bytes are transmitting to the slave: buffer[0], buffer[1] and buffer[2]
	if(HAL_I2C_Master_Transmit(&hi2c1,0x44<<1,buffer,3,100) == HAL_OK)
	{
		return HAL_OK;
	}
	else
	{
		return HAL_ERROR;
	}
}

void OPT_3001_shutdown(void)
{
	unsigned char buffer[3];
	buffer[0]=0x01; //buffer
	buffer[1]=0xCA; //MSB byte
	buffer[2]=0x10; //LSB byte

	//TODO: Error handling of below
	HAL_I2C_Master_Transmit(&hi2c1,0x44<<1,buffer,3,100); //config sensor
	//device address 0x44 in the datasheet is shifted 1-bit to the left.
	//3 bytes are transmitting to the slave: buffer[0], buffer[1] and buffer[2]
}

uint8_t OPT_3001_request_reading(void)
{
	uint8_t id_reg;

	//Send read command
	id_reg = 0x00;

	if(send_read_cmd(id_reg) == HAL_OK)
	{
		return HAL_OK;
	}
	else
	{
		debug_printf("request reading ERROR\n\r");
		return HAL_ERROR;
	}
}

uint8_t OPT_3001_reading_ready(void)
{
	uint16_t rawL;
	uint8_t id_reg;
	uint8_t buffer[2] = {0,0};

	//Send read-command
	id_reg 	= 0x01;

	if(send_read_cmd(id_reg) == HAL_OK)
	{
		if(read_data(buffer) == HAL_OK)
		{
			//Combine data buffer to unsigned int 16
			rawL 		= (((uint16_t)buffer[0]) << 8) | buffer[1];

			//Check if data is ready from sensor
			//TODO: Add #defines for these magic numbers
			if ((rawL & 0x80) == 128)
			{
				return HAL_OK;
			}
			else
			{
				return HAL_ERROR;
			}
		}
		else
		{
			debug_printf("read_data ERROR\n\r");
			return HAL_ERROR;
		}
	}
	else
	{
		debug_printf("send_read_command ERROR\n\r");
		return HAL_ERROR;
	}
}

uint8_t OPT_3001_get_reading(float* lux_ptr)
{
	uint8_t buffer[2] = {0,0};
	uint8_t id_reg;
	unsigned int response;

	//Read sensor data
	id_reg 		= 0x00;
	if(send_read_cmd(id_reg) == HAL_OK)
	{
		if(read_data(buffer) == HAL_OK)
		{
			response	= (((uint16_t)buffer[0]) << 8) | buffer[1];

			// Calculate LUX from sensor data
			*lux_ptr	= SensorOpt3001_convert(response);

			return HAL_OK;
		}
		else
		{
			debug_printf("OPT_3001_get_reading read_data error\n\r");
			return HAL_ERROR;
		}
	}
	else
	{
		debug_printf("OPT_3001_get_reading send_read_cmd error\n\r");
		return HAL_ERROR;
	}
}
