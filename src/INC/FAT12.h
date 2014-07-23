/*
* Author: Copyright (C) Rudolf Boeddeker 					Date: 2010-01-26
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

#ifndef INCLUDE_FAT12_H
#define INCLUDE_FAT12_H

#define SECTORSIZE 512

//#pragma pack(1)

/*******************************************************************************

  tBootSectorType

*******************************************************************************/

typedef struct 
{
  unsigned char  JmpBoot[3];                  // 0
  unsigned char  OSName[8];                   // 3
  unsigned short BytesPerSector;              // 11
  unsigned char  SectorsPerCluster;           // 13
  unsigned short ReservedSectors;             // 14
  unsigned char  FATCount;                    // 16
  unsigned short RootEntries;                 // 17
  unsigned short Sectors;                     // 19
  unsigned char  MediaType;                   // 21
  unsigned short FATSectors;                  // 22
  unsigned short SectorsPerTrack;             // 24
  unsigned short Heads;                       // 26
  unsigned long  HiddenSectors;               // 28
  unsigned long  TotSec32;                    // 32
  unsigned char  Drive;                       // 36
  unsigned char  Res1;                        // 37
  unsigned char  BootSig;            					// 38
  unsigned long  VolumeID;                    // 39
  unsigned char  VolumeLabel[11];             // 43
  unsigned char  FatType[8];                  // 54
  unsigned char  Res2[450];                   // 62
} __attribute((packed)) tBootSectorType;


/*******************************************************************************

  tDirEntryType

*******************************************************************************/

#define ATTR_READ_ONLY 		0x01
#define ATTR_HIDDEN 			0x02
#define ATTR_SYSTEM 			0x04
#define ATTR_VOLUME_ID 		0x08
#define ATTR_DIRECTORY 		0x10
#define ATTR_ARCHIVE 			0x20

typedef struct 
{
	unsigned char  DirName[11];									// 0
	unsigned char  DirAttribute;								// 11 
	unsigned char  DirNTRes; 										// 12 
	unsigned char  DirCrtTimeTenth;							// 13 
	unsigned short DirCrtTime;									// 14 
	unsigned short DirCrtDate;									// 16 
	unsigned short DirLstAccDate;								// 18 
	unsigned short DirFstClusHi;								// 20 
	unsigned short DirWrtTime;									// 22 
	unsigned short DirWrtDate;									// 24 
	unsigned short DirFstClusLo;								// 26 
	unsigned long  DirFileSize;									// 28 
} __attribute((packed)) tDirEntryType;


void InitBootSector (char *pRamDisk);
void InitFatSector (char *pRamDisk);
void CreateDefaultDirEntry (char *pRamDisk);
void SetFAT12Entry (char *pFAT12Sec,unsigned short nEntry,unsigned short nData);
unsigned short GetFAT12Entry (char *pFAT12Sec,unsigned short nEntry);
void CreateFilesystem (char *pRamDisk);

#endif /* INCLUDE_FAT12_H */
