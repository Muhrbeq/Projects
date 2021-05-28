/*
 * utils.c
 *
 *  Created on: 10 jan. 2019
 *      Author: Rasmus.Muhrbeck
 */

/* Macro for converting char to digit */

#define CharToDigit(c)                        ((c) - '0')

#define IsDigit(c)                            (((c) >= '0') && ((c) <= '9'))

#include "utils.h"

/* Convert char to hex */
uint8_t CharToHex(char in, uint8_t *get)
{
	uint8_t ret = SUCCESS;

	if (get)
	{
		/* Is the character digit */
		if (IsDigit(in))
		{
			*get = CharToDigit(in);
		}
		/* Is the character capital letter */
		else if (in >= 'A' && in <= 'F')
		{
			*get = (in - 'A' + 0x0a);
		}
		/* Is the character small letter */
		else if (in >= 'a' && in <= 'f')
		{
			*get = (in - 'a' + 0x0a);
		}
		else
			ret = false; //CharToHex_InvalidHexChar;
	}
	else
		ret = false; //CharToHex_NullDataIn;

	return ret;
}

uint8_t StringToHEX(char *str, int len, uint32_t *retVal)
{
	uint8_t ret;
	uint8_t hex;
	int i = 0;

	if (str && retVal)
	{
		/* Must be valid length (4bit / len, max 8 = 32bit) */
		if (len <= 8 && len > 0)
		{
			/* Reset Value */
			*retVal = 0;

			/* Run trough string and add accordingly */
			for (i = 0; i < len; i++)
			{
				/* Convert single char to hex value */
				ret = CharToHex(str[i], &hex);
				if (ret == SUCCESS)
				{
					/* Add value correct by shifting it in */
					*retVal += (hex << (4 * (len - i - 1)));
				}
				else
					break;
			}
		}
		else
			ret = false; //StringtoHEX_InvalidLen;
	}
	else
		ret = false; //StringtoHEX_NullDataIn;

	return ret;
}


