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


#include "stm32f10x.h"
#include "platform_config.h"
#include "stm32f10x_crc.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "CCID_usb.h"
#include "keyboard.h"
#include "AccessInterface.h"
#include "hotp.h"
#include "report_protocol.h"
#include "CCIDHID_usb_desc.h"


uint8_t parse_report(uint8_t *report,uint8_t *output){
uint8_t cmd_type=report[0];
uint32_t received_crc32;
uint32_t calculated_crc32;
uint8_t i;

uint8_t slot=*(report+REPORT_SLOT_NUMBER_OFFSET);
//uint64_t counter=*((uint64_t *)(report+REPORT_COUNTER_VALUE_OFFSET));


received_crc32=((uint32_t *)report)[KEYBOARD_FEATURE_COUNT/4-1];
CRC_ResetDR();
calculated_crc32=CRC_CalcBlockCRC((uint32_t*) report, KEYBOARD_FEATURE_COUNT/4-1);

if (calculated_crc32!=received_crc32)
return 1;

((uint32_t *)output)[0]=0;
((uint32_t *)output)[1]=calculated_crc32;
((uint32_t *)output)[2]=received_crc32;



switch (cmd_type){

case CMD_GET_STATUS:

	break;

case CMD_WRITE_TO_SLOT:
	if (slot>=NUMBER_OF_SLOTS)
	return 1;
	
	
	FLASH_Unlock();
    FLASH_ErasePage(oath_slots[slot]);
	FLASH_ErasePage(oath_slots[slot]+COUNTER_PAGE_OFFSET);
	
	write_data_to_flash(report+REPORT_COUNTER_VALUE_OFFSET,8,oath_slots[slot]+COUNTER_PAGE_OFFSET);
	write_data_to_flash(report+REPORT_SECRET_VALUE_OFFSET,20,oath_slots[slot]+SECRET_OFFSET);
	
	FLASH_Lock();
	
	
	break;

default:
	break;

}

output[1]='O';
output[2]='K';
return 0;
}