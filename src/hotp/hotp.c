/*
 * Author: Copyright (C) Andrzej Surowiec 2012
 *
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

#include "stm32f10x_it.h"
#include "stm32f10x_flash.h"
#include "hw_config.h"
#include "platform_config.h"
#include "sha1.h"
#include "hmac-sha1.h"
#include "hotp.h"
#include "string.h"
#include "memory_ops.h"

const int SECRET_LENGTH = SECRET_LENGTH_DEFINE;

__I uint32_t hotp_slot_counters[NUMBER_OF_HOTP_SLOTS] = { SLOT1_COUNTER_ADDRESS,
    SLOT2_COUNTER_ADDRESS,
    SLOT3_COUNTER_ADDRESS,
    SLOT4_COUNTER_ADDRESS,
};

uint8_t page_buffer[SLOT_PAGE_SIZE];


uint32_t get_HOTP_slot_offset(int slot_count){
    return SLOTS_PAGE1_ADDRESS + get_slot_offset(slot_count);
}

uint32_t get_TOTP_slot_offset(int slot_count){
    return SLOTS_PAGE1_ADDRESS + get_slot_offset(slot_count + NUMBER_OF_HOTP_SLOTS);
}
/*
78 per slot
0 64-141
1 142-219
2 220-297
3 298-375
4 376-453
5 454-531
6 532-609
7 610-687
8 688-765
9 766-843
10 844-921
11 922-999
12 1110-1187
13 1188-1265
14 1266-1343
15 1344-1421
16 1422-1499
17 1500-1577
 */
uint32_t get_slot_offset(int slot_count){
    const int global_config_offset = 64;
    size_t slot_offset = sizeof(OTP_slot) * slot_count + global_config_offset;
    const int first_page_limit = SLOT_PAGE_SIZE - sizeof(OTP_slot);
    const int second_page_start = 1024+8;
    if (slot_offset > first_page_limit){
        slot_offset -= first_page_limit;
        slot_offset += second_page_start;
    }
    return slot_offset;
}

uint64_t endian_swap (uint64_t x)
{
    x = (x >> 56) |
        ((x << 40) & 0x00FF000000000000) |
        ((x << 24) & 0x0000FF0000000000) |
        ((x << 8) & 0x000000FF00000000) |
        ((x >> 8) & 0x00000000FF000000) | ((x >> 24) & 0x0000000000FF0000) | ((x >> 40) & 0x000000000000FF00) | (x << 56);
    return x;
}

uint32_t dynamic_truncate (uint8_t * hmac_result)
{

uint8_t offset = hmac_result[19] & 0xf;

uint32_t bin_code = (hmac_result[offset] & 0x7f) << 24
        | (hmac_result[offset + 1] & 0xff) << 16 | (hmac_result[offset + 2] & 0xff) << 8 | (hmac_result[offset + 3] & 0xff);

    return bin_code;
}

extern uint8_t HID_GetReport_Value[32 + 1];

uint32_t crc (uint32_t time)
{

int i;

uint32_t value = time << 8;

    for (i = 0; i < 24; i++)
    {
        if (value & 0x80000000)
            value = (value) ^ 0x98800000;   // Polynomial used in STM32

        value = value << 1;
    }

    time = (time << 8) + (value >> 24);

    return time;
}

void write_data_to_flash (uint8_t * data, uint16_t len, uint32_t addr)
{
uint16_t i;

FLASH_Status err = FLASH_COMPLETE;

    // FLASH_Unlock();
    // FLASH_ErasePage(addr);

    // for (i = 0; i < len; i++){
    // FLASH_ProgramHalfWord(addr+(i*2), data[i]);
    // }

    for (i = 0; i < len; i += 2)
    {
uint16_t halfword = (data[i]) + (data[i + 1] << 8);

        err = FLASH_ProgramHalfWord (addr + i, halfword);
        if (err != FLASH_COMPLETE)
        {
        };
    }



    // FLASH_Lock();

}

/* Get a HOTP/TOTP truncated value counter - HOTP/TOTP counter value secret - pointer to secret stored in memory secret_length - length of the secret
   len - length of the truncated result, 6 or 8 */
uint32_t get_hotp_value (uint64_t counter, uint8_t * secret, uint8_t secret_length, uint8_t len)
{
uint8_t hmac_result[20];

uint64_t c = endian_swap (counter);

    hmac_sha1 (hmac_result, secret, secret_length * 8, &c, 64);
uint32_t hotp_result = dynamic_truncate (hmac_result);

    if (len == 6)
        hotp_result = hotp_result % 1000000;
    else if (len == 8)
        hotp_result = hotp_result % 100000000;
    else
        return 0;
    StartBlinkingOATHLED (2);
    return hotp_result;
}

/* Get the HOTP counter stored in flash addr - counter page address */
uint64_t get_counter_value (uint32_t addr)
{

uint16_t i;

uint64_t counter = 0;

uint8_t* ptr = (uint8_t *) addr;

    // for (i=0;i<4;i++){
    // counter+=*ptr<<(8*i);
    // ptr++;
    // }

    counter = *((uint64_t *) addr);
    ptr += 8;

    i = 0;
    while (i < 1016)
    {
        if (*ptr == 0xff)
            break;
        ptr++;
        counter++;
        i++;
    }

    return counter;
}

uint32_t get_time_value ()
{

    // uint64_t time = *((uint64_t *)TIME_ADDRESS);
    // return time;
int i, flag = 0;

uint32_t time = 0;

    if (getu32 (TIME_ADDRESS) == 0xffffffff)
        return 0xffffffff;

    for (i = 1; i < 32; i++)
    {
        if (getu32 (TIME_ADDRESS + TIME_OFFSET * i) == 0xffffffff)
        {
            time = getu32 (TIME_ADDRESS + TIME_OFFSET * (i - 1));
            flag = 1;
            break;
        }
    }

    if (!flag)
        time = getu32 (TIME_ADDRESS + TIME_OFFSET * 31);

    if (time != crc (time >> 8))
        return 0;

    // uint8_t *ptr=(uint8_t *)TIME_ADDRESS;

    // for (i=0;i<4;i++){
    // time+=*ptr<<(8*i);
    // ptr++;
    // }

    return time >> 8;

}

uint8_t set_time_value (uint32_t time)
{

int i, flag = 0;

FLASH_Status err = FLASH_COMPLETE;

    FLASH_Unlock ();

    time = crc (time);

    for (i = 0; i < 32; i++)
    {
        if (getu32 (TIME_ADDRESS + TIME_OFFSET * i) == 0xffffffff)
        {
            err = FLASH_ProgramWord (TIME_ADDRESS + TIME_OFFSET * i, (time) & 0xffffffff);
            if (err != FLASH_COMPLETE)
                return err;
            flag = 1;
            break;
        }
    }

    if (!flag)
    {
        err = FLASH_ErasePage (TIME_ADDRESS);
        if (err != FLASH_COMPLETE)
            return err;
        err = FLASH_ProgramWord (TIME_ADDRESS, (time) & 0xffffffff);
        if (err != FLASH_COMPLETE)
            return err;
    }

    // err=FLASH_ProgramWord(TIME_ADDRESS, (time)&0xffffffff);
    // if (err!=FLASH_COMPLETE) return err;
    // err=FLASH_ProgramWord(TIME_ADDRESS+4, (time>>32)&0xffffffff);
    // if (err!=FLASH_COMPLETE) return err;

    FLASH_Lock ();

    return 0;
}


uint8_t set_counter_value (uint32_t addr, uint64_t counter)
{
FLASH_Status err = FLASH_COMPLETE;

    FLASH_Unlock ();

    err = FLASH_ErasePage (addr);
    if (err != FLASH_COMPLETE)
        return err;

    err = FLASH_ProgramWord (addr, counter & 0xffffffff);
    if (err != FLASH_COMPLETE)
        return err;
    err = FLASH_ProgramWord (addr + 4, (counter >> 32) & 0xffffffff);
    if (err != FLASH_COMPLETE)
        return err;

    FLASH_Lock ();

    return 0;
}


/* Increment the HOTP counter stored in flash addr - counter page address */
uint8_t increment_counter_page (uint32_t addr)
{
uint16_t i;

uint8_t* ptr = (uint8_t *) addr;

uint64_t counter;

FLASH_Status err = FLASH_COMPLETE;

    if (ptr[1023] == 0x00)
    {
        // Entire page is filled, erase cycle
        counter = get_counter_value (addr) + 1;


        /*
           err=FLASH_ErasePage(BACKUP_PAGE_ADDRESS); if (err!=FLASH_COMPLETE) return err;

           //write address to backup page err=GEASH_ProgramHalfWord(BACKUP_PAGE_ADDRESS, addr); if (err!=FLASH_COMPLETE) return err;

           err=FLASH_ProgramWord(BACKUP_PAGE_ADDRESS+4, counter&0xffffffff); if (err!=FLASH_COMPLETE) return err;
           err=FLASH_ProgramWord(BACKUP_PAGE_ADDRESS+8, (counter>>32)&0xffffffff); if (err!=FLASH_COMPLETE) return err; */

        FLASH_Unlock ();
        err = FLASH_ErasePage (BACKUP_PAGE_ADDRESS);
        if (err != FLASH_COMPLETE)
            return err;

        // write address to backup page


        err = FLASH_ProgramWord (BACKUP_PAGE_ADDRESS, counter & 0xffffffff);
        if (err != FLASH_COMPLETE)
            return err;
        err = FLASH_ProgramWord (BACKUP_PAGE_ADDRESS + 4, (counter >> 32) & 0xffffffff);
        if (err != FLASH_COMPLETE)
            return err;


        err = FLASH_ProgramWord (BACKUP_PAGE_ADDRESS + BACKUP_ADDRESS_OFFSET, addr);
        if (err != FLASH_COMPLETE)
            return err;

        err = FLASH_ProgramWord (BACKUP_PAGE_ADDRESS + BACKUP_LENGTH_OFFSET, 8);
        if (err != FLASH_COMPLETE)
            return err;



        err = FLASH_ErasePage (addr);
        if (err != FLASH_COMPLETE)
            return err;

        err = FLASH_ProgramWord (addr, counter & 0xffffffff);
        if (err != FLASH_COMPLETE)
            return err;
        err = FLASH_ProgramWord (addr + 4, (counter >> 32) & 0xffffffff);
        if (err != FLASH_COMPLETE)
            return err;


        err = FLASH_ProgramHalfWord (BACKUP_PAGE_ADDRESS + BACKUP_OK_OFFSET, 0x4F4B);
        if (err != FLASH_COMPLETE)
            return err;

        FLASH_Lock ();
    }
    else
    {

        ptr += 8;
        i = 0;
        while (i < 1016)
        {
            if (*ptr == 0xff)
                break;
            ptr++;
            i++;
        }

        FLASH_Unlock ();

        if ((uint32_t) ptr % 2)
        {   // odd byte

            err = FLASH_ProgramHalfWord ((uint32_t) ptr - 1, 0x0000);
            if (err != FLASH_COMPLETE)
                return err;

        }
        else
        {   // even byte

            err = FLASH_ProgramHalfWord ((uint32_t) ptr, 0xff00);
            if (err != FLASH_COMPLETE)
                return err;
        }
        FLASH_Lock ();

    }

    return err; // no error
}

int validate_code_from_hotp_slot(uint8_t slot_number, uint32_t code_to_verify) {
  const int RET_GENERAL_ERROR = -1;
  const int RET_CODE_NOT_VALID = -2;

  uint8_t generated_hotp_code_length = 6;
  FLASH_Status err;
  uint32_t calculated_code;

  if (slot_number >= NUMBER_OF_HOTP_SLOTS)
    return RET_GENERAL_ERROR;

  OTP_slot * hotp_slot = ((OTP_slot *) get_HOTP_slot_offset(slot_number));
  if (hotp_slot->type == SLOT_TYPE_UNPROGRAMMED) // unprogrammed slot_number
    return RET_GENERAL_ERROR;

  if (hotp_slot->use_8_digits)
    generated_hotp_code_length = 8;

  const uint64_t counter = get_counter_value (hotp_slot_counters[slot_number]);

  int counter_offset = 0;
  const int calculate_ahead_values = 10;
  for (counter_offset = 0; counter_offset < calculate_ahead_values; counter_offset++){
    calculated_code = get_hotp_value (counter+counter_offset, hotp_slot->secret, SECRET_LENGTH, generated_hotp_code_length);
    if (calculated_code == code_to_verify) break;
  }

  const bool code_found = counter_offset < calculate_ahead_values;
  if(code_found){
    //increment the counter for the lacking values, plus one to be ready for next validation
    for (int i=0; i<counter_offset+1; i++){
      err = (FLASH_Status) increment_counter_page (hotp_slot_counters[slot_number]);
      if (err != FLASH_COMPLETE) return RET_GENERAL_ERROR;
    }
    return counter_offset;
  }

  return RET_CODE_NOT_VALID;
}


uint32_t get_code_from_hotp_slot (uint8_t slot)
{
uint32_t result;

uint8_t len = 6;

uint64_t counter;

FLASH_Status err;

uint8_t config = 0;

    if (slot >= NUMBER_OF_HOTP_SLOTS)
        return 0;

    config = get_hotp_slot_config (slot);

    if (config & (1 << SLOT_CONFIG_DIGITS))
        len = 8;

    OTP_slot * hotp_slot = ((OTP_slot *) get_HOTP_slot_offset(slot));
    result = hotp_slot->type;
    if (result == 0xFF) // unprogrammed slot
        return 0;

    counter = get_counter_value (hotp_slot_counters[slot]);
    result = get_hotp_value (counter, hotp_slot->secret, SECRET_LENGTH, len);
    err = increment_counter_page (hotp_slot_counters[slot]);
    if (err != FLASH_COMPLETE)
        return 0;

    return result;
}

// backup data to the backup page
// data -data to be backed up
// len - length of the data
// addr - original address of the data
void backup_data (uint8_t * data, uint16_t len, uint32_t addr)
{
FLASH_Status err = FLASH_COMPLETE;

    FLASH_Unlock ();
    FLASH_ErasePage (BACKUP_PAGE_ADDRESS);
    write_data_to_flash (data, len, BACKUP_PAGE_ADDRESS);
    err = FLASH_ProgramHalfWord (BACKUP_PAGE_ADDRESS + BACKUP_LENGTH_OFFSET, len);
    if (err != FLASH_COMPLETE)
    {
    };
    err = FLASH_ProgramWord (BACKUP_PAGE_ADDRESS + BACKUP_ADDRESS_OFFSET, addr);
    if (err != FLASH_COMPLETE)
    {
    }

    FLASH_Lock ();
}

void erase_counter (uint8_t slot)
{
    FLASH_Unlock ();
    FLASH_ErasePage (hotp_slot_counters[slot]);
    FLASH_Lock ();
}


void write_to_slot(OTP_slot *new_slot_data, uint32_t offset, uint16_t len)
{
  FLASH_Status err = FLASH_COMPLETE;

    // choose the proper slot page
  uint32_t current_slot_address;

    if (offset > SLOTS_PAGE2_ADDRESS){
        current_slot_address = SLOTS_PAGE2_ADDRESS;
        offset-= SLOTS_PAGE2_ADDRESS;
    } else {
        current_slot_address = SLOTS_PAGE1_ADDRESS;
        if (offset>SLOTS_PAGE1_ADDRESS){
            offset -= SLOTS_PAGE1_ADDRESS;
        }
    }

    // copy entire page to ram
    uint8_t* page = (uint8_t *) current_slot_address;
    memcpy (page_buffer, page, SLOT_PAGE_SIZE);

    // check if the secret from the tool is empty and if it is use the old
    // secret
    uint8_t* secret = new_slot_data->secret;
    uint8_t empty = TRUE;
    for (int i = 0; i<SECRET_LENGTH; i++) {
      if (secret[i] != 0x00) {
        empty = FALSE;
        break;
      }
    }
    if (empty == TRUE) {
      OTP_slot * stored_otp_slot = (OTP_slot *) (page_buffer + offset);
      memcpy (new_slot_data->secret, stored_otp_slot->secret, SECRET_LENGTH);
    }

    // make changes to page
    memcpy (page_buffer + offset, new_slot_data, len);

    // write page to backup location
    backup_data (page_buffer, SLOT_PAGE_SIZE, current_slot_address);

    // write page to regular location

    FLASH_Unlock ();
    FLASH_ErasePage (current_slot_address);
    write_data_to_flash (page_buffer, SLOT_PAGE_SIZE, current_slot_address);
    err = FLASH_ProgramHalfWord (BACKUP_PAGE_ADDRESS + BACKUP_OK_OFFSET, 0x4F4B);
    if (err != FLASH_COMPLETE)
    {
    };
    FLASH_Lock ();

    StartBlinkingOATHLED (2);
}


// check for any data on the backup page
uint8_t check_backups ()
{

uint32_t address = getu32 ((uint8_t *) BACKUP_PAGE_ADDRESS + BACKUP_ADDRESS_OFFSET);
uint16_t ok = getu16 ((uint8_t *) BACKUP_PAGE_ADDRESS + BACKUP_OK_OFFSET);

uint16_t length = getu16 ((uint8_t *) BACKUP_PAGE_ADDRESS + BACKUP_LENGTH_OFFSET);

    if (ok == 0x4F4B)   // backed up data was correctly written to its
        // destination
        return 0;
    else
    {

        if (address != 0xffffffff && length <= 1000)
        {
            FLASH_Unlock ();

            FLASH_ErasePage (address);
            write_data_to_flash ((uint8_t *) BACKUP_PAGE_ADDRESS, length, address);
            FLASH_ErasePage (BACKUP_PAGE_ADDRESS);
            FLASH_Lock ();


            return 1;   // backed up page restored
        }
        else
            return 2;   // something bad happened, but before the original
        // page was earsed, so we're safe (or there is
        // nothing on the backup page)


    }



}

uint8_t get_hotp_slot_config (uint8_t slot_number)
{
uint8_t result = 0;

    if (slot_number >= NUMBER_OF_HOTP_SLOTS)
        return 0;
    else
    {
      OTP_slot* otp_slot = (OTP_slot *) get_HOTP_slot_offset(slot_number);
      result = otp_slot->config;
    }

    return result;
}

uint8_t get_totp_slot_config (uint8_t slot_number)
{
uint8_t result = 0;

    if (slot_number >= NUMBER_OF_TOTP_SLOTS)
        return 0;
    else
    {
      OTP_slot* otp_slot = (OTP_slot *) get_TOTP_slot_offset(slot_number);
      result = otp_slot->config;
    }

    return result;
}


uint32_t get_code_from_totp_slot (uint8_t slot, uint64_t challenge)
{

uint64_t time;

uint16_t interval;

uint32_t result;

uint8_t config = 0;

uint8_t len = 6;

    if (slot >= NUMBER_OF_TOTP_SLOTS)
        return 0;

    // memset(time,0,18);

    OTP_slot* otp_slot = (OTP_slot *) get_TOTP_slot_offset(slot);

    interval = otp_slot->interval_or_counter;

    time = current_time / interval;

    result = otp_slot->type;
    if (result == 0xFF) // unprogrammed slot
        return 0;

    config = get_totp_slot_config (slot);

    if (config & (1 << SLOT_CONFIG_DIGITS))
        len = 8;

    // result= get_hotp_value(challenge,(uint8_t
    // *)(totp_slots[slot]+SECRET_OFFSET),20,len);
    result = get_hotp_value (time, otp_slot->secret, SECRET_LENGTH, len);

    return result;

}
