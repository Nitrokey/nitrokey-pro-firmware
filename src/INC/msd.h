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
#ifndef __MSD_H
#define __MSD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
/* Block Size */
#define BLOCK_SIZE    512

/* Dummy byte */
#define DUMMY   0xFF

/* Start Data tokens  */
/* Tokens (necessary because at nop/idle (and CS active) only 0xff is on the data/command line) */
#define MSD_START_DATA_SINGLE_BLOCK_READ 0xFE  /* Data token start byte, Start Single Block Read */
#define MSD_START_DATA_MULTIPLE_BLOCK_READ  0xFE  /* Data token start byte, Start Multiple Block Read */
#define MSD_START_DATA_SINGLE_BLOCK_WRITE 0xFE  /* Data token start byte, Start Single Block Write */
#define MSD_START_DATA_MULTIPLE_BLOCK_WRITE 0xFD  /* Data token start byte, Start Multiple Block Write */
#define MSD_STOP_DATA_MULTIPLE_BLOCK_WRITE 0xFD  /* Data toke stop byte, Stop Multiple Block Write */

/* MSD functions return */
#define MSD_SUCCESS       0x00
#define MSD_FAIL          0xFF

/* MSD reponses and error flags */
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

/* Data response error */
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

/* Commands: CMDxx = CMD-number | 0x40 */
#define MSD_GO_IDLE_STATE          0   /* CMD0=0x40 */
#define MSD_SEND_OP_COND           1   /* CMD1=0x41 */
#define MSD_SEND_CSD               9   /* CMD9=0x49 */
#define MSD_SEND_CID               10  /* CMD10=0x4A */
#define MSD_STOP_TRANSMISSION      12  /* CMD12=0x4C */
#define MSD_SEND_STATUS            13  /* CMD13=0x4D */
#define MSD_SET_BLOCKLEN           16  /* CMD16=0x50 */
#define MSD_READ_SINGLE_BLOCK      17  /* CMD17=0x51 */
#define MSD_READ_MULTIPLE_BLOCK    18  /* CMD18=0x52 */
#define MSD_SET_BLOCK_COUNT        23  /* CMD23=0x57 */
#define MSD_WRITE_BLOCK            24  /* CMD24=0x58 */
#define MSD_WRITE_MULTIPLE_BLOCK   25  /* CMD25=0x59 */
#define MSD_PROGRAM_CSD            27  /* CMD27=0x5B */
#define MSD_SET_WRITE_PROT         28  /* CMD28=0x5C */
#define MSD_CLR_WRITE_PROT         29  /* CMD29=0x5D */
#define MSD_SEND_WRITE_PROT        30  /* CMD30=0x5E */
#define MSD_TAG_SECTOR_START       32  /* CMD32=0x60 */
#define MSD_TAG_SECTOR_END         33  /* CMD33=0x61 */
#define MSD_UNTAG_SECTOR           34  /* CMD34=0x62 */
#define MSD_TAG_ERASE_GROUP_START  35  /* CMD35=0x63 */
#define MSD_TAG_ERASE_GROUP_END    36  /* CMD36=0x64 */
#define MSD_UNTAG_ERASE_GROUP      37  /* CMD37=0x65 */
#define MSD_ERASE                  38  /* CMD38=0x66 */
#define MSD_READ_OCR               39  /* CMD39=0x67 */
#define MSD_CRC_ON_OFF             40  /* CMD40=0x68 */

/* Exported types ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct _MSD_CSD      /*Card Specific Data*/
{
  __IO uint8_t  CSDStruct;            /* CSD structure */
  __IO uint8_t  SysSpecVersion;       /* System specification version */
  __IO uint8_t  Reserved1;            /* Reserved */
  __IO uint8_t  TAAC;                 /* Data read access-time 1 */
  __IO uint8_t  NSAC;                 /* Data read access-time 2 in CLK cycles */
  __IO uint8_t  MaxBusClkFrec;        /* Max. bus clock frequency */
  __IO uint16_t CardComdClasses;      /* Card command classes */
  __IO uint8_t  RdBlockLen;           /* Max. read data block length */
  __IO uint8_t  PartBlockRead;        /* Partial blocks for read allowed */
  __IO uint8_t  WrBlockMisalign;      /* Write block misalignment */
  __IO uint8_t  RdBlockMisalign;      /* Read block misalignment */
  __IO uint8_t  DSRImpl;              /* DSR implemented */
  __IO uint8_t  Reserved2;            /* Reserved */
  __IO uint16_t DeviceSize;           /* Device Size */
  __IO uint8_t  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min */
  __IO uint8_t  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max */
  __IO uint8_t  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min */
  __IO uint8_t  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max */
  __IO uint8_t  DeviceSizeMul;        /* Device size multiplier */
  __IO uint8_t  EraseGrSize;          /* Erase group size */
  __IO uint8_t  EraseGrMul;           /* Erase group size multiplier */
  __IO uint8_t  WrProtectGrSize;      /* Write protect group size */
  __IO uint8_t  WrProtectGrEnable;    /* Write protect group enable */
  __IO uint8_t  ManDeflECC;           /* Manufacturer default ECC */
  __IO uint8_t  WrSpeedFact;          /* Write speed factor */
  __IO uint8_t  MaxWrBlockLen;        /* Max. write data block length */
  __IO uint8_t  WriteBlockPaPartial;  /* Partial blocks for write allowed */
  __IO uint8_t  Reserved3;            /* Reserded */
  __IO uint8_t  ContentProtectAppli;  /* Content protection application */
  __IO uint8_t  FileFormatGrouop;     /* File format group */
  __IO uint8_t  CopyFlag;             /* Copy flag (OTP) */
  __IO uint8_t  PermWrProtect;        /* Permanent write protection */
  __IO uint8_t  TempWrProtect;        /* Temporary write protection */
  __IO uint8_t  FileFormat;           /* File Format */
  __IO uint8_t  ECC;                  /* ECC code */
  __IO uint8_t  msd_CRC;                  /* CRC */
  __IO uint8_t  Reserved4;            /* always 1*/
}
sMSD_CSD;

typedef struct _MSD_CID      /*Card Identification Data*/
{
  __IO uint8_t  ManufacturerID;       /* ManufacturerID */
  __IO uint16_t OEM_AppliID;          /* OEM/Application ID */
  __IO uint32_t ProdName1;            /* Product Name part1 */
  __IO uint8_t  ProdName2;            /* Product Name part2*/
  __IO uint8_t  ProdRev;              /* Product Revision */
  __IO uint32_t ProdSN;               /* Product Serial Number */
  __IO uint8_t  Reserved1;            /* Reserved1 */
  __IO uint16_t ManufactDate;         /* Manufacturing Date */
  __IO uint8_t  msd_CRC;                  /* CRC */
  __IO uint8_t  Reserved2;            /* always 1*/
}
sMSD_CID;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/*----- High layer function -----*/
uint8_t MSD_Init(void);
uint8_t MSD_WriteBlock(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
uint8_t MSD_ReadBlock(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint8_t MSD_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
uint8_t MSD_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
uint8_t MSD_GetCSDRegister(sMSD_CSD* MSD_csd);
uint8_t MSD_GetCIDRegister(sMSD_CID* MSD_cid);

/*----- Medium layer function -----*/
void MSD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc);
uint8_t MSD_GetResponse(uint8_t Response);
uint8_t MSD_GetDataResponse(void);
uint8_t MSD_GoIdleState(void);
uint16_t MSD_GetStatus(void);

/*----- Low layer function -----*/
void MSD_WriteByte(uint8_t byte);
uint8_t MSD_ReadByte(void);

#endif /* __MSD_H */

