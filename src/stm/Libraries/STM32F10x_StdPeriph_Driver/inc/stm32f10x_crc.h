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
#ifndef __STM32F10x_CRC_H
#define __STM32F10x_CRC_H

/* Includes
   ------------------------------------------------------------------ */
#include "stm32f10x.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @addtogroup CRC
  * @{
  */

/** @defgroup CRC_Exported_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Exported_Functions
  * @{
  */

void CRC_ResetDR (void);

uint32_t CRC_CalcCRC (uint32_t Data);

uint32_t CRC_CalcBlockCRC (uint32_t pBuffer[], uint32_t BufferLength);

uint32_t CRC_GetCRC (void);

void CRC_SetIDRegister (uint8_t IDValue);

uint8_t CRC_GetIDRegister (void);

#endif /* __STM32F10x_CRC_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
