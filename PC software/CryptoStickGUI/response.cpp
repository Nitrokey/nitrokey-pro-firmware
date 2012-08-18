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

Response::Response()
{
}


 int Response::getResponse(Device *device){
     int res;

     memset(reportBuffer,0,sizeof(reportBuffer));
     res = hid_get_feature_report(device->handle, reportBuffer, sizeof(reportBuffer));
     qDebug() << "get report size:" << res;
     if (res!=-1){
         deviceStatus=reportBuffer[1];
         lastCommandType=reportBuffer[2];
         lastCommandCRC=((uint32_t *)(reportBuffer+3))[0];
         lastCommandStatus=reportBuffer[7];
         responseCRC=((uint32_t *)(reportBuffer+61))[0];

         memcpy(data,reportBuffer+8,PAYLOAD_SIZE);




         return 0;
     }
    else
        return -1;
 }



