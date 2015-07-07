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

/* Includes ------------------------------------------------------------------ */
#include "usb_lib.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Extern variables ---------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */
/*******************************************************************************
* Function Name  : UserToPMABufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf: pointer to user memory area.
*                  - wPMABufAddr: address into PMA.
*                  - wNBytes: no. of bytes to be copied.
* Output         : None.
* Return         : None	.
*******************************************************************************/
void UserToPMABufferCopy (uint8_t * pbUsrBuf, uint16_t wPMABufAddr, uint16_t wNBytes)
{
uint32_t n = (wNBytes + 1) >> 1;    /* n = (wNBytes + 1) / 2 */

uint32_t i, temp1, temp2;

uint16_t* pdwVal;

    pdwVal = (uint16_t *) (wPMABufAddr * 2 + PMAAddr);
    for (i = n; i != 0; i--)
    {
        temp1 = (uint16_t) * pbUsrBuf;
        pbUsrBuf++;
        temp2 = temp1 | (uint16_t) * pbUsrBuf << 8;
        *pdwVal++ = temp2;
        pdwVal++;
        pbUsrBuf++;
    }
}

/*******************************************************************************
* Function Name  : PMAToUserBufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf    = pointer to user memory area.
*                  - wPMABufAddr = address into PMA.
*                  - wNBytes     = no. of bytes to be copied.
* Output         : None.
* Return         : None.
*******************************************************************************/
void PMAToUserBufferCopy (uint8_t * pbUsrBuf, uint16_t wPMABufAddr, uint16_t wNBytes)
{
uint32_t n = (wNBytes + 1) >> 1;    /* /2 */

uint32_t i;

uint32_t* pdwVal;

    pdwVal = (uint32_t *) (wPMABufAddr * 2 + PMAAddr);
    for (i = n; i != 0; i--)
    {
        *(uint16_t *) pbUsrBuf++ = *pdwVal++;
        pbUsrBuf++;
    }
}
