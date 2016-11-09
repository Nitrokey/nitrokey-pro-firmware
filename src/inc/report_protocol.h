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

#define FIRMWARE_VERSION (0x08)

#define CMD_GET_STATUS                      0x00
#define CMD_WRITE_TO_SLOT                   0x01
#define CMD_READ_SLOT_NAME                  0x02
#define CMD_READ_SLOT                       0x03
#define CMD_GET_CODE                        0x04
#define CMD_WRITE_CONFIG                    0x05
#define CMD_ERASE_SLOT                      0x06
#define CMD_FIRST_AUTHENTICATE              0x07
#define CMD_AUTHORIZE                       0x08
#define CMD_GET_PASSWORD_RETRY_COUNT        0x09
#define CMD_SET_TIME                        0x0B
#define CMD_TEST_COUNTER                    0x0C
#define CMD_TEST_TIME                       0x0D
#define CMD_USER_AUTHENTICATE               0x0E
#define CMD_GET_USER_PASSWORD_RETRY_COUNT   0x0F
#define CMD_USER_AUTHORIZE                  0x10
#define CMD_UNLOCK_USER_PASSWORD            0x11
#define CMD_LOCK_DEVICE                     0x12
#define CMD_FACTORY_RESET                   0x13
#define CMD_CHANGE_USER_PIN                 0x14
#define CMD_CHANGE_ADMIN_PIN                0x15
#define CMD_WRITE_TO_SLOT_2                 0x16

#define CMD_GET_PW_SAFE_SLOT_STATUS       0x60
#define CMD_GET_PW_SAFE_SLOT_NAME         0x61
#define CMD_GET_PW_SAFE_SLOT_PASSWORD     0x62
#define CMD_GET_PW_SAFE_SLOT_LOGINNAME    0x63
#define CMD_SET_PW_SAFE_SLOT_DATA_1       0x64
#define CMD_SET_PW_SAFE_SLOT_DATA_2       0x65
#define CMD_PW_SAFE_ERASE_SLOT            0x66
#define CMD_PW_SAFE_ENABLE                0x67
#define CMD_PW_SAFE_INIT_KEY              0x68
#define CMD_PW_SAFE_SEND_DATA             0x69

#define CMD_DETECT_SC_AES                 0x6a
#define CMD_NEW_AES_KEY                   0x6b
#define GET_PRO_DEBUG                     0x6c

#define CMD_DATA_OFFSET                   0x01

#define STATUS_READY                      0x00
#define STATUS_BUSY                       0x01
#define STATUS_ERROR                      0x02
#define STATUS_RECEIVED_REPORT            0x03

#define CMD_STATUS_OK                               0
#define CMD_STATUS_WRONG_CRC                        1
#define CMD_STATUS_WRONG_SLOT                       2
#define CMD_STATUS_SLOT_NOT_PROGRAMMED              3
#define CMD_STATUS_WRONG_PASSWORD                   4
#define CMD_STATUS_NOT_AUTHORIZED                   5
#define CMD_STATUS_TIMESTAMP_WARNING                6
#define CMD_STATUS_NO_NAME_ERROR                    7
#define CMD_STATUS_NOT_SUPPORTED                    8
#define CMD_STATUS_UNKNOWN_COMMAND                  9
#define CMD_STATUS_AES_DEC_FAILED                   10
#define CMD_STATUS_AES_CREATE_KEY_FAILED            11
#define CMD_STATUS_ERROR_CHANGING_USER_PASSWORD     12
#define CMD_STATUS_ERROR_CHANGING_ADMIN_PASSWORD    13
#define CMD_STATUS_ERROR_UNBLOCKING_PIN             14

/*
   Output report size offset description 1 0 device status 1 1 last command's type 4 2 last command's CRC 1 6 last command's status 53 7 last
   command's output 4 60 this report's CRC (with device status equal 0) */

#define OUTPUT_CMD_TYPE_OFFSET      1
#define OUTPUT_CMD_CRC_OFFSET       2
#define OUTPUT_CMD_STATUS_OFFSET    6
#define OUTPUT_CMD_RESULT_OFFSET    7
#define OUTPUT_CMD_RESULT_LENGTH    53
#define OUTPUT_CRC_OFFSET           60


#define CMD_TYPE_OFFSET             0


/*
   CMD_WRITE_TO_SLOT

   1b command type 1b slot number 15b slot name 20b secret 1b configuration flags 12b token id 1b keyboard layout 8b counter */

#define CMD_WTS_SLOT_NUMBER_OFFSET    1
#define CMD_WTS_SLOT_NAME_OFFSET      2
#define CMD_WTS_SECRET_OFFSET         17
#define CMD_WTS_CONFIG_OFFSET         37
#define CMD_WTS_TOKEN_ID_OFFSET       38
#define CMD_WTS_COUNTER_OFFSET        51


/*
   CMD_READ_SLOT

   1b command type 1b slot number

 */

#define CMD_RS_SLOT_NUMBER_OFFSET     1
#define	CMD_RS_OUTPUT_COUNTER_OFFSET  34

/*
   CMD_GET_CODE

   report: 1b command type 1b slot number 8b challenge (for TOTP slot only)

   output: 4b generated OTP 1b config flags 13b tokenID

 */

#define CMD_GC_PASSWORD_OFFSET      1
#define CMD_GC_SLOT_NUMBER_OFFSET   (CMD_GC_PASSWORD_OFFSET + 25)
#define CMD_GC_CHALLENGE_OFFSET     (CMD_GC_PASSWORD_OFFSET + 25 + 1)

/*
 * CMD_GET_PASSWORD_RETRY_COUNT
 *
 * output:
 * 1b number of password tries left
 */


/*
   CMD_WRITE_CONFIG

   report: 1b command type 1b Numlock slot 1b Capslock slot 1b Scrolllock slot

   output:


 */


/*
   CMD_FIRST_AUTHENTICATE

   report: 1b command type 25b card password 25b new temporary password

 */



__IO extern uint8_t device_status;

//extern __IO uint8_t temp_password[25];
//extern uint8_t temp_password[25];

uint8_t parse_report (uint8_t * report, uint8_t * output);

uint8_t cmd_get_status (uint8_t * report, uint8_t * output);

uint8_t cmd_write_to_slot (uint8_t * report, uint8_t * output);

uint8_t cmd_read_slot_name (uint8_t * report, uint8_t * output);

uint8_t cmd_read_slot (uint8_t * report, uint8_t * output);

uint8_t cmd_get_code (uint8_t * report, uint8_t * output);

uint8_t cmd_write_config (uint8_t * report, uint8_t * output);

uint8_t cmd_erase_slot (uint8_t * report, uint8_t * output);

uint8_t cmd_first_authenticate (uint8_t * report, uint8_t * output);

uint8_t cmd_get_password_retry_count (uint8_t * report, uint8_t * output);

uint8_t cmd_user_authenticate (uint8_t * report, uint8_t * output);

uint8_t cmd_factory_reset (uint8_t * report, uint8_t * output);

uint8_t cmd_get_user_password_retry_count (uint8_t * report, uint8_t * output);
uint8_t cmd_set_time (uint8_t * report, uint8_t * output);

uint8_t cmd_getPasswordSafeStatus (uint8_t * report, uint8_t * output);

uint8_t cmd_getPasswordSafeSlotName (uint8_t * report, uint8_t * output);

uint8_t cmd_getPasswordSafeSlotPassword (uint8_t * report, uint8_t * output);

uint8_t cmd_getPasswordSafeSlotLoginName (uint8_t * report, uint8_t * output);

uint8_t cmd_setPasswordSafeSetSlotData_1 (uint8_t * report, uint8_t * output);

uint8_t cmd_setPasswordSafeSetSlotData_2 (uint8_t * report, uint8_t * output);

uint8_t cmd_getPasswordSafeEraseSlot (uint8_t * report, uint8_t * output);

uint8_t cmd_getPasswordSafeEnable (uint8_t * report, uint8_t * output);

uint8_t cmd_getPasswordSafeInitKey (uint8_t * report, uint8_t * output);

uint8_t cmd_getPasswordSafeSendData (uint8_t * report, uint8_t * output);

uint8_t cmd_detectSmartCardAES (uint8_t * report, uint8_t * output);

uint8_t cmd_newAesKey (uint8_t * report, uint8_t * output);

uint8_t cmd_lockDevice (uint8_t * report, uint8_t * output);

uint8_t cmd_change_user_pin (uint8_t * report, uint8_t * output);

uint8_t cmd_change_admin_pin (uint8_t * report, uint8_t * output);

uint8_t cmd_unblock_pin (uint8_t * report, uint8_t * output);


uint8_t cmd_getProDebug (uint8_t * report, uint8_t * output);

// START - OTP Test Routine --------------------------------
/*
   uint8_t cmd_test_counter(uint8_t *report,uint8_t *output); uint8_t cmd_test_time(uint8_t *report,uint8_t *output); */
// END - OTP Test Routine ----------------------------------

#define __packed __attribute__((__packed__))


typedef struct {
    uint8_t _command_type;
    uint8_t temporary_admin_password[25];
    uint8_t slot_secret[20];
    union {
        uint8_t _slot_config;
        struct {
            bool use_8_digits   : 1;
            bool use_enter      : 1;
            bool use_tokenID    : 1;
        };
    };
    union {
        uint8_t slot_token_id[13]; /** OATH Token Identifier */
        struct { /** @see https://openauthentication.org/token-specs/ */
            uint8_t omp[2];
            uint8_t tt[2];
            uint8_t mui[8];
            uint8_t keyboard_layout; //disabled feature in nitroapp as of 20160805
        } slot_token_fields;
    };
} __packed write_to_slot_1_payload;

typedef struct {
    uint8_t _command_type;
    uint8_t temporary_admin_password[25];
    uint8_t slot_number;
    uint8_t slot_name[15];
    union {
        uint64_t slot_counter;
        uint8_t slot_counter_s[8];
        struct {
            uint16_t slot_interval;
            uint16_t __padding[3];
        };
    } __packed;
} __packed write_to_slot_2_payload;

typedef struct {
    uint8_t _command_type;
    uint8_t slot_number;
    uint8_t slot_name[15];
    uint8_t slot_secret[20];
    union {
        uint8_t _slot_config;
        struct {
            bool use_8_digits   : 1;
            bool use_enter      : 1;
            bool use_tokenID    : 1;
        };
    };
    union {
        uint8_t slot_token_id[13]; /** OATH Token Identifier */
        struct { /** @see https://openauthentication.org/token-specs/ */
            uint8_t omp[2];
            uint8_t tt[2];
            uint8_t mui[8];
            uint8_t keyboard_layout; //disabled feature in nitroapp as of 20160805
        } slot_token_fields;
    };
    union {
        uint64_t slot_counter;
        uint8_t slot_counter_s[8];
    } __packed;
} __packed OTP_slot_content;

static const int CMD_WRITE_CONFIG_PASSWORD_OFFSET = 6;
static const int CMD_ERASE_SLOT_PASSWORD_OFFSET = 2;