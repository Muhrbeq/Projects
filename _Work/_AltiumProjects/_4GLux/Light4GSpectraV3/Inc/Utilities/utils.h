/*
 * utils.h
 *
 *  Created on: 10 jan. 2019
 *      Author: Rasmus.Muhrbeck
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "Global.h"

#define BitSet(arg , posn) ((arg) |= (1L << (posn)))
#define BitClr(arg , posn) ((arg) &= ~(1L << (posn)))
#define BitTst(arg , posn) BOOL((arg) & (1L << (posn)))
#define BitFlp(arg , posn) ((arg) ^= (1L << (posn)))
#define BOOL(x) (!(!(x)))

uint8_t
CharToHex(char in, uint8_t *get);
uint8_t
StringToHEX(char *str, int len, uint32_t *retVal);
void InitEverything(void);



//void InitStages(SenRadStages *s);

#endif /* UTILS_H_ */
