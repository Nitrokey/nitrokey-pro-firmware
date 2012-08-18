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

#define NUMBER_OF_HOTP_SLOTS 2
#define NUMBER_OF_TOTP_SLOTS 4

//Flash memory pages:
//0x801E800
//0x801EC00 <- slots
//0x801F000 <- slot 1 counter
//0x801F400 <- slot 2 counter
//0x801F800
//0x801FC00 <- backup page


/*
slot structure:
1b	0x01 if slot is used (programmed)
15b slot name
20b secret
1b configuration flags:
	MSB [x|x|x|x|x|send token id|send enter after code?|no. of digits 6/8] LSB
12b token id
1b keyboard layout

*/

#define SLOT_CONFIG_DIGITS 0
#define SLOT_CONFIG_ENTER 1
#define SLOT_CONFIG_TOKENID 2

/*
global config slot:

1b slot sent after numlock 
1b slot sent after caps lock
1b slot sent after scroll lock

*/

#define SLOT_PAGE_SIZE 1000 //less than actual page, so we can copy it to backup page with additional info

#define SLOTS_ADDRESS 0x801EC00
#define SLOT1_COUNTER_ADDRESS 0x801F000
#define SLOT2_COUNTER_ADDRESS 0x801F400
#define BACKUP_PAGE_ADDRESS 0x801FC00

#define BACKUP_ADDRESS_OFFSET 1000
#define BACKUP_LENGTH_OFFSET 1004
#define BACKUP_OK_OFFSET 1006

#define GLOBAL_CONFIG_OFFSET 0

#define HOTP_SLOT1_OFFSET 64
#define HOTP_SLOT2_OFFSET 128

#define TOTP_SLOT1_OFFSET 192
#define TOTP_SLOT2_OFFSET 256
#define TOTP_SLOT3_OFFSET 320
#define TOTP_SLOT4_OFFSET 384

#define SLOT_TYPE_OFFSET 0
#define SLOT_NAME_OFFSET 1
#define SECRET_OFFSET 16
#define CONFIG_OFFSET 36
#define TOKEN_ID_OFFSET 37




extern __I uint32_t hotp_slots[NUMBER_OF_HOTP_SLOTS];
extern __I uint32_t totp_slots[NUMBER_OF_TOTP_SLOTS];
extern __I uint32_t hotp_slot_counters[NUMBER_OF_HOTP_SLOTS];
extern __I uint32_t hotp_slot_offsets[NUMBER_OF_HOTP_SLOTS];
extern __I uint32_t totp_slot_offsets[NUMBER_OF_TOTP_SLOTS];

extern uint8_t page_buffer[SLOT_PAGE_SIZE];


uint64_t endian_swap (uint64_t x);
uint32_t dynamic_truncate (uint8_t * hmac_result);
void write_data_to_flash(uint8_t *data,uint16_t len,uint32_t addr);
uint32_t get_hotp_value(uint64_t counter,uint8_t * secret,uint8_t secret_length,uint8_t len);
uint64_t get_counter_value(uint32_t addr);
uint8_t set_counter_value(uint32_t addr, uint64_t counter);
uint32_t get_code_from_hotp_slot(uint8_t slot);
uint8_t increment_counter_page(uint32_t addr);
void write_to_slot(uint8_t *data, uint16_t offset, uint16_t len);
void backup_data(uint8_t *data,uint8_t len, uint32_t addr);
uint8_t check_backups(void);
uint8_t get_hotp_slot_config(uint8_t slot_number);
uint32_t get_code_from_totp_slot(uint8_t slot, uint64_t challenge);
