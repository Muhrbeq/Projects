/*
 * SdCard.c
 *
 *  Created on: 5 jan. 2021
 *      Author: rasmu
 */

#include "__ExegerGeneric.h"
#include "_SDCard.h"
#include "_Global.h"
#include "_LowLevel.h"
#include "_Time.h"

#include "ff.h"
#include "bsp_driver_sd.h"
#include "fatfs.h"
#include "sdmmc.h"

#include "rtc.h"

extern char sd_fileName[13];

void EnableSD(uint8_t On)
{
	if(On != 0)
	{
		LL_GPIO_SetOutputPin(SD_Enable_GPIO_Port, SD_Enable_Pin);
		return;
	}
	LL_GPIO_ResetOutputPin(SD_Enable_GPIO_Port, SD_Enable_Pin);
}

void EnableSD_Card(void)
{
	MX_FATFS_Init();

	HAL_SD_MspInit(&hsd1);

	EnableSD(1);
//
	SleepMax2s(500);

	uint8_t ucounter = 0;

	while (SD_Init() != HAL_OK)
	{

		if (ucounter >= 5)
		{
			InfoLogAdd(ERRORLEVEL_FATAL,
			LOG_FUNC_SD,
			LOG_SUB_INIT,
			LOG_TYPE_MOUNT,
			LOG_STATUS_TIMEOUT, 0, 0, 0, GetTempInfoStruct());

			break;
		}
		ucounter++;
	}
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
//		ErrorAddLog(LOG_FUNC_SD, LOG_SUB_WRITE, LOG_TYPE_TIMESTAMP,
//		LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
		return HAL_ERROR;
	}

	return HAL_OK;
}

/*
 * 	Searches for a proper file to write to. They are named "filename_X.txt" where X is 'numFiles' so they are listed as
 * 	filename_1.txt, filename_2.txt and so on.
 * 	Stores the filename_X.txt in "sd_fileName".
 */
uint8_t SD_getFileName(char *filename)
{
	sprintf(sd_fileName, filename);

	return HAL_OK;
}

static void SD_ArchivePATHFile(char *fname, char* PATH)
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
uint8_t SD_FindFile(void)
{
	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */
	uint8_t retVal = false;

	/* Start to search for sensor files */
	fr = f_findfirst(&dj, &fno, "", "D_*.txt");

	if (fr == FR_OK && fno.fname[0])
	{
		printf("FiLE: %s\r\n", fno.fname);
		printf("File Found\r\n");
		retVal = true;
	}

	f_closedir(&dj);

	printf("End of SD_FindFile\r\n");
	return retVal;
}

uint8_t SD_FindInfoFile(void)
{
	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */
	uint8_t retVal = false;

	/* Start to search for sensor files */
	fr = f_findfirst(&dj, &fno, "", "I_*.txt");

	if (fr == FR_OK && fno.fname[0])
	{
		printf("FiLE: %s\r\n", fno.fname);
		printf("File Found\r\n");
		retVal = true;
	}

	f_closedir(&dj);

	printf("End of SD_FindFile\r\n");
	return retVal;
}

void SD_ArchiveSensorFilesExceptNewest(char* exceptFile)
{
	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */

	fr = f_findfirst(&dj, &fno, "", "D_*");

	while (fr == FR_OK && fno.fname[0])
	{
		if (strcmp(fno.fname, exceptFile) == 0)
		{
			fr = f_findnext(&dj, &fno);
		}
		else
		{
			SD_ArchivePATHFile(fno.fname, TRANSMIT_PATH);

			fr = f_findnext(&dj, &fno);
		}
	}
}

void SD_ArchiveInfoFilesExceptNewest(char* exceptFile)
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
			SD_ArchivePATHFile(fno.fname, INFO_PATH);

			fr = f_findnext(&dj, &fno);
		}
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
			InfoLogAdd(
					ERRORLEVEL_WARNING,
					LOG_FUNC_SD,
					LOG_SUB_READ,
					LOG_TYPE_OPEN,
					LOG_STATUS_FAIL,
					0,
					0,
					0,
					GetInfoStruct());
			return HAL_ERROR;
		}
		else
		{
			/* Want to start reading the data that we just wrote. Not from the total beginning. Sets write pointer to the end of the file - size */
			if (f_lseek(&SDFile, f_size(&userfile) - size) != FR_OK)
			{
				InfoLogAdd(
						ERRORLEVEL_WARNING,
						LOG_FUNC_SD,
						LOG_SUB_VERIFY,
						LOG_TYPE_SEEK,
						LOG_STATUS_FAIL,
						1,
						0,
						0,
						GetInfoStruct());
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
						InfoLogAdd(
								ERRORLEVEL_WARNING,
								LOG_FUNC_SD,
								LOG_SUB_READ,
								LOG_TYPE_READ,
								LOG_STATUS_FAIL,
								0,
								0,
								0,
								GetInfoStruct());
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
							InfoLogAdd(
									ERRORLEVEL_WARNING,
									LOG_FUNC_SD,
									LOG_SUB_VERIFY,
									LOG_TYPE_COMPARE,
									LOG_STATUS_FAIL,
									1,
									0,
									0,
									GetInfoStruct());
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
//		ErrorAddLog(
//				LOG_FUNC_SD,
//				LOG_SUB_WRITE,
//				LOG_TYPE_OPEN,
//				LOG_STATUS_FAIL,
//				1,
//				0,
//				0,
//				GetErrorStruct());
	}
	else
	{
		for (int i = 0; i < sensCount; i++)
		{
			uint32_t lux;
			uint16_t tempHum;
			lux = (data->rData.sData[i].tempHumLux >> 15);
			tempHum = (data->rData.sData[i].tempHumLux & 0b111111111111111);

			/* Only want to add this the first cycle */
			if (timeBat)
			{
				/* Add time and battery */
				sprintf(
						BatTime,
						"&%X.%X",
						(unsigned int) data->rData.timeStamp,
						data->rData.batteryLevel);

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
					data->rData.sData[i].spectrum.violet,
					data->rData.sData[i].spectrum.blue,
					data->rData.sData[i].spectrum.green,
					data->rData.sData[i].spectrum.yellow,
					data->rData.sData[i].spectrum.orange,
					data->rData.sData[i].spectrum.red);

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
		if (SD_getFileName(sensDate) == HAL_OK)
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

/*
 *	Writes data to SD to file 'filename' from address 'ram_address' to address 'ram_address' + 'size'.
 */
uint8_t SD_Write(FIL userfile, char* filename, uint32_t ram_address, uint32_t size)
{

	uint32_t byteswritten;

	/* Open or create file */
	if (f_open(&SDFile, filename, FA_OPEN_APPEND | FA_WRITE) != FR_OK)
	{
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
//					if (GetErrorFlagsStruct()->errorHourCounter == 2)
//					{
//						ArchiveInfoFilesExceptNewest(infoDate);
//					}
//					else if (GetErrorFlagsStruct()->everythingOK == 1)
//					{
//						ArchiveInfoFilesExceptNewest(infoDate);
//					}

					return HAL_OK;
				}

			}
		}
	}

	return HAL_ERROR;

}
