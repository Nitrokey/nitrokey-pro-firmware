/*
* Author: Copyright (C) STMicroelectronics	 			Date:	04/27/2009
*												 MCD Application Team			Version V3.0.1
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
#ifndef __memory_H
#define __memory_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define TXFR_IDLE     0
#define TXFR_ONGOING  1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Write_Memory (uint32_t Memory_Offset, uint32_t Transfer_Length);
void Read_Memory (uint32_t Memory_Offset, uint32_t Transfer_Length);
#endif /* __memory_H */


