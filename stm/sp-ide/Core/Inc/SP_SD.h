#ifndef SP_SD_H_
#define SP_SD_H_

#include "main.h"
#include <stdbool.h>

void SD_Init(void);

void SD_RefreshDateTime(void);
void SD_GetDateTime(char[], char[]);
void SD_SetDateTime(uint8_t[], uint8_t[]);
void SD_RemoveAllJsons(void);

uint8_t SD_CreateJson(bool, float[], char[], char[]);
uint32_t SD_GetNofJsons(void);
uint32_t SD_GetLastFileno(void);

char* SD_ReadFile(char*, uint32_t*);
char* SD_GetLastJson(uint32_t*);
char* SD_ListJsons(uint8_t*, uint32_t);
char* SD_GetJsonFromEnd(uint32_t, uint32_t*);


#endif
