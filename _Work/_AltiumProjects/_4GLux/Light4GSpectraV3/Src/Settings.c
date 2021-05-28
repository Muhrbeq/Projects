/*
 * Settings.c
 *
 *  Created on: Nov 20, 2012
 *      Author: Daniel
 */

#include "Settings.h"
#include "Global.h"
#include "Error.h"
/* Settings structure */

/* Assign unit ID to struct */
uint8_t SetUnitID(char *data, int len, SaraConfig *s)
{
	uint8_t ret = SUCCESS;

	/* Write to struct */
	strcpy((char*) s->id, data);

	return ret;
}

/* Assign port to struct */
uint8_t SetPORT(char *data, int len, SaraConfig *s)
{
	uint8_t ret = SUCCESS;

	/* Write to struct */
	strcpy((char*) s->port, data);

	return ret;
}

uint8_t SetRevision(char *data, int len, SaraConfig *s)
{
	uint8_t ret = SUCCESS;

	/* Write to struct */
	strcpy((char*) s->revision, data);

	return ret;
}

/* Assign IP to struct */
uint8_t SetIP(char *data, int len, SaraConfig *s)
{
	uint8_t ret = SUCCESS;

	/* Write to struct */
	strcpy(s->ip, data);

	return ret;
}

/* Assign if server want the module to do something to struct */
uint8_t SetServerToDo(char *data, int len, SaraConfig *s)
{
	uint8_t ret = SUCCESS;

	/* Write to struct */
	s->serverToDo = strtol(data, NULL, 10);

	return ret;
}

uint8_t SetUpdateFrequency(char *data, int len, SaraConfig *s)
{
	uint8_t ret = SUCCESS;

	/* Write to struct */
	s->updateFrequency = strtol(data, NULL, 10);

	return ret;
}

uint8_t SetHourToSend(char *data, int len, SaraConfig *s)
{
	uint8_t ret = SUCCESS;

	/* Write to struct */
	s->hourToSend = strtol(data, NULL, 10);

	return ret;
}

uint8_t FirstMessageToResend(char *data, int len, SaraConfig *s)
{
	uint8_t ret = SUCCESS;

	/* Write to struct */
	s->firstMessageToReSend = strtol(data, NULL, 10);

	return ret;
}

uint8_t LastMessageToResend(char *data, int len, SaraConfig *s)
{
	uint8_t ret = SUCCESS;

	/* Write to struct */
	s->lastMessageToReSend = strtol(data, NULL, 10);

	return ret;
}




TOKHeader ParseJSMNForHeader(const char *js, JsmnTok tok)
{
	TOKHeader h = TOK_NO_HEADER;
	char str[32] = {0};
	int len = tok.end - tok.start;

	if(js)
	{
		/* If data length between 0 and 32 = header */
		if(len > 0 && len < 32)
		{
			/* Copy substring to str */
			strncpy(str, (js + tok.start), len);

			if (!strcmp(str, "ip"))
			{
				h = TOK_IP;
			}
			else if (!strcmp(str, "port"))
			{
				h = TOK_PORT;
			}
			else if (!strcmp(str, "id"))
			{
				h = TOK_ID;
			}
			else if (!strcmp(str, "serverToDo"))
			{
				h = TOK_SERVERTODO;
			}
			else if (!strcmp(str, "updateFrequency"))
			{
				h = TOK_UPDATEFREQ;
			}
			else if (!strcmp(str, "hourToSend"))
			{
				h = TOK_HOURTOSEND;
			}
			else if (!strcmp(str, "revision"))
			{
				h = TOK_REVISION;
			}
			else if (!strcmp(str, "fMTR")) //firstMessageToResend
			{
				h = TOK_FIRSTMESSAGE;
			}
			else if (!strcmp(str, "lMTR")) //firstMessageToResend
			{
				h = TOK_LASTMESSAGE;
			}
			else
			{
				h = TOK_NO_HEADER;
			}
		}
	}

	return h;
}

static uint8_t ParseJSMNData(const char *js, JsmnTok tok, TOKHeader h,
		SaraConfig *s)
{
	uint8_t res = SUCCESS;
	char *data;
	int dataLen = tok.end - tok.start;

	if(js && s)
	{
		if(h != TOK_NO_HEADER)
		{
			/* Check that the data is valid */
			if(dataLen > 0 && dataLen < 4096)
			{
				/* Allocate memory for data */
				data = malloc(dataLen + 1);
				if(data)
				{
					/* Clear data buffer */
					memset(data, 0, dataLen + 1);

					/* Copy the data into buffer */
					strncpy(data, (js + tok.start), dataLen);

					switch(h)
					{
					case TOK_ID:
						res = SetUnitID(data, dataLen, s);
						break;

					case TOK_PORT:
						res = SetPORT(data, dataLen, s);
						break;

					case TOK_IP:
						res = SetIP(data, dataLen, s);
						break;

					case TOK_SERVERTODO:
						res = SetServerToDo(data, dataLen, s);
						break;

					case TOK_UPDATEFREQ:
						res = SetUpdateFrequency(data, dataLen, s);
						break;

					case TOK_HOURTOSEND:
						res = SetHourToSend(data, dataLen, s);
						break;

					case TOK_REVISION:
						res = SetRevision(data, dataLen, s);
						break;

					case TOK_FIRSTMESSAGE:
						res = FirstMessageToResend(data, dataLen, s);
						break;

					case TOK_LASTMESSAGE:
						res = LastMessageToResend(data, dataLen, s);
						break;

					case TOK_IGNORE:
						res = SUCCESS;
						break;

					default:
						res = SUCCESS;
						break;
					}

					free(data);
				}
				else
				{
					ErrorAddLog(LOG_FUNC_SETTING, LOG_SUB_SECTOR, LOG_TYPE_WRITE, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

					res = false;
				}
			}
			else
			{
				ErrorAddLog(LOG_FUNC_SETTING, LOG_SUB_PARSE, LOG_TYPE_WRITE, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

				res = false;
			}
		}
		else
		{
			ErrorAddLog(LOG_FUNC_SETTING, LOG_SUB_PARSE, LOG_TYPE_START, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			res = false;
		}
	}
	else
	{
		ErrorAddLog(LOG_FUNC_SETTING, LOG_SUB_DATA, LOG_TYPE_NONE, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

		res = false;
	}

	return res;
}

uint8_t ParseSettingsString(char *set, SaraConfig *s)
{
	JsmnParser p;
	JsmnTok tok[66]; //1 data pair = 2 tokens ("unitId":":1234567890")
	uint8_t ret = SUCCESS;
	int i = 1;

	/* Initialize JSMN */
	JsmnInitialize(&p);

	if(set && s)
	{
		/* Parse string (max 64 headers + data ) */
		if(JsmnParse(&p, set, tok, 64) == JSMN_SUCCESS)
		{
			/* If token 0 is a object = { yyyy } */
			if(tok[0].type == JSMN_OBJECT)
			{
				/* Run trough all data tokens in object */
				for(i = 1; (i <= tok[0].size) && (ret == SUCCESS); i += 2)
				{
					/* Look for strings "btn" : "02" */
					if(tok[i].type == JSMN_STRING)
					{
						/* Find header in first token (i) then parse data from second (i+1) */
						ret = ParseJSMNData(set, tok[i + 1], ParseJSMNForHeader(set, tok[i]), s);
					}
					else
					{
						ErrorAddLog(LOG_FUNC_SETTING, LOG_SUB_JSMN, LOG_TYPE_ID, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

						ret = false;
					}
				}
			}
			else
			{
				ErrorAddLog(LOG_FUNC_SETTING, LOG_SUB_JSMN, LOG_TYPE_READ, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

				ret = false;
			}
		}
		else
		{
			ErrorAddLog(LOG_FUNC_SETTING, LOG_SUB_JSMN, LOG_TYPE_SEEK, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			ret = false;
		}
	}
	else
	{
		ErrorAddLog(LOG_FUNC_SETTING, LOG_SUB_JSMN, LOG_TYPE_NONE, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

		ret = false;
	}

	return ret;
}

/* Not used, only for testing */
//uint8_t PrintSettings(SaraConfig *s)
//{
//	printf("ID: %s\r\n"
//			"IP: %s\r\n"
//			"Port: %d\r\n", s->id, s->ip,
//			s->port);
//
//	return true;
//}
//
//
//void TestSetting(void)
//{
//	char idJason[] = "{\"id\":\"R01M9410\"}";
//	char ownJSON[] = "{\"ip\":\"192.168.1.2\"}";
//	char portJSON[] = "{\"port\":\"1337\"}";
//	char allJSON[] =
//			"{\"id\":\"ramuramu\",\"ip\":\"192.168.1.1337\",\"port\":\"1234\"}";
//
//	SaraConfig ownJSONset;
//
//	ParseSettingsString(idJason, &ownJSONset);
//	PrintSettings(&ownJSONset);
//	ParseSettingsString(allJSON, &ownJSONset);
//	PrintSettings(&ownJSONset);
//	ParseSettingsString(ownJSON, &ownJSONset);
//	PrintSettings(&ownJSONset);
//	ParseSettingsString(portJSON, &ownJSONset);
//	PrintSettings(&ownJSONset);
//
//}

