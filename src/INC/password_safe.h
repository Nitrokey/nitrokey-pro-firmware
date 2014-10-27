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
 * password_safe.h
 *
 *  Created on: 29.07.2014
 *      Author: RB
 */

#ifndef PASSWORD_SAFE_H_
#define PASSWORD_SAFE_H_

#include "stm32f10x.h"

void IBN_PWS_Tests (unsigned char nParamsGet_u8,unsigned char CMD_u8,unsigned int Param_u32,unsigned char *String_pu8);

// #define FLASH_PAGE_SIZE 512 // AVR
#define FLASH_PAGE_SIZE 1024 // stm32f10x


#define FLASH_START             0x80000000
#define PWS_FLASH_START_PAGE    120

//0x801E000
#define PWS_FLASH_START_ADDRESS (FLASH_START + PWS_FLASH_START_PAGE * FLASH_PAGE_SIZE + (FLASH_PAGE_SIZE*0))


#define PWS_SLOT_COUNT            16

#define PWS_SLOT_ACTIV_TOKEN      0xA5
#define PWS_SLOT_INACTIV_TOKEN    0x5A



#define PWS_SLOTSTATE_START     0
#define PWS_SLOTSTATE_LENGTH    1

#define PWS_SLOTNAME_START      1
#define PWS_SLOTNAME_LENGTH     11

#define PWS_PASSWORD_START      (PWS_SLOTSTATE_LENGTH + PWS_SLOTNAME_LENGTH)    // 12 = 1 + 11
#define PWS_PASSWORD_LENGTH     20

#define PWS_LOGINNAME_START     (PWS_SLOTSTATE_LENGTH + PWS_SLOTNAME_LENGTH + PWS_PASSWORD_LENGTH)    // 32 = 1 + 11 + 20
#define PWS_LOGINNAME_LENGTH    32

#define PWS_SLOT_LENGTH         (PWS_SLOTSTATE_LENGTH + PWS_SLOTNAME_LENGTH + PWS_PASSWORD_LENGTH + PWS_LOGINNAME_LENGTH) //  64 = 1 + 11 + 20 + 32
#define PWS_WRITECOUNTER_START  (PWS_SLOT_COUNT*PWS_SLOT_LENGTH)

#define PWS_SEND_PASSWORD     0
#define PWS_SEND_LOGINNAME    1
#define PWS_SEND_TAB          2
#define PWS_SEND_CR           3

/*******************************************************************************

  Typedef password slot

*******************************************************************************/

typedef
  #if (defined __ICCAVR32__)
    #pragma pack(1)
  #endif
struct
  #if (defined __GNUC__)
    __attribute__((__packed__))
  #endif
{
  u8 SlotActiv_u8;                            // Shows the state of the slot                         1 byte  //   1
  u8 SlotName_au8[PWS_SLOTNAME_LENGTH];       // Contains the name of the slot                      11 byte  //  12
  u8 SlotPassword_au8[PWS_PASSWORD_LENGTH];   // Contains the password of the slot                  20 byte  //  32
  u8 SlotLoginName_au8[PWS_LOGINNAME_LENGTH]; // Contains the loginname of the slot                 32 byte  //  64
}                                             // Sum 32 byte
  #if (defined __ICCAVR32__)
  #pragma pack()
  #endif
typePasswordSafeSlot_st;


/*******************************************************************************

  Typedef password struct

*******************************************************************************/

typedef
  #if (defined __ICCAVR32__)
    #pragma pack(1)
  #endif
struct
  #if (defined __GNUC__)
    __attribute__((__packed__))
  #endif
{
  typePasswordSafeSlot_st Slot_st[PWS_SLOT_COUNT];      // Contains the password of the slot                  16 byte  //  16
  u32 SlotWriteCount_u32;                               // Shows flash write count                             4 byte  //   1
}                                                       // Sum 32 byte
  #if (defined __ICCAVR32__)
  #pragma pack()
  #endif
typePasswordSafe_st;


u8 PWS_GetAllSlotStatus (u8 *StatusArray_pu8);
u8 PWS_GetSlotName (u8 Slot_u8, u8 *Name_pu8);
u8 PWS_GetSlotPassword (u8 Slot_u8, u8 *Password_pu8);
u8 PWS_GetSlotLoginName (u8 Slot_u8, u8 *Loginname_pu8);
u8 PWS_WriteSlotData_1 (u8 Slot_u8,u8 *Name_pu8, u8 *Password_pu8);
u8 PWS_WriteSlotData_2 (u8 Slot_u8,u8 *Loginname_pu8);
u8 PWS_EraseSlot (u8 Slot_u8);
u8 PWS_DecryptedPasswordSafeKey (void);
u8 PWS_EnableAccess (u8 *password);
u8 PWS_InitKey (void);
u8 PWS_GetDecryptedPasswordSafeKey (u8 **Key_pu8);
u8 PWS_SendData (u8 Slot_u8,u8 Kind_u8);

#endif /* PASSWORD_SAFE_H_ */
