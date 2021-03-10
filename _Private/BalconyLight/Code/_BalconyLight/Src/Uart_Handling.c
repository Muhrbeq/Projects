/*
 * Uart_Handling.c
 *
 *  Created on: 26 aug. 2020
 *      Author: Rasmus.Muhrbeck
 */

#include "Uart_Handling.h"
#include "Global.h"
#include "main.h"


Uart_HeadTail headTail;

uint8_t checkCommando = 0;

/* UART buffer init */
volatile uint8_t uart_Buffer[TX_BUFF_SIZE] = { 0 };

void EnableBLE(void)
{
	HAL_GPIO_WritePin(BLE_En_GPIO_Port, BLE_En_Pin, GPIO_PIN_SET);
}

void DisableBLE(void)
{
	HAL_GPIO_WritePin(BLE_En_GPIO_Port, BLE_En_Pin, GPIO_PIN_RESET);
}

void USART_Callback(void)
{
	/* Check if RXNE flag high */
	if ((USART2->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
	{
		/* Read Data Register on USART1 (Sara module) */
		uint8_t data = USART2->RDR;

		/* Disable \r and \n from buffer */
		if (data != '\r')
		{
			if (data != '\n')
			{
				/* Add data to Buffer */
				uart_Buffer[headTail.Head] = data;
				headTail.Head++;
			}
			else
			{
				checkCommando = 1;
			}
		}

		/* Overflow Safety */
		if (headTail.Head > TX_BUFF_SIZE - 1)
		{
			headTail.Head = 0;
		}
	}
}

void GetStringBLE(uint8_t *get)
{

	uint16_t u_counter = 0;

	if (headTail.Head < headTail.Tail)
	{
		u_counter = TX_BUFF_SIZE - (headTail.Tail - headTail.Head);
	}

	else
	{
		u_counter = headTail.Head - headTail.Tail;
	}

	for(int i = 0; i < u_counter; i++)
	{
		get[i] = uart_Buffer[headTail.Tail];

		/* Tail jumps one position */
		headTail.Tail++;

		/* If tail gets bigger than buffer */
		if (headTail.Tail > TX_BUFF_SIZE - 1)
		{
			/* Jump back to first position */
			headTail.Tail = 0;
		}
	}
}

void PrintFromBLE(void)
{
	char printString[256] = {0};

	char uartString[256] = {0};

	uint16_t u_counter = 0;

	if (headTail.Head < headTail.Tail)
	{
		u_counter = TX_BUFF_SIZE - (headTail.Tail - headTail.Head);
	}

	else
	{
		u_counter = headTail.Head - headTail.Tail;
	}

	for(int i = 0; i < u_counter; i++)
	{
		uartString[i] = uart_Buffer[headTail.Tail];

		/* Tail jumps one position */
		headTail.Tail++;

		/* If tail gets bigger than buffer */
		if (headTail.Tail > TX_BUFF_SIZE - 1)
		{
			/* Jump back to first position */
			headTail.Tail = 0;
		}
	}

	sprintf(printString, "%s\r\n", uartString);

	printf(printString);
}
