/*
 * SdCard.h
 *
 *  Created on: 5 jan. 2021
 *      Author: rasmu
 */

#ifndef SDCARD_H_
#define SDCARD_H_

#include "main.h"

#define SD_MAX_FILE_ID				99  	// filename limited to 8 characters. Error_ is already 6 characters. Sens_ is 5. (3 character number wouldn't work)


#define TRANSMIT_PATH 		"transmit"
#define ERROR_PATH			"error"
#define INFO_PATH			"info"

void EnableSD_Card(void);
void SD_DeInit(void);
void InitFoldersSD(void);
void ArchiveFiles(void);
uint8_t FindFile(void);
void ArchiveInfoFilesExceptNewest(char* exceptFile);
uint8_t FindInfoFile(void);
void ArchiveSensorFilesExceptNewest(char* exceptFile);

#endif /* SDCARD_H_ */
