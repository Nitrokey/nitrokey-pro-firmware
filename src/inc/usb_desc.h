/*
 * Author: Copyright (C) STMicroelectronics
 *                                              MCD Application Team            Date:   04/27/2009
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

/* Define to prevent recursive inclusion ------------------------------------- */
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------ */
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------ */
/* Exported constants -------------------------------------------------------- */
/* Exported vari -------------------------------------------------------- */

/* Exported macro ------------------------------------------------------------ */
/* Exported define ----------------------------------------------------------- */
#define MASS_SIZ_DEVICE_DESC              18
#define MASS_SIZ_CONFIG_DESC              32

#define MASS_SIZ_STRING_LANGID            4
#define MASS_SIZ_STRING_VENDOR            18
#define MASS_SIZ_STRING_PRODUCT           54
#define MASS_SIZ_STRING_SERIAL            50
#define MASS_SIZ_STRING_INTERFACE         24

/* Exported functions ------------------------------------------------------- */
extern const uint8_t MASS_DeviceDescriptor[MASS_SIZ_DEVICE_DESC];

extern const uint8_t MASS_ConfigDescriptor[MASS_SIZ_CONFIG_DESC];

extern const uint8_t MASS_StringLangID[MASS_SIZ_STRING_LANGID];

extern const uint8_t MASS_StringVendor[MASS_SIZ_STRING_VENDOR];

extern const uint8_t MASS_StringProduct[MASS_SIZ_STRING_PRODUCT];

extern uint8_t MASS_StringSerial[MASS_SIZ_STRING_SERIAL];

extern const uint8_t MASS_StringInterface[MASS_SIZ_STRING_INTERFACE];

#endif /* __USB_DESC_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
