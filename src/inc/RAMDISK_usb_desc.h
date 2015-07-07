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
#ifndef __RAMDISK_USB_DESC_H
#define __RAMDISK_USB_DESC_H

/* Includes
   ------------------------------------------------------------------ */
#include "stm32f10x.h"
/* Exported types
   ------------------------------------------------------------ */
/* Exported constants
   -------------------------------------------------------- */
/* Exported vari -------------------------------------------------------- */

/* Exported macro
   ------------------------------------------------------------ */
/* Exported define
   ----------------------------------------------------------- */
#define RAMDISK_SIZ_DEVICE_DESC              18
#define RAMDISK_SIZ_CONFIG_DESC              32

#define RAMDISK_SIZ_STRING_LANGID            4
#define RAMDISK_SIZ_STRING_VENDOR            38
#define RAMDISK_SIZ_STRING_PRODUCT           38
#define RAMDISK_SIZ_STRING_SERIAL            50
#define RAMDISK_SIZ_STRING_INTERFACE         16

/* Exported functions -------------------------------------------------------
 */
extern const uint8_t RAMDISK_DeviceDescriptor[RAMDISK_SIZ_DEVICE_DESC];

extern const uint8_t RAMDISK_ConfigDescriptor[RAMDISK_SIZ_CONFIG_DESC];

extern const uint8_t RAMDISK_StringLangID[RAMDISK_SIZ_STRING_LANGID];

extern const uint8_t RAMDISK_StringVendor[RAMDISK_SIZ_STRING_VENDOR];

extern const uint8_t RAMDISK_StringProduct[RAMDISK_SIZ_STRING_PRODUCT];

extern uint8_t RAMDISK_StringSerial[RAMDISK_SIZ_STRING_SERIAL];

extern const uint8_t RAMDISK_StringInterface[RAMDISK_SIZ_STRING_INTERFACE];

#endif /* __USB_DESC_H */
