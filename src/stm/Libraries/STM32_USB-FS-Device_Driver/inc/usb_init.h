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
#ifndef __USB_INIT_H
#define __USB_INIT_H

/* Includes ------------------------------------------------------------------ */
/* Exported types ------------------------------------------------------------ */
/* Exported constants -------------------------------------------------------- */
/* Exported macro ------------------------------------------------------------ */
/* Exported functions ------------------------------------------------------- */
void USB_SetDeviceConfiguration (int nGDC);

void USB_Init (void);

/* External variables -------------------------------------------------------- */
/* The number of current endpoint, it will be used to specify an endpoint */
extern uint8_t EPindex;

/* The number of current device, it is an index to the Device_Table */
/* extern uint8_t Device_no; */
/* Points to the DEVICE_INFO structure of current device */
/* The purpose of this register is to speed up the execution */
extern DEVICE_INFO* pInformation;

/* Points to the DEVICE_PROP structure of current device */
/* The purpose of this register is to speed up the execution */
extern DEVICE_PROP* pProperty;

/* Temporary save the state of Rx & Tx status. */
/* Whenever the Rx or Tx state is changed, its value is saved */
/* in this variable first and will be set to the EPRB or EPRA */
/* at the end of interrupt process */
extern USER_STANDARD_REQUESTS* pUser_Standard_Requests;

extern uint16_t SaveState;

extern uint16_t wInterrupt_Mask;

#endif /* __USB_INIT_H */
