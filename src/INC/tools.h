/*
* Author: Copyright (C) Rudolf Boeddeker  Date: 2012-08-18
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
 * tools.h
 *
 *  Created on: 24.06.2010
 *      Author: RB
 */

#ifndef TOOLS_H_
#define TOOLS_H_

/*
typedef unsigned char  	   u8;
typedef unsigned short 	   u16;
typedef unsigned int  	   u32;
typedef signed char  	   s8;
typedef signed short 	   s16;
typedef signed long        s32;
typedef unsigned long long u64;
typedef signed long long   s64;
*/

void Delay1Ms_Counting (void);
void Delay1Ms (void);
void DelayMs (int nMs);

#endif /* TOOLS_H_ */
