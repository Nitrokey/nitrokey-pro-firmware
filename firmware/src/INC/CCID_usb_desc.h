/*
* Author: Copyright (C) Rudolf Boeddeker 					Date: 2010-01-13
*												STMicroelectronics	 			Date:	04/27/2009
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
#ifndef __CCID_USB_DESC_H
#define __CCID_USB_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define CCID_SIZ_DEVICE_DESC              18
//#define CCID_SIZ_CONFIG_DESC              (1*0x09 + 1*0x09 +	0x36 + 3*0x07 + 1)
#define CCID_SIZ_CONFIG_DESC              (1*0x09 + 3*0x09 +	0x36 + 4*0x07 + 1)
    


#define CCID_SIZ_STRING_LANGID            4
#define CCID_SIZ_STRING_VENDOR            (38+14)
#define CCID_SIZ_STRING_PRODUCT           36
#define CCID_SIZ_STRING_SERIAL            50
#define CCID_SIZ_STRING_INTERFACE         16

/* Exported functions ------------------------------------------------------- */
extern const uint8_t CCID_DeviceDescriptor[CCID_SIZ_DEVICE_DESC];
extern const uint8_t CCID_ConfigDescriptor[CCID_SIZ_CONFIG_DESC];

extern const uint8_t CCID_StringLangID[CCID_SIZ_STRING_LANGID];
extern const uint8_t CCID_StringVendor[CCID_SIZ_STRING_VENDOR];
extern const uint8_t CCID_StringProduct[CCID_SIZ_STRING_PRODUCT];
extern uint8_t       CCID_StringSerial[CCID_SIZ_STRING_SERIAL];
extern const uint8_t CCID_StringInterface[CCID_SIZ_STRING_INTERFACE];

#endif /* __CCID_USB_DESC_H */




