/*
 * Tmp112.h
 *
 *  Created on: 22 Nov 2018
 *      Author: andreas.fredlund
 */

#ifndef TMP112_H_
#define TMP112_H_

/*
 * Defines for TMP112
 */
#define TMP112_ADDRESS   			0x48    //todo 7-bit unshifted default I2C Address
#define TMP112_ADDR_WRITE  			0x90
#define TMP112_ADDR_READ   			0x91

/** Internal register within TMP112 */
#define TMP112_TEMP_REG       		0x00
#define TMP112_CONF_REG        		0x01
#define TMP112_TLOW_REG       		0x02
#define TMP112_THIGH_REG      		0x03

/* Data to return if its an invalid temperature */
#define TMP112_INVALID_TEMP 		0x0000				//(0x9C0) -100C
#define TMP112_TIMEOUT_TX_END		50 			// todo Counts in transmit (write) loop
#define TMP112_TIMEOUT_RX_END		50			// Counts in receive loop
#define TMP112_MAX_TEMP				255			// 8 bit decimal value

uint8_t TMP112_writeRegister(unsigned char* data, uint8_t len, uint8_t throw);
uint16_t TMP112_readRegister(unsigned char* data, uint8_t len, uint8_t throw);
uint8_t TMP112_GetTemperature_raw(uint16_t *get);
uint8_t TMP112_StartOneShot(void);
uint8_t TMP112_WaitForOneShot(void);

uint16_t TMP112_ReadTemp_raw(void);
void TMP112_Init(void);
void TMP112_ShutDown(void);
uint8_t TMP112_ReadTemp(void);


#endif /* TMP112_H_ */
