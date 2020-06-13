#include "SP_SD.h"

#include "ff.h"
#include "SP_LCD.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	uint8_t date[6];
	uint8_t time[6];
} SD_Time;

#define MAX_FILESIZE 		12500
#define MAX_FILENAME_LEN 	13
#define MAX_LS_LEN			1400+2
#define MAX_FILENAME 		99999999

extern RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef _Time;
RTC_DateTypeDef _Date;

FATFS _ff;
uint32_t DISK_LEFT;

FRESULT _res;
FIL _fileH;
SD_Time _dTime;

UINT _writtenB;
UINT _readB;

BYTE _buffer[MAX_FILESIZE];

uint32_t _lastJsonNum;
char _jsonBuffer[MAX_FILENAME_LEN];
char _lsDir[MAX_LS_LEN];

void _SD_ResetBuffer(void) {
	for (int i = 0; i < MAX_FILESIZE; i++) {
		_buffer[i] = 0;
	}
}

uint32_t _SD_GetDiskSpace(void) {
	FATFS *ptr;
	uint32_t fre_clust = 0;

	_res = f_getfree("", &fre_clust, &ptr);
	if (_res != FR_OK) {
		return 0;
	}

	DISK_LEFT = (fre_clust * ptr->csize) / 2;
	return ((ptr->n_fatent - 2) * ptr->csize) / 2; /* kilobajty */
}

void _SD_FormatDisk(void) {
	/* fat32 */
	while ((_res = f_mkfs("", FM_FAT32, (DWORD) 0, NULL, FF_MAX_SS)) != FR_OK) {
		HAL_Delay(1);
	}

	_SD_GetDiskSpace();
}

#define __resetJsonBuf() for(int i=0;i<MAX_FILENAME_LEN;i++)_jsonBuffer[i]=0

char* _SD_GetNextFilename(void) {
	if (_lastJsonNum == MAX_FILENAME) {
		_lastJsonNum = 0;
	} else {
		_lastJsonNum++;
	}

	__resetJsonBuf();
	sprintf(_jsonBuffer, "%08lu.jso", _lastJsonNum);

	return (char*) _jsonBuffer;
}

void _SD_WriteLastFilename(void) {
	FIL temp;

	_res = f_unlink("sp.cfg");

	if ((_res = f_open(&temp, "sp.cfg", FA_CREATE_ALWAYS | FA_WRITE))
			!= FR_OK) {
		return;
	}

	f_printf(&temp, "%lu", _lastJsonNum);

	f_close(&temp);
}

void _SD_RestartNaming(void) {
	_lastJsonNum = MAX_FILENAME;
	_SD_WriteLastFilename();
}

void _SD_ReadLastFilename(void) {
	FIL temp;

	if (f_open(&temp, "sp.cfg", FA_READ) != FR_OK) {
		_SD_RestartNaming();
		return;
	}

	_SD_ResetBuffer();
	uint32_t index = 0;

	while (!f_eof(&temp)) {
		_res = f_read(&temp, &_buffer[index++], 1, &_readB);
	}

	f_close(&temp);

	_lastJsonNum = atoi((char*) _buffer);
}

void SD_Init(void) {
	if ((_res = f_mount(&_ff, "", 1)) == FR_NO_FILESYSTEM) {
		LCD_FatalSDScreen();
		_SD_FormatDisk();
	}

	SD_RefreshDateTime();

	_SD_ReadLastFilename();
	_SD_GetDiskSpace();
}

void _SD_ClearDateTimeRegisters(void) {
	_Time.DayLightSaving = 0;
	_Time.Hours = 0;
	_Time.Minutes = 0;
	_Time.SecondFraction = 0;
	_Time.Seconds = 0;
	_Time.StoreOperation = 0;
	_Time.SubSeconds = 0;
	_Time.TimeFormat = 0;

	_Date.Date = 0;
	_Date.Month = 0;
	_Date.WeekDay = 0;
	_Date.Year = 0;
}

void SD_RefreshDateTime(void) {
	_SD_ClearDateTimeRegisters();

	HAL_RTC_GetTime(&hrtc, &_Time, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &_Date, RTC_FORMAT_BCD);

	_dTime.time[0] = (_Time.Hours / 16) + 48;
	_dTime.time[1] = (_Time.Hours % 16) + 48;
	_dTime.time[2] = (_Time.Minutes / 16) + 48;
	_dTime.time[3] = (_Time.Minutes % 16) + 48;
	_dTime.time[4] = (_Time.Seconds / 16) + 48;
	_dTime.time[5] = (_Time.Seconds % 16) + 48;

	_dTime.date[0] = (_Date.Date / 16) + 48;
	_dTime.date[1] = (_Date.Date % 16) + 48;
	_dTime.date[2] = (_Date.Month / 16) + 48;
	_dTime.date[3] = (_Date.Month % 16) + 48;
	_dTime.date[4] = (_Date.Year / 16) + 48;
	_dTime.date[5] = (_Date.Year % 16) + 48;
}

void SD_GetDateTime(char date[], char time[]) {
	sprintf(date, "%c%c.%c%c.%c%c", _dTime.date[0], _dTime.date[1],
			_dTime.date[2], _dTime.date[3], _dTime.date[4], _dTime.date[5]);
	sprintf(time, "%c%c:%c%c:%c%c", _dTime.time[0], _dTime.time[1],
			_dTime.time[2], _dTime.time[3], _dTime.time[4], _dTime.time[5]);
}

void SD_SetDateTime(uint8_t date[], uint8_t time[]) {
	_SD_ClearDateTimeRegisters();

	_Date.Date = date[0] * 16 + date[1];
	_Date.Month = date[2] * 16 + date[3];
	_Date.Year = date[4] * 16 + date[5];

	_Time.Hours = time[0] * 16 + time[1];
	_Time.Minutes = time[2] * 16 + time[3];
	_Time.Seconds = time[4] * 16 + time[5];

	HAL_RTC_SetTime(&hrtc, &_Time, RTC_FORMAT_BCD);
	HAL_RTC_SetDate(&hrtc, &_Date, RTC_FORMAT_BCD);
}

char* SD_ReadFile(char *filename, uint32_t *size) {
	if ((_res = f_open(&_fileH, filename, FA_READ)) != FR_OK) {
		*size = 0;
		return NULL;
	}

	_SD_ResetBuffer();
	uint32_t index = 0;

	while (!f_eof(&_fileH)) {
		_res = f_read(&_fileH, &_buffer[index++], 1, &_readB);

		if (_res != FR_OK) {
			break;
		}
	}

	f_close(&_fileH);
	*size = index;
	return (char*) _buffer;
}

/* Template structure
 *
 * {
 * 		"id" : 99999999,
 * 		"inn" : 1,
 * 		"temp" : 24,
 * 		"hum" : 35,
 * 		"date" : "03.30.20",
 * 		"time" : "12:03:30"
 * }
 */

uint8_t SD_CreateJson(bool innTHS, float data[], char date[], char time[]) {
	/* ignoruj boot log */
	if ((int) data[0] == 0 && (int) data[1] == 0) {
		return 1;
	}

	_SD_ResetBuffer();
	/* oof */
	uint32_t len =
			sprintf((char*) _buffer,
					"{\"id\":\"%08lu\",\"inn\":%s,\"temp\":%.0f,\"hum\":%.0f,\"date\":\"%s\",\"time\":\"%s\"}",
					(_lastJsonNum + 1 > MAX_FILENAME) ? 0 : _lastJsonNum + 1,
					innTHS ? "true" : "false", data[0], data[1], date, time);
	/* /oof */
	_SD_GetDiskSpace();

	if (len > DISK_LEFT) {
		_SD_RestartNaming();
	}

	char *name = _SD_GetNextFilename();

	_res = f_open(&_fileH, name, FA_CREATE_ALWAYS | FA_WRITE);
	if (_res != FR_OK) {
		return 2;
	}

	f_write(&_fileH, _buffer, len, &_writtenB);

	f_close(&_fileH);

	_SD_WriteLastFilename();
	return 0;
}

#define __fileIs(arg) strcmp(fno.fname,arg)==0
#define __resetDir() for(int i=0;i<MAX_LS_LEN;i++)_lsDir[i]=0

char* SD_ListJsons(uint8_t *size, uint32_t offset) {
	DIR dir;
	static FILINFO fno;

	__resetDir();
	uint32_t amount = 0;

	_res = f_opendir(&dir, "");
	if (_res == FR_OK) {
		while (1) {
			_res = f_readdir(&dir, &fno);
			if (_res != FR_OK || fno.fname[0] == 0)
				break;

			if (__fileIs("SYSTEM~1") ||__fileIs("SP.CFG")
			||__fileIs("ABOUT.HTM") || __fileIs("INDEX.HTM")
			|| __fileIs("DATA.HTM") || __fileIs("ERROR.HTM")) {
				/* ignoruj pliki systemowe */
				continue;
			}

			/* przesuwamy bufor o okreslone pozycje */
			while (offset != 0) {
				offset--;
				continue;
			}
			if (amount == 0) {
				strcpy((char*) _lsDir, fno.fname);
			} else {
				strcat((char*) _lsDir, fno.fname);
			}
			strcat((char*) _lsDir, ";");
			amount++;

			if (amount > 100) {
				/* i tak wiecej nie przetrzymamy w buforze */
				break;
			}
		}
	}

	*size = amount;
	return (char*) _lsDir;
}

uint32_t SD_GetNofJsons(void) {
	DIR dir;
	static FILINFO fno;

	uint32_t amount = 0;

	_res = f_opendir(&dir, "");
	if (_res == FR_OK) {
		while (1) {
			_res = f_readdir(&dir, &fno);
			if (_res != FR_OK || fno.fname[0] == 0)
				break;

			if (__fileIs("SYSTEM~1") ||__fileIs("SP.CFG")
			||__fileIs("ABOUT.HTM") || __fileIs("INDEX.HTM")
			|| __fileIs("DATA.HTM") || __fileIs("ERROR.HTM")) {
				/* ignoruj pliki systemowe */
				continue;
			}

			amount++;
		}
	}

	return amount - 1;
}

char* SD_GetLastJson(uint32_t *size) {
	return SD_ReadFile(_jsonBuffer, size);
}

char* SD_GetJsonFromEnd(uint32_t offset, uint32_t *size) {
	char filename[13] = { 0 };

	int name = _lastJsonNum - offset;

	sprintf(filename, "%08d.jso", (name >= 0) ? name : MAX_FILENAME + name);

	return SD_ReadFile(filename, size);
}

void SD_RemoveAllJsons(void) {
	uint8_t listSize;
	char *list = SD_ListJsons(&listSize, 0);

	while (listSize != 0) {
		int index = 0;

		while (list[index] != 0) {
			char filename[13] = { 0 };
			int i = 0;

			while (list[index] != ';') {
				filename[i++] = list[index++];
			}

			_res = f_unlink(filename);
			index++;
		}

		list = SD_ListJsons(&listSize, 0);
	}

	_SD_RestartNaming();
}

uint32_t SD_GetLastFileno(void) {
	return _lastJsonNum;
}
