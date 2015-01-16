/*
* Author: Copyright (C) Rudolf Boeddeker 					Date: 2010-01-13
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


#ifndef __ACCESSINTERFACE_H_ 
#define __ACCESSINTERFACE_H_ 

int USB_SwitchUSBDevice (int nNewUSBDev);
int USB_Reconnect (void);
short SetRAMFileText (u16 nPos, char *szText);
short SetFileHeader (void);
short SetInputLine (short nFilePos,short *nInputCharPos);
char WaitForInputChar (short nInputCharPos);
short ShowHelp (short  nFilePos);
short PrintPasswordMatrix (short nFilePos);

int ValidateUSBAccess (void);
int RamDiskUserInterface (void);

#endif /* __ACCESSINTERFACE_H_ */



