/*
* Author: Copyright (C) STMicroelectronics	 			
*												MCD Application Team			Date:	04/06/2009
*
* This file is part of Nitrokey.
*
* Nitrokey is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* Nitrokey is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Nitrokey. If not, see <http://www.gnu.org/licenses/>.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_RTC_H
#define __STM32F10x_RTC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @addtogroup RTC
  * @{
  */ 

/** @defgroup RTC_Exported_Types
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup RTC_Exported_Constants
  * @{
  */

/** @defgroup RTC_interrupts_define 
  * @{
  */

#define RTC_IT_OW            ((uint16_t)0x0004)  /* Overflow interrupt */
#define RTC_IT_ALR           ((uint16_t)0x0002)  /* Alarm interrupt */
#define RTC_IT_SEC           ((uint16_t)0x0001)  /* Second interrupt */
#define IS_RTC_IT(IT) ((((IT) & (uint16_t)0xFFF8) == 0x00) && ((IT) != 0x00))
#define IS_RTC_GET_IT(IT) (((IT) == RTC_IT_OW) || ((IT) == RTC_IT_ALR) || \
                           ((IT) == RTC_IT_SEC))
/**
  * @}
  */ 

/** @defgroup RTC_interrupts_flags 
  * @{
  */

#define RTC_FLAG_RTOFF       ((uint16_t)0x0020)  /* RTC Operation OFF flag */
#define RTC_FLAG_RSF         ((uint16_t)0x0008)  /* Registers Synchronized flag */
#define RTC_FLAG_OW          ((uint16_t)0x0004)  /* Overflow flag */
#define RTC_FLAG_ALR         ((uint16_t)0x0002)  /* Alarm flag */
#define RTC_FLAG_SEC         ((uint16_t)0x0001)  /* Second flag */
#define IS_RTC_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0xFFF0) == 0x00) && ((FLAG) != 0x00))
#define IS_RTC_GET_FLAG(FLAG) (((FLAG) == RTC_FLAG_RTOFF) || ((FLAG) == RTC_FLAG_RSF) || \
                               ((FLAG) == RTC_FLAG_OW) || ((FLAG) == RTC_FLAG_ALR) || \
                               ((FLAG) == RTC_FLAG_SEC))
#define IS_RTC_PRESCALER(PRESCALER) ((PRESCALER) <= 0xFFFFF)

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup RTC_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup RTC_Exported_Functions
  * @{
  */

void RTC_ITConfig(uint16_t RTC_IT, FunctionalState NewState);
void RTC_EnterConfigMode(void);
void RTC_ExitConfigMode(void);
uint32_t  RTC_GetCounter(void);
void RTC_SetCounter(uint32_t CounterValue);
void RTC_SetPrescaler(uint32_t PrescalerValue);
void RTC_SetAlarm(uint32_t AlarmValue);
uint32_t  RTC_GetDivider(void);
void RTC_WaitForLastTask(void);
void RTC_WaitForSynchro(void);
FlagStatus RTC_GetFlagStatus(uint16_t RTC_FLAG);
void RTC_ClearFlag(uint16_t RTC_FLAG);
ITStatus RTC_GetITStatus(uint16_t RTC_IT);
void RTC_ClearITPendingBit(uint16_t RTC_IT);

#endif /* __STM32F10x_RTC_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


