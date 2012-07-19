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

#define NUMBER_OF_SLOTS 2

//Flash memory pages:
//0x801E800
//0x801EC00 <- slot 1
//0x801F000 <- slot 1 counter
//0x801F400 <- slot 2
//0x801F800 <- slot 2 counter
//0x801FC00 <- backup page

#define SLOT1_ADDRESS 0x801EC00
#define SLOT2_ADDRESS 0x801F400
#define BACKUP_PAGE_ADDRESS 0x801FC00

#define SLOT_TYPE_OFFSET 0x00
#define SECRET_LENGTH_OFFSET 0x01
#define SECRET_OFFSET 0x03EB
#define COUNTER_PAGE_OFFSET 0x400

uint64_t endian_swap (uint64_t x);
uint32_t dynamic_truncate (uint8_t * hmac_result);
void write_data_to_flash(uint8_t *data,uint8_t len,uint32_t addr);
uint32_t get_htop_value(uint64_t counter,uint8_t * secret,uint8_t secret_length,uint8_t len);
uint32_t get_counter_value(uint32_t addr);
uint32_t get_code_from_slot(uint8_t slot);
uint8_t increment_counter_page(uint32_t addr);