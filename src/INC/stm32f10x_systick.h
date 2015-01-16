/*
* Author: Copyright (C) Rudolf Boeddeker 					Date: 2010-01-13
*												STMicroelectronics	 			Date:	07/11/2008
*												 MCD Application Team			Version V2.0.2
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
#ifndef __STM32F10x_SYSTICK_H
#define __STM32F10x_SYSTICK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* SysTick clock source */

/* SysTick counter state */
#define SysTick_Counter_Disable        ((u32)0xFFFFFFFE)
#define SysTick_Counter_Enable         ((u32)0x00000001)
#define SysTick_Counter_Clear          ((u32)0x00000000)

#define IS_SYSTICK_COUNTER(COUNTER) (((COUNTER) == SysTick_Counter_Disable) || \
                                     ((COUNTER) == SysTick_Counter_Enable)  || \
                                     ((COUNTER) == SysTick_Counter_Clear))

/* SysTick Flag */
#define SysTick_FLAG_COUNT             ((u32)0x00000010)
#define SysTick_FLAG_SKEW              ((u32)0x0000001E)
#define SysTick_FLAG_NOREF             ((u32)0x0000001F)

#define IS_SYSTICK_FLAG(FLAG) (((FLAG) == SysTick_FLAG_COUNT) || \
                               ((FLAG) == SysTick_FLAG_SKEW)  || \
                               ((FLAG) == SysTick_FLAG_NOREF))

#define IS_SYSTICK_RELOAD(RELOAD) (((RELOAD) > 0) && ((RELOAD) <= 0xFFFFFF))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SysTick_CLKSourceConfig(u32 SysTick_CLKSource);
void SysTick_SetReload(u32 Reload);
void SysTick_CounterCmd(u32 SysTick_Counter);
void SysTick_ITConfig(FunctionalState NewState);
u32 SysTick_GetCounter(void);
FlagStatus SysTick_GetFlagStatus(u8 SysTick_FLAG);

#endif /* __STM32F10x_SYSTICK_H */

