/*
 * Author: Copyright (C) Rudolf Boeddeker                   Date: 2010-01-13
 *                                              STMicroelectronics              Date:   04/27/2009
 *                                               MCD Application Team           Version V3.0.1
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
#ifndef __ramdisk_usb_prop_H
#define __ramdisk_usb_prop_H
/* Includes
   ------------------------------------------------------------------ */
/* Exported types
   ------------------------------------------------------------ */




/* Exported constants
   -------------------------------------------------------- */
#define RAMDISK_Storage_GetConfiguration          NOP_Process
/* #define RAMDISK_Storage_SetConfiguration NOP_Process */
#define RAMDISK_Storage_GetInterface              NOP_Process
#define RAMDISK_Storage_SetInterface              NOP_Process
#define RAMDISK_Storage_GetStatus                 NOP_Process
/* #define RAMDISK_Storage_ClearFeature NOP_Process */
#define RAMDISK_Storage_SetEndPointFeature        NOP_Process
#define RAMDISK_Storage_SetDeviceFeature          NOP_Process
/* #define RAMDISK_Storage_SetDeviceAddress NOP_Process */

/* MASS Storage Requests */
#define GET_MAX_LUN                0xFE
#define RAMDISK_STORAGE_RESET         0xFF
#define LUN_DATA_LENGTH            1

/* Exported macro
   ------------------------------------------------------------ */
/* Exported functions -------------------------------------------------------
 */
void RAMDISK_init (void);

void RAMDISK_Reset (void);

void RAMDISK_Storage_SetConfiguration (void);

void RAMDISK_Storage_ClearFeature (void);

void RAMDISK_Storage_SetDeviceAddress (void);

void RAMDISK_Status_In (void);

void RAMDISK_Status_Out (void);

RESULT RAMDISK_Data_Setup (uint8_t);

RESULT RAMDISK_NoData_Setup (uint8_t);

RESULT RAMDISK_Get_Interface_Setting (uint8_t Interface,
                                      uint8_t AlternateSetting);
uint8_t* RAMDISK_GetDeviceDescriptor (uint16_t);

uint8_t* RAMDISK_GetConfigDescriptor (uint16_t);

uint8_t* RAMDISK_GetStringDescriptor (uint16_t);

uint8_t* RAMDISK_Get_Max_Lun (uint16_t Length);

#endif /* __ramdisk_usb_prop_H */
