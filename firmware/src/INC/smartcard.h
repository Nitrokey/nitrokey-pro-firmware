/*
* Author: Copyright (C) Rudolf Boeddeker 					Date: 2010-01-13
*												STMicroelectronics	 			
*												MCD Application Team			Date:	07/11/2008	 V2.0.2
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SMARTCARD_H
#define __SMARTCARD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported constants --------------------------------------------------------*/
#define T0_PROTOCOL        0x00  /* T0 protocol */
#define DIRECT             0x3B  /* Direct bit convention */
#define INDIRECT           0x3F  /* Indirect bit convention */
#define SETUP_LENGTH       20
#define HIST_LENGTH        20
#define LCmax              20
#define SC_Receive_Timeout 0x8000  /* Direction to reader */


/* Smartcard Inteface GPIO pins */
#define EXTI9_5_IRQChannel           ((unsigned char)0x17)  /* External Line [9:5] Interrupts */
#define USART3_IRQChannel            ((unsigned char)0x27)  /* USART3 global Interrupt */

//  #define SC_3_5V                  GPIO_Pin_0  /* GPIOB Pin 0 */
//  #define SC_CMDVCC                GPIO_Pin_6  /* GPIOC Pin 6  */
//  #define SC_OFF                   GPIO_Pin_7  /* GPIOC Pin 7 */ 
//  #define GPIO_3_5V                GPIOB
//  #define GPIO_CMDVCC              GPIOC
//  #define GPIO_OFF                 GPIOC
//  #define RCC_APB2Periph_3_5V      RCC_APB2Periph_GPIOB
//  #define RCC_APB2Periph_CMDVCC    RCC_APB2Periph_GPIOC
//  #define RCC_APB2Periph_OFF       RCC_APB2Periph_GPIOC

#define SC_RESET                 GPIO_Pin_0 									/* GPIOB Pin 0 */
#define GPIO_RESET               GPIOB
#define RCC_APB2Periph_RESET     RCC_APB2Periph_GPIOB

#define SC_PortSource            	 GPIO_PortSourceGPIOB
#define SC_PinSource_1             GPIO_PinSource13
#define SC_PinSource_2             GPIO_PinSource14

#define SC_EXTI                  EXTI_Line7
#define SC_EXTI_IRQ              EXTI9_5_IRQChannel



/* SC Tree Structure -----------------------------------------------------------
                              MasterFile
                           ________|___________
                          |        |           |
                        System   UserData     Note
------------------------------------------------------------------------------*/

/* SC ADPU Command: Operation Code -------------------------------------------*/
#define SC_CLA_GSM11       0xA0
#define SC_CLA_TEST        0x80

/*------------------------ Data Area Management Commands ---------------------*/
#define SC_SELECT_FILE     0xA4
#define SC_GET_RESPONCE    0xC0
#define SC_STATUS          0xF2
#define SC_UPDATE_BINARY   0xD6
#define SC_READ_BINARY     0xB0
#define SC_WRITE_BINARY    0xD0
#define SC_UPDATE_RECORD   0xDC
#define SC_READ_RECORD     0xB2

/*-------------------------- Administrative Commands -------------------------*/ 
#define SC_CREATE_FILE     0xE0

/*-------------------------- Safety Management Commands ----------------------*/
#define SC_VERIFY          0x20
#define SC_CHANGE          0x24
#define SC_DISABLE         0x26
#define SC_ENABLE          0x28
#define SC_UNBLOCK         0x2C
#define SC_EXTERNAL_AUTH   0x82
#define SC_GET_CHALLENGE   0x84

/*-------------------------- Answer to reset Commands ------------------------*/ 
#define SC_GET_A2R         0x00


/* SC STATUS: Status Code ----------------------------------------------------*/
#define SC_EF_SELECTED     			0x9F
#define SC_DF_SELECTED     			0x9F
#define SC_OP_TERMINATED   			0x9000
#define SC_OP_CMD_NOT_ALLOWED   0x6D00

/* Smartcard Voltage */
#define SC_Voltage_5V      0
#define SC_Voltage_3V      1

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  SC_POWER_ON = 0x00,
  SC_RESET_LOW = 0x01,
  SC_RESET_HIGH = 0x02,
  SC_ACTIVE = 0x03,	 
  SC_ACTIVE_ON_T0 = 0x04,
  SC_POWER_OFF = 0x05
} SC_State;

/* ATR structure - Answer To Reset -------------------------------------------*/
typedef struct
{
	u8 cATR_Valid;       /* Flag that indicates the receive of an ATR */
  u8 TS;               /* Bit Convention */
  u8 T0;               /* High nibble = Number of setup byte; low nibble = Number of historical byte */
  u8 T[SETUP_LENGTH];  /* Setup array */
  u8 H[HIST_LENGTH];   /* Historical array */
  u8 Tlength;          /* Setup array dimension */
  u8 Hlength;          /* Historical array dimension */
	u8 ATR_ReciveLength;
	u8 CheckSumPresent;
} SC_ATR;

/* ADPU-Header command structure ---------------------------------------------*/
typedef struct
{
  u8 CLA;  /* Command class */
  u8 INS;  /* Operation code */
  u8 P1;   /* Selection Mode */
  u8 P2;   /* Selection Option */
} SC_Header;

/* ADPU-Body command structure -----------------------------------------------*/
typedef struct 
{
  u8 LC;           /* Data field length */
  u8 Data[LCmax];  /* Command parameters */
  u8 LE;           /* Expected length of data to be returned */
} SC_Body;

/* ADPU Command structure ----------------------------------------------------*/
typedef struct
{
  SC_Header Header;
  SC_Body Body;
} SC_ADPU_Commands;

/* SC response structure -----------------------------------------------------*/
typedef struct
{
  u8 Data[LCmax];  /* Data returned from the card */
  u8 SW1;          /* Command Processing status */
  u8 SW2;          /* Command Processing qualification */
} SC_ADPU_Responce;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* APPLICATION LAYER ---------------------------------------------------------*/
void SC_Handler(SC_State *SCState, SC_ADPU_Commands *SC_ADPU, SC_ADPU_Responce *SC_Response);
void SC_PowerCmd(FunctionalState NewState);
void SC_Reset(BitAction ResetState);
void SC_ParityErrorHandler(void);
void SC_PTSConfig(void);
void SmartCardInitInterface (void);
void SmartcardOn (void);
void SmartcardOff (void);
char RestartSmartcard (void);

void SC_SetHwParams	(u8 cBaudrateIndex,u8 cConversion,u8 Guardtime,u8 Waitingtime);
int CRD_SendCommand ( unsigned char *pTransmitBuffer,	
											unsigned int   nCommandSize,
											unsigned int   nExpectedAnswerSize,
											unsigned int  *nReceivedAnswerSize);

void GPIO_Configuration_Smartcard (void);
void Delay(u32 nCount);
void Delay_noUSBCheck (u32 nCount);
void InvalidateATR (void);
void initSCHardware (void);
int WaitForATR (void);

extern SC_ATR SC_A2R;

#endif /* __SMARTCARD_H */

