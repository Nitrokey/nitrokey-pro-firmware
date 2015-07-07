/*
 * Author: Copyright (C) Rudolf Boeddeker                                                   Date: 2010-01-13
 *                                              STMicroelectronics                                              Date:   02/20/2000
 *                                               MICROCONTROLLER DIVISION / ST San Jose     Version V 0.9
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


#ifndef MACRO_H
#define MACRO_H

typedef unsigned int Word;

typedef unsigned char Byte;

#define MAKEWORD(msb, lsb)      ((Word)(((Word)((Byte)(msb))) << 8) | ((Byte)(lsb)))
#define MAKELONG(msw, lsw)		((LWord)(((Word)(lsw)) | ((LWord)((Word)(msw))) << 16))
#define LOWORD(l)				((Word)(l))
#define HIWORD(l)				((Word)(((LWord)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)				((Byte)(w))
#define HIBYTE(w)				((Byte)(((Word)(w) >> 8) & 0xFF))
#define LOLONG(l)				((short)(l))


#endif
