/*
 * _SaraR4.h
 *
 *  Created on: 7 jan. 2021
 *      Author: rasmu
 */

#ifndef SARAR4_H_
#define SARAR4_H_

uint8_t SaraIsAlive();
uint8_t StartUpSara();
uint8_t SaraInitAtCommands();

uint8_t SaraPowerDown(void);
uint8_t SaraDisableEchoMode();
uint8_t SaraCheckSimStatus();
uint8_t SaraReboot();

/* Sara Commands */
uint16_t SaraWaitForResponse(uint8_t *get, uint16_t count, uint32_t timeout_ms);
void SaraSendCommand(const char *command);
uint8_t SaraCheckCommand(const char *sendCommand, const char *receiveCommand, uint32_t timeout_ms);


#endif /* SARAR4_H_ */
