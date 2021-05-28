/*
 * Settings.h
 *
 *  Created on: Nov 20, 2012
 *      Author: Daniel
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Jsmn.h"
#include "Global.h"

typedef enum
{
	TOK_IP = 0,
	TOK_REVISION,
	TOK_PORT,
	TOK_ID,
	TOK_SERVERTODO,
	TOK_UPDATEFREQ,
	TOK_HOURTOSEND,
	TOK_NO_HEADER,
	TOK_IGNORE,
	TOK_LASTMESSAGE,
	TOK_FIRSTMESSAGE,

} TOKHeader;

uint8_t ParseSettingsString(char *set, SaraConfig *s);

uint8_t SetServerToDo(char *data, int len, SaraConfig *s);
uint8_t SetRevision(char *data, int len, SaraConfig *s);
uint8_t SetUnitID(char *data, int len, SaraConfig *s);
uint8_t SetIP(char *data, int len, SaraConfig *s);
uint8_t SetPORT(char *data, int len, SaraConfig *s);
uint8_t SetUpdateFrequency(char *data, int len, SaraConfig *s);
uint8_t SetHourToSend(char *data, int len, SaraConfig *s);
uint8_t FirstMessageToResend(char *data, int len, SaraConfig *s);

/* Not used, only for testing */
uint8_t PrintSettings(SaraConfig *s);
void TestSetting(void);


#endif /* SETTINGS_H_ */
