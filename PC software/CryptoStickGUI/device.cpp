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


#include "response.h"
#include "string.h"
#include "crc32.h"
#include "sleep.h"
#include "device.h"
/*******************************************************************************

 External declarations

*******************************************************************************/


/*******************************************************************************

 Local defines

*******************************************************************************/

//#define LOCAL_DEBUG                               // activate for debugging

/*******************************************************************************

  Device

  Constructor Device

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

Device::Device(int vid, int pid,int vidStick20, int pidStick20)
{
    handle=NULL;
    isConnected=false;

    this->vid=vid;
    this->pid=pid;

    validPassword    =false;
    passwordSet      =false;



    memset(password,0,50);
    //handle = hid_open(vid,pid, NULL);

    HOTPSlots[0]=new HOTPSlot();
    HOTPSlots[1]=new HOTPSlot();

    HOTPSlots[0]->slotNumber=0x10;
    HOTPSlots[1]->slotNumber=0x11;

    TOTPSlots[0]=new TOTPSlot();
    TOTPSlots[1]=new TOTPSlot();
    TOTPSlots[2]=new TOTPSlot();
    TOTPSlots[3]=new TOTPSlot();

    TOTPSlots[0]->slotNumber=0x20;
    TOTPSlots[1]->slotNumber=0x21;
    TOTPSlots[2]->slotNumber=0x22;
    TOTPSlots[3]->slotNumber=0x23;

    newConnection=true;

// Init data for stick 20

   this->vidStick20   = vidStick20;
   this->pidStick20   = pidStick20;

//    this->vidStick20   = 0x046d;
//    this->pidStick20   = 0xc315;

    activStick20       = false;
    waitForAckStick20  = false;
    lastBlockNrStick20 = 0;


}

/*******************************************************************************

  checkConnection

  Check the presents of stick 1.0 or 2.0

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::checkConnection()
{
    uint8_t buf[65];
    buf[0]=0;
    int res;

    //handle = hid_open(vid,pid, NULL);

    if (!handle){
        isConnected=false;
        newConnection=true;
        return -1;
    }
    else{
        res = hid_get_feature_report (handle, buf, 65);
        if (res < 0){
            isConnected=false;
            newConnection=true;
            return -1;
        }

        if (newConnection){
            isConnected=true;
            newConnection=false;
            passwordSet=false;
            validPassword=false;

            // stick 20 with no OTP
            if (false == activStick20)
            {
            }
            initializeConfig();          // init data for OTP
            return 1;
        }
        return 0;

    }

}

/*******************************************************************************

  connect

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

void Device::connect()
{
    // Disable stick 20
    activStick20 = false;

    handle = hid_open(vid,pid, NULL);

    // Check for stick 20
    if (NULL == handle)
    {
        handle = hid_open(vidStick20,pidStick20, NULL);
        if (NULL != handle)
        {
            // Stick 20 found
            activStick20 = true;
        }
    }
}

/*******************************************************************************

  sendCommand

  Send a command to the stick via the send feature report HID message

  Report size is 64 byte

  Command data size COMMAND_SIZE = 59 byte (should 58 ???)

  Byte  0       = 0
  Byte  1       = cmd type
  Byte  2-59    = payload       (To do check lenght)
  Byte 60-63    = CRC 32 from byte 0-59 = 15 long words


  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::sendCommand(Command *cmd)
{
    uint8_t report[REPORT_SIZE+1];
    int i;
    int err;

    memset(report,0,sizeof(report));
    report[1]=cmd->commandType;


    memcpy(report+2,cmd->data,COMMAND_SIZE);

    uint32_t crc=0xffffffff;
    for (i=0;i<15;i++){
        crc=Crc32(crc,((uint32_t *)(report+1))[i]);
    }
    ((uint32_t *)(report+1))[15]=crc;

    cmd->crc=crc;

    {
         char text[1000];
         sprintf(text,"computed crc :%08x:\n",crc );
         DebugAppendText (text);
    }

    err = hid_send_feature_report(handle, report, sizeof(report));

    {
            char text[1000];
            int i;
            static int Counter = 0;

            sprintf(text,"%6d :sendCommand: ",Counter);
            Counter++;
            DebugAppendText (text);
            for (i=0;i<=64;i++)
            {
                sprintf(text,"%02x ",(unsigned char)report[i]);
                DebugAppendText (text);
            }
            sprintf(text,"\n");
            DebugAppendText (text);

     }
    return err;
}


int Device::sendCommandGetResponse(Command *cmd, Response *resp)
{
    uint8_t report[REPORT_SIZE+1];
    int i;
    int err;

    if (!isConnected)
        return ERR_NOT_CONNECTED;

    memset(report,0,sizeof(report));
    report[1]=cmd->commandType;


    memcpy(report+2,cmd->data,COMMAND_SIZE);

    uint32_t crc=0xffffffff;
    for (i=0;i<15;i++){
        crc=Crc32(crc,((uint32_t *)(report+1))[i]);
    }
    ((uint32_t *)(report+1))[15]=crc;

    cmd->crc=crc;

    err = hid_send_feature_report(handle, report, sizeof(report));

    if (err==-1)
        return ERR_SENDING;

    Sleep::msleep(100);

    //Response *resp=new Response();
    resp->getResponse(this);

    if (cmd->crc!=resp->lastCommandCRC)
        return ERR_WRONG_RESPONSE_CRC;

    if (resp->lastCommandStatus==CMD_STATUS_OK)
        return 0;



    return ERR_STATUS_NOT_OK;
}


/*******************************************************************************

  getSlotName

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::getSlotName(uint8_t slotNo){
     int res;
     uint8_t data[1];

     data[0]=slotNo;


     if (isConnected){
     Command *cmd=new Command(CMD_READ_SLOT_NAME,data,1);
     res=sendCommand(cmd);

     if (res==-1)
         return -1;
     else{  //sending the command was successful
         //return cmd->crc;
         Sleep::msleep(100);
         Response *resp=new Response();
         resp->getResponse(this);

         qDebug() << cmd->crc;
         qDebug() << resp->lastCommandCRC;

         if (cmd->crc==resp->lastCommandCRC){ //the response was for the last command
             if (resp->lastCommandStatus==CMD_STATUS_OK){
                 if (slotNo>=0x10&&slotNo<=0x11){
                    memcpy(HOTPSlots[slotNo&0x0F]->slotName,resp->data,15);
                    HOTPSlots[slotNo&0x0F]->isProgrammed=true;
                 }
                 else if (slotNo>=0x20&&slotNo<=0x23){
                     memcpy(TOTPSlots[slotNo&0x0F]->slotName,resp->data,15);
                     TOTPSlots[slotNo&0x0F]->isProgrammed=true;
                 }

             }
             else if (resp->lastCommandStatus==CMD_STATUS_SLOT_NOT_PROGRAMMED){
                 if (slotNo>=0x10&&slotNo<=0x11)
                    HOTPSlots[slotNo&0x0F]->isProgrammed=false;
                 else if (slotNo>=0x20&&slotNo<=0x23)
                     TOTPSlots[slotNo&0x0F]->isProgrammed=false;
             }

         }
/*
         {

             QMessageBox message;
             QString str;
             QByteArray *data =new QByteArray((char*)resp->reportBuffer,REPORT_SIZE+1);

 //            str.append(QString::number(testResponse->lastCommandCRC,16));
             str.append(QString(data->toHex()));

             message.setText(str);
             message.exec();

             str.clear();
         }
*/
         return 0;
     }

    }

     return -1;
}

/*******************************************************************************

  eraseSlot

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::eraseSlot(uint8_t slotNo)
{
    int res;
    uint8_t data[1];

    data[0]=slotNo;



    if (isConnected){
    Command *cmd=new Command(CMD_ERASE_SLOT,data,1);
    authorize(cmd);
    res=sendCommand(cmd);

    if (res==-1)
        return -1;
    else{  //sending the command was successful
        //return cmd->crc;
        Sleep::msleep(100);
        Response *resp=new Response();
        resp->getResponse(this);

        qDebug() << cmd->crc;
        qDebug() << resp->lastCommandCRC;

        }

        return 0;
    }

    return -1;

}

/*******************************************************************************

  writeToHOTPSlot

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::writeToHOTPSlot(HOTPSlot *slot)
{
    qDebug() << "preparing to send";
    qDebug() << slot->slotNumber;
    qDebug() << QString((char *)slot->slotName);
    if (slot->slotNumber>=0x10&&slot->slotNumber<=0x11){
        int res;
        uint8_t data[COMMAND_SIZE];
        memset(data,0,COMMAND_SIZE);
        qDebug() << "prepared data array";

        data[0]=slot->slotNumber;
        memcpy(data+1,slot->slotName,15);
        memcpy(data+16,slot->secret,20);
        data[36]=slot->config;
        memcpy(data+37,slot->tokenID,13);

        memcpy(data+50,slot->counter,8);



        qDebug() << "copied data to array";

        if (isConnected){
        Command *cmd=new Command(CMD_WRITE_TO_SLOT,data,COMMAND_SIZE);
        qDebug() << "sending";
        authorize(cmd);
        res=sendCommand(cmd);
        qDebug() << "sent";

        if (res==-1)
            return -1;
        else{  //sending the command was successful
            //return cmd->crc;
            Sleep::msleep(100);
            Response *resp=new Response();
            resp->getResponse(this);

             if (cmd->crc==resp->lastCommandCRC&&resp->lastCommandStatus==CMD_STATUS_OK){
                 qDebug() << "sent sucessfully!";
                 return 0;

             }

        }

        return -2;
        }


    }
    return -1;
}

/*******************************************************************************

  writeToTOTPSlot

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::writeToTOTPSlot(TOTPSlot *slot)
{
    if (slot->slotNumber>=0x20&&slot->slotNumber<=0x23){
        int res;
        uint8_t data[COMMAND_SIZE];
        memset(data,0,COMMAND_SIZE);
        qDebug() << "prepared data array";

        data[0]=slot->slotNumber;
        memcpy(data+1,slot->slotName,15);
        memcpy(data+16,slot->secret,20);
        data[36]=slot->config;
        memcpy(data+37,slot->tokenID,13);

        qDebug() << "copied data to array";

        if (isConnected){
        Command *cmd=new Command(CMD_WRITE_TO_SLOT,data,COMMAND_SIZE);
        qDebug() << "sending";
        authorize(cmd);
        res=sendCommand(cmd);
        qDebug() << "sent";

        if (res==-1)
            return -1;
        else{  //sending the command was successful
            //return cmd->crc;
            Sleep::msleep(100);
            Response *resp=new Response();
            resp->getResponse(this);

             if (cmd->crc==resp->lastCommandCRC&&resp->lastCommandStatus==CMD_STATUS_OK){
                 qDebug() << "sent sucessfully!";
                 return 0;

             }

        }

        return -2;
        }


    }
    return -1;


}
/*******************************************************************************

  getCode

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::getCode(uint8_t slotNo, uint64_t challenge,uint8_t result[18])
{

    qDebug() << "getting code" << slotNo;
    int res;
    uint8_t data[9];

    data[0]=slotNo;

    memcpy(data+1,&challenge,8);


    if (isConnected){
       qDebug() << "sending command";
    Command *cmd=new Command(CMD_GET_CODE,data,9);
    res=sendCommand(cmd);

    if (res==-1)
        return -1;
    else{  //sending the command was successful
        //return cmd->crc;
         qDebug() << "command sent";
        Sleep::msleep(100);
        Response *resp=new Response();
        resp->getResponse(this);


        if (cmd->crc==resp->lastCommandCRC){ //the response was for the last command
            if (resp->lastCommandStatus==CMD_STATUS_OK){
                memcpy(result,resp->data,18);

            }

        }

        return 0;
    }

   }

    return -1;

}


int Device::getHOTP(uint8_t slotNo)
{

    qDebug() << "getting code" << slotNo;
    int res;
    uint8_t data[9];

    data[0]=slotNo;

    //memcpy(data+1,&challenge,8);


    Command *cmd=new Command(CMD_GET_CODE,data,9);
    Response *resp=new Response();
    res=sendCommandGetResponse(cmd,resp);

        //    if (res==0)
        //        memcpy(result,resp->data,18);

    return res;

}

/*******************************************************************************

  readSlot

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::readSlot(uint8_t slotNo)
{
    int res;
    uint8_t data[1];

    data[0]=slotNo;

    qDebug()<< "reading slot number:" << slotNo;

    if (isConnected){
    Command *cmd=new Command(CMD_READ_SLOT,data,1);
    authorize(cmd);
    res=sendCommand(cmd);

    if (res==-1)
        return -1;
    else{  //sending the command was successful
        //return cmd->crc;
        Sleep::msleep(100);
        Response *resp=new Response();
        resp->getResponse(this);

        qDebug() << cmd->crc;
        qDebug() << resp->lastCommandCRC;

        if (cmd->crc==resp->lastCommandCRC){ //the response was for the last command
            if (resp->lastCommandStatus==CMD_STATUS_OK){
                if (slotNo>=0x10&&slotNo<=0x11){
                   memcpy(HOTPSlots[slotNo&0x0F]->secret,resp->data,20);
                   HOTPSlots[slotNo&0x0F]->config=resp->data[20];
                   memcpy(HOTPSlots[slotNo&0x0F]->counter,resp->data+34,8);
                   memcpy(HOTPSlots[slotNo&0x0F]->tokenID,resp->data+21,13);
                   HOTPSlots[slotNo&0x0F]->isProgrammed=true;
                }
                else if (slotNo>=0x20&&slotNo<=0x23){
                    memcpy(TOTPSlots[slotNo&0x0F]->secret,resp->data,20);
                    TOTPSlots[slotNo&0x0F]->config=resp->data[20];
                    memcpy(TOTPSlots[slotNo&0x0F]->tokenID,resp->data+21,13);
                    TOTPSlots[slotNo&0x0F]->isProgrammed=true;

                }
                qDebug() << "programmed";
            }
            else if (resp->lastCommandStatus==CMD_STATUS_SLOT_NOT_PROGRAMMED){
                if (slotNo>=0x10&&slotNo<=0x11){
                    HOTPSlots[slotNo&0x0F]=new HOTPSlot();
                  // HOTPSlots[slotNo&0x0F]->isProgrammed=false;
                }
                else if (slotNo>=0x20&&slotNo<=0x23){
                    TOTPSlots[slotNo&0x0F]=new TOTPSlot();
                   // TOTPSlots[slotNo&0x0F]->isProgrammed=false;
                }

                qDebug() << "not programmed";
            }

        }

        return 0;
    }

   }

    return -1;
}
/*******************************************************************************

  initializeConfig

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

void Device::initializeConfig()
{
    getSlotName(0x10);
    getSlotName(0x11);
    getSlotName(0x20);
    getSlotName(0x21);
    getSlotName(0x22);
    getSlotName(0x23);
}

/*******************************************************************************

  getSlotConfigs

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

void Device::getSlotConfigs()
{
 /* Removed from firmware
    readSlot(0x10);
    readSlot(0x11);
    readSlot(0x20);
    readSlot(0x21);
    readSlot(0x22);
    readSlot(0x23);*/
}
/*******************************************************************************

  getStatus

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::getStatus()
{
    int res;
    uint8_t data[1];


    if (isConnected){
    Command *cmd=new Command(CMD_GET_STATUS,data,0);
    res=sendCommand(cmd);

    if (res==-1)
        return -1;
    else{  //sending the command was successful
        Sleep::msleep(100);
        Response *resp=new Response();
        resp->getResponse(this);

        if (cmd->crc==resp->lastCommandCRC){
            memcpy(firmwareVersion,resp->data,2);
            memcpy(cardSerial,resp->data+2,4);
            memcpy(generalConfig,resp->data+6,3);


        }
    }
    return 0;
    }
    return -2;
}

/*******************************************************************************

  getPasswordRetryCount

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

int Device::getPasswordRetryCount()
{
    int res;
    uint8_t data[1];


    if (isConnected){
    Command *cmd=new Command(CMD_GET_PASSWORD_RETRY_COUNT,data,0);
    res=sendCommand(cmd);

    if (res==-1)
        return ERR_SENDING;
    else{  //sending the command was successful
        Sleep::msleep(100);
        Response *resp=new Response();
        resp->getResponse(this);

        if (cmd->crc==resp->lastCommandCRC){
            passwordRetryCount=resp->data[0];
        }
        else
            return ERR_WRONG_RESPONSE_CRC;
    }
    }
    return ERR_NOT_CONNECTED;
}


/*******************************************************************************

  getGeneralConfig

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

void Device::getGeneralConfig()
{
}

/*******************************************************************************

  writeGeneralConfig

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::writeGeneralConfig(uint8_t data[])
{
    int res;


    if (isConnected){
    Command *cmd=new Command(CMD_WRITE_CONFIG,data,3);
    authorize(cmd);
    res=sendCommand(cmd);

    if (res==-1)
        return ERR_SENDING;
    else{  //sending the command was successful
        Sleep::msleep(100);
        Response *resp=new Response();
        resp->getResponse(this);

        if (cmd->crc==resp->lastCommandCRC){
        if (resp->lastCommandStatus==CMD_STATUS_OK)
            return 0;
        }
        else
            return ERR_WRONG_RESPONSE_CRC;
    }
    }
    return ERR_NOT_CONNECTED;


}
/*******************************************************************************

  firstAuthenticate

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::firstAuthenticate(uint8_t cardPassword[], uint8_t tempPasswrod[])
{

    int res;
    uint8_t data[50];
    uint32_t crc;
    memcpy(data,cardPassword,25);
    memcpy(data+25,tempPasswrod,25);


    if (isConnected){
    Command *cmd=new Command(CMD_FIRST_AUTHENTICATE,data,50);
    res=sendCommand(cmd);
    crc=cmd->crc;
    //remove the card password from memory
    delete cmd;
    memset(data,0,sizeof(data));

    if (res==-1)
        return -1;
    else{  //sending the command was successful
        //return cmd->crc;
        Sleep::msleep(1000);
        Response *resp=new Response();
        resp->getResponse(this);
{
        char text[1000];
     sprintf(text,"send crc :%08x: get :%08x:\n",cmd->crc,resp->lastCommandCRC );
     DebugAppendText (text);
}
        if (cmd->crc==resp->lastCommandCRC)
        { //the response was for the last command
            if (resp->lastCommandStatus==CMD_STATUS_OK)
            {
                memcpy(password,tempPasswrod,25);
                validPassword=true;
                return 0;
            }
            else if (resp->lastCommandStatus==CMD_STATUS_WRONG_PASSWORD)
            {
                return -3;
            }

        }

    }

   }

    return -2;


}



/*******************************************************************************

  authorize

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Device::authorize(Command *authorizedCmd)
{
   authorizedCmd->generateCRC();
   uint32_t crc=authorizedCmd->crc;
   uint8_t data[29];
   int res;

   memcpy(data,&crc,4);
   memcpy(data+4,password,25);

   if (isConnected){
   Command *cmd=new Command(CMD_AUTHORIZE,data,29);
   res=sendCommand(cmd);


   if (res==-1)
       return -1;
   else{

       Sleep::msleep(200);
       Response *resp=new Response();
       resp->getResponse(this);

       if (cmd->crc==resp->lastCommandCRC){ //the response was for the last command
           if (resp->lastCommandStatus==CMD_STATUS_OK){
               return 0;
           }

       }
       return -2;
   }
   }

   return -1;
}

/*******************************************************************************

    Here starts the new commands for stick 2.0

*******************************************************************************/

/*******************************************************************************

  stick20EnableCryptedPartition

  Send the command

    STICK20_CMD_ENABLE_CRYPTED_PARI

  with the password to cryptostick 2.0

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

#define CS20_MAX_PASSWORD_LEN       30

bool Device::stick20EnableCryptedPartition  (uint8_t *password)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check password length
    n = strlen ((const char*)password);
    if (CS20_MAX_PASSWORD_LEN <= n)
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_ENABLE_CRYPTED_PARI,password,n);
    res = sendCommand(cmd);

    return (true);
}
/*******************************************************************************

  stick20DisableCryptedPartition

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20DisableCryptedPartition  (void)
{
    int      res;
    Command *cmd;

    cmd = new Command(STICK20_CMD_DISABLE_CRYPTED_PARI,NULL,0);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20EnableHiddenCryptedPartition

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20EnableHiddenCryptedPartition  (uint8_t *password)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check password length
    n = strlen ((const char*)password);

    if (CS20_MAX_PASSWORD_LEN <= n)
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_ENABLE_HIDDEN_CRYPTED_PARI,password,n);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20DisableHiddenCryptedPartition

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20DisableHiddenCryptedPartition  (void)
{
    int      res;
    Command *cmd;

    cmd = new Command(STICK20_CMD_DISABLE_HIDDEN_CRYPTED_PARI,NULL,0);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20EnableFirmwareUpdate

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20EnableFirmwareUpdate (uint8_t *password)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check password length
    n = strlen ((const char*)password);
    if (CS20_MAX_PASSWORD_LEN <= n)
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_ENABLE_FIRMWARE_UPDATE,password,n);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20ExportFirmware

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20ExportFirmware (uint8_t *password)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check password length
    n = strlen ((const char*)password);
    if (CS20_MAX_PASSWORD_LEN <= n)
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_EXPORT_FIRMWARE_TO_FILE,password,n);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20CreateNewKeys

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20CreateNewKeys (uint8_t *password)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check password length
    n = strlen ((const char*)password);
    if (CS20_MAX_PASSWORD_LEN <= n)
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_GENERATE_NEW_KEYS,password,n);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20FillSDCardWithRandomChars

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20FillSDCardWithRandomChars (uint8_t *password,uint8_t VolumeFlag)
{
    uint8_t  n;
    int      res;
    Command *cmd;
    uint8_t  data[CS20_MAX_PASSWORD_LEN+2];

    // Check password length
    n = strlen ((const char*)password);
    if (CS20_MAX_PASSWORD_LEN <= n)
    {
        return (false);
    }

    data[0] = VolumeFlag;
    strcpy ((char*)&data[1],(char*)password);

    cmd = new Command(STICK20_CMD_FILL_SD_CARD_WITH_RANDOM_CHARS,data,n+1);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20SetupHiddenVolume

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20SetupHiddenVolume (void)
{
//    uint8_t n;
    int     res;
    Command *cmd;
//    Response *resp;

    cmd = new Command(STICK20_CMD_SEND_HIDDEN_VOLUME_SETUP,NULL,0);
    res = sendCommand(cmd);


/*
    Sleep::msleep(200);
    Response *resp=new Response();
    resp->getResponse(this);

    if (cmd->crc==resp->lastCommandCRC)
    { //the response was for the last command
        if (resp->lastCommandStatus!=CMD_STATUS_OK){
            return (FALSE);
        }
    }
*/

    return (true);
}

/*******************************************************************************

  stick20GetPasswordMatrix

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20GetPasswordMatrix (void)
{
//    uint8_t n;
    int     res;
    Command *cmd;
//    Response *resp;

    cmd = new Command(STICK20_CMD_SEND_PASSWORD_MATRIX,NULL,0);
    res = sendCommand(cmd);

/*

    Sleep::msleep(200);
    Response *resp=new Response();
    resp->getResponse(this);

    if (cmd->crc==resp->lastCommandCRC)
    { //the response was for the last command
        if (resp->lastCommandStatus==CMD_STATUS_OK){
            return 0;
        }
    }
*/

    return (true);
}

/*******************************************************************************

  stick20SendPasswordMatrixPinData

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20SendPasswordMatrixPinData (uint8_t *Pindata)
{
    uint8_t n;
    int     res;
    Command *cmd;
//    Response *resp;

    // Check pin data length
    n = strlen ((const char*)Pindata);
    if (STICK20_PASSOWRD_LEN + 2 <= n)      // Kind byte + End byte 0
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_SEND_PASSWORD_MATRIX_PINDATA,Pindata,n);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20SendPasswordMatrixSetup

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20SendPasswordMatrixSetup (uint8_t *Setupdata)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check pin data length
    n = strlen ((const char*)Setupdata);
    if (STICK20_PASSOWRD_LEN + 1 <= n)
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_SEND_PASSWORD_MATRIX_SETUP,Setupdata,n);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20GetStatusData

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

bool Device::stick20GetStatusData ()
{
    int     res;
    Command *cmd;

    cmd = new Command(STICK20_CMD_GET_DEVICE_STATUS,NULL,0);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20SendPassword

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

int Device::stick20SendPassword (uint8_t *Pindata)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check pin data length
    n = strlen ((const char*)Pindata);
    if (STICK20_PASSOWRD_LEN + 2 <= n)      // Kind byte + End byte 0
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_SEND_PASSWORD,Pindata,n);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20SendNewPassword

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

int Device::stick20SendNewPassword (uint8_t *NewPindata)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check pin data length
    n = strlen ((const char*)NewPindata);
    if (STICK20_PASSOWRD_LEN + 2 <= n)      // Kind byte + End byte 0
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_SEND_NEW_PASSWORD,NewPindata,n);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20SendSetReadonlyToUncryptedVolume

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

int Device::stick20SendSetReadonlyToUncryptedVolume (uint8_t *Pindata)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check pin data length
    n = strlen ((const char*)Pindata);
    if (STICK20_PASSOWRD_LEN + 2 <= n)      // Kind byte + End byte 0
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_ENABLE_READONLY_UNCRYPTED_LUN,Pindata,n);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20SendSetReadwriteToUncryptedVolume

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

int Device::stick20SendSetReadwriteToUncryptedVolume (uint8_t *Pindata)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check pin data length
    n = strlen ((const char*)Pindata);
    if (STICK20_PASSOWRD_LEN + 2 <= n)      // Kind byte + End byte 0
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_ENABLE_READWRITE_UNCRYPTED_LUN,Pindata,n);
    res = sendCommand(cmd);

    return (true);
}

/*******************************************************************************

  stick20SendClearNewSdCardFound

  Changes
  Date      Author          Info
  11.02.14  RB              Implementation: New SD card found

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

int Device::stick20SendClearNewSdCardFound (uint8_t *Pindata)
{
    uint8_t n;
    int     res;
    Command *cmd;

    // Check pin data length
    n = strlen ((const char*)Pindata);
    if (STICK20_PASSOWRD_LEN + 2 <= n)      // Kind byte + End byte 0
    {
        return (false);
    }

    cmd = new Command(STICK20_CMD_CLEAR_NEW_SD_CARD_FOUND,Pindata,n);
    res = sendCommand(cmd);

    return (true);
}

