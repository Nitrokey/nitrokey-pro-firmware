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


#include <stdint.h>
#include "parse_tlv.h"
#include <string.h>
#include <stdbool.h>

static uint8_t depth = 0;

static bool is_constructed(uint8_t tag_1st_byte){
        return (tag_1st_byte & (1<<5)) > 0;
}

uint32_t parse_tlv(uint8_t *data, const uint8_t *data_end, int (*tag_callback)(TLV *tlv, uint16_t depth)){
        uint8_t tag_length = 1;
        uint32_t position = 0;

        struct TLV tlv = {};

        if (depth++ > 100) {
                return 0;
        }
        if (data_end-data < 2){
                return 0;
        }
        if (data>=data_end){
                return 0;
        }

        // Determine tag
        for(uint32_t i=0; data+ i <data_end; i++){
                // Check if there are optional bytes
                if (i == 0 && (data[i] & 0x1F) != 0x1F) // 0x1F -> b0001_1111
                        break;
                // Check if there are another optional bytes
                if (i != 0 && (data[i] & 0x80) != 0x80) // 0x80 -> b1000_0000
                        break;

                // another tag byte
                tag_length++;
        }
        memmove(tlv.tag, data, tag_length);
        tlv.tag_constructed = is_constructed(tlv.tag[0]);
        tlv.tag_length = tag_length;
        position += tag_length;

        // Determine length
        if ( (data[position] & 0x80) != 0x80 ){  // 0x80 -> b1000_0000
                tlv.length = data[position];
                position++;
        } else {
                uint32_t length_field_len = 0;
                length_field_len = data[position] - 0x80u;
                if(length_field_len > 4){
                        return 0;
                }

                tlv.length = 0;
                position++;

                if(length_field_len==1){
                        tlv.length = data[position];
                        position++;
                } else {
                        // FIXME handle longer fields
                        return 0;
                }
        }
        memmove(tlv.value, data+position, tlv.length);


        position += tlv.length;

        int res = tag_callback(&tlv, depth);
        if (res)
                return 1;

        if (tlv.tag_constructed) {
                // Run it on value
                uint32_t offset = 0;
                uint32_t p = 0;
                for(;;){
                        p = parse_tlv(tlv.value + offset, tlv.value + tlv.length, tag_callback);
                        if (p==0) break;
                        if (p==1) return 1;
                        offset += p;
                }
        }

        depth--;

        return position;
}