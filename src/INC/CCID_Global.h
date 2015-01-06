/*
* Author: Copyright (C) Rudolf Boeddeker 											    Date: 2010-04-10
												STMicroelectronics	 											Date:	04/05/2000
*												MICROCONTROLLER DIVISION / ST Rousset		  Version V 0.96
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

/************************************************************************/
/* 		Constants Definitions											*/
/************************************************************************/

/************************************************************************/
/* 		Variables and Definition of authorized values					*/
/************************************************************************/

// Defines for USB_vSetup structure 
#define	REQ_ERROR				0			// Error on process
#define	REQ_SUCCESS			2			// Process sucessfully
#define	REQ_UNSUPPORT		4			// request unsupported, false EP,etc...

#define   USB_MESSAGE_BUFFER_LENGTH (272+30)

extern unsigned char	UsbMessageBuffer[USB_MESSAGE_BUFFER_LENGTH+50];			// + 50 for secure

#define USB_MESSAGE_BUFFER_MAX_LENGTH			(0x010F+10)
#define ICC_MESSAGE_BUFFER_MAX_LENGTH			(0x0105+10)
#define USB_MESSAGE_HEADER_SIZE					10


/* Offsets in UsbMessageBuffer for Bulk Out messages			*/

#define OFFSET_BMESSAGETYPE		  	0x00
#define OFFSET_DWLENGTH				    0x01
#define OFFSET_BSLOT				      0x05
#define OFFSET_BSEQ					      0x06
#define OFFSET_BPOWERSELECT		  	0x07
#define OFFSET_BBWI					      0x07
#define OFFSET_BPROTOCOLNUM_OUT		0x07
#define OFFSET_BCLOCKCOMMAND		  0x07
#define OFFSET_BMCHANGES			    0x07
#define OFFSET_BFUNCTION			    0x07
#define OFFSET_ABRFU_3B				    0x07
#define OFFSET_WLEVELPARAMETER		0x08
#define OFFSET_BCLASSGETRESPONSE	0x08
#define OFFSET_ABRFU_2B				    0x08
#define OFFSET_BCLASSENVELOPE		  0x09
#define OFFSET_ABDATA				      0x0A

/* Offsets in UsbMessageBuffer for Bulk In messages			*/
#define OFFSET_BSTATUS				    0x07
#define OFFSET_BERROR				      0x08
#define OFFSET_BCHAINPARAMETER		0x09
#define OFFSET_BCLOCKSTATUS			  0x09
#define OFFSET_BPROTOCOLNUM_IN		0x09
#define OFFSET_ABPROTOCOLDATASTRUCTURE	0x0A
#define OFFSET_BRFU					0x09

//#pragma DATA_SEG	CCID_VAR

extern unsigned char	CpuFrequency, IntTbuPeriod;

extern unsigned char	UsbIntMessageBuffer[4];

#define OFFSET_INT_BMESSAGETYPE			0x00
#define OFFSET_INT_BMSLOTICCSTATE		0x01
#define OFFSET_INT_BSLOT				0x01
#define OFFSET_INT_BSEQ					0x02
#define OFFSET_INT_BHARDWAREERRORCODE	0x03

extern unsigned char	AbortSequenceNumber;

extern unsigned char	SlotStabilizationDelay;
	#define INITIALSLOTSTABILIZATIONDELAY	100

extern unsigned char	KeyStabilizationDelay, KeyPushed;
	#define INITIALKEYSTABILIZATIONDELAY	200

extern unsigned int		HWErrorResetDelay;
	#define INITIALHWERRORRESETDELAY	1000

extern unsigned char	BlinkDelay, LedBlink;
	#define INITIALBLINKDELAY			100

//#pragma DATA_SEG SHORT	CCID_FLAGS

extern volatile unsigned char	UsbMessageFlags;
	#define BULKOUTCOMPLETEFLAG				0x01
	#define BULKINCOMPLETEFLAG				0x02
	#define PREVIOUSSLOTSTATEFLAG			0x04
	#define SLOTCHANGEDFLAG					0x08
	#define HARDWAREERRORFLAG				0x10
	#define ABORTREQUESTFLAG				0x20
	#define bBulkOutCompleteFlag						(UsbMessageFlags &  BULKOUTCOMPLETEFLAG)
	#define Set_bBulkOutCompleteFlag				(UsbMessageFlags |= BULKOUTCOMPLETEFLAG)
	#define Reset_bBulkOutCompleteFlag			(UsbMessageFlags &= (~BULKOUTCOMPLETEFLAG))
	#define bBulkInCompleteFlag							(UsbMessageFlags &  BULKINCOMPLETEFLAG)
	#define Set_bBulkInCompleteFlag					(UsbMessageFlags |= BULKINCOMPLETEFLAG)
	#define Reset_bBulkInCompleteFlag				(UsbMessageFlags &= (~BULKINCOMPLETEFLAG))
	#define bPreviousSlotStateFlag					(UsbMessageFlags &  PREVIOUSSLOTSTATEFLAG)
	#define Set_bPreviousSlotStateFlag			(UsbMessageFlags |= PREVIOUSSLOTSTATEFLAG)
	#define Reset_bPreviousSlotStateFlag		(UsbMessageFlags &= (~PREVIOUSSLOTSTATEFLAG))
	#define bSlotChangedFlag								(UsbMessageFlags & SLOTCHANGEDFLAG)
	#define Set_bSlotChangedFlag						(UsbMessageFlags |= SLOTCHANGEDFLAG)
	#define Reset_bSlotChangedFlag					(UsbMessageFlags &= (~SLOTCHANGEDFLAG))
	#define bHardwareErrorFlag							(UsbMessageFlags & HARDWAREERRORFLAG)
	#define Set_bHardwareErrorFlag					(UsbMessageFlags |= HARDWAREERRORFLAG)
	#define Reset_bHardwareErrorFlag				(UsbMessageFlags &= (~HARDWAREERRORFLAG))
	#define bAbortRequestFlag								(UsbMessageFlags & ABORTREQUESTFLAG)
	#define Set_bAbortRequestFlag						(UsbMessageFlags |= ABORTREQUESTFLAG)
	#define Reset_bAbortRequestFlag					(UsbMessageFlags &= (~ABORTREQUESTFLAG))


