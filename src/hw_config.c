/*
* Author: Copyright (C) Rudolf Boeddeker 					Date: 2010-01-13
*												STMicroelectronics
*												MCD Application Team			Date:	04/27/2009
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "hw_config.h"
#include "sdcard.h"
#include "platform_config.h"
#include "mass_mal.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "CCID_usb.h"
#include "CCID_usb_prop.h"
#include "CCID_usb_desc.h"
#include "RAMDISK_usb_desc.h"

#include "CcidLocalAccess.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
uint8_t blinkOATHLEDTimes=0;
uint64_t lastOATHBlinkTime=0;
/* Private function prototypes -----------------------------------------------*/
void RCC_Config(void);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************

  DisableFirmwareDownloadPort

*******************************************************************************/

void DisableFirmwareDownloadPort (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

// enable port clock
  RCC_APB2PeriphClockCmd(FIRMWARE_DL_PERIPH, ENABLE);

// set pin modes
  GPIO_InitStructure.GPIO_Pin   = SMARTCARD_POWER_PIN_1 | SMARTCARD_POWER_PIN_2;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(FIRMWARE_DL_PIN_PORT, &GPIO_InitStructure);

}

/*******************************************************************************

  DisableSmartcardLED

	not installed

*******************************************************************************/

void DisableSmartcardLED (void)
{
/*
	GPIO_InitTypeDef GPIO_InitStructure;

// enable port clock
  RCC_APB2PeriphClockCmd(SMARTCARD_LED_PERIPH, ENABLE);

// set pin modes
  GPIO_InitStructure.GPIO_Pin   = SMARTCARD_LED_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SMARTCARD_LED_PIN_PORT, &GPIO_InitStructure);
*/
}

/*******************************************************************************

  EnableSmartcardLED


*******************************************************************************/

void EnableSmartcardLED (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

// enable port clock
  RCC_APB2PeriphClockCmd(SMARTCARD_LED_PERIPH, ENABLE);

// set pin modes
  GPIO_InitStructure.GPIO_Pin   = SMARTCARD_LED_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SMARTCARD_LED_PIN_PORT, &GPIO_InitStructure);

}

/*******************************************************************************

  EnableOATHLED


*******************************************************************************/

void EnableOATHLED (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

// enable port clock
  RCC_APB2PeriphClockCmd(OATH_LED_PERIPH, ENABLE);

// set pin modes
  GPIO_InitStructure.GPIO_Pin   = OATH_LED_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(OATH_LED_PIN_PORT, &GPIO_InitStructure);

}

/*******************************************************************************

  EnableButton

*******************************************************************************/

void EnableButton (void)
{

  GPIO_InitTypeDef  GPIO_InitStructure;


  RCC_APB2PeriphClockCmd(BUTTON_PERIPH, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  BUTTON_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(BUTTON_PIN_PORT, &GPIO_InitStructure);
}

/*******************************************************************************

  EnableTimer2

*******************************************************************************/

void EnableTimer2 (void)
{


/* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

/* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 9;
  TIM_TimeBaseStructure.TIM_Prescaler = 7200;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

/* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);

    /* Enable TIM2 Update interrupt */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/*******************************************************************************

 ReadButton

*******************************************************************************/

uint8_t ReadButton(void){
return GPIO_ReadInputDataBit(BUTTON_PIN_PORT,BUTTON_PIN);
}

/*******************************************************************************

 	SwitchSmartcardLED

*******************************************************************************/

void SwitchSmartcardLED (FunctionalState NewState)
{

  if (NewState == ENABLE)
  {
    GPIO_ResetBits(SMARTCARD_LED_PIN_PORT, SMARTCARD_LED_PIN);
  }
  else
  {
    GPIO_SetBits(SMARTCARD_LED_PIN_PORT, SMARTCARD_LED_PIN);
  }

}

/*******************************************************************************

 	SwitchOATHLED

*******************************************************************************/

void SwitchOATHLED (FunctionalState NewState)
{

  if (NewState == ENABLE)
  {
    GPIO_SetBits(OATH_LED_PIN_PORT, OATH_LED_PIN);
  }
  else
  {
    GPIO_ResetBits(OATH_LED_PIN_PORT, OATH_LED_PIN);
  }

}


void StartBlinkingOATHLED(uint8_t times){
blinkOATHLEDTimes+=times;

}

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/

void Set_System(void)
{
/* RCC configuration */
  RCC_Config();

/* Enable and GPIOD clock */
  USB_Disconnect_Config();

/* Disable firmware download port */
 	DisableFirmwareDownloadPort ();

EnableSmartcardLED ();
EnableOATHLED ();

/*Enable CRC calculator*/
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);

/*Enable Timer 2 */
 EnableTimer2();

/* Enable button */
	EnableButton();

/* MAL configuration */
	CCID_Init();	// set CCID default values

}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/

void Set_USBClock(void)
{
  /* USBCLK = PLLCLK */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/

void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/

void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }

}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/

void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

#ifdef USB_4BIT_SD_CARD_INTERFACE
  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif /* USB_4BIT_SD_CARD_INTERFACE */

}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : None.
* Return         : Status
*******************************************************************************/

void USB_Cable_Config (FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}

/*******************************************************************************

  HexToAscii

*******************************************************************************/

char HexToAscii (uint8_t nHex)
{
	if (10 > nHex)
	{
		return (nHex + '0');
	}
	return (nHex + 'A' - 10);
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

__IO uint32_t cardSerial=0;;

void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;
	uint8_t  *SerialString;

//  Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);			// UNIQUE DEVICE ID
//  Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
//  Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

// No device id
//  Device_Serial0 = 0;
//  Device_Serial1 = 0;
//  Device_Serial2 = 0;


//device id from smart card

getAID();
uint8_t b0=getByteOfData(10);
uint8_t b1=getByteOfData(11);
uint8_t b2=getByteOfData(12);
uint8_t b3=getByteOfData(13);


  Device_Serial0 = b3+(b2<<8)+(b1<<16)+(b0<<24);

  cardSerial=Device_Serial0;

  Device_Serial1 = 0;
  Device_Serial2 = 0;


	//SerialString = MASS_StringSerial; // default value


SerialString = CCID_StringSerial;


  if (Device_Serial0 != 0)
  {
    SerialString[48] = HexToAscii ((uint8_t) ((Device_Serial2 & 0xF0000000) >> 28));
    SerialString[46] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x0F000000) >> 24));
    SerialString[44] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x00F00000) >> 20));
    SerialString[42] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x000F0000) >> 16));
    SerialString[40] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x0000F000) >> 12));
    SerialString[38] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x00000F00) >>  8));
    SerialString[36] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x000000F0) >>  4));
    SerialString[34] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x0000000F) >>  0));

    SerialString[32] = HexToAscii ((uint8_t) ((Device_Serial1 & 0xF0000000) >> 28));
    SerialString[30] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x0F000000) >> 24));
    SerialString[28] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x00F00000) >> 20));
    SerialString[26] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x000F0000) >> 16));
    SerialString[24] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x0000F000) >> 12));
    SerialString[22] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x00000F00) >>  8));
    SerialString[20] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x000000F0) >>  4));
    SerialString[18] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x0000000F) >>  0));

    SerialString[2] = HexToAscii ((uint8_t) ((Device_Serial0 & 0xF0000000) >> 28));
    SerialString[4] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x0F000000) >> 24));
    SerialString[6] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x00F00000) >> 20));
    SerialString[8] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x000F0000) >> 16));
    SerialString [10] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x0000F000) >> 12));
    SerialString [12] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x00000F00) >>  8));
    SerialString [14] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x000000F0) >>  4));
    SerialString [16] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x0000000F) >>  0));
  }
}

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void RCC_Config(void)
{
 /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();

}

/*******************************************************************************
* Function Name  : MAL_Config
* Description    : MAL_layer configuration
* Input          : None.
* Return         : None.
*******************************************************************************/
void MAL_Config(void)
{
 // MAL_Init();

#ifdef USB_4BIT_SD_CARD_INTERFACE
  /* Enable the FSMC Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
#endif
}

/*******************************************************************************
* Function Name  : USB_Disconnect_Config
* Description    : Disconnect pin configuration
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Disconnect_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable USB_DISCONNECT GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

  /* USB_DISCONNECT_PIN used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin   = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;

  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

  USB_Cable_Config(DISABLE);

}

