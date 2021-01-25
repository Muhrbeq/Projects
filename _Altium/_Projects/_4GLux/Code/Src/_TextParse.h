/*
 * _TextParse.h
 *
 *  Created on: 25 jan. 2021
 *      Author: rasmu
 */

#ifndef TEXTPARSE_H_
#define TEXTPARSE_H_

#include "__ExegerGeneric.h"
#include "_Global.h"

uint8_t
CharToHex(char in, uint8_t *get);
uint8_t
StringToHEX(char *str, int len, uint32_t *retVal);

#endif /* TEXTPARSE_H_ */
