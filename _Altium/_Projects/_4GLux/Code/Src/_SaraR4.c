/*
 * _SaraR4.c
 *
 *  Created on: 7 jan. 2021
 *      Author: rasmu
 */


#include "_Global.h"
#include "__ExegerGeneric.h"

#include <_SaraR4.h>

UART uart2;

/* UART buffer init */
volatile uint8_t uart_Buffer[TX_BUFF_SIZE] = { 0 };

/* Usart2 IRQ handler */
void USART2_Callback(void)
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
				uart_Buffer[uart2.Head] = data;
				uart2.Head++;
			}
		}

		/* Overflow Safety */
		if (uart2.Head > TX_BUFF_SIZE - 1)
		{
			uart2.Head = 0;
		}
	}
}

/* See if Sara is alive */
uint8_t SaraIsAlive()
{
	/* Check if sara is alive */
	return SaraCheckCommand("AT\r\n", "OK", LOW_TIMEOUT);
}


uint8_t StartUpSara()
{
	/* Toggle LED to show init of startup sequence */
	//todo LED_Blink();

	/*-____________________________________________________-*/
	/* Set Radio Pin high to enable power to Radio module */

	// todo DisEnableRadioPin(1);
	HAL_Delay(1000);

	/*-____________________________________________________-*/

	/* Check if Sara module already ON */
	if (SaraCheckCommand("AT\r\n", "OK", LOW_TIMEOUT))
	{
		/* Sara is on */
		return true;
	}

	/* Set PIN_B14 (Power_Sara) high */
	// todo DisEnableSaraPin(1);

	/* 500-1500 ms to power on Sara */
	HAL_Delay(1050);

	/* Reset PIN_B14 (Sara is now ON) */
	//DisEnableSaraPin(0); //todo

	/* Wait for 5 sec (minimum 4.5s) for Sara to be fuctional*/
	HAL_Delay(5000);

//Heartbeat at end of cycle to show startup done
	//todo LED_Blink();

	/* Check if something is in the buffer, remove it */
	LL_USART_ReceiveData8(USART2);

	/* Init start of Head */
	uart2.Head = 0;

	/* Init start of Tail */
	uart2.Tail = 0;

	/* Check if the module is ON */
	if (SaraCheckCommand("AT\r\n", "ATOK", LOW_TIMEOUT))
	{
		/* Sara is on */
		return true;
	}

	/* Sara is not on */
	return false;
}

/* Init commands for Sara module */
uint8_t SaraInitAtCommands()
{
	/* Disables Echo mode */
	if (!SaraDisableEchoMode())
	{
		return false;
	}

	/* Activates the internet-LED to get visual confirmation when network is found */
	if (!SaraCheckCommand("AT+UGPIOC=16,2\r\n", "OK", LOW_TIMEOUT))
	{
		return false;
	}

	/* Checks if SIM is ready */
	/* This does not work for SIM cards with PIN code */
	if (!SaraCheckSimStatus())
	{
		return false;
	}

	return true;
}

/* Powers down Sara module */
uint8_t SaraPowerDown(void)
{
	return SaraCheckCommand("AT+CPWROFF\r\n", "OK", DEFAULT_TIMEOUT);
}

/* Disables Echo mode on responses from Sara */
uint8_t SaraDisableEchoMode()
{
	if (SaraCheckCommand("ATE0\r\n", "ATE0OK", LOW_TIMEOUT))
	{
		return true;
	}
	else if (SaraCheckCommand("ATE0\r\n", "OK", LOW_TIMEOUT))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* Checks if the SIM card is ready or not */
uint8_t SaraCheckSimStatus()
{
	return SaraCheckCommand(
			"AT+CPIN?\r\n",
			"+CPIN: READYOK",
			DEFAULT_TIMEOUT);
}

/* Set function of Sara module (Never used) */
uint8_t SaraSetCFUN(uint8_t mode)
{
	char txBuff[20] = { 0 };

	sprintf(txBuff, "AT+CFUN=%d\r\n", mode);

	return SaraCheckCommand(txBuff, "OK", 2 * DEFAULT_TIMEOUT);
}

/* Reboot Sara (never used) */
uint8_t SaraReboot()
{
	return SaraCheckCommand("AT+CFUN=15\r\n", "OK", DEFAULT_TIMEOUT);
}

/* Wait for response from sara module after AT command */
uint16_t SaraWaitForResponse(uint8_t *get, uint16_t count, uint32_t timeout_ms)
{
	uint32_t loop = 0;
	uint16_t u_counter = 0;

	/* Loop for 'timeout' specified time */
	while (loop <= timeout_ms)
	{
		if (uart2.Head < uart2.Tail)
		{
			u_counter = TX_BUFF_SIZE - (uart2.Tail - uart2.Head);
		}

		else
		{
			u_counter = uart2.Head - uart2.Tail;
		}

		/* When uart buffer has specified number of bytes, copy them */
		if (count == u_counter)
		{
			/* Fetch the data back to call */
			for (int i = 0; i < count; i++)
			{
				/* Assign received buffer the values of UART buffer */
				get[i] = uart_Buffer[uart2.Tail];

				/* Tail jumps one position */
				uart2.Tail++;

				/* If tail gets bigger than buffer */
				if (uart2.Tail > TX_BUFF_SIZE - 1)
				{
					/* Jump back to first position */
					uart2.Tail = 0;
				}
			}
			/* Reset counter for next command */
			uart2.Tail = uart2.Head;

			/* Return number of bytes copied */
			printf("count == counter\r\n");
			return u_counter;
		}
		/* Delay wait for more data */
		HAL_Delay(1);

		/* (1 ms increments) */
		loop += 1;
	}

	/* Just copy the volatile counter locally */
	if (uart2.Head < uart2.Tail)
	{
		/* See above explanation */
		u_counter = TX_BUFF_SIZE - (uart2.Tail - uart2.Head);
	}
	else
	{
		/* See above explanation */
		u_counter = uart2.Head - uart2.Tail;
	}

	/* If the uart counter is bigger than the count specified */
	if (u_counter > count)
	{
		/* Reassign ucounter = count */
		u_counter = count;
	}

	/* Loop through the uart buffer */
	for (int i = 0; i < u_counter; i++)
	{
		/* Received buffer = uart buffer */
		get[i] = uart_Buffer[uart2.Tail];

		/* Tail jumps one position */
		uart2.Tail++;

		/* If tail gets bigger than buffer */
		if (uart2.Tail > TX_BUFF_SIZE - 1)
		{
			/* Jump back to first position */
			uart2.Tail = 0;
		}
	}
	/* Make sure that the tail = head so that they start at same place for new command */
	uart2.Tail = uart2.Head;

	/* Return number of bytes copied */
	printf("count != counter\r\n");
	return u_counter;
}

/* Send Command to sara module */
void SaraSendCommand(const char *cmd)
{
	uint8_t len = strlen(cmd);
	uint8_t *pchar = (uint8_t*) cmd;

	/* Send characters one per one, until last char to be sent */
	for (uint8_t i = 0; i < len; i++)
	{
		/* Wait for TXE flag to be raised */
		while (!LL_USART_IsActiveFlag_TXE(USART2))
		{
		}

		/* Write character in Transmit Data register.
		 TXE flag is cleared by writing data in TDR register */
		LL_USART_TransmitData8(USART2, *pchar++);
	}

	/* Wait for TC flag to be raised for last char */
	while (!LL_USART_IsActiveFlag_TC(USART2))
	{
	}

}

/* Send command and check if the received command is as expected */
uint8_t SaraCheckCommand(const char *sendCommand, const char *receiveCommand, uint32_t timeout_ms)
{
	/* Get length of expected response */
	uint8_t len = strlen(receiveCommand);

	/* Init local receiveBuffer to compare against expected response */
	uint8_t receiveBuff[TX_BUFF_SIZE] = { 0 };

	/* Send desired command to Sara module */
	SaraSendCommand(sendCommand);

	/* Wait for response from Sara module with the given length of expected response */
	SaraWaitForResponse(receiveBuff, len, timeout_ms);

	/* Debug */
	printf("Received data: %s, Expected Data: %s\r\n", receiveBuff, receiveCommand);

	/* Loop through both Buffers to see if they match */
	for (int i = 0; i < len; i++)
	{
		if (receiveBuff[i] != receiveCommand[i])
		{
			/* If an element does not match, print the response and the expected response */
			return false;
		}
	}

	/* If all element do match, print the response and the expected response */
	return true;

}
