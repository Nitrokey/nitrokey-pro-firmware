/*
* Author: Copyright (C) Rudolf Boeddeker  Date: 29.07.2014
*
* This file is part of GPF Crypto Stick 2
*
* GPF Crypto Stick 2  is free software: you can redistribute it and/or modify
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
 * password_safe.c
 *
 *  Created on: 29.07.2014
 *      Author: RB
 */



//#include "compiler.h"
//#include "preprocessor.h"
//#include "board.h"
//#include "gpio.h"
//#include "flashc.h"
//#include "string.h"
#include "aes.h"
//#include "stdio.h"
#include <string.h>
#include <stdlib.h>

//#include "global.h"
#include "tools.h"
//#include "TIME_MEASURING.h"

#include "CcidLocalAccess.h"
#include "smartcard.h"
#include "password_safe.h"
#include "report_protocol.h"

//#include "HiddenVolume.h"
//#include "CCID/USART/ISO7816_USART.h"
//#include "CCID/USART/ISO7816_ADPU.h"
//#include "CCID/USART/ISO7816_Prot_T1.h"
//#include "CCID/LOCAL_ACCESS/OpenPGP_V20.h"
//#include "USB_CCID/USB_CCID.h"
#include "FlashStorage.h"
#include "HandleAesStorageKey.h"
//#include "OTP/keyboard.h"
//#include "LED_test.h"

/*******************************************************************************

 Local defines

*******************************************************************************/

//#define ENABLE_IBN_PWS_TESTS
//#define ENABLE_IBN_PWS_TESTS_ENCRYPTION

#ifdef ENABLE_IBN_PWS_TESTS
  int CI_LocalPrintf (char *szFormat,...);
  int CI_TickLocalPrintf (char *szFormat,...);
#else
  #define CI_LocalPrintf(...)
  #define CI_TickLocalPrintf(...)
  #define CI_StringOut(...)
  #define CI_Print8BitValue(...)
  #define HexPrint(...)
#endif


/*******************************************************************************

 Global declarations

*******************************************************************************/

/*******************************************************************************

 External declarations

*******************************************************************************/

/*******************************************************************************

 Local declarations

*******************************************************************************/
#define AES_KEYSIZE_256_BIT     32        // 32 * 8 = 256

static u8 DecryptedPasswordSafeKey_u8 = FALSE;

#if (defined __GNUC__) && (defined __AVR32__)
  __attribute__((__aligned__(4)))
#elif (defined __ICCAVR32__)
  #pragma data_alignment = 4
#endif
static u8 DecryptedPasswordSafeKey_au8[AES_KEYSIZE_256_BIT];

/*
#if (defined __GNUC__) && (defined __AVR32__)
  __attribute__((__aligned__(4)))
#elif (defined __ICCAVR32__)
  #pragma data_alignment = 4
#endif
typePasswordSafe_st PasswordSafe_st;
*/
#if (defined __GNUC__) && (defined __AVR32__)
  __attribute__((__aligned__(4)))
#elif (defined __ICCAVR32__)
  #pragma data_alignment = 4
#endif
typePasswordSafeSlot_st PWS_BufferSlot_st;

/*******************************************************************************

  PWS_WriteSlot

  Changes
  Date      Reviewer        Info
  29.07.14  RB              Implementation of function

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_WriteSlot (u8 Slot_u8, typePasswordSafeSlot_st *Slot_st)
{
  u8 *WritePointer_pu8;
  u8 *AesKeyPointer_pu8;
  void *p;

  CI_LocalPrintf ("PWS_WriteSlot: Slot %d. Name -%s- Loginname -%s- PW -%s-\r\n",Slot_u8,Slot_st->SlotName_au8,Slot_st->SlotLoginName_au8,Slot_st->SlotPassword_au8);

  if (PWS_SLOT_COUNT <= Slot_u8)
  {
    CI_LocalPrintf ("PWS_WriteSlot: Wrong slot nr %d\r\n",Slot_u8);
    return (FALSE);
  }

  if (FALSE == PWS_GetDecryptedPasswordSafeKey(&AesKeyPointer_pu8))
  {
    CI_LocalPrintf ("PWS_WriteSlot: Key not decrypted\r\n");
    return (FALSE);
  }

  //LED_GreenOn ();

// Activate data in slot
  Slot_st->SlotActiv_u8 = PWS_SLOT_ACTIV_TOKEN;

#ifdef ENABLE_IBN_PWS_TESTS_ENCRYPTION
  CI_LocalPrintf ("PWS_WriteSlot decrypted  : ");
  HexPrint (PWS_SLOT_LENGTH, &Slot_st);
  CI_LocalPrintf ("\n\r");
#endif

// Encrypt data (max 256 byte per encryption)
  unsigned char Slot_st_encrypted[PWS_SLOT_LENGTH];
  aes_context aes_ctx;
  aes_setkey_enc (&aes_ctx, AesKeyPointer_pu8, 256);
  int i;
  for (i=0; i<PWS_SLOT_LENGTH; i+=16) {
      aes_crypt_ecb (&aes_ctx, AES_ENCRYPT, 
                    &(((unsigned char*)(Slot_st))[i]), 
                    &(Slot_st_encrypted[i]));
    }
  
  memcpy( Slot_st, Slot_st_encrypted, PWS_SLOT_LENGTH);

#ifdef ENABLE_IBN_PWS_TESTS_ENCRYPTION
  CI_LocalPrintf ("PWS_WriteSlot encrypted  : ");
  HexPrint (PWS_SLOT_LENGTH, &Slot_st_encrypted);
  CI_LocalPrintf ("\n\r");
#endif


// Get write address
  WritePointer_pu8 = (u8*)(PWS_FLASH_START_ADDRESS + PWS_SLOT_LENGTH * Slot_u8);


// Write to flash
  p = (void*)Slot_st_encrypted;
	FLASH_Unlock();
	write_data_to_flash( p, PWS_SLOT_LENGTH, WritePointer_pu8);
	FLASH_Lock();

  //LED_GreenOff ();

  return (TRUE);
}


/*******************************************************************************

  PWS_EraseSlot

  Changes
  Date      Reviewer        Info
  30.07.14  RB              Implementation of function

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_EraseSlot (u8 Slot_u8)
{
  u8 *WritePointer_pu8;
  u8 *AesKeyPointer_pu8;
  void *p;

#if (defined __GNUC__) && (defined __AVR32__)
  __attribute__((__aligned__(4)))
#elif (defined __ICCAVR32__)
  #pragma data_alignment = 4
#endif
  typePasswordSafeSlot_st Slot_st;

  CI_LocalPrintf ("PWS_EraseSlot: Slot %d\r\n",Slot_u8);

  if (PWS_SLOT_COUNT <= Slot_u8)
  {
    CI_LocalPrintf ("PWS_EraseSlot: Wrong slot nr %d\r\n",Slot_u8);
    return (FALSE);
  }

// Check for unlock
  if (FALSE == PWS_GetDecryptedPasswordSafeKey(&AesKeyPointer_pu8))
  {
    CI_LocalPrintf ("PWS_EraseSlot: user password not entered\r\n");
    return (FALSE);
  }

//  //LED_GreenOn ();

// Clear data in slot
  memset ((char*)&Slot_st,0,sizeof (Slot_st));
  Slot_st.SlotActiv_u8 = PWS_SLOT_INACTIV_TOKEN;

#ifdef ENABLE_IBN_PWS_TESTS_ENCRYPTION
  CI_LocalPrintf ("PWS_EraseSlot decrypted  : ");
  HexPrint (PWS_SLOT_LENGTH, &Slot_st);
  CI_LocalPrintf ("\n\r");
#endif

// Encrypt data (max 256 byte per encryption) 
  unsigned char Slot_st_encrypted[PWS_SLOT_LENGTH];
  aes_context aes_ctx;
  aes_setkey_enc (&aes_ctx, AesKeyPointer_pu8, 256);
  int i;
  for (i=0; i<PWS_SLOT_LENGTH; i+=16) {
      aes_crypt_ecb (&aes_ctx, AES_ENCRYPT,
                    &(((unsigned char*)(&Slot_st))[i]), 
                    &(Slot_st_encrypted[i]));
    }
  
#ifdef ENABLE_IBN_PWS_TESTS_ENCRYPTION
  CI_LocalPrintf ("PWS_EraseSlot encrypted  : ");
  HexPrint (PWS_SLOT_LENGTH, &Slot_st_encrypted);
  CI_LocalPrintf ("\n\r");
#endif

// Get write address
  WritePointer_pu8 = (u8*)(PWS_FLASH_START_ADDRESS + PWS_SLOT_LENGTH * Slot_u8);

// Write to flash
  p = (void*)&Slot_st_encrypted;
    FLASH_Unlock();
    write_data_to_flash (Slot_st_encrypted, PWS_SLOT_LENGTH, WritePointer_pu8);
    FLASH_Lock();

  //LED_GreenOff ();

  return (TRUE);
}


/*******************************************************************************

  PWS_ReadSlot

  Changes
  Date      Reviewer        Info
  29.07.14  RB              Implementation of function

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_ReadSlot (u8 Slot_u8, typePasswordSafeSlot_st *Slot_st)
{
  u8 *ReadPointer_pu8;
  u8 *AesKeyPointer_pu8;

  if (PWS_SLOT_COUNT <= Slot_u8)
  {
    CI_LocalPrintf ("PWS_ReadSlot: Wrong slot nr %d\r\n",Slot_u8);
    return (FALSE);
  }

  if (FALSE == PWS_GetDecryptedPasswordSafeKey(&AesKeyPointer_pu8))
  {
    CI_LocalPrintf ("PWS_ReadSlot: key not decrypted\r\n");
    return (FALSE);     // Aes key is not decrypted
  }

  //LED_GreenOn ();

// Get read address
  ReadPointer_pu8 = (u8*)(PWS_FLASH_START_ADDRESS + (PWS_SLOT_LENGTH * Slot_u8));
  memcpy (Slot_st, ReadPointer_pu8, PWS_SLOT_LENGTH);

/*
#ifdef ENABLE_IBN_PWS_TESTS_ENCRYPTION
  CI_LocalPrintf ("PWS_ReadSlot encrypted  : ");
  HexPrint (PWS_SLOT_LENGTH, Slot_st);
  CI_LocalPrintf ("\n\r");
#endif
*/

// Decrypt data (max 256 byte per encryption)
  unsigned char Slot_st_decrypted[PWS_SLOT_LENGTH];
  aes_context aes_ctx;
  aes_setkey_dec (&aes_ctx, AesKeyPointer_pu8, 256);
  // TODO: Create aes_crypt_ecb with length as parameter and break the input internally
  int i;
  for (i=0; i<PWS_SLOT_LENGTH; i+=16) {
      aes_crypt_ecb (&aes_ctx, AES_DECRYPT, 
                    &(((unsigned char*)(Slot_st))[i]), 
                    &(Slot_st_decrypted[i]));
    }
  
   memcpy( (unsigned char*)(Slot_st), Slot_st_decrypted, PWS_SLOT_LENGTH);

/*
#ifdef ENABLE_IBN_PWS_TESTS_ENCRYPTION
  CI_LocalPrintf ("PWS_ReadSlot decrypted  : ");
  HexPrint (PWS_SLOT_LENGTH, Slot_st_decrypted);
  CI_LocalPrintf ("\n\r");
#endif
*/
  //LED_GreenOff ();

  return (TRUE);
}

/*******************************************************************************

  PWS_GetAllSlotStatus

  Changes
  Date      Reviewer        Info
  30.07.14  RB              Implementation of function

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_GetAllSlotStatus (u8 *StatusArray_pu8)
{
  u32 i;
  u8 *AesKeyPointer_pu8;

#if (defined __GNUC__) && (defined __AVR32__)
  __attribute__((__aligned__(4)))
#elif (defined __ICCAVR32__)
  #pragma data_alignment = 4
#endif
  typePasswordSafeSlot_st Slot_st; 

// Clear the output array
  memset (StatusArray_pu8, 0, PWS_SLOT_COUNT);

// Check for user password enable
  if (FALSE == PWS_GetDecryptedPasswordSafeKey(&AesKeyPointer_pu8))
  {
    CI_LocalPrintf ("PWS_ReadSlot: key not decrypted\r\n");
    return (FALSE);     // Aes key is not decrypted
  }

  for (i=0; i<PWS_SLOT_COUNT; i++)
  {
    if (TRUE == PWS_ReadSlot (i, &Slot_st))
    {
      if (PWS_SLOT_ACTIV_TOKEN == Slot_st.SlotActiv_u8)
      {
        StatusArray_pu8[i] = TRUE;
      }
    }
  }

  CI_LocalPrintf ("PWS_ReadSlot: Slot status : ");
  for (i=0;i<PWS_SLOT_COUNT;i++)
  {
    if (TRUE == StatusArray_pu8[i])
    {
      CI_LocalPrintf ("1");
    }
    else
    {
      CI_LocalPrintf ("0");
    }
  }
  CI_LocalPrintf ("\r\n");

  return (TRUE);
}

/*******************************************************************************

  PWS_GetSlotName

  Changes
  Date      Reviewer        Info
  30.07.14  RB              Implementation of function

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_GetSlotName (u8 Slot_u8, u8 *Name_pu8)
{
#if (defined __GNUC__) && (defined __AVR32__)
  __attribute__((__aligned__(4)))
#elif (defined __ICCAVR32__)
  #pragma data_alignment = 4
#endif
  typePasswordSafeSlot_st Slot_st;

  CI_LocalPrintf ("PWS_GetSlotName: Slot %d\r\n",Slot_u8);

// Clear the output arry
  memset (Name_pu8,0,PWS_SLOTNAME_LENGTH);

  if (FALSE == PWS_ReadSlot (Slot_u8, &Slot_st))
  {
    return (FALSE);
  }

  memcpy (Name_pu8,Slot_st.SlotName_au8,PWS_SLOTNAME_LENGTH);

  return (TRUE);
}

/*******************************************************************************

  PWS_GetSlotPassword

  Changes
  Date      Reviewer        Info
  30.07.14  RB              Implementation of function

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_GetSlotPassword (u8 Slot_u8, u8 *Password_pu8)
{
#if (defined __GNUC__) && (defined __AVR32__)
  __attribute__((__aligned__(4)))
#elif (defined __ICCAVR32__)
  #pragma data_alignment = 4
#endif
  typePasswordSafeSlot_st Slot_st;

  CI_LocalPrintf ("PWS_GetSlotPassword: Slot %d\r\n",Slot_u8);

// Clear the output array
  memset (Password_pu8,0,PWS_SLOTNAME_LENGTH);

  if (FALSE == PWS_ReadSlot (Slot_u8, &Slot_st))
  {
    return (FALSE);
  }

  memcpy (Password_pu8,Slot_st.SlotPassword_au8,PWS_PASSWORD_LENGTH);

  return (TRUE);
}

/*******************************************************************************

  PWS_GetSlotLoginName

  Changes
  Date      Reviewer        Info
  30.07.14  RB              Implementation of function

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_GetSlotLoginName (u8 Slot_u8, u8 *Loginname_pu8)
{
#if (defined __GNUC__) && (defined __AVR32__)
  __attribute__((__aligned__(4)))
#elif (defined __ICCAVR32__)
  #pragma data_alignment = 4
#endif
  typePasswordSafeSlot_st Slot_st;

  CI_LocalPrintf ("PWS_GetSlotLoginName: Slot %d\r\n",Slot_u8);

// Clear the output array
  memset (Loginname_pu8,0,PWS_LOGINNAME_LENGTH);

  if (FALSE == PWS_ReadSlot (Slot_u8, &Slot_st))
  {
    return (FALSE);
  }

  memcpy (Loginname_pu8,Slot_st.SlotLoginName_au8,PWS_LOGINNAME_LENGTH);

  return (TRUE);
}

/*******************************************************************************

  PWS_WriteSlotData_1

  Changes
  Date      Reviewer        Info
  30.07.14  RB              Implementation of function

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_WriteSlotData_1 (u8 Slot_u8,u8 *Name_pu8, u8 *Password_pu8)
{
// Clear the output array
  memset (&PWS_BufferSlot_st,0,sizeof (PWS_BufferSlot_st));

  memcpy (PWS_BufferSlot_st.SlotName_au8, Name_pu8, PWS_SLOTNAME_LENGTH);
  memcpy (PWS_BufferSlot_st.SlotPassword_au8, Password_pu8, PWS_PASSWORD_LENGTH);
  memcpy (PWS_BufferSlot_st.SlotName_au8, Name_pu8, PWS_LOGINNAME_LENGTH);

  return (TRUE);
}

/*******************************************************************************

  PWS_WriteSlotData_2

  Changes
  Date      Reviewer        Info
  30.07.14  RB              Implementation of function

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_WriteSlotData_2 (u8 Slot_u8,u8 *Loginname_pu8)
{

  memcpy (PWS_BufferSlot_st.SlotLoginName_au8, Loginname_pu8, PWS_LOGINNAME_LENGTH);

  if (FALSE == PWS_WriteSlot (Slot_u8,&PWS_BufferSlot_st))
  {
    //LED_GreenOff ();
    return (FALSE);
  }

  return (TRUE);
}


/*******************************************************************************

  BuildPasswordSafeKey_u32

  Changes
  Date      Author          Info
  01.08.14  RB              Frist Implementation

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u32 BuildPasswordSafeKey_u32 (void)
{
  u8 Key_au8[AES_KEYSIZE_256_BIT];

  CI_TickLocalPrintf ("BuildPasswordSafeKey_u32\r\n");
  RestartSmartcard ();
  //LA_RestartSmartcard_u8 ();

// Get a random number for the master key
  if (FALSE == getRandomNumber (AES_KEYSIZE_256_BIT/2,Key_au8))
  {
    CI_LocalPrintf ("GetRandomNumber fails 1\n\r");
    return (FALSE);
  }

// Get a random number for the master key
  if (FALSE == getRandomNumber (AES_KEYSIZE_256_BIT/2,&Key_au8[AES_KEYSIZE_256_BIT/2]))
  {
    CI_LocalPrintf ("GetRandomNumber fails 2\n\r");
    return (FALSE);
  }

  WritePasswordSafeKey (Key_au8);

// Old Key is invalid
   DecryptedPasswordSafeKey_u8 = FALSE;

  return (TRUE);
}


/*******************************************************************************

  DecryptedPasswordSafeKey

  Changes
  Date      Reviewer        Info
  01.08.14  RB              Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_DecryptedPasswordSafeKey (void)
{
  if (TRUE == DecryptedPasswordSafeKey_u8)
  {
    return (TRUE);
  }

  CI_LocalPrintf ("Decrypt password safe key\r\n");

// Get the encrypted hidden volume slots key
  ReadPasswordSafeKey (DecryptedPasswordSafeKey_au8);

// Decrypt the slots key of the hidden volumes

  if (FALSE == DecryptKeyViaSmartcard_u32 (DecryptedPasswordSafeKey_au8))
  {
    return (FALSE);
  }

// Key is ready
  DecryptedPasswordSafeKey_u8 = TRUE;

  return (TRUE);
}


/*******************************************************************************

  PWS_EnableAccess

  Changes
  Date      Reviewer        Info
  01.08.14  RB              Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_EnableAccess (u8 *password)
{
  unsigned short ret;

  CI_LocalPrintf ("PWS_EnableAccess: ");

  ret = CcidVerifyPin (2,(unsigned char *)password); // 2 = user pw
  if (APDU_ANSWER_COMMAND_CORRECT != ret)
  {
    CI_LocalPrintf (" *** FAIL ***\r\n");
    return CMD_STATUS_WRONG_PASSWORD;
  }

  ret = PWS_DecryptedPasswordSafeKey ();

  if (TRUE != ret)
  {
    CI_LocalPrintf (" *** FAIL ***. Can't decrypt key\r\n");
    return CMD_STATUS_AES_DEC_FAILED;
  }

  CI_LocalPrintf ("OK\r\n");

  return CMD_STATUS_OK;
}


/*******************************************************************************

  PWS_EnableAccess

  Changes
  Date      Reviewer        Info
  01.08.14  RB              Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_InitKey (void)
{
  u32 Ret_u32;

  CI_LocalPrintf ("PWS_InitKey\r\n");

  Ret_u32 = PWS_DecryptedPasswordSafeKey ();
  if (TRUE != Ret_u32)
  {
    CI_LocalPrintf ("PWS_InitKey: *** FAIL ***\r\n");
    return (FALSE);
  }

  return (TRUE);
}


/*******************************************************************************

  PWS_GetDecryptedPasswordSafeKey

  Changes
  Date      Reviewer        Info
  01.08.14  RB              Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_GetDecryptedPasswordSafeKey (u8 **Key_pu8)
{
  *Key_pu8 = NULL;

  if (FALSE == DecryptedPasswordSafeKey_u8)
  {
    return (FALSE);
  }

  *Key_pu8 = DecryptedPasswordSafeKey_au8;

  return (TRUE);
}

/*******************************************************************************

  PWS_SendData

  Changes
  Date      Reviewer        Info
  01.08.14  RB              Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 PWS_SendData (u8 Slot_u8,u8 Kind_u8)
{
  u8  SendString_au8[40];
  u32 Ret_u32;

  //LED_GreenOn ();

  switch (Kind_u8)
  {
    case PWS_SEND_PASSWORD :
            Ret_u32 = PWS_GetSlotPassword (Slot_u8, SendString_au8);
            if (FALSE == Ret_u32)
            {
              return (FALSE);
            }
            sendString ((char*)SendString_au8,strlen ((char*)SendString_au8));
            break;
    case PWS_SEND_LOGINNAME :
            Ret_u32 = PWS_GetSlotLoginName (Slot_u8, SendString_au8);
            if (FALSE == Ret_u32)
            {
              return (FALSE);
            }
            sendString ((char*)SendString_au8,strlen ((char*)SendString_au8));
            break;
    case PWS_SEND_TAB :
            sendTab ();
            break;
    case PWS_SEND_CR :
            sendEnter ();
            break;

  }

  //LED_GreenOff ();

  return (TRUE);
}

/*******************************************************************************

  IBN_PWS_Tests

  Changes
  Date      Author          Info
  29.07.14  RB              Creation

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

#ifdef ENABLE_IBN_PWS_TESTS

#if (defined __GNUC__) && (defined __AVR32__)
  __attribute__((__aligned__(4)))
#elif (defined __ICCAVR32__)
  #pragma data_alignment = 4
#endif
typePasswordSafeSlot_st PWS_TestSlot_st;

void IBN_PWS_Tests (unsigned char nParamsGet_u8,unsigned char CMD_u8,unsigned int Param_u32,unsigned char *String_pu8)
{
  u32 Ret_u32;

  if (0 == nParamsGet_u8)
  {
    CI_LocalPrintf ("Password safe test functions\r\n");
    CI_LocalPrintf ("\r\n");
    CI_LocalPrintf ("0 [slot] Init test slot [slot]\r\n");
    CI_LocalPrintf ("1 [slot] Read test slot\r\n");
    CI_LocalPrintf ("2        Get password safe key\r\n");
    CI_LocalPrintf ("3        Enable password safe access (PIN = 123456)\r\n");
    CI_LocalPrintf ("4        Build new key\r\n");
    CI_LocalPrintf ("5 [slot] Send password\r\n");
    CI_LocalPrintf ("\r\n");
    return;
  }

  switch (CMD_u8)
  {
    case 0:
      CI_LocalPrintf ("Init write test slot %d\r\n",Param_u32);
      sprintf ((char*)PWS_TestSlot_st.SlotName_au8,"Slot %d",Param_u32);
      sprintf ((char*)PWS_TestSlot_st.SlotLoginName_au8,"login name %d",Param_u32);
      sprintf ((char*)PWS_TestSlot_st.SlotPassword_au8,"password slot %d",Param_u32);
      PWS_TestSlot_st.SlotActiv_u8 = PWS_SLOT_ACTIV_TOKEN;

//      PWS_WriteSlot (0,"aaaa","bbbb");
      PWS_WriteSlot (Param_u32,&PWS_TestSlot_st);
//      CI_LocalPrintf ("Init write test slot done\r\n");
      break;

    case 1:
      CI_LocalPrintf ("Read slot %d\r\n",Param_u32);
      Ret_u32 = PWS_ReadSlot (Param_u32, &PWS_TestSlot_st);

      if (PWS_SLOT_ACTIV_TOKEN == PWS_TestSlot_st.SlotActiv_u8)
      {
        CI_LocalPrintf ("Slotname  : %s\r\n",PWS_TestSlot_st.SlotName_au8);
        CI_LocalPrintf ("Loginname : %s\r\n",PWS_TestSlot_st.SlotLoginName_au8);
        CI_LocalPrintf ("Password  : %s\r\n",PWS_TestSlot_st.SlotPassword_au8);
      }
      else
      {
        CI_LocalPrintf ("Slot not active : %d\r\n",PWS_TestSlot_st.SlotActiv_u8);
      }
      break;

    case 2 :
      if (FALSE == PWS_DecryptedPasswordSafeKey ())
      {
        CI_LocalPrintf ("Get password safe failed. User//admin password entered ?\r\n");
      }
      break;

    case 3 :
      CI_LocalPrintf ("Enable password safe PIN %s\r\n","123456");
      PWS_EnableAccess ((u8*)"123456");
      break;

    case 4 :
      BuildPasswordSafeKey_u32 ();
      break;

    case 5 :
      PWS_SendData (Param_u32,PWS_SEND_PASSWORD);
      break;
  }
}
#endif


