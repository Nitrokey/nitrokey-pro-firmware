/*
 * Author: Copyright (C) Rudolf Boeddeker                                           Date: 2010-01-13
 *                                              STMicroelectronics / ST Rousset             Date:   29-V-2001
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


#include "string.h"

#include "smartcard.h"

#include "CCID_Global.h"
#include "CCID_Macro.h"
#include "CCID_Ifd_ccid.h"
#include "CCID_SlotErrorCode.h"
#include "CCID_Ifd_protocol.h"
#include "CCID_Crd.h"

const unsigned int FvsFI[] =
    { 0, 372, 558, 744, 1116, 1488, 1860, 0, 0, 512, 768, 1024, 1536, 2048, 0,
0 };
const unsigned int Dmul64vsDI[] =
    { 0, 64, 128, 256, 512, 1024, 2048, 0, 768, 1280, 32, 16, 8, 4, 2, 1 };


static volatile unsigned char IccTransactionLevelType;

static unsigned char XfrFlag;

static unsigned int XfrNbBytesLoaded;

static unsigned char* pXfrAddressBytesLoaded;

static unsigned long XfrWaitingTime;

typedef struct
{
    unsigned char FiDi;
    unsigned char T01ConvChecksum;
    unsigned char GuardTime;
    unsigned char WaitingInteger;
    unsigned char ClockStop;
    unsigned char Ifsc;
    unsigned char Nad;
} Param;

static Param IccParameters;

/************************************************************************/
/* ROUTINE void Ifd_Init(void) */
/* */
/************************************************************************/

void IFD_Init (void)
{
    SetChar_bmTransactionLevel;
    SetT0_bTransactionType;
    XfrFlag = INS;

    IccParameters.FiDi = DEFAULT_FIDI;
    IccParameters.T01ConvChecksum = DEFAULT_T01CONVCHECKSUM;

    if (CRD_GetConvention () == CRD_DIRECTCONV)
    {
        IccParameters.T01ConvChecksum &= ~(0x02);
    }
    else
    {
        IccParameters.T01ConvChecksum |= 0x02;
    }

    IccParameters.GuardTime = DEFAULT_GUARDTIME;
    IccParameters.WaitingInteger = DEFAULT_WAITINGINTEGER;
    IccParameters.ClockStop = DEFAULT_CLOCKSTOP;
    IccParameters.Ifsc = DEFAULT_IFSC;
    IccParameters.Nad = DEFAULT_NAD;
}


/************************************************************************/
/* ROUTINE unsigned char IFD_ApplyParametersStructure() */
/* */
/* Load hardware UART registers with parameters values.  */
/* Return ErrorCode if bad parameter */
/* ETU = F / ( D * Freq ) ; WWT = 960 * WI * F / Freq */
/* So, WWT = 960 * WI * D * ETU */
/* SLOT_NO_ERROR if OK */
/************************************************************************/

unsigned char IFD_ApplyParametersStructure (void)
{
    unsigned long F, D, Etu;

    unsigned char Comp, ErrorCode;

    unsigned int GuardTime;

    F = FvsFI[IccParameters.FiDi >> 4];
    D = Dmul64vsDI[IccParameters.FiDi & 0x0F];

    Comp = 0;
    Etu = (2 * 64) * F;
    Etu = Etu / D;

    if (Etu & 1)
    {
        Comp = 1;
        Etu++;
    }

    Etu = Etu / 2;
    ErrorCode = CRD_SetEtu ((unsigned int) Etu, Comp);

    if (ErrorCode != 0x00)
        return SLOTERROR_BAD_FIDI;

    if (IccParameters.T01ConvChecksum & 0x10)
    {
        SetT1_bTransactionType;
    }
    else
    {
        SetT0_bTransactionType;
    }

    if (IccParameters.GuardTime != 0xFF)
    {
        GuardTime = 12 + IccParameters.GuardTime;
    }
    else
    {
        if (bTransactionType == T0_TYPE)
        {
            GuardTime = 12;
        }
        else
        {
            GuardTime = 11;
        }
    }

    CRD_SetGuardTime (GuardTime);

    XfrWaitingTime =
        (960 / 64) * D * ((unsigned long) IccParameters.WaitingInteger);

    CRD_SetWaitingTime (XfrWaitingTime);

    // use only local setup
    // SC_SetHwParams
    // (IccParameters.FiDi,IccPavChecksum,IccPavChecksum,IccParameters.GuardTime,IccParameters.WaitingInteger);

    return SLOT_NO_ERROR;
}

/************************************************************************/
/* ROUTINE unsigned char IFD_UpdateConvParameterStructure() */
/* */
/* Update Parameters Structure with Convention currently in use.  */
/************************************************************************/

void IFD_UpdateConvParameterStructure (void)
{
    if (CRD_GetConvention () == CRD_DIRECTCONV)
    {
        IccParameters.T01ConvChecksum &= ~(0x02);
    }
    else
    {
        IccParameters.T01ConvChecksum |= 0x02;
    }
}

/************************************************************************/
/* ROUTINE unsigned char IFD_XfrBlock() */
/* */
/* Input : buffer filled with command and nb of bytes in this command */
/* Output : buffer filled with answer and nb of bytes in this answer */
/* Return an Error code : */
/* 0x00 if OK */
/************************************************************************/

unsigned char IFD_XfrBlock (unsigned char* pBlockBuffer,
                            unsigned int* pBlockSize,
                            unsigned int ExpectedAnswerSize)
{
    unsigned char ErrorCode = SLOT_NO_ERROR;

    if (bmTransactionLevel != CHARACTER_LEVEL && ExpectedAnswerSize != 0x00)
    {
        return XFR_BADLEVELPARAMETER;
    }

    if ((bmTransactionLevel == CHARACTER_LEVEL)
        && (bTransactionType == T0_TYPE))
    {
        ErrorCode =
            IFD_XfrCharT0 (pBlockBuffer, pBlockSize, ExpectedAnswerSize);
    }

    if ((bmTransactionLevel == CHARACTER_LEVEL)
        && (bTransactionType == T1_TYPE))
    {
        ErrorCode =
            IFD_XfrCharT0 (pBlockBuffer, pBlockSize, ExpectedAnswerSize);
    }
    // why not this ??? T0 protocol ? >> ErrorCode =
    // IFD_XfrCharT1(pBlockBuffer, pBlockSize);

    if ((bmTransactionLevel == TPDU_LEVEL) && (bTransactionType == T0_TYPE))
        ErrorCode = IFD_XfrTpduT0 (pBlockBuffer, pBlockSize);

    if ((bmTransactionLevel == TPDU_LEVEL) && (bTransactionType == T1_TYPE))
        ErrorCode = IFD_XfrTpduT1 (pBlockBuffer, pBlockSize);

    if (ErrorCode != SLOT_NO_ERROR)
        return ErrorCode;

    return ErrorCode;
}

/*******************************************************************************

	GetExpectedAnswerSizeFromAPDU

*******************************************************************************/
#define TRAILER_SIZE 2

int GetExpectedAnswerSizeFromAPDU (unsigned char* pAPDU, int nSize)
{
    if (4 == nSize) // Only a command
    {
        return (TRAILER_SIZE);
    }

    if (5 == nSize) // A command with LE parameter
    {
        return (pAPDU[4] + TRAILER_SIZE);
    }

    if (nSize == 5 + pAPDU[4])  // A command with LC parameter and command
                                // data
    {
        return (TRAILER_SIZE);
    }

    if (nSize == 5 + pAPDU[4] + 1)  // A command with LC parameter, command
                                    // data and LE parameter
    {
        return (pAPDU[5 + pAPDU[4]] + TRAILER_SIZE);
    }

    return (0); // Error
}

/************************************************************************/
/* ROUTINE unsigned char IFD_XfrCharT0() */
/* */
/* Return an Error code : */
/* 0x00 if OK */
/************************************************************************/

unsigned char IFD_XfrCharT0 (unsigned char* pBlockBuffer,
                             unsigned int* pBlockSize,
                             unsigned int AnswerSize)
{
    unsigned int nReceivedAnserSize;

    unsigned int i;

    switch (XfrFlag)
    {
        case INS:
            if (*pBlockSize == 0)
            {
                return SLOTERROR_BAD_LENTGH;
            }

            AnswerSize =
                GetExpectedAnswerSizeFromAPDU (pBlockBuffer, *pBlockSize);
            CRD_SendCommand (pBlockBuffer, *pBlockSize, AnswerSize,
                             &nReceivedAnserSize);
            *pBlockSize = nReceivedAnserSize;
            break;

        case DATA:
            if (*pBlockSize != 0)
                return SLOTERROR_BAD_LENTGH;

            for (i = 0; i < AnswerSize; i++)
            {
                *(pBlockBuffer + i) = *pXfrAddressBytesLoaded++;
            }

            if (AnswerSize < XfrNbBytesLoaded)
            {
                XfrNbBytesLoaded -= AnswerSize;
                *pBlockSize = AnswerSize;
            }
            else
            {
                *pBlockSize = XfrNbBytesLoaded;
                XfrFlag = INS;
            }
            break;

    }

    return (SLOT_NO_ERROR);
}

/************************************************************************/
/* ROUTINE unsigned char IFD_XfrCharT1() */
/* */
/* Return an Error code : */
/* 0x00 if OK */
/************************************************************************/

unsigned char IFD_XfrCharT1 (unsigned char* pBlockBuffer,
                             unsigned int* pBlockSize)
{
    return 0xF6;
}

/************************************************************************/
/* ROUTINE unsigned char IFD_XfrTpduT0() */
/* */
/* Return an Error code : */
/* 0x00 if OK */
/************************************************************************/

unsigned char IFD_XfrTpduT0 (unsigned char* pBlockBuffer,
                             unsigned int* pBlockSize)
{
    return 0xF6;
}

/************************************************************************/
/* ROUTINE unsigned char IFD_XfrTpduT1() */
/* */
/* Return an Error code : */
/* 0x00 if OK */
/************************************************************************/

unsigned char IFD_XfrTpduT1 (unsigned char* pBlockBuffer,
                             unsigned int* pBlockSize)
{
    return 0xF6;
}


/************************************************************************/
/* ROUTINE unsigned char IFD_GetParameters() */
/* */
/* Input : Address of the buffer to fill */
/* Output : buffer filled with CCID Parameter structure */
/* Return Protocol Type : */
/* 0x00 (for T=0) */
/* 0x01 (for T=1) */
/************************************************************************/

unsigned char IFD_GetParameters (unsigned char* pParamBuffer)
{
    *pParamBuffer = IccParameters.FiDi;
    *(pParamBuffer + 1) = IccParameters.T01ConvChecksum;
    *(pParamBuffer + 2) = IccParameters.GuardTime;
    *(pParamBuffer + 3) = IccParameters.WaitingInteger;
    *(pParamBuffer + 4) = IccParameters.ClockStop;
    *(pParamBuffer + 5) = IccParameters.Ifsc;
    *(pParamBuffer + 6) = IccParameters.Nad;

    if (IccParameters.T01ConvChecksum & 0x10)
        return 0x01;

    // return 0x00;
    return 0x01;
}

/************************************************************************/
/* ROUTINE unsigned char IFD_SetParameters() */
/* */
/* Input : Buffer filled with Parameters to set (CCID structure) */
/* Return Error Code : */
/* 0x00 if OK */
/************************************************************************/

unsigned char IFD_SetParameters (unsigned char* pParamBuffer,
                                 unsigned char T01)
{
    unsigned char ErrorCode;

    Param NewIccParameters;

    NewIccParameters.FiDi = *pParamBuffer;
    NewIccParameters.T01ConvChecksum = *(pParamBuffer + 1);
    NewIccParameters.GuardTime = *(pParamBuffer + 2);
    NewIccParameters.WaitingInteger = *(pParamBuffer + 3);
    NewIccParameters.ClockStop = *(pParamBuffer + 4);

    if (T01 == 0x01)
    {
        NewIccParameters.Ifsc = *(pParamBuffer + 5);
        NewIccParameters.Nad = *(pParamBuffer + 6);
    }
    else
    {
        NewIccParameters.Ifsc = 0x00;
        NewIccParameters.Nad = 0x00;
    }

    if ((Dmul64vsDI[(NewIccParameters.FiDi & 0x0F)] == 0) ||
        (Dmul64vsDI[(NewIccParameters.FiDi >> 0x04)] == 0))
    {
        return SLOTERROR_BAD_FIDI;
    }

    if ((T01 == 0x00) &&
        (NewIccParameters.T01ConvChecksum != 0x00) &&
        (NewIccParameters.T01ConvChecksum != 0x02))
    {
        return SLOTERROR_BAD_T01CONVCHECKSUM;
    }

    if ((T01 == 0x01)
        && (NewIccParameters.T01ConvChecksum != 0x10)
        && (NewIccParameters.T01ConvChecksum != 0x11)
        && (NewIccParameters.T01ConvChecksum != 0x12)
        && (NewIccParameters.T01ConvChecksum != 0x13))
        return SLOTERROR_BAD_T01CONVCHECKSUM;

    if ((NewIccParameters.WaitingInteger >= 0xA0)
        && ((NewIccParameters.T01ConvChecksum & 0x10) == 0x10))
        return SLOTERROR_BAD_WAITINGINTEGER;

    if ((NewIccParameters.ClockStop != 0x00) &&
        (NewIccParameters.ClockStop != 0x03))
    {
        return SLOTERROR_BAD_CLOCKSTOP;
    }

    if (NewIccParameters.Nad != 0x00)
    {
        return SLOTERROR_BAD_NAD;
    }

    IccParameters.FiDi = NewIccParameters.FiDi;
    IccParameters.T01ConvChecksum = NewIccParameters.T01ConvChecksum;

    if (CRD_GetConvention () == CRD_DIRECTCONV)
    {
        IccParameters.T01ConvChecksum &= ~(0x02);
    }
    else
    {
        IccParameters.T01ConvChecksum |= 0x02;
    }

    IccParameters.GuardTime = NewIccParameters.GuardTime;
    IccParameters.WaitingInteger = NewIccParameters.WaitingInteger;
    IccParameters.ClockStop = NewIccParameters.ClockStop;
    IccParameters.Ifsc = NewIccParameters.Ifsc;
    IccParameters.Nad = NewIccParameters.Nad;

    ErrorCode = IFD_ApplyParametersStructure ();

    return ErrorCode;
}


/************************************************************************/
/* ROUTINE unsigned char IFD_Escape() */
/* */
/* Return an Error code : */
/* 0x00 if OK */
/************************************************************************/

#define CCID_HW_NAME  "GemTwin-V2.00-GT00"

void IFD_EscapeSendHwName (unsigned char* pBlockBuffer,
                           unsigned int* pBlockSize)
{
    strcpy ((char *) pBlockBuffer, CCID_HW_NAME);
    *pBlockSize = strlen (CCID_HW_NAME);
}

#define IFD_ESCAPE_SEND_HW_NAME 2

unsigned char IFD_Escape (unsigned char* pBlockBuffer,
                          unsigned int* pBlockSize)
{
    unsigned int nSize;

    nSize = *pBlockSize;

    *pBlockSize = 0;    // No data bytes to send

    if (1 == nSize)
    {
        if (IFD_ESCAPE_SEND_HW_NAME == pBlockBuffer[0])
        {
            IFD_EscapeSendHwName (pBlockBuffer, pBlockSize);
        }
    }

    return SLOT_NO_ERROR;
}


/************************************************************************/
/* ROUTINE unsigned char IFD_SetClock() */
/* */
/* Return an Error code : */
/* 0x00 if OK */
/************************************************************************/

unsigned char IFD_SetClock (unsigned char ClockCmd)
{
    unsigned char ErrorCode;

    ErrorCode = SLOT_NO_ERROR;

    if (IccParameters.ClockStop == PARAM_CLOCK_NOTSTOPPED)
    {
        return (SLOTERROR_CMD_NOT_SUPPORTED);
    }

    if (ClockCmd == 0x00)
    {
        ErrorCode = CRD_SetClock (ClockCmd);
    }
    else
    {
        ErrorCode = CRD_SetClock (IccParameters.ClockStop);
    }

    return ErrorCode;
}

/******************************************************************************

  IFD_SetATRData

******************************************************************************/

unsigned char IFD_SetATRData (void)
{
    int i;

    if (FALSE == SC_A2R.cATR_Valid)
    {
        return (SLOTERROR_BAD_POWERSELECT);
    }

    UsbMessageBuffer[OFFSET_DWLENGTH] = SC_A2R.Tlength + SC_A2R.Hlength + 2;
    UsbMessageBuffer[OFFSET_DWLENGTH + 1] = 0x00;
    UsbMessageBuffer[OFFSET_DWLENGTH + 2] = 0x00;
    UsbMessageBuffer[OFFSET_DWLENGTH + 3] = 0x00;

    UsbMessageBuffer[OFFSET_ABDATA] = SC_A2R.TS;
    UsbMessageBuffer[OFFSET_ABDATA + 1] = SC_A2R.T0;

    for (i = 0; i < SC_A2R.Tlength; i++)
    {
        UsbMessageBuffer[OFFSET_ABDATA + 2 + i] = SC_A2R.T[i];
    }

    for (i = 0; i < SC_A2R.Hlength; i++)
    {
        UsbMessageBuffer[OFFSET_ABDATA + 2 + SC_A2R.Tlength + i] =
            SC_A2R.H[i];
    }

    return (SLOT_NO_ERROR);
}
