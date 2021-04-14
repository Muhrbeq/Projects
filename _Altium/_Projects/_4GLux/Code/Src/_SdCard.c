/*
 * _SDCard.c
 *
 *  Created on: 9 apr. 2021
 *      Author: Rasmus.Muhrbeck
 */

#include <_SDCard.h>
#include "__ExegerGeneric.h"
#include "_Global.h"
#include "_LowLevel.h"
#include "_Time.h"

#include "ff.h"
#include "bsp_driver_sd.h"
#include "fatfs.h"
#include "sdmmc.h"

#include "rtc.h"

extern char sd_fileName[13];


SD_States sdState;
static SD_Folder sdFolder = SD_FolderNotInitiated;

SD_States SD_GetState(void)
{
	return sdState;
}

void SD_SetState(SD_States sState)
{
	sdState = sState;
}

static void SD_StartUp(void)
{
	/*
	 * Start by enabling GPIO
	 *
	 *  */

	MX_SDMMC1_SD_Init();
	MX_FATFS_Init();

}

static HAL_StatusTypeDef SD_Init(void)
{
	BSP_SD_Init();

	FRESULT res = f_mount(&SDFatFS, (TCHAR const*) SDPath, 1);

	TRACE_DEBUG("Mount result = %d\n\r", res);

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

static HAL_StatusTypeDef SD_Unmount(void)
{
	if(f_mount(NULL, "", 0) == FR_OK)
	{
		if(FATFS_UnLinkDriver(SDPath) == 0)
		{
			return HAL_OK;
		}
	}

	return HAL_ERROR;
}

static HAL_StatusTypeDef SD_InitFoldersOnSD(void)
{
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

	return HAL_OK;
}

static HAL_StatusTypeDef SD_SetSDFilename(char *fileName)
{
	sprintf(sd_fileName, fileName);
	return HAL_OK;
}

 /* Sets the timestamp on the file
 */
static HAL_StatusTypeDef SD_SetTimestamp(char *filename)
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

static void SD_ArchiveFileToPath(char *fname, char* PATH)
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
		TRACE_DEBUG("File already in archive folder, delete current one in root\n\r");

		/* Delete file */
		f_unlink(fname);
	}
	else if (res == FR_OK)
	{
		TRACE_DEBUG("Programmed File %s moved to %s\n\r", fname, archivePath);
	}
	else
	{
		TRACE_DEBUG("res = %d\r\n", res);
	}
}

static void SD_ArchiveFilesExcept(char* exceptFile, const char *find, char* PATH)
{
	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */

	fr = f_findfirst(&dj, &fno, "", find);

	while (fr == FR_OK && fno.fname[0])
	{
		if (strcmp(fno.fname, exceptFile) == 0)
		{
			fr = f_findnext(&dj, &fno);
		}
		else
		{
			SD_ArchiveFileToPath(fno.fname, PATH);

			fr = f_findnext(&dj, &fno);
		}
	}
}

static uint8_t SD_FindFile(const char *find)
{
	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */
	uint8_t retVal = false;

	/* Start to search for sensor files */
	fr = f_findfirst(&dj, &fno, "", find);

	if (fr == FR_OK && fno.fname[0])
	{
		TRACE_DEBUG("File Found\r\n");
		retVal = true;
	}
	else
	{
		TRACE_DEBUG("File Not Found\r\n");
	}

	f_closedir(&dj);

	return retVal;
}

static uint8_t SD_WriteDataCharacterToFile(char* fileName, Data *data)
{
	FIL sdFile;
	UINT num;
#define MAXIMUMSIZEOFMESSAGE	150

	if(f_open(&sdFile, fileName, FA_OPEN_APPEND | FA_WRITE) == FR_OK)
	{
		for(int i = 0; i < data->sensCounter; i++)
		{
			char message[256] = { 0 };
			char sensData[64] = { 0 };
			if(i == 0)
			{
				char BatTime[32] = { 0 };
				/* Add time and battery */
				sprintf(BatTime,
						"&%lX.%X",
						data->rData.timeStamp,
						data->rData.batteryLevel);
				/* Add time and battery to start of message */
				strcat(message, BatTime);
			}
			/* Add sensor values to spectra array */
			sprintf(
					sensData,
					"*%lX:%X:%X:%X:%X:%X:%X",
					data->rData.sData[i].tempHumLux,
					data->rData.sData[i].spectrum.violet,
					data->rData.sData[i].spectrum.blue,
					data->rData.sData[i].spectrum.green,
					data->rData.sData[i].spectrum.yellow,
					data->rData.sData[i].spectrum.orange,
					data->rData.sData[i].spectrum.red);
			strcat(message, sensData);

			if(i != data->sensCounter - 1)
			{
				if(strlen(message) > MAXIMUMSIZEOFMESSAGE)
				{
					/* Add new line to ease the reading of the file */
					strcat(message, "\n");

					/* Write message to file */
					f_write(&sdFile, (char*) message, strlen(message), (void *) &num);
				}
			}
			else
			{
				/* Add new line to ease the reading of the file */
				strcat(message, "\n");

				/* Write message to file */
				f_write(&sdFile, (char*) message, strlen(message), (void *) &num);
			}
		}
		f_close(&sdFile);
	}
	else
	{

		return HAL_ERROR;
	}

	return HAL_OK;
}

static HAL_StatusTypeDef SD_WriteInfoCharactersToFile(char* fileName, Data *data)
{
	/*
	 *
	 * Implement when Info implemented
	 *  */
	return HAL_OK;
}

static HAL_StatusTypeDef SD_WriteDataToFile(Data *data)
{
	Time fileDate;
	char dataDate[16] = {0};

	GetTime(&fileDate);

	sprintf(dataDate, "%02d-%02d-%02d.txt", fileDate.Year, fileDate.Month, fileDate.Day);

	if(SD_SetSDFilename(dataDate) == HAL_OK)
	{
		if(SD_SetTimestamp(sd_fileName) == HAL_OK)
		{
			if(SD_WriteDataCharacterToFile(sd_fileName, data) == HAL_OK)
			{
				TRACE_DEBUG("[SD] - Writing data to SD\r\n");
				data->sensCounter = 0;
				return HAL_OK;
			}
		}
	}
	return HAL_ERROR;
}

void SD_WriteStateMachine(Data *data)
{
	switch(sdState)
	{
	case SD_STARTUP:
	{
		SD_StartUp();
		SD_SetState(SD_INIT);
		break;
	}
	case SD_INIT:
	{
		if(SD_Init() == HAL_OK)
		{
			if(sdFolder == SD_FolderInitiated)
			{
				SD_SetState(SD_INITFOLDERSONSD);
			}
			else
			{
				SD_SetState(SD_WRITEDATA);
			}
		}
		break;
	}
	case SD_INITFOLDERSONSD:
	{
		if(SD_InitFoldersOnSD() == HAL_OK)
		{
			sdFolder = SD_FolderInitiated;
			SD_SetState(SD_UNMOUNT);
		}
		break;
	}
	case SD_WRITEDATA:
	{
		if(SD_WriteDataToFile(data) == HAL_OK)
		{
			SD_SetState(SD_WRITEINFO);
		}
		break;
	}
	case SD_WRITEINFO:
	{
		SD_SetState(SD_UNMOUNT);
		break;
	}
	case SD_UNMOUNT:
	{
		if(SD_Unmount() == HAL_OK)
		{
			SD_SetState(SD_IDLE);
		}
		break;
	}
	default:
	{
		/*
		 * SD Idle when sending...
		 *  */
		break;
	}
	}
}

void SD_ReadStateMachine(Data *data)
{
	switch(sdState)
	{
	case SD_STARTUP:
	{
		SD_StartUp();
		SD_SetState(SD_INIT);
		break;
	}
	case SD_INIT:
	{
		if(SD_Init() == HAL_OK)
		{
			SD_SetState(SD_READANDSENDDATA);
		}
		break;
	}
	case SD_READANDSENDDATA:
	{

		/*
		 * Read and send data from SD, Archive after
		 * */
		break;
	}
	case SD_ARCHIVE:
	{
		/*
		 *
		 * Archive everyexcept newest
		 * */
	}

	case SD_UNMOUNT:
	{
		if(SD_Unmount() == HAL_OK)
		{
			SD_SetState(SD_IDLE);
		}
		break;
	}
	default:
	{
		/*
		 * SD Idle when sending...
		 *  */
		break;
	}
	}
}


