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

typedef enum SD_States
{
	SD_STARTUP,
	SD_INIT,
	SD_INITFOLDERSONSD,
	SD_WRITEDATA,
	SD_WRITEINFO,
	SD_ARCHIVE,
	SD_READANDSENDDATA,
	SD_SENDDATA,
	SD_UNMOUNT,
	SD_IDLE,
}SD_States;

typedef enum SD_Folder
{
	SD_FolderInitiated,
	SD_FolderNotInitiated,
}SD_Folder;

SD_States SD_GetState(void);
void SD_SetState(SD_States sState);


void EnableSD_Card(void);
void SD_DeInit(void);
void InitFoldersSD(void);
void ArchiveFiles(void);
uint8_t FindFile(void);
void ArchiveInfoFilesExceptNewest(char* exceptFile);
uint8_t FindInfoFile(void);
void ArchiveSensorFilesExceptNewest(char* exceptFile);

#endif /* SDCARD_H_ */
