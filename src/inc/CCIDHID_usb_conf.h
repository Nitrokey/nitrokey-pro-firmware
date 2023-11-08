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

/* Define to prevent recursive inclusion ------------------------------------- */
#ifndef __CCID_USB_CONF_H
#define __CCID_USB_CONF_H

/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/
#define CCID_EP_NUM                          (5)

/*-------------------------------------------------------------*/
/* -------------- Buffer Description Table ----------------- */
/*-------------------------------------------------------------*/
/* buffer table base address */

#define BTABLE_ADDRESS           (0x00)

// Entries in the Buffer Description Table describe each endpoint buffer location and size:

// BTABLE_ADDRESS + EPn * 8 + 0: USB_ADDRn_TX  (TX buffer address inside the PMA)
// BTABLE_ADDRESS + EPn * 8 + 2: USB_COUNTn_TX (bytes present in the TX buffer)
// BTABLE_ADDRESS + EPn * 8 + 4: USB_ADDRn_RX  (RX buffer address inside the PMA)
// BTABLE_ADDRESS + EPn * 8 + 6: USB_COUNTn_RX (bytes available/present for the RX buffer)

// EP0, size = 64
#define CCID_ENDP0_RXADDR        (0x40) // BTABLE_ADDRESS + BTABLE max size (64)
#define CCID_ENDP0_TXADDR        (0x80)

// EP1, size = 64 (needs to be checked, found nothing using over 4 bytes in code)
#define CCID_ENDP1_TXADDR        (0xC0)

// EP2, size = 64
#define CCID_ENDP2_RXADDR        (0x100)
#define CCID_ENDP2_TXADDR        (0x140)

// EP4, size = 8
#define CCID_ENDP4_TXADDR        (0x180)

// PMA size is 512 bytes, last buffer address + size must be < 0x200

/* ISTR events */
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
// #define IMR_MSK (CNTR_CTRM | CNTR_RESETM)
#define IMR_MSK (CNTR_CTRM  | CNTR_WKUPM | CNTR_SUSPM | CNTR_ERRM  | CNTR_SOFM \
                 | CNTR_ESOFM | CNTR_RESETM )


/* CTR service routines */
/* associated to defined endpoints */
// #define CCID_EP1_IN_Callback NOP_Process
// #define CCID_EP2_IN_Callback NOP_Process
#define  CCID_EP3_IN_Callback   NOP_Process
// #define CCID_EP4_IN_Callback NOP_Process
#define  CCID_EP5_IN_Callback   NOP_Process
#define  CCID_EP6_IN_Callback   NOP_Process
#define  CCID_EP7_IN_Callback   NOP_Process


#define  CCID_EP1_OUT_Callback   NOP_Process
// #define CCID_EP2_OUT_Callback NOP_Process
#define  CCID_EP3_OUT_Callback   NOP_Process
#define  CCID_EP4_OUT_Callback   NOP_Process
#define  CCID_EP5_OUT_Callback   NOP_Process
#define  CCID_EP6_OUT_Callback   NOP_Process
#define  CCID_EP7_OUT_Callback   NOP_Process

#endif /* __CCID_USB_CONF_H */
