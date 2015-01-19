/*
* Author: Copyright (C) STMicroelectronics / ST Rousset	 	Date:	04/05/2000
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


/************************************************************************************/
/*	ERROR CODES for USB Bulk In Messages : bError																		*/
/************************************************************************************/

#define		SLOT_NO_ERROR													0x81

#define		SLOTERROR_UNKNOWN											0x82

#define		SLOTERROR_CMD_NOT_SUPPORTED						0x00
#define		SLOTERROR_BAD_LENTGH									0x01
#define		SLOTERROR_BAD_SLOT										0x05
#define		SLOTERROR_BAD_POWERSELECT							0x07
#define		SLOTERROR_BAD_PROTOCOLNUM							0x07
#define		SLOTERROR_BAD_CLOCKCOMMAND						0x07
#define		SLOTERROR_BAD_ABRFU_3B								0x07
#define		SLOTERROR_BAD_ABRFU_2B								0x08
#define		SLOTERROR_BAD_LEVELPARAMETER					0x08
#define		SLOTERROR_BAD_FIDI										0x0A
#define		SLOTERROR_BAD_T01CONVCHECKSUM					0x0B
#define		SLOTERROR_BAD_GUARDTIME								0x0C
#define		SLOTERROR_BAD_WAITINGINTEGER					0x0D
#define		SLOTERROR_BAD_CLOCKSTOP								0x0E
#define		SLOTERROR_BAD_IFSC										0x0F
#define		SLOTERROR_BAD_NAD											0x10

#define		SLOTERROR_CMD_ABORTED									0xFF
#define		SLOTERROR_ICC_MUTE										0xFE
#define		SLOTERROR_XFR_PARITY_ERROR						0xFD
#define		SLOTERROR_XFR_OVERRUN									0xFC
#define		SLOTERROR_HW_ERROR										0xFB
#define		SLOTERROR_BAD_ATR_TS									0xF8
#define		SLOTERROR_BAD_ATR_TCK									0xF7
#define		SLOTERROR_ICC_PROTOCOL_NOT_SUPPORTED	0xF6
#define		SLOTERROR_ICC_CLASS_NOT_SUPPORTED			0xF5
#define		SLOTERROR_PROCEDURE_BYTE_CONFLICT			0xF4
#define		SLOTERROR_DEACTIVATED_PROTOCOL				0xF3
#define		SLOTERROR_BUSY_WITH_AUTO_SEQUENCE			0xF2
#define		SLOTERROR_PIN_TIMEOUT									0xF0
#define		SLOTERROR_PIN_CANCELLED								0xEF
#define		SLOTERROR_CMD_SLOT_BUSY								0xE0


/************************************************************************************/
/*	ERROR CODES for RDR_TO_PC_HARDWAREERROR Message : bHardwareErrorCode						*/
/************************************************************************************/

#define		HARDWAREERRORCODE_OVERCURRENT					0x01
#define		HARDWAREERRORCODE_VOLTAGEERROR				0x02
#define		HARDWAREERRORCODE_OVERCURRENT_IT			0x04
#define		HARDWAREERRORCODE_VOLTAGEERROR_IT			0x08

