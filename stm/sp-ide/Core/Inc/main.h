/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_D6_Pin GPIO_PIN_2
#define LCD_D6_GPIO_Port GPIOE
#define LCD_D7_Pin GPIO_PIN_3
#define LCD_D7_GPIO_Port GPIOE
#define LCD_RS_Pin GPIO_PIN_4
#define LCD_RS_GPIO_Port GPIOE
#define LCD_RW_Pin GPIO_PIN_5
#define LCD_RW_GPIO_Port GPIOE
#define LCD_E_Pin GPIO_PIN_6
#define LCD_E_GPIO_Port GPIOE
#define LCD_Screen_Pin GPIO_PIN_15
#define LCD_Screen_GPIO_Port GPIOC
#define THS_Sensor1_Pin GPIO_PIN_0
#define THS_Sensor1_GPIO_Port GPIOC
#define THS_Sensor2_Pin GPIO_PIN_1
#define THS_Sensor2_GPIO_Port GPIOC
#define RGB_Green_Pin GPIO_PIN_1
#define RGB_Green_GPIO_Port GPIOA
#define RGB_Blue_Pin GPIO_PIN_2
#define RGB_Blue_GPIO_Port GPIOA
#define RGB_Red_Pin GPIO_PIN_5
#define RGB_Red_GPIO_Port GPIOA
#define MENU_Btn1_Pin GPIO_PIN_7
#define MENU_Btn1_GPIO_Port GPIOE
#define MENU_Btn2_Pin GPIO_PIN_8
#define MENU_Btn2_GPIO_Port GPIOE
#define MENU_Btn3_Pin GPIO_PIN_9
#define MENU_Btn3_GPIO_Port GPIOE
#define MENU_Btn4_Pin GPIO_PIN_10
#define MENU_Btn4_GPIO_Port GPIOE
#define STM_Green_Pin GPIO_PIN_12
#define STM_Green_GPIO_Port GPIOD
#define STM_Orange_Pin GPIO_PIN_13
#define STM_Orange_GPIO_Port GPIOD
#define STM_Red_Pin GPIO_PIN_14
#define STM_Red_GPIO_Port GPIOD
#define STM_Blue_Pin GPIO_PIN_15
#define STM_Blue_GPIO_Port GPIOD
#define HTTP_TX_Pin GPIO_PIN_10
#define HTTP_TX_GPIO_Port GPIOC
#define HTTP_RX_Pin GPIO_PIN_11
#define HTTP_RX_GPIO_Port GPIOC
#define SD_SCK_Pin GPIO_PIN_3
#define SD_SCK_GPIO_Port GPIOB
#define SD_MISO_Pin GPIO_PIN_4
#define SD_MISO_GPIO_Port GPIOB
#define SD_MOSI_Pin GPIO_PIN_5
#define SD_MOSI_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_0
#define LCD_D4_GPIO_Port GPIOE
#define LCD_D5_Pin GPIO_PIN_1
#define LCD_D5_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* Debug helpers */
#define LED(color, state) HAL_GPIO_WritePin(GPIOD, STM_ ## color ## _Pin, GPIO_PIN_ ## state)
#define LED_T(color) HAL_GPIO_TogglePin(GPIOD, STM_ ## color ## _Pin)
/* Timer macro */
#define ResetTIM(arg) __HAL_TIM_SET_COUNTER(&htim ## arg, 0);
/* IT MACRO */
#define SYS_DisableITLowerThan(arg) __set_BASEPRI(arg << 4)
#define SYS_EnableAllIT() __set_BASEPRI(0)

#define UART_PRI 1

typedef enum {
	ST_Clock,
	ST_Options,
	ST_WiFi,
	ST_PassInput,
	ST_SetDateTime,
	ST_SetDisplay,
	ST_None
} StateEnum;

typedef enum {
	MD_ClientConn,
	MD_ClientDConn,
	MD_AccessPoint,
	MD_LostHost
} ModeEnum;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
