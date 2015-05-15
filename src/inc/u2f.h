/*
 * u2f.h
 *
 *  Created on: 13 May, 2015
 *  Author: Thanos Tsakiris
 *
 * This file is part of Nitrokey
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

#ifndef __U2F_H_
#define __U2F_H_

#include <stdint.h>

/* Length definitions of various message segments used in the
 * communication between the Client and the U2F Authenticator.
 */

/* Registration Request */
#define CHALLENGE_PARAMETER_BYTE_LENGTH       32; // SHA­256 hash of the client data (shared with authentication request)
#define APPLICATION_PARAMETER_BYTE_LENGTH     32; // SHA­256 hash of the application identity (shared with authentication request)

/* Registration Response */
#define REGISTRATION_RESPONSE_RESERVED_BYTE   0x05; // Fixed value, used for legacy reasons
#define USER_PUBLIC_KEY_BYTE_LENGTH           65; //  User's public key byte length
#define KEY_HANDLE_LENGTH_BYTE_LENGTH         1; // This byte specifies the length of key handle (shared with authentication request)
/* - Signature parts - */
#define SIG_RESERVED_BYTE_VAL                 0x00; // Reserved for future use

/* Authentication Request */
#define CONTROL_BYTE_LENGTH                             1; // Control byte for check or enforce-user-presence-and-sign
#define CONTROL_BYTE_CHECK_ONLY_VAL                     0x07; // Control byte value for checking if the token has been already registered
#define CONTROL_BYTE_ENFORCE_USER_PRESENCE_AND_SIGN_VAL 0x03; // Control byte value for U2F authentication

/* Authentication Response */
#define SUCC_USER_PRESENCE_BYTE_VAL           0x01; // Bit 0 has to be 1, bits 1-7 should be 0
#define SUCC_COUNTER_BYTE_LENGTH              4;    // The counter byte length, counter is incremented every time an authentication is performed



/**
 * \typedef u2f_registrationRequestMessage
 * \brief Message structure sent by the FIDO client
 *
 * This is the message structure sent by the FIDO client (i.e. web browser) to be
 * processed by the U2F Authenticator.
 */
typedef struct {
  uint8_t challengeParameter[8*CHALLENGE_PARAMETER_BYTE_LENGTH];
  uint8_t applicationParameter[8*APPLICATION_PARAMETER_BYTE_LENGTH];
} u2f_registrationRequestMessage;


#endif /* __U2F_H_ */
