/*
* Author: Copyright (C) Rudolf Boeddeker  Date: 2013-08-12
*
* This file is part of GPF Crypto Stick 2
*
* GPF Crypto Stick 2  is free software: you can redistribute it and/or modify
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

#ifndef HID_STICK_20_H

#define HID_STICK_20_H




#ifdef __cplusplus
extern "C" {
#endif

// Offset of status block
#define OUTPUT_CMD_RESULT_STICK20_STATUS_START  20

// Status block
typedef struct {
  unsigned char  CommandCounter_u8;
  unsigned char  LastCommand_u8;
  unsigned char  Status_u8;
  unsigned char  ProgressBarValue_u8;
} HID_Stick20Status_est;




#define OUTPUT_CMD_RESULT_STICK20_DATA_START        25


/** Only for debugging */

#define STICK20_DEBUG_TEXT_LEN          600000

extern char DebugText_Stick20[STICK20_DEBUG_TEXT_LEN];
extern unsigned long DebugTextlen_Stick20;
extern int DebugingActive;

void DebugClearText (void);
void DebugAppendText (char *Text);


/** Only for debugging - End */

#define OUTPUT_CMD_STICK20_SEND_DATA_TYPE_NONE        0
#define OUTPUT_CMD_STICK20_SEND_DATA_TYPE_DEBUG       1
#define OUTPUT_CMD_STICK20_SEND_DATA_TYPE_PW_DATA     2
#define OUTPUT_CMD_STICK20_SEND_DATA_TYPE_STATUS      3


#define OUTPUT_CMD_STICK20_SEND_DATA_SIZE            25

typedef struct {
  unsigned char  SendCounter_u8;
  unsigned char  SendDataType_u8;
  unsigned char  FollowBytesFlag_u8;
  unsigned char  SendSize_u8;
  unsigned char  SendData_u8[OUTPUT_CMD_STICK20_SEND_DATA_SIZE];
} HID_Stick20SendData_est;

extern HID_Stick20SendData_est HID_Stick20ReceiveData_st;



#define STICK20_PASSWORD_MATRIX_DATA_LEN 100

#define STICK20_PASSWORD_MATRIX_STATUS_IDLE                 0
#define STICK20_PASSWORD_MATRIX_STATUS_GET_NEW_BLOCK        1
#define STICK20_PASSWORD_MATRIX_STATUS_NEW_BLOCK_RECEIVED   2

typedef struct {
  unsigned char  StatusFlag_u8;
           char  PasswordMatrix_u8[STICK20_PASSWORD_MATRIX_DATA_LEN];
} HID_Stick20MatrixPasswordData_est;

extern HID_Stick20MatrixPasswordData_est HID_Stick20MatrixPasswordData_st;



typedef struct {
    unsigned char  MatrixPasswordUserActiv_u8;
    unsigned char  MatrixPasswordAdminActiv_u8;
    unsigned char  ActivPasswordStatus_u8;
    unsigned char  VolumeStatus_u8;
    unsigned long  SD_BlockSize_u32;
} HID_Stick20AccessStatus_est;


extern HID_Stick20AccessStatus_est HID_Stick20AccessStatus_st;

int HID_GetStick20AccessStatusData (void);
int HID_GetStick20PasswordMatrixData (void);
int HID_GetStick20DebugData (void);
int HID_GetStick20ReceiveData (unsigned char *data);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // HID_STICK_20_H
