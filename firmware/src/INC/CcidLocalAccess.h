/*
* Author: Copyright (C) Rudolf Boeddeker 				Date: 2010-02-01
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

#ifndef CCID_LOCAL_ACCESS
#define CCID_LOCAL_ACCESS

#ifdef NOT_USED
#define APDU_ANSWER_T0_COMMAND_CORRECT 0x6100  /* Command correct, xx bytes available in response (normally used under T=0 or for commands under any protocol with long response data that cannot be transmitted in one response) */
#define APDU_ANSWER_				0x6285					/* Selected file in termination state */
#define APDU_ANSWER_				0x6581					/* Memory failure */
#define APDU_ANSWER_				0x6700					/* Wrong length (Lc and/or Le) */
#define APDU_ANSWER_				0x6882					/* Secure messaging not supported */
#define APDU_ANSWER_				0x6884					/* Last command of the chain expected */
#define APDU_ANSWER_				0x6982					/* Security status not satisfied */
#define APDU_ANSWER_				0x6983					/* Authentication method blocked */
#define APDU_ANSWER_				0x6985					/* Condition of use not satisfied */
#define APDU_ANSWER_				0x6987					/* Expected SM data objects missing (e.g. SM-key, SSC) */
#define APDU_ANSWER_				0x6988					/* SM data objects incorrect (e.g. wrong TLV-structure in command data) */
#define APDU_ANSWER_				0x6A80					/* Incorrect parameters in the data field */
#define APDU_ANSWER_				0x6A88					/* Referenced data not found */
#define APDU_ANSWER_				0x6B00					/* Wrong parameters P1-P2 */
#define APDU_ANSWER_				0x6D00					/* Instruction (INS) not supported */
#define APDU_ANSWER_				0x6E00					/* Class (CLA) not supported */
#endif

#define APDU_ANSWER_COMMAND_CORRECT				0x9000					/* Command correct */
#define APDU_ANSWER_RECEIVE_CRC_ERROR			0xA000					/* Receive CRC error  */
#define APDU_ANSWER_RECEIVE_INCORRECT			0xA001					/* Receive wrong answer struct  */
#define APDU_ANSWER_CHAINED_DATA    			0xA002					/* Receive chained data  */
																	  

void CcidLocalAccessTest (void);

int getAID(void);
uint32_t getSerialNumber(void);
uint8_t getByteOfData(uint8_t x);
uint8_t cardAuthenticate(uint8_t *password);
uint8_t getPasswordRetryCount(void);
#endif /* CCID_LOCAL_ACCESS */

