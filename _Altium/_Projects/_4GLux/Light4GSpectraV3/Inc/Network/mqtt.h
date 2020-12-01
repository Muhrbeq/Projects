/*
 * mqtt.h
 *
 *  Created on: 4 dec. 2018
 *      Author: rasmus.muhrbeck
 */

#ifndef MQTT_H_
#define MQTT_H_

#include "Global.h"

/* Functions */
/*__________________________________________________________*/

/* ID and authentication (not used atm) */
/*__________________________________________________________*/
uint8_t MqttSetClientID(char* clientID);
uint8_t MqttSetAuthentication(char* userName, char* passwd);

/* Will message (not used atm) */
/*__________________________________________________________*/
uint8_t MqttSetWill(char* topic, char* message, uint8_t QoS, uint8_t retain);
uint8_t MqttClearWill();

/* Profile functions */
/*__________________________________________________________*/
uint8_t MqttSaveProfile();
uint8_t MqttRestoreProfile();
uint8_t MqttDeleteProfile();

/* Connect/Disconnect to server (standard Publish/Subscribe)*/
/*__________________________________________________________*/
uint8_t MqttSetServer(char* server, uint16_t port);
uint8_t MqttConnect();
uint8_t MqttSetHttp(char *server);
uint8_t MqttPublish(const char* topic, const char* message, uint8_t QoS,
	     uint8_t retain);
uint8_t MqttPublishFile(uint8_t QoS, uint8_t retain, const char* topic, const char* filename);
uint8_t MqttSubscribe(char* topic, uint8_t QoS);
uint8_t MqttUnSubscribe(char* topic);
uint8_t MqttDisconnect();
uint8_t MqttPublishFile(uint8_t QoS, uint8_t retain, const char* topic,
		const char* filename);

/* Send/Receive message (modified)*/
/*__________________________________________________________*/
uint8_t MqttPushPublish(const char* topic, const char* message, uint8_t QoS, uint8_t retain);
uint8_t MqttPushPublishSD(const char* topic, const char* message, uint8_t QoS, uint8_t retain);
uint8_t MqttPushDataToServer();
void ExtractOneMeasurementFromFlash(uint32_t flashAddress, uint16_t *tempHum, uint32_t *lux, uint16_t *violet, uint16_t *blue, uint16_t *green, uint16_t *yellow, uint16_t *orange, uint16_t *red);
uint8_t MqttPushDataToServerFlash(uint8_t amountOfDays);
uint32_t MqttParsePublish(const char* topic, const char* message, uint8_t QoS,
		  uint8_t retain);
uint8_t MqttCheckRxData(uint8_t * buffRX);
uint8_t MqttReturnPageRead(uint8_t *get, const char *startPattern,
		    const char *endPattern, const char *typeCommand);

/* Not sure if necessary */
/*__________________________________________________________*/
uint8_t MqttWaitForResponse(uint32_t timeout_sec);

/* Parse functions */
/*__________________________________________________________*/
uint8_t MqttParseID(uint8_t *get, const char *startPattern,
		const char *endPattern);
uint8_t MqttParseMessage(uint8_t *get, const char *startPattern,
		const char *endPattern);
uint8_t MqttParseJSONMessage(uint8_t *get, const char *startPattern,
		const char *endPattern, uint8_t *set);
uint8_t MqttParseJSONCRC(uint8_t *get, uint32_t *fCrc, const char *startPattern);

/* CRC verification of received message */
/*__________________________________________________________*/
uint8_t FetchVerifyParseJSON(uint8_t *get, uint8_t *set, SaraConfig *s);
uint8_t GetCRC(uint8_t *get, uint32_t *cCrc);
uint8_t VerifyFile(uint8_t *get, uint32_t *fCrc, uint32_t *cCrc);

void AssignTimeStruct(Time *tS);



/* Flowchart */
/*__________________________________________________________*/
void
SaraMqttFlowChart(uint16_t *stageSara);



/* Not used */
uint8_t MqttSetInactiveTimeout(uint16_t timeout);
uint8_t MqttClearSession(uint8_t clear);
uint8_t MqttPing(char* server);

/* Outdated */
void MqttSaraStages(uint16_t *stageSara);
uint16_t * MqttReadCorrectData(uint8_t *get, const char *startPattern,
		const char *endPattern, const char *typeCommand);
uint8_t MqttReadCorrectDataReturn(uint8_t *get, const char *startPattern,
		const char *endPattern, const char *typeCommand, char *returnValue);



#endif /* MQTT_H_ */
