/*
* Author: Copyright (C) STMicroelectronics	 			Date:	04/27/2009
*												 MCD Application Team			Version V3.0.1
*
* This file is part of GPF Crypto Stick.
*
* GPF Crypto Stick is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* GPF Crypto Stick is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with GPF Crypto Stick. If not, see <http://www.gnu.org/licenses/>.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NAND_IF_H
#define __NAND_IF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define NAND_OK   0
#define NAND_FAIL 1

#define FREE_BLOCK  (1 << 12 )
#define BAD_BLOCK   (1 << 13 )
#define VALID_BLOCK (1 << 14 )
#define USED_BLOCK  (1 << 15 )

#define MAX_PHY_BLOCKS_PER_ZONE  1024
#define MAX_LOG_BLOCKS_PER_ZONE  1000
/* Private Structures---------------------------------------------------------*/
typedef struct __SPARE_AREA {
	uint16_t LogicalIndex;
	uint16_t DataStatus;
	uint16_t BlockStatus;
} SPARE_AREA;	

typedef enum {
  WRITE_IDLE = 0,
  POST_WRITE,
  PRE_WRITE,
  WRITE_CLEANUP,
  WRITE_ONGOING  
}WRITE_STATE;  

typedef enum {
  OLD_BLOCK = 0,
  UNUSED_BLOCK
}BLOCK_STATE; 

/* Private macro --------------------------------------------------------------*/
//#define WEAR_LEVELLING_SUPPORT
#define WEAR_DEPTH         10
#define PAGE_TO_WRITE      (Transfer_Length/512)
/* Private variables ----------------------------------------------------------*/
/* Private function prototypes ------------------------------------------------*/
/* exported functions ---------------------------------------------------------*/
uint16_t NAND_Init (void);
uint16_t NAND_Write (uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length);
uint16_t NAND_Read  (uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length);
uint16_t NAND_Format (void);
SPARE_AREA ReadSpareArea (uint32_t address);
#endif

