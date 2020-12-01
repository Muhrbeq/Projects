/*
 * sara.h
 *
 *  Created on: 26 nov. 2018
 *      Author: Rasmus.Muhrbeck
 */

#ifndef SARA_H_
#define SARA_H_

#include "Global.h"

/*
 * Defines for Sara
 */
#define TX_BUFF_SIZE				256

#define DEFAULT_TIMEOUT				1000
#define LOW_TIMEOUT 				10
#define POWEROFF_TIMEOUT			5
#define MAXIMUM_WAIT_FOR_NETWORK	300

#define MAX_SECOND_DIFFERENCE		15

#define MAX_FAILED_ATTEMPTS					3
#define MAX_FAILED_ATTEMPTS_GET_SIGNAL		15
#define MAX_FAILED_ATTEMPTS_SERVER			10
#define MAX_FAILED_ATTEMPTS_CONNECTING		3
#define MAX_FAILED_ATTEMPTS_HOURS 			3
#define MAX_FAILED_ATTEMPTS_DAYS 			5
#define TIMEOUT_BEFORE_START_AGAIN 			3600

//extern uint8_t tmpStage;

typedef enum Socket_type
{
  CLOSED = 0, TCP = 6, UDP = 17
} Socket_type;



/* Functions */
/*______________________________________________________*/

/* USART IRQ */
void USART2_Callback(void);

/* Sara Startup */
uint8_t SaraIsAlive();
uint8_t StartUpSara();
uint8_t SaraInitAtCommands();

/* Sara functions */
uint8_t SaraPowerDown(void);
uint8_t SaraDisableEchoMode();
uint8_t SaraCheckSimStatus();
uint8_t SaraSetCFUN(uint8_t mode);
uint8_t SaraReboot();

/* Sara Commands */
uint16_t SaraWaitForResponse(uint8_t *get, uint16_t count, uint32_t timeout_ms);
void SaraSendCommand(const char *command);
uint8_t SaraCheckCommand(const char *sendCommand, const char *receiveCommand, uint32_t timeout_ms);

/* Sara Utility commands that are good to know for MCU */
void SARA_getRealTimeClock(char *time);
uint8_t GMTcalculator(int *year, int *month, int *day, int *hour, int *minute, int *second, int *GMT, Time *timeSaraLastConnected);
uint8_t SaraCompareRealTimeClock(char *time, Time *timeSaraLastConnected, Time *mcuClock);
//int32_t RTC_GetElapsedSeconds(struct Time *StartTime, struct Time *EndTime);
uint8_t SaraGetRSSI();
uint8_t SaraGetSignalStrength();

void InitSaraStruct(SaraConfig *s);
void InitErrorStruct(struct radioFlags *rF);
void InitTimeStruct(Time *tS);



/* NOT used 
 * ________________________________________*/

/* Sara Socket commands (not used because MQTT) */
uint8_t SaraCreateSocket(Socket_type sockType);
uint8_t SaraSocketConnect(uint8_t sockID, char *hostIP, uint16_t port);
uint8_t SaraSocketClose(uint8_t sockID);
uint8_t SaraPowerDown(void);
uint8_t SaraSocketWriteData(uint8_t sockID, char *data, uint16_t dataSize);
void SaraSocketConnect2(const char *sendCommand);
uint8_t SaraSocketCheckRxData();

/* Test functions */
void SaraStartUpSequence(uint8_t *stageOfStartUp);
void SaraClearBuffer();
void TestSara();

/* Sara Network commands */
uint8_t SaraWaitForNetworkRegistered(uint32_t timeout_sec);
uint8_t SaraWaitNetwork(uint32_t timeoutMinutes);

#endif /* SARA_H_ */
