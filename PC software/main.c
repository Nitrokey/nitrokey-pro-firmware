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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "hiddata.h"
#include "inttypes.h"
#include "report_protocol.h"

#define REPORT_SIZE 64

uint32_t Crc32(uint32_t Crc, uint32_t Data)
{
  int i;

  Crc = Crc ^ Data;

  for(i=0; i<32; i++)
    if (Crc & 0x80000000)
      Crc = (Crc << 1) ^ 0x04C11DB7; // Polynomial used in STM32
    else
      Crc = (Crc << 1);

  return(Crc);
}
	
void read_hex(char *input,uint8_t *buf,int len){
int i;
int j=0;
char byte[2];

for (i=0;i<len;i++){
byte[0]=input[j++];
byte[1]=input[j++];
buf[i]=strtol(byte,NULL,16);
}

}
	
int main(int argc, char **argv)
{
usbDevice_t *dev;
char        buffer[REPORT_SIZE+1];    /* room for dummy report ID */
char		get_report[REPORT_SIZE+1];
int         err;
int 		i;


uint8_t slot=0xff;
uint8_t secret[20];
uint64_t counter=0;

   int c;
     
       while (1)
         {
           static struct option long_options[] =
             {
			 {"counter", required_argument, 0, 'c'},
           	  {"slot", required_argument, 0, 's'},
			  {"secret", required_argument, 0, 'S'},
				{0, 0, 0, 0}
             };
           /* getopt_long stores the option index here. */
           int option_index = 0;
     
           c = getopt_long (argc, argv, "c:s:S:",
                            long_options, &option_index);
     
           /* Detect the end of the options. */
           if (c == -1)
             break;
     
           switch (c)
             {
             case 0:
               break;
     
     
             case 'c':
			 counter=strtoull(optarg,NULL,0);
			 if(counter==0){
             printf ("Incorrect counter value\n");
			 return 1;
			 }
             break;
			   
			 case 's':
			 slot=strtol(optarg,NULL,10);
			 if(slot==0||slot>2){
             printf ("Incorrect slot value\n");
			 return 1;
			 }
             break;
			   
			 case 'S':
			 if (strlen(optarg)!=40){
			 printf ("Secret must be exactly 20 bytes.\n");
			 return 1;
			 }
             read_hex(optarg,secret,20);
             break;
     
             case '?':
               break;
     
             default:
               abort ();
             }
         }
     
     
       /* Remaining command line arguments (not options). */
       if (optind < argc)
         {
			printf("Too many arguments.\n");
			return 1;
		 }

	if (slot>2){
	printf("Please select slot with the -s option.\n");
	return 1;
	}
	
	printf("Slot no.:%d\n",slot);
	printf("Counter value:%" PRIx64 "\n",counter);
	printf("Secret:%.20s\n",secret);

  err = usbhidOpenDevice(&dev, 0x20a0, NULL, 0x4106, NULL, 0);
  if (err!=0){
  printf("Error opening device:%d\n",err);
  return 1;
  }
  
  int len=REPORT_SIZE+1;
  err = usbhidGetReport(dev, 0, get_report, &len);
  
  memset(buffer,0,REPORT_SIZE+1);
  
  
  
  buffer[1]=CMD_WRITE_TO_SLOT;
  buffer[REPORT_SLOT_NUMBER_OFFSET+1]=slot-1;
  ((uint64_t *)(buffer+REPORT_COUNTER_VALUE_OFFSET+1))[0]=counter;
  memcpy(buffer+REPORT_SECRET_VALUE_OFFSET+1,secret,20);
  
  
		uint32_t crc=0xffffffff;
		for (i=0;i<15;i++){
		printf("%08X\n\n",((uint32_t *)(buffer+1))[i]);
		crc=Crc32(crc,((uint32_t *)(buffer+1))[i]);
		}
		
		((uint32_t *)(buffer+1))[15]=crc;
		 printf("%08X\n\n",crc);
  
  err = usbhidSetReport(dev, buffer, REPORT_SIZE+1);
  
  if (err!=0){
  printf("Error sending report:%d\n",err);
  return 1;
  }
  
    usbhidCloseDevice(dev);
    return 0;
}
