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

#include "device.h"
#include "string.h"
#include "response.h"
#include "crc32.h"
#include "sleep.h"



Device::Device(int vid, int pid)
{
    handle=NULL;
    isConnected=false;
    this->vid=vid;
    this->pid=pid;
    validPassword=false;
    passwordSet=false;

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




}

int Device::checkConnection(){
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
        initializeConfig();
        return 1;
        }
        return 0;

    }

}

void Device::connect(){
    handle = hid_open(vid,pid, NULL);
}



int Device::sendCommand(Command *cmd){

    uint8_t report[REPORT_SIZE+1];
    memset(report,0,sizeof(report));

    int i;
    int err;

    report[1]=cmd->commandType;


        memcpy(report+2,cmd->data,COMMAND_SIZE);

        uint32_t crc=0xffffffff;
        for (i=0;i<15;i++){
            crc=Crc32(crc,((uint32_t *)(report+1))[i]);
        }
        ((uint32_t *)(report+1))[15]=crc;

        cmd->crc=crc;

        err = hid_send_feature_report(handle, report, sizeof(report));

        return err;


}


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

         return 0;
     }

    }

     return -1;
}

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

void Device::initializeConfig()
{
    passwordSet=false;
    validPassword=false;
    getSlotName(0x10);
    getSlotName(0x11);
    getSlotName(0x20);
    getSlotName(0x21);
    getSlotName(0x22);
     getSlotName(0x23);
}

void Device::getSlotConfigs()
{
    readSlot(0x10);
    readSlot(0x11);
    readSlot(0x20);
    readSlot(0x21);
    readSlot(0x22);
    readSlot(0x23);
}

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

void Device::getGeneralConfig()
{
}

int Device::writeGeneralConfig(uint8_t data[])
{
    int res;


    if (isConnected){
    Command *cmd=new Command(CMD_WRITE_CONFIG,data,3);
    authorize(cmd);
    res=sendCommand(cmd);

    if (res==-1)
        return -1;
    else{  //sending the command was successful
        Sleep::msleep(100);
        Response *resp=new Response();
        resp->getResponse(this);

        if (cmd->crc==resp->lastCommandCRC){
        if (resp->lastCommandStatus==CMD_STATUS_OK)
            return 0;
        }
    }
    return -2;
    }
    return -3;


}

int Device::firstAuthenticate(uint8_t cardPassword[], uint8_t tempPasswrod[])
{

    int res;
    uint8_t data[50];

    memcpy(data,cardPassword,25);
    memcpy(data+25,tempPasswrod,25);


    if (isConnected){
    Command *cmd=new Command(CMD_FIRST_AUTHENTICATE,data,50);
    res=sendCommand(cmd);

    if (res==-1)
        return -1;
    else{  //sending the command was successful
        //return cmd->crc;
        Sleep::msleep(1000);
        Response *resp=new Response();
        resp->getResponse(this);

        if (cmd->crc==resp->lastCommandCRC){ //the response was for the last command
            if (resp->lastCommandStatus==CMD_STATUS_OK){
                memcpy(password,tempPasswrod,25);
                validPassword=true;
            return 0;
            }
            else if (resp->lastCommandStatus==CMD_STATUS_WRONG_PASSWORD){
            return -3;
            }

        }


    }

   }

    return -2;


}

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


