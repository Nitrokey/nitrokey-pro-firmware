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
#include "string.h"
#include "memory_ops.h"
#include "CcidLocalAccess.h"

__IO uint8_t temp_password[25];
__IO uint8_t tmp_password_set=0;
__IO uint32_t authorized_crc=0xFFFFFFFF;


uint8_t parse_report(uint8_t *report,uint8_t *output){
	uint8_t cmd_type=report[CMD_TYPE_OFFSET];
	uint32_t received_crc32;
	uint32_t calculated_crc32;
	uint8_t i;

	uint8_t not_authorized=0;

	//uint64_t counter=*((uint64_t *)(report+REPORT_COUNTER_VALUE_OFFSET));



	//received_crc32=((uint32_t *)report)[KEYBOARD_FEATURE_COUNT/4-1];

	received_crc32=getu32(report+KEYBOARD_FEATURE_COUNT-4);
	CRC_ResetDR();
	calculated_crc32=CRC_CalcBlockCRC((uint32_t*) report, KEYBOARD_FEATURE_COUNT/4-1);

	memset(output,0,KEYBOARD_FEATURE_COUNT);
	output[OUTPUT_CMD_TYPE_OFFSET]=cmd_type;

	output[OUTPUT_CMD_CRC_OFFSET]=calculated_crc32&0xFF;
	output[OUTPUT_CMD_CRC_OFFSET+1]=(calculated_crc32>>8)&0xFF;
	output[OUTPUT_CMD_CRC_OFFSET+2]=(calculated_crc32>>16)&0xFF;
	output[OUTPUT_CMD_CRC_OFFSET+3]=(calculated_crc32>>24)&0xFF;

	if (calculated_crc32==received_crc32){


		switch (cmd_type){

		case CMD_GET_STATUS:
			cmd_get_status(report,output);
			break;

		case CMD_WRITE_TO_SLOT:
			if (calculated_crc32==authorized_crc)
			cmd_write_to_slot(report,output);
			else
			not_authorized=1;
			break;

		case CMD_READ_SLOT_NAME:
			cmd_read_slot_name(report,output);
			break;
			
		case CMD_READ_SLOT:
			//if (calculated_crc32==authorized_crc)
			cmd_read_slot(report,output);
			//else
			//not_authorized=1;
			break;
			
		case CMD_GET_CODE:
			cmd_get_code(report,output);
			break;
			
		case CMD_WRITE_CONFIG:
			if (calculated_crc32==authorized_crc)
			cmd_write_config(report,output);
			else
			not_authorized=1;
			break;
			
		case CMD_ERASE_SLOT:
			if (calculated_crc32==authorized_crc)
			cmd_erase_slot(report,output);
			else
			not_authorized=1;
			break;
			
		case CMD_FIRST_AUTHENTICATE:
			cmd_first_authenticate(report,output);
			break;
			
		case CMD_AUTHORIZE:
			cmd_authorize(report,output);
			break;
		
		case CMD_GET_PASSWORD_RETRY_COUNT:
			cmd_get_password_retry_count(report,output);
			break;

			//FLASH_Unlock();
			// FLASH_ErasePage(oath_slots[slot]);
			//FLASH_ErasePage(oath_slots[slot]+COUNTER_PAGE_OFFSET);
			
			//write_data_to_flash(report+REPORT_COUNTER_VALUE_OFFSET,8,oath_slots[slot]+COUNTER_PAGE_OFFSET);
			//write_data_to_flash(report+REPORT_SECRET_VALUE_OFFSET,20,oath_slots[slot]+SECRET_OFFSET);
			
			//FLASH_Lock();
			

		default:
			break;

		}
		
		if (not_authorized)
		output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_NOT_AUTHORIZED;
		
		if (calculated_crc32==authorized_crc)
		authorized_crc=0xFFFFFFFF;

	}
	else
	output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_WRONG_CRC;

	CRC_ResetDR();
	calculated_crc32=CRC_CalcBlockCRC((uint32_t*) output, KEYBOARD_FEATURE_COUNT/4-1);

	output[OUTPUT_CRC_OFFSET]=calculated_crc32&0xFF;
	output[OUTPUT_CRC_OFFSET+1]=(calculated_crc32>>8)&0xFF;
	output[OUTPUT_CRC_OFFSET+2]=(calculated_crc32>>16)&0xFF;
	output[OUTPUT_CRC_OFFSET+3]=(calculated_crc32>>24)&0xFF;

	return 0;
}

uint8_t cmd_get_status(uint8_t *report,uint8_t *output){

	output[OUTPUT_CMD_RESULT_OFFSET]=FIRMWARE_VERSION&0xFF;
	output[OUTPUT_CMD_RESULT_OFFSET+1]=(FIRMWARE_VERSION>>8)&0xFF;
	output[OUTPUT_CMD_RESULT_OFFSET+2]=cardSerial&0xFF;
	output[OUTPUT_CMD_RESULT_OFFSET+3]=(cardSerial>>8)&0xFF;
	output[OUTPUT_CMD_RESULT_OFFSET+4]=(cardSerial>>16)&0xFF;
	output[OUTPUT_CMD_RESULT_OFFSET+5]=(cardSerial>>24)&0xFF;
	memcpy(output+OUTPUT_CMD_RESULT_OFFSET+6,(uint8_t *)SLOTS_PAGE1_ADDRESS+GLOBAL_CONFIG_OFFSET,3);

	return 0;
}


uint8_t cmd_get_password_retry_count(uint8_t *report,uint8_t *output){

	output[OUTPUT_CMD_RESULT_OFFSET]=getPasswordRetryCount();

	return 0;
}

uint8_t cmd_write_to_slot(uint8_t *report,uint8_t *output){

	uint8_t slot_no=report[CMD_WTS_SLOT_NUMBER_OFFSET];
	uint8_t slot_tmp[64];//this is will be the new slot contents

	memset(slot_tmp,0,64);
	slot_tmp[0]=0x01; //marks slot as programmed
	memcpy(slot_tmp+1,report+CMD_WTS_SLOT_NAME_OFFSET,49);

	if (slot_no>=0x10&&slot_no<=0x10+NUMBER_OF_HOTP_SLOTS){//HOTP slot
		slot_no=slot_no&0x0F;
		
		uint64_t counter = getu64(report+CMD_WTS_COUNTER_OFFSET);
		set_counter_value(hotp_slot_counters[slot_no], counter);
		write_to_slot(slot_tmp, hotp_slot_offsets[slot_no], 64);
		

	}
	else if (slot_no>=0x20&&slot_no<=0x20+NUMBER_OF_TOTP_SLOTS){//TOTP slot
		slot_no=slot_no&0x0F;
		
		write_to_slot(slot_tmp, totp_slot_offsets[slot_no], 64);
		

	}
	else{
		output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_WRONG_SLOT;


	}


	return 0;
}


uint8_t cmd_read_slot_name(uint8_t *report,uint8_t *output){

	uint8_t slot_no=report[1];
	uint64_t counter;

	if (slot_no>=0x10&&slot_no<=0x10+NUMBER_OF_HOTP_SLOTS){//HOTP slot
		slot_no=slot_no&0x0F;
		uint8_t is_programmed=*((uint8_t *)(hotp_slots[slot_no]));
		if (is_programmed==0x01){
			memcpy(output+OUTPUT_CMD_RESULT_OFFSET,(uint8_t *)(hotp_slots[slot_no]+SLOT_NAME_OFFSET),15);
		} else {
			output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_SLOT_NOT_PROGRAMMED;
		}

	}
	else if (slot_no>=0x20&&slot_no<=0x20+NUMBER_OF_TOTP_SLOTS){//TOTP slot
		slot_no=slot_no&0x0F;
		uint8_t is_programmed=*((uint8_t *)(totp_slots[slot_no]));
		if (is_programmed==0x01){
			memcpy(output+OUTPUT_CMD_RESULT_OFFSET,(uint8_t *)(totp_slots[slot_no]+SLOT_NAME_OFFSET),15);
		} else {
			output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_SLOT_NOT_PROGRAMMED;
		}

	}
	else{
		output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_WRONG_SLOT;

	}


	return 0;
}


uint8_t cmd_read_slot(uint8_t *report,uint8_t *output){
	
	uint8_t slot_no=report[1];
        uint64_t counter;

        if (slot_no>=0x10&&slot_no<=0x10+NUMBER_OF_HOTP_SLOTS){//HOTP slot
                slot_no=slot_no&0x0F;
                uint8_t is_programmed=*((uint8_t *)(hotp_slots[slot_no]));
                if (is_programmed==0x01){
                        memcpy(output+OUTPUT_CMD_RESULT_OFFSET,(uint8_t *)(hotp_slots[slot_no]+SLOT_NAME_OFFSET),15);
            		memcpy(output+OUTPUT_CMD_RESULT_OFFSET+15,(uint8_t *)(hotp_slots[slot_no]+CONFIG_OFFSET),1);
                        memcpy(output+OUTPUT_CMD_RESULT_OFFSET+16,(uint8_t *)(hotp_slots[slot_no]+TOKEN_ID_OFFSET),13);
                        counter=get_counter_value(hotp_slot_counters[slot_no]);
                        memcpy(output+OUTPUT_CMD_RESULT_OFFSET+29,&counter,8);
	        } else {
                        output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_SLOT_NOT_PROGRAMMED;
                }

        }
        else if (slot_no>=0x20&&slot_no<=0x20+NUMBER_OF_TOTP_SLOTS){//TOTP slot
                slot_no=slot_no&0x0F;
                uint8_t is_programmed=*((uint8_t *)(totp_slots[slot_no]));
                if (is_programmed==0x01){
                        memcpy(output+OUTPUT_CMD_RESULT_OFFSET,(uint8_t *)(totp_slots[slot_no]+SLOT_NAME_OFFSET),15);
			memcpy(output+OUTPUT_CMD_RESULT_OFFSET+15,(uint8_t *)(totp_slots[slot_no]+CONFIG_OFFSET),1);
                        memcpy(output+OUTPUT_CMD_RESULT_OFFSET+16,(uint8_t *)(totp_slots[slot_no]+TOKEN_ID_OFFSET),13);
                } else {
                        output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_SLOT_NOT_PROGRAMMED;
                }

        }
        else{
                output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_WRONG_SLOT;

        }


        return 0;
}


uint8_t cmd_get_code(uint8_t *report,uint8_t *output){
	
	uint64_t challenge=getu64(report+CMD_GC_CHALLENGE_OFFSET);
	uint32_t result=0;
	

	uint8_t slot_no=report[CMD_GC_SLOT_NUMBER_OFFSET];
	
	if (slot_no>=0x10&&slot_no<=0x10+NUMBER_OF_HOTP_SLOTS){//HOTP slot
		slot_no=slot_no&0x0F;
		uint8_t is_programmed=*((uint8_t *)(hotp_slots[slot_no]));
		if (is_programmed==0x01){
			
			result=get_code_from_hotp_slot(slot_no);
			memcpy(output+OUTPUT_CMD_RESULT_OFFSET,&result,4);
			memcpy(output+OUTPUT_CMD_RESULT_OFFSET+4,(uint8_t *)hotp_slots[slot_no]+CONFIG_OFFSET,14);
			
			
			
		}
		else
		output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_SLOT_NOT_PROGRAMMED;

	}
	else if (slot_no>=0x20&&slot_no<=0x20+NUMBER_OF_TOTP_SLOTS){//TOTP slot
		slot_no=slot_no&0x0F;
		uint8_t is_programmed=*((uint8_t *)(totp_slots[slot_no]));
		if (is_programmed==0x01){
			result=get_code_from_totp_slot(slot_no,challenge);
			memcpy(output+OUTPUT_CMD_RESULT_OFFSET,&result,4);
			memcpy(output+OUTPUT_CMD_RESULT_OFFSET+4,(uint8_t *)totp_slots[slot_no]+CONFIG_OFFSET,14);
			
		}
		else
		output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_SLOT_NOT_PROGRAMMED;

	}
	else{
		output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_WRONG_SLOT;
	}


	return 0;
}


uint8_t cmd_write_config(uint8_t *report,uint8_t *output){
	
	uint8_t slot_tmp[3];//this is will be the new slot contents
	memset(slot_tmp,0,3);

	memcpy(slot_tmp,report+1,3);

	write_to_slot(slot_tmp,GLOBAL_CONFIG_OFFSET, 3);
	
	return 0;
	
}


uint8_t cmd_erase_slot(uint8_t *report,uint8_t *output){

	uint8_t slot_no=report[CMD_WTS_SLOT_NUMBER_OFFSET];
	uint8_t slot_tmp[64];

	memset(slot_tmp,0xFF,64);
	

	if (slot_no>=0x10&&slot_no<=0x10+NUMBER_OF_HOTP_SLOTS){//HOTP slot
		slot_no=slot_no&0x0F;
		write_to_slot(slot_tmp, hotp_slot_offsets[slot_no], 64);
		erase_counter(slot_no);
	}
	else if (slot_no>=0x20&&slot_no<=0x20+NUMBER_OF_TOTP_SLOTS){//TOTP slot
		slot_no=slot_no&0x0F;	
		write_to_slot(slot_tmp, totp_slot_offsets[slot_no], 64);
	}
	else{
		output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_WRONG_SLOT;
	}

	return 0;
}

uint8_t cmd_first_authenticate(uint8_t *report,uint8_t *output){
	
	uint8_t res=1;	
	uint8_t card_password[26];
	

		
		memset(card_password,0,26);
		
		memcpy(card_password,report+1,25);
		
		
		res=cardAuthenticate(card_password);
		
		if (res==0){
			memcpy(temp_password,report+26,25);
			tmp_password_set=1;
			getAID();
			return 0;
		}
		else{
			output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_WRONG_PASSWORD;
			return 1; //wrong card password
		}
	

}


uint8_t cmd_authorize(uint8_t *report,uint8_t *output){
	
	if (tmp_password_set==1){
	
	if (memcmp(report+5,temp_password,25)==0)	
	authorized_crc=getu32(report+1);
	else	
	output[OUTPUT_CMD_STATUS_OFFSET]=CMD_STATUS_WRONG_PASSWORD;

	}
	
}

