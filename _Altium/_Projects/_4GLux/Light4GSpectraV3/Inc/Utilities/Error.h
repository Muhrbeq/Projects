/*
 * Error.h
 *
 *  Created on: 17 Oct 2018
 *      Author: andreas.fredlund
 */

#ifndef ERROR_H_
#define ERROR_H_

#include "Global.h"



void ErrorStruct_init(Error *er);
void ErrorAddLog(uint8_t function, uint8_t subFunction, uint8_t type, uint8_t status, uint32_t valueA, uint32_t valueB, uint8_t logLevel, Error *er);
void ErrorPrintLog(Error *er);
uint32_t ErrorGetSize(Error *er);

void ErrorTimerHourly(struct errorTimer *sTimer, struct radioFlags *rFlags);
void ErrorTimerHourlyCheck(struct errorTimer *sTimer, struct errorTimer *eTimer,
		struct radioFlags *rFlags);
void InitInfoStruct(Info *iL);
void ResetInfoStruct(Info *iL);
void InfoLogAdd(ErrorLevel errorLevel, uint8_t function, uint8_t subFunction, uint8_t type,
		uint8_t status, uint8_t valueA, uint16_t valueB, uint32_t valueC, Info *iL);
uint32_t InfoGetSize(Info *in);
void InfoPrintLog(Info *in);
void CopyTempToInfo(void);
#endif /* ERROR_H_ */
