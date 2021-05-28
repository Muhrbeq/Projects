/*
 * firmware.c
 *
 *  Created on: 10 jan. 2019
 *      Author: Rasmus.Muhrbeck
 */


#include "sara.h"
#include "main.h"
#include "Settings.h"
#include "mqtt.h"
#include "Global.h"

/* Copy settings from one struct to another of same type */
uint8_t CopySettings(SaraConfig *to, SaraConfig *from)
{
	if (!to || !from)
		return false;

	/* Copy structs */
	memcpy(to, from, sizeof(SaraConfig));
	return SUCCESS;
}

uint8_t ServerMessage(uint8_t *rxBuff)
{
	uint8_t retVal = false;

	/* Check if server returned anything */
	SaraSendCommand("AT+UMQTTC=6\r\n");
	SaraWaitForResponse(rxBuff, TX_BUFF_SIZE - 1, 2 * DEFAULT_TIMEOUT);

	/* If there is a message on the topic, read it */
	if (MqttCheckRxData(rxBuff))
	{
		/* Message on server received */
		retVal = true;
	}

	/* Return true or false */
	return retVal;
}

uint8_t ServerDo(char *topic)
{
	SaraConfig *sC = GetSaraConfigurationStruct();

	/* Init all buffers that are needed */
	uint8_t rxBuff[TX_BUFF_SIZE] = { 0 };
	uint8_t JSONBuff[TX_BUFF_SIZE] = { 0 };

	/* Return value starts false */
	uint8_t retVal = false;

	/* Temporary struct */
	SaraConfig tempSara;

	/* Copy previous settings to temp struct */
	CopySettings(&tempSara, GetSaraConfigurationStruct());

	/* Publish to testTT */
	if (MqttPublish(topic, sC->id, 0, 0))
	{
		/* Check if server returned anything */
		if (ServerMessage(rxBuff))
		{
			/* Parse the settings and assign them to temp struct */
			if (FetchVerifyParseJSON(rxBuff, JSONBuff, &tempSara))
			{
				/* Check if the id from the server matches the modules ID */
				if (!strcmp(tempSara.id, sC->id))
				{
					/* If they do, download settings by copying them to SaraConfiguration */
					CopySettings(GetSaraConfigurationStruct(), &tempSara);
					retVal = true;
				}
			}
		}
	}
	/* Return true or false */
	return retVal;
}


/*____________________________________NOT USED____________________________________*/










//uint8_t NewFirmware(SaraConfig *s)
//{
//	/* if(s->firmwareUpdateAvailable == 1)
//	 * 		return true
//	 * else
//	 * 		return false*/
//
//	uint8_t retVal = true;
//
//	return retVal;
//}
//
//uint8_t DownloadFirmware(void)
//{
//	/* Need to think bout this one */
//
//	uint8_t retVal = true;
//
//	return retVal;
//}
//
//uint8_t NewSettings(SaraConfig *s)
//{
//	uint8_t retVal = true;
//
//	if (s->newSetAvail == 0)
//	{
//		retVal = false; //No new settings
//	}
//
//	return retVal;
//}

//uint8_t DownloadSettings(SaraConfig *s)
//{
//	uint8_t retVal = false;
//
//	uint8_t txBuff[64];
//	uint8_t rxBuff[TX_BUFF_SIZE] =
//	{ 0 };
//	uint8_t topic[] = "testTT";
//	sprintf(txBuff, "AT+UMQTTC=2,0,0,\"%s\",\"%s\"\r\n", (char*) topic, s->id);
//	if (SaraCheckCommand((char*) txBuff, "+UMQTTC: 2,1", 5 * DEFAULT_TIMEOUT))
//	{
//		SaraSendCommand("AT+UMQTTC=6\r\n");
//		SaraWaitForResponse(rxBuff, TX_BUFF_SIZE - 1, DEFAULT_TIMEOUT);
//	}
//
//	return retVal;
//}

//
//uint8_t ServerToDo(SaraConfig *s)
//{
//	uint8_t rxBuff[TX_BUFF_SIZE] =
//	{ 0 };
//	uint8_t JSONBuff[TX_BUFF_SIZE] =
//	{ 0 };
//	uint8_t retVal = false;
//	uint8_t topic[] = "testTT";
//
//	/* Subscribe to Update */
//	if (MqttSubscribe((char*) topic, 0))
//	{
//		if (FetchVerifyParseJSON(rxBuff, JSONBuff, s))
//		{
//			if (s->serverToDo == 1)
//			{
//				printf("new setting available\r\n");
//				s->newSetAvail = 1;
//				retVal = true;
//			}
//			else if (s->serverToDo == 2)
//			{
//				s->newFwAvail = 1;
//				printf("new firmware available\r\n");
//				retVal = true;
//			}
//			else if (s->serverToDo == 3)
//			{
//				s->newFwAvail = 1;
//				s->newSetAvail = 1;
//				printf("new setting and firmware available\r\n");
//				retVal = true;
//			}
//		}
//	}
//	else if (MqttUnSubscribe((char*) topic))
//	{
//		if (MqttSubscribe((char*) topic, 0))
//		{
//			if (FetchVerifyParseJSON(rxBuff, JSONBuff, s))
//			{
//				if (s->serverToDo == 1)
//				{
//					s->newSetAvail = 1;
//					printf("new setting available\r\n");
//					retVal = true;
//				}
//				else if (s->serverToDo == 2)
//				{
//					s->newFwAvail = 1;
//					printf("new firmware available\r\n");
//					retVal = true;
//				}
//				else if (s->serverToDo == 3)
//				{
//					s->newFwAvail = 1;
//					s->newSetAvail = 1;
//					printf("new setting and firmware available\r\n");
//					retVal = true;
//				}
//			}
//		}
//	}
//	return retVal;
//}

