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

#include "stm32f10x.h"
#include "platform_config.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "CCID_usb.h"
#include "keyboard.h"
#include "AccessInterface.h"
#include "hotp.h"


uint8_t keyboardBuffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
__IO uint8_t PrevXferComplete = 1;

void sendChar(uint8_t chr){

    keyboardBuffer[0]=0;
    keyboardBuffer[3]=0;
		
		if (chr>='A'&&chr<='Z'){	
            keyboardBuffer[0]=MOD_SHIFT_LEFT;
            keyboardBuffer[3]=chr-61;
		}
		else if (chr>='a'&&chr<='z'){	
            keyboardBuffer[3]=chr-93;
		}
		else if (chr>='0'&&chr<='9'){
            if (chr=='0')
                keyboardBuffer[3]=39;
            else
                keyboardBuffer[3]=chr-19;
		}
		
  sendKeys(keyboardBuffer);

  keyboardBuffer[0]=0;
  keyboardBuffer[3]=0;
	
  sendKeys(keyboardBuffer);

}

void sendEnter(){
keyboardBuffer[3]=KEY_RETURN;
sendKeys(keyboardBuffer);
keyboardBuffer[0]=0;
keyboardBuffer[3]=0;
sendKeys(keyboardBuffer);
}

void sendKeys(uint8_t* buffer){
  if (bDeviceState == CONFIGURED)
    {
      while(!PrevXferComplete);
      
        PrevXferComplete = 0;
  /* Use the memory interface function to write to the selected endpoint */
    UserToPMABufferCopy(buffer, ENDP4_TXADDR, 8);
   
  /* Update the data length in the control register */
  SetEPTxCount(ENDP4, 8);
  SetEPTxStatus (ENDP4, EP_TX_VALID);
      
    }

}



void sendNumber(uint32_t number){
uint8_t result[10]={0,0,0,0,0,0,0,0,0,0};
uint32_t tmp_number=number;
uint8_t len=0;
uint8_t i;

do{
len++;
tmp_number/=10;

}while(tmp_number>0);


for (i=len;i>0;i--){
result[i-1]=number%10;
number/=10;
}

for (i=0;i<len;i++)
sendChar(result[i]+'0');

}


void sendNumberN(uint32_t number,uint8_t len){
uint8_t result[10]={0,0,0,0,0,0,0,0,0,0};
uint32_t tmp_number=number;
uint8_t i;

if (len<=10){

for (i=len;i>0;i--){
result[i-1]=number%10;
number/=10;
}

for (i=0;i<len;i++)
sendChar(result[i]+'0');

}

}

void sendString(char* string, uint8_t len){
uint8_t i;
for (i=0;i<len;i++)
sendChar(string[i]);

}


void sendTab (void)
{
  keyboardBuffer[3] = KEY_TAB;
  sendKeys(keyboardBuffer);   

  keyboardBuffer[0]=0;
  keyboardBuffer[3]=0;
  sendKeys(keyboardBuffer);
}
