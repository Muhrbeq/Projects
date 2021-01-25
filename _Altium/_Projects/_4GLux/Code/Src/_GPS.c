/*
 * _GPS.c
 *
 *  Created on: 25 jan. 2021
 *      Author: rasmu
 */

#include "__ExegerGeneric.h"
#include "_Global.h"
#include "_GPS.h"

volatile uint8_t uart3_Buffer[TX_BUFF_SIZE] = { 0 };

/* Usart3 IRQ handler */
void USART3_Callback(void)
{

	/* Check if RXNE flag high */
	if ((USART3->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)
	{
		/* Read Data Register on USART3 (Sara module) */
		uint8_t data = USART3->RDR;

		/* Disable \r and \n from buffer */
		if (data != '\r')
		{
			if (data != '\n')
			{
				/* Add data to Buffer */
				uart3_Buffer[uart3.Head] = data;
				uart3.Head++;
			}
		}

		/* Overflow Safety */
		if (uart3.Head > TX_BUFF_SIZE - 1)
		{
			uart3.Head = 0;
		}
	}
}
