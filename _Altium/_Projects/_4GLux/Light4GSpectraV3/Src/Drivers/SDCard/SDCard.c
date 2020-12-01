/*
 * SDCard.c
 *
 *  Created on: 18 mars 2019
 *      Author: Rasmus.Muhrbeck
 */


#include "Global.h"

#include "Error.h"
#include "MultiSensor.h"
#include "TimeFunc.h"
#include "ff.h"
#include "mqtt.h"
#include "sara.h"
#include "LowLevel.h"

#include "SDCard.h"

/*
 * SD Globals
 */
extern char sd_fileName[13];


/* Creates the folders "Errors" and "Transmit" and "Info" */
void InitFoldersSD(void)
{
	SD_Init();

	FILINFO fno;
	/* Check if archive folder exists, if not create */
	if (f_stat(TRANSMIT_PATH, &fno) == FR_NO_FILE)
	{
		/* Create Folder */
		f_mkdir(TRANSMIT_PATH);
	}

	/* Check if archive folder exists, if not create */
	if (f_stat(ERROR_PATH, &fno) == FR_NO_FILE)
	{
		/* Create Folder */
		f_mkdir(ERROR_PATH);
	}

	if (f_stat(INFO_PATH, &fno) == FR_NO_FILE)
	{
		/* Create Folder */
		f_mkdir(INFO_PATH);
	}
}

/*
 * Checks if a file contains the same data as the data on address 'ram_address' and 'size' bytes forward.
 * The function does not look at the whole file. Just from the end of the file and 'size' steps back. That is, the latest stored data.
 */
uint8_t SD_Verify(FIL userfile, char* filename, uint32_t ram_address, uint32_t size)
{
	uint32_t total_bytesread = 0;
	uint32_t temp_bytesread = 0;
	uint32_t data_temp[256 / (sizeof(uint32_t))];		// 256 bytes
	uint32_t temp_addr = (uint32_t) &data_temp;

	if (SD_Init() != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		if (f_open(&SDFile, filename, FA_READ) != FR_OK)
		{
			ErrorAddLog(
					LOG_FUNC_SD,
					LOG_SUB_READ,
					LOG_TYPE_OPEN,
					LOG_STATUS_FAIL,
					0,
					0,
					0,
					GetErrorStruct());
			return HAL_ERROR;
		}
		else
		{
			/* Want to start reading the data that we just wrote. Not from the total beginning. Sets write pointer to the end of the file - size */
			if (f_lseek(&SDFile, f_size(&userfile) - size) != FR_OK)
			{
				ErrorAddLog(
						LOG_FUNC_SD,
						LOG_SUB_VERIFY,
						LOG_TYPE_SEEK,
						LOG_STATUS_FAIL,
						1,
						0,
						0,
						GetErrorStruct());
				return HAL_ERROR;
			}

			else
			{
				/* The reading is divided into smaller parts just to not flush the RAM-memory. It reads and compares 256 bytes in every loop  */
				for (total_bytesread = 0; total_bytesread < size; total_bytesread += temp_bytesread)
				{
					/* Reads maximum 256 bytes (sizeof(data_temp)) of the file and stores the data in data_temp. The size that it has read is stored in temp_bytesread  */
					if (f_read(
							&SDFile,
							(uint32_t*) temp_addr,
							sizeof(data_temp),
							(UINT*) &temp_bytesread) != FR_OK)
					{
						ErrorAddLog(
								LOG_FUNC_SD,
								LOG_SUB_READ,
								LOG_TYPE_READ,
								LOG_STATUS_FAIL,
								0,
								0,
								0,
								GetErrorStruct());
						return HAL_ERROR;
					}
					else
					{
						/* Compares the data on SD with flash. If there is a difference -> error  */
						if (memcmp(
								(void*) &data_temp,
								(void*) (ram_address + total_bytesread),
								temp_bytesread) != 0)
						{
							ErrorAddLog(
									LOG_FUNC_SD,
									LOG_SUB_VERIFY,
									LOG_TYPE_COMPARE,
									LOG_STATUS_FAIL,
									1,
									0,
									0,
									GetErrorStruct());
							return HAL_ERROR;
						}
					}
				}
			}
			f_close(&SDFile);
		}
	}

	return HAL_OK;
}

/*
 * Erased file with name 'filename'
 */
uint8_t SD_EraseFile(char *filename)
{
	if (f_unlink(filename) != FR_OK)
	{
		ErrorAddLog(
				LOG_FUNC_SD,
				LOG_SUB_ERASE,
				LOG_TYPE_NONE,
				LOG_STATUS_FAIL,
				0,
				0,
				0,
				GetErrorStruct());
		return HAL_ERROR;
	}
	return HAL_OK;
}

/*
 * Erases all files that starts with 'filename'. Could erase Sens_1.txt, Sens_2.txt ..... Sens_80.txt for example.
 *
 */
uint8_t SD_EraseMultipleFiles(char *filename) // todo
{
	FILINFO fileinfo;
	uint8_t numFiles;
	uint16_t i;

	if (SD_Init() == HAL_OK)
	{
		numFiles = SD_getNumFiles(filename, &fileinfo);

		for (i = 1; i <= numFiles; i++)
		{
			sprintf(sd_fileName, filename, i);
			if (SD_EraseFile(sd_fileName) != HAL_OK)
			{
				return HAL_ERROR;
			}
		}
	}
	else
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

/*
 * Initialization
 */
uint8_t SD_Init(void)
{
	BSP_SD_Init();

	FRESULT res = f_mount(&SDFatFS, (TCHAR const*) SDPath, 1);

	printf("Mount result = %d\n\r", res);

	if (res != FR_OK)
	{
		ErrorAddLog(
				LOG_FUNC_SD,
				LOG_SUB_INIT,
				LOG_TYPE_MOUNT,
				LOG_STATUS_FAIL,
				0,
				0,
				0,
				GetErrorStruct());

		InfoLogAdd(ERRORLEVEL_WARNING,
		LOG_FUNC_SD,
		LOG_SUB_INIT,
		LOG_TYPE_MOUNT,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		return HAL_ERROR;
	}

	return HAL_OK;
}

/*
 * DeInitialization
 */
void SD_DeInit(void)
{
	f_mount(NULL, "", 0);

	FATFS_UnLinkDriver(SDPath);
}

/*
 * Sets the timestamp on the file
 */
uint8_t SD_set_timestamp(char *filename)
{
	FILINFO fileinfo;
	RTC_DateTypeDef sDate;
	RTC_TimeTypeDef timeStruct;

	HAL_RTC_GetTime(&hrtc, &timeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	fileinfo.fdate = (WORD) (((sDate.Year + 20) * 512U) | sDate.Month * 32U | sDate.Date);
	fileinfo.ftime = (WORD) (timeStruct.Hours * 2048U | timeStruct.Minutes * 32U
			| timeStruct.Seconds / 2U);

	if (f_utime(filename, &fileinfo) != FR_OK)
	{
		ErrorAddLog(LOG_FUNC_SD, LOG_SUB_WRITE, LOG_TYPE_TIMESTAMP,
		LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
		return HAL_ERROR;
	}

	return HAL_OK;
}

/*
 * 	Searches for a proper file to write to. They are named "filename_X.txt" where X is 'numFiles' so they are listed as
 * 	filename_1.txt, filename_2.txt and so on.
 * 	Stores the filename_X.txt in "sd_fileName".
 */
uint8_t SD_getFileName(char *filename, uint16_t maxFileSize)
{
	FILINFO fileinfo;
	uint8_t numFiles = SD_getNumFiles(filename, &fileinfo);

	if (numFiles == SD_MAX_FILE_ID)
	{
		printf("getFilename error\r\n");
		return HAL_ERROR;
	}
	else
	{
		// Checks if we can write to the last existing file. If it is too big we add 1 and later create a new file.
		if (fileinfo.fsize >= maxFileSize)
		//if (f_size(&sd_fileName) >= maxFileSize)
		{
			numFiles = numFiles + 1;
		}

		//numFiles = numFiles + 1;

		sprintf(sd_fileName, filename, numFiles);
	}

	return HAL_OK;
}

/*
 * Counts how many files that starts with 'filename' there is. Returns the number or SD_MAX_FILE_ID as error message and stores the fileinfo about the last
 * file in fno.
 */
uint8_t SD_getNumFiles(char *filename, FILINFO* fno)
{
	uint8_t i;

	for (i = 1; i < SD_MAX_FILE_ID; i++)
	{
		if (SD_DoFileExist(i, filename, fno) == 0)
		{
			if (i == 1)
			{
				return i;
			}
			else
			{
				return i - 1;
			}
		}
	}

	ErrorAddLog(
			LOG_FUNC_SD,
			LOG_SUB_SD_GETNUM_FILES,
			LOG_TYPE_NONE,
			LOG_STATUS_FAIL,
			i,
			0,
			0,
			GetErrorStruct());
	return SD_MAX_FILE_ID;
}

/*
 * Checks if a certain file exists
 */
uint8_t SD_DoFileExist(uint8_t fileID, char *filename, FILINFO* fno)
{
	char temp_fileName[13];
	sprintf(temp_fileName, filename, fileID);

	if (f_stat(temp_fileName, fno) == FR_NO_FILE)
	{
		return 0;
	}

	return 1;
}

void ArchivePATHFile(char *fname, char* PATH)
{
	FRESULT res;
	char archivePath[32] = { 0 };

	/* New path will be in 'old' folder */
	sprintf(archivePath, "%s/%s", PATH, fname);

	/* Rename an object and move it to another directory in the drive */
	res = f_rename(fname, archivePath);

	/* File already in folder, delete current one */
	if (res == FR_EXIST)
	{
		printf("File already in archive folder, delete current one in root\n\r");

		/* Delete file */
		f_unlink(fname);
	}
	else if (res == FR_OK)
	{
		printf("Programmed File %s moved to %s\n\r", fname, archivePath);
	}
	else
	{
		printf("res = %d\r\n", res);
	}
}

/* Finds if there is a date-file with sensor measurements to be sent */
uint8_t FindFile(void)
{
	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */
	uint8_t retVal = false;

	/* Start to search for sensor files */
	fr = f_findfirst(&dj, &fno, "", "*-*.txt");

	if (fr == FR_OK && fno.fname[0])
	{
		printf("FiLE: %s\r\n", fno.fname);
		printf("File Found\r\n");
		retVal = true;
	}

	f_closedir(&dj);

	printf("End of FindFile\r\n");
	return retVal;
}

uint8_t FindInfoFile(void)
{
	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */
	uint8_t retVal = false;

	/* Start to search for sensor files */
	fr = f_findfirst(&dj, &fno, "", "I*.txt");

	if (fr == FR_OK && fno.fname[0])
	{
		printf("FiLE: %s\r\n", fno.fname);
		printf("File Found\r\n");
		retVal = true;
	}

	f_closedir(&dj);

	printf("End of FindFile\r\n");
	return retVal;
}


void FindAndSendFile(void)
{
	radioFlags *rE = GetErrorFlagsStruct();

	rE->counterMessage = 1;

	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */

	fr = f_findfirst(&dj, &fno, "", "*-*.txt"); /* Start to search for photo files */

	printf("FileName: %s\r\n", fno.fname);

	if (fr == FR_OK && fno.fname[0])
	{
		/* Send the file  */
		SDReadAndSendPackets(fno.fname);
	}

	f_closedir(&dj);
}

void ArchiveFiles(void)
{
	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */

	printf("Trying to archive\r\n");

	fr = f_findfirst(&dj, &fno, "", "*-*.txt"); /* Start to search for photo files */

	if (fr == FR_OK)
	{
		/* Archive it */
		printf("File to Archive: %s\r\n", fno.fname);
		ArchivePATHFile(fno.fname, TRANSMIT_PATH);
		printf("File Archived\r\n");
	}

	f_closedir(&dj);
}

void ArchiveInfoFilesExceptNewest(char* exceptFile)
{
	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */

	fr = f_findfirst(&dj, &fno, "", "I_*");

	while (fr == FR_OK && fno.fname[0])
	{
		if (strcmp(fno.fname, exceptFile) == 0)
		{
			fr = f_findnext(&dj, &fno);
		}
		else
		{
			ArchivePATHFile(fno.fname, INFO_PATH);

			fr = f_findnext(&dj, &fno);
		}
	}
}

/* Reads the filename on the SD card and sends the data to the server (MQTT) */
uint8_t SDReadAndSend(char* filename)
{
	SaraConfig *sC = GetSaraConfigurationStruct();

	radioFlags *rE = GetErrorFlagsStruct();

	FIL sendFile;
	char data[256] = { 0 };
	const char cC[16] = "&CHECKCOUNTER";

	/* Opne filename */
	if (f_open(&sendFile, filename, FA_READ) != FR_OK)
	{
		ErrorAddLog(LOG_FUNC_SD, LOG_SUB_READ, LOG_TYPE_OPEN,
		LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
		return HAL_ERROR;
	}
	else
	{

		/* Read one row (until \n char) and push it to server */
		while (f_gets(data, 256, &sendFile) != NULL)
		{

			/* Push to server */
			MqttPushPublishSD(sC->connectPubTopic, (const char*) data, 0, 0);

			printf("data: %s \r\n", data);

			/* Reset sendbuffer */
			memset(data, '\0', 256);

			rE->counterMessage++;
		}

		rE->counterMessage--;

		MqttPushPublishSD(sC->connectPubTopic, cC, 0, 0);

		/* Important to close the file */
		f_close(&sendFile);
	}

	return HAL_OK;
}

/* Reads the filename on the SD card and sends the data to the server (MQTT) */
uint8_t SDReadAndSendPackets(char* filename)
{
	SaraConfig *sC = GetSaraConfigurationStruct();

	radioFlags *rE = GetErrorFlagsStruct();

	FIL sendFile;
	char data[256] = { 0 };
	char dummyData[256] = { 0 };
	const char cC[16] = "&CHECKCOUNTER";

	/* Opne filename */
	if (f_open(&sendFile, filename, FA_READ) != FR_OK)
	{
		ErrorAddLog(LOG_FUNC_SD, LOG_SUB_READ, LOG_TYPE_OPEN,
		LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
		return HAL_ERROR;
	}

	else
	{

		if (sC->firstMessageToReSend != 1)
		{
			for (int i = 1; i < sC->firstMessageToReSend; i++)
			{
				f_gets(dummyData, 256, &sendFile);
				rE->counterMessage++;
			}
		}

		/* Read one row (until \n char) and push it to server */
		while (f_gets(data, 256, &sendFile) != NULL)
		{
			/* Push to server */
			MqttPushPublishSD(sC->connectPubTopic, (const char*) data, 0, 0);

			printf("data: %s \r\n", data);

			/* Reset sendbuffer */
			memset(data, '\0', 256);

			rE->counterMessage++;

			if (rE->counterMessage == sC->lastMessageToReSend)
			{
				break;
			}
		}

		/* Clear UART buffer after send */
		uint8_t rxBuff[256] = { 0 };
		SaraWaitForResponse(rxBuff, 256, 200);
		rE->counterMessage--;
//
		MqttPushPublishSD(sC->connectPubTopic, cC, 0, 0);

		sC->firstMessageToReSend = 1;
		sC->lastMessageToReSend = 1;

		/* Important to close the file */
		f_close(&sendFile);
	}

	return HAL_OK;
}

uint8_t SDWriteChars(char* filename)
{

	FIL charFile;
	UINT num;
	char BatTime[32] = { 0 };
	char spectra[256] = { 0 };
	char message[256] = { 0 };
	uint8_t timeBat = 1;

	Data *data = GetDataStruct();

	int sensCount = data->sensCounter;

	/* Open or create file */
	if (f_open(&charFile, filename, FA_OPEN_APPEND | FA_WRITE) != FR_OK)
	{
		ErrorAddLog(
				LOG_FUNC_SD,
				LOG_SUB_WRITE,
				LOG_TYPE_OPEN,
				LOG_STATUS_FAIL,
				1,
				0,
				0,
				GetErrorStruct());
	}
	else
	{
		for (int i = 0; i < sensCount; i++)
		{
			uint32_t lux;
			uint16_t tempHum;
			lux = (data->readData.sensData[i].tempHumLux >> 15);
			tempHum = (data->readData.sensData[i].tempHumLux & 0b111111111111111);

			/* Only want to add this the first cycle */
			if (timeBat)
			{
				/* Add time and battery */
				sprintf(
						BatTime,
						"&%X.%X",
						(unsigned int) data->readData.timeStamp,
						data->readData.batteryLevel);

				/* Add time and battery to start of message */
				strcat(message, BatTime);
				timeBat = 0;
			}
			/* Add sensor values to spectra array */
			sprintf(
					spectra,
					"*%X:%X:%X:%X:%X:%X:%X:%X",
					tempHum,
					(unsigned int) lux,
					data->readData.sensData[i].spectrum.violet,
					data->readData.sensData[i].spectrum.blue,
					data->readData.sensData[i].spectrum.green,
					data->readData.sensData[i].spectrum.yellow,
					data->readData.sensData[i].spectrum.orange,
					data->readData.sensData[i].spectrum.red);

			/* Calculate total length of string after adding them together */
			strcat(message, spectra);
			uint16_t totLength = strlen(message);

			if (i != sensCount - 1)
			{
				if (totLength > 130)
				{
					/* Add new line to ease the reading of the file */
					strcat(message, "\n");

					/* Write message to file */
					f_write(&charFile, (char*) message, strlen(message), (void *) &num);

					/* Reset both arrays */
					memset(message, '\0', 256);
					memset(spectra, '\0', 256);
				}
			}
			else
			{
				/* Add new line to ease the reading of the file */
				strcat(message, "\n");

				/* Write message to file */
				f_write(&charFile, (char*) message, strlen(message), (void *) &num);
			}
		}
//		}
		/* Critical to close the file. Else the data will be corrupted. */
		f_close(&charFile);
	}

	return HAL_OK;
}

void EnableSD_Card(void)
{
	MX_FATFS_Init();

	HAL_SD_MspInit(&hsd1);

	DisEnableSDPin(1);

	SleepForApproxMs(500);

	uint8_t ucounter = 0;

	while (SD_Init() != HAL_OK)
	{

		if (ucounter >= 5)
		{
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_SD,
			LOG_SUB_INIT,
			LOG_TYPE_MOUNT,
			LOG_STATUS_TIMEOUT, 0, 0, 0, GetTempInfoStruct());

			ErrorAddLog(
			LOG_FUNC_SD,
			LOG_SUB_INIT,
			LOG_TYPE_MOUNT,
			LOG_STATUS_TIMEOUT, 0, 0, 0, GetErrorStruct());

			break;
		}
		ucounter++;
	}
}

uint8_t SDWriteSensorData(Data *data)
{
	Time fileDate;
	char sensDate[16];

	GetTime(&fileDate);

	sprintf(sensDate, "%d-%02d-%02d.txt", fileDate.Year, fileDate.Month, fileDate.Day);

	/* Initialization */
	if (SD_Init() == HAL_OK)
	{
		/* Stores the filename in sd_filename */
		if (SD_getFileName(sensDate, SD_MAX_FILE_SIZE_DATA) == HAL_OK)
		{
			/* Opens or creates a file and writes the data. */
			if (SDWriteChars(sd_fileName) == HAL_OK)
			{
				/* Sets the date and time on the file. */
				if (SD_set_timestamp(sd_fileName) == HAL_OK)
				{
					/* Should reset the data_sensCounter here  */
					printf("Writing to file: %s \r\n", sd_fileName);

					data->sensCounter = 0;

					return HAL_OK;
				}
			}
		}
	}
	InfoLogAdd(ERRORLEVEL_WARNING,
	LOG_FUNC_SD,
	LOG_SUB_WRITE,
	LOG_TYPE_WRITE,
	LOG_STATUS_TIMEOUT, 0, 0, 0, GetTempInfoStruct());

	return HAL_ERROR;
}

uint8_t SD_WriteInfoLog(Info *info)
{
	Time fileDate;
	char infoDate[16];

	uint32_t size = InfoGetSize(GetInfoStruct());


	GetTime(&fileDate);

	sprintf(
			infoDate,
			"I_%d%02d%02d.TXT",
			fileDate.Year,
			fileDate.Month,
			fileDate.Day);

	if (SD_Init() == HAL_OK)
	{
		sprintf(sd_fileName, infoDate);
			/* Opens or creates a file and writes the data. */
		if (SD_Write(SDFile, sd_fileName, (uint32_t) &info->infoLog, size) == HAL_OK)
		{
			/* Checks if the written data on sd is the same as in RAM. */
			if (SD_Verify(SDFile, sd_fileName, (uint32_t) &info->infoLog, size) == HAL_OK)
			{
				/* Sets the date and time on the file. */
				if (SD_set_timestamp(sd_fileName) == HAL_OK)
				{
					printf("Writing to file: %s \r\n", sd_fileName);

					/* Archives */
					if (GetErrorFlagsStruct()->errorHourCounter == 2)
					{
						ArchiveInfoFilesExceptNewest(infoDate);
					}
					else if (GetErrorFlagsStruct()->everythingOK == 1)
					{
						ArchiveInfoFilesExceptNewest(infoDate);
					}

					return HAL_OK;
				}

			}
		}
	}

	return HAL_ERROR;

}

/* Write error struct to  */
uint8_t SDwriteErrorLog(Error *er)
{
	uint32_t size = ErrorGetSize(GetErrorStruct());

	radioFlags *rE = GetErrorFlagsStruct();

	Time fileDate;
	char errorDate[16];

	GetTime(&fileDate);

	sprintf(
			errorDate,
			"E_%d%02d%02d.txt",
			fileDate.Year,
			fileDate.Month,
			fileDate.Day);

	if (SD_Init() == HAL_OK)
	{
		/* Stores the filename in sd_filename */
		sprintf(sd_fileName, errorDate);
		/* Opens or creates a file and writes the data. */
		if (SD_Write(SDFile, sd_fileName, (uint32_t) &er->errorLog, size) == HAL_OK)
		{
			/* Checks if the written data on sd is the same as in RAM. */
			if (SD_Verify(SDFile, sd_fileName, (uint32_t) &er->errorLog, size) == HAL_OK)
			{
				/* Sets the date and time on the file. */
				if (SD_set_timestamp(sd_fileName) == HAL_OK)
				{
					printf("Writing to file: %s \r\n", sd_fileName);

					/* Resets the errorCounter after writong to SD. todo Maybe this should be done outside of this function  */
					GetErrorStruct()->errorCounter = 0;

					/* Archives */
					if (rE->errorDayCounter == 2)
					{
						ArchivePATHFile(errorDate, ERROR_PATH);
					}
					else if (rE->everythingOK == 1)
					{
						ArchivePATHFile(errorDate, ERROR_PATH);
					}
					return HAL_OK;
				}

			}
		}

	}

	return HAL_ERROR;
}


/*
 *	Writes data to SD to file 'filename' from address 'ram_address' to address 'ram_address' + 'size'.
 */
uint8_t SD_Write(FIL userfile, char* filename, uint32_t ram_address, uint32_t size)
{

	uint32_t byteswritten;

	/* Open or create file */
	if (f_open(&SDFile, filename, FA_OPEN_APPEND | FA_WRITE) != FR_OK)
	{
		ErrorAddLog(
				LOG_FUNC_SD,
				LOG_SUB_WRITE,
				LOG_TYPE_OPEN,
				LOG_STATUS_FAIL,
				1,
				0,
				0,
				GetErrorStruct());

		InfoLogAdd(ERRORLEVEL_WARNING,
		LOG_FUNC_SD,
		LOG_SUB_WRITE,
		LOG_TYPE_OPEN,
		LOG_STATUS_FAIL, 1, 0, 0, GetTempInfoStruct());
	}
	else
	{
		/* Writes the data */
		if (f_write(&SDFile, (uint32_t*) ram_address, size, (void *) &byteswritten) != FR_OK)
		{
			ErrorAddLog(
					LOG_FUNC_SD,
					LOG_SUB_WRITE,
					LOG_TYPE_WRITE,
					LOG_STATUS_FAIL,
					1,
					0,
					0,
					GetErrorStruct());

			InfoLogAdd(ERRORLEVEL_WARNING,
			LOG_FUNC_SD,
			LOG_SUB_WRITE,
			LOG_TYPE_WRITE,
			LOG_STATUS_FAIL, 1, 0, 0, GetTempInfoStruct());
			return HAL_ERROR;
		}
//		}
		/* Critical to close the file. Else the data will be corrupted. */
		f_close(&SDFile);
	}

	return HAL_OK;
}
/*
 *
 *
 *
 *
 * ___________________________________NOT USED___________________________________
 *
 *
 *
 *
 *
 * */

/*
 *	Writes the sensor data to a file. Return hal_ok for success. Else hel_error.
 *
 */
uint8_t SD_write_SensorData(Data *data)
{
	uint32_t size = GetDataSize(GetDataStruct());
	Time SensorDate;
	char sensDate[16];

	GetTime(&SensorDate);

	sprintf(sensDate, "%d-%02d-%02d.txt", SensorDate.Year, SensorDate.Month, SensorDate.Day);

	if (SD_Init() == HAL_OK)		// Initialization
	{
		if (SD_getFileName(sensDate, SD_MAX_FILE_SIZE_DATA) == HAL_OK)// Stores the filename in sd_filename
		{
			if (SDWriteChars(sd_fileName) == HAL_OK)	   // creates a new file and writes the data
			{
				if (SD_Verify(SDFile, sd_fileName, (uint32_t) &data->readData, size) == HAL_OK)	// checks if the written data on sd is the same as in RAM.
				{
					if (SD_set_timestamp(sd_fileName) == HAL_OK)// sets the date and time on the file.
					{
						/* Should reset the data_sensCounter here  */
						printf("Writing to file: %s \r\n", sd_fileName);

						return HAL_OK;
					}
				}
			}
		}
	}

	return HAL_ERROR;
}

/*
 *	Writes the errorlog to a file. Return hal_ok for success. Else hel_error.
 *
 */
uint8_t SD_write_ErrorLog(Error *er)
{
	uint32_t size = ErrorGetSize(GetErrorStruct());
	//HAL_GPIO_WritePin(SD_Power_EN_GPIO_Port, SD_Power_EN_Pin, GPIO_PIN_SET);
	if (SD_Init() == HAL_OK)
	{
		if (SD_getFileName("Error_%d.txt", SD_MAX_FILE_SIZE_ERROR) == HAL_OK)// Stores the filename in sd_filename
		{
			if (SD_Write(SDFile, sd_fileName, (uint32_t) &er->errorLog, size) == HAL_OK)// Opens or creates a file and writes the data.
			{
				if (SD_Verify(SDFile, sd_fileName, (uint32_t) &er->errorLog, size) == HAL_OK)// checks if the written data on sd is the same as in RAM.
				{
					if (SD_set_timestamp(sd_fileName) == HAL_OK)// sets the date and time on the file.
					{
						printf("Writing to file: %s \r\n", sd_fileName);

						/* Resets the errorCounter after writong to SD. todo Maybe this should be done outside of this function  */
						GetErrorStruct()->errorCounter = 0;

						return HAL_OK;
					}

				}
			}
		}
	}

	return HAL_ERROR;
}

uint8_t SdWriteFirmWare(uint8_t get)
{
	return true;
}

/*__________________________________________________*/
/*	Following functions are not in use right now 	*/

///*
// *  Prints the file in the terminal
// */
//void SD_print_file(FIL userfile, char* filename)
//{
//
//	FILINFO fileinfo;
//
//	if (f_stat(filename, &fileinfo) == FR_OK)
//	{
//		uint32_t size = fileinfo.fsize;
//		uint32_t data_temp[size / (sizeof(uint32_t))];
//		uint32_t temp_addr = (uint32_t) &data_temp;
//		uint16_t i = 0;
//
//		if (SD_Read(userfile, filename, temp_addr, size) == HAL_OK)
//		{
//			for (i = 0; i < size; i = i + 4)
//			{
//				printf("\t %lx \r\n", *(uint32_t*) temp_addr);
//				temp_addr = temp_addr + 4;
//			}
//		}
//	}
//
//	printf("\r\n");
//}
//
//
///*
// * todo Not in use right now but want to keep parts of it for later if we want to read from sd and send via sara.
// */
//uint8_t SD_ReadFiles(char *filename, uint32_t ram_addr)
//{
//	FILINFO fileinfo;
//	uint16_t i;
//	uint8_t numFiles;
//	char temp_fileName[13];
//
//	if (SD_Init() != HAL_OK)
//	{
//		return HAL_ERROR;
//	}
//	else
//	{
//		numFiles = SD_getNumFiles(filename, &fileinfo);
//
//		printf("Files on the sd-card: %d \r\n", numFiles);
//
//		//Data values;
//		//GetDataStruct()->sensCounter = 720; //GetDataStruct()->sensCounter;
//		for (i = 1; i <= numFiles; i++)
//		{
//			sprintf(temp_fileName, filename, i);		//sd_fileName
//			if (SD_Read(USERFile, temp_fileName, (uint32_t) &GetDataStruct()->readData, 0) == HAL_OK)
//			{
//				if (f_stat(temp_fileName, &fileinfo) == FR_OK)
//				{
//					for (int j = 0; j < 10; j++)
//					{
//						printf("%c", temp_fileName[j]);
//					}
//				}
//			}
//		}
//	}
//
//	return HAL_OK;
//}
//
///*
// * Reads a file from the sd-card and stores in in address 'ram_address'.
// * Note. 'Size' could be smaller than the file size without causing error. In that case it only reads the 'size' LAST bytes
// * in the file. If 'size' is the same size as the file (or bigger) it reads the whole file. Also if 'size' is set to zero it reads the whole file.
// */
//uint8_t SD_Read(FIL userfile, char* filename, uint32_t ram_address, uint32_t size)
//{
//	uint32_t bytesread;
//	FILINFO fileinfo;
//
//	if (SD_Init() != HAL_OK)
//	{
//		return HAL_ERROR;
//	}
//	else
//	{
//		if (f_stat(filename, &fileinfo) == FR_OK)
//		{
//			if (size > fileinfo.fsize || size == 0)		// the reading size can't be bigger than the file itself or zero
//			{
//				size = fileinfo.fsize;
//			}
//		}
//
//		if (f_open(&userfile, filename, FA_READ) != FR_OK)
//		{
//			ErrorAddLog(LOG_FUNC_SD, LOG_SUB_READ, LOG_TYPE_OPEN, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
//			return HAL_ERROR;
//		}
//		else
//		{
//			if (f_lseek(&userfile, fileinfo.fsize - size) != FR_OK)			// sets write pointer to the end of the file - size
//			{
//				ErrorAddLog(LOG_FUNC_SD, LOG_SUB_READ, LOG_TYPE_SEEK, LOG_STATUS_FAIL, 1, 0, 0, GetErrorStruct());
//				return HAL_ERROR;
//			}
//			else
//			{
//				if (f_read(&userfile, (uint32_t*) ram_address, size, (UINT*) &bytesread) != FR_OK)
//				{
//					ErrorAddLog(LOG_FUNC_SD, LOG_SUB_READ, LOG_TYPE_READ, LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
//					return HAL_ERROR;
//				}
//			}
//
//			f_close(&USERFile);
//		}
//	}
//
//	return HAL_OK;
//}
//
