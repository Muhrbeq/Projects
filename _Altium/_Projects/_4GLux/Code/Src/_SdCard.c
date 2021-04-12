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

uint8_t SD_FindFile(const char *find)
{
	FRESULT fr; /* Return value */
	DIR dj; /* Directory search object */
	FILINFO fno; /* File information */
	uint8_t retVal = false;

	/* Start to search for sensor files */
	fr = f_findfirst(&dj, &fno, "", find);

	if (fr == FR_OK && fno.fname[0])
	{
		TRACE_DEBUG("FiLE: %s\r\n", fno.fname);
		TRACE_DEBUG("File Found\r\n");
		retVal = true;
	}

	f_closedir(&dj);

	TRACE_DEBUG("End of SD_FindFile\r\n");
	return retVal;
}






