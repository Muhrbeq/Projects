/*
 * Uart_Handling.h
 *
 *  Created on: 26 aug. 2020
 *      Author: Rasmus.Muhrbeck
 */

#ifndef UART_HANDLING_H_
#define UART_HANDLING_H_

#include "main.h"
#include "usart.h"

void USART_Callback(void);
void PrintFromBLE(void);
void CheckUart(void);
void GetStringBLE(uint8_t *get);
void EnableBLE(void);
void DisableBLE(void);

#endif /* UART_HANDLING_H_ */
