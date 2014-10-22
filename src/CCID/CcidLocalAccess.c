/*
* Author: Copyright (C) Rudolf Boeddeker 				Date: 2010-02-01
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

/*
		to do : CRC check of receive TPDU data
*/


#include "stm32f10x.h"
#include "platform_config.h"
#include "hw_config.h"
#include "CCID_usb.h"
#include "string.h"
#include "smartcard.h"
#include "CCID_Ifd_protocol.h"
#include "CcidLocalAccess.h"

#include "time.h"

#define CCID_TRANSFER_BUFFER_MAX		256

#define CCID_TPDU_OVERHEAD 					4
#define CCID_TPDU_PROLOG  					3
#define CCID_TPDU_ANSWER_OVERHEAD  	6


#define CCID_TPDU_NAD				 0
#define CCID_TPDU_PCD				 1
#define CCID_TPDU_LENGTH 		 2
#define CCID_TPDU_DATASTART	 3

#define CCID_TPDU_R_BLOCK_FLAG								0x80
#define CCID_TPDU_R_BLOCK_SEQUENCE_FLAG 			0x10
#define CCID_TPDU_CHAINING_FLAG								0x20


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
	unsigned char  		cAPDULength;
	unsigned short		cAPDUAnswerStatus;
	unsigned char			cAPDUAnswerLength;
	unsigned char  		cTPDUSequence;
	unsigned char  		cTPDULength;
	unsigned char			cAPDU[CCID_TRANSFER_BUFFER_MAX];
	unsigned char			cTPDU[CCID_TRANSFER_BUFFER_MAX+CCID_TPDU_OVERHEAD];
} typeSmartcardTransfer;

static typeSmartcardTransfer tSCT;

/*******************************************************************************

  InitSCTStruct

*******************************************************************************/

void InitSCTStruct (typeSmartcardTransfer *tSCT)
{
	tSCT->cAPDULength       = 0;
	tSCT->cAPDUAnswerLength = 0;
	tSCT->cAPDUAnswerStatus = 0;
	tSCT->cTPDUSequence     = 0;
}

/*******************************************************************************

  GenerateCRC

*******************************************************************************/

unsigned char GenerateCRC (unsigned char *pData,unsigned char cLength)
{
	unsigned char cCRC = 0;
	int i;

  for (i=0;i<cLength;i++)
	{
    cCRC ^= pData[i];
  }

	return (cCRC);
}

/*******************************************************************************

  GenerateTPDU

*******************************************************************************/

void GenerateTPDU (typeSmartcardTransfer *tSCT)
{
	tSCT->cTPDU[CCID_TPDU_NAD]    = 0;																		// Node Address (NAD)
	tSCT->cTPDU[CCID_TPDU_PCD]    = (tSCT->cTPDUSequence & 1) << 6;			  // Protocol Control Byte
	tSCT->cTPDU[CCID_TPDU_LENGTH] = tSCT->cAPDULength;

	tSCT->cTPDULength = tSCT->cAPDULength + CCID_TPDU_OVERHEAD; 					// the length of the TPDU
	tSCT->cTPDUSequence++;																								// switch sequence

	memcpy (&tSCT->cTPDU[CCID_TPDU_DATASTART],tSCT->cAPDU,tSCT->cAPDULength); // copy APDU data

 	tSCT->cTPDU[tSCT->cAPDULength+CCID_TPDU_OVERHEAD-1] = 								// set CRC at end of data
						GenerateCRC ((unsigned char*)&tSCT->cTPDU,tSCT->cAPDULength+CCID_TPDU_PROLOG);
}

/*******************************************************************************

  GenerateChainedTPDU

*******************************************************************************/

void GenerateChainedTPDU (typeSmartcardTransfer *tSCT)
{
	tSCT->cTPDU[CCID_TPDU_NAD]    = 0;												// Node Address (NAD)
	tSCT->cTPDU[CCID_TPDU_PCD]    = ((tSCT->cTPDUSequence & 1) << 4) + CCID_TPDU_R_BLOCK_FLAG;			  // Protocol Control Byte
	tSCT->cTPDU[CCID_TPDU_LENGTH] = 0;

	tSCT->cTPDULength = CCID_TPDU_OVERHEAD; 									// the length of the TPDU
	tSCT->cTPDUSequence++;																		// switch sequence

 	tSCT->cTPDU[CCID_TPDU_OVERHEAD-1] = GenerateCRC ((unsigned char*)&tSCT->cTPDU,CCID_TPDU_PROLOG);
}



/*******************************************************************************

  SendTPDU

*******************************************************************************/

unsigned short SendTPDU (typeSmartcardTransfer *tSCT)
{
	unsigned int   nAnswerLength = 0;
	unsigned char  nOverhead     = 0;

// Send TPDU to smartcard an receive answer
	CRD_SendCommand ((unsigned char*) tSCT->cTPDU,
									                  tSCT->cTPDULength,
																	  CCID_TRANSFER_BUFFER_MAX,
									 (unsigned int*) &nAnswerLength);


	if (CCID_TPDU_ANSWER_OVERHEAD > nAnswerLength)
	{																																					// answer length incorrect
		tSCT->cAPDUAnswerStatus = APDU_ANSWER_RECEIVE_INCORRECT;
		return (tSCT->cAPDUAnswerStatus);
	}

	if (0 != (tSCT->cTPDU[CCID_TPDU_PCD] & CCID_TPDU_CHAINING_FLAG))					// chained data
	{
		nOverhead               = CCID_TPDU_ANSWER_OVERHEAD - 2;								// no status data
		tSCT->cAPDUAnswerStatus = APDU_ANSWER_CHAINED_DATA;
	}
	else
	{
		nOverhead = CCID_TPDU_ANSWER_OVERHEAD;
		tSCT->cAPDUAnswerStatus  = tSCT->cTPDU[nAnswerLength-3] << 8; 					  // Statusbyte SW1
		tSCT->cAPDUAnswerStatus += tSCT->cTPDU[nAnswerLength-2];	             	  // Statusbyte SW2
	}
	 
	memcpy (&tSCT->cAPDU[tSCT->cAPDUAnswerLength],
	        &tSCT->cTPDU[CCID_TPDU_DATASTART],
					 nAnswerLength - nOverhead);	 																		// add new data to receive data

	tSCT->cAPDUAnswerLength += nAnswerLength - nOverhead;		  								// add length of recieved data

	return (tSCT->cAPDUAnswerStatus);
}


/*******************************************************************************

  SendAPDU

*******************************************************************************/

unsigned short SendAPDU (typeSmartcardTransfer *tSCT)
{
	tSCT->cAPDUAnswerLength = 0;

 	GenerateTPDU (tSCT);

	SendTPDU (tSCT);

	if (APDU_ANSWER_RECEIVE_INCORRECT == tSCT->cAPDUAnswerStatus)								// return on error ??
	{
		return (tSCT->cAPDUAnswerStatus);
	}

// Chained answer ?
	while (0 != (tSCT->cTPDU[CCID_TPDU_PCD] & CCID_TPDU_CHAINING_FLAG))
	{
 		GenerateChainedTPDU (tSCT);
		SendTPDU (tSCT);

		if ((APDU_ANSWER_CHAINED_DATA    != tSCT->cAPDUAnswerStatus)	&&							// return on error ??
		    (APDU_ANSWER_COMMAND_CORRECT != tSCT->cAPDUAnswerStatus))
		{
			return (tSCT->cAPDUAnswerStatus);
		}
	}

	return (tSCT->cAPDUAnswerStatus);
}

/*******************************************************************************

  CcidSelectOpenPGPApp

*******************************************************************************/
#define CCID_SIZE_OPEN_PGP_APP	11

unsigned short CcidSelectOpenPGPApp (void)
{
	unsigned short cRet;
  unsigned char  cOpenPGPApp[CCID_SIZE_OPEN_PGP_APP] = { 0x00,0xA4,0x04,0x00,0x06,0xD2,0x76,0x00,0x01,0x24,0x01 };

	tSCT.cAPDULength = CCID_SIZE_OPEN_PGP_APP;

  memcpy ((void*)&tSCT.cAPDU,cOpenPGPApp,tSCT.cAPDULength);

	cRet = SendAPDU (&tSCT);

	return (cRet);
}

/*******************************************************************************

  CcidGetData

*******************************************************************************/
#define CCID_SIZE_GETDATA	5

unsigned short CcidGetData (unsigned char cP1,unsigned char cP2,unsigned char *nRetSize)
{
	unsigned short cRet;

	tSCT.cAPDULength = CCID_SIZE_GETDATA;

	tSCT.cAPDU[CCID_CLA] = 0x00;
	tSCT.cAPDU[CCID_INS] = 0xCA;
	tSCT.cAPDU[CCID_P1]  = cP1;
	tSCT.cAPDU[CCID_P2]  = cP2;
	tSCT.cAPDU[CCID_LC]  = 0;						// LE = 256

	cRet = SendAPDU (&tSCT);

	*nRetSize = tSCT.cAPDUAnswerLength;

	return (cRet);
}

/*******************************************************************************

  CcidVerifyPin

*******************************************************************************/

unsigned short CcidVerifyPin (unsigned char cPinNr,const char *szPin)
{
	unsigned short cRet;

	tSCT.cAPDULength = strlen (szPin);

	tSCT.cAPDU[CCID_CLA] = 0x00;
	tSCT.cAPDU[CCID_INS] = 0x20;
	tSCT.cAPDU[CCID_P1]  = 0x00;
	tSCT.cAPDU[CCID_P2]  = 0x80+cPinNr;
	tSCT.cAPDU[CCID_LC]  = tSCT.cAPDULength;

	strcpy ((char*)&tSCT.cAPDU[CCID_DATA],szPin);

	tSCT.cAPDULength += CCID_DATA;

	cRet = SendAPDU (&tSCT);

	return (cRet);
}

/*******************************************************************************

  CcidDecipher

*******************************************************************************/

#define CCID_SIZE_DECIPHER 7

#define CCID_DECIPHER_STRING_SIZE	0x80

unsigned short CcidDecipher (unsigned char *nRetSize)
{
	unsigned short cRet;
	unsigned char cDecipherString[CCID_DECIPHER_STRING_SIZE] = 
	{
		0x40,0xA3,0xBE,0x75,0xFA,0xF4,0x3E,0xAB,0xBB,0x8C,0xD8,0xB3,0x31,0x96,0x81,0xCF,
		0x3A,0x09,0x85,0x8D,0x2A,0x4A,0x18,0x6F,0x13,0x4D,0x3D,0x11,0x22,0x72,0xFA,0x66,
		0x6D,0x2D,0x44,0xF3,0x0D,0x23,0xDF,0x3A,0x77,0x4D,0x3F,0x89,0x20,0x87,0x37,0xAC,
		0x97,0x3A,0x9B,0x1F,0x24,0xC5,0xFE,0x05,0xDE,0x8B,0x2C,0x21,0xCE,0x25,0x67,0xA6,
		0x10,0xB7,0xC8,0xF3,0xE8,0x46,0x62,0x4F,0x31,0x5B,0x0C,0xCD,0x3D,0xAF,0x64,0x41,
		0x3B,0x17,0x45,0x39,0xFE,0x56,0x4D,0x6A,0x68,0xDA,0x8C,0x43,0xA5,0xA7,0x7C,0x59,
		0xFB,0x57,0x6A,0x9F,0x96,0x93,0x04,0xDC,0x77,0xE8,0x3F,0xC8,0xC0,0x54,0xB8,0x99,
		0x8D,0xE8,0x2F,0x43,0xE4,0x55,0xBA,0x0E,0x94,0xE6,0xA3,0x10,0x59,0xF0,0xCF,0x56
  };

	tSCT.cAPDULength = CCID_SIZE_DECIPHER + CCID_DECIPHER_STRING_SIZE;

	tSCT.cAPDU[CCID_CLA]  = 0x00;
	tSCT.cAPDU[CCID_INS]  = 0x2A;
	tSCT.cAPDU[CCID_P1]   = 0x80;
	tSCT.cAPDU[CCID_P2]   = 0x86;
	tSCT.cAPDU[CCID_LC]   = 0x81;
	tSCT.cAPDU[CCID_LC+1] = 0x00;						 									// Padding indicator byte

	memcpy (&tSCT.cAPDU[CCID_LC+2],cDecipherString,CCID_DECIPHER_STRING_SIZE);	 // string to decipher

	tSCT.cAPDU[CCID_LC+1+CCID_DECIPHER_STRING_SIZE+1] = 0x00;		 // Le = 256 Byte

	cRet = SendAPDU (&tSCT);

	*nRetSize = tSCT.cAPDUAnswerLength;

	return (cRet);
}



/*******************************************************************************

  CcidGetChallenge

*******************************************************************************/

unsigned short CcidGetChallenge (int nReceiveLength, unsigned char *nReceiveData)
{
  int     cRet;
  int     n;

  // Command
  tSCT.cAPDU[CCID_CLA]    = 0x00; 
  tSCT.cAPDU[CCID_INS]    = 0x84;
  tSCT.cAPDU[CCID_P1]     = 0x00; 
  tSCT.cAPDU[CCID_P2]     = 0x00;

  tSCT.cAPDU[CCID_LC]     = 0;

// Something to receive
  //tSCT.cAPDU.nLe      = nReceiveLength; // nReceiveLength;
 
  // Encode Le 
  if (nReceiveLength > 255)
  {
    tSCT.cAPDU[CCID_DATA] = 0;
    tSCT.cAPDU[CCID_DATA + 1] = (unsigned char) nReceiveLength >> 8;
    tSCT.cAPDU[CCID_DATA + 2] = (unsigned char) (nReceiveLength & 0xFF);
  }
  else
    tSCT.cAPDU[CCID_DATA] = nReceiveLength;

  // cRet = ISO7816_SendAPDU_Le_NoLc ( &tSCT );
  cRet = SendAPDU(&tSCT);

  n = tSCT.cAPDUAnswerLength;
  if (n < nReceiveLength)
  {
    n = nReceiveLength;
  }

  memcpy (nReceiveData, &(tSCT.cAPDU[CCID_DATA]), n);

  return cRet;
}


/*******************************************************************************

    GetRandomNumber_u32

*******************************************************************************/

u32 getRandomNumber (u32 Size_u32, u8 *Data_pu8)
{
    u32 Ret_u32;
    u32 i;
    time_t  now;
    static u8 FlasgTimeIsSet_u8 = FALSE;

    // Size ok ?
    if (APDU_MAX_RESPONSE_LEN <= Size_u32)
    {
        return (FALSE);
    }

    // Get a random number from smartcard
    Ret_u32 = CcidGetChallenge (Size_u32, Data_pu8);

    #ifdef GENERATE_RANDOM_NUMBER_WITH_2ND_SOURCE
    // Paranoia: if the random number is not really random, xor it with another random number from a second source
    if (FALSE == FlasgTimeIsSet_u8)
    {
        time (&now);                      // Get the local time
        srand (now + Data_pu8[0]);        // Init the local random generator
        FlasgTimeIsSet_u8 = TRUE;
    }

    for (i=0;i<Size_u32;i++)
        Data_pu8[i] = Data_pu8[i] ^ (u8)(rand () % 256);
    #endif
}

/*******************************************************************************

  AccessTestGPG

*******************************************************************************/

int AccessTestGPG (void)
{
	unsigned short cRet;
	unsigned char nReturnSize;
//CcidSelect1 ();

 	if (APDU_ANSWER_COMMAND_CORRECT != CcidSelectOpenPGPApp ())
	{
		return (FALSE);
	}

/* GETDATA 0x00 0x4F - Application identifier (AID), 16 (dec.) bytes (ISO 7816-4) */
	cRet = CcidGetData (0x00,0x4F,&nReturnSize);
 	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return (FALSE);
	}
	if (16 != nReturnSize)
	{
		return (FALSE);
	}

/* GETDATA 0x5F 0x52 - Historical bytes, up to 15 bytes (dec.), according to ISO 7816-4. Card capabilities shall be included.*/
	cRet = CcidGetData (0x5F,0x52,&nReturnSize);
 	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return (FALSE);
	}
	if (10 != nReturnSize)
	{
		return (FALSE);
	}

/* GETDATA 0x00 0xC4 - PW Status Bytes*/
	cRet = CcidGetData (0x00,0xC4,&nReturnSize);
 	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return (FALSE);
	}
	if (7 != nReturnSize)
	{
		return (FALSE);
	}

/* GETDATA 0x00 0x6E - Application Related Data */
	cRet = CcidGetData (0x00,0x6E,&nReturnSize);
 	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return (FALSE);
	}
	if (217 != nReturnSize)
	{
		return (FALSE);
	}

/* GETDATA 0x 0x5E - Login data */
	cRet = CcidGetData (0x00,0x5E,&nReturnSize);
 	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return (FALSE);
	}
	if (0 != nReturnSize)
	{
		return (FALSE);
	}

/* GETDATA 0x00 0x6E - Application Related Data */
	cRet = CcidGetData (0x00,0x6E,&nReturnSize);
 	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return (FALSE);
	}
	if (217 != nReturnSize)
	{
		return (FALSE);
	}

/* GETDATA 0x00 0x6E - Application Related Data */
	cRet = CcidGetData (0x00,0x6E,&nReturnSize);
 	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return (FALSE);
	}
	if (217 != nReturnSize)
	{
		return (FALSE);
	}

/* GETDATA 0x00 0x6E - Application Related Data */
	cRet = CcidGetData (0x00,0x6E,&nReturnSize);
 	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return (FALSE);
	}
	if (217 != nReturnSize)
	{
		return (FALSE);
	}


/* VERIFY PIN 2 */
	cRet = CcidVerifyPin (2,"123456");
 	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return (FALSE);
	}


/* VERIFY PIN 1 */
	cRet = CcidVerifyPin (1,"123456");
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


int getAID(void){

InitSCTStruct (&tSCT);

unsigned short cRet;
unsigned char nReturnSize;

CcidSelectOpenPGPApp ();
cRet = CcidGetData (0x00,0x4F,&nReturnSize);

return nReturnSize;
}

uint32_t getSerialNumber(void){

uint32_t serial;
uint8_t buffer[4];

InitSCTStruct (&tSCT);

unsigned short cRet;
unsigned char nReturnSize;

CcidSelectOpenPGPApp ();
cRet = CcidGetData (0x00,0x4F,&nReturnSize);



return 0;


}


uint8_t getByteOfData(uint8_t x){


return tSCT.cAPDU[x];
}

uint8_t cardAuthenticate(uint8_t *password){
	InitSCTStruct (&tSCT);

	unsigned short cRet;
	unsigned char nReturnSize;

	CcidSelectOpenPGPApp ();
	cRet = CcidVerifyPin (3,password);
	

	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return 1;
	}
	

return 0;	
}

uint8_t userAuthenticate(uint8_t *password){
        InitSCTStruct (&tSCT);

        unsigned short cRet;
        unsigned char nReturnSize;

        CcidSelectOpenPGPApp ();
        cRet = CcidVerifyPin (1,password);


        if (APDU_ANSWER_COMMAND_CORRECT != cRet)
        {
                return 1;
        }


return 0;
}


uint8_t getPasswordRetryCount(){
	
	InitSCTStruct (&tSCT);

	unsigned short cRet;
	unsigned char nReturnSize;


	CcidSelectOpenPGPApp ();
	cRet = CcidGetData (0x00,0xC4,&nReturnSize);
 	if (APDU_ANSWER_COMMAND_CORRECT != cRet)
	{
		return (0xFF);
	}

	return tSCT.cAPDU[6];
}

uint8_t getUserPasswordRetryCount(){

        InitSCTStruct (&tSCT);

        unsigned short cRet;
        unsigned char nReturnSize;


        CcidSelectOpenPGPApp ();
        cRet = CcidGetData (0x00,0xC4,&nReturnSize);
        if (APDU_ANSWER_COMMAND_CORRECT != cRet)
        {
                return (0xFF);
        }

        return tSCT.cAPDU[4];
}


