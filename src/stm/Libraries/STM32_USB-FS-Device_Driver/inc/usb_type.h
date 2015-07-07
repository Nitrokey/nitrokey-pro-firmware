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

/* Define to prevent recursive inclusion
   ------------------------------------- */
#ifndef __USB_TYPE_H
#define __USB_TYPE_H

/* Includes
   ------------------------------------------------------------------ */
#include "usb_conf.h"

/* Exported types
   ------------------------------------------------------------ */
/* Exported constants
   -------------------------------------------------------- */
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef __STM32F10x_H

typedef signed long s32;

typedef signed short s16;

typedef signed char s8;

typedef volatile signed long vs32;

typedef volatile signed short vs16;

typedef volatile signed char vs8;

typedef unsigned long u32;

typedef unsigned short u16;

typedef unsigned char u8;

typedef unsigned long const uc32;   /* Read Only */

typedef unsigned short const uc16;  /* Read Only */

typedef unsigned char const uc8;    /* Read Only */

typedef volatile unsigned long vu32;

typedef volatile unsigned short vu16;

typedef volatile unsigned char vu8;

typedef volatile unsigned long const vuc32; /* Read Only */

typedef volatile unsigned short const vuc16;    /* Read Only */

typedef volatile unsigned char const vuc8;  /* Read Only */


typedef enum
{
    FALSE = 0, TRUE = !FALSE
}
bool;

typedef enum
{ RESET = 0, SET = !RESET } FlagStatus, ITStatus;

typedef enum
{ DISABLE = 0, ENABLE = !DISABLE } FunctionalState;

typedef enum
{ ERROR = 0, SUCCESS = !ERROR } ErrorStatus;
#endif
/* Exported macro
   ------------------------------------------------------------ */
/* Exported functions -------------------------------------------------------
 */
/* External variables
   -------------------------------------------------------- */

#endif /* __USB_TYPE_H */
