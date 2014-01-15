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

#ifndef DEVICE_H
#define DEVICE_H

#include <QtGui>

#include "hidapi.h"
#include "command.h"
#include "hotpslot.h"
#include "totpslot.h"


#define REPORT_SIZE 64
#define PAYLOAD_SIZE 53

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

class Device
{



public:
    Device(int vid, int pid);
    hid_device *handle;
    int checkConnection();
    bool isConnected;
    int sendCommand(Command *cmd);
    void connect();
    int getSlotName(uint8_t slotNo);
    int eraseSlot(uint8_t slotNo);
    int writeToHOTPSlot(HOTPSlot *slot);
    int writeToTOTPSlot(TOTPSlot *slot);
    int getCode(uint8_t slotNo, uint64_t challenge,uint8_t result[18]);
    int readSlot(uint8_t slotNo);
    int getPasswordRetryCount();
    bool newConnection;
    void initializeConfig();
    HOTPSlot *HOTPSlots[2];
    TOTPSlot *TOTPSlots[4];
    void getSlotConfigs();
    uint8_t *password[25];
    bool validPassword;
    bool passwordSet;
    uint8_t passwordRetryCount;

    uint8_t cardSerial[4];
    uint8_t firmwareVersion[2];

    uint8_t generalConfig[3];

    int getStatus();
    void getGeneralConfig();
    int writeGeneralConfig(uint8_t data[3]);

    int firstAuthenticate(uint8_t cardPassword[25],uint8_t tempPasswrod[25]);
    int authorize(Command *authorizedCmd);


private:
    int vid;
    int pid;




};

#endif // DEVICE_H
