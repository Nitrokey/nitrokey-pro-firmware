/*
 * Author: Copyright (C) Rudolf Boeddeker                                                   Date: 2010-01-13
 *                                              STMicroelectronics                                              Date:   29-V-2001
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



#define		XFR_NOERROR							0x00
#define		XFR_NOANSWER						GETATR_NOANSWER
#define		XFR_BADLEVELPARAMETER   SLOTERROR_BAD_LEVELPARAMETER
#define		XFR_BUFFEROVERFLOW			GETATR_BUFFEROVERFLOW
#define		XFR_PARITYERROR					GETATR_PARITYERROR


// static volatile unsigned char IccTransactionLevelType;
#define TYPE_MASK					0x01
#define LEVEL_MASK				0x06

#define T0_TYPE						0x00
#define T1_TYPE						0x01

#define CHARACTER_LEVEL		0x00
#define TPDU_LEVEL				0x02
#define SHORTAPDU_LEVEL		0x04

#define bTransactionType							(IccTransactionLevelType & 		TYPE_MASK)
#define SetT0_bTransactionType				(IccTransactionLevelType &= (~TYPE_MASK))
#define SetT1_bTransactionType				(IccTransactionLevelType |= 	TYPE_MASK)
#define bmTransactionLevel						(IccTransactionLevelType & 		LEVEL_MASK)
#define SetChar_bmTransactionLevel		(IccTransactionLevelType &= (~LEVEL_MASK))


#define		INS			0x00
#define		DATA		0x01


#define		DEFAULT_FIDI							0x11
#define		DEFAULT_T01CONVCHECKSUM		0x00
#define		DEFAULT_GUARDTIME					0x00
#define		DEFAULT_WAITINGINTEGER		0x0A
#define		DEFAULT_CLOCKSTOP					0x03
#define		DEFAULT_IFSC							0x20
#define		DEFAULT_NAD								0x00

#define		PARAM_CLOCK_NOTSTOPPED		0x00
#define		PARAM_CLOCK_STOPLOW				0x01
#define		PARAM_CLOCK_STOPHIGH			0x02
#define		PARAM_CLOCK_STOPHIGHORLOW	0x03



void IFD_Init (void);

unsigned char IFD_ApplyParametersStructure (void);

void IFD_UpdateConvParameterStructure (void);

unsigned char IFD_IccPowerOn (unsigned char, unsigned char* , unsigned int* );

unsigned char IFD_XfrBlock (unsigned char* , unsigned int* , unsigned int);

unsigned char IFD_XfrCharT0 (unsigned char* , unsigned int* , unsigned int);

unsigned char IFD_XfrCharT1 (unsigned char* , unsigned int* );

unsigned char IFD_XfrTpduT0 (unsigned char* , unsigned int* );

unsigned char IFD_XfrTpduT1 (unsigned char* , unsigned int* );

unsigned char IFD_GetParameters (unsigned char* );

unsigned char IFD_SetParameters (unsigned char* , unsigned char);

unsigned char IFD_Escape (unsigned char* , unsigned int* );

unsigned char IFD_SetClock (unsigned char ClockCmd);

unsigned char IFD_SetATRData (void);

int GetExpectedAnswerSizeFromAPDU (unsigned char* pAPDU, int nSize);

void IFD_EscapeSendHwName (unsigned char* pBlockBuffer, unsigned int* pBlockSize);
