/*
 * Author: Copyright (C) STMicroelectronics
 *                                              MCD Application Team            Date:   04/06/2009
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

/* Define to prevent recursive inclusion
   ------------------------------------- */
#ifndef __STM32F10x_DBGMCU_H
#define __STM32F10x_DBGMCU_H

/* Includes
   ------------------------------------------------------------------ */
#include "stm32f10x.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @addtogroup DBGMCU
  * @{
  */

/** @defgroup DBGMCU_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup DBGMCU_Exported_Constants
  * @{
  */

#define DBGMCU_SLEEP                 ((uint32_t)0x00000001)
#define DBGMCU_STOP                  ((uint32_t)0x00000002)
#define DBGMCU_STANDBY               ((uint32_t)0x00000004)
#define DBGMCU_IWDG_STOP             ((uint32_t)0x00000100)
#define DBGMCU_WWDG_STOP             ((uint32_t)0x00000200)
#define DBGMCU_TIM1_STOP             ((uint32_t)0x00000400)
#define DBGMCU_TIM2_STOP             ((uint32_t)0x00000800)
#define DBGMCU_TIM3_STOP             ((uint32_t)0x00001000)
#define DBGMCU_TIM4_STOP             ((uint32_t)0x00002000)
#define DBGMCU_CAN1_STOP             ((uint32_t)0x00004000)
#define DBGMCU_I2C1_SMBUS_TIMEOUT    ((uint32_t)0x00008000)
#define DBGMCU_I2C2_SMBUS_TIMEOUT    ((uint32_t)0x00010000)
#define DBGMCU_TIM8_STOP             ((uint32_t)0x00020000)
#define DBGMCU_TIM5_STOP             ((uint32_t)0x00040000)
#define DBGMCU_TIM6_STOP             ((uint32_t)0x00080000)
#define DBGMCU_TIM7_STOP             ((uint32_t)0x00100000)

#define IS_DBGMCU_PERIPH(PERIPH) ((((PERIPH) & 0xFFE000F8) == 0x00) && ((PERIPH) != 0x00))
/**
  * @}
  */

/** @defgroup DBGMCU_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup DBGMCU_Exported_Functions
  * @{
  */

uint32_t DBGMCU_GetREVID (void);

uint32_t DBGMCU_GetDEVID (void);

void DBGMCU_Config (uint32_t DBGMCU_Periph, FunctionalState NewState);

#endif /* __STM32F10x_DBGMCU_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
