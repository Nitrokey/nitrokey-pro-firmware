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
#include "time.h"
#include "password_safe.h"

__IO uint8_t temp_password[25];
__IO uint8_t temp_user_password[25];

__IO struct OTP_slot_content local_slot_content;

int write_to_slot_transaction_started = 0;

bool is_valid_temp_user_password(const uint8_t *user_password);
bool is_valid_admin_temp_password(const uint8_t *password);
bool is_user_PIN_protection_enabled();
bool is_HOTP_slot_number(uint8_t slot_no);
bool is_TOTP_slot_number(uint8_t slot_no);

uint8_t parse_report(uint8_t *report, uint8_t *output) {
  uint8_t cmd_type = report[CMD_TYPE_OFFSET];
  uint32_t received_crc32;
  uint32_t calculated_crc32;
  uint8_t not_authorized = 0;

  received_crc32 = getu32(report + KEYBOARD_FEATURE_COUNT - 4);
  CRC_ResetDR();
  calculated_crc32 = CRC_CalcBlockCRC((uint32_t *) report, KEYBOARD_FEATURE_COUNT / 4 - 1);

  memset(output, 0, KEYBOARD_FEATURE_COUNT);
  output[OUTPUT_CMD_TYPE_OFFSET] = cmd_type;

  output[OUTPUT_CMD_CRC_OFFSET] = calculated_crc32 & 0xFF;
  output[OUTPUT_CMD_CRC_OFFSET + 1] = (calculated_crc32 >> 8) & 0xFF;
  output[OUTPUT_CMD_CRC_OFFSET + 2] = (calculated_crc32 >> 16) & 0xFF;
  output[OUTPUT_CMD_CRC_OFFSET + 3] = (calculated_crc32 >> 24) & 0xFF;

  if (calculated_crc32 == received_crc32) {


    switch (cmd_type) {

      case CMD_GET_STATUS:
        cmd_get_status(report, output);
        break;

      case CMD_WRITE_TO_SLOT_2: {
        struct write_to_slot_2_payload *const payload = (struct write_to_slot_2_payload*) report;
        if(is_valid_admin_temp_password(payload->temporary_admin_password)
            && write_to_slot_transaction_started == 1){
          write_to_slot_transaction_started = 0;
          local_slot_content.slot_number = payload->slot_number;
          local_slot_content.slot_counter = payload->slot_counter;
          memcpy(local_slot_content.slot_name, payload->slot_name, sizeof(payload->slot_name));
          cmd_write_to_slot( (uint8_t*) &local_slot_content, output);
        } else
          not_authorized = 1;
      }
        break;

      case CMD_WRITE_TO_SLOT: {
        struct write_to_slot_1_payload *const payload = (struct write_to_slot_1_payload*) report;
        if(is_valid_admin_temp_password(payload->temporary_admin_password)) {
          write_to_slot_transaction_started = 1;
          memset((void *) &local_slot_content, 0, sizeof(local_slot_content));
          local_slot_content._slot_config = payload->_slot_config;
          memcpy(local_slot_content.slot_token_id, payload->slot_token_id, sizeof(payload->slot_token_id));
          memcpy(local_slot_content.slot_secret, payload->slot_secret, sizeof(payload->slot_secret));
        } else
          not_authorized = 1;
        }
        break;

      case CMD_READ_SLOT_NAME:
        cmd_read_slot_name(report, output);
        break;

      case CMD_READ_SLOT:
        cmd_read_slot(report, output);
        break;

      case CMD_GET_CODE: {
        uint8_t *const user_temp_password = report + CMD_GC_PASSWORD_OFFSET;
        if (!is_user_PIN_protection_enabled() || is_valid_temp_user_password(user_temp_password)) {
              cmd_get_code(report, output);
            } else
              not_authorized = 1;
        }
        break;

      case CMD_WRITE_CONFIG:
        if (is_valid_admin_temp_password(report + 6))
          cmd_write_config(report, output);
        else
          not_authorized = 1;
        break;

      case CMD_ERASE_SLOT:
        if (is_valid_admin_temp_password(report + 2))
          cmd_erase_slot(report, output);
        else
          not_authorized = 1;
        break;

      case CMD_FIRST_AUTHENTICATE:
        cmd_first_authenticate(report, output);
        break;

      case CMD_AUTHORIZE:
        output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_UNKNOWN_COMMAND;
        break;

      case CMD_USER_AUTHORIZE:
        output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_UNKNOWN_COMMAND;
        break;

      case CMD_GET_PASSWORD_RETRY_COUNT:
        cmd_get_password_retry_count(report, output);
        break;

      case CMD_GET_USER_PASSWORD_RETRY_COUNT:
        cmd_get_user_password_retry_count(report, output);
        break;

      case CMD_USER_AUTHENTICATE:
        cmd_user_authenticate(report, output);
        break;

      case CMD_FACTORY_RESET:
        cmd_factory_reset(report, output);
        break;

      case CMD_SET_TIME:
        cmd_set_time(report, output);
        break;

        // Password Safe functions
      case CMD_GET_PW_SAFE_SLOT_STATUS:
        cmd_getPasswordSafeStatus(report, output);
        break;
      case CMD_GET_PW_SAFE_SLOT_NAME:
        cmd_getPasswordSafeSlotName(report, output);
        break;
      case CMD_GET_PW_SAFE_SLOT_PASSWORD:
        cmd_getPasswordSafeSlotPassword(report, output);
        break;
      case CMD_GET_PW_SAFE_SLOT_LOGINNAME:
        cmd_getPasswordSafeSlotLoginName(report, output);
        break;
      case CMD_SET_PW_SAFE_SLOT_DATA_1:
        cmd_setPasswordSafeSetSlotData_1(report, output);
        break;
      case CMD_SET_PW_SAFE_SLOT_DATA_2:
        cmd_setPasswordSafeSetSlotData_2(report, output);
        break;
      case CMD_PW_SAFE_ERASE_SLOT:
        cmd_getPasswordSafeEraseSlot(report, output);
        break;
      case CMD_PW_SAFE_ENABLE:
        cmd_getPasswordSafeEnable(report, output);
        break;
      case CMD_PW_SAFE_INIT_KEY:
        cmd_getPasswordSafeInitKey(report, output);
        break;
      case CMD_PW_SAFE_SEND_DATA:
        cmd_getPasswordSafeSendData(report, output);
        break;


      case CMD_LOCK_DEVICE:
        cmd_lockDevice(report, output);
        break;

      case CMD_DETECT_SC_AES:
        cmd_detectSmartCardAES(report, output);
        break;

      case CMD_NEW_AES_KEY:
        cmd_newAesKey(report, output);
        break;

      case GET_PRO_DEBUG:
        cmd_getProDebug(report, output);
        break;

      case CMD_CHANGE_USER_PIN:
        cmd_change_user_pin(report, output);
        break;

      case CMD_CHANGE_ADMIN_PIN:
        cmd_change_admin_pin(report, output);
        break;

      case CMD_UNLOCK_USER_PASSWORD:
        cmd_unblock_pin(report, output);
        break;

      default:   // Non of the above cases was selected => unknown
        // command
        output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_UNKNOWN_COMMAND;
        break;
    }

    if (not_authorized)
      output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_AUTHORIZED;

  } else
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_CRC;

  CRC_ResetDR();
  calculated_crc32 = CRC_CalcBlockCRC((uint32_t *) output, KEYBOARD_FEATURE_COUNT / 4 - 1);

  output[OUTPUT_CRC_OFFSET] = calculated_crc32 & 0xFF;
  output[OUTPUT_CRC_OFFSET + 1] = (calculated_crc32 >> 8) & 0xFF;
  output[OUTPUT_CRC_OFFSET + 2] = (calculated_crc32 >> 16) & 0xFF;
  output[OUTPUT_CRC_OFFSET + 3] = (calculated_crc32 >> 24) & 0xFF;

  return 0;
}

bool is_user_PIN_protection_enabled() { return *((uint8_t *) (SLOTS_PAGE1_ADDRESS + GLOBAL_CONFIG_OFFSET + 3)) == 1; }

uint8_t cmd_get_status(uint8_t *report, uint8_t *output) {

  output[OUTPUT_CMD_RESULT_OFFSET] = FIRMWARE_VERSION & 0xFF;
  output[OUTPUT_CMD_RESULT_OFFSET + 1] = (FIRMWARE_VERSION >> 8) & 0xFF;
  output[OUTPUT_CMD_RESULT_OFFSET + 2] = cardSerial & 0xFF;
  output[OUTPUT_CMD_RESULT_OFFSET + 3] = (cardSerial >> 8) & 0xFF;
  output[OUTPUT_CMD_RESULT_OFFSET + 4] = (cardSerial >> 16) & 0xFF;
  output[OUTPUT_CMD_RESULT_OFFSET + 5] = (cardSerial >> 24) & 0xFF;
  memcpy(output + OUTPUT_CMD_RESULT_OFFSET + 6, (uint8_t *) SLOTS_PAGE1_ADDRESS + GLOBAL_CONFIG_OFFSET, 3);
  memcpy(output + OUTPUT_CMD_RESULT_OFFSET + 9, (uint8_t *) SLOTS_PAGE1_ADDRESS + GLOBAL_CONFIG_OFFSET + 3, 2);

  return 0;
}


uint8_t cmd_get_password_retry_count(uint8_t *report, uint8_t *output) {

  output[OUTPUT_CMD_RESULT_OFFSET] = getPasswordRetryCount();

  return 0;
}

uint8_t cmd_get_user_password_retry_count(uint8_t *report, uint8_t *output) {

  output[OUTPUT_CMD_RESULT_OFFSET] = getUserPasswordRetryCount();

  return 0;
}


uint8_t cmd_write_to_slot(uint8_t *report, uint8_t *output) {
  uint8_t slot_no = report[CMD_WTS_SLOT_NUMBER_OFFSET];
  uint8_t slot_tmp[64];           // this is will be the new slot contents

  memset(slot_tmp, 0, 64);
  slot_tmp[0] = 0x01; // marks slot as programmed
  uint8_t *slot_name = report + CMD_WTS_SLOT_NAME_OFFSET;
  memcpy(slot_tmp + 1, slot_name, 51);

  if (slot_name[0] == 0) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NO_NAME_ERROR;
    return 1;
  }

  if (is_HOTP_slot_number(slot_no)) {
    slot_no = slot_no & 0x0F;
    uint64_t counter = getu64(report + CMD_WTS_COUNTER_OFFSET);
    set_counter_value(hotp_slot_counters[slot_no], counter);
    write_to_slot(slot_tmp, hotp_slot_offsets[slot_no], 64);

  } else if (is_TOTP_slot_number(slot_no)) {
    slot_no = slot_no & 0x0F;
    write_to_slot(slot_tmp, totp_slot_offsets[slot_no], 64);

  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_SLOT;
  }


  return 0;
}

bool is_TOTP_slot_number(uint8_t slot_no) { return slot_no >= 0x20 && slot_no < 0x20 + NUMBER_OF_TOTP_SLOTS; }

bool is_HOTP_slot_number(uint8_t slot_no) { return slot_no >= 0x10 && slot_no < 0x10 + NUMBER_OF_HOTP_SLOTS; }


uint8_t cmd_read_slot_name(uint8_t *report, uint8_t *output) {

  uint8_t slot_no = report[1];

  uint64_t counter;

  if (is_HOTP_slot_number(slot_no)) {   // HOTP slot
    slot_no = slot_no & 0x0F;
    uint8_t is_programmed = *((uint8_t *) (hotp_slots[slot_no]));

    if (is_programmed == 0x01) {
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET, (uint8_t *) (hotp_slots[slot_no] + SLOT_NAME_OFFSET), 15);
    } else {
      output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_SLOT_NOT_PROGRAMMED;
    }

  } else if (is_TOTP_slot_number(slot_no)) {   // TOTP slot
    slot_no = slot_no & 0x0F;
    uint8_t is_programmed = *((uint8_t *) (totp_slots[slot_no]));

    if (is_programmed == 0x01) {
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET, (uint8_t *) (totp_slots[slot_no] + SLOT_NAME_OFFSET), 15);
    } else {
      output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_SLOT_NOT_PROGRAMMED;
    }

  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_SLOT;

  }


  return 0;
}


uint8_t cmd_read_slot(uint8_t *report, uint8_t *output) {

  uint8_t slot_no = report[1];

  uint64_t counter;

  if (is_HOTP_slot_number(slot_no)) {   // HOTP slot
    slot_no = slot_no & 0x0F;
    uint8_t is_programmed = *((uint8_t *) (hotp_slots[slot_no]));

    if (is_programmed == 0x01) {
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET, (uint8_t *) (hotp_slots[slot_no] + SLOT_NAME_OFFSET), 15);
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET + 15, (uint8_t *) (hotp_slots[slot_no] + CONFIG_OFFSET), 1);
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET + 16, (uint8_t *) (hotp_slots[slot_no] + TOKEN_ID_OFFSET), 13);
      counter = get_counter_value(hotp_slot_counters[slot_no]);
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET + 29, &counter, 8);
    } else {
      output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_SLOT_NOT_PROGRAMMED;
    }

  } else if (is_TOTP_slot_number(slot_no)) {   // TOTP slot
    slot_no = slot_no & 0x0F;
    uint8_t is_programmed = *((uint8_t *) (totp_slots[slot_no]));

    if (is_programmed == 0x01) {
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET, (uint8_t *) (totp_slots[slot_no] + SLOT_NAME_OFFSET), 15);
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET + 15, (uint8_t *) (totp_slots[slot_no] + CONFIG_OFFSET), 1);
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET + 16, (uint8_t *) (totp_slots[slot_no] + TOKEN_ID_OFFSET), 13);
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET + 29, (uint8_t *) (totp_slots[slot_no] + INTERVAL_OFFSET), 2);
    } else {
      output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_SLOT_NOT_PROGRAMMED;
    }

  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_SLOT;

  }


  return 0;
}


uint8_t cmd_get_code(uint8_t *report, uint8_t *output) {

  uint64_t challenge = getu64(report + CMD_GC_CHALLENGE_OFFSET);

  uint32_t result = 0;


  uint8_t slot_no = report[CMD_GC_SLOT_NUMBER_OFFSET];

  if (is_HOTP_slot_number(slot_no)) {   // HOTP slot
    slot_no = slot_no & 0x0F;
    uint8_t is_programmed = *((uint8_t *) (hotp_slots[slot_no]));

    if (is_programmed == 0x01) {

      result = get_code_from_hotp_slot(slot_no);
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET, &result, 4);
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET + 4, (uint8_t *) hotp_slots[slot_no] + CONFIG_OFFSET, 14);
    } else
      output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_SLOT_NOT_PROGRAMMED;

  } else if (is_TOTP_slot_number(slot_no)) {   // TOTP slot
    slot_no = slot_no & 0x0F;
    uint8_t is_programmed = *((uint8_t *) (totp_slots[slot_no]));

    if (is_programmed == 0x01) {
      result = get_code_from_totp_slot(slot_no, challenge);
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET, &result, 4);
      memcpy(output + OUTPUT_CMD_RESULT_OFFSET + 4, (uint8_t *) totp_slots[slot_no] + CONFIG_OFFSET, 14);
    } else
      output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_SLOT_NOT_PROGRAMMED;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_SLOT;
  }

  return 0;
}


uint8_t cmd_write_config(uint8_t *report, uint8_t *output) {

  uint8_t slot_tmp[64];           // this will be the new slot contents

  memset(slot_tmp, 0, 64);

  memcpy(slot_tmp, report + 1, 5);

  write_to_slot(slot_tmp, GLOBAL_CONFIG_OFFSET, 64);

  return 0;

}


uint8_t cmd_erase_slot(uint8_t *report, uint8_t *output) {

  uint8_t slot_no = report[CMD_WTS_SLOT_NUMBER_OFFSET];

  uint8_t slot_tmp[64];

  memset(slot_tmp, 0xFF, 64);


  if (is_HOTP_slot_number(slot_no))  // HOTP
    // slot
  {
    slot_no = slot_no & 0x0F;
    write_to_slot(slot_tmp, hotp_slot_offsets[slot_no], 64);
    erase_counter(slot_no);
  } else if (is_TOTP_slot_number(slot_no)) // TOTP
    // slot
  {
    slot_no = slot_no & 0x0F;
    write_to_slot(slot_tmp, totp_slot_offsets[slot_no], 64);
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_SLOT;
  }
  return 0;
}

uint8_t cmd_first_authenticate(uint8_t *report, uint8_t *output) {

  uint8_t res = 1;

  uint8_t card_password[26];

  memset(card_password, 0, 26);
  memcpy(card_password, report + 1, 25);

  res = cardAuthenticate(card_password);
  memset(card_password, 0, sizeof(card_password));

  if (res == TRUE) {
    memcpy(temp_password, report + 26, 25);
    getAID();
    return 0;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_PASSWORD;
    return 1;   // wrong card password
  }
}

uint8_t cmd_user_authenticate(uint8_t *report, uint8_t *output) {

  uint8_t res = 1;

  uint8_t user_password[26];

  memset(user_password, 0, 26);
  memcpy(user_password, report + 1, 25);

  res = userAuthenticate(user_password);
  memset(user_password, 0, sizeof(user_password));

  if (res == 0) {
    memcpy(temp_user_password, report + 26, 25);
    getAID();
    return 0;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_PASSWORD;
    return 1;   // wrong card password
  }
}


uint8_t cmd_change_user_pin(uint8_t *report, uint8_t *output) {
  uint8_t res = 1;

  uint8_t old_pin[26];

  uint8_t new_pin[26];

  memset(old_pin, 0, 26);
  memset(new_pin, 0, 26);

  memcpy(old_pin, report + 1, 25);
  memcpy(new_pin, report + 26, 25);

  res = userAuthenticate(old_pin);
  if (res != 0) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_PASSWORD;
    return 1;   // wrong card password
  }

  res = changeUserPin(old_pin, new_pin);
  memset(old_pin, 0, sizeof(old_pin));
  memset(new_pin, 0, sizeof(new_pin));

  if (0 == res) {
    return 0;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_ERROR_CHANGING_USER_PASSWORD;
    return 1;
  }
}

uint8_t cmd_change_admin_pin(uint8_t *report, uint8_t *output) {
  uint8_t res = 1;

  uint8_t old_admin_pin[26];

  uint8_t new_admin_pin[26];

  memset(old_admin_pin, 0, 26);
  memset(new_admin_pin, 0, 26);

  memcpy(old_admin_pin, report + 1, 25);
  memcpy(new_admin_pin, report + 26, 25);

  res = cardAuthenticate(old_admin_pin);
  if (res != TRUE) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_PASSWORD;
    return 1;
  }

  res = changeAdminPin(old_admin_pin, new_admin_pin);
  memset(old_admin_pin, 0, sizeof(old_admin_pin));
  memset(old_admin_pin, 0, sizeof(new_admin_pin));

  if (0 == res) {
    return 0;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_ERROR_CHANGING_ADMIN_PASSWORD;
    return 1;
  }
}

uint8_t cmd_unblock_pin(uint8_t *report, uint8_t *output) {
  uint8_t res = 1;

  uint8_t admin_pin[25];

  uint8_t new_pin[25];

  memset(admin_pin, 0, 25);
  memcpy(admin_pin, report + 1, 25);

  memset(new_pin, 0, 25);
  memcpy(new_pin, &(report[26]), 25);


  res = cardAuthenticate(admin_pin);
  memset(admin_pin, 0, sizeof(admin_pin));

  if (res != TRUE) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_PASSWORD;
    return 1;
  }

  res = unblockPin(new_pin);
  memset(new_pin, 0, sizeof(new_pin));
  if (0 == res) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
    return 0;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_ERROR_UNBLOCKING_PIN;
    return 1;
  }
}

bool is_valid_admin_temp_password(const uint8_t *password) { return memcmp(password, temp_password, 25) == 0; }

bool is_valid_temp_user_password(const uint8_t *user_password) { return memcmp(user_password, temp_user_password, 25) == 0; }


uint8_t cmd_factory_reset(uint8_t *report, uint8_t *output) {
  uint8_t res = 1;

  uint8_t admin_password[26];

  memset(admin_password, 0, 26);
  memcpy(admin_password, report + 1, 25);

  res = factoryReset(admin_password);

  if (0 == res) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
    return 0;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_PASSWORD;
    return 1;
  }
}

uint8_t cmd_set_time(uint8_t *report, uint8_t *output) {

  int err;

  uint64_t new_time = (getu64(report + CMD_DATA_OFFSET + 1));

  uint32_t old_time = get_time_value();

  uint32_t new_time_minutes = (new_time - 1388534400) / 60;

  if (0 == report[CMD_DATA_OFFSET])   // Check valid time only at check
    // time
  {
    if (old_time == 0) {
      output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_TIMESTAMP_WARNING;
      return 1;
    }
  }

  if (old_time <= new_time_minutes || old_time == 0xffffffff || *((uint8_t *) (report + CMD_DATA_OFFSET)) == 1) {
    current_time = new_time;
    err = set_time_value(new_time_minutes);
    if (err) {
      output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_TIMESTAMP_WARNING;
      return 1;
    }
    return 0;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_TIMESTAMP_WARNING;
    return 1;
  }

}


uint8_t cmd_getPasswordSafeStatus(uint8_t *report, uint8_t *output) {
  uint8_t Data_u8[PWS_SLOT_COUNT];

  uint32_t Ret_u32;

  Ret_u32 = PWS_GetAllSlotStatus(Data_u8);
  if (TRUE == Ret_u32) {
    memcpy(&output[OUTPUT_CMD_RESULT_OFFSET], Data_u8, PWS_SLOT_COUNT);
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_AUTHORIZED;
  }

  return (0);
}

uint8_t cmd_getPasswordSafeSlotName(uint8_t *report, uint8_t *output) {
  uint32_t Ret_u32;

  Ret_u32 = PWS_GetSlotName(report[1], &output[OUTPUT_CMD_RESULT_OFFSET]);
  if (TRUE == Ret_u32) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_AUTHORIZED;
  }

  return (0);
}

uint8_t cmd_getPasswordSafeSlotPassword(uint8_t *report, uint8_t *output) {
  uint32_t Ret_u32;

  Ret_u32 = PWS_GetSlotPassword(report[1], &output[OUTPUT_CMD_RESULT_OFFSET]);
  if (TRUE == Ret_u32) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_AUTHORIZED;
  }

  return (0);
}

uint8_t cmd_getPasswordSafeSlotLoginName(uint8_t *report, uint8_t *output) {
  uint32_t Ret_u32;

  Ret_u32 = PWS_GetSlotLoginName(report[1], &output[OUTPUT_CMD_RESULT_OFFSET]);
  if (TRUE == Ret_u32) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_AUTHORIZED;
  }

  return (0);
}

uint8_t cmd_setPasswordSafeSetSlotData_1(uint8_t *report, uint8_t *output) {
  uint32_t Ret_u32;

  // Slot name, Slot password. Don't write it into flash
  Ret_u32 = PWS_WriteSlotData_1(report[1], &report[2], &report[2 + PWS_SLOTNAME_LENGTH]);

  if (TRUE == Ret_u32) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_AUTHORIZED;
  }

  return (0);
}

uint8_t cmd_setPasswordSafeSetSlotData_2(uint8_t *report, uint8_t *output) {
  u32 Ret_u32;

  // Slot login name and write to flash
  Ret_u32 = PWS_WriteSlotData_2(report[1], &report[2]);

  if (TRUE == Ret_u32) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_AUTHORIZED;
  }

  return (0);
}

uint8_t cmd_getPasswordSafeEraseSlot(uint8_t *report, uint8_t *output) {
  u32 Ret_u32;

  Ret_u32 = PWS_EraseSlot(report[1]);
  if (TRUE == Ret_u32) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_AUTHORIZED;
  }

  return (0);
}

uint8_t cmd_getPasswordSafeEnable(uint8_t *report, uint8_t *output) {
  u8 ret;

  ret = PWS_EnableAccess(&report[1]);
  output[OUTPUT_CMD_STATUS_OFFSET] = ret;
  return 0;
}

uint8_t cmd_getPasswordSafeInitKey(uint8_t *report, uint8_t *output) {
  u32 Ret_u32;

  Ret_u32 = PWS_InitKey();
  if (TRUE == Ret_u32) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_AUTHORIZED;
  }

  return (0);
}

uint8_t cmd_getPasswordSafeSendData(uint8_t *report, uint8_t *output) {
  u32 Ret_u32;

  Ret_u32 = PWS_SendData(report[1], report[2]);
  if (TRUE == Ret_u32) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_AUTHORIZED;
  }

  return (0);
}

uint8_t cmd_detectSmartCardAES(uint8_t *report, uint8_t *output) {
  unsigned short ret;

  unsigned char user_password[26];

  memset(user_password, 0, 26);
  memcpy(user_password, report + 1, 25);

  ret = CcidVerifyPin(2, user_password);
  if (APDU_ANSWER_COMMAND_CORRECT != ret) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_WRONG_PASSWORD;
    return (0);
  }

  ret = isAesSupported();

  if (TRUE == ret) {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  } else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_NOT_SUPPORTED;
  }
  return (0);
}

uint8_t cmd_newAesKey(uint8_t *report, uint8_t *output) {
  u32 ret;

  unsigned char admin_password[26];
  memset(admin_password, 0, 26);
  memcpy(admin_password, report + 1, 25);

  ret = BuildPasswordSafeKey_u32();
  if (TRUE == ret)
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  else {
    output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_AES_CREATE_KEY_FAILED;
    return 0;
  }

  // ret = sendAESMasterKey (AES_KEYSIZE_256_BIT, MasterKey_pu8);
#define AES_KEYSIZE_256_BIT     32  // 32 * 8 = 256
  u8 MasterKey_au8[AES_KEYSIZE_256_BIT];
  ret = BuildNewAesMasterKey_u32(admin_password, MasterKey_au8);
  // ret = BuildStorageKeys_u32 (admin_password);
  output[OUTPUT_CMD_STATUS_OFFSET] = ret;
  return (0);
}


uint8_t cmd_getProDebug(uint8_t *report, uint8_t *output) {
  u32 ret;

  unsigned char data[OUTPUT_CMD_RESULT_LENGTH];

  unsigned int data_length = 0;

  GetDebug(data, &data_length);

  output[OUTPUT_CMD_STATUS_OFFSET] = CMD_STATUS_OK;
  memcpy(output + OUTPUT_CMD_RESULT_OFFSET, data, data_length);
  return (0);
}


uint8_t cmd_lockDevice(uint8_t *report, uint8_t *output) {
  // Disable password safe
  PWS_DisableKey();
  return 0;
}


// START - OTP Test Routine --------------------------------
/*
   uint8_t cmd_test_counter(uint8_t *report,uint8_t *output){

   int i; uint8_t slot_no = report[CMD_DATA_OFFSET]; uint16_t tests_number = getu16(report+CMD_DATA_OFFSET+1); uint16_t results = 0; uint64_t counter
   = get_counter_value(hotp_slot_counters[slot_no]); uint64_t counter_new = 0;

   for(i=0;i<tests_number;i++){ set_counter_vao],(hotp_slot_counters[slot_no], counter); counter_new=get_counter_value(hotp_slot_counters[slot_no]);
   if(counter == counter_new){ results++; } }

   memcpy(output+OUTPUT_CMD_RESULT_OFFSET,&results,2);

   return 0; }

   uint8_t cmd_test_time(uint8_t *report,uint8_t *output){

   int i,err; uint32_t time; uint32_t read_time; uint16_t tests_number = getu16(report+CMD_DATA_OFFSET+1); uint16_t results = 0;

   for(i=0;i<tests_number;i++){ time = (current_time - 1388534400)/60; err = set_time_value(time); read_time = get_time_value(); if(!err && read_time
   == time) { results++; } }

   memcpy(output+OUTPUT_CMD_RESULT_OFFSET,&results,2);

   return 0; } */
// END - OTP Test Routine ----------------------------------
