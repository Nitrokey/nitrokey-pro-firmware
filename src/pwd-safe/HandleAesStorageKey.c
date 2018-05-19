/*
 * Author: Copyright (C) Rudolf Boeddeker  Date: 15.06.2012
 *
 * This file is part of Nitrokey 2
 *
 * Nitrokey 2  is free software: you can redistribute it and/or modify
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
 * HandleAesStorageKey.c
 *
 *  Created on: 15.06.2012
 *      Author: RB
 */

// #include <stdio.h>
#include <stdlib.h>
#include <password_safe.h>
#include "string.h"
#include "CcidLocalAccess.h"
#include "report_protocol.h"
#include "smartcard.h"
#include "aes.h"
// #include "flashc.h"
#include "time.h"

// #include "global.h"
#include "delays.h"
// #include "Interpreter.h"
// #include "CCID/USART/ISO7816_ADPU.h"
// #include "CCID/Local_ACCESS/OpenPGP_V20.h"
#include "FlashStorage.h"
#include "HandleAesStorageKey.h"


/*******************************************************************************

 Local defines

*******************************************************************************/

#define LOCAL_DEBUG
// #define LOCAL_DEBUG_CHECK_KEY_GENERATION

#define AES_KEYSIZE_256_BIT     32  // 32 * 8 = 256


/*******************************************************************************

  BuildNewAesStorageKey_u32

  Build a new storage key for the SD card encryption

  Steps:

  - get random number for the storage key (SD card key)
  - encrypt the storage key
  - store the storage key in the user page

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u32 BuildNewAesStorageKey_u32 (u8 * MasterKey_pu8)
{
    // u32 Ret_u32;
u8 StorageKey_au8[AES_KEYSIZE_256_BIT];

u8 Buffer_au8[AES_KEYSIZE_256_BIT];

#ifdef LOCAL_DEBUG
    // CI_TickLocalPrintf ("BuildNewAesStorageKey\r\n");
#endif
    // Wait for next smartcard cmd
    DelayMs (10);

    // Get a random number for the storage key
    if (FALSE == getRandomNumber (AES_KEYSIZE_256_BIT / 2, StorageKey_au8))
    {
#ifdef LOCAL_DEBUG
        // CI_LocalPrintf ("GetRandomNumber fails\n\r");
#endif
        return (FALSE);
    }

    // Get a random number for the storage key
    if (FALSE == getRandomNumber (AES_KEYSIZE_256_BIT / 2, &StorageKey_au8[AES_KEYSIZE_256_BIT / 2]))
    {
#ifdef LOCAL_DEBUG
        // CI_LocalPrintf ("GetRandomNumber fails\n\r");
#endif
        return (FALSE);
    }

#ifdef LOCAL_DEBUG
    // CI_LocalPrintf ("Uncrypted storage key : ");
    // HexPrint (AES_KEYSIZE_256_BIT,StorageKey_au8);
    // CI_LocalPrintf ("\r\n");
#endif

    memcpy (Buffer_au8, StorageKey_au8, AES_KEYSIZE_256_BIT);

    // Local encryption of the storage key
unsigned char Buffer_au8_encrypted[AES_KEYSIZE_256_BIT];

aes_context aes_ctx;

    aes_setkey_enc (&aes_ctx, MasterKey_pu8, 256);
int i;

    for (i = 0; i < AES_KEYSIZE_256_BIT; i += 16)
    {
        aes_crypt_ecb (&aes_ctx, AES_ENCRYPT, &(Buffer_au8[i]), &(Buffer_au8_encrypted[i]));
    }
    // AES_StorageKeyEncryption (AES_KEYSIZE_256_BIT, Buffer_au8,
    // MasterKey_pu8, AES_PMODE_CIPHER);

#ifdef LOCAL_DEBUG
    // CI_LocalPrintf ("Local encrypted storage key : ");
    // HexPrint (AES_KEYSIZE_256_BIT,Buffer_au8_encrypted);
    // CI_LocalPrintf ("\r\n");
#endif

    // Store the encrypted storage key in USER PAGE
    WriteAESStorageKeyToUserPage (Buffer_au8_encrypted);

#ifdef LOCAL_DEBUG_CHECK_KEY_GENERATION
    // Test the storage key

    // Wait for next smartcard cmd
    DelayMs (10);


    /* Enable smartcard */
    if (FALSE == LA_OpenPGP_V20_Test_SendUserPW2 ((u8 *) "123456"))
    {
        return (FALSE);
    }

    // Wait for next smartcard cmd
    DelayMs (10);

    /* SC AES key decryption */
    if (FALSE == testScAesKey (AES_KEYSIZE_256_BIT, Buffer_au8))
    {
        return (FALSE);
    }

    // CI_LocalPrintf ("SC decrypted storage key : ");
    // HexPrint (AES_KEYSIZE_256_BIT,Buffer_au8);
    // CI_LocalPrintf ("\r\n");
#endif

    // Clear the critical memory
    memset (StorageKey_au8, 0, AES_KEYSIZE_256_BIT);
    memset (Buffer_au8, 0, AES_KEYSIZE_256_BIT);

    return (TRUE);

}


/*******************************************************************************

  BuildNewAesMasterKey_u32

  Build a new storage key for the SD card encryption

  Steps:

  - get random number for the master key
  - store master key on smartcard

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u32 BuildNewAesMasterKey_u32 (u8 * AdminPW_pu8, u8 * MasterKey_pu8)
{
    RestartSmartcard ();

    // Wait for next smartcard cmd
    DelayMs (10);

    // Get a random number for the master key
    if (FALSE == getRandomNumber (AES_KEYSIZE_256_BIT / 2, MasterKey_pu8))
    {
        return (FALSE);
    }

    // Get a random number for the master key
    if (FALSE == getRandomNumber (AES_KEYSIZE_256_BIT / 2, &MasterKey_pu8[AES_KEYSIZE_256_BIT / 2]))
    {
        return (FALSE);
    }

    // Wait for next smartcard cmd
    DelayMs (10);

    // Unlock smartcard for sending master key
    if (FALSE == cardAuthenticate (AdminPW_pu8))
    {
        return CMD_STATUS_WRONG_PASSWORD;
    }

    // Wait for next smartcard cmd
    DelayMs (10);

    // Store master key in smartcard
int ret = sendAESMasterKey (AES_KEYSIZE_256_BIT, MasterKey_pu8);

    if (TRUE != ret)
    {
        return CMD_STATUS_AES_CREATE_KEY_FAILED;
    }

    // ClearStickKeysNotInitatedToFlash ();
    return (CMD_STATUS_OK);
}


/*******************************************************************************

  BuildNewXorPattern_u32

  Build a xor pattern, with this pattern every key received from the smartcard is xored

  Changes
  Date      Author          Info
  20.05.14  RB              Implementation of build xor pattern

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u32 BuildNewXorPattern_u32 (void)
{
u8 XorPattern_au8[AES_KEYSIZE_256_BIT];

#ifdef LOCAL_DEBUG
    // CI_TickLocalPrintf ("BuildNewXorPattern_u32\r\n");
#endif

    RestartSmartcard ();
    // LA_RestartSmartcard_u8 ();


#ifdef LOCAL_DEBUG
    // CI_LocalPrintf ("GetRandomNumber\n\r");
#endif

    // Get a random number for the master key
    if (FALSE == getRandomNumber (AES_KEYSIZE_256_BIT / 2, XorPattern_au8))
    {
#ifdef LOCAL_DEBUG
        // CI_LocalPrintf ("GetRandomNumber fails\n\r");
#endif
        return (FALSE);
    }

    // Get a random number for the master key
    if (FALSE == getRandomNumber (AES_KEYSIZE_256_BIT / 2, &XorPattern_au8[AES_KEYSIZE_256_BIT / 2]))
    {
#ifdef LOCAL_DEBUG
        // CI_LocalPrintf ("GetRandomNumber fails\n\r");
#endif
        return (FALSE);
    }

    WriteXorPatternToFlash (XorPattern_au8);

    return (TRUE);
}

/*******************************************************************************

  XorAesKey_u32

  Changes
  Date      Author          Info
  20.05.14  RB              Implementation of xor a aes key

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u32 XorAesKey_u32 (u8 * AesKey_au8)
{
u8 XorKey_au8[AES_KEYSIZE_256_BIT];

u32 i;

    ReadXorPatternFromFlash (XorKey_au8);

#ifdef LOCAL_DEBUG
    // CI_LocalPrintf ("XorAesKey with : ");
    // HexPrint (AES_KEYSIZE_256_BIT,XorKey_au8);
    // CI_LocalPrintf ("\r\n");
#endif

    for (i = 0; i < AES_KEYSIZE_256_BIT; i++)
    {
        AesKey_au8[i] = AesKey_au8[i] ^ XorKey_au8[i];
    }

#ifdef LOCAL_DEBUG
    // CI_LocalPrintf ("New AesKey : ");
    // HexPrint (AES_KEYSIZE_256_BIT,AesKey_au8);
    // CI_LocalPrintf ("\r\n");
#endif
    return (TRUE);
}

/*******************************************************************************

  BuildStorageKeys_u32

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u32 BuildStorageKeys_u32 (u8 * AdminPW_pu8)
{
u32 Ret_u32;
u8 MasterKey_au8[AES_KEYSIZE_256_BIT];

    if (FALSE == cardAuthenticate (AdminPW_pu8))
    {
        return CMD_STATUS_WRONG_PASSWORD;
    }

    /* Ret_u32 = EraseLocalFlashKeyValues_u32 (); if (FALSE == Ret_u32) { return CMD_STATUS_AES_CREATE_KEY_FAILED; } */

    Ret_u32 = BuildNewXorPattern_u32 ();
    if (FALSE == Ret_u32)
    {
        return CMD_STATUS_AES_CREATE_KEY_FAILED;
    }

    Ret_u32 = BuildNewAesMasterKey_u32 (AdminPW_pu8, MasterKey_au8);
    if (TRUE != Ret_u32)
    {
        return CMD_STATUS_AES_CREATE_KEY_FAILED;
    }

    Ret_u32 = BuildNewAesStorageKey_u32 (MasterKey_au8);
    if (FALSE == Ret_u32)
    {
        return CMD_STATUS_AES_CREATE_KEY_FAILED;
    }

    // Clear the critical memory
    memset (MasterKey_au8, 0, AES_KEYSIZE_256_BIT);

    return CMD_STATUS_OK;
}


/*******************************************************************************

  DecryptKeyViaSmartcard_u32

  Changes
  Date      Reviewer        Info
  13.04.14  RB              Modification for multiple hidden volumes

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u32 DecryptKeyViaSmartcard_u32 (u8 * StorageKey_pu8)
{
#ifdef LOCAL_DEBUG
    // CI_LocalPrintf ("DecryptKeyViaSmartcard\r\n");
    // //HexPrint (AES_KEYSIZE_256_BIT,StorageKey_pu8);
    // //CI_LocalPrintf ("\r\n");
#endif

    /* SC AES key decryption */
    if (FALSE == testScAesKey (AES_KEYSIZE_256_BIT, StorageKey_pu8))
    {
#ifdef LOCAL_DEBUG
        // CI_LocalPrintf ("Smartcard access failed\r\n");
#endif
        return (FALSE);
    }

    XorAesKey_u32 (StorageKey_pu8);

#ifdef LOCAL_DEBUG
    // //CI_LocalPrintf ("DecryptKeyViaSmartcard_u32: Decrypted key : ");
    // //HexPrint (AES_KEYSIZE_256_BIT,StorageKey_pu8);
    // //CI_LocalPrintf ("\r\n");
#endif

    return (TRUE);
}

/*******************************************************************************

  GetStorageKey_u32

  Key for the normal crypted volume (not the hidden volumes)

  Changes
  Date      Reviewer        Info
  13.04.14  RB              Modification for multiple hidden volumes

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u32 GetStorageKey_u32 (u8 * UserPW_pu8, u8 * StorageKey_pu8)
{
    /* Enable smartcard */
    if (FALSE == testSendUserPW2 (UserPW_pu8))
    {
        return (FALSE);
    }

    // Wait for next smartcard cmd
    DelayMs (10);

    // Get encrypted key for the crypted volume
    ReadAESStorageKeyToUserPage (StorageKey_pu8);

    if (FALSE == DecryptKeyViaSmartcard_u32 (StorageKey_pu8))
    {
        return (FALSE);
    }

    return (TRUE);
}

/*******************************************************************************

  CheckStorageKey_u8

  Check Key for the normal encrypted volume (not the hidden volumes)

  Changes
  Date      Reviewer        Info
  05.05.14  RB              Creation

  Reviews
  Date      Reviewer        Info


*******************************************************************************/

u8 CheckStorageKey_u8 (void)
{
u8 StorageKey_au8[AES_KEYSIZE_256_BIT];

u32* p_pu32;

    // Get encrypted key for the crypted volume
    ReadAESStorageKeyToUserPage (StorageKey_au8);

    p_pu32 = (u32 *) & StorageKey_au8[0];
    if ((u32) 0xFFFFFFFF == *p_pu32)
    {
        return (FALSE); // No key generated - this is a security leak
    }

    return (TRUE);
}

/*******************************************************************************

  StartupCheck_u8

  Changes
  Date      Reviewer        Info
  05.05.14  RB              Creation

  Reviews
  Date      Reviewer        Info


*******************************************************************************/
#ifdef STORAGE
u8 StartupCheck_u8 (void)
{
u8 CheckStatus_u8 = TRUE;

    if (FALSE == PWS_CheckPasswordSafeKey_u8 ())
    {
        CheckStatus_u8 = FALSE;
    }

    /*
       if (TRUE == CheckStorageKey_u8 ()) { return (TRUE); // Keys ok } */

    if (TRUE == CheckStatus_u8)
    {
        return (TRUE);
    }

    // CI_LocalPrintf ("*** AES keys unsecure ***\r\n");

    ReadStickConfigurationFromUserPage ();

    if (TRUE == StickConfiguration_st.StickKeysNotInitiated_u8)
    {
        // CI_LocalPrintf ("*** Set flash bit NotInitated ***\r\n");
        SetStickKeysNotInitatedToFlash ();
    }

    return (FALSE);
}
#endif

/*******************************************************************************

  HighLevelTests

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/
#ifdef COMPILE_TESTS
void HighLevelTests (unsigned char nParamsGet_u8, unsigned char CMD_u8, unsigned int Param_u32, unsigned char* String_pu8)
{
    u8 Buffer_au8[32];

    if (0 == nParamsGet_u8)
    {
        // CI_LocalPrintf ("Highlevel test functions\r\n");
        // CI_LocalPrintf ("\r\n");
        // CI_LocalPrintf ("0 Build storage keys\r\n");
        // CI_LocalPrintf ("1 Print 32 byte of USER PAGE\r\n");
        // CI_LocalPrintf ("2 Write 12345678 to USER PAGE\r\n");
        // CI_LocalPrintf ("3 Write 987654321 to USER PAGE\r\n");
        // CI_LocalPrintf ("4 Clear AES storage key\r\n");
        // CI_LocalPrintf ("5 Print AES storage key\r\n");
        // CI_LocalPrintf ("6 Set new SD card found\r\n");
        // CI_LocalPrintf ("\r\n");
        return;
    }

    switch (CMD_u8)
    {
        case 0:
            BuildStorageKeys_u32 ((u8 *) "12345678");
            break;

        case 1:
            // CI_LocalPrintf ("Print USER PAGE : ");
            // HexPrint (32,(u8*)AVR32_FLASHC_USER_PAGE);
            // CI_LocalPrintf ("\r\n");
            break;

        case 2:
            // CI_LocalPrintf ("Write 12345678 to USER PAGE\r\n");
            // flashc_memcpy(AVR32_FLASHC_USER_PAGE,"12345678",8,TRUE);
            break;
        case 3:
            // CI_LocalPrintf ("Write 987654321 to USER PAGE\r\n");
            // flashc_memcpy(AVR32_FLASHC_USER_PAGE,"987654321",9,TRUE);
            break;
        case 4:
            // CI_LocalPrintf ("Clear AES storage key in flash\r\n");
            memset (Buffer_au8, 255, 32);
            // Store the encrypted storage key in USER PAGE
            WriteAESStorageKeyToUserPage (Buffer_au8);
            break;
        case 5:
            ReadAESStorageKeyToUserPage (Buffer_au8);
            // CI_LocalPrintf ("Print AES storage key : ");
            // HexPrint (32,(u8*)Buffer_au8);
            // CI_LocalPrintf ("\r\n");
            break;
        case 6:
            // CI_LocalPrintf ("Set new SD card found\r\n");
            SetSdCardNotFilledWithRandomCharsToFlash ();
            break;

        default:
            break;
    }
}
#endif
