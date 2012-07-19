/*
* Author: Copyright (C) Rudolf Boeddeker 													Date: 2010-01-13
*												STMicroelectronics	 											Date:	04/05/2000
*												 MICROCONTROLLER DIVISION / ST Rousset		Version V 0.96
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




#define		PC_TO_RDR_ICCPOWERON										0x62
#define		PC_TO_RDR_ICCPOWEROFF										0x63
#define		PC_TO_RDR_GETSLOTSTATUS									0x65
#define		PC_TO_RDR_XFRBLOCK											0x6F
#define		PC_TO_RDR_GETPARAMETERS									0x6C
#define		PC_TO_RDR_RESETPARAMETERS								0x6D
#define		PC_TO_RDR_SETPARAMETERS									0x61
#define		PC_TO_RDR_ESCAPE												0x6B
#define		PC_TO_RDR_ICCCLOCK											0x6E
#define		PC_TO_RDR_T0APDU												0x6A
#define		PC_TO_RDR_SECURE												0x69
#define		PC_TO_RDR_MECHANICAL										0x71
#define		PC_TO_RDR_ABORT													0x72
#define		PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY	0x73

#define		RDR_TO_PC_DATABLOCK											0x80
#define		RDR_TO_PC_SLOTSTATUS										0x81
#define		RDR_TO_PC_PARAMETERS										0x82
#define		RDR_TO_PC_ESCAPE												0x83
#define		RDR_TO_PC_DATARATEANDCLOCKFREQUENCY			0x84

#define		RDR_TO_PC_NOTIFYSLOTCHANGE							0x50
#define		RDR_TO_PC_HARDWAREERROR									0x51

#define		CCIDCLASSREQUEST_ABORT									0x01
#define		CCIDCLASSREQUEST_GET_CLOCK_FREQUENCIES	0x02
#define		CCIDCLASSREQUEST_GET_DATA_RATES					0x03


/************************************************************************/
/* 			Routines																												*/
/************************************************************************/
 
void CCID_Init(void);
void CCID_Init_IT(void);
void CCID_Suspend_IT(void);
void CCID_Bot_Abort(unsigned char Direction);
void CCID_Storage_Out (void);

void CCID_BulkOutMessage(void);
unsigned char CCID_BulkInMessage(void);
void CCID_DispatchMessage(void);

void CCID_IntMessage(void);

void CcidClassRequestAbort(void);
void CcidClassRequestGetClockFrequencies(void);
void CcidClassRequestGetDataRates(void);

void CCID_SendCardDetect(void);
void CCID_CheckUsbCommunication(void);
void CCID_SetCardState (unsigned char nState);
unsigned char CCID_GetCardState (void);


