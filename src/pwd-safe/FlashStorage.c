/*
 * Author: Copyright (C) Rudolf Boeddeker  Date: 2013-07-12
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
 * FlashStorage.c
 *
 *  Created on: 12.07.2013
 *      Author: RB
 */

#include <stdlib.h>
#include <stddef.h>
#include "string.h"

#include "delays.h"
#include "FlashStorage.h"
#include "password_safe.h"
#include "hotp.h"

typeStick20Configuration_st StickConfiguration_st;


/*******************************************************************************

 Local defines

*******************************************************************************/

unsigned int debug_len = 0;

#define FLASHC_USER_PAGE 0x801dc00

/*

   Userpage layout PAGE: 0x801DC00

   Byte 
   0 - 31     AES Storage key 
   32 - 51    Matrix columns for user password 
   52 - 71    Matrix columns for admin password 
   72 - 101   Stick Configuration 
   102 - 133  Base for AES key hidden volume (32 byte) 
   134 - 137  ID of sd card (4 byte) 
   138 - 141  Last stored real timestamp (4 byte)
   142 - 145  ID of sc card (4 byte) 
   146 - 177  XOR mask for sc tranfered keys (32 byte) 
   178 - 209  Password safe key (32 byte) 
   210 -      Debug */

#ifdef ADD_DEBUG_COMMANDS

void WriteDebug (u8 * data, unsigned int length)
{
    unsigned char page_buffer[FLASH_PAGE_SIZE];

    memcpy (page_buffer, FLASHC_USER_PAGE, FLASH_PAGE_SIZE);
    memcpy (page_buffer + 210, data, length);

    debug_len += length;

    FLASH_Unlock ();
    FLASH_ErasePage (FLASHC_USER_PAGE);
    write_data_to_flash (page_buffer, FLASH_PAGE_SIZE, FLASHC_USER_PAGE);
    FLASH_Lock ();
}


void GetDebug (u8 * data, unsigned int* length)
{
    unsigned char page_buffer[FLASH_PAGE_SIZE];

    memcpy (page_buffer, FLASHC_USER_PAGE, FLASH_PAGE_SIZE);
    memcpy (data, page_buffer + 210, debug_len);
    *length = debug_len;
    debug_len = 0;
}

#endif

/*******************************************************************************

  WriteAESStorageKeyToUserPage

  Reviews
  Date      Reviewer        Info
  03.02.14  RB              First review

*******************************************************************************/

u8 WriteAESStorageKeyToUserPage (u8 * data)
{
    // flashc_memcpy(FLASHC_USER_PAGE,data,32,TRUE);
unsigned char page_buffer[FLASH_PAGE_SIZE];

    memcpy (page_buffer, (const void *) FLASHC_USER_PAGE, FLASH_PAGE_SIZE);
    memcpy (page_buffer, data, 32);

    FLASH_Unlock ();
    FLASH_ErasePage (FLASHC_USER_PAGE);
    write_data_to_flash (page_buffer, FLASH_PAGE_SIZE, FLASHC_USER_PAGE);
    FLASH_Lock ();

    return (TRUE);
}

/*******************************************************************************

  ReadAESStorageKeyToUserPage

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 ReadAESStorageKeyToUserPage (u8 * data)
{
    memcpy (data, (void *) (FLASHC_USER_PAGE), 32);
    return (TRUE);
}


/*******************************************************************************

  WriteMatrixColumsUserPWToUserPage

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/
/*

   u8 WriteMatrixColumsUserPWToUserPage (u8 *data) { flashc_memcpy(FLASHC_USER_PAGE + 32,data,20,TRUE); return (TRUE); }

 */
/*******************************************************************************

  ReadMatrixColumsUserPWFromUserPage

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/
/*

   u8 ReadMatrixColumsUserPWFromUserPage (u8 *data) { memcpy (data,(void*)(FLASHC_USER_PAGE + 32),20); return (TRUE); }

 */
/*******************************************************************************

  WriteMatrixColumsAdminPWFromUserPage

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/
/*

   u8 WriteMatrixColumsAdminPWFromUserPage (u8 *data) { flashc_memcpy(FLASHC_USER_PAGE + 52,data,20,TRUE); return (TRUE); }

 */
/*******************************************************************************

  ReadMatrixColumsAdminPWFromUserPage

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/
/*

   u8 ReadMatrixColumsAdminPWFromUserPage (u8 *data) { memcpy (data,(void*)(FLASHC_USER_PAGE + 52),20); return (TRUE); }

 */
/*******************************************************************************

  WriteStickConfigurationToUserPage

  Changes
  Date      Author          Info
  03.02.14  RB              Change to struct

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 WriteStickConfigurationToUserPage (void)
{
    // Set actual firmware version
    // StickConfiguration_st.VersionInfo_au8[0] = VERSION_MAJOR;
    // StickConfiguration_st.VersionInfo_au8[1] = VERSION_MINOR;
    StickConfiguration_st.VersionInfo_au8[2] = 0;   // Build number not used
    StickConfiguration_st.VersionInfo_au8[3] = 0;   // Build number not used

    // flashc_memcpy(FLASHC_USER_PAGE + 72,&StickConfiguration_st,30,TRUE);

uint8_t page_buffer[FLASH_PAGE_SIZE];

    memcpy (page_buffer, (const void *) FLASHC_USER_PAGE, FLASH_PAGE_SIZE);
    memcpy (page_buffer + 72, (u8 *) & StickConfiguration_st, 28);

    FLASH_Unlock ();
    FLASH_ErasePage (FLASHC_USER_PAGE);
    write_data_to_flash (page_buffer, FLASH_PAGE_SIZE, FLASHC_USER_PAGE);
    FLASH_Lock ();

    return (TRUE);
}

/*******************************************************************************

  ReadStickConfigurationFromUserPage

  Changes
  Date      Author          Info
  03.02.14  RB              Change to struct

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

u8 ReadStickConfigurationFromUserPage (void)
{
u8 UserPwRetryCount;

u8 AdminPwRetryCount;

u32 ActiveSmartCardID_u32;

    // Save dynamic data
    UserPwRetryCount = StickConfiguration_st.UserPwRetryCount;
    AdminPwRetryCount = StickConfiguration_st.AdminPwRetryCount;
    ActiveSmartCardID_u32 = StickConfiguration_st.ActiveSmartCardID_u32;

    // memcpy (&StickConfiguration_st,(void*)(FLASHC_USER_PAGE + 72),sizeof
    // (typeStick20Configuration_st));

    // Write actual version info
    // StickConfiguration_st.VersionInfo_au8[0] = VERSION_MAJOR;
    // StickConfiguration_st.VersionInfo_au8[1] = VERSION_MINOR;
    StickConfiguration_st.VersionInfo_au8[2] = 0;   // Build number not used
    StickConfiguration_st.VersionInfo_au8[3] = 0;   // Build number not used

    // Restore dynamic data
    StickConfiguration_st.UserPwRetryCount = UserPwRetryCount;
    StickConfiguration_st.AdminPwRetryCount = AdminPwRetryCount;
    StickConfiguration_st.ActiveSmartCardID_u32 = ActiveSmartCardID_u32;

    if (MAGIC_NUMBER_STICK20_CONFIG != StickConfiguration_st.MagicNumber_StickConfig_u16)
    {
        return (FALSE);
    }
    return (TRUE);
}

/*******************************************************************************

  InitStickConfigurationToUserPage_u8

  Changes
  Date      Author          Info
  03.02.14  RB              Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 InitStickConfigurationToUserPage_u8 (void)
{
    StickConfiguration_st.MagicNumber_StickConfig_u16 = MAGIC_NUMBER_STICK20_CONFIG;
    StickConfiguration_st.ReadWriteFlagUncryptedVolume_u8 = READ_WRITE_ACTIVE;
    StickConfiguration_st.ReadWriteFlagCryptedVolume_u8 = READ_WRITE_ACTIVE;
    StickConfiguration_st.ReadWriteFlagHiddenVolume_u8 = READ_WRITE_ACTIVE;
    StickConfiguration_st.FirmwareLocked_u8 = 0;
    StickConfiguration_st.ActiveSD_CardID_u32 = 0;  // todo: check endian
    // StickConfiguration_st.VersionInfo_au8[0] = VERSION_MAJOR;
    // StickConfiguration_st.VersionInfo_au8[1] = VERSION_MINOR;
    StickConfiguration_st.VersionInfo_au8[2] = 0;   // Build number not used
    StickConfiguration_st.VersionInfo_au8[3] = 0;   // Build number not used
    StickConfiguration_st.NewSDCardFound_u8 = 0;
    StickConfiguration_st.SDFillWithRandomChars_u8 = FALSE;
    StickConfiguration_st.VolumeActiceFlag_u8 = 0;
    StickConfiguration_st.NewSmartCardFound_u8 = 0;
    StickConfiguration_st.ActiveSmartCardID_u32 = 0;
    StickConfiguration_st.StickKeysNotInitiated_u8 = TRUE;

    WriteStickConfigurationToUserPage ();
    return (TRUE);
}

/*******************************************************************************

  WriteHiddenVolumeSlotKey

  Stores the encrypted hidden volume slot key

  Byte
  Len = 32 Byte

  Changes
  Date      Author          Info
  14.04.14  RB              Renaming

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/
/*

   u8 WriteHiddenVolumeSlotKey (u8 *data) { flashc_memcpy((void*)(FLASHC_USER_PAGE + 102),data,32,TRUE); return (TRUE); }

 */
/*******************************************************************************

  ReadHiddenVolumeSlotsKey

  Read the encrypted hidden volume slot key

  Changes
  Date      Author          Info
  14.04.14  RB              Renaming


  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/
/*

   u8 ReadHiddenVolumeSlotsKey (u8 *data) { memcpy (data,(void*)(FLASHC_USER_PAGE + 102),32); return (TRUE); }




 */
/*******************************************************************************

  SendStickStatusToHID

  Changes
  Date      Author          Info
  08.02.14  RB              Implementation of sending volume status

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/
/*

   void SendStickStatusToHID (typeStick20Configuration_st *Status_st) { // If configuration not found then init it if (FALSE ==
   ReadStickConfigurationFromUserPage ()) { InitStickConfigurationToUserPage_u8 (); }

   memcpy (Status_st,&StickConfiguration_st,sizeof (typeStick20Configuration_st)); // Not the retry counter and sc serial no

   // Set the actual volume status Status_st->VolumeActiceFlag_u8 = 0;

   if (TRUE == GetSdUncryptedCardEnableState ()) { Status_st->VolumeActiceFlag_u8 |= (1 << SD_UNCRYPTED_VOLUME_BIT_PLACE); }

   // Only 1 cypted volume could be active if (TRUE == GetSdEncryptedCardEnableState ()) { if (TRUE == GetSdEncryptedHiddenState ()) {
   Status_st->VolumeActiceFlag_u8 |= (1 << SD_HIDDEN_VOLUME_BIT_PLACE); } else { Status_st->VolumeActiceFlag_u8 |= (1 <<
   SD_CRYPTED_VOLUME_BIT_PLACE); } }

   ReadSdId (&Status_st->ActiveSD_CardID_u32);

   Status_st->FirmwareLocked_u8 = FALSE; if (TRUE == flashc_is_security_bit_active()) { Status_st->FirmwareLocked_u8 = TRUE; } }

 */
/*******************************************************************************

  GetStickStatusFromHID

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/
/*

                                                                                                                                                                                                                                                                                                                 void GetStickStatusFromHID (typeStick20Configuration_st *Status_st) { *//*
                                                                                                                                                                                                                                                                                                                 HID_Stick20AccessStatus_st->MatrixPasswordUserActiv_u8
                                                                                                                                                                                                                                                                                                                 = FALSE;
                                                                                                                                                                                                                                                                                                                 HID_Stick20AccexPassworixPassworixPasswordAdminActiv_u8
                                                                                                                                                                                                                                                                                                                 = FALSE;
                                                                                                                                                                                                                                                                                                                 HID_Stick20AccesasswordvPasswordvPasswordStatus_u8 =
                                                                                                                                                                                                                                                                                                                 FALSE; HID_Stick20AccessStatus_st->VolumeStatus_u8 =
                                                                                                                                                                                                                                                                                                                 FALSE;

                                                                                                                                                                           sd_mmc_mci_read_capacity_0(&HID_Stick20AccessStatus_st->SD_BlockSize_u32); *//*
                                                                                                                                                                           }

                                                                                                                                                                         */
/*******************************************************************************

  Read_ReadWriteStatusUncryptedVolume_u8

  Changes
  Date      Author          Info
  03.02.14  RB              Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 Read_ReadWriteStatusUncryptedVolume_u8 (void) { // If configuration not found then init it if (FALSE == ReadStickConfigurationFromUserPage ())
   { InitStickConfigurationToUserPage_u8 (); } return (StickConfiguration_st.ReadWriteFlagUncryptedVolume_u8); }

 */
/*******************************************************************************

  Write_ReadWriteStatusUncryptedVolume_u8

  Changes
  Date      Author          Info
  03.02.14  RB              Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 Write_ReadWriteStatusUncryptedVolume_u8 (u8 NewStatus_u8) { // If configuration not found then init it if (FALSE ==
   ReadStickConfigurationFromUserPage ()) { InitStickConfigurationToUserPage_u8 (); }

   StickConfiguration_st.ReadWriteFlagUncryptedVolume_u8 = NewStatus_u8;

   WriteStickConfigurationToUserPage ();

   return (TRUE); }

 */
/*******************************************************************************

  WriteSdId

  Changes
  Date      Author          Info
  08.02.14  RB              Implementation of save SD id in CPU flash

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 WriteSdId (u32 *SdId_u32) { flashc_memcpy(FLASHC_USER_PAGE + 134,SdId_u32,4,TRUE);

   StickConfiguration_st.ActiveSD_CardID_u32 = *SdId_u32; return (TRUE); }

 */
/*******************************************************************************

  ReadSdId

  Changes
  Date      Author          Info
  08.02.14  RB              Implementation of save SD id in CPU flash

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 ReadSdId (u32 *SdId_u32) { memcpy (SdId_u32,(void*)(FLASHC_USER_PAGE + 134),4);

   StickConfiguration_st.ActiveSD_CardID_u32 = *SdId_u32; return (TRUE); }



 */
/*******************************************************************************

  WriteNewSdCardFoundToFlash

  NewSDCardFound_u8
  Bit 0 = 0   New SD card found
  Bit 0 = 1   Previous SD card found


  SDFillWithRandomChars_u8
  Bit 0 = 0   SD card is *** not *** filled with random chars
  Bit 0 = 1   SD card is filled with random chars

  Changes
  Date      Author          Info
  08.02.14  RB              Implementation of save new SD card found

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 WriteNewSdCardFoundToFlash (u32 *SdId_u32) { // If configuration not found then init it if (FALSE == ReadStickConfigurationFromUserPage ()) {
   InitStickConfigurationToUserPage_u8 (); }

   // CI_LocalPrintf ("*** New SD card found *** Serial number 0x%08x\r\n",*SdId_u32);

   WriteSdId (SdId_u32);

   StickConfiguration_st.NewSDCardFound_u8 |= 0x01; // Bit 0 = new card found // StickConfiguration_st.NewSDCardFound_u8 += 2; // add change counter
   +1

   StickConfiguration_st.SDFillWithRandomChars_u8 &= 0xFE; // Clear the "card with random chars filled" bit

   WriteStickConfigurationToUserPage ();

   return (TRUE); }

 */
/*******************************************************************************

  SetSdCardFilledWithRandomsToFlash

  Changes
  Date      Author          Info
  10.02.14  RB              Implementation: New SD card found

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 SetSdCardFilledWithRandomsToFlash (void) { // If configuration not found then init it if (FALSE == ReadStickConfigurationFromUserPage ()) {
   InitStickConfigurationToUserPage_u8 (); }

   // CI_LocalPrintf ("SD is filled with random chars\r\n");

   StickConfiguration_st.SDFillWithRandomChars_u8 |= 0x01; // Set the "SD card filled with randoms" bit //
   StickConfiguration_st.SDFillWithRandomChars_u8 += 2; // add counter +1

   WriteStickConfigurationToUserPage ();

   return (TRUE); }

 */
/*******************************************************************************

  ClearNewSdCardFoundToFlash

  Changes
  Date      Author          Info
  08.02.14  RB              Implementation of clear new SD card found

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 ClearNewSdCardFoundToFlash (void) { // If configuration not found then init it if (FALSE == ReadStickConfigurationFromUserPage ()) {
   InitStickConfigurationToUserPage_u8 (); }

   // CI_LocalPrintf ("Clear new SD card found\r\n");

   StickConfiguration_st.NewSDCardFound_u8 &= 0xFE; // Clear the "new SD card found" bit

   WriteStickConfigurationToUserPage ();

   return (TRUE); }

 */
/*******************************************************************************

  SetSdCardFilledWithRandomCharsToFlash

  Changes
  Date      Author          Info
  06.05.14  RB              Implementation of clear new SD card found

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 SetSdCardFilledWithRandomCharsToFlash (void) { // If configuration not found then init it if (FALSE == ReadStickConfigurationFromUserPage ()) {
   InitStickConfigurationToUserPage_u8 (); }

   // CI_LocalPrintf ("Set new SD card filled with random chars\r\n");

   StickConfiguration_st.SDFillWithRandomChars_u8 |= 0x01; // Set the "SD card filled with randoms" bit

   WriteStickConfigurationToUserPage ();

   return (TRUE); }

 */
/*******************************************************************************

  SetSdCardNotFilledWithRandomCharsToFlash

  Bit 0 = 0   SD card is *** not *** filled with random chars
  Bit 0 = 1   SD card is filled with random chars

  Changes
  Date      Author          Info
  06.05.14  RB              Implementation of clear new SD card found

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
#ifdef STORAGE
u8 SetSdCardNotFilledWithRandomCharsToFlash (void)
{
    // If configuration not found then init it
    if (FALSE == ReadStickConfigurationFromUserPage ())
    {
        InitStickConfigurationToUserPage_u8 ();
    }

    // CI_LocalPrintf ("Set new SD card *** not *** filled with random
    // chars\r\n");

    StickConfiguration_st.SDFillWithRandomChars_u8 &= 0xFE; // Clear the
    // "card with
    // random chars
    // filled" bit

    WriteStickConfigurationToUserPage ();

    return (TRUE);
}
#endif

/*******************************************************************************

  SetStickKeysNotInitatedToFlash

  Changes
  Date      Author          Info
  05.05.14  RB              Implementation of clear new SD card found

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 SetStickKeysNotInitatedToFlash (void)
{
    // If configuration not found then init it
    if (FALSE == ReadStickConfigurationFromUserPage ())
    {
        InitStickConfigurationToUserPage_u8 ();
    }

    // CI_LocalPrintf ("Set stick keys not initiated\r\n");

    StickConfiguration_st.StickKeysNotInitiated_u8 = TRUE;

    WriteStickConfigurationToUserPage ();

    return (TRUE);
}

/*******************************************************************************

  ClearStickKeysNotInitatedToFlash

  Changes
  Date      Author          Info
  05.05.14  RB              Implementation of clear new SD card found

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
#ifdef STORAGE

u8 ClearStickKeysNotInitatedToFlash (void)
{
    // If configuration not found then init it
    if (FALSE == ReadStickConfigurationFromUserPage ())
    {
        InitStickConfigurationToUserPage_u8 ();
    }

    // CI_LocalPrintf ("Clear: Stick keys not initiated\r\n");

    StickConfiguration_st.StickKeysNotInitiated_u8 = FALSE;

    WriteStickConfigurationToUserPage ();

    return (TRUE);
}
#endif

/*******************************************************************************

  CheckForNewFirmwareVersion

  Changes
  Date      Author          Info
  05.07.14  RB              Implementation

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 CheckForNewFirmwareVersion (void) { static u8 UpdateFlag_u8 = FALSE; u8 update_u8;

   if (TRUE == UpdateFlag_u8) { return (TRUE); }

   UpdateFlag_u8 = TRUE; // Run only once

   // If configuration not found then init it if (FALSE == ReadStickConfigurationFromUserPage ()) { InitStickConfigurationToUserPage_u8 (); }

   update_u8 = FALSE; if (VERSION_MAJOR != StickConfiguration_st.VersionInfo_au8[0]) { update_u8 = TRUE; }

   if (VERSION_MINOR != StickConfiguration_st.VersionInfo_au8[1]) { update_u8 = TRUE; } */
/*
   StickConfiguration_st.VersionInfo_au8[2] = 0; // Build number not used StickConfiguration_st.VersionInfo_au8[3] = 0; // Build number not used */
/*
   if (TRUE == update_u8) { WriteStickConfigurationToUserPage (); }

   return (TRUE); }

 */
/*******************************************************************************

  WriteDatetime

  Changes
  Date      Author          Info
  08.02.14  RB              Implementation of save datetime in flash

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 WriteDatetime (u32 Datetime_u32) { flashc_memcpy(FLASHC_USER_PAGE + 138,&Datetime_u32,4,TRUE); return (TRUE); }

 */
/*******************************************************************************

  ReadDatetime

  Changes
  Date      Author          Info
  08.02.14  RB              Implementation of read datetime in flash

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 ReadDatetime (u32 *Datetime_u32) { memcpy (Datetime_u32,(void*)(FLASHC_USER_PAGE + 138),4); return (TRUE); }


 */
/*******************************************************************************

  WriteScId

  Changes
  Date      Author          Info
  20.05.14  RB              Implementation of save SC id in CPU flash

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 WriteScId (u32 *ScId_u32) { flashc_memcpy(FLASHC_USER_PAGE + 142,ScId_u32,4,TRUE);

   StickConfiguration_st.ActiveSmartCardID_u32 = *ScId_u32; return (TRUE); }

 */
/*******************************************************************************

  ReadSdId

  Changes
  Date      Author          Info
  20.05.14  RB              Implementation of save SC id in CPU flash

  Reviews
  Date      Reviewer        Info

*******************************************************************************/
/*

   u8 ReadScId (u32 *ScId_u32) { memcpy (ScId_u32,(void*)(FLASHC_USER_PAGE + 142),4);

   StickConfiguration_st.ActiveSmartCardID_u32 = *ScId_u32; return (TRUE); }


 */
/*******************************************************************************

  WriteXorPatternToFlash

  Changes
  Date      Author          Info
  20.05.14  RB              Implementation of save xor pattern to flash

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 WriteXorPatternToFlash (u8 * XorPattern_pu8)
{
    // flashc_memcpy(FLASHC_USER_PAGE + 146,XorPattern_pu8,32,TRUE);
unsigned char page_buffer[FLASH_PAGE_SIZE];

    memcpy (page_buffer, (const void *) FLASHC_USER_PAGE, FLASH_PAGE_SIZE);
    memcpy (page_buffer + 146, XorPattern_pu8, 32);

    FLASH_Unlock ();
    FLASH_ErasePage (FLASHC_USER_PAGE);
    write_data_to_flash (page_buffer, FLASH_PAGE_SIZE, FLASHC_USER_PAGE);
    FLASH_Lock ();

    return (TRUE);
}

/*******************************************************************************

  ReadXorPatternFromFlash

  Changes
  Date      Author          Info
  20.05.14  RB              Implementation of read xor pattern from flash

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 ReadXorPatternFromFlash (u8 * XorPattern_pu8)
{
    memcpy (XorPattern_pu8, (void *) (FLASHC_USER_PAGE + 146), 32);

    return (TRUE);
}


/*******************************************************************************

  WritePasswordSafeKey

  Stores the encrypted password safe key

  Byte
  Len = 32 Byte

  Changes
  Date      Author          Info
  01.08.14  RB              Renaming

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 WritePasswordSafeKey (u8 * data)
{
    // memcpy ((void*)(FLASHC_USER_PAGE + 178),data,32);

unsigned char page_buffer[FLASH_PAGE_SIZE];

    memcpy (page_buffer, (const void *) FLASHC_USER_PAGE, FLASH_PAGE_SIZE);
    memcpy (page_buffer + 178, data, 32);

    FLASH_Unlock ();
    FLASH_ErasePage (FLASHC_USER_PAGE);
    write_data_to_flash (page_buffer, FLASH_PAGE_SIZE, FLASHC_USER_PAGE);
    FLASH_Lock ();
    return (TRUE);
}

/*******************************************************************************

  ReadPasswordSafeKey

  Read the encrypted password safe key

  Changes
  Date      Author          Info
  01.08.14  RB              Renaming


  Reviews
  Date      Reviewer        Info

*******************************************************************************/

u8 ReadPasswordSafeKey (u8 * data)
{
    memcpy (data, (void *) (FLASHC_USER_PAGE + 178), 32);
    return (TRUE);
}

/*******************************************************************************

  EraseLocalFlashKeyValues_u32

  Changes
  Date      Author          Info
  10.09.14  RB              Implementation


  Reviews
  Date      Reviewer        Info

*******************************************************************************/

#if (defined __GNUC__) && (defined __AVR32__)
__attribute__ ((__aligned__ (4)))
#elif (defined __ICCAVR32__)
#pragma data_alignment = 4
#endif
u8 EraseStoreData_au8[256];

u32 EraseLocalFlashKeyValues_u32 (void)
{
u32 i;

u32 i1;

uint8_t page_buffer[FLASH_PAGE_SIZE];

    // Clear user page
    for (i1 = 0; i1 < 7; i1++)
    {
        for (i = 0; i < 256; i++)
        {
            EraseStoreData_au8[i] = (u8) (rand () % 256);
        }
        // flashc_memcpy((void*)FLASHC_USER_PAGE,EraseStoreData_au8,256,TRUE);
        memcpy (page_buffer, (const void *) FLASHC_USER_PAGE, FLASH_PAGE_SIZE);
        memcpy (page_buffer, EraseStoreData_au8, 256);
        FLASH_Unlock ();
        FLASH_ErasePage (FLASHC_USER_PAGE);
        write_data_to_flash (page_buffer, FLASH_PAGE_SIZE, FLASHC_USER_PAGE);
        FLASH_Lock ();

    }

    // flashc_erase_user_page (TRUE);

    // Set default values
    InitStickConfigurationToUserPage_u8 ();

    // DFU_DisableFirmwareUpdate (); // Stick always starts in application
    // mode
    // CheckForNewSdCard (); // Get SD ID

    // Clear password safe

    for (i1 = 0; i1 < 7; i1++)
    {
        for (i = 0; i < 256; i++)
        {
            EraseStoreData_au8[i] = (u8) (rand () % 256);
        }
        // flashc_memcpy((void*)(PWS_FLASH_START_ADDRESS
        // ),EraseStoreData_au8,256,TRUE);
        // flashc_memcpy((void*)(PWS_FLASH_START_ADDRESS+256),EraseStoreData_au8,256,TRUE);

        // memcpy(page_buffer, PWS_FLASH_START_ADDRESS, FLASH_PAGE_SIZE);
        memcpy (page_buffer, EraseStoreData_au8, 256);
        memcpy (page_buffer + 256, EraseStoreData_au8, 256);
        FLASH_Unlock ();
        FLASH_ErasePage (PWS_FLASH_START_ADDRESS);
        write_data_to_flash (page_buffer, FLASH_PAGE_SIZE, PWS_FLASH_START_ADDRESS);
        FLASH_Lock ();

    }


    // flashc_erase_page(PWS_FLASH_START_PAGE,TRUE);

    // Clear OTP
    for (i1 = 0; i1 < 7; i1++)
    {
        for (i = 0; i < 256; i++)
        {
            EraseStoreData_au8[i] = (u8) (rand () % 256);
        }
        for (i = 0; i < 10; i++)
        {
            // flashc_memcpy((void*)(SLOTS_ADDRESS+i*512
            // ),EraseStoreData_au8,256,TRUE);
            // flashc_memcpy((void*)(SLOTS_ADDRESS+i*512+256),EraseStoreData_au8,256,TRUE);
            // TODO:
            // write_data_to_flash ( EraseStoreData_au8, 256, SLOTS_ADDRESS +
            // i*512);
            // write_data_to_flash ( EraseStoreData_au8, 256, SLOTS_ADDRESS +
            // i*512 + 256);
        }
    }

    for (i = 0; i < 10; i++)
    {
        // flashc_erase_page(OTP_FLASH_START_PAGE+i,TRUE);
    }

    return (TRUE);
}
