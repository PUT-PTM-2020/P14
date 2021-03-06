#ifndef SP_LCD_h
#define SP_LCD_h

#include "main.h"

void LCD_Init(void);

void LCD_BackgroundOn(void);
void LCD_BackgroundOff(void);
void LCD_ClearScreen(void);
void LCD_ResetCursor(void);
void LCD_DisplayOff(void);
void LCD_DisplayOn(void);
void LCD_WakeScreen(void);

void LCD_NextLine(const char[]);
void LCD_Print(const char[]);
void LCD_PrintCentered(const char[]);
void LCD_PrintTempInfo(float*, float*);
void LCD_PrintDateTime(const char[], const char[]);
void LCD_WriteChar(char);
void LCD_PrintNetworks(char*, int);
void LCD_PrintOptionsScreen(const char[], int);
void LCD_PrintNetworkStatus(ModeEnum, char*);
void LCD_FatalSDScreen(void);

void LCD_DisableBlink(void);
void LCD_EnableBlink(void);
void LCD_DisableCursor(void);
void LCD_EnableCursor(void);
void LCD_ScrollOneLeft(void);
void LCD_ScrollOneRight(void);
void LCD_AlignLeft(void);
void LCD_AlignRight(void);
void LCD_EnableAutoscroll(void);
void LCD_DisableAutoscroll(void);
void LCD_DefineCustomChar(uint8_t, uint8_t[]);
void LCD_SetCursor(uint8_t, uint8_t);

uint8_t LCD_CursorUp(void);
uint8_t LCD_CursorDown(void);
uint8_t LCD_CursorLeft(void);
uint8_t LCD_CursorRight(void);


#endif
