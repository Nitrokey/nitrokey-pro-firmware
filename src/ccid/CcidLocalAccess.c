/*
 * Author: Copyright (C) Rudolf Boeddeker               Date: 2010-02-01
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

/*
   to do : CRC check of receive TPDU data */


#include <FlashStorage.h>
#include <sys/param.h>
#include <delays.h>
#include "stm32f10x.h"
#include "platform_config.h"
#include "hw_config.h"
#include "CCID_usb.h"
#include "string.h"
#include "smartcard.h"
#include "CCID_Ifd_protocol.h"
#include "CcidLocalAccess.h"
#include "hotp.h"
#include "parse_tlv.h"

#include "time.h"

#define CCID_TRANSFER_BUFFER_MAX    512

#define CCID_TPDU_OVERHEAD          4
#define CCID_TPDU_PROLOG            3
#define CCID_TPDU_ANSWER_OVERHEAD   6


#define CCID_TPDU_NAD           0
#define CCID_TPDU_PCD           1
#define CCID_TPDU_LENGTH        2
#define CCID_TPDU_DATASTART     3

#define CCID_TPDU_R_BLOCK_FLAG          0x80
#define CCID_TPDU_R_BLOCK_SEQUENCE_FLAG 0x10
#define CCID_TPDU_CHAINING_FLAG         0x20


#define CCID_CLA  0
#define CCID_INS  1
#define CCID_P1   2
#define CCID_P2   3
#define CCID_LC   4
#define CCID_DATA 5

/*******************************************************************************

  Definition of smartcard transfer struct

*******************************************************************************/


typedef struct
{
    unsigned char cAPDULength;
    unsigned short cAPDUAnswerStatus;
    unsigned short cAPDUAnswerLength;
    unsigned char cTPDUSequence;
    unsigned char cTPDULength;
    unsigned char cAPDU[CCID_TRANSFER_BUFFER_MAX];
    unsigned char cTPDU[CCID_TRANSFER_BUFFER_MAX + CCID_TPDU_OVERHEAD];
} typeSmartcardTransfer;

static typeSmartcardTransfer tSCT;

/*******************************************************************************

  InitSCTStruct

*******************************************************************************/


void ClearSCTStruct (typeSmartcardTransfer * _tSCT)
{
    memset(_tSCT->cAPDU, 0, sizeof _tSCT->cAPDU);
    memset(_tSCT->cTPDU, 0, sizeof _tSCT->cTPDU);
    _tSCT->cAPDULength = 0;
    _tSCT->cAPDUAnswerLength = 0;
    _tSCT->cAPDUAnswerStatus = 0;
}

void InitSCTStruct (typeSmartcardTransfer * _tSCT)
{
    _tSCT->cTPDUSequence = 0;
    ClearSCTStruct(_tSCT);
}


/*******************************************************************************

  GenerateCRC

*******************************************************************************/

unsigned char GenerateCRC (unsigned char* pData, unsigned char cLength)
{
    unsigned char cCRC = 0;

    int i;

    for (i = 0; i < cLength; i++)
    {
        cCRC ^= pData[i];
    }

    return (cCRC);
}

/*******************************************************************************

  GenerateTPDU

*******************************************************************************/

void GenerateTPDU (typeSmartcardTransfer * _tSCT)
{
    _tSCT->cTPDU[CCID_TPDU_NAD] = 0; // Node Address (NAD)
    _tSCT->cTPDU[CCID_TPDU_PCD] = (_tSCT->cTPDUSequence & 1) << 6;    // Protocol Control Byte
    _tSCT->cTPDU[CCID_TPDU_LENGTH] = _tSCT->cAPDULength;

    _tSCT->cTPDULength = _tSCT->cAPDULength + CCID_TPDU_OVERHEAD; // the length of the TPDU
    _tSCT->cTPDUSequence++;  // switch sequence

    memcpy (&_tSCT->cTPDU[CCID_TPDU_DATASTART], _tSCT->cAPDU, _tSCT->cAPDULength); // copy APDU data

    _tSCT->cTPDU[_tSCT->cAPDULength + CCID_TPDU_OVERHEAD - 1] =   // set CRC at end of data
        GenerateCRC ((unsigned char *) &_tSCT->cTPDU, _tSCT->cAPDULength + CCID_TPDU_PROLOG);
}

/*******************************************************************************

  GenerateChainedTPDU

*******************************************************************************/

void GenerateChainedTPDU (typeSmartcardTransfer * _tSCT)
{
    _tSCT->cTPDU[CCID_TPDU_NAD] = 0; // Node Address (NAD)
    _tSCT->cTPDU[CCID_TPDU_PCD] = ((_tSCT->cTPDUSequence & 1) << 4) + CCID_TPDU_R_BLOCK_FLAG; // Protocol Control Byte
    _tSCT->cTPDU[CCID_TPDU_LENGTH] = 0;

    _tSCT->cTPDULength = CCID_TPDU_OVERHEAD; // the length of the TPDU
    _tSCT->cTPDUSequence++;  // switch sequence

    _tSCT->cTPDU[CCID_TPDU_OVERHEAD - 1] = GenerateCRC ((unsigned char *) &_tSCT->cTPDU, CCID_TPDU_PROLOG);
}



/*******************************************************************************

  SendTPDU

*******************************************************************************/

unsigned short SendTPDU (typeSmartcardTransfer * _tSCT)
{
unsigned int nAnswerLength = 0;

unsigned char nOverhead = 0;

    // Send TPDU to smartcard an receive answer
    CRD_SendCommand ((unsigned char *) _tSCT->cTPDU, _tSCT->cTPDULength, CCID_TRANSFER_BUFFER_MAX, (unsigned int *) &nAnswerLength);


    if (CCID_TPDU_ANSWER_OVERHEAD > nAnswerLength)  // answer length incorrect
    {
        _tSCT->cAPDUAnswerStatus = APDU_ANSWER_RECEIVE_INCORRECT;
        return (_tSCT->cAPDUAnswerStatus);
    }

    if (0 != (_tSCT->cTPDU[CCID_TPDU_PCD] & CCID_TPDU_CHAINING_FLAG))    // chained data
    {
        nOverhead = CCID_TPDU_ANSWER_OVERHEAD - 2;  // no status data
        _tSCT->cAPDUAnswerStatus = APDU_ANSWER_CHAINED_DATA;
    }
    else
    {
        nOverhead = CCID_TPDU_ANSWER_OVERHEAD;
        _tSCT->cAPDUAnswerStatus = _tSCT->cTPDU[nAnswerLength - 3] << 8;  // Statusbyte SW1
        _tSCT->cAPDUAnswerStatus += _tSCT->cTPDU[nAnswerLength - 2];  // Statusbyte SW2
    }

    // add new data to receive data
    memcpy (&_tSCT->cAPDU[_tSCT->cAPDUAnswerLength],
            &_tSCT->cTPDU[CCID_TPDU_DATASTART],
            nAnswerLength - nOverhead);

    _tSCT->cAPDUAnswerLength += nAnswerLength - nOverhead;   // add length of recieved data

    return (_tSCT->cAPDUAnswerStatus);
}


/*******************************************************************************

  SendAPDU

*******************************************************************************/

unsigned short SendAPDU (typeSmartcardTransfer * _tSCT)
{
    _tSCT->cAPDUAnswerLength = 0;
    GenerateTPDU (_tSCT);
    SendTPDU (_tSCT);

    if (APDU_ANSWER_RECEIVE_INCORRECT == _tSCT->cAPDUAnswerStatus
        && APDU_EOF != tSCT->cAPDUAnswerStatus)
    {
        // return on error
        return (_tSCT->cAPDUAnswerStatus); // FAIL
    }

    // Chained answer check
    while (0 != (_tSCT->cTPDU[CCID_TPDU_PCD] & CCID_TPDU_CHAINING_FLAG))
    {
        GenerateChainedTPDU (_tSCT);
        SendTPDU (_tSCT);

        if ((APDU_ANSWER_CHAINED_DATA != _tSCT->cAPDUAnswerStatus) &&
            (APDU_ANSWER_COMMAND_CORRECT != _tSCT->cAPDUAnswerStatus))
        {
            // return on error
            return (_tSCT->cAPDUAnswerStatus); // FAIL CHAINED
        }
    }

    return (_tSCT->cAPDUAnswerStatus); // SUCCESS
}

/*******************************************************************************

  CcidSelectOpenPGPApp

*******************************************************************************/
#define CCID_SIZE_OPEN_PGP_APP	11

unsigned short CcidSelectOpenPGPApp (void)
{
    unsigned short cRet;
    unsigned char cOpenPGPApp[CCID_SIZE_OPEN_PGP_APP] = { 0x00, 0xA4, 0x04, 0x00, 0x06, 0xD2, 0x76, 0x00, 0x01, 0x24, 0x01 };

    tSCT.cAPDULength = CCID_SIZE_OPEN_PGP_APP;

    memcpy ((void *) &tSCT.cAPDU, cOpenPGPApp, tSCT.cAPDULength);

    cRet = SendAPDU (&tSCT);

    return (cRet);
}

/*******************************************************************************

  CcidGetData

*******************************************************************************/
#define CCID_SIZE_GETDATA	5

unsigned short CcidGetData (unsigned char cP1, unsigned char cP2, unsigned char* nRetSize)
{
    unsigned short cRet;

    tSCT.cAPDULength = CCID_SIZE_GETDATA;

    tSCT.cAPDU[CCID_CLA] = 0x00;
    tSCT.cAPDU[CCID_INS] = 0xCA;
    tSCT.cAPDU[CCID_P1] = cP1;
    tSCT.cAPDU[CCID_P2] = cP2;
    tSCT.cAPDU[CCID_LC] = 0;    // LE = 256

    cRet = SendAPDU (&tSCT);

    *nRetSize = tSCT.cAPDUAnswerLength;

    return (cRet);
}

/*******************************************************************************

  CcidChangePin

*******************************************************************************/

unsigned short CcidChangePin (unsigned char cPinNr, const uint8_t * szPin, const uint8_t * szNewPin)
{
    unsigned short cRet;
    const uint8_t maximum_PIN_length = CCID_MAX_PIN_LENGTH;
    const uint8_t pin_len_old = (const uint8_t) strnlen((const char *) szPin, maximum_PIN_length);
    const uint8_t pin_len_new = (const uint8_t) strnlen((const char *) szNewPin, maximum_PIN_length);

    const uint16_t total_pin_length = pin_len_old + pin_len_new;
    if (total_pin_length>CCID_MAX_PIN_LENGTH) return 0xFFFF;

    tSCT.cAPDULength = (uint8_t) total_pin_length;

    tSCT.cAPDU[CCID_CLA] = 0x00;
    tSCT.cAPDU[CCID_INS] = 0x24;
    tSCT.cAPDU[CCID_P1] = 0x00;
    tSCT.cAPDU[CCID_P2] = (unsigned char) (0x80 + cPinNr);
    tSCT.cAPDU[CCID_LC] = tSCT.cAPDULength;

    strncpy ((char *) &tSCT.cAPDU[CCID_DATA], (const char *) szPin, pin_len_old);
    strncpy ((char *) &tSCT.cAPDU[CCID_DATA] + pin_len_old, (const char *) szNewPin, pin_len_new);

    tSCT.cAPDULength += CCID_DATA;

    cRet = SendAPDU (&tSCT);

    return (cRet);
}

/*******************************************************************************

  CcidVerifyPin

*******************************************************************************/

unsigned short CcidVerifyPin (unsigned char cPinNr, const uint8_t * szPin)
{
    const uint8_t maximum_PIN_length = CCID_MAX_PIN_LENGTH;
    const uint8_t pin_len = (const uint8_t) strnlen((const char *) szPin, maximum_PIN_length);

    unsigned short cRet;

    tSCT.cAPDULength = pin_len;

    tSCT.cAPDU[CCID_CLA] = 0x00;
    tSCT.cAPDU[CCID_INS] = 0x20;
    tSCT.cAPDU[CCID_P1] = 0x00;
    tSCT.cAPDU[CCID_P2] = 0x80 + cPinNr;
    tSCT.cAPDU[CCID_LC] = tSCT.cAPDULength;

    strncpy ((char *) &tSCT.cAPDU[CCID_DATA], (const char *)szPin, pin_len);

    tSCT.cAPDULength += CCID_DATA;

    cRet = SendAPDU (&tSCT);

    return (cRet);
}


/*******************************************************************************

  CcidUnblockPin

*******************************************************************************/

unsigned short CcidUnblockPin(uint8_t *new_pin)
{
    const uint8_t maximum_PIN_length = CCID_MAX_PIN_LENGTH;
    const uint8_t new_pin_len = (const uint8_t) strnlen((const char *) new_pin, maximum_PIN_length);
    unsigned short nRet;

    // Command
    tSCT.cAPDU[CCID_CLA] = 0x00;
    tSCT.cAPDU[CCID_INS] = 0x2C;
    tSCT.cAPDU[CCID_P1] = 0x02;
    tSCT.cAPDU[CCID_P2] = 0x81;
    tSCT.cAPDU[CCID_LC] = new_pin_len;

    // New password
    strncpy ((char *) &tSCT.cAPDU[CCID_DATA], (const char *) new_pin, new_pin_len);

    tSCT.cAPDULength = (uint8_t) (CCID_DATA + new_pin_len);

    nRet = SendAPDU (&tSCT);
    return (nRet);
}


/*******************************************************************************

  CcidDecipher

*******************************************************************************/

#define CCID_SIZE_DECIPHER 7

#define CCID_DECIPHER_STRING_SIZE	0x80

unsigned short CcidDecipher (unsigned char* nRetSize)
{
    unsigned short cRet;

    unsigned char cDecipherString[CCID_DECIPHER_STRING_SIZE] = {
        0x40, 0xA3, 0xBE, 0x75, 0xFA, 0xF4, 0x3E, 0xAB, 0xBB, 0x8C, 0xD8,
        0xB3, 0x31, 0x96, 0x81, 0xCF,
        0x3A, 0x09, 0x85, 0x8D, 0x2A, 0x4A, 0x18, 0x6F, 0x13, 0x4D, 0x3D,
        0x11, 0x22, 0x72, 0xFA, 0x66,
        0x6D, 0x2D, 0x44, 0xF3, 0x0D, 0x23, 0xDF, 0x3A, 0x77, 0x4D, 0x3F,
        0x89, 0x20, 0x87, 0x37, 0xAC,
        0x97, 0x3A, 0x9B, 0x1F, 0x24, 0xC5, 0xFE, 0x05, 0xDE, 0x8B, 0x2C,
        0x21, 0xCE, 0x25, 0x67, 0xA6,
        0x10, 0xB7, 0xC8, 0xF3, 0xE8, 0x46, 0x62, 0x4F, 0x31, 0x5B, 0x0C,
        0xCD, 0x3D, 0xAF, 0x64, 0x41,
        0x3B, 0x17, 0x45, 0x39, 0xFE, 0x56, 0x4D, 0x6A, 0x68, 0xDA, 0x8C,
        0x43, 0xA5, 0xA7, 0x7C, 0x59,
        0xFB, 0x57, 0x6A, 0x9F, 0x96, 0x93, 0x04, 0xDC, 0x77, 0xE8, 0x3F,
        0xC8, 0xC0, 0x54, 0xB8, 0x99,
        0x8D, 0xE8, 0x2F, 0x43, 0xE4, 0x55, 0xBA, 0x0E, 0x94, 0xE6, 0xA3,
        0x10, 0x59, 0xF0, 0xCF, 0x56
    };

    tSCT.cAPDULength = CCID_SIZE_DECIPHER + CCID_DECIPHER_STRING_SIZE;

    tSCT.cAPDU[CCID_CLA] = 0x00;
    tSCT.cAPDU[CCID_INS] = 0x2A;
    tSCT.cAPDU[CCID_P1] = 0x80;
    tSCT.cAPDU[CCID_P2] = 0x86;
    tSCT.cAPDU[CCID_LC] = 0x81;
    tSCT.cAPDU[CCID_LC + 1] = 0x00; // Padding indicator byte

    memcpy (&tSCT.cAPDU[CCID_LC + 2], cDecipherString, CCID_DECIPHER_STRING_SIZE);  // string
    // to
    // decipher

    tSCT.cAPDU[CCID_LC + 1 + CCID_DECIPHER_STRING_SIZE + 1] = 0x00; // Le =
    // 256
    // Byte

    cRet = SendAPDU (&tSCT);

    *nRetSize = tSCT.cAPDUAnswerLength;

    return (cRet);
}



/*******************************************************************************

  CcidGetChallenge

*******************************************************************************/

unsigned short CcidGetChallenge (const size_t dest_size, unsigned char* dest)
{
    ClearSCTStruct(&tSCT);

    // Command
    tSCT.cAPDU[CCID_CLA] = 0x00;
    tSCT.cAPDU[CCID_INS] = 0x84;
    tSCT.cAPDU[CCID_P1] = 0x00;
    tSCT.cAPDU[CCID_P2] = 0x00;
    tSCT.cAPDULength = 4;

    // Encode Le
    if (dest_size > 255)
    {
        tSCT.cAPDULength += 2;
        tSCT.cAPDU[CCID_DATA] = 0;
        tSCT.cAPDU[CCID_DATA + 1] = (unsigned char) dest_size >> 8;
        tSCT.cAPDU[CCID_DATA + 2] = (unsigned char) (dest_size & 0xFF);
    }
    else {
        tSCT.cAPDULength += 1;
        tSCT.cAPDU[CCID_DATA] = dest_size;
    }

    g_scReqSource = REQ_SRC_INTERNAL;
    int cRet = SendAPDU (&tSCT);
    g_scReqSource = REQ_SRC_NOT_SET;

    // clamp to the received data size, and requested length
    // if response is shorter than requested, the bytes in the target buffer are not touched at all
    const int received_data_size_total = tSCT.cAPDUAnswerLength;
    const int header_size = CCID_DATA;
    const int total_received_data_size = received_data_size_total - header_size;
    const size_t total_data_buffer_size = sizeof(tSCT.cAPDU) - header_size;
    const int sizeToCopySrc = MIN(total_data_buffer_size, total_received_data_size);   // clamp to source data size
    const int sizeToCopyFinal = MIN(dest_size, sizeToCopySrc);                         // clamp to target buffer size
    if (cRet == APDU_ANSWER_COMMAND_CORRECT && sizeToCopyFinal > 0) {
        memcpy (dest, &(tSCT.cAPDU[CCID_DATA]), sizeToCopyFinal);
    }

    return cRet;
}


/*******************************************************************************

    CcidPutAesKey

*******************************************************************************/
unsigned short CcidPutAesKey (unsigned int cKeyLen, unsigned char* pcAES_Key)
{
    int nRet;

    // Correct key len ?
    if ((16 != cKeyLen) && (32 != cKeyLen))
    {
        return (FALSE);
    }

    // Command
    tSCT.cAPDU[CCID_CLA] = 0x00;
    tSCT.cAPDU[CCID_INS] = 0xDA;
    tSCT.cAPDU[CCID_P1] = 0x00;
    tSCT.cAPDU[CCID_P2] = 0xD5;

    // Send password
    tSCT.cAPDU[CCID_LC] = cKeyLen;

    memcpy (&(tSCT.cAPDU[CCID_DATA]), pcAES_Key, cKeyLen);

    // Nothing to receive
    // tSCT.tAPDU.nLe = 0;

    tSCT.cAPDULength = 4 + 1 + cKeyLen;
    nRet = SendAPDU (&tSCT);

    return (nRet);
}


/*******************************************************************************

    CcidAesDecSub

*******************************************************************************/
int CcidAesDecSub (int nSendLength, unsigned char* cSendData, int nReceiveLength, unsigned char* cReceiveData)
{
    int nRet;

    int n;

#ifdef DEBUG_OPENPGP_SHOW_CALLS
    // CI_TickLocalPrintf ("ISO7816: Call AES_Dec_SUB\r\n");
#endif

    // Correct key len ?
    if ((16 != nSendLength) && (32 != nSendLength))
    {
        return (FALSE);
    }

    /* Don't restart because the password access is lost if (FALSE == LA_SC_StartSmartcard ()) { return (FALSE); } */
    // Command
    tSCT.cAPDU[CCID_CLA] = 0x00;
    tSCT.cAPDU[CCID_INS] = 0x2A;
    tSCT.cAPDU[CCID_P1] = 0x80;
    tSCT.cAPDU[CCID_P2] = 0x86;

    // Send cryptogram
    tSCT.cAPDU[CCID_LC] = nSendLength + 1;
    tSCT.cAPDU[CCID_DATA] = 0x02;   // AES key

    memcpy (&tSCT.cAPDU[CCID_DATA + 1], cSendData, nSendLength);

    // Something to receive
    tSCT.cAPDU[CCID_DATA + 1 + nSendLength] = nSendLength;  // nReceiveLength;

    tSCT.cAPDULength = 5    // APDU header
        + 1 // AES key selector
        + nSendLength;  // Payload


    nRet = SendAPDU (&tSCT);

    n = tSCT.cAPDUAnswerLength;
    if (n < nReceiveLength)
    {
        n = nReceiveLength;
    }

    memcpy (cReceiveData, &(tSCT.cAPDU[CCID_DATA]), n);

    return (nRet);
}


/*******************************************************************************

    CcidAesDec

*******************************************************************************/
int CcidAesDec (int nSendLength, unsigned char* cSendData, int nReceiveLength, unsigned char* cReceiveData)
{
    int nRet;

    // 128 bit key ?
    if (16 == nSendLength)
    {
        nRet = CcidAesDecSub (nSendLength, cSendData, nReceiveLength, cReceiveData);
        return nRet;
        /* if (nRet != APDU_ANSWER_COMMAND_CORRECT) // Sc send no ok { return (FALSE); }

           return TRUE; */
    }


    // 256 bit key ?
    if (32 == nSendLength)
    {
        // Decrypt first 16 Byte
        nRet = CcidAesDecSub (16, cSendData, 16, cReceiveData);
        if (nRet != APDU_ANSWER_COMMAND_CORRECT)
        {
            return nRet;
        }

        // Decrypt second 16 Byte
        nRet = CcidAesDecSub (16, &cSendData[16], 16, &cReceiveData[16]);
        return nRet;
        /*
           if ( nRet != APDU_ANSWER_COMMAND_CORRECT ) { return (FALSE); } return TRUE; */
    }

    return (FALSE);
}


/*******************************************************************************

    GetRandomNumber_u32

*******************************************************************************/

u32 getRandomNumber (u32 Size_u32, u8 * Data_pu8)
{
    // Size ok ?
    if (APDU_MAX_RESPONSE_LEN <= Size_u32)
    {
        return (FALSE);
    }

    // Get a random number from smartcard
    for (int i = 0; i < 10; ++i) {
        if (CcidGetChallenge(Size_u32, Data_pu8) == APDU_ANSWER_COMMAND_CORRECT) {
            return TRUE;
        }
        Delay_noUSBCheck(1);
    }
    return (FALSE);

#ifdef GENERATE_RANDOM_NUMBER_WITH_2ND_SOURCE
    // FIXME check does this actually add entropy?
    // Paranoia: if the random number is not really random, xor it with
    // another random number from a second source
    if (FALSE == FlasgTimeIsSet_u8)
    {
        time (&now);    // Get the local time
        srand (now + Data_pu8[0]);  // Init the local random generator
        FlasgTimeIsSet_u8 = TRUE;
    }

    for (i = 0; i < Size_u32; i++)
        Data_pu8[i] = Data_pu8[i] ^ (u8) (rand () % 256);
#endif

    return (TRUE);
}

/*******************************************************************************

  AccessTestGPG

*******************************************************************************/

#ifdef COMPILE_TESTS
int AccessTestGPG (void)
{
    unsigned short cRet;

    unsigned char nReturnSize;

    // CcidSelect1 ();

    if (APDU_ANSWER_COMMAND_CORRECT != CcidSelectOpenPGPApp ())
    {
        return (FALSE);
    }

    /* GETDATA 0x00 0x4F - Application identifier (AID), 16 (dec.) bytes (ISO 7816-4) */
    cRet = CcidGetData (0x00, 0x4F, &nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }
    if (16 != nReturnSize)
    {
        return (FALSE);
    }

    /* GETDATA 0x5F 0x52 - Historical bytes, up to 15 bytes (dec.), according to ISO 7816-4. Card capabilities shall be included. */
    cRet = CcidGetData (0x5F, 0x52, &nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }
    if (10 != nReturnSize)
    {
        return (FALSE);
    }

    /* GETDATA 0x00 0xC4 - PW Status Bytes */
    cRet = CcidGetData (0x00, 0xC4, &nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }
    if (7 != nReturnSize)
    {
        return (FALSE);
    }

    /* GETDATA 0x00 0x6E - Application Related Data */
    cRet = CcidGetData (0x00, 0x6E, &nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }
    if (217 != nReturnSize)
    {
        return (FALSE);
    }

    /* GETDATA 0x 0x5E - Login data */
    cRet = CcidGetData (0x00, 0x5E, &nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }
    if (0 != nReturnSize)
    {
        return (FALSE);
    }

    /* GETDATA 0x00 0x6E - Application Related Data */
    cRet = CcidGetData (0x00, 0x6E, &nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }
    if (217 != nReturnSize)
    {
        return (FALSE);
    }

    /* GETDATA 0x00 0x6E - Application Related Data */
    cRet = CcidGetData (0x00, 0x6E, &nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }
    if (217 != nReturnSize)
    {
        return (FALSE);
    }

    /* GETDATA 0x00 0x6E - Application Related Data */
    cRet = CcidGetData (0x00, 0x6E, &nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }
    if (217 != nReturnSize)
    {
        return (FALSE);
    }


    /* VERIFY PIN 2 */
    cRet = CcidVerifyPin (2, "123456");
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }


    /* VERIFY PIN 1 */
    cRet = CcidVerifyPin (1, "123456");
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }


    /* DECIPHER */
    cRet = CcidDecipher (&nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (FALSE);
    }
    if (35 != nReturnSize)
    {
        return (FALSE);
    }

    return (TRUE);
}


/*******************************************************************************

  CcidLocalAccessTest

*******************************************************************************/

void CcidLocalAccessTest (void)
{

    InitSCTStruct (&tSCT);

    SmartCardInitInterface ();

    /* Access sequence like gpg */
    AccessTestGPG ();


    while (1)
    {
    }

}
#endif //COMPILE_TESTS

/*
 * Needs Admin Authentication before called
 */
unsigned int CcidReset (void)
{
    unsigned short cRet;

    // TERMINATE DF
    tSCT.cAPDU[CCID_CLA] = 0x00;
    tSCT.cAPDU[CCID_INS] = 0xE6;
    tSCT.cAPDU[CCID_P1] = 0x00;
    tSCT.cAPDU[CCID_P2] = 0x00;
    tSCT.cAPDU[CCID_LC] = 0;
    tSCT.cAPDULength = 4;
    cRet = SendAPDU (&tSCT);

    // Select OpenPGP application
    if (APDU_ANSWER_COMMAND_CORRECT == cRet)
    {
        cRet = CcidSelectOpenPGPApp ();

        // ACTIVATE FILE
        if (APDU_ANSWER_SEL_FILE_TERM_STATE == cRet)
        {   // Card in termination state
            tSCT.cAPDU[CCID_CLA] = 0x00;
            tSCT.cAPDU[CCID_INS] = 0x44;
            tSCT.cAPDU[CCID_P1] = 0x00;
            tSCT.cAPDU[CCID_P2] = 0x00;
            tSCT.cAPDU[CCID_LC] = 0;
            tSCT.cAPDULength = 4;
            cRet = SendAPDU (&tSCT);

            return (cRet);
        }
        else
            return (cRet);
    }
    else
        return (cRet);


    return (cRet);
}

int getAID (void)
{

    InitSCTStruct (&tSCT);

    unsigned char nReturnSize;

    CcidSelectOpenPGPApp ();
    CcidGetData (0x00, 0x4F, &nReturnSize);

    return nReturnSize;
}

uint32_t min(const uint32_t a, const uint32_t b){
  return a>b ? b : a;
}

uint16_t send_data_no_response(uint8_t *data, uint8_t data_length){
  uint16_t cRet;
  tSCT.cAPDULength = data_length;
  memcpy ((void *) &tSCT.cAPDU, data, tSCT.cAPDULength);
  cRet = SendAPDU (&tSCT);
  return cRet;
}


static uint8_t * parse_tlv_out_buf = NULL;
static uint16_t parse_tlv_out_len = 0;

int tag_callback(TLV *tlv, uint16_t depth){
    if (tlv->tag[0] == 0x5F && tlv->tag[1] == 0x20){
        tlv->length = min(tlv->length, 0x10);
        memcpy(parse_tlv_out_buf, tlv->value, min(parse_tlv_out_len, tlv->length));
        return 1;
    }
    return 0;
}

/**
 * Get smart card's serial number and copy it to out_serial_number.
 * @param out_serial_number buffer to place the data, result is in ASCII
 * @param buffer_length length of out_serial_number
 * @return smartcard status and execution phase info
 */
retCode getSerialNumber (uint8_t * out_serial_number, const uint16_t buffer_length)
{
  InitSCTStruct (&tSCT);
  retCode cRet = {};

  //Select HSM App - 1
  unsigned char cSelectHSM1[] = { 0x00, 0xA4, 0x04, 0x00, 0x0B, 0xE8, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x81, 0xC3, 0x1F, 0x02, 0x01 };
  cRet.smartcard_ret = send_data_no_response(cSelectHSM1, sizeof(cSelectHSM1));
  cRet.execution_phase++;
  if (APDU_ANSWER_COMMAND_CORRECT != cRet.smartcard_ret) return cRet;

  //Select HSM App - 2
  unsigned char cSelectHSM[] = { 0x00, 0xA4, 0x00, 0x00, 0x02, 0x2F, 0x02, 0x00 };
  cRet.smartcard_ret = send_data_no_response (cSelectHSM, sizeof(cSelectHSM));
  cRet.execution_phase++;
  if (APDU_ANSWER_COMMAND_CORRECT != cRet.smartcard_ret) return cRet;

  //Request data from smartcard
  unsigned char getSerial[] = { 0x00, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x04, 0x54, 0x02, 0x00, 0x00, 0x04, 0x00 };
  cRet.smartcard_ret = send_data_no_response (getSerial, sizeof(getSerial));
  cRet.execution_phase++;
  cRet.len = tSCT.cAPDUAnswerLength;
  if (APDU_ANSWER_COMMAND_CORRECT != cRet.smartcard_ret && APDU_EOF != cRet.smartcard_ret) return cRet;

  parse_tlv_out_buf = out_serial_number;
  parse_tlv_out_len = buffer_length;
  parse_tlv(tSCT.cAPDU, tSCT.cAPDU+cRet.len, tag_callback);

  InitSCTStruct (&tSCT);

  cRet.execution_phase++;
  return cRet;
}


uint8_t getByteOfData (uint8_t x)
{


    return tSCT.cAPDU[x];
}

uint8_t cardAuthenticate (uint8_t * password)
{
    unsigned short cRet;
    InitSCTStruct (&tSCT);

    CcidSelectOpenPGPApp ();
    const int VERIFY_ADMIN_PIN = 3;
    cRet = CcidVerifyPin (VERIFY_ADMIN_PIN, password);

    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return FALSE;
    }

    return TRUE;
}

uint8_t userAuthenticate (uint8_t * password)
{
    unsigned short cRet;
    InitSCTStruct (&tSCT);

    CcidSelectOpenPGPApp ();
    const int VERIFY_USER_PIN = 1;
    cRet = CcidVerifyPin (VERIFY_USER_PIN, password);

    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return 1; //incorrect
    }

    return 0; //correct
}

uint8_t factoryReset (uint8_t * password)
{
unsigned short cRet;

    // Reset smart card
    CcidSelectOpenPGPApp ();

    cRet = CcidVerifyPin (3, password);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
        return 1;

    cRet = CcidReset ();
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
        return 0;

  const size_t SLOT_SIZE = sizeof(OTP_slot);
    // Erase OTP slots
uint8_t slot_no;

uint8_t slot_tmp[SLOT_SIZE];

    memset (slot_tmp, 0xFF, SLOT_SIZE);

  for (slot_no = 0; slot_no < NUMBER_OF_HOTP_SLOTS; slot_no++)    // HOTP
        // slots
    {
        write_to_slot ((OTP_slot *) slot_tmp, get_HOTP_slot_offset(slot_no), SLOT_SIZE);
        erase_counter (slot_no);
    }
    for (slot_no = 0; slot_no < NUMBER_OF_TOTP_SLOTS; slot_no++)    // TOTP
        // slots
    {
        write_to_slot ((OTP_slot *) slot_tmp, get_TOTP_slot_offset(slot_no), SLOT_SIZE);
    }

    // Clear USER_PAGE but leave update password intact
    u8 UpdatePinHash_u8[32];
    u8 UpdatePinSalt_u8[10];
    ReadUpdatePinHashFromFlash(UpdatePinHash_u8);
    ReadUpdatePinSaltFromFlash(UpdatePinSalt_u8);

    // Default flash memory
    EraseLocalFlashKeyValues_u32 ();

    WriteUpdatePinHashToFlash (UpdatePinHash_u8);
    WriteUpdatePinSaltToFlash (UpdatePinSalt_u8);
    memset(UpdatePinHash_u8, 0, sizeof UpdatePinHash_u8);
    memset(UpdatePinSalt_u8, 0, sizeof UpdatePinSalt_u8);

    return 0;
}

uint8_t changeUserPin (uint8_t * password, uint8_t * new_password)
{
    InitSCTStruct (&tSCT);

unsigned short cRet;

    CcidSelectOpenPGPApp ();
    cRet = CcidChangePin (1, password, new_password);


    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return -1;
    }

    return 0;
}

uint8_t changeAdminPin (uint8_t * password, uint8_t * new_password)
{
    InitSCTStruct (&tSCT);

unsigned short cRet;

    CcidSelectOpenPGPApp ();
    cRet = CcidChangePin (3, password, new_password);


    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return -1;
    }

    return 0;
}

uint8_t unblockPin (uint8_t * new_pin)
{
    InitSCTStruct (&tSCT);

unsigned short cRet;

    CcidSelectOpenPGPApp ();

    cRet = CcidUnblockPin (new_pin);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return -1;
    }

    return 0;
}

uint8_t getPasswordRetryCount ()
{

    InitSCTStruct (&tSCT);

unsigned short cRet;

unsigned char nReturnSize;


    CcidSelectOpenPGPApp ();
    cRet = CcidGetData (0x00, 0xC4, &nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (0xFF);
    }

    return tSCT.cAPDU[6];
}

uint8_t getUserPasswordRetryCount ()
{

    InitSCTStruct (&tSCT);

unsigned short cRet;

unsigned char nReturnSize;


    CcidSelectOpenPGPApp ();
    cRet = CcidGetData (0x00, 0xC4, &nReturnSize);
    if (APDU_ANSWER_COMMAND_CORRECT != cRet)
    {
        return (0xFF);
    }

    return tSCT.cAPDU[4];
}

uint8_t isAesSupported (void)
{
    InitSCTStruct (&tSCT);

    CcidSelectOpenPGPApp ();

unsigned short cRet;

    tSCT.cAPDU[CCID_CLA] = 0x00;
    tSCT.cAPDU[CCID_INS] = 0x2A;
    tSCT.cAPDU[CCID_P1] = 0x80;
    tSCT.cAPDU[CCID_P2] = 0x86;
    tSCT.cAPDU[CCID_LC] = 17;

char test_data[17] = { 0x02,    // Use AES to DECIPHER
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
    memcpy ((char *) &tSCT.cAPDU[CCID_DATA], test_data, 17);
    tSCT.cAPDU[CCID_DATA + 17] = 0;
    tSCT.cAPDU[CCID_DATA + 18] = 0;

    tSCT.cAPDULength = 5 + 17;
    cRet = SendAPDU (&tSCT);


    /* Possible answers and intepretations: * * APDU_ANSWER_COMMAND_CORRECT (SW1=90, SW2=00) * Successfully used AES module to decipher => AES module
       exists * * APDU_ANSWER_REF_DATA_NOT_FOUND (SW1=6A, SW2=88) * AES key not found => AES module exists, and the error is at the non existing key *
       * APDU_ANSWER_USE_CONDIT_NOT_SATISFIED (SW1=69, SW2=85) * AES module doen't exist * */

    // Determine if AES module exists
    if ((APDU_ANSWER_COMMAND_CORRECT == cRet) || (APDU_ANSWER_REF_DATA_NOT_FOUND == cRet))
        return TRUE;
    else
        return FALSE;
}

uint8_t sendAESMasterKey (int nLen, unsigned char* pcMasterKey)
{
unsigned int nRet;

    nRet = CcidPutAesKey (nLen, pcMasterKey);
    if (FALSE == nRet)
    {
        return (FALSE);
    }

    if (APDU_ANSWER_COMMAND_CORRECT == nRet)
        return (TRUE);
    else
        return (FALSE);
}

//FIXME correct naming
uint8_t testScAesKey (int nLen, unsigned char* pcKey)
{
int nRet;

unsigned char acBufferOut[32];

    if (32 < nLen)
    {
        return (FALSE);
    }

    memset (acBufferOut, 0, nLen);

    nRet = CcidAesDec (nLen, pcKey, nLen, acBufferOut);

    if (APDU_ANSWER_COMMAND_CORRECT == nRet)
        return (TRUE);
    else
        return (FALSE);
}

//FIXME correct naming
uint8_t testSendUserPW2 (unsigned char* pcPW)
{
    unsigned short nRet;

    // CI_LocalPrintf ("Send user password : ");
    nRet = CcidVerifyPin (2, pcPW);
    if (APDU_ANSWER_COMMAND_CORRECT == nRet)
    {
        // CI_LocalPrintf ("fail\n\r");
        return (TRUE);
    }
    // CI_LocalPrintf ("OK \n\r");

    return (FALSE);
}

