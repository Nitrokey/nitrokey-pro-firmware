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
#ifndef __CCID_USB_CONF_H
#define __CCID_USB_CONF_H

/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/
#define CCID_EP_NUM                          (5)

/*-------------------------------------------------------------*/
/* --------------   Buffer Description Table  -----------------*/
/*-------------------------------------------------------------*/
/* buffer table base address */

#define BTABLE_ADDRESS      (0x00)

/* EP0  */
/* rx/tx buffer base address */
#define CCID_ENDP0_RXADDR        (0x18)
#define CCID_ENDP0_TXADDR        (0x58)

/* EP1  */
/* tx buffer base address */
#define CCID_ENDP1_TXADDR        (0x98)

/* EP2  */
/* Rx buffer base address */
#define CCID_ENDP2_RXADDR        (0xD8)
/* Tx buffer base address */
#define CCID_ENDP2_TXADDR        (0x118)

/* EP3  */
/* tx buffer base address */
//#define ENDP3_TXADDR        (0x158)
#define ENDP4_TXADDR        (0x19C)

/* ISTR events */
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
#define IMR_MSK (CNTR_CTRM  | CNTR_RESETM)

				 
/* CTR service routines */
/* associated to defined endpoints */
//#define  CCID_EP1_IN_Callback   NOP_Process
//#define  CCID_EP2_IN_Callback   NOP_Process
#define  CCID_EP3_IN_Callback   NOP_Process
//#define  CCID_EP4_IN_Callback   NOP_Process
#define  CCID_EP5_IN_Callback   NOP_Process
#define  CCID_EP6_IN_Callback   NOP_Process
#define  CCID_EP7_IN_Callback   NOP_Process


#define  CCID_EP1_OUT_Callback   NOP_Process
//#define  CCID_EP2_OUT_Callback   NOP_Process
#define  CCID_EP3_OUT_Callback   NOP_Process
#define  CCID_EP4_OUT_Callback   NOP_Process
#define  CCID_EP5_OUT_Callback   NOP_Process
#define  CCID_EP6_OUT_Callback   NOP_Process
#define  CCID_EP7_OUT_Callback   NOP_Process

#endif /* __CCID_USB_CONF_H */

