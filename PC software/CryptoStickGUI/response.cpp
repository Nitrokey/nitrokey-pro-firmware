/*
* Author: Copyright (C) Andrzej Surowiec 2012
*						Parts Rudolf Boeddeker  Date: 2013-08-13
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

#include "stick20responsedialog.h"


/*******************************************************************************

 External declarations

*******************************************************************************/

/*******************************************************************************

 Local defines

*******************************************************************************/

/*******************************************************************************

  Response

  Constructor Response

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

Response::Response()
{
}

/*******************************************************************************

  getResponse

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

int Response::getResponse(Device *device)
{
     int res;

     memset(reportBuffer,0,sizeof(reportBuffer));

     if (NULL == device->handle)
     {
         return -1;
     }

     res = hid_get_feature_report(device->handle, reportBuffer, sizeof(reportBuffer));

//     qDebug() << "get report size:" << res;
    if (res!=-1)
    {
        deviceStatus=reportBuffer[1];
        lastCommandType=reportBuffer[2];
        lastCommandCRC=((uint32_t *)(reportBuffer+3))[0];
        lastCommandStatus=reportBuffer[7];
        responseCRC=((uint32_t *)(reportBuffer+61))[0];

        memcpy(data,reportBuffer+8,PAYLOAD_SIZE);

        /* Copy Stick 2.0 status vom HID response data */
        memcpy ((void*)&HID_Stick20Status_st,reportBuffer+1+OUTPUT_CMD_RESULT_STICK20_STATUS_START,sizeof (HID_Stick20Status_st));

        return 0;
    }
    else
    {
        return -1;
    }
 }



