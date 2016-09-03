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
#ifndef __USB_CONF_H
#define __USB_CONF_H

/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/
#define EP_NUM                          (6)

/*-------------------------------------------------------------*/
/* -------------- Buffer Description Table ----------------- */
/*-------------------------------------------------------------*/
/* buffer table base address */

#define BTABLE_ADDRESS      (0x00)

/* EP0 */
/* rx/tx buffer base address */
#define ENDP0_RXADDR        (0x018)
#define ENDP0_TXADDR        (0x058)

/* EP1 */
/* Tx buffer base address */
#define ENDP1_TXADDR        (0x98)

/* EP2 */
/* Rx buffer base address */
#define ENDP2_RXADDR        (0x0D8)
/* Tx buffer base address */
#define ENDP2_TXADDR        (0x118)

/* EP3 */
/* tx buffer base address */
// #define ENDP3_TXADDR (0x158)
#define ENDP4_TXADDR        (0x19C)

/* EP5 */
/* tx buffer base address */
#define ENDP5_TXADDR        (0x1DC)

/* ISTR events */
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
// #define IMR_MSK (CNTR_CTRM | CNTR_RESETM)
#define IMR_MSK (CNTR_CTRM  | CNTR_WKUPM | CNTR_SUSPM | CNTR_ERRM  | CNTR_SOFM \
                 | CNTR_ESOFM | CNTR_RESETM )


/* CTR service routines */
/* associated to defined endpoints */
// #define EP1_IN_Callback NOP_Process
// #define EP2_IN_Callback NOP_Process
#define  EP3_IN_Callback   NOP_Process
// #define EP4_IN_Callback NOP_Process
// #define  EP5_IN_Callback   NOP_Process
#define  EP6_IN_Callback   NOP_Process
#define  EP7_IN_Callback   NOP_Process


#define  EP1_OUT_Callback   NOP_Process
// #define EP2_OUT_Callback NOP_Process
#define  EP3_OUT_Callback   NOP_Process
#define  EP4_OUT_Callback   NOP_Process
#define  EP5_OUT_Callback   NOP_Process
#define  EP6_OUT_Callback   NOP_Process
#define  EP7_OUT_Callback   NOP_Process

#endif /* __USB_CONF_H */
