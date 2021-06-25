/*
 * Author: Copyright (C) Rudolf Boeddeker                                           Date: 2010-01-13
 *                                              STMicroelectronics / ST Rousset             Date:   22-V-2001
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


#include "type.h"
#include "CCID_SlotErrorCode.h"
#include "CCID_Crd.h"
#include "CCID_Ifd_ccid.h"


unsigned char* pCrdBuffer;

volatile unsigned char CrdFlags;

unsigned int CrdNbData;

unsigned int CrdBufferSize;

unsigned char InstructionByte, ProcedureByte;


/****************************************************************/
/****************************************************************/
/* HARDWARE FUNCTIONS */
/****************************************************************/
/****************************************************************/


/****************************************************************/
/* ROUTINE void CRD_Init(void) */
/****************************************************************/
void CRD_Init (void)
{
    CrdFlags = 0x00;
}

/****************************************************************/
/* ROUTINE unsigned char CRD_GetHwError() */
/* Fill *pHwErrorCode with OverCurrent(flag and int) and */
/* Voltage not OK (flag and int).  */
/* return SLOT_NO_ERROR if OK.  */
/****************************************************************/
unsigned char CRD_GetHwError (unsigned char* pHwErrorCode)
{
    unsigned char ErrorCode;

    ErrorCode = SLOT_NO_ERROR;

    *pHwErrorCode = 0x00;

    if (bCurrentErrorFlag)
        *pHwErrorCode |= HARDWAREERRORCODE_OVERCURRENT_IT;
    if (bVoltageErrorFlag)
        *pHwErrorCode |= HARDWAREERRORCODE_VOLTAGEERROR_IT;

    if (*pHwErrorCode != 0x00)
        ErrorCode = SLOTERROR_HW_ERROR;

    if (ErrorCode != SLOT_NO_ERROR)
    {
        CRD_VccOff ();
    };  /* Switch off power card */

    return ErrorCode;
}

/****************************************************************/
/* ROUTINE unsigned char CRD_VccOn(unsigned char Voltage) */
/* unsigned char Voltage can be : */
/* CRD_VOLTAGE18V (for 1.8 volts) */
/* CRD_VOLTAGE3V (for 3.0 volts) */
/* CRD_VOLTAGE5V (for 5.0 volts charge pump on) */
/* return SLOT_NO_ERROR if OK.  */
/****************************************************************/
unsigned char CRD_VccOn (unsigned char Voltage)
{
    unsigned char HwErrorCode;

    // Not used

    switch (Voltage)
    {
        case CRD_VOLTAGE18V:
            break;
        case CRD_VOLTAGE3V:
            break;
        case CRD_VOLTAGE5V:
            break;
        default:
            break;
    }

    return (CRD_GetHwError (&HwErrorCode));
}


/****************************************************************/
/* ROUTINE void CRD_VccOff(void) */
/* Switch off the Vcc Card and all other smart card pins */
/****************************************************************/
void CRD_VccOff (void)
{
    // Not used
}

/****************************************************************/
/* ROUTINE void CRD_VccOff_IT(void) */
/* Switch off the Vcc Card and all other smart card pins */
/* Routine to be used in Interrupt process only to avoid */
/* reentrance */
/****************************************************************/
void CRD_VccOff_IT (void)
{
    // Not used
}

/****************************************************************************/
/* ROUTINE void CRD_SetMode() */
/* unsigned char Mode can be : */
/* CRD_MANUALMODE (for Manual) */
/* CRD_UARTMODE (for UART) */
/****************************************************************************/
void CRD_SetMode (unsigned char Mode)
{
    switch (Mode)
    {
        case CRD_MANUALMODE:
            break;
        case CRD_UARTMODE:
            /* Set CRDIO pin to be able to go in UART mode */
            break;
        default:
            break;
    }
}

/****************************************************************************/
/* ROUTINE void CRD_SetConvention() */
/* unsigned char Convention can be : */
/* CRD_DIRECTCONV */
/* CRD_INVERSECONV */
/****************************************************************************/
void CRD_SetConvention (unsigned char Convention)
{
    switch (Convention)
    {
        case CRD_DIRECTCONV:
            break;
        case CRD_INVERSECONV:
            break;
        default:
            break;
    }
}

/****************************************************************/
/* ROUTINE unsigned char CRD_GetSlotStatus(void) */
/* return the SlotStatus described in CCID Spec */
/****************************************************************/
unsigned char CRD_GetSlotStatus (void)
{
    // Always present
    return (CRD_PRESENTACTIVE);
}

/****************************************************************/
/* ROUTINE unsigned char CRD_GetConvention(void) */
/* return the Convention currently in use.  */
/****************************************************************/
unsigned char CRD_GetConvention (void)
{
    return (CRD_DIRECTCONV);    // always
}

/****************************************************************/
/* ROUTINE unsigned char CRD_GetClockStatus(void) */
/* return the Clock State.  */
/****************************************************************/
unsigned char CRD_GetClockStatus (void)
{
    return (CRD_CLOCKRUNNING);
}

#define UNUSED(x) (void)(x)
/****************************************************************/
/* ROUTINE unsigned char CRD_SetClock(void) */
/* return Error Code : */
/* SLOT_NO_ERROR if OK.  */
/****************************************************************/
unsigned char CRD_SetClock (unsigned char ClockState)
{
    UNUSED(ClockState);
    if (CRD_GetSlotStatus () != CRD_PRESENTACTIVE)
    {
        return SLOTERROR_ICC_MUTE;
    }

    return SLOT_NO_ERROR;   // Clock is running
}

/****************************************************************/
/* ROUTINE void CRD_SetEtu(unsigned int Etu) */
/* unsigned int Etu is a positive integer : */
/* from 12 to 2048 */
/****************************************************************/
unsigned char CRD_SetEtu (unsigned int Etu, unsigned char Comp)
{
    UNUSED(Comp);

    if ((Etu < 12) || (Etu > 2048))
        return 0xFF;

    if (Etu == 2048)
    {
        // CRDETU0 = 0x00;
        // CRDETU1 = 0x00;
    }
    else
    {
        // CRDETU0 = (unsigned char) (Etu);
        // CRDETU1 = (unsigned char) (Etu>>8);
    }

    // if(Comp) CRDETU1 |= COMP;

    return 0x00;
}

/****************************************************************/
/* ROUTINE void CRD_SetGuardTime(unsigned int GuardTime) */
/* unsigned int GuardTime is a positive integer : */
/* from 11 to 511 */
/****************************************************************/
void CRD_SetGuardTime (unsigned int GuardTime)
{
    UNUSED(GuardTime);
}

/****************************************************************/
/* ROUTINE void CRD_SetWaitingTime(unsigned long WaitingTime) */
/* unsigned long WaitingTime is a positive long integer : */
/* from 0 to 16777215 */
/****************************************************************/
void CRD_SetWaitingTime (unsigned long WaitingTime)
{
    UNUSED(WaitingTime);
}

/****************************************************************/
/* ROUTINE void CRD_StartWaitingTime(void) */
/****************************************************************/
void CRD_StartWaitingTime (void)
{
    Reset_bWaitingTimeFlag;
}

/****************************************************************/
/* ROUTINE void CRD_StopWaitingTime(void) */
/****************************************************************/
void CRD_StopWaitingTime (void)
{
}

/****************************************************************/
/* ROUTINE void CRD_WaitingTime(unsigned long WaitingTimeInEtu) */
/****************************************************************/
void CRD_WaitingTime (unsigned long WaitingTimeInEtu)
{
    CRD_SetWaitingTime (WaitingTimeInEtu);
    CRD_StartWaitingTime ();
    while (!bWaitingTimeFlag);
    CRD_StopWaitingTime ();
}


/****************************************************************/
/* ROUTINE void CRD_InitReceive(unsigned char * pBuffer) */
/* Init global pCrdBuffer with pBuffer */
/* Valid Receive interrupt */
/****************************************************************/

void CRD_InitReceive (unsigned int ReceiveBufSize, unsigned char* pBuffer)
{
    pCrdBuffer = pBuffer;
    CrdBufferSize = ReceiveBufSize;
    CrdNbData = 0;
    Reset_bBufferOverflowFlag;
}


/****************************************************************/
/* ROUTINE void CRD_CharRepeatOn(void) */
/* Init global pCrdBuffer with pBuffer */
/* Valid Receive interrupt */
/****************************************************************/
void CRD_CharRepeatOn (void)
{
}

/****************************************************************/
/* ROUTINE void CRD_CharRepeatOff(void) */
/* Init global pCrdBuffer with pBuffer */
/* Valid Receive interrupt */
/****************************************************************/
void CRD_CharRepeatOff (void)
{
    Reset_bParityErrorFlag;
}


/****************************************************************/
/* ROUTINE unsigned int CRD_EndReceive(void) */
/* Unvalidate Receive interrupt */
/* Return the number of bytes received */
/****************************************************************/
unsigned int CRD_EndReceive (void)
{
    return CrdNbData;
}

/****************************************************************/
/* ROUTINE void CRD_InitTransmit() */
/* Init global pCrdBuffer with pBuffer */
/* Valid Transmit interrupt */
/****************************************************************/
void CRD_InitTransmit (unsigned int TransmitBufSize, unsigned char* pBuffer)
{
    pCrdBuffer = pBuffer;
    CrdNbData = TransmitBufSize;
    Reset_bBufferOverflowFlag;
    pCrdBuffer++;
}

/****************************************************************/
/* ROUTINE unsigned int CRD_NumberOfBytesToTransmit(void) */
/* Return number of bytes to transmit before buffer empty.  */
/****************************************************************/
unsigned int CRD_NumberOfBytesToTransmit (void)
{
    return CrdNbData;
}

/****************************************************************/
/* ROUTINE unsigned int CRD_EndTransmit(void) */
/* Unvalid Transmit interrupt */
/****************************************************************/
unsigned int CRD_EndTransmit (void)
{
    return CrdNbData;
}



/****************************************************************/
/****************************************************************/
/* ICC LEVEL FUNCTIONS */
/****************************************************************/
/****************************************************************/

/************************************************************************/
/* ROUTINE void ICC_ResetAsync(void) */
/************************************************************************/
void ICC_ResetAsync (void)
{
    InstructionByte = DEFAULT_INSTRUCTION_BYTE;
    ProcedureByte = DEFAULT_PROCEDURE_BYTE;

    CRD_SetMode (CRD_MANUALMODE);

    CRD_WaitingTime (POWERUP_WAITINGTIME);
}

/****************************************************************/
/* ROUTINE unsigned char ICC_SendCommandAsync() */
/* Send a command to the ICC for Asynchronous ICC only.  */
/* Return error code : */
/****************************************************************/
unsigned char ICC_SendCommandAsync (unsigned char* pTransmitBuffer, unsigned int CommandSize, unsigned char ProtocolType)
{
    if (ProtocolType == 0x00)
    {
        Reset_bProtocolTypeFlag;
        if ((CommandSize == 0x05) && (ProcedureByte != InstructionByte) && (ProcedureByte != (~InstructionByte)))
        {
            InstructionByte = *(pTransmitBuffer + 1);
        }
    }
    else
    {
        Set_bProtocolTypeFlag;
    }


    CRD_InitTransmit (CommandSize, pTransmitBuffer);

    CRD_StartWaitingTime ();

    while ((!bParityErrorFlag) && (!bWaitingTimeFlag) && (!bBufferOverflowFlag))
    {
    }

    CRD_StopWaitingTime ();

    if (bParityErrorFlag)
        return SLOTERROR_XFR_PARITY_ERROR;

    if (CRD_EndTransmit () != 0)
        return SLOTERROR_ICC_MUTE;

    return SLOT_NO_ERROR;
}


/****************************************************************/
/* ROUTINE unsigned char ICC_ReceiveAnswerAsync() */
/* Read the answer to a command from the ICC.  */
/* *pAnswerSize : Input : Max answer size */
/* Output : Real answer size */
/* Return error code : */
/* 0x00 if OK */
/****************************************************************/
unsigned char ICC_ReceiveAnswerAsync (unsigned char* pReceiveBuffer, unsigned int* pAnswerSize)
{
    unsigned int BufferMaxSize;

    BufferMaxSize = *pAnswerSize;
    Set_bProcedureByteFlag;

    CRD_InitReceive (BufferMaxSize, pReceiveBuffer);

    CRD_StartWaitingTime ();

    while ((!bParityErrorFlag) && (!bWaitingTimeFlag) && (!bBufferOverflowFlag))
    {
    }

    *pAnswerSize = CRD_EndReceive ();
    CRD_StopWaitingTime ();

    if (bParityErrorFlag)
        return SLOTERROR_XFR_PARITY_ERROR;

    if (bBufferOverflowFlag)
        return SLOTERROR_XFR_OVERRUN;

    if (*pAnswerSize == 0)
        return SLOTERROR_ICC_MUTE;

    return SLOT_NO_ERROR;
}
