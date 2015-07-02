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
#ifndef __USB_MEM_H
#define __USB_MEM_H

/* Includes
   ------------------------------------------------------------------ */
/* Exported types
   ------------------------------------------------------------ */
/* Exported constants
   -------------------------------------------------------- */
/* Exported macro
   ------------------------------------------------------------ */
/* Exported functions -------------------------------------------------------
 */
void UserToPMABufferCopy (uint8_t * pbUsrBuf, uint16_t wPMABufAddr,
                          uint16_t wNBytes);
void PMAToUserBufferCopy (uint8_t * pbUsrBuf, uint16_t wPMABufAddr,
                          uint16_t wNBytes);

/* External variables
   -------------------------------------------------------- */

#endif  /*__USB_MEM_H*/
