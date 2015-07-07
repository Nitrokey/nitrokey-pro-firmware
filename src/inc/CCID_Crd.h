/*
 * Author: Copyright (C) STMicroelectronics                                             Date:   22-V-2001
 *                                               MICROCONTROLLER DIVISION / ST Rousset      Version V 0.00
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

#define		CRD_VOLTAGEAUTO						0x00
#define		CRD_VOLTAGE5V							0x01
#define		CRD_VOLTAGE3V							0x02
#define		CRD_VOLTAGE18V						0x03

#define		CRD_MANUALMODE						0x01
#define		CRD_UARTMODE							0x02

#define		CRD_DIRECTCONV						0x00
#define		CRD_INVERSECONV						0x01

#define		CRD_PRESENTACTIVE					0x00
#define		CRD_PRESENTINACTIVE				0x01
#define		CRD_NOTPRESENT						0x02

#define		CRD_CLOCKRUNNING					0x00
#define		CRD_CLOCKSTOPPEDLOW				0x01
#define		CRD_CLOCKSTOPPEDHIGH			0x02
#define		CRD_CLOCKSTOPPEDLOWORHIGH	0x03


// For ATR reading : answer must arrive before 40000 CLK
// = 10 ms ; with ETU = 93 us : 107 ETU + Margin
// #define ATR_WAITINGTIME 110

// Different waiting time for some cards (60 ms between 1st byte
// and 2nd one : example : GemSAFE Logon card)
#define		POWERUP_WAITINGTIME		110
#define		ATR_WAITINGTIME			1000


// #pragma DATA_SEG SHORT CRD_LIB_RAM

extern unsigned char* pCrdBuffer;

extern volatile unsigned char CrdFlags;

#define		PARITYERRORFLAG						0x01
#define		VOLTAGEERRORFLAG					0x02
#define		CURRENTERRORFLAG					0x04
#define		WAITINGTIMEFLAG						0x08
#define		BUFFEROVERFLOWFLAG				0x10
#define		PROTOCOLTYPEFLAG					0x20
#define		PROCEDUREBYTEFLAG					0x40
#define		bParityErrorFlag					(CrdFlags & PARITYERRORFLAG)
#define		Set_bParityErrorFlag			(CrdFlags |= PARITYERRORFLAG)
#define		Reset_bParityErrorFlag		(CrdFlags &= (~PARITYERRORFLAG))
#define		bVoltageErrorFlag					(CrdFlags & VOLTAGEERRORFLAG)
#define		Set_bVoltageErrorFlag			(CrdFlags |= VOLTAGEERRORFLAG)
#define		Reset_bVoltageErrorFlag		(CrdFlags &= (~VOLTAGEERRORFLAG))
#define		bCurrentErrorFlag					(CrdFlags & CURRENTERRORFLAG)
#define		Set_bCurrentErrorFlag			(CrdFlags |= CURRENTERRORFLAG)
#define		Reset_bCurrentErrorFlag		(CrdFlags &= (~CURRENTERRORFLAG))
#define		bWaitingTimeFlag					(CrdFlags & WAITINGTIMEFLAG)
#define		Set_bWaitingTimeFlag			(CrdFlags |= WAITINGTIMEFLAG)
#define		Reset_bWaitingTimeFlag		(CrdFlags &= (~WAITINGTIMEFLAG))
#define		bBufferOverflowFlag				(CrdFlags & BUFFEROVERFLOWFLAG)
#define		Set_bBufferOverflowFlag		(CrdFlags |= BUFFEROVERFLOWFLAG)
#define		Reset_bBufferOverflowFlag	(CrdFlags &= (~BUFFEROVERFLOWFLAG))
#define		bProtocolTypeFlag					(CrdFlags & PROTOCOLTYPEFLAG)
#define		Set_bProtocolTypeFlag			(CrdFlags |= PROTOCOLTYPEFLAG)
#define		Reset_bProtocolTypeFlag		(CrdFlags &= (~PROTOCOLTYPEFLAG))
#define		bProcedureByteFlag				(CrdFlags & PROCEDUREBYTEFLAG)
#define		Set_bProcedureByteFlag		(CrdFlags |= PROCEDUREBYTEFLAG)
#define		Reset_bProcedureByteFlag	(CrdFlags &= (~PROCEDUREBYTEFLAG))


extern unsigned int CrdNbData;

extern unsigned int CrdBufferSize;

extern unsigned char InstructionByte, ProcedureByte;

#define		DEFAULT_INSTRUCTION_BYTE				0x01
#define		DEFAULT_PROCEDURE_BYTE					0x03
#define		PROCEDUREBYTE_NULL							0x60



void CRD_Init (void);

unsigned char CRD_GetHwError (unsigned char* );

unsigned char CRD_VccOn (unsigned char);

void CRD_VccOff (void);

void CRD_VccOff_IT (void);

void CRD_SetMode (unsigned char);

void CRD_SetConvention (unsigned char);

unsigned char CRD_GetSlotStatus (void);

unsigned char CRD_GetConvention (void);

unsigned char CRD_GetClockStatus (void);

unsigned char CRD_SetClock (unsigned char);

unsigned char CRD_SetEtu (unsigned int, unsigned char);

void CRD_SetGuardTime (unsigned int);

void CRD_SetWaitingTime (unsigned long);

void CRD_StartWaitingTime (void);

void CRD_StopWaitingTime (void);

void CRD_WaitingTime (unsigned long);

void CRD_InitReceive (unsigned int, unsigned char* );

unsigned int CRD_NumberOfBytesReceived (void);

unsigned int CRD_EndReceive (void);

void CRD_InitTransmit (unsigned int, unsigned char* );

unsigned int CRD_NumberOfBytesToTransmit (void);

unsigned int CRD_EndTransmit (void);

void CRD_CharRepeatOn (void);

void CRD_CharRepeatOff (void);

unsigned char ICC_PowerOnAsync (unsigned char);

void ICC_ResetAsync (void);

unsigned char ICC_GetAtrAsync (unsigned char, unsigned char* , unsigned int* );
unsigned char ICC_SendCommandAsync (unsigned char* , unsigned int, unsigned char);
unsigned char ICC_ReceiveAnswerAsync (unsigned char* , unsigned int* );
