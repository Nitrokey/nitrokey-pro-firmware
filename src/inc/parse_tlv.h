/*
 * Author: Copyright (C) Nitrokey UG 2019
 *
 * This file is part of Nitrokey Pro firmware.
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


#ifndef TLV_PARSE_TLV_H
#define TLV_PARSE_TLV_H

#include <stdbool.h>


#define TAG_LENGTH      4
#define LEN_VALUE_LENGTH      4
#define VALUE_LENGTH      512

typedef struct TLV{
        uint8_t tag[TAG_LENGTH];
        uint8_t tag_length;
        bool tag_constructed;
        uint32_t length;
        uint8_t value[VALUE_LENGTH];
} TLV;

uint32_t parse_tlv(uint8_t *data, const uint8_t *data_end, int (*tag_callback)(TLV *tlv, uint16_t depth));




#endif //TLV_PARSE_TLV_H
