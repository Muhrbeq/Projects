/*
 * Global.h
 *
 *  Created on: 26 aug. 2020
 *      Author: Rasmus.Muhrbeck
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "main.h"
#include "__ExegerGeneric.h"

#define TX_BUFF_SIZE				256

typedef struct Uart_HeadTail
{
	uint16_t Head;
	uint16_t Tail;
} Uart_HeadTail;

#endif /* GLOBAL_H_ */
