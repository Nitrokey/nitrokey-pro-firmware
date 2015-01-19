/*
* Author: Copyright (C) Rudolf Boeddeker 									Date: 2010-01-13
*												STMicroelectronics / ST Rousset	 	Date:	04/05/2000
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

#include "memory.h"
#include "memory.h"
#include "stm32f10x_usart.h"
#include "usb_scsi.h"
#include "usb_bot.h"
#include "usb_regs.h"
#include "usb_mem.h"
#include "usb_core.h"
#include "CCIDHID_usb_prop.h"
#include "CCIDHID_usb_conf.h"
#include "smartcard.h"
#include "hw_config.h"
#include "mass_mal.h"
#include "CCID_SlotErrorCode.h"

extern __IO uint32_t Block_Read_count;
extern __IO uint32_t Block_offset;
extern __IO uint32_t Counter;
extern uint8_t TransferState;

extern uint8_t Bulk_Data_Buff[BULK_MAX_PACKET_SIZE];  /* data buffer*/
extern uint16_t Data_Len;
extern uint8_t Bot_State;
extern Bulk_Only_CBW CBW;
extern Bulk_Only_CSW CSW;
extern uint32_t Mass_Memory_Size[2];
extern uint32_t Mass_Block_Size[2];


// old
#include "type.h"

#include "CCID_Global.h"
#include "CCID_Crd.h"
#include "CCID_Macro.h"
#include "CCID_Ifd_ccid.h"
#include "CCID_usb.h"
#include "CCID_Ifd_protocol.h"


// Defines for USB_vSetup structure 
#define	REQ_ERROR				0			// Error on process
#define	REQ_SUCCESS			2			// Process sucessfully
#define	REQ_UNSUPPORT		4			// request unsupported, false EP,etc...


#define RECEIVE_FIRST_PART_INIT						0x00
#define RECEIVE_FIRST_PART_END						0x01

#define RECEIVE_OTHER_INIT								0x02
#define RECEIVE_OTHER_END									0x03

#define RECEIVE_UNCORRECTLENGTH_INIT			0x04
#define RECEIVE_UNCORRECTLENGTH_END				0x05

#define RECEIVE_TOOLONGMESSAGE_FINISHED		0x06
#define RECEIVE_FINISHED									0x07
#define TRANSMIT_HEADER										0x08
#define TRANSMIT_OTHER										0x09
#define TRANSMIT_FINISHED									0x0A

unsigned char	UsbMessageBuffer[USB_MESSAGE_BUFFER_LENGTH+50];
unsigned char	UsbIntMessageBuffer[4];

volatile unsigned char	UsbMessageFlags;
unsigned char	AbortSequenceNumber;

unsigned char		        BulkStatus;
static int							UsbMessageLength;
static unsigned char *	pUsbMessageBuffer;

static unsigned char    cCRD_CardPresent = FALSE; 			// Flag card present


/************************************************************************/
/* ROUTINE 	void CCID_Init(void)																				*/
/*                                                                      */
/*  Set variables with default values.																	*/
/************************************************************************/

void CCID_Init(void)
{
	pUsbMessageBuffer = UsbMessageBuffer;
	BulkStatus        = RECEIVE_FIRST_PART_INIT;
	UsbMessageFlags   = 0x00;	
	CrdFlags          = 0x00;
}

/************************************************************************/
/* ROUTINE 	void CCID_Init_IT(void)																			*/
/*                                                                      */
/*  Set variables with default values after a USB bus reset interrupt.	*/
/************************************************************************/

void CCID_Init_IT(void)
{

	pUsbMessageBuffer 	= UsbMessageBuffer;
	BulkStatus 					= RECEIVE_FIRST_PART_INIT;
	UsbMessageFlags 		= 0x00;
	
	/* Check a card presence  */
	if(TRUE == cCRD_CardPresent) 
	{
		Set_bSlotChangedFlag;
	}
	
	/* Switch off CardVcc   */
	CRD_VccOff_IT();
	
	CrdFlags = 0x00;
}

/************************************************************************/
/* ROUTINE 	void CCID_Suspend_IT(void)																	*/
/*                                                                      */
/*  Set variables with default values after a USB bus reset interrupt.	*/
/************************************************************************/

void CCID_Suspend_IT(void)
{
	pUsbMessageBuffer = UsbMessageBuffer;
	BulkStatus = RECEIVE_FIRST_PART_INIT;
	UsbMessageFlags = 0x00;
	
	/* Check a card presence  */
	if(TRUE == cCRD_CardPresent)
	{
		Set_bSlotChangedFlag;
	}
	
	/* Switch off CardVcc   */
	CRD_VccOff_IT();
	
	CrdFlags = 0x00;
};

/*******************************************************************************
* Function Name  : Bot_Abort
* Description    : Stall the needed Endpoint according to the selected direction.
* Input          : Endpoint direction IN, OUT or both directions
* Output         : None.
* Return         : None.
*******************************************************************************/
void CCID_Bot_Abort(uint8_t Direction)
{
  switch (Direction)
  {
    case DIR_IN :
      SetEPTxStatus(ENDP2, EP_TX_STALL);
      break;
    case DIR_OUT :
      SetEPRxStatus(ENDP2, EP_RX_STALL);
      break;
    case BOTH_DIR :
      SetEPTxStatus(ENDP2, EP_TX_STALL);
      SetEPRxStatus(ENDP2, EP_RX_STALL);
      break;
    default:
      break;
  }
}

/*******************************************************************************
* Function Name  : CCID_Storage_Out
* Description    : CCID Storage OUT transfer.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

void CCID_Storage_Out (void)
{
  Data_Len = GetEPRxCount(ENDP2);

  PMAToUserBufferCopy(Bulk_Data_Buff, CCID_ENDP2_RXADDR, Data_Len);

  switch (Bot_State)
  {
    case BOT_IDLE:
      break;

    case BOT_DATA_OUT:
      break;

    default:
      break;
  }
}


/************************************************************************/
/* ROUTINE 	void CCID_BulkOutMessage(void)															*/
/*                                                                      */
/*  Receive the CCID message by the Bulk In Endpoint 2.                 */
/*    save this message in UsbMessageBuffer.                            */
/************************************************************************/
static int UsbDataLength = 0;

void CCID_BulkOutMessage (void)
{
	unsigned char cnt;

	if (RECEIVE_FIRST_PART_INIT	== BulkStatus)
	{
	  pUsbMessageBuffer	= UsbMessageBuffer; 				// max USB_MESSAGE_BUFFER_LENGTH -  BulkOut Byte 
		UsbDataLength     = 0;
	}

/* Get Data into Buffer */
	cnt = GetEPRxCount(ENDP2);			 																		
	PMAToUserBufferCopy(pUsbMessageBuffer, CCID_ENDP2_RXADDR, cnt);			// copy data in USB buffer

	UsbDataLength     += cnt;
	pUsbMessageBuffer  = &pUsbMessageBuffer[cnt];											  // set new the end of the buffer


/* Get the message length of the transfer */
	if (RECEIVE_FIRST_PART_INIT	== BulkStatus)
	{
/* Calculate number of byte to receive to finish the message  */
		UsbMessageLength  = USB_MESSAGE_HEADER_SIZE;
		UsbMessageLength += UsbMessageBuffer[OFFSET_DWLENGTH];
		UsbMessageLength += UsbMessageBuffer[OFFSET_DWLENGTH+1] << 8;

/* check for length errors */
		if((UsbMessageBuffer[OFFSET_DWLENGTH+2] != 0) || 
		   (UsbMessageBuffer[OFFSET_DWLENGTH+3] != 0) ||
			 (UsbMessageLength > USB_MESSAGE_BUFFER_MAX_LENGTH) )
		{	
			BulkStatus         =  RECEIVE_UNCORRECTLENGTH_INIT;
			pUsbMessageBuffer  = &UsbMessageBuffer[OFFSET_ABDATA];
		}
	}

	UsbMessageLength -= (int) cnt;											// bytes to get after this packet

/* Set transfer state  */
	if(UsbMessageLength > 0)
	{	
		BulkStatus = RECEIVE_OTHER_INIT;
	}

	if(UsbMessageLength == 0)
	{
		BulkStatus = RECEIVE_FINISHED;
	}

	if(UsbMessageLength < 0)
	{
		BulkStatus = RECEIVE_TOOLONGMESSAGE_FINISHED;
	}


	switch(BulkStatus)
	{	
		case RECEIVE_OTHER_INIT:
			SetEPRxStatus(ENDP2, EP_RX_VALID); 																	// enable the next transaction
			break;
	
		case RECEIVE_UNCORRECTLENGTH_INIT:
			BulkStatus = RECEIVE_UNCORRECTLENGTH_END;
			break;
	
		case RECEIVE_OTHER_END:
		case RECEIVE_UNCORRECTLENGTH_END:
			if(UsbDataLength == 0)
			{
				if (BulkStatus == RECEIVE_OTHER_END)
				{	
					BulkStatus = RECEIVE_FINISHED;	
				}
				else
				{	
					BulkStatus = RECEIVE_TOOLONGMESSAGE_FINISHED;	
				}
			}
			if(UsbDataLength != 0xFF)
			{
				UsbMessageLength -= (int) UsbDataLength;
				if( (UsbMessageLength > 0) && (BulkStatus == RECEIVE_OTHER_END) )
				{// Jeffery Lee 2002/12/05 <+> <1>
					pUsbMessageBuffer += UsbDataLength;
					BulkStatus = RECEIVE_OTHER_INIT;// Jeffery Lee 2002/12/05 <+> <1>
				}// Jeffery Lee 2002/12/05 <+> <1>
				if(UsbMessageLength == 0)
				{
					if(BulkStatus == RECEIVE_OTHER_END)
					{	
						BulkStatus = RECEIVE_FINISHED;	
					}
					else
					{	
						BulkStatus = RECEIVE_TOOLONGMESSAGE_FINISHED;	
					}
				}

				if(UsbMessageLength < 0)
				{
					BulkStatus = RECEIVE_TOOLONGMESSAGE_FINISHED;
				}
			}
			break;
	
		case RECEIVE_TOOLONGMESSAGE_FINISHED:
			UsbMessageBuffer[OFFSET_DWLENGTH]   = 0xFF;
			UsbMessageBuffer[OFFSET_DWLENGTH+1] = 0xFF;
			UsbMessageBuffer[OFFSET_DWLENGTH+2] = 0xFF;
			UsbMessageBuffer[OFFSET_DWLENGTH+3] = 0xFF;

		case RECEIVE_FINISHED:
			UsbDataLength = 0;
			Set_bBulkOutCompleteFlag;
			break;
	
		default:
			UsbDataLength = 0;
			Set_bBulkOutCompleteFlag;
			break;
	}
}

/************************************************************************/
/* ROUTINE 	void CCID_BulkInMessage(void)																*/
/*                                                                      */
/*  Transmit the CCID message by the Bulk In Endpoint 2.								*/
/*    this message was in UsbMessageBuffer.															*/
/************************************************************************/

unsigned char CCID_BulkInMessage (void)
{
	switch(BulkStatus)
	{
		case TRANSMIT_HEADER:
			UsbMessageLength = USB_MESSAGE_HEADER_SIZE + 
			                   MAKEWORD(UsbMessageBuffer[OFFSET_DWLENGTH+1],UsbMessageBuffer[OFFSET_DWLENGTH]);

			if (USB_MESSAGE_BUFFER_LENGTH	< UsbMessageLength)   // Buffer overflow ??
			{
				UsbMessageLength = USB_MESSAGE_BUFFER_LENGTH;
			}
	
			pUsbMessageBuffer = UsbMessageBuffer;
			// ">=" instead of ">" to implement the ZLP feature (Zero Lentgh Packet).
			if(UsbMessageLength >= USB_MAX_PACKET_SIZE)
			{

		    UserToPMABufferCopy((uint8_t *)pUsbMessageBuffer, CCID_ENDP2_TXADDR, USB_MAX_PACKET_SIZE);
		
				pUsbMessageBuffer += USB_MAX_PACKET_SIZE;
				UsbMessageLength  -= USB_MAX_PACKET_SIZE;
				BulkStatus         = TRANSMIT_OTHER;
		    SetEPTxCount  (ENDP2, USB_MAX_PACKET_SIZE);
		    SetEPTxStatus (ENDP2, EP_TX_VALID);

			}
			else
			{
		    UserToPMABufferCopy((uint8_t *)pUsbMessageBuffer, CCID_ENDP2_TXADDR, UsbMessageLength);
				BulkStatus         = TRANSMIT_FINISHED;
		    SetEPTxCount  (ENDP2, UsbMessageLength);
		    SetEPTxStatus (ENDP2, EP_TX_VALID);
			}
			break;
	
		case TRANSMIT_OTHER:
			// ">=" instead of ">" to implement the ZLP feature (Zero Lentgh Packet).
			if(UsbMessageLength >= USB_MAX_PACKET_SIZE)
			{
		    UserToPMABufferCopy((uint8_t *)pUsbMessageBuffer, CCID_ENDP2_TXADDR, USB_MAX_PACKET_SIZE);
		
				pUsbMessageBuffer += USB_MAX_PACKET_SIZE;
				UsbMessageLength  -= USB_MAX_PACKET_SIZE;

		    SetEPTxCount  (ENDP2, USB_MAX_PACKET_SIZE);
		    SetEPTxStatus (ENDP2, EP_TX_VALID);
			}
			else
			{
		    UserToPMABufferCopy((uint8_t *)pUsbMessageBuffer, CCID_ENDP2_TXADDR, UsbMessageLength);
		
				pUsbMessageBuffer += UsbMessageLength;
				BulkStatus         = TRANSMIT_FINISHED;

		    SetEPTxCount  (ENDP2, UsbMessageLength);
		    SetEPTxStatus (ENDP2, EP_TX_VALID);
			}
			break;
	
		case TRANSMIT_FINISHED:
			Set_bBulkInCompleteFlag;
			BulkStatus       = RECEIVE_FIRST_PART_INIT;
    	Bot_State        = BOT_DATA_IN_LAST;
      SetEPRxStatus(ENDP2, EP_RX_VALID);
			break;
	
		default:
			break;
	}

	return (BulkStatus);
}


/************************************************************************/
/* ROUTINE 	void CCID_DispatchMessage(void)															*/
/*                                                                      */
/*  Call the IFD function corresponding to the message received.				*/
/*    this message was in UsbMessageBuffer.															*/
/************************************************************************/

void CCID_DispatchMessage(void)
{
	unsigned char ErrorCode = SLOT_NO_ERROR;
 
	if(bBulkOutCompleteFlag)
	{
		switch(UsbMessageBuffer[OFFSET_BMESSAGETYPE])
		{

			case PC_TO_RDR_ICCPOWERON:
			  ErrorCode	= PC_to_RDR_IccPowerOn();
				if (SLOT_NO_ERROR == ErrorCode)
				{
					ErrorCode = IFD_SetATRData ();									// Create ATR output Message
				}
				RDR_to_PC_DataBlock(ErrorCode);
				break;
			case PC_TO_RDR_ICCPOWEROFF:
				ErrorCode = PC_to_RDR_IccPowerOff();
//				RDR_to_PC_SlotStatus(ErrorCode);
				RDR_to_PC_SlotStatus_CardStopped (ErrorCode);		 	// simulate power off
				break;
			case PC_TO_RDR_GETSLOTSTATUS:
				ErrorCode = PC_to_RDR_GetSlotStatus();
				RDR_to_PC_SlotStatus(ErrorCode);
				break;
			case PC_TO_RDR_XFRBLOCK:
				ErrorCode = PC_to_RDR_XfrBlock();
				RDR_to_PC_DataBlock(ErrorCode);
				break;
			case PC_TO_RDR_GETPARAMETERS:
				ErrorCode = PC_to_RDR_GetParameters();
				RDR_to_PC_Parameters(ErrorCode);
				break;
			case PC_TO_RDR_RESETPARAMETERS:
				ErrorCode = PC_to_RDR_ResetParameters();
				RDR_to_PC_Parameters(ErrorCode);
				break;
			case PC_TO_RDR_SETPARAMETERS:
				ErrorCode = PC_to_RDR_SetParameters();
				RDR_to_PC_Parameters(ErrorCode);
				break;
			case PC_TO_RDR_ESCAPE:
				ErrorCode = PC_to_RDR_Escape();
				RDR_to_PC_Escape(ErrorCode);
				break;
			case PC_TO_RDR_ICCCLOCK:
				ErrorCode = PC_to_RDR_IccClock();
				RDR_to_PC_SlotStatus(ErrorCode);
				break;
			case PC_TO_RDR_ABORT:
				ErrorCode = PC_to_RDR_Abort();
				RDR_to_PC_SlotStatus(ErrorCode);
				break;
			case PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY:
			case PC_TO_RDR_SECURE:
			case PC_TO_RDR_T0APDU:
			case PC_TO_RDR_MECHANICAL:
			default:
				CmdNotSupported();
				break;
		}
	
		BulkStatus = TRANSMIT_HEADER;
		Reset_bBulkOutCompleteFlag;
	}
}


/************************************************************************/
/* ROUTINE 	void CCID_IntMessage(void)																	*/
/*                                                                      */
/*  Transmit the CCID message by the Interrupt Endpoint 1.							*/
/*    this message was in UsbIntMessageBuffer.													*/
/************************************************************************/

unsigned char	SlotStabilizationDelay = 0; // RB INITIALSLOTSTABILIZATIONDELAY;

void CCID_IntMessage(void)
{
	unsigned char MessageSize;
	
	if( bSlotChangedFlag )
	{
		RDR_to_PC_NotifySlotChange();
		MessageSize = 0x02;

		if( SlotStabilizationDelay == 0x00 )
		{
			if( (UsbIntMessageBuffer[OFFSET_INT_BMSLOTICCSTATE] == 0x02) && bPreviousSlotStateFlag )
			{	
				UserToPMABufferCopy((uint8_t *)UsbIntMessageBuffer, CCID_ENDP1_TXADDR, MessageSize);
				SetEPTxCount  (ENDP1, MessageSize);
				SetEPTxStatus (ENDP1, EP_TX_VALID);
// needed ??				if( USB_SendDataEP1(UsbIntMessageBuffer, MessageSize) == REQ_SUCCESS )
				{	
					Reset_bSlotChangedFlag;
					Reset_bPreviousSlotStateFlag;	
				}
			}
			else
			{
				if( (UsbIntMessageBuffer[OFFSET_INT_BMSLOTICCSTATE] == 0x03) && (!bPreviousSlotStateFlag) ) 
					{	
						UserToPMABufferCopy((uint8_t *)UsbIntMessageBuffer, CCID_ENDP1_TXADDR, MessageSize);
						SetEPTxCount  (ENDP1, MessageSize);
						SetEPTxStatus (ENDP1, EP_TX_VALID);
						{	
							Reset_bSlotChangedFlag;
							Set_bPreviousSlotStateFlag;	
						}
					}
				else
				{	
					Reset_bSlotChangedFlag;	
				}
			}
		}
		else
		{
			RDR_to_PC_NotifySlotChange();
			MessageSize = 0x02;
			UserToPMABufferCopy((uint8_t *)UsbIntMessageBuffer, CCID_ENDP1_TXADDR, MessageSize);
			SetEPTxCount  (ENDP1, MessageSize);
			SetEPTxStatus (ENDP1, EP_TX_VALID);
		  Reset_bSlotChangedFlag;
		}
	}
	
	if( bHardwareErrorFlag )
	{
		RDR_to_PC_HardwareError();
		MessageSize = 0x04;
		UserToPMABufferCopy((uint8_t *)UsbIntMessageBuffer, CCID_ENDP1_TXADDR, MessageSize);
		SetEPTxCount  (ENDP1, MessageSize);
		SetEPTxStatus (ENDP1, EP_TX_VALID);
		{	
			Reset_bHardwareErrorFlag;	
		}
	}
}


/************************************************************************/
/* ROUTINE 	void CcidClassRequestAbort(void)														*/
/*                                                                      */
/*  Manage the ABORT REQUEST.																						*/
/************************************************************************/

void CcidClassRequestAbort(void)
{
/*
	if( (sUSB_vSetup.USBbmRequestType == 0x21) &&
			(sUSB_vSetup.USBwValue0 == 0x00) && (sUSB_vSetup.USBwIndex1 == 0x00) &&
			(sUSB_vSetup.USBwIndex0 == 0x00) && (sUSB_vSetup.USBwLength1 == 0x00) &&
			(sUSB_vSetup.USBwLength0 == 0x00) )
	{
		Set_bAbortRequestFlag;
		AbortSequenceNumber = sUSB_vSetup.USBwValue1;
	}
*/
}

/************************************************************************/
/* ROUTINE 	void CcidClassRequestGetClockFrequencies(void)							*/
/*                                                                      */
/*  Manage the ABORT REQUEST.																						*/
/************************************************************************/

void CcidClassRequestGetClockFrequencies(void)
{
}

/************************************************************************/
/* ROUTINE 	void CcidClassRequestGetDataRates(void)											*/
/*                                                                      */
/*  Manage the ABORT REQUEST.																						*/
/************************************************************************/

void CcidClassRequestGetDataRates(void)
{
}

/************************************************************************

	CCID_SendCardDetect

************************************************************************/

void CCID_SendCardDetect(void)
{
	Set_bSlotChangedFlag;
	Reset_bPreviousSlotStateFlag;

	CCID_IntMessage ();
}

/************************************************************************

	CCID_CheckUsbCommunication

************************************************************************/

void CCID_CheckUsbCommunication(void)
{
  CCID_DispatchMessage ();

  if ((TRANSMIT_HEADER    == BulkStatus)	||
			(TRANSMIT_FINISHED  == BulkStatus))
  {
	  CCID_BulkInMessage();									 // something to send ?
	}
}

/************************************************************************

	CCID_SetCardState

************************************************************************/

void CCID_SetCardState (unsigned char nState)
{
	cCRD_CardPresent = nState;	
}

/************************************************************************

	CCID_GetCardState

************************************************************************/

unsigned char CCID_GetCardState (void)
{
	return (cCRD_CardPresent);	
}



