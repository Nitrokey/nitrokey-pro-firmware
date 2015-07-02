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
#ifndef __STM32F10x_PWR_H
#define __STM32F10x_PWR_H

/* Includes
   ------------------------------------------------------------------ */
#include "stm32f10x.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @addtogroup PWR
  * @{
  */

/** @defgroup PWR_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup PWR_Exported_Constants
  * @{
  */

/** @defgroup PVD_detection_level
  * @{
  */

#define PWR_PVDLevel_2V2          ((uint32_t)0x00000000)
#define PWR_PVDLevel_2V3          ((uint32_t)0x00000020)
#define PWR_PVDLevel_2V4          ((uint32_t)0x00000040)
#define PWR_PVDLevel_2V5          ((uint32_t)0x00000060)
#define PWR_PVDLevel_2V6          ((uint32_t)0x00000080)
#define PWR_PVDLevel_2V7          ((uint32_t)0x000000A0)
#define PWR_PVDLevel_2V8          ((uint32_t)0x000000C0)
#define PWR_PVDLevel_2V9          ((uint32_t)0x000000E0)
#define IS_PWR_PVD_LEVEL(LEVEL) (((LEVEL) == PWR_PVDLevel_2V2) || ((LEVEL) == PWR_PVDLevel_2V3)|| \
                                 ((LEVEL) == PWR_PVDLevel_2V4) || ((LEVEL) == PWR_PVDLevel_2V5)|| \
                                 ((LEVEL) == PWR_PVDLevel_2V6) || ((LEVEL) == PWR_PVDLevel_2V7)|| \
                                 ((LEVEL) == PWR_PVDLevel_2V8) || ((LEVEL) == PWR_PVDLevel_2V9))
/**
  * @}
  */

/** @defgroup Regulator_state_is_STOP_mode
  * @{
  */

#define PWR_Regulator_ON          ((uint32_t)0x00000000)
#define PWR_Regulator_LowPower    ((uint32_t)0x00000001)
#define IS_PWR_REGULATOR(REGULATOR) (((REGULATOR) == PWR_Regulator_ON) || \
                                     ((REGULATOR) == PWR_Regulator_LowPower))
/**
  * @}
  */

/** @defgroup STOP_mode_entry
  * @{
  */

#define PWR_STOPEntry_WFI         ((uint8_t)0x01)
#define PWR_STOPEntry_WFE         ((uint8_t)0x02)
#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPEntry_WFI) || ((ENTRY) == PWR_STOPEntry_WFE))

/**
  * @}
  */

/** @defgroup PWR_Flag
  * @{
  */

#define PWR_FLAG_WU               ((uint32_t)0x00000001)
#define PWR_FLAG_SB               ((uint32_t)0x00000002)
#define PWR_FLAG_PVDO             ((uint32_t)0x00000004)
#define IS_PWR_GET_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB) || \
                               ((FLAG) == PWR_FLAG_PVDO))

#define IS_PWR_CLEAR_FLAG(FLAG) (((FLAG) == PWR_FLAG_WU) || ((FLAG) == PWR_FLAG_SB))
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup PWR_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup PWR_Exported_Functions
  * @{
  */

void PWR_DeInit (void);

void PWR_BackupAccessCmd (FunctionalState NewState);

void PWR_PVDCmd (FunctionalState NewState);

void PWR_PVDLevelConfig (uint32_t PWR_PVDLevel);

void PWR_WakeUpPinCmd (FunctionalState NewState);

void PWR_EnterSTOPMode (uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);

void PWR_EnterSTANDBYMode (void);

FlagStatus PWR_GetFlagStatus (uint32_t PWR_FLAG);

void PWR_ClearFlag (uint32_t PWR_FLAG);

#endif /* __STM32F10x_PWR_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
