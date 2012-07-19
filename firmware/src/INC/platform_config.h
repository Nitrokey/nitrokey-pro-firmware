/*
* Author: Copyright (C) Rudolf Boeddeker 					Date: 2010-01-13
*												STMicroelectronics	 			Date:	04/27/2009
*												 MCD Application Team			Version V3.0.1
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
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */

#undef 	USE_STM3210E_EVAL
#define USE_STM3210B_EVAL 						// RB used for USE_STM3210_KEIL_EVAL

#define USE_BOARD_STICK_V12					// Cheange CPU to V12 = R8 else Keil = RC	!!!!

/******************************************************************************

	USB_4BIT_SD_CARD_INTERFACE

	activate the 4 Bit DMA SD card interface

******************************************************************************/
#define USB_4BIT_SD_CARD_INTERFACE		// only RC CPU !!!!


/* Define the STM32F10x hardware depending on the used evaluation board */
#ifdef USE_STM3210B_EVAL

//#define USB_DISCONNECT                    GPIOC  										 	// development board + Stick V12
//#define USB_DISCONNECT_PIN                GPIO_Pin_6									
//#define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOC

#define USB_DISCONNECT                    GPIOD  										 	// development board + Stick V12
#define USB_DISCONNECT_PIN                GPIO_Pin_2									
#define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOD
/*
#ifdef USE_BOARD_STICK_V12
#else
  #define USB_DISCONNECT                    GPIOD  									 		// 
  #define USB_DISCONNECT_PIN                GPIO_Pin_2									
  #define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOD
#endif
#ifdef USE_BOARD_STICK_V12
#else
#endif
*/
// smartcard power supply
#define SMARTCARD_POWER_PORT              GPIOB  												// change SC_PortSource in smartcard.h too 
#define SMARTCARD_POWER_PIN_1            	GPIO_Pin_13										// change SC_PinSource  in smartcard.h too
#define SMARTCARD_POWER_PIN_2            	GPIO_Pin_14										// change SC_PinSource  in smartcard.h too



// for disabling download firmware pins, used parallel to usb
#define FIRMWARE_DL_PERIPH							 	RCC_APB2Periph_GPIOC
#define FIRMWARE_DL_PIN_PORT							GPIOC											 	
#define FIRMWARE_DL_PIN_1									GPIO_Pin_1												  
#define FIRMWARE_DL_PIN_2									GPIO_Pin_2

// port for possible smartcard LED

#ifdef USE_BOARD_STICK_V12
	#define SMARTCARD_LED_PERIPH							RCC_APB2Periph_GPIOA		 	// Stick V12
	#define SMARTCARD_LED_PIN_PORT						GPIOA											 	
	#define SMARTCARD_LED_PIN									GPIO_Pin_2												  
#else
	#define SMARTCARD_LED_PERIPH							RCC_APB2Periph_GPIOB			// development board
	#define SMARTCARD_LED_PIN_PORT						GPIOB											 	
	#define SMARTCARD_LED_PIN									GPIO_Pin_15												  
#endif

#endif /* USE_STM3210B_EVAL */

#define BUTTON_PERIPH						RCC_APB2Periph_GPIOA
#define BUTTON_PIN_PORT						GPIOA											 	
#define BUTTON_PIN						GPIO_Pin_0		


/**** Definitions for global stickstate ****/

#define STICK_STATE_RAMDISK						0
#define STICK_STATE_SD_DISK						1
#define STICK_STATE_SMARTCARD					2
#define STICK_STATE_FLASHDISK					3
#define STICK_STATE_COMPOSITE					4
#define STICK_STATE_COMPOSITE_SC_SD		5

extern int  nGlobalStickState;


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */


