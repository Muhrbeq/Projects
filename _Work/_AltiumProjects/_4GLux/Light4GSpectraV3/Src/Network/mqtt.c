/*
 * mqtt.c
 *
 *  Created on: 4 dec. 2018
 *      Author: rasmus.muhrbeck
 */

#include "Global.h"
#include "mqtt.h"
#include "Settings.h"
#include "crc.h"
#include "firmware.h"
#include "utils.h"

#include "Error.h"
#include "sara.h"
#include "TimeFunc.h"
#include "LowLevel.h"


/* Local buffer size for each function */


/*__________________________*/

/* Functions starts */
/*__________________________*/

/* MqttSetClientID
 *
 * Sets a unique client ID that will be used in distinguishing it
 * from other clients by the Broker.
 *
 * @clientID	- Unique ID
 *
 * @return value: True/SUCCESS or False/FAILURE
 * */
uint8_t MqttSetClientID(char* clientID)
{
	char txBuff[TX_BUFF_SIZE] = { 0 };

	/* Copying to txBuff */
	sprintf(txBuff, "AT+UMQTT=0,\"%s\"\r\n", clientID);

	/* Returns true or false */
	return SaraCheckCommand(txBuff, "+UMQTT: 0,1", DEFAULT_TIMEOUT);
}

/* MqttSetAuthentication
 *
 * Sets the username and password to authenticate the the user
 * is allowed on Broker (only in case of secure Broker)
 *
 * @userName	- Username defined by the Broker
 * @passwd		- Password defined by the Broker
 *
 * @return value: True/SUCCESS or False/FAILURE
 * */
uint8_t MqttSetAuthentication(char* userName, char* passwd)
{
	char txBuff[TX_BUFF_SIZE] = { 0 };

	/* Copying to txBuff */
	sprintf(txBuff, "AT+UMQTT=4,\"%s\",\"%s\"\r\n", userName, passwd);

	/* Returns true or false */
	return SaraCheckCommand(txBuff, "+UMQTT: 4,1", DEFAULT_TIMEOUT);
}

/* MqttSetWill
 *
 * In case of unexpected disconnect, the Will message will be on the Broker
 *
 * @topic	- The topic that you would like to publish the Will to
 * @message	- The message you like to publish on the topic
 * @QoS		- Quality of service:
 0 (default value): at most once delivery
 1: at least once delivery
 2: exactly once delivery
 * @retain	- 0 (default value): the message will not be retained at the MQTT server across
 disconnects from MQTT client
 1: the message will be retained by the MQTT server across disconnects from MQTT
 client (until superceded by another message)
 *
 * @return value: True/SUCCESS or False/FAILURE
 *  */
uint8_t MqttSetWill(char* topic, char* message, uint8_t QoS, uint8_t retain) {
	char txBuff[TX_BUFF_SIZE] = { 0 };

	/* Copying to txBuff */
	sprintf(txBuff, "AT+UMQTTWTOPIC=%d,%d,%s\r\n", QoS, retain, topic);

	/* If will on topic */
	if (!SaraCheckCommand(txBuff, "+UMQTTWTOPIC: 1", DEFAULT_TIMEOUT)) {
		return false;
	}

	/* Copying to txBuff */
	sprintf(txBuff, "AT+UMQTTWMSG=\"%s\"\r\n", message);

	/* Publish Will-message to topic */
	if (!SaraCheckCommand(txBuff, "+UMQTTWMSG: 1", DEFAULT_TIMEOUT)) {
		return false;
	}

	/* If will on topic */
	return true;
}

/* MqttClearWill
 *
 * Clears the Will message
 *
 * @return value: True/SUCCESS or False/FAILURE
 *  */
uint8_t MqttClearWill() {
	char txBuff[TX_BUFF_SIZE] = { 0 };

	/* Copying to txBuff */
	sprintf(txBuff, "AT+UMQTTWTOPIC=%d,%d,%s\r\n", 0, 0, "");

	/* If will on topic */
	if (!SaraCheckCommand(txBuff, "+UMQTTWTOPIC: 1", DEFAULT_TIMEOUT)) {
		printf("Error\r\n");
		return false;
	}

	/* Copying to txBuff */
	sprintf(txBuff, "AT+UMQTTWMSG=\"%s\"\r\n", "");

	/* Publish Will-message to topic */
	if (!SaraCheckCommand(txBuff, "+UMQTTWMSG: 1", DEFAULT_TIMEOUT)) {
		return false;
	}

	/* Will cleared */
	return true;
}




uint8_t MqttSaveProfile() {
  return SaraCheckCommand ("AT+UMQTTNV=2\r\n", "+UMQTTNV: 2,1",
  DEFAULT_TIMEOUT);
}

uint8_t MqttRestoreProfile() {
  return SaraCheckCommand ("AT+UMQTTNV=1\r\n", "+UMQTTNV: 1,1",
  DEFAULT_TIMEOUT);
}

uint8_t MqttDeleteProfile() {
  return SaraCheckCommand ("AT+UMQTTNV=0\r\n", "+UMQTTNV: 0,1",
  DEFAULT_TIMEOUT);
}

/* MqttSetServer
 *
 * Sets the server that the MQTT communication should work towards,
 * neither subscribe or publish has been chosen yet.
 *
 * @server	- Server IP address
 * @port	- Server port address
 *
 * @return value: True/SUCCESS or False/FAILURE
 * */
uint8_t MqttSetServer(char* server, uint16_t port)
{
	char txBuff[TX_BUFF_SIZE] = { 0 };

	/* Copying to txBuff */
	sprintf(txBuff, "AT+UMQTT=3,\"%s\",%d\r\n", server, port);

	/* If true, server is set. If false, server set failed */
	return SaraCheckCommand(txBuff, "+UMQTT: 3,1", LOW_TIMEOUT);
}

/* Connects to the server defined by MqttSetServer()
 *
 * @return value: True/SUCCESS or False/FAILURE
 * */
uint8_t MqttConnect()
{
	/* Tries to connect to server, if true -> server connection successful.
	 * If false -> server connection failed */
	return SaraCheckCommand("AT+UMQTTC=1\r\n", "+UMQTTC: 1,1OK+UUMQTTC: 1,0",
			5 * DEFAULT_TIMEOUT);
}

/* Sets the webPage that Sara should connect against */
uint8_t MqttSetHttp(char *server)
{
	char txBuff[TX_BUFF_SIZE] = { 0 };

	/* Copying to txBuff */
	sprintf(txBuff, "AT+UMQTT=2,\"%s\"\r\n", server);

	/* If true, server is set. If false, server set failed */
	return SaraCheckCommand(txBuff, "+UMQTT: 2,1OK", LOW_TIMEOUT);
}

/* Publishes to the server defined by MqttSetServer()
 *
 * @topic	- The topic that you would like to publish to
 * @message	- The message you like to publish on the topic
 * @QoS		- Quality of service:
 0 (default value): at most once delivery
 1: at least once delivery
 2: exactly once delivery
 * @retain	- 0 (default value): the message will not be retained at the MQTT server across
 disconnects from MQTT client
 1: the message will be retained by the MQTT server across disconnects from MQTT
 client (until superceded by another message)
 *
 * @return value: True/SUCCESS or False/FAILURE
 * */
uint8_t MqttPublish(const char* topic, const char* message, uint8_t QoS,
	     uint8_t retain)
{
	char txBuff[TX_BUFF_SIZE] = { 0 };

	/* Copying to txBuff */
	sprintf((char*) txBuff, "AT+UMQTTC=2,%d,%d,\"%s\",\"%s\"\r\n", QoS, retain,
	   topic, message);

	/* If true, publish successful */
	return SaraCheckCommand((char*) txBuff, "+UMQTTC: 2,1OK", 5 * DEFAULT_TIMEOUT);
}

/* THIS IS NOT SUPPORTED BY SARA R4 (NO FILE TRANSFER) */
uint8_t MqttPublishFile(uint8_t QoS, uint8_t retain, const char* topic,
		const char* filename)
{
	char txBuff[TX_BUFF_SIZE] = { 0 };

	/* Copying to txBuff */
	sprintf((char*) txBuff, "AT+UMQTTC=3,%d,%d,\"%s\",\"%s\"\r\n", QoS, retain,
			topic, filename);

	/* If true, publish successful */
	return SaraCheckCommand((char*) txBuff, "+UMQTTC: 3,1", 5 * DEFAULT_TIMEOUT);
}

/* Subscribes to the server defined by MqttSetServer()
 *
 * @topic	- The topic that you would like to subscribe to
 * @QoS		- Quality of service:
 0 (default value): at most once delivery
 1: at least once delivery
 2: exactly once delivery
 *
 * @return value: True/SUCCESS or False/FAILURE
 * */
uint8_t MqttSubscribe(char* topic, uint8_t QoS)
{
	char txBuff[TX_BUFF_SIZE] = { 0 };
	char expectedAns[64] = { 0 };

	sprintf(expectedAns, "+UMQTTC: 4,1OK");

	/* Copying to txBuff */
	sprintf(txBuff, "AT+UMQTTC=4,%d,\"%s\"\r\n", QoS, topic);

	/* If true, Subscribe successful */
	return SaraCheckCommand(txBuff, expectedAns, 5 * DEFAULT_TIMEOUT);

//return true;
}

/* Unsubscribes to the server defined by MqttSetServer()
 *
 * @topic	- The topic that you would like to unsubscribe to
 *
 * @return value: True/SUCCESS or False/FAILURE
 * */
uint8_t MqttUnSubscribe(char* topic)
{
	char txBuff[TX_BUFF_SIZE] = { 0 };

	/* Copying to txBuff */
	sprintf(txBuff, "AT+UMQTTC=5,\"%s\"\r\n", topic);

	/* If true, Unsubscription successful */
	return SaraCheckCommand(txBuff, "+UMQTTC: 5,1OK", 5 * DEFAULT_TIMEOUT);
}

/* Disconnects to the server defined by MqttSetServer()
 *
 * @return value: True/SUCCESS or False/FAILURE
 * */
uint8_t MqttDisconnect()
{
	/* If true, disconnect successful */
	return SaraCheckCommand("AT+UMQTTC=0\r\n", "+UMQTTC: 0,1OK",
			5 * DEFAULT_TIMEOUT);
}

/* Publish with ID and CRC together with message */
uint8_t MqttPushPublish(const char* topic, const char* message, uint8_t QoS, uint8_t retain)
{
	SaraConfig *sC = GetSaraConfigurationStruct();

	char txBuff[256] = { 0 };

	uint32_t cCrc;

	/* Get Crc of message */
	GetCRC((uint8_t*) message, &cCrc);

	/* Copy all to txBuff */
	sprintf((char*) txBuff, "AT+UMQTTC=2,%d,%d,\"%s\",\"$%s@%s^%08X#\"\r\n", QoS, retain, topic,
			sC->id,
			message,
			(unsigned int) cCrc);

	/* Send txBuff to Sara */
//	SaraSendCommand(txBuff);  		//
	SaraCheckCommand(txBuff, "+UMQTTC: 2,1OK", 100);

	HAL_Delay(20);

	/* Wait for 100 ms so that all packets can be sent before sending next */
	/* ONLY NEEDED WHEN SaraSendCommand IS USED, NOT WHEN SaraCheckCommand IS USED */
//	HAL_Delay(100);
	return true;
}

/* Publish with ID and CRC together with message */
uint8_t MqttPushPublishSD(const char* topic, const char* message, uint8_t QoS, uint8_t retain)
{
	SaraConfig *sC = GetSaraConfigurationStruct();

	radioFlags *rE = GetErrorFlagsStruct();
	
	char txBuff[256] = { 0 };

	uint32_t cCrc;

	/* Get Crc of message */
	GetCRC((uint8_t*) message, &cCrc);

	/* Copy all to txBuff */
	sprintf((char*) txBuff, "AT+UMQTTC=2,%d,%d,\"%s\",\"$%s@%d%s^%08X#\"\r\n", QoS, retain, topic,
			sC->id,
			(int) rE->counterMessage,
			message,
			(unsigned int) cCrc);

	printf("DataSentFromSara: %s", message);
	/* Send txBuff to Sara */
//	SaraSendCommand(txBuff);  		//
	SaraCheckCommand(txBuff, "+UMQTTC: 2,1OK", LOW_TIMEOUT);

	/* Wait for 100 ms so that all packets can be sent before sending next */
	/* ONLY NEEDED WHEN SaraSendCommand IS USED, NOT WHEN SaraCheckCommand IS USED */
//	HAL_Delay(100);
	HAL_Delay(50);
	return true;
}

///* Not used for SD-push */
//uint8_t MqttPushDataToServer()
//{
//	/* Pushed data to server, not waiting for response */
//
//	/* Message you want to send buffer (max is 256) */
//	/* One message is approximately 50 chars max */
//
//	char submsg[64] = { 0 };
//	char msg[256] = { 0 };
//	uint8_t newPage = 0;
//	uint8_t firstMsg = 1;
//	uint8_t sentMsg = 1;
//
//	/* Timers */
//	Time startPush;
//	Time endPush;
//
//	int sensCount = data_global.sensCounter;
//
//	/* start of timer */
//	GetTime(&startPush);
//
//	/* Loop for each value in sensData */
//
//	for (int i = 0; i < sensCount; i++)
//	{
//		uint32_t lux;
//		uint16_t tempHum;
//		lux = (data_global.readData.sensData[i].tempHumLux >> 15);
//		tempHum = (data_global.readData.sensData[i].tempHumLux & 0b111111111111111);
//
//		/* Timer */
//		GetTime(&endPush);
//
//		/* How long has the foor loop been on for? */
//		if (abs(RTC_GetElapsedSeconds(&startPush, &endPush)) > 60)
//		{
//			/* If more than 30 sec, break */
//			break;
//		}
//
//		if (sentMsg == 1)
//		{
//			if (firstMsg == 1)
//			{
//				/* First message, add timestamp and battery level to message */
//				//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
//				/* Cant use ! or ? to separate %X.%X for some reason */
//				sprintf(msg, "@%d/%d&%X.%X", i, sensCount, (unsigned int) data_global.readData.timeStamp, data_global.readData.batteryLevel);
//				firstMsg = 0;
//			}
//
//			if (newPage == 1)
//			{
//
//
//				/* new page, add the number of the first struct */
//				//LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13);
//				//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
//				sprintf(submsg, "@%d/%d*%X:%X:%X:%X:%X:%X:%X:%X", i, sensCount, tempHum, (unsigned int) lux,
//						data_global.readData.sensData[i].spectrum.violet, data_global.readData.sensData[i].spectrum.blue, data_global.readData.sensData[i].spectrum.green,
//						data_global.readData.sensData[i].spectrum.yellow, data_global.readData.sensData[i].spectrum.orange, data_global.readData.sensData[i].spectrum.red);
//				newPage = 0;
//			}
//			else
//			{
//				/* Not new page, only add the data of the struct */
//				sprintf(submsg, "*%X:%X:%X:%X:%X:%X:%X:%X", tempHum, (unsigned int) lux, data_global.readData.sensData[i].spectrum.violet,
//						data_global.readData.sensData[i].spectrum.blue, data_global.readData.sensData[i].spectrum.green, data_global.readData.sensData[i].spectrum.yellow,
//						data_global.readData.sensData[i].spectrum.orange, data_global.readData.sensData[i].spectrum.red);
//			}
//
//			/* Calculate total length of string after adding them together */
//			uint16_t totLength = strlen(msg) + strlen(submsg);
//
//			/* Copy submsg to message */
//			strcat(msg, submsg);
//
//			/* Check if it is the last data sample or not */
//			if (i != sensCount - 1)
//			{
//
//				/* Not last sample, If the length of the message is ready to send */
//				if (totLength > 150)
//				{
//
//					/* For logic testing */
//					//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
//					//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
//					/* Publish the whole message to server with defined topic */
//					if (MqttPushPublish(SaraConfiguration.connectPubTopic, msg, 0, 0))
//					{
//						/* Reset whole message */
//						memset(msg, '\0', 256);
//
//						/* Reset sub message */
//						memset(submsg, '\0', 64);
//
//						/* Raise newPage flag */
//						newPage = 1;
//					}
//
//					else
//					{
//						sentMsg = 0;
//
//						i--;
//					}
////					MqttPushPublish(SaraConfiguration.connectPubTopic, msg, 1, 0);
//
////					/* Reset whole message */
////					memset(msg, '\0', 256);
////
////					/* Reset sub message */
////					memset(submsg, '\0', 64);
////
////					/* Raise newPage flag */
////					newPage = 1;
//
//					//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
//				}
//				else
//				{
//					/* Reset sub message */
//					memset(submsg, '\0', 64);
//				}
//			}
//			else
//			{
//				/* Last data, send */
//				//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
//				//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
//				MqttPushPublish(SaraConfiguration.connectPubTopic, msg, 0, 0);
//				//printf("All sent");
//				//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
//				return true;
//			}
//		}
//
//		else
//		{
//			if (MqttPushPublish(SaraConfiguration.connectPubTopic, msg, 0, 0))
//			{
//				/* Reset whole message */
//				memset(msg, '\0', 256);
//
//				/* Reset sub message */
//				memset(submsg, '\0', 64);
//
//				/* Raise newPage flag */
//				newPage = 1;
//
//				sentMsg = 1;
//
////				i--;
//			}
//			else
//			{
//				i--;
//			}
//		}
//	}
//	printf("Not all sent lel\r\n");
//	return false;
//}

///* Not used for SD-push */
//void ExtractOneMeasurementFromFlash(uint32_t flashAddress,
//		uint16_t *tempHum, uint32_t *lux, uint16_t *violet,
//		uint16_t *blue, uint16_t *green, uint16_t *yellow, uint16_t *orange,
//		uint16_t *red)
//{
//	/* Lux, tempHum */
//	uint32_t ltH;
//
//	/* Blue, Violet */
//	uint32_t bV;
//
//	/* Yellow, Green */
//	uint32_t yG;
//
//	/* Red, Orange */
//	uint32_t rO;
//
//	/* The address and placement of each measurement looks like
//	 *
//	 * | Lux-tempHum | Blue-Violet | Yellow-Green | Red-Orange |
//	 *
//	 * | 0xFFFF-FFFF | 0xFFFF-FFFF | 0xFFFF-FFFF  | 0xFFFF-FFFF|
//	 *  */
//
//	ltH = (*(uint32_t*) flashAddress);
//
//	flashAddress = flashAddress + 4;
//
//	bV = (*(uint32_t*) flashAddress);
//
//	flashAddress = flashAddress + 4;
//
//	yG = (*(uint32_t*) flashAddress);
//
//	flashAddress = flashAddress + 4;
//
//	rO = (*(uint32_t*) flashAddress);
//
//	flashAddress = flashAddress + 4;
//
//	*tempHum = ltH & 0b0111111111111111;
//
//	*lux = ltH >> 15;
//
//	*violet = bV & 0xFFFF;
//
//	*blue = bV >> 16;
//
//	*green = yG & 0xFFFF;
//
//	*yellow = yG >> 16;
//
//	*orange = rO & 0xFFFF;
//
//	*red = rO >> 16;
//
//}
//
///* Not used for SD-push */
//uint8_t MqttPushDataToServerFlash(uint8_t amountOfDays)
//{
//	/* Pushed data to server, not waiting for response */
//
//	/* Message you want to send buffer (max is 256) */
//	/* One message is approximately 50 chars max */
//	char submsg[64] = { 0 };
//	char msg[256] = { 0 };
//	uint8_t newPage = 0;
//
//	uint32_t counterOfDataPerDay = 0;
//
//	uint16_t tempHum, violet, blue, green, yellow, orange, red;
//	uint32_t lux;
//
//	/* Timers */
//	Time startPush;
//	Time endPush;
//
//	/* start of timer */
//	AssignTimeStruct(&startPush);
//
//	/* Loop through all days */
//	for (uint8_t i = amountOfDays; i > 0; i--)
//	{
//		/* Get address for day i */
//		uint32_t addressFlash = Flash_GetAddressDayX(i);
//
//		/* Get maxSize of 1 day */
//		uint32_t sizeT = Flash_GetAddressDayX(2) - Flash_GetAddressDayX(1);
//
//		/* Compute how many values are worth looking at */
//		uint32_t lenFlash = Flash_EOF(addressFlash, sizeT);
//
//		/* Haven't sent one yet, so this is first message */
//		uint8_t firstMsg = 1;
//
//		/* To get amount of measurements */
//		counterOfDataPerDay = (lenFlash - 2) / 4;
//
//		/* Timer */
//		AssignTimeStruct(&endPush);
//
//		/* How long has the foor loop been on for? */
//		if (abs(RTC_GetElapsedSeconds(&startPush, &endPush)) > 60)
//		{
//			/* If more than 60 sec, break */
//			return false;
//		}
//
//		/* Loop through the measurements */
//		for (int k = 0; k < counterOfDataPerDay; k++)
//		{
//			/* Timer */
//			AssignTimeStruct(&endPush);
//
//			/* How long has the foor loop been on for? */
//			if (abs(RTC_GetElapsedSeconds(&startPush, &endPush)) > 60)
//			{
//				/* If more than 60 sec, break */
//				return false;
//			}
//
//			/* If first message */
//			if (firstMsg == 1)
//			{
//				/* Take out timeStamp */
//				uint32_t timeStamp = (*(uint32_t*) addressFlash);
//
//				/* Move forward 1 register (4 byte) */
//				addressFlash += 4;
//
//				/* This register containes batteryLevel */
//				uint32_t batteryLevel = (*(uint32_t*) addressFlash);
//
//				/* Copy them to the message thats going to be transmittet */
//				sprintf(msg, "FLASH@%d/%d&%X.%X", k, (int) counterOfDataPerDay,
//						(unsigned int) timeStamp, (unsigned int) batteryLevel);
//
//				/* Move forward 1 register */
//				addressFlash += 4;
//
//				/* Not first message anymore */
//				firstMsg = 0;
//			}
//
//			if (newPage == 1)
//			{
//				/* If we have a new page */
//				sprintf(msg, "FLASH@%d/%d", k, (int) counterOfDataPerDay);
//
//				/* Next will not be a new page */
//				newPage = 0;
//			}
//
//			/* Extract one measurement from flash and assign to variables */
//			ExtractOneMeasurementFromFlash(addressFlash, &tempHum, &lux, &violet, &blue, &green, &yellow, &orange, &red);
//
//			/* Jump forward 4 registers (you will land on start of next measurement) */
//			addressFlash += 16;
//
//			/* Copy to submessage */
//			sprintf(submsg, "*%X:%X:%X:%X:%X:%X:%X:%X", tempHum, (unsigned int) lux, violet,
//					blue, green, yellow, orange, red);
//
//			/* Check how long the string of both message and submessage is */
//			uint16_t totLength = strlen(msg) + strlen(submsg);
//
//			/* Copy submessage to message */
//			strcat(msg, submsg);
//
//			/* If it is not the last day */
//			if (k != counterOfDataPerDay - 1)
//			{
//				/* If the total length is above 150 chars */
//				if (totLength > 150)
//				{
//					/* Publish the whole message to server with defined topic */
//					MqttPushPublish(SaraConfiguration.connectPubTopic, msg, 0,
//							0);
//
//					/* Reset whole message */
//					memset(msg, '\0', 256);
//
//					/* Reset sub message */
//					memset(submsg, '\0', 64);
//
//					/* Raise newPage flag */
//					newPage = 1;
//				}
//
//				/* If it is not above 150 */
//				else
//				{
//					/* Reset sub message */
//					memset(submsg, '\0', 64);
//				}
//			}
//
//			/* If it is the last message */
//			else
//			{
//				/* Publish it */
//				MqttPushPublish(SaraConfiguration.connectPubTopic, msg, 0, 0);
//			}
//		}
//		/* Remove one day from buffer */
//		Flash_data_subtractDay();
//
//		/* Set next address to write on to be the day you just sent */
//		if (i > 1)
//		{
//			Flash_SetNextAddress(FLASH_DATA_SETUP_ADDR, Flash_GetAddressDayX(i - 1));
//		}
//		else
//		{
//			Flash_SetNextAddress(FLASH_DATA_SETUP_ADDR, Flash_GetAddressDayX(i));
//		}
//
//
//	}
//
//	return true;
//}

///* Not used for SD-push */
//uint32_t MqttParsePublish(const char* topic, const char* message, uint8_t QoS, uint8_t retain)
//{
//	/* Init buffers and CRC */
//	char txBuff[256] = { 0 };
//	uint8_t subMsg[256] = { 0 };
//	uint32_t cCrc;
//
//	/* How long is the message? Do it need to be cut into smaller pieces? */
//	uint16_t strLenMsg = strlen(message);
//	uint8_t strLenTopic = strlen(topic);
//
//	/* Calculate how many messages is needed */
//	uint16_t totMsg = (strLenMsg / (255 - strLenTopic - 50)) + 1;
//	uint16_t countMsg = 1;
//
//	/* If the message needs to be cut into smaller pieces */
//	if (strLenMsg > 255 - strLenTopic - 50)
//	{
//		/* Loop through the whole message */
//		for (int i = 0, j = 0; i < strLenMsg; i++, j++)
//		{
//			/* If message is ready to be sent */
//			if (j > 255 - strLenTopic - 50)
//			{
//				/* Get CRC for submessage */
//				GetCRC(subMsg, &cCrc);
//
//				/* Copying to txBuff */
//				sprintf(
//						txBuff,
//						"AT+UMQTTC=2,%d,%d,\"%s\",\"%d/%d:%s:%X\"\r\n",
//						QoS,
//						retain,
//						topic,
//						countMsg,
//						totMsg,
//						subMsg,
//						(unsigned int) cCrc);
//
//				//printf("CRC: %X\r\n", (unsigned int) cCrc);
//
//				/* Sending submsg to server with CRC */
//				if (SaraCheckCommand(txBuff, "+UMQTTC: 2,1", 5 * DEFAULT_TIMEOUT))
//				{
//					/* Reset the subMsg buffer to be able to send more msg */
//					memset(subMsg, '\0', 256);
//
//					/* Resets subMsg counter */
//					j = 0;
//
//					/* Message counter + 1 */
//					countMsg++;
//				}
//			}
//			/* Assign the message in the submsg */
//			subMsg[j] = message[i];
//		}
//
//		/* If everything is sent, raise flag */
//		if (countMsg == totMsg)
//		{
//			/* Get CRC for last submessage */
//			GetCRC((uint8_t*) subMsg, &cCrc);
//
//			/* Copying to txBuff */
//			sprintf(
//					txBuff,
//					"AT+UMQTTC=2,%d,%d,\"%s\",\"%d/%d:%s:%X\"\r\n",
//					QoS,
//					retain,
//					topic,
//					countMsg,
//					totMsg,
//					subMsg,
//					(unsigned int) cCrc);
//
//			/* Send last submessage */
//			if (SaraCheckCommand(txBuff, "+UMQTTC: 2,1", 5 * DEFAULT_TIMEOUT))
//			{
//				/* Raise data sent flag */
//				comServer.dataSentToServerFlag = 1;
//				return true;
//			}
//		}
//	}
//
//	/* If the message is not needed to be cut into smaller pieces */
//	else
//	{
//		/* Get CRC for message */
//		GetCRC((uint8_t*) message, &cCrc);
//
//		/* Copying to txBuff */
//		sprintf(
//				txBuff,
//				"AT+UMQTTC=2,%d,%d,\"%s\",\"1/1:%s:%X\"\r\n",
//				QoS,
//				retain,
//				topic,
//				message,
//				(unsigned int) cCrc);
//
//		//printf("txBuff :       %s\r\n", txBuff);
//
//		/* Send and check if command got sent */
//		if (SaraCheckCommand(txBuff, "+UMQTTC: 2,1", 5 * DEFAULT_TIMEOUT))
//		{
//			/* Raise data sent flag */
//			comServer.dataSentToServerFlag = 1;
//			return true;
//		}
//	}
//	return false;
//}

/* Not used for Version 1 */
uint8_t MqttCheckRxData(uint8_t * buffRX)
{
	char *p;

	/* Debug */
	printf("BufferRead: %s\r\n", (char*) buffRX);

	/* Check if message has been received */
	p = strstr((char*) buffRX, "+UUMQTTCM:");

	/* If true, continue */
	if (p != NULL) {
		return true;
	}
	return false;
}

///*  Later USE of these two ( more for FW download)  */
//uint8_t MqttReturnPageRead(uint8_t *get, const char *startPattern,
//		const char *endPattern, const char *typeCommand)
//{
//	const char *iStartPattern = strstr((char*) get, startPattern);
//	if (iStartPattern != NULL)
//	{
//		const size_t placeStart = strlen(startPattern);
//		const char *iEndPattern = strstr(iStartPattern + placeStart,
//				endPattern);
//		if (iEndPattern != NULL)
//		{
//			const size_t messageLen = iEndPattern
//					- (iStartPattern + placeStart);
//			char ret[messageLen + 1];
//
//			if (ret != NULL)
//			{
//
//				memcpy(ret, iStartPattern + placeStart, messageLen);
//				ret[messageLen] = '\0';
//				printf("%s: %s, %d\r\n", typeCommand, (char*) ret,
//						(int) messageLen);
//				//pageMsg = ret;
//				printf("ReturnVal: %s\r\n", (char*) ret);
//
//				printf("pageMsg: %s\r\n.\r\n", (char*) ret);
//
//				uint32_t pageOf, pageTot;
//
//				if (2
//						== sscanf((char*) ret, "%d/%d", (int*) &pageOf,
//								(int*) &pageTot))
//				{
//					printf("page %d of %d\r\n", (int) pageOf, (int) pageTot);
//					comServer.messageReceived = (uint8_t) (pageOf & 0xFF);
//					comServer.messageTotal = (uint8_t) (pageTot & 0xFF);
//
//					char message[32] = { 0 };
//					sprintf(message, "%d/%d..OK", comServer.messageReceived,
//							comServer.messageTotal);
//					MqttPublish("testTT", message, 1, 0);
//
//					if (comServer.messageReceived == comServer.messageTotal)
//					{
//						comServer.prevMessageReceived = 0;
//						printf("PageOf == PageTot\r\n.\r\n");
//						return true;
//
//					}
//					printf("PageOf != PageTot\r\n.\r\n");
//					return false;
//				}
//			}
//		}
//	}
//	printf("not paired");
//	return false;
//
//}

/* Not used */
uint8_t MqttWaitForResponse(uint32_t timeout_sec)
{
  uint32_t loop = 0;
  uint32_t timeout_ms = timeout_sec * 1000;

	uint8_t processedBuffUart[256] = { 0 };
  char *p;

  while (loop < timeout_ms)
    {
      SaraWaitForResponse (processedBuffUart, 256, DEFAULT_TIMEOUT);

      printf ("BufferRead: %s\r\n", (char*) processedBuffUart);

      p = strstr ((char*) processedBuffUart, "+UUMQTTCM:");
      if (p != NULL)
	{
	  return true;
	}
      loop = loop + 1000;
    }
  return false;

}

/*  Later USE of these two ( more for FW download)  */
uint8_t MqttParseID(uint8_t *get, const char *startPattern,
 const char *endPattern)
{
  const char *iStartPattern = strstr ((char*) get, startPattern);
  if (iStartPattern != NULL)
    {
      const size_t placeStart = strlen (startPattern);
      const char *iEndPattern = strstr (iStartPattern + placeStart, endPattern);
      if (iEndPattern != NULL)
{
	  const size_t messageLen = iEndPattern - (iStartPattern + placeStart);
	  char ret[messageLen + 1];

	  if (ret != NULL)
	{


		memcpy (ret, iStartPattern + placeStart, messageLen);
	      ret[messageLen] = '\0';
	      printf ("ID: %s\r\n", (char*) ret);
	      printf ("ReturnVal: %s\r\n", (char*) ret);
	      return true;
	}
}
    }
  printf ("not paired");
  return false;
}

/*  Later USE of these two ( more for FW download)  */
uint8_t MqttParseMessage(uint8_t *get, const char *startPattern,
		  const char *endPattern)
{
  const char *iStartPattern = strstr ((char*) get, startPattern);
  if (iStartPattern != NULL)
    {
      const size_t placeStart = strlen (startPattern);
      const char *iEndPattern = strstr (iStartPattern + placeStart, endPattern);
      if (iEndPattern != NULL)
{
	  const size_t messageLen = iEndPattern - (iStartPattern + placeStart);
	  char ret[messageLen + 1];

	  if (ret != NULL)
	{
	      memcpy (ret, iStartPattern + placeStart, messageLen);
	      ret[messageLen] = '\0';
	      printf ("Message: %s\r\n", (char*) ret);
	      printf ("ReturnVal: %s\r\n", (char*) ret);
	      return true;

	}
}
    }
  printf ("not paired");
  return false;
}

/* Parses out the first JSON string that are in the buffer */
uint8_t MqttParseJSONMessage(uint8_t *get, const char *startPattern,
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

/* Parses out the first JSON CRC that are in the buffer */
uint8_t MqttParseJSONCRC(uint8_t *get, uint32_t *fCrc, const char *startPattern)
{
	/* Check if the startpattern appears in the string */
	const char *iStartPattern = strstr((char*) get, startPattern);

	/* if it appears */
	if (iStartPattern != NULL) {
		/* The place to satrt the reading of CRC will be at the END of startpattern */
		const size_t placeStart = strlen(startPattern);

		/* length of JSON will be 8, (length of CRC) */
		const size_t messageLen = 8;

		/* Assign buffer */
		char ret[messageLen + 1];

		if (ret != NULL) {
			/* Copy the message to buffer */
			memcpy(ret, iStartPattern + placeStart, messageLen);

			/* put an end sign to the buffer */
			ret[messageLen] = '\0';

			/* The CRC is written as string, convert to HEX */
			StringToHEX((char*) ret, 8, fCrc);

			/* Return true, CRC found*/
			return true;
		}
	}
	/* Return false, CRC not found */
	return false;
}

/* These are used for fething settings */
uint8_t FetchVerifyParseJSON(uint8_t *get, uint8_t *set, SaraConfig *s)
{
	/* init values needed */
	uint8_t retVal = false;
	uint32_t fCrc, cCrc;

	if (MqttParseJSONMessage(get, "{\"", "\"}", set))
	{
		/* Aquire the CRC from JSON message */
//		if (MqttParseJSONCRC(get, &fCrc, "\"}"))
//		{
			/* Compare CRC */
//			if (VerifyFile(set, &fCrc, &cCrc))
//			{
				/* Parse JSON into struct of choice */
				if (ParseSettingsString((char*) set, s))
				{
					/* Everything successful, JSON parsed into struct */
					retVal = true;
				}
//			}
//		}
	}
	return retVal;
}

uint8_t GetCRC(uint8_t *get, uint32_t *cCrc)
{
	uint32_t bSize = 0;

	/* Reset local CRC calculator */
	__HAL_CRC_DR_RESET(&hcrc);

	/* define size of string */
	bSize = strlen((char*) get);

	/* Calculated CRC-32/MPEG-2 for the buffer */
	*cCrc = HAL_CRC_Accumulate(&hcrc, (uint32_t*) get, bSize);

	return true;
}

uint8_t VerifyFile(uint8_t *get, uint32_t *fCrc, uint32_t *cCrc)
{
	uint32_t bSize = 0;

	/* Reset local CRC calculator */
	__HAL_CRC_DR_RESET(&hcrc);

	/* define size of string */
	bSize = strlen((char*) get);

	/* Calculated CRC for the entire application just flashed */
	*cCrc = HAL_CRC_Accumulate(&hcrc, (uint32_t*) get, bSize);

	/* If calculated CRC is the same as server CRC */
	if (*cCrc == *fCrc) {
		return true;
	}

	else /* Message corrupted */
	{
		return false;
	}
}

///* Not used, replaced by GetTime() */
//void AssignTimeStruct(Time *tS)
//{
//	RTC_DateTypeDef sDateATS;
//	RTC_TimeTypeDef timeStructATS;
//
//	HAL_RTC_GetTime(&hrtc, &timeStructATS, RTC_FORMAT_BIN);
//	HAL_RTC_GetDate(&hrtc, &sDateATS, RTC_FORMAT_BIN);
//
//	/* Assign to struct */
//	tS->Year = sDateATS.Year;
//	tS->Month = sDateATS.Month;
//	tS->Day = sDateATS.Date;
//	tS->Hour = timeStructATS.Hours;
//	tS->Minute = timeStructATS.Minutes;
//	tS->Second = timeStructATS.Seconds;
//}




/* Stages of Sara, executed from the main loop
 * :::::::::::::::::::::::::::::::::::::::::::
 * ___________________________________________
 *
 * Preparing with flow chart sessions */
//void SaraMqttFlowChart(uint16_t *stageSara)
//{
//  if (*stageSara == 0)
//    {
//      /* Starts up Sara, "bulletproof" design to secure boot up */
//      if (StartUpSara ())
//	{
//	  *stageSara = 1;
//	}
//    }
//
//	else if (*stageSara == 1)
//    {
//      /* Disables Echo, checks SIM status and enable radio LED (GPIO16) */
//      printf ("SaraInitAtCommands\r\n.\r\n");
//      if (SaraInitAtCommands ())
//	{
//	  *stageSara = 2;
//	}
//    }
//
//	else if (*stageSara == 2)
//    {
//      /* waiting for Network for 5 minutes, can be redone, right now it stops everything else for 5 minutes
//       * which is not ideal, there are another function that only checks once every loop. But very
//       * easily implemented to have "5 min deadline"
//       *
//       * ( SaraCheckCommand("AT+CEREG?\r\n", "+CEREG: 0,1OK", LOW_TIMEOUT) ) */
//      uint8_t timeout_minutes = 5;
//		printf("SaraWaitNetwork\r\n.\r\n"); //TODO
//      if (SaraWaitNetwork (timeout_minutes))
//	{
//	  *stageSara = 3;
//	}
//      else
//	{
//	  /* If network is not found in 5 min, go to sleep and try again in 1 hour */
//	  printf ("ERROR 1\r\n");
//	  ErrorTimerHourly (&startTimer, &radioErrors);
//	  while (!SaraPowerDown ())
//	{
//	    };
//	  HAL_Delay (10000);
//	  *stageSara = 0;
//	  /* Fail counter that reaches X and then sets timer
//	   *
//	   * Ex:
//	   * failCounter ++;
//	   * if (failCounter == 3)
//	   * {
//	   * 		Go to errorHandler
//	   * }*/
//	}
//
//    }
//
//	else if (*stageSara == 3)
//    {
//      printf ("SaraGetSignal\r\n.\r\n");
//      /* Checks if the max attempts has been reached */
//      if (radioErrors.testCounterGetSignal == MAX_FAILED_ATTEMPTS_GET_SIGNAL)
//	{
//
//	  printf ("ERROR 2\r\n");
//	  radioErrors.testCounterGetSignal = 0;
//	  ErrorTimerHourly (&startTimer, &radioErrors);
//	  while (!SaraPowerDown ())
//	{
//	    };
//	  HAL_Delay (10000);
//	  *stageSara = 0;
//
//	  /* Fail counter that reaches X and then sets timer */
//
//	  /* If counter x3 -> set timer "try again in 1h" */
//	}
//      /* Gets the signal strength of the network Sara is connected to */
//      else if (SaraGetSignalStrength ())
//	{
//	  /* Check signal strength to see if its good to continue */
//	  radioErrors.testCounterGetSignal = 0;
//	  if (SaraGetRSSI ())
//	    {
//	      *stageSara = 4;
//	    }
//	}
//      /* If the signal strength is bad or 99 (undefined) then try again after 2 seconds,
//       * if no signal been aquired during 5 tries, go to sleep and try again in 1 hour */
//      else
//	{
//	  radioErrors.testCounterGetSignal++;
//	  HAL_Delay (2000);
//	}
//
//    }
//
//	else if (*stageSara == 4)
//    {
//		//SaraCheckCommand("AT+CTZU=0\r\n", "OK", LOW_TIMEOUT);
//      /* Get the real time clock from the internet */
//      printf ("SaraGetClock\r\n.\r\n");
////      SARA_getRealTimeClock (timeBuffer);
//      *stageSara = 5;
//    }
//
//	else if (*stageSara == 5)
//    {
//      /* Compares the clock aquired from the network to the clock of the MCU,
//       * if it differs too much (MAX_SECOND_DIFFERENCE), adjust the time on
//       * the MCU to the Internet time */
//      printf ("SaraCompareClock\r\n.\r\n");
////      if (SaraCompareRealTimeClock (timeBuffer, &SaraLastConnected,
////				    &mcuClockTime))
////	{
////	  *stageSara = 6;
////	}
//      /* If it has failed to aquire the time more than MAX_FAILED_ATTEMPTS_CONNECTING,
//       * try again in 1 hour */
//		if (radioErrors.testCounterTime == MAX_FAILED_ATTEMPTS_CONNECTING)
//	{
//	  printf ("ERROR 2\r\n");
//	  radioErrors.testCounterTime = 0;
//	  ErrorTimerHourly (&startTimer, &radioErrors);
//	  while (!SaraPowerDown ())
//	{
//	    };
//			//HAL_Delay (10000);
//	  *stageSara = 0;
//	}
//      else
//	{
//	  radioErrors.testCounterTime++;
//	  *stageSara = 4;
//	}
//    }
//
//	else if (*stageSara == 6)
//    {
//      /* Configure the server that Sara should connect to,
//       * IP and port. */
//      printf ("SaraSetServer\r\n.\r\n");
//		if (MqttSetServer(SaraConfiguration.ip, 1883))
//	{
//	  *stageSara = 7;
//	}
//    }
//
//	else if (*stageSara == 7)
//    {
//      /* Test to connect to server, if it fails MAX_FAILED_ATTEMPTS_CONNECTING,
//       * then try again in 1 hour flag is raised */
//      if (radioErrors.testCounterConnectServer == MAX_FAILED_ATTEMPTS_CONNECTING)
//	{
//	  printf ("ERROR 3\r\n");
//	  radioErrors.testCounterConnectServer = 0;
//	  ErrorTimerHourly (&startTimer, &radioErrors);
//	  /* Connection to server failed */
//	  while (!SaraPowerDown ())
//	{
//	    };
//	  HAL_Delay (10000);
//	  *stageSara = 0;
//	}
//      /* Here it tries to connect, maybe rearrange so that this is first??
//       * saves 1 comparison/cycle */
//      else if (MqttConnect ())
//	{
//
//	  printf ("SaraConnectedToServer\r\n.\r\n");
//	  radioErrors.testCounterConnectServer = 0;
//	  *stageSara = 8;
//	}
//      else
//	{
//	  radioErrors.testCounterConnectServer++;
//	}
//    }
//
//
//  /* End of "Radio Flowchart", next is TX/RX flowchart */
//
//  /* These stages are still in testing... not sure which way they should be in. */
//
//	else if (*stageSara == 8)
//    {
//      /*  */
//		MqttPushDataToServer();
//		*stageSara = 9;
//
////		const char mess[] = "ejParsin";
////		char mess[TX_BUFF_SIZE] = { 0 };
////		//sprintf(mess, "%X,%X", (unsigned int) data.readData.timeStamp, data.readData.batteryLevel);
////		for (int i = 0; i < 300; i++)
////		{
////			if (i == 0)
////			{
////				sprintf(mess, "%d/300,%X,%X,%X,%X,%X,%X,%X,%X,%X", i, (unsigned int) data.readData.timeStamp, data.readData.batteryLevel, data.readData.sensData[i].tempHum,
////						data.readData.sensData[i].spectrum.blue, data.readData.sensData[i].spectrum.green, data.readData.sensData[i].spectrum.orange, data.readData.sensData[i].spectrum.violet,
////						data.readData.sensData[i].spectrum.yellow, data.readData.sensData[i].spectrum.red);
////			}
////			else
////			{
////			sprintf(mess, "%d/300,%X,%X,%X,%X,%X,%X,%X", i, data.readData.sensData[i].tempHum,
////					data.readData.sensData[i].spectrum.blue, data.readData.sensData[i].spectrum.green,
////					data.readData.sensData[i].spectrum.orange, data.readData.sensData[i].spectrum.violet, data.readData.sensData[i].spectrum.yellow,
////					data.readData.sensData[i].spectrum.red);
////			}
////			printf("mess: %d %s\r\n", i, mess);
////			MqttPushPublish("testTTj", mess, 0, 0);
////			memset(mess, '\0', 256);
////    }
////		sprintf(mess, "ejParsin");
////		sprintf(mess, "%08X,%02X,%02X,%04X,%04X,%04X,%04X,%04X,%04X,%04X", (unsigned int) data.readData.timeStamp, data.readData.batteryLevel, data.readData.sensData[0].lux,
////				data.readData.sensData[0].tempHum, data.readData.sensData[0].spectrum.blue, data.readData.sensData[0].spectrum.green, data.readData.sensData[0].spectrum.orange,
////				data.readData.sensData[0].spectrum.violet, data.readData.sensData[0].spectrum.yellow, data.readData.sensData[0].spectrum.red);
////		printf("%s\r\n", mess);
////      printf ("Trying to publish...\r\n.\r\n");
////		if (MqttParsePublish("testTTj", mess, 0, 0))
////
////		{
////			*stageSara = 9;
////	  }
////
////      else if (radioErrors.publishError == MAX_FAILED_ATTEMPTS_CONNECTING)
////	  {
////	  radioErrors.publishError = 0;
////	  ErrorTimerHourly (&startTimer, &radioErrors);
////	    /* publish to server failed */
////	    while (!SaraPowerDown ())
////	      {
////	      };
////	    HAL_Delay (10000);
////	    *stageSara = 0;
////
////	  }
////		else
////		{
////			radioErrors.publishError++;
////		}
//	}
//
//	else if (*stageSara == 9)
//	{
//		if (MqttSubscribe("testTTj", 0))
//		{
//			*stageSara = 10;
//		}
//		else if (MqttUnSubscribe("testTTj"))
//		{
//			*stageSara = 9;
//		}
//		else
//		{
//			radioErrors.subscribeError++;
//
//			if (radioErrors.subscribeError == MAX_FAILED_ATTEMPTS_CONNECTING)
//			{
//				radioErrors.subscribeError = 0;
//				ErrorTimerHourly(&startTimer, &radioErrors);
//				/* publish to server failed */
//				while (!SaraPowerDown())
//				{
//				};
//				HAL_Delay(10000);
//				*stageSara = 0;
//			}
//		}
//	}
//
//	else if (*stageSara == 10)
//      {
//		if (ServerDo(SaraConfiguration.connectSubTopic))
//		{
//			*stageSara = 11;
//		}
//		else
//		{
//			radioErrors.serverDo++;
//			if (radioErrors.serverDo == MAX_FAILED_ATTEMPTS_CONNECTING)
//			{
//				radioErrors.serverDo = 0;
//				ErrorTimerHourly(&startTimer, &radioErrors);
//				/* publish to server failed */
//				while (!SaraPowerDown())
//				{
//				};
//				HAL_Delay(10000);
//				*stageSara = 0;
//			}
//		}
////
////		if (ServerToDo(&SaraConfiguration))
////		{
////			radioErrors.noNewSettings = 0;
////			*stageSara = 11;
////		}
////		else
////		{
////			radioErrors.noNewSettings++;
////			if (radioErrors.noNewSettings == MAX_FAILED_ATTEMPTS_CONNECTING)
////			{
////				radioErrors.noNewSettings = 0;
////				//ErrorTimerHourly(&startTimer, &radioErrors);
////				while (!SaraPowerDown())
////				{
////				};
////				HAL_Delay(10000);
////				*stageSara = 0;
////			}
////		}
//	}
//
//
//
//
//
//
//
//    else if (*stageSara == 12)
//    {
//		uint8_t rxBuff[TX_BUFF_SIZE] =
//	{ 0 };
//		uint8_t JSONBuff[TX_BUFF_SIZE] =
//	{ 0 };
//      uint32_t fCrc, cCrc;
//      SaraSendCommand ("AT+UMQTTC=6\r\n");
//      SaraWaitForResponse (rxBuff, TX_BUFF_SIZE - 1, DEFAULT_TIMEOUT);
//
//
//	{
//	  MqttParseJSONMessage (rxBuff, "{\"", "\"}", JSONBuff);
//	  printf ("JSONMessage: %s\r\n", (char*) JSONBuff);
//	  MqttParseJSONCRC (rxBuff, &fCrc, "\"}");
//	  VerifyFile (JSONBuff, &fCrc, &cCrc);
//	  ParseSettingsString ((char*) JSONBuff, &SaraConfiguration);
//	  PrintSettings (&SaraConfiguration);
////			if (MqttParseMessage(rxBuff, "$$", "##"))
////			{
//	  MqttParseMessage (rxBuff, "$$", "##");
//	  MqttParseID (rxBuff, "!!", "@@");
//	  if (MqttReturnPageRead (rxBuff, "@@", "$$", "PageOF"))
//	    {
//	      printf ("Im in the Parse MSG looop \r\n");
//	      *stageSara = 0;
//	      while (!SaraPowerDown ())
//		{
//		};
//	      HAL_Delay (10000);
//	}
//	}
//    }
//
//  if (*stageSara == 10)
//    {
//      HAL_Delay (1000);
//    }
//
//}
//
//
//
//















/* ------------------------------------------------ NEVER USED ------------------------------------------------ */

//uint8_t MqttSetInactiveTimeout(uint16_t timeout)
//{
//	char txBuff[TX_BUFF_SIZE] =
//	{ 0 };
//
//	sprintf(txBuff, "AT+UMQTT=10,%d\r\n", timeout);
//	return SaraCheckCommand(txBuff, "+UMQTT: 10,1", DEFAULT_TIMEOUT);
//}
//
//uint8_t MqttClearSession(uint8_t clear)
//{
//	char txBuff[TX_BUFF_SIZE] =
//	{ 0 };
//
//	sprintf(txBuff, "AT+UMQTT=12,%d\r\n", clear);
//	return SaraCheckCommand(txBuff, "+UMQTT: 12,1", DEFAULT_TIMEOUT);
//}
//
///* Pings a server (IP-address)
// *
// * @server	- Server IP
// *
// * @return value: True/SUCCESS or False/FAILURE
// *  */
//uint8_t MqttPing(char* server)
//{
//	char txBuff[TX_BUFF_SIZE] =
//	{ 0 };
//
//	sprintf(txBuff, "AT+UMQTTC=8,\"%s\"\r\n", server);
//	return SaraCheckCommand(txBuff, "UMQTTC: 8,1", DEFAULT_TIMEOUT);
//}
//
//
///* Outdated */
//
//uint16_t *
//MqttReadCorrectData (uint8_t *get, const char *startPattern,
//		     const char *endPattern, const char *typeCommand)
//{
//  char * returnValue = 0;
//  const char *iStartPattern = strstr ((char*) get, startPattern);
//  if (iStartPattern != NULL)
//    {
//      const size_t placeStart = strlen (startPattern);
//      const char *iEndPattern = strstr (iStartPattern + placeStart, endPattern);
//      if (endPattern != NULL)
//	{
//	  const size_t messageLen = iEndPattern - (iStartPattern + placeStart);
//	  char *ret = malloc (messageLen + 1);
//
//	  if (ret != NULL)
//	    {
//
//	      memcpy (ret, iStartPattern + placeStart, messageLen);
//	      ret[messageLen] = '\0';
//	      printf ("%s: %s\r\n", typeCommand, ret);
//	      returnValue = ret;
//	      //strcpy(returnValue, ret);
//	      printf ("ReturnVal: %s\r\n", returnValue);
//	      free (ret);
//	      return (uint16_t*) returnValue;
//	    }
//	  free (ret);
//	}
//    }
//  return (uint16_t*) returnValue;
//}
//
///* Outdated */
//uint8_t
//MqttReadCorrectDataReturn (uint8_t *get, const char *startPattern,
//			   const char *endPattern, const char *typeCommand,
//			   char *returnValue)
//{
//
//  const char *iStartPattern = strstr ((char*) get, startPattern);
//  const size_t placeStart = strlen (startPattern);
//  const char *iEndPattern = strstr (iStartPattern + placeStart, endPattern);
//
//  if (iStartPattern == NULL || iEndPattern == NULL)
//    {
//      return false;
//    }
//
//  const size_t messageLen = iEndPattern - (iStartPattern + placeStart);
//  if (messageLen <= 0)
//    {
//      return false;
//    }
//
//  char *parsed = (char*) calloc (messageLen + 1, sizeof(char));
//  if (parsed == NULL)
//    {
//      return false;
//    }
//
//  memcpy (parsed, iStartPattern, messageLen);
//  *returnValue = *parsed;
//
//  printf ("Parsed: %s\r\n.\r\n", parsed);
//  free (parsed);
//
//  return true;
//
//}
//
//void
//MqttSaraStages (uint16_t *stageSara)
//{
//  if (*stageSara == 0)
//    {
//      /* Starts by throwing AT command to SARA */
//      SaraCheckCommand ("AT\r\n", "OK", LOW_TIMEOUT);
//      printf ("Im Heeeere\r\n");
//      *(stageSara) = 1;
//    }
//
//  else if (*stageSara == 1)
//    {
//      /* Sends "Echo off", expects answer OK */
//      if (SaraCheckCommand ("ATE0\r\n", "OK", LOW_TIMEOUT))
//	{
//	  SaraCheckCommand ("AT+UGPIOC=16,2\r\n", "OK", LOW_TIMEOUT);
//	  *stageSara = 2;
//	  printf ("Echo Turned OFF\r\n");
//	  printf (".\r\n");
//	}
//    }
//
//  else if (*stageSara == 2)
//    {
//      /* Checks if Sara is connected to internet (0,1) */
//      if (SaraCheckCommand ("AT+CEREG?\r\n", "+CEREG: 0,1OK", LOW_TIMEOUT))
//	{
//	  /* Network found */
//	  char timeBuffer[64] =
//	    { 0 };
//	  *stageSara = 3;
//	  printf ("Network found!\r\n");
//
//	  /* Check status of COPS (operator etc) and CIND (see quality of signal etc) */
//	  SaraCheckCommand ("AT+COPS?\r\n", "+COPS: 0,0,\"Telia Telia\",9OK",
//	  LOW_TIMEOUT);
//	  SaraCheckCommand ("AT+CIND?\r\n", "+CIND: 5,4,1,0,0,1,0,0,1,0,0,2OK",
//	  LOW_TIMEOUT);
//
//	  /* Prints the real time clock (GMT +0) */
//	  SARA_getRealTimeClock (timeBuffer);
//	  printf ("Time: %s\r\n", timeBuffer);
//	  printf (".\r\n");
//
//	  //SaraCompareRealTimeClock(timeBuffer);
//
//	  SaraGetSignalStrength ();
//
//	}
//    }
//
//  else if (*stageSara == 3)
//    {
//      /* Sets the server that should be connected towards, both with Publish and Subscribe */
////      if (MqttSetServer (serverRaPi, 1883))
////	{
//////			printf("Time is (myParse) in MAINLOOP: %d/%d/%d,%d:%d:%d\r\n", year,
//////					month, day, hour, minute, second);
////	  printf ("Server Set!\r\n");
////	  *stageSara = 4;
////	}
//    }
//
//  else if (*stageSara == 4)
//    {
//      /* Trying to connect to the server using MQTT, once connected, both subscribe and publish can be used */
//      printf ("Trying to connect...\r\n");
//      if (MqttConnect ())
//	{
//	  printf ("Connected!\r\n");
//	  *stageSara = 5;
//	}
//    }
//
//  else if (*stageSara == 5)
//    {
//      /* Publishes a string to the chosen topic, good to have a subscriber to that topic so that one can debug the message */
//      printf ("publishing to testTT\r\n");
////MqttUnSubscribe("testTT");
//      if (MqttParsePublish ("testTT", "EEESssssss", 0, 0))
//	{
//	  *stageSara = 7;
//	}
//    }
//
//  else if (*stageSara == 6)
//    {
////		if (MqttCheckRxData()) {
////			uint8_t rxBuff[256] =
////			{ 0 };
////
////			SaraSendCommand("AT+UMQTTC=6\r\n");
////			SaraWaitForResponse(rxBuff, 256, 5 * DEFAULT_TIMEOUT);
////			//uint16_t readCMD = MqttReadCorrectData(rxBuff, "PP", "QQ",
////			//		"Command");
////			//printf("Test Done!\r\n");
////			//HAL_Delay(100);
////			*stageSara = 7;
////		}
//    }
//  else if (*stageSara == 7)
//    {
//      HAL_Delay (1000);
//    }
//}
