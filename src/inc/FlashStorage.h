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
 * FlashStorage.h
 *
 *  Created on: 12.07.2013
 *      Author: RB
 */

#ifndef FLASHSTORAGE_H_
#define FLASHSTORAGE_H_

#include "stm32f10x.h"

u8 WriteAESStorageKeyToUserPage (u8 * data);

u8 WriteStickConfigurationToUserPage (void);

u8 ReadStickConfigurationFromUserPage (void);

u8 InitStickConfigurationToUserPage_u8 (void);

u32 EraseLocalFlashKeyValues_u32 (void);

/***************************************************************************************

  Structure is also send to GUI

***************************************************************************************/

#define MAGIC_NUMBER_STICK20_CONFIG     0x1810  // Change when
                                                // typeStick20Configuration_st
                                                // is changed

#define READ_WRITE_ACTIVE             0
#define READ_ONLY_ACTIVE              1

#define SD_UNCRYPTED_VOLUME_BIT_PLACE   0
#define SD_CRYPTED_VOLUME_BIT_PLACE     1
#define SD_HIDDEN_VOLUME_BIT_PLACE      2

/*
   #if (defined __GNUC__) && (defined __AVR32__) __attribute__((__aligned__(4))) #elif (defined __ICCAVR32__) #pragma data_alignment = 4 #endif */
/* Look for 4 byte alignment of 32 bit values */
typedef struct
{
    u16 MagicNumber_StickConfig_u16;    // Shows that the structure is valid
    // 2 byte // 2
    u8 ReadWriteFlagUncryptedVolume_u8; // Flag stores the read/write flag in
    // the CPU flash 1 byte // 3
    u8 ReadWriteFlagCryptedVolume_u8;   // Flag stores the read/write flag in
    // the CPU flash 1 byte // 4
    u8 VersionInfo_au8[4];      // 4 byte // 8
    u8 ReadWriteFlagHiddenVolume_u8;    // Flag stores the read/write flag in
    // the CPU flash 1 byte // 9
    u8 FirmwareLocked_u8;       // 1 byte // 10
    u8 NewSDCardFound_u8;       // Bit 0 new card found, bit 1-7 change counter 1
    // byte // 11
    u8 SDFillWithRandomChars_u8;    // Bit 0 = 1 = filled, bit 1-7 change
    // counter 1 byte // 12
    u32 ActiveSD_CardID_u32;    // 4 byte // 16
    u8 VolumeActiceFlag_u8;     // 1 byte // 17
    u8 NewSmartCardFound_u8;    // Bit 0 new card found, bit 1-7 change
    // counter 1 byte
    u8 UserPwRetryCount;        // User password retry count 1 byte // 19
    u8 AdminPwRetryCount;       // Admin password retry count 1 byte // 20 Byte
    // not packed
    u32 ActiveSmartCardID_u32;  // 4 byte
    u8 StickKeysNotInitiated_u8;    // No AES keys computed (1 = AES are
    // builded) 1 byte // 25 Byte not packed
} typeStick20Configuration_st;  // Sum 25 byte (Max 25 Byte) // not packed


extern typeStick20Configuration_st StickConfiguration_st;

u8 WriteXorPatternToFlash (u8 * XorPattern_pu8);
u8 ReadXorPatternFromFlash (u8 * XorPattern_pu8);

u8 WritePasswordSafeKey (u8 * data);

u8 ReadPasswordSafeKey (u8 * data);
u8 ReadAESStorageKeyToUserPage (u8 * data);

u8 WriteUpdatePinHashToFlash (u8 * PIN_Hash_pu8);
u8 ReadUpdatePinHashFromFlash (u8 * PIN_Hash_pu8);
u8 WriteUpdatePinSaltToFlash (u8 * PIN_pu8);
u8 ReadUpdatePinSaltFromFlash (u8 * PIN_pu8);

u8 CheckUpdatePin (u8 * Password_pu8, u32 PasswordLen_u32);
u8 StoreNewUpdatePinHashInFlash (u8 * Password_pu8, u32 PasswordLen_u32);

u8 WriteBootloaderFlagToFlash (void);
u8 EraseBootloaderFlagFromFlash (void);


u8 ClearStickKeysNotInitatedToFlash (void);
u8 SetStickKeysNotInitatedToFlash (void);

#endif /* FLASHSTORAGE_H_ */
