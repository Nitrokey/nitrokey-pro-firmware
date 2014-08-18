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

#define FIRMWARE_VERSION 14

#define CMD_GET_STATUS 0x00
#define CMD_WRITE_TO_SLOT 0x01
#define CMD_READ_SLOT_NAME 0x02
#define CMD_READ_SLOT 0x03
#define CMD_GET_CODE 0x04
#define CMD_WRITE_CONFIG 0x05
#define CMD_ERASE_SLOT 0x06
#define CMD_FIRST_AUTHENTICATE 0x07
#define CMD_AUTHORIZE 0x08
#define CMD_GET_PASSWORD_RETRY_COUNT 0x09
#define CMD_SET_TIME 0x0B
#define CMD_TEST_COUNTER 0x0C
#define CMD_TEST_TIME 0x0D
#define CMD_USER_AUTHENTICATE 0x0E
#define CMD_GET_USER_PASSWORD_RETRY_COUNT 0x0F
#define CMD_USER_AUTHORIZE 0x10


#define CMD_DATA_OFFSET 0x01

#define STATUS_READY 0x00
#define STATUS_BUSY	 0x01
#define STATUS_ERROR 0x02
#define STATUS_RECEIVED_REPORT 0x03

#define CMD_STATUS_OK 0
#define CMD_STATUS_WRONG_CRC 1
#define CMD_STATUS_WRONG_SLOT 2
#define CMD_STATUS_SLOT_NOT_PROGRAMMED 3
#define CMD_STATUS_WRONG_PASSWORD 4
#define CMD_STATUS_NOT_AUTHORIZED 5
#define CMD_STATUS_TIMESTAMP_WARNING 6
#define CMD_STATUS_NO_NAME_ERROR 7

/*
Output report
size	offset	description
1		0		device status
1		1		last command's type
4		2		last command's CRC
1		6		last command's status
53		7		last command's output
4		60		this report's CRC (with device status equal 0)
*/

#define OUTPUT_CMD_TYPE_OFFSET 1
#define OUTPUT_CMD_CRC_OFFSET 2
#define OUTPUT_CMD_STATUS_OFFSET 6
#define OUTPUT_CMD_RESULT_OFFSET 7
#define OUTPUT_CRC_OFFSET 60


#define CMD_TYPE_OFFSET 0


/*
CMD_WRITE_TO_SLOT

1b command type
1b slot number
15b slot name
20b	secret
1b configuration flags
12b token id
1b keyboard layout
8b counter
*/

#define CMD_WTS_SLOT_NUMBER_OFFSET 1
#define CMD_WTS_SLOT_NAME_OFFSET 2
#define CMD_WTS_SECRET_OFFSET 17
#define CMD_WTS_CONFIG_OFFSET 37
#define CMD_WTS_TOKEN_ID_OFFSET 38
#define CMD_WTS_COUNTER_OFFSET 51


/*
CMD_READ_SLOT

1b command type
1b slot number

*/

#define CMD_RS_SLOT_NUMBER_OFFSET 1

#define	CMD_RS_OUTPUT_COUNTER_OFFSET 34

/*
CMD_GET_CODE

report:
1b command type
1b slot number
8b challenge (for TOTP slot only)

output:
4b generated OTP
1b config flags
13b tokenID
	
*/

#define CMD_GC_SLOT_NUMBER_OFFSET 1
#define CMD_GC_CHALLENGE_OFFSET 2

/*
 * CMD_GET_PASSWORD_RETRY_COUNT
 *
 * output:
 * 1b number of password tries left
 */


/*
CMD_WRITE_CONFIG	

report:
1b command type
1b Numlock slot
1b Capslock slot
1b Scrolllock slot

output:

	
*/
	
	
/*
CMD_FIRST_AUTHENTICATE

report:
1b command type
25b card password
25b new temporary password

*/


/*
CMD_AUTHORIZE

report:
1b command type
4b authorized crc
25b temporary password


*/



__IO extern uint8_t device_status;
extern __IO uint8_t temp_password[25];
extern __IO uint8_t tmp_password_set;
extern __IO uint32_t authorized_crc;

uint8_t parse_report(uint8_t *report,uint8_t *output);
uint8_t cmd_get_status(uint8_t *report,uint8_t *output);
uint8_t cmd_write_to_slot(uint8_t *report,uint8_t *output);
uint8_t cmd_read_slot_name(uint8_t *report,uint8_t *output);
uint8_t cmd_read_slot(uint8_t *report,uint8_t *output);
uint8_t cmd_get_code(uint8_t *report,uint8_t *output);
uint8_t cmd_write_config(uint8_t *report,uint8_t *output);
uint8_t cmd_erase_slot(uint8_t *report,uint8_t *output);
uint8_t cmd_first_authenticate(uint8_t *report,uint8_t *output);
uint8_t cmd_authorize(uint8_t *report,uint8_t *output);
uint8_t cmd_get_password_retry_count(uint8_t *report,uint8_t *output);
uint8_t cmd_user_authenticate(uint8_t *report,uint8_t *output);
uint8_t cmd_user_authorize(uint8_t *report,uint8_t *output);
uint8_t cmd_get_user_password_retry_count(uint8_t *report,uint8_t *output);
uint8_t cmd_set_time(uint8_t *report,uint8_t *output);
//START - OTP Test Routine --------------------------------
/*
uint8_t cmd_test_counter(uint8_t *report,uint8_t *output);
uint8_t cmd_test_time(uint8_t *report,uint8_t *output);
*/
//END - OTP Test Routine ----------------------------------
