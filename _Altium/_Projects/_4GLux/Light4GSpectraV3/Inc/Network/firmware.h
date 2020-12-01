/*
 * firmware.h
 *
 *  Created on: 10 jan. 2019
 *      Author: Rasmus.Muhrbeck
 */

#ifndef FIRMWARE_H_
#define FIRMWARE_H_

#include "Global.h"

uint8_t CopySettings(SaraConfig *to, SaraConfig *from);
uint8_t ServerMessage(uint8_t *rxBuff);
uint8_t ServerDo(char *topic);

/*
uint8_t NewFirmware(SaraConfig *s);
uint8_t DownloadFirmware(void);
uint8_t NewSettings(SaraConfig *s);
uint8_t DownloadSettings(uint8_t *get);
uint8_t ServerToDo(SaraConfig *s);
 */


#endif /* FIRMWARE_H_ */
