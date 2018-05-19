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
 * HandleAesStorageKey.h
 *
 *  Created on: 15.06.2012
 *      Author: RB
 */

#ifndef HANDLE_AES_STORAGE_KEY_H
#define HANDLE_AES_STORAGE_KEY_H

#include "stm32f10x.h"

u32 BuildStorageKeys_u32 (u8 * AdminPW_pu8);

u32 GetStorageKey_u32 (u8 * UserPW_pu8, u8 * StorageKey_pu8);

u32 DecryptKeyViaSmartcard_u32 (u8 * StorageKey_pu8);

u8 CheckStorageKey_u8 (void);

u8 StartupCheck_u8 (void);

void HighLevelTests (unsigned char nParamsGet_u8, unsigned char CMD_u8, unsigned int Param_u32, unsigned char* String_pu8);

u32 BuildNewAesMasterKey_u32 (u8 * AdminPW_pu8, u8 * MasterKey_pu8);

u32 BuildNewAesStorageKey_u32 (u8 * MasterKey_pu8);
u32 BuildNewXorPattern_u32 (void);
u32 XorAesKey_u32 (u8 * AesKey_au8);



#endif /* HANDLE_AES_STORAGE_KEY_H */
