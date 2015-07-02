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
#ifndef __USB_DEF_H
#define __USB_DEF_H

/* Includes
   ------------------------------------------------------------------ */
/* Exported types
   ------------------------------------------------------------ */
typedef enum _RECIPIENT_TYPE
{
    DEVICE_RECIPIENT,   /* Recipient device */
    INTERFACE_RECIPIENT,    /* Recipient interface */
    ENDPOINT_RECIPIENT, /* Recipient endpoint */
    OTHER_RECIPIENT
} RECIPIENT_TYPE;


typedef enum _STANDARD_REQUESTS
{
    GET_STATUS = 0,
    CLEAR_FEATURE,
    RESERVED1,
    SET_FEATURE,
    RESERVED2,
    SET_ADDRESS,
    GET_DESCRIPTOR,
    SET_DESCRIPTOR,
    GET_CONFIGURATION,
    SET_CONFIGURATION,
    GET_INTERFACE,
    SET_INTERFACE,
    TOTAL_sREQUEST, /* Total number of Standard request */
    SYNCH_FRAME = 12
} STANDARD_REQUESTS;

/* Definition of "USBwValue" */
typedef enum _DESCRIPTOR_TYPE
{
    DEVICE_DESCRIPTOR = 1,
    CONFIG_DESCRIPTOR,
    STRING_DESCRIPTOR,
    INTERFACE_DESCRIPTOR,
    ENDPOINT_DESCRIPTOR
} DESCRIPTOR_TYPE;

/* Feature selector of a SET_FEATURE or CLEAR_FEATURE */
typedef enum _FEATURE_SELECTOR
{
    ENDPOINT_STALL,
    DEVICE_REMOTE_WAKEUP
} FEATURE_SELECTOR;

/* Exported constants
   -------------------------------------------------------- */
/* Definition of "USBbmRequestType" */
#define REQUEST_TYPE      0x60  /* Mask to get request type */
#define STANDARD_REQUEST  0x00  /* Standard request */
#define CLASS_REQUEST     0x20  /* Class request */
#define VENDOR_REQUEST    0x40  /* Vendor request */

#define RECIPIENT         0x1F  /* Mask to get recipient */

/* Exported macro
   ------------------------------------------------------------ */
/* Exported functions -------------------------------------------------------
 */

#endif /* __USB_DEF_H */
