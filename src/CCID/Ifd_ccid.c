/*
* Author: Copyright (C) Rudolf Boeddeker 											Date: 2010-01-13
*												STMicroelectronics / ST Rousset	 			Date:	22-V-2001
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
//#include "stm32f10x.h"


#include "smartcard.h"
#include "type.h"
#include "CCID_Global.h"
#include "CCID_Crd.h"
#include "CCID_SlotErrorCode.h"
#include "CCID_Ifd_protocol.h"
#include "CCID_Ifd_ccid.h"
#include "CCID_usb.h"
#include "CCID_Macro.h"

/************************************************************************************/
/************************************************************************************/
/*		BULK OUT ROUTINES																															*/
/************************************************************************************/

unsigned char PC_to_RDR_IccPowerOn(void)
{	
	if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
		return SLOTERROR_BAD_SLOT;

	if(UsbMessageBuffer[OFFSET_DWLENGTH] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+1] != 0
	   || UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0)
		return SLOTERROR_BAD_LENTGH;

	if( bAbortRequestFlag )
		return SLOTERROR_CMD_ABORTED;
	
	if (FALSE == RestartSmartcard())
	{
		return (SLOTERROR_HW_ERROR);
	}
	
	return (SLOT_NO_ERROR);
}

/************************************************************************/
/* ROUTINE 	unsigned char PC_to_RDR_IccPowerOff(void)										*/
/*                                                                      */
/*  PC_TO_RDR_ICCPOWEROFF message execution.														*/
/*    Execute the message in UsbMessageBuffer														*/
/*    Return in UsbMessageBuffer the answer.														*/
/************************************************************************/

unsigned char PC_to_RDR_IccPowerOff(void)
{
/*
	if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
		return SLOTERROR_BAD_SLOT;

	if(UsbMessageBuffer[OFFSET_DWLENGTH] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+1] != 0
		|| UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0)
		return SLOTERROR_BAD_LENTGH;

	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
		||UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERROR_BAD_ABRFU_3B;
*/	
//{unsigned long i;for (i=0;i<50000;i++);}
//  SC_Reset(Bit_RESET);
//	SmartcardOff ();
//SC_Init ();	
/*
{unsigned int i;for (i=0;i<50000;i++);}
{unsigned int i;for (i=0;i<50000;i++);}
{unsigned int i;for (i=0;i<50000;i++);}
//	Delay (1); // Wait for 10 ms
*/
	return SLOT_NO_ERROR;
}

/************************************************************************/
/* ROUTINE 	unsigned char PC_to_RDR_GetSlotStatus(void)									*/
/*                                                                      */
/*  PC_TO_RDR_GETSLOTSTATUS message execution.													*/
/*    Execute the message in UsbMessageBuffer														*/
/*    Return in UsbMessageBuffer the answer.														*/
/************************************************************************/


unsigned char PC_to_RDR_GetSlotStatus(void)
{
	unsigned char ErrorCode;
	unsigned char HwErrorCode;
	
	if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
		return SLOTERROR_BAD_SLOT;

	if(UsbMessageBuffer[OFFSET_DWLENGTH] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+1] != 0
		|| UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0)
		return SLOTERROR_BAD_LENTGH;

	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
		|| UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERROR_BAD_ABRFU_3B;
	
	ErrorCode = CRD_GetHwError(&HwErrorCode);

	return ErrorCode;
}

/************************************************************************/
/* ROUTINE 	unsigned char PC_to_RDR_XfrBlock(void)											*/
/*                                                                      */
/*  PC_TO_RDR_XFRBLOCK message execution.																*/
/*    Execute the message in UsbMessageBuffer														*/
/*    Return in UsbMessageBuffer the answer.														*/
/************************************************************************/

unsigned char PC_to_RDR_XfrBlock(void)
{
	unsigned char ErrorCode;
	unsigned char HwErrorCode;
	unsigned int BlockSize, ExpectedAnswerSize;
	unsigned int UsbMessageLength;
	
	if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
		return SLOTERROR_BAD_SLOT;

	BlockSize = MAKEWORD(UsbMessageBuffer[OFFSET_DWLENGTH+1],
							UsbMessageBuffer[OFFSET_DWLENGTH]);

	UsbMessageLength = USB_MESSAGE_HEADER_SIZE + BlockSize;

	if(UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0
		|| UsbMessageLength > USB_MESSAGE_BUFFER_MAX_LENGTH)
		return SLOTERROR_BAD_LENTGH;
	
	if( bAbortRequestFlag )
		return SLOTERROR_CMD_ABORTED;
	
	ErrorCode = CRD_GetHwError(&HwErrorCode);

	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;

// This parameter did not define the answer size RB ??? 	
	ExpectedAnswerSize = MAKEWORD(UsbMessageBuffer[OFFSET_WLEVELPARAMETER+1],
									              UsbMessageBuffer[OFFSET_WLEVELPARAMETER]);
	
	ErrorCode = IFD_XfrBlock(&UsbMessageBuffer[OFFSET_ABDATA], &BlockSize, ExpectedAnswerSize);
	
	#ifdef	USB_POLLING_MODEL
		USB_Polling();		// replace old lib function USB_Action ...
	#endif
	
	
	if( bAbortRequestFlag )
		return SLOTERROR_CMD_ABORTED;
	
	if(ErrorCode == SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_DWLENGTH]   = (unsigned char)  BlockSize;
		UsbMessageBuffer[OFFSET_DWLENGTH+1] = (unsigned char) (BlockSize>>8);
		UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0x00;
	}
	
	return ErrorCode;
}


/************************************************************************/
/* ROUTINE 	unsigned char PC_to_RDR_GetParameters(void)									*/
/*                                                                      */
/*  PC_TO_RDR_GETPARAMETERS message execution.													*/
/*    Execute the message in UsbMessageBuffer														*/
/*    Return in UsbMessageBuffer the answer.														*/
/************************************************************************/

unsigned char PC_to_RDR_GetParameters(void)
{
	unsigned char ErrorCode;
	unsigned char HwErrorCode;
	
	if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
		return SLOTERROR_BAD_SLOT;

	if((UsbMessageBuffer[OFFSET_DWLENGTH]   != 0) || 
	   (UsbMessageBuffer[OFFSET_DWLENGTH+1] != 0) || 
		 (UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0) || 
		 (UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0))
	{
		return SLOTERROR_BAD_LENTGH;
	}

	if((UsbMessageBuffer[OFFSET_ABRFU_3B]   != 0) || 
	   (UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0) || 
		 (UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0))
	{
		return SLOTERROR_BAD_ABRFU_3B;
	}

	ErrorCode = IFD_SetParameters(&UsbMessageBuffer[OFFSET_ABDATA], 0x00);

	if(ErrorCode != SLOT_NO_ERROR)
	{
		return ErrorCode;
	}

	ErrorCode = CRD_GetHwError(&HwErrorCode);

	return ErrorCode;
}


/************************************************************************/
/* ROUTINE 	unsigned char PC_to_RDR_ResetParameters(void)								*/
/*                                                                      */
/*  PC_TO_RDR_RESETPARAMETERS message execution.												*/
/*    Execute the message in UsbMessageBuffer														*/
/*    Return in UsbMessageBuffer the answer.														*/
/************************************************************************/

unsigned char PC_to_RDR_ResetParameters(void)
{
	unsigned char ErrorCode;
	unsigned char HwErrorCode;
	
	if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
		return SLOTERROR_BAD_SLOT;

	if(UsbMessageBuffer[OFFSET_DWLENGTH] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+1] != 0
		|| UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0)
		return SLOTERROR_BAD_LENTGH;

	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
		|| UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERROR_BAD_ABRFU_3B;
	
	UsbMessageBuffer[OFFSET_ABDATA]   = DEFAULT_FIDI;
	UsbMessageBuffer[OFFSET_ABDATA+1] = DEFAULT_T01CONVCHECKSUM;
	UsbMessageBuffer[OFFSET_ABDATA+2] = DEFAULT_GUARDTIME;
	UsbMessageBuffer[OFFSET_ABDATA+3] = DEFAULT_WAITINGINTEGER;
	UsbMessageBuffer[OFFSET_ABDATA+4] = DEFAULT_CLOCKSTOP;
	UsbMessageBuffer[OFFSET_ABDATA+5] = 0x00;
	UsbMessageBuffer[OFFSET_ABDATA+6] = 0x00;
	
	ErrorCode = IFD_SetParameters(&UsbMessageBuffer[OFFSET_ABDATA], 0x00);

	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;
	
	ErrorCode = CRD_GetHwError(&HwErrorCode);

	return ErrorCode;
}


/************************************************************************/
/* ROUTINE 	unsigned char PC_to_RDR_SetParameters(void)									*/
/*                                                                      */
/*  PC_TO_RDR_SETPARAMETERS message execution.													*/
/*    Execute the message in UsbMessageBuffer														*/
/*    Return in UsbMessageBuffer the answer.														*/
/************************************************************************/

unsigned char PC_to_RDR_SetParameters(void)
{
	unsigned char ErrorCode;
	unsigned char HwErrorCode;
	
	if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
		return SLOTERROR_BAD_SLOT;

	if( UsbMessageBuffer[OFFSET_DWLENGTH+1] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0
		|| UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0
		|| ((UsbMessageBuffer[OFFSET_DWLENGTH] != 0x05)
				&& (UsbMessageBuffer[OFFSET_DWLENGTH] != 0x07)) )
		return SLOTERROR_BAD_LENTGH;

// RB Germalto driver set wrong parameter ??? protocol = T0 > Length = 7 ? (=T1)
	if ((UsbMessageBuffer[OFFSET_DWLENGTH]         == 0x07)	&& 
	    (UsbMessageBuffer[OFFSET_BPROTOCOLNUM_OUT] != 0x01))
	{
//		UsbMessageBuffer[OFFSET_BPROTOCOLNUM_OUT] = 0x01;						 // Set to T1
		UsbMessageBuffer[OFFSET_DWLENGTH]         = 0x05;							 // Set to T0
	}

	if( ((UsbMessageBuffer[OFFSET_DWLENGTH] == 0x05)
				&& (UsbMessageBuffer[OFFSET_BPROTOCOLNUM_OUT] != 0x00))
		|| ((UsbMessageBuffer[OFFSET_DWLENGTH] == 0x07)
				&& (UsbMessageBuffer[OFFSET_BPROTOCOLNUM_OUT] != 0x01)) )
		return SLOTERROR_BAD_PROTOCOLNUM;



	if(UsbMessageBuffer[OFFSET_ABRFU_2B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_2B+1] != 0)
		return SLOTERROR_BAD_ABRFU_2B;
	
//	ErrorCode = IFD_SetParameters(&UsbMessageBuffer[OFFSET_ABDATA],	UsbMessageBuffer[OFFSET_BPROTOCOLNUM_OUT]);

ErrorCode = SLOT_NO_ERROR;

	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;

// Answer of SetParameters	Test only for OpenPGG cards
	UsbMessageBuffer[OFFSET_BMESSAGETYPE]   = RDR_TO_PC_PARAMETERS;
	UsbMessageBuffer[OFFSET_BSTATUS] 			  = CRD_GetSlotStatus();
	UsbMessageBuffer[OFFSET_BERROR] 			  = 0x00;
	
	UsbMessageBuffer[OFFSET_DWLENGTH]       = (unsigned char) 7;				// Protocol T=1
	UsbMessageBuffer[OFFSET_DWLENGTH+1]     = (unsigned char) (7>>8);
	UsbMessageBuffer[OFFSET_DWLENGTH+2]     = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+3]     = 0x00;

	UsbMessageBuffer[OFFSET_BCLASSENVELOPE] = 0x01;				// Protocol T=1
	
	ErrorCode = CRD_GetHwError(&HwErrorCode);

	return ErrorCode;
}


/************************************************************************/
/* ROUTINE 	unsigned char PC_to_RDR_Escape(void)												*/
/*                                                                      */
/*  PC_TO_RDR_ESCAPE message execution.																	*/
/*    Execute the message in UsbMessageBuffer														*/
/*    Return in UsbMessageBuffer the answer.														*/
/************************************************************************/


unsigned char PC_to_RDR_Escape(void)
{
	unsigned char ErrorCode;
	unsigned char HwErrorCode;
	unsigned int BlockSize, UsbMessageLength;
	
	if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
		return SLOTERROR_BAD_SLOT;

	BlockSize = MAKEWORD(UsbMessageBuffer[OFFSET_DWLENGTH+1],
							UsbMessageBuffer[OFFSET_DWLENGTH]);

	UsbMessageLength = USB_MESSAGE_HEADER_SIZE + BlockSize;

	if(UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0
		|| UsbMessageLength > USB_MESSAGE_BUFFER_MAX_LENGTH)
		return SLOTERROR_BAD_LENTGH;

	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
			|| UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERROR_BAD_ABRFU_3B;
	
	if( bAbortRequestFlag )
		return SLOTERROR_CMD_ABORTED;
	
	ErrorCode = IFD_Escape(&UsbMessageBuffer[OFFSET_ABDATA], &BlockSize);
	
	// USB_Action();
	#ifdef	USB_POLLING_MODEL
		USB_Polling();		// replace old lib function USB_Action ...
	#endif
	
	if( bAbortRequestFlag )
		return SLOTERROR_CMD_ABORTED;
	
	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;
	
	UsbMessageBuffer[OFFSET_DWLENGTH]   = (unsigned char) BlockSize;
	UsbMessageBuffer[OFFSET_DWLENGTH+1] = (unsigned char) (BlockSize>>8);
	UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0x00;
	
	ErrorCode = CRD_GetHwError(&HwErrorCode);
	return ErrorCode;
}


/************************************************************************/
/* ROUTINE 	unsigned char PC_to_RDR_IccClock(void)											*/
/*                                                                      */
/*  PC_TO_RDR_ICCCLOCK message execution.																*/
/*    Execute the message in UsbMessageBuffer														*/
/*    Return in UsbMessageBuffer the answer.														*/
/************************************************************************/

unsigned char PC_to_RDR_IccClock(void)
{
	unsigned char ErrorCode;
	unsigned char HwErrorCode;
	
	if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
		return SLOTERROR_BAD_SLOT;

	if(UsbMessageBuffer[OFFSET_DWLENGTH] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+1] != 0
	   || UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0)
		return SLOTERROR_BAD_LENTGH;

	if(UsbMessageBuffer[OFFSET_BCLOCKCOMMAND] > 0x01)
		return SLOTERROR_BAD_CLOCKCOMMAND;

	if(UsbMessageBuffer[OFFSET_ABRFU_2B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_2B+1] != 0)
		return SLOTERROR_BAD_ABRFU_2B;
	
	ErrorCode = IFD_SetClock(UsbMessageBuffer[OFFSET_BCLOCKCOMMAND]);

	if(ErrorCode != SLOT_NO_ERROR)
		return ErrorCode;
	
	ErrorCode = CRD_GetHwError(&HwErrorCode);

	return ErrorCode;
};



/************************************************************************/
/* ROUTINE 	unsigned char PC_to_RDR_Abort(void)													*/
/*                                                                      */
/*  PC_TO_RDR_ABORT message execution.																	*/
/*    Execute the message in UsbMessageBuffer														*/
/*    Return in UsbMessageBuffer the answer.														*/
/************************************************************************/

unsigned char PC_to_RDR_Abort(void)
{
//	unsigned char ErrorCode;
	
	if(UsbMessageBuffer[OFFSET_BSLOT] != 0)
		return SLOTERROR_BAD_SLOT;

	if(UsbMessageBuffer[OFFSET_DWLENGTH] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+1] != 0
	   || UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0 || UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0)
		return SLOTERROR_BAD_LENTGH;

	if(UsbMessageBuffer[OFFSET_ABRFU_3B] != 0 || UsbMessageBuffer[OFFSET_ABRFU_3B+1] != 0
			|| UsbMessageBuffer[OFFSET_ABRFU_3B+2] != 0)
		return SLOTERROR_BAD_ABRFU_3B;
	
	if( (!bAbortRequestFlag) || (AbortSequenceNumber != UsbMessageBuffer[OFFSET_BSEQ]) )
		return SLOTERROR_CMD_ABORTED;
	
	Reset_bAbortRequestFlag;
	
	return SLOT_NO_ERROR;
}



/************************************************************************************/
/************************************************************************************/
/*		BULK IN ROUTINES																															*/
/************************************************************************************/

/************************************************************************/
/* ROUTINE 	void RDR_to_PC_DataBlock(unsigned char ErrorCode)						*/
/*                                                                      */
/*    Fill Header of UsbMessageBuffer.																	*/
/************************************************************************/

void RDR_to_PC_DataBlock(unsigned char ErrorCode)
{
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_DATABLOCK;
	UsbMessageBuffer[OFFSET_BSTATUS]      = CRD_GetSlotStatus();
	UsbMessageBuffer[OFFSET_BERROR]       = 0x00;

	if(ErrorCode != SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_BSTATUS] += 0x40;
		UsbMessageBuffer[OFFSET_DWLENGTH] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+1] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0x00;
		UsbMessageBuffer[OFFSET_BERROR] = ErrorCode;
	}

	UsbMessageBuffer[OFFSET_BCHAINPARAMETER] = 0x00;
}

/************************************************************************/
/* ROUTINE 	void RDR_to_PC_SlotStatus(unsigned char ErrorCode)					*/
/*                                                                      */
/*    Fill Header of UsbMessageBuffer.																	*/
/************************************************************************/

void RDR_to_PC_SlotStatus(unsigned char ErrorCode)
{
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_SLOTSTATUS;
	UsbMessageBuffer[OFFSET_BSTATUS] 			= CRD_GetSlotStatus();
	UsbMessageBuffer[OFFSET_BERROR]				= 0x00;

	if(ErrorCode != SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_BSTATUS]    += 0x40;
		UsbMessageBuffer[OFFSET_DWLENGTH]    = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+1]  = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+2]  = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+3]  = 0x00;
		UsbMessageBuffer[OFFSET_BERROR]      = ErrorCode;
	}

	UsbMessageBuffer[OFFSET_BCLOCKSTATUS] = CRD_GetClockStatus();
}

/************************************************************************/
/* ROUTINE 	void RDR_to_PC_SlotStatus(unsigned char ErrorCode)					*/
/*                                                                      */
/*    Fill Header of UsbMessageBuffer.																	*/
/************************************************************************/

void RDR_to_PC_SlotStatus_CardStopped (unsigned char ErrorCode)
{
	RDR_to_PC_SlotStatus(ErrorCode);

	UsbMessageBuffer[OFFSET_BSTATUS] 			= CRD_PRESENTINACTIVE;
	UsbMessageBuffer[OFFSET_BCLOCKSTATUS] = CRD_CLOCKSTOPPEDLOW;
}

/************************************************************************/
/* ROUTINE 	void RDR_to_PC_Parameters(unsigned char ErrorCode)					*/
/*                                                                      */
/*    Fill Header of UsbMessageBuffer.																	*/
/************************************************************************/


void RDR_to_PC_Parameters(unsigned char ErrorCode)
{
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_PARAMETERS;
	UsbMessageBuffer[OFFSET_BSTATUS] 			= CRD_GetSlotStatus();
	UsbMessageBuffer[OFFSET_DWLENGTH+1] 	= 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+2] 	= 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+3] 	= 0x00;
	UsbMessageBuffer[OFFSET_BERROR] 			= 0x00;

	if(ErrorCode != SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_BSTATUS] += 0x40;
		UsbMessageBuffer[OFFSET_DWLENGTH] = 0x00;
		UsbMessageBuffer[OFFSET_BERROR] 	= ErrorCode;
		return;
	}
	
	UsbMessageBuffer[OFFSET_BPROTOCOLNUM_IN]
		= IFD_GetParameters(&UsbMessageBuffer[OFFSET_ABDATA]);
	
	if(UsbMessageBuffer[OFFSET_BPROTOCOLNUM_IN])
	{	
		UsbMessageBuffer[OFFSET_DWLENGTH] = 0x07;	
	}
	else
	{	
		UsbMessageBuffer[OFFSET_DWLENGTH] = 0x05;	
	}
}

/************************************************************************/
/* ROUTINE 	void RDR_to_PC_DataBlock(unsigned char ErrorCode)						*/
/*                                                                      */
/*    Fill Header of UsbMessageBuffer.																	*/
/************************************************************************/

void RDR_to_PC_Escape(unsigned char ErrorCode)
{
	UsbMessageBuffer[OFFSET_BMESSAGETYPE] = RDR_TO_PC_ESCAPE;
	UsbMessageBuffer[OFFSET_BSTATUS] 			= CRD_GetSlotStatus();
	UsbMessageBuffer[OFFSET_BERROR] 			= 0x00;

	if(ErrorCode != SLOT_NO_ERROR)
	{
		UsbMessageBuffer[OFFSET_BSTATUS]    += 0x40;
		UsbMessageBuffer[OFFSET_DWLENGTH]    = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+1]  = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+2]  = 0x00;
		UsbMessageBuffer[OFFSET_DWLENGTH+3]  = 0x00;
		UsbMessageBuffer[OFFSET_BERROR]      = ErrorCode;
	}

	UsbMessageBuffer[OFFSET_BRFU] = 0x00;
}


/************************************************************************/
/* ROUTINE 	void CmdNotSupported(void)																	*/
/*                                                                      */
/*    Fill Header of UsbMessageBuffer.																	*/
/************************************************************************/

void CmdNotSupported(void)
{
	//UsbMessageBuffer[OFFSET_BMESSAGETYPE] = 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH] 				= 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+1] 			= 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+2] 			= 0x00;
	UsbMessageBuffer[OFFSET_DWLENGTH+3]		 		= 0x00;
	UsbMessageBuffer[OFFSET_BSTATUS] 					= 0x40 + CRD_GetSlotStatus();
	UsbMessageBuffer[OFFSET_BERROR] 					= 0x00;
	UsbMessageBuffer[OFFSET_BCHAINPARAMETER] 	= 0x00;
};


/************************************************************************************/
/************************************************************************************/
/*		INTERRUPT IN ROUTINES																													*/
/************************************************************************************/

/************************************************************************/
/* ROUTINE 	void RDR_to_PC_NotifySlotChange(void)												*/
/************************************************************************/

void RDR_to_PC_NotifySlotChange(void)
{

	UsbIntMessageBuffer[OFFSET_INT_BMESSAGETYPE] = RDR_TO_PC_NOTIFYSLOTCHANGE;
	
	if( CRD_GetSlotStatus() == CRD_NOTPRESENT )
	{	
		UsbIntMessageBuffer[OFFSET_INT_BMSLOTICCSTATE] = 0x02;	
	}
	else
	{	
		UsbIntMessageBuffer[OFFSET_INT_BMSLOTICCSTATE] = 0x03;	
	}
}

/************************************************************************/
/* ROUTINE 	void RDR_to_PC_HardwareError(void)													*/
/************************************************************************/

void RDR_to_PC_HardwareError(void)
{
	unsigned char HwErrorCode = SLOT_NO_ERROR;
//	unsigned char ErrorCode;
	
	UsbIntMessageBuffer[OFFSET_INT_BMESSAGETYPE] 				= RDR_TO_PC_HARDWAREERROR;
	UsbIntMessageBuffer[OFFSET_INT_BSLOT] 							= 0x00;
	UsbIntMessageBuffer[OFFSET_INT_BSEQ] 								= UsbMessageBuffer[OFFSET_BSEQ];
//	ErrorCode 																					= CRD_GetHwError(&HwErrorCode);
	UsbIntMessageBuffer[OFFSET_INT_BHARDWAREERRORCODE] 	= HwErrorCode;
}

