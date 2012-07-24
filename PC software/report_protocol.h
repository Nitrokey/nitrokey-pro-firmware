/*
* Author: Copyright (C) Andrzej Surowiec 2012
*												
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

#define CMD_GET_STATUS 0x00
#define CMD_WRITE_TO_SLOT 0x01


/*
CMD_WRITE_TO_SLOT
size	offset	description
1		0 		command type
1 		1		slot number
8		2		counter value
20		10		secret
*/
#define		REPORT_SLOT_NUMBER_OFFSET 1
#define		REPORT_COUNTER_VALUE_OFFSET 2
#define		REPORT_SECRET_VALUE_OFFSET 10

