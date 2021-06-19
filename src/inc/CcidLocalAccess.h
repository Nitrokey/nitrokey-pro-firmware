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

#ifndef CCID_LOCAL_ACCESS
#define CCID_LOCAL_ACCESS

#include "stm32f10x.h"

// #ifdef NOT_USED
#define APDU_ANSWER_T0_COMMAND_CORRECT          0x6100  /* Command correct, xx bytes available in response (normally used under T=0 or for commands
                                                           under any protocol with long response data that cannot be transmitted in one response) */
#define APDU_ANSWER_SEL_FILE_TERM_STATE         0x6285  /* Selected file in termination state */
#define APDU_ANSWER_MEMORY_FAILURE              0x6581  /* Memory failure */
#define APDU_ANSWER_WRONG_LENGTH                0x6700  /* Wrong length (Lc and/or Le) */
#define APDU_ANSWER_SEC_MSG_NOT_SUPPORTED       0x6882  /* Secure messaging not supported */
#define APDU_ANSWER_LAST_CHAIN_CMD_EXPECTED     0x6884  /* Last command of the chain expected */
#define APDU_ANSWER_SEC_STATUS_NOT_SATISFIED    0x6982  /* Security status not satisfied */
#define APDU_ANSWER_AUTH_METHOD_BLOCKED         0x6983  /* Authentication method blocked */
#define APDU_ANSWER_USE_CONDIT_NOT_SATISFIED    0x6985  /* Condition of use not satisfied */
#define APDU_ANSWER_SM_DATA_MISSING				0x6987  /* Expected SM data objects missing (e.g. SM-key, SSC) */
#define APDU_ANSWER_SM_WRONG_DATA				0x6988  /* SM data objects incorrect (e.g. wrong TLV-structure in command data) */
#define APDU_ANSWER_WRONG_DATA_FIELD            0x6A80  /* Incorrect parameters in the data field */
#define APDU_ANSWER_REF_DATA_NOT_FOUND          0x6A88  /* Referenced data not found */
#define APDU_ANSWER_WRONG_P1_P2             	0x6B00  /* Wrong parameters P1-P2 */
#define APDU_ANSWER_INS_NOT_SUPPORTED			0x6D00  /* Instruction (INS) not supported */
#define APDU_ANSWER_CLA_NOT_SUPPORTED           0x6E00  /* Class (CLA) not supported */
#define APDU_EOF           0x6282  /* End of file/record reached before reading Le bytes */
// #endif

#define APDU_ANSWER_COMMAND_CORRECT				0x9000  /* Command correct */
#define APDU_ANSWER_RECEIVE_CRC_ERROR			0xA000  /* Receive CRC error */
#define APDU_ANSWER_RECEIVE_INCORRECT			0xA001  /* Receive wrong answer struct */
#define APDU_ANSWER_CHAINED_DATA    			0xA002  /* Receive chained data */

#define APDU_MAX_RESPONSE_LEN       500


// Definitions for APDU
#define ISO7816_APDU_MAX_RESPONSE_LEN   500
#define ISO7816_APDU_SEND_HEADER_LEN      5
#define ISO7816_MAX_APDU_DATA           500
#define ISO7816_APDU_OFERHEAD             4 // for Le+Lc and checksum

void CcidLocalAccessTest (void);

int getAID (void);

typedef struct {
  uint16_t smartcard_ret;
  uint8_t execution_phase;
  uint8_t len;
} retCode;

retCode getSerialNumber (uint8_t*, uint16_t);

uint8_t getByteOfData (uint8_t x);

uint8_t cardAuthenticate (uint8_t * password);

uint8_t userAuthenticate (uint8_t * password);

uint8_t changeUserPin (uint8_t * password, uint8_t * new_password);

uint8_t changeAdminPin (uint8_t * password, uint8_t * new_password);

uint8_t unblockPin (uint8_t * new_pin);

uint8_t getPasswordRetryCount (void);

uint8_t getUserPasswordRetryCount (void);

uint32_t getRandomNumber (uint32_t Size_u32, uint8_t * Data_pu8);

uint8_t isAesSupported (void);

uint8_t sendAESMasterKey (int nLen, unsigned char* pcMasterKey);

uint8_t testScAesKey (int nLen, unsigned char* pcKey);

uint8_t testSendUserPW2 (unsigned char* pcPW);

unsigned int CcidReset (void);

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

#define CCID_MAX_PIN_LENGTH (255u-CCID_DATA)


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

void InitSCTStruct (typeSmartcardTransfer * _tSCT);
unsigned char GenerateCRC (unsigned char* pData, unsigned char cLength);
void GenerateTPDU (typeSmartcardTransfer * _tSCT);
void GenerateChainedTPDU (typeSmartcardTransfer * _tSCT);
unsigned short SendTPDU (typeSmartcardTransfer * _tSCT);
unsigned short SendAPDU (typeSmartcardTransfer * _tSCT);



unsigned short CcidGetData (unsigned char cP1, unsigned char cP2, unsigned char* nRetSize);
unsigned short CcidSelectOpenPGPApp (void);
unsigned short CcidChangePin (unsigned char cPinNr, const uint8_t* szPin, const uint8_t* szNewPin);
unsigned short CcidVerifyPin (unsigned char cPinNr, const uint8_t* szPin);
unsigned short CcidUnblockPin(uint8_t *new_pin);
unsigned short CcidDecipher (unsigned char* nRetSize);
unsigned short CcidGetChallenge (int nReceiveLength, unsigned char* nReceiveData);
unsigned short CcidPutAesKey (unsigned int cKeyLen, unsigned char* pcAES_Key);
int CcidAesDecSub (int nSendLength, unsigned char* cSendData, int nReceiveLength, unsigned char* cReceiveData);
int CcidAesDec (int nSendLength, unsigned char* cSendData, int nReceiveLength, unsigned char* cReceiveData);
uint8_t factoryReset (uint8_t * password);








#endif /* CCID_LOCAL_ACCESS */
