/*
 * Author: Copyright (C) STMicroelectronics             Date:   04/27/2009
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
#ifndef __USB_PWR_H
#define __USB_PWR_H
/* Includes
   ------------------------------------------------------------------ */
#include "usb_core.h"
/* Exported types
   ------------------------------------------------------------ */
typedef enum _RESUME_STATE
{
    RESUME_EXTERNAL,
    RESUME_INTERNAL,
    RESUME_LATER,
    RESUME_WAIT,
    RESUME_START,
    RESUME_ON,
    RESUME_OFF,
    RESUME_ESOF
} RESUME_STATE;

typedef enum _DEVICE_STATE
{
    UNCONNECTED,
    ATTACHED,
    POWERED,
    SUSPENDED,
    ADDRESSED,
    CONFIGURED
} DEVICE_STATE;

/* Exported constants
   -------------------------------------------------------- */
/* Exported macro
   ------------------------------------------------------------ */
/* Exported functions -------------------------------------------------------
 */
void Suspend (void);

void Resume_Init (void);

void Resume (RESUME_STATE eResumeSetVal);

RESULT PowerOn (void);

RESULT PowerOff (void);

void USB_Start (void);

/* External variables
   -------------------------------------------------------- */
extern __IO uint32_t bDeviceState;  /* USB device status */

extern __IO bool fSuspendEnabled;   /* true when suspend is possible */

#endif  /*__USB_PWR_H*/
