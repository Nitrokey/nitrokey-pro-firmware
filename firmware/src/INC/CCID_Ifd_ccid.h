/*
* Author: Copyright (C) STMicroelectronics	 											Date:	29-V-2001
*												 MICROCONTROLLER DIVISION / ST Rousset		Version V 0.00
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

#define		ICC_DATA_BLOCK_MAX_SIZE		261


unsigned char PC_to_RDR_IccPowerOn(void);
unsigned char PC_to_RDR_IccPowerOff(void);
unsigned char PC_to_RDR_GetSlotStatus(void);
unsigned char PC_to_RDR_XfrBlock(void);
unsigned char PC_to_RDR_GetParameters(void);
unsigned char PC_to_RDR_ResetParameters(void);
unsigned char PC_to_RDR_SetParameters(void);
unsigned char PC_to_RDR_Escape(void);
unsigned char PC_to_RDR_IccClock(void);
unsigned char PC_to_RDR_Abort(void);

void RDR_to_PC_DataBlock(unsigned char ErrorCode);
void RDR_to_PC_SlotStatus(unsigned char ErrorCode);
void RDR_to_PC_SlotStatus_CardStopped(unsigned char ErrorCode);
void RDR_to_PC_Parameters(unsigned char ErrorCode);
void RDR_to_PC_Escape(unsigned char ErrorCode);

void RDR_to_PC_NotifySlotChange(void);
void RDR_to_PC_HardwareError(void);

void CmdNotSupported(void);

