/*
 * SDCard.h
 *
 *  Created on: 18 mars 2019
 *      Author: Rasmus.Muhrbeck
 */

#ifndef SDCARD_H_
#define SDCARD_H_

/*
 * Defines for the sd-card
 */
#define SD_MAX_FILE_ID				99  	// filename limited to 8 characters. Error_ is already 6 characters. Sens_ is 5. (3 character number wouldn't work)
#define SD_MAX_FILE_SIZE_ERROR		500		//TODO  to be decided. Applicable for both errorlog and sensordata?
#define SD_MAX_FILE_SIZE_DATA		sizeof(ReadingData)
/* Defines for the SD card folders */
#define TRANSMIT_PATH 		"transmit"
#define ERROR_PATH			"error"
#define INFO_PATH			"info"

uint8_t SD_Verify(FIL USERFile, char* filename, uint32_t ram_address, uint32_t size);
uint8_t SD_set_timestamp(char *filename);
uint8_t SD_EraseFile(char *filename);
uint8_t SD_EraseMultipleFiles(char *filename);

uint8_t SD_Init(void);
void SD_DeInit(void);
uint8_t SD_getFileName(char *filename, uint16_t maxFileSize);
uint8_t SD_getNumFiles(char *filename, FILINFO* fno);
uint8_t SD_DoFileExist(uint8_t i, char *filename, FILINFO* fno);

uint8_t SDReadAndSendPackets(char* filename);
void ArchiveFiles(void);
void ArchivePATHFile(char *fname, char* PATH);
uint8_t SDwriteErrorLog(Error *er);
uint8_t FindFile(void);
void FindAndSendFile(void);

uint8_t SDReadAndSend(char* filename);
uint8_t SDWriteChars(char* filename);
uint8_t SDWriteSensorData(Data *data);
void InitFoldersSD(void);

/* NOT USED */
uint8_t SD_write_ErrorLog(Error *er);
uint8_t SD_write_SensorData(Data *data);
uint8_t SD_WriteInfoLog(Info *info);
uint8_t SD_Write(FIL USERFile, char* filename, uint32_t ram_address, uint32_t size);
uint8_t SD_Read(FIL USERFile, char* filename, uint32_t ram_address, uint32_t size);
uint8_t SD_ReadFiles(char *filename, uint32_t ram_addr);
void SD_print_file(FIL USERFile, char* filename);
void EnableSD_Card(void);
uint8_t FindInfoFile(void);
void ArchiveInfoFilesExceptNewest(char* exceptFile);


#endif /* SDCARD_H_ */
