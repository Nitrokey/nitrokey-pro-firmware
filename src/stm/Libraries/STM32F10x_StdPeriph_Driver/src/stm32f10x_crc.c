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

/* Includes
   ------------------------------------------------------------------ */
#include "stm32f10x_crc.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @defgroup CRC
  * @brief CRC driver modules
  * @{
  */

/** @defgroup CRC_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Private_Defines
  * @{
  */

/* CR register bit mask */

#define CR_RESET_Set    ((uint32_t)0x00000001)

/**
  * @}
  */

/** @defgroup CRC_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Private_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup CRC_Private_Functions
  * @{
  */

/**
  * @brief  Resets the CRC Data register (DR).
  * @param  None
  * @retval : None
  */
void CRC_ResetDR (void)
{
    /* Reset CRC generator */
    CRC->CR = CR_RESET_Set;
}

/**
  * @brief  Computes the 32-bit CRC of a given data word(32-bit).
  * @param Data: data word(32-bit) to compute its CRC
  * @retval : 32-bit CRC
  */
uint32_t CRC_CalcCRC (uint32_t Data)
{
    CRC->DR = Data;

    return (CRC->DR);
}

/**
  * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param pBuffer: pointer to the buffer containing the data to be
  *   computed
  * @param BufferLength: length of the buffer to be computed
  * @retval : 32-bit CRC
  */
uint32_t CRC_CalcBlockCRC (uint32_t pBuffer[], uint32_t BufferLength)
{
uint32_t index = 0;

    for (index = 0; index < BufferLength; index++)
    {
        CRC->DR = pBuffer[index];
    }
    return (CRC->DR);
}

/**
  * @brief  Returns the current CRC value.
  * @param  None
  * @retval : 32-bit CRC
  */
uint32_t CRC_GetCRC (void)
{
    return (CRC->DR);
}

/**
  * @brief  Stores a 8-bit data in the Independent Data(ID) register.
  * @param IDValue: 8-bit value to be stored in the ID register
  * @retval : None
  */
void CRC_SetIDRegister (uint8_t IDValue)
{
    CRC->IDR = IDValue;
}

/**
  * @brief  Returns the 8-bit data stored in the Independent Data(ID) register
  * @param  None
  * @retval : 8-bit value of the ID register
  */
uint8_t CRC_GetIDRegister (void)
{
    return (CRC->IDR);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
