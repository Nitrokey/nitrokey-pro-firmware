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

#include "stm32f10x_it.h"
#include "stm32f10x_flash.h"
#include "hw_config.h"
#include "platform_config.h"
#include "sha1.h"
#include "hmac-sha1.h"
#include "hotp.h"


uint32_t oath_slots[2]={SLOT1_ADDRESS,SLOT2_ADDRESS};


uint64_t endian_swap (uint64_t x)
{
  x = (x >> 56) |
    ((x << 40) & 0x00FF000000000000) |
    ((x << 24) & 0x0000FF0000000000) |
    ((x << 8) & 0x000000FF00000000) |
    ((x >> 8) & 0x00000000FF000000) |
    ((x >> 24) & 0x0000000000FF0000) |
    ((x >> 40) & 0x000000000000FF00) | (x << 56);
  return x;
}

uint32_t dynamic_truncate (uint8_t * hmac_result)
{

  uint8_t offset = hmac_result[19] & 0xf;
  uint32_t bin_code = (hmac_result[offset] & 0x7f) << 24
    | (hmac_result[offset + 1] & 0xff) << 16
    | (hmac_result[offset + 2] & 0xff) << 8
    | (hmac_result[offset + 3] & 0xff);

  return bin_code;
}

extern uint8_t HID_GetReport_Value[32+1];

void write_data_to_flash(uint8_t *data,uint8_t len,uint32_t addr){
uint16_t i;

   FLASH_Unlock();
    FLASH_ErasePage(addr);
	
	for (i = 0; i < len; i+=2){	
	uint16_t halfword=(data[i])+(data[i+1]<<8);
	FLASH_ProgramHalfWord(addr+i, halfword);
	}
	
	FLASH_Lock();

}

/*Get a HOTP/TOTP truncated value 
counter - HOTP/TOTP counter value
secret - pointer to secret stored in memory
secret_length - length of the secret
len - length of the truncated result, 6 or 8
*/
uint32_t get_htop_value(uint64_t counter,uint8_t * secret,uint8_t secret_length,uint8_t len){
	  uint8_t hmac_result[20];
	  uint64_t c = endian_swap(counter);
      hmac_sha1 (hmac_result, secret, secret_length*8, &c, 64);
      uint32_t hotp_result = dynamic_truncate (hmac_result);
	  if (len==6)
      hotp_result = hotp_result % 1000000;
	  else if (len==8)
	  hotp_result = hotp_result % 100000000;
	  else return 0;
      return hotp_result;
}

/*Get the HOTP counter stored in flash
addr - counter page address
*/
uint32_t get_counter_value(uint32_t addr){

uint16_t i;
uint32_t counter=0;
uint8_t *ptr=(uint8_t *)addr;

for (i=0;i<4;i++){
counter+=*ptr<<(8*i);
ptr++;
}

i=0;
while(i<1020){
if (*ptr==0xff)
break;
ptr++;
counter++;
i++;
}

return counter;
}



/*Increment the HOTP counter stored in flash
addr - counter page address
*/
uint8_t increment_counter_page(uint32_t addr){
uint16_t i;
uint8_t *ptr=(uint8_t *)addr;
uint32_t counter;
FLASH_Status err=FLASH_COMPLETE;

if (ptr[1023]==0x00){
//Entire page is filled, erase cycle
counter=get_counter_value(addr)+1;

FLASH_Unlock();
err=FLASH_ErasePage(BACKUP_PAGE_ADDRESS);
if (err!=FLASH_COMPLETE) return err;
err=FLASH_ProgramHalfWord(BACKUP_PAGE_ADDRESS, addr);
if (err!=FLASH_COMPLETE) return err;
err=FLASH_ProgramWord(BACKUP_PAGE_ADDRESS+4, counter);
if (err!=FLASH_COMPLETE) return err;


err=FLASH_ErasePage(addr);
if (err!=FLASH_COMPLETE) return err;
err=FLASH_ProgramWord(addr, counter);
if (err!=FLASH_COMPLETE) return err;

err=FLASH_ProgramHalfWord(BACKUP_PAGE_ADDRESS+8, "OK");
if (err!=FLASH_COMPLETE) return err;

FLASH_Lock();
}
else{

ptr+=4;
i=0;
while(i<1020){
if (*ptr==0xff)
break;
ptr++;
i++;
}

FLASH_Unlock();

if ((uint32_t)ptr%2){ //odd byte

err=FLASH_ProgramHalfWord((uint32_t)ptr-1, 0x0000);
 

}
else{ //even byte

err=FLASH_ProgramHalfWord((uint32_t)ptr, 0xff00);

}
FLASH_Lock();

}

return err; //no error
}


uint32_t get_code_from_slot(uint8_t slot){
uint32_t result;
uint8_t len=6;
uint64_t counter;
FLASH_Status err;

if (slot>=NUMBER_OF_SLOTS) return 0;

counter=get_counter_value(oath_slots[slot]+COUNTER_PAGE_OFFSET);
result= get_htop_value(counter,(uint8_t *)(oath_slots[slot]+SECRET_OFFSET),20,len);
err=increment_counter_page(oath_slots[slot]+COUNTER_PAGE_OFFSET);
if (err!=FLASH_COMPLETE) return 0;

return counter;
}



