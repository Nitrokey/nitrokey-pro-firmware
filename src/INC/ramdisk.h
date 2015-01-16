/*
* Author: Copyright (C) Rudolf Boeddeker						Date: 2010-01-13
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


#ifndef RAMDISK_H
#define RAMDISK_H

#define RAMDISK_START_IN_MEM				((uint32_t)(0x20000000 + 0x4000))								/* r8 = 0x4000, rc = 0xa0000 Check build config and ram chip size	*/
#define RAMDISK_SIZE								((uint32_t) 0x1000)
#define RAMDISK_BLOCK_SIZE          ((uint16_t) 0x0200) 														/* 512 bytes per block */
#define RAMDISK_MAX_BLOCK           ((uint16_t)(RAMDISK_SIZE / RAMDISK_BLOCK_SIZE)) 

extern char *RamdiskStart;

uint16_t RamdiskInit (void);
uint16_t RamdiskWrite (uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length);
uint16_t RamdiskRead (uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length);


#endif /* RAMDISK_H */







