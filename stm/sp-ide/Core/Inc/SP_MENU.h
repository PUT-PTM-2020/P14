#ifndef SP_MENU_H_
#define SP_MENU_H_

#include "main.h"

#include <stdbool.h>

void MENU_Init(void);

void MENU_Clock(void);
void MENU_Options(void);
void MENU_OptionsSetDateTime(void);
void MENU_OptionsWifiList(void);
void MENU_SetDisplayTimeout(void);
void MENU_PasswdInput(void);
void MENU_IncTick(void);
void MENU_ForceUpdate(void);

uint8_t MENU_HandleInput(void);

#endif
