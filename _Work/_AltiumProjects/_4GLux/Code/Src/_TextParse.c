/*
 * _TextParse.c
 *
 *  Created on: 25 jan. 2021
 *      Author: rasmu
 */

#define CharToDigit(c)                        ((c) - '0')

#define IsDigit(c)                            (((c) >= '0') && ((c) <= '9'))

#include "_TextParse.h"


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

/* Parses out the first JSON string that are in the buffer */
uint8_t TextParse_ParseText(uint8_t *get, const char *startPattern,
		      const char *endPattern, uint8_t *set)
{
	/* Check if the startpattern appears in the string */
	const char *iStartPattern = strstr((char*) get, startPattern);

	/* If it appears */
	if (iStartPattern != NULL)
    {
		/* Check if endpattern appears */
		const size_t placeEnd = strlen(endPattern);
		const char *iEndPattern = strstr(iStartPattern, endPattern);

		/* if it appears */
		if (iEndPattern != NULL) {
			/* length of JSON will be the start of startpattern and END of endpattern */
			const size_t messageLen = iEndPattern + placeEnd - (iStartPattern);

			/* Assign buffer */
			uint8_t ret[messageLen + 1];

			/* If buffer isn't null, continue */
			if (ret != NULL) {
				/* Copy the message to buffer */
				memcpy(ret, iStartPattern, messageLen);

				/* put an end sign to the buffer */
				ret[messageLen] = '\0';

				/* Copy to the set buffer */
				for (int i = 0; i < messageLen; i++) {
					set[i] = ret[i];
				}

				/* return true */
				return true;
			}
		}
    }
	/* start/endpattern not found, return false */
	return false;
}

