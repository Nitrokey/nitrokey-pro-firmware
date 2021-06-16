/*
 * Author: Copyright (C) Rudolf Boeddeker                   Date: 2010-01-13
 *                                              STMicroelectronics
 *                                              MCD Application Team            Date:   04/27/2009
 *
 * This file is part of Nitrokey.
 *
 * Nitrokey is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Nitrokey is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nitrokey. If not, see <http://www.gnu.org/licenses/>.
 */

/* Includes ------------------------------------------------------------------ */
#include <stdlib.h>
#include <CcidLocalAccess.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_pwr.h>
#include <stm32f10x_bkp.h>
#include "stm32f10x_it.h"
#include "stm32f10x_systick.h"
#include "hw_config.h"
#include "sdcard.h"
#include "platform_config.h"
#include "mass_mal.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "CCID_usb.h"
#include "CCID_usb_prop.h"
#include "CCIDHID_usb_desc.h"
//#include "CCID_usb_desc.h"
#include "RAMDISK_usb_desc.h"

#include "CcidLocalAccess.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */

/* Extern variables ---------------------------------------------------------- */
Blink blinkOATH;
Blink blinkVerifyError;
Blink blinkVerifyCorrect;

/* Private function prototypes ----------------------------------------------- */
void RCC_Config (void);

/* Private functions --------------------------------------------------------- */
void DisableFirmwareDownloadPort (void);

#if DISABLE_FW_PORT == 1
void DisableFirmwareDownloadPort (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // enable port clock
    RCC_APB2PeriphClockCmd (FIRMWARE_DL_PERIPH, ENABLE);

    // set pin modes
    GPIO_InitStructure.GPIO_Pin = FIRMWARE_DL_PIN_1 | FIRMWARE_DL_PIN_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init (FIRMWARE_DL_PIN_PORT, &GPIO_InitStructure);
}
#endif

enum Hardware {
    HW_UNKNOWN = 0,
    HW_NON_BGA_REV3 = NK_HW_REV3_ID,
    HW_BGA_REV4 = NK_HW_REV4_ID
};
enum Hardware detect_hardware(void);
/*******************************************************************************

  DisableSmartcardLED

	not installed

*******************************************************************************/

void DisableSmartcardLED (void)
{
    /*
       GPIO_InitTypeDef GPIO_InitStructure;

       // enable port clock RCC_APB2PeriphClockCmd(SMARTCARD_LED_PERIPH, ENABLE);

       // set pin modes GPIO_InitStructure.GPIO_Pin = SMARTCARD_LED_PIN; GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; GPIO_Init(SMARTCARD_LED_PIN_PORT, &GPIO_InitStructure); */
}

/*******************************************************************************

  EnableSmartcardLED


*******************************************************************************/

void EnableSmartcardLED (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // enable port clock
    RCC_APB2PeriphClockCmd (SMARTCARD_LED_PERIPH, ENABLE);

    // set pin modes
    GPIO_InitStructure.GPIO_Pin = SMARTCARD_LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init (SMARTCARD_LED_PIN_PORT, &GPIO_InitStructure);

}

/*******************************************************************************

  EnableOATHLED


*******************************************************************************/

void EnableOATHLED (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // enable port clock
    RCC_APB2PeriphClockCmd (OATH_LED_PERIPH, ENABLE);

    // set pin modes
    GPIO_InitStructure.GPIO_Pin = OATH_LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init (OATH_LED_PIN_PORT, &GPIO_InitStructure);

}

/*******************************************************************************

  EnableButton

*******************************************************************************/
#if ENABLE_BUTTON == 1
void EnableButton (void)
{

    GPIO_InitTypeDef GPIO_InitStructure;


    RCC_APB2PeriphClockCmd (BUTTON_PERIPH, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init (BUTTON_PIN_PORT, &GPIO_InitStructure);
}
#endif
/*******************************************************************************

  EnableTimer2

*******************************************************************************/

void EnableTimer2 (void)
{


    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 9;
    TIM_TimeBaseStructure.TIM_Prescaler = 7200;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit (TIM2, &TIM_TimeBaseStructure);

    /* TIM2 enable counter */
    TIM_Cmd (TIM2, ENABLE);

    /* Enable TIM2 Update interrupt */
    TIM_ITConfig (TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);

}

/*******************************************************************************

 ReadButton

*******************************************************************************/

#if ENABLE_BUTTON == 1
uint8_t ReadButton (void)
{
    return GPIO_ReadInputDataBit (BUTTON_PIN_PORT, BUTTON_PIN);
}
#endif

/*******************************************************************************

 	SwitchSmartcardLED

*******************************************************************************/

void SwitchSmartcardLED (FunctionalState NewState)
{

    if (NewState == DISABLE)
    {
        GPIO_ResetBits (SMARTCARD_LED_PIN_PORT, SMARTCARD_LED_PIN);
    }
    else
    {
        GPIO_SetBits (SMARTCARD_LED_PIN_PORT, SMARTCARD_LED_PIN);
    }

}

/*******************************************************************************

 	SwitchOATHLED

*******************************************************************************/

void SwitchOATHLED (FunctionalState NewState)
{

    if (NewState == ENABLE)
    {
        GPIO_SetBits (OATH_LED_PIN_PORT, OATH_LED_PIN);
    }
    else
    {
        GPIO_ResetBits (OATH_LED_PIN_PORT, OATH_LED_PIN);
    }

}

void ClearAllBlinking(){
    Blink_init_all();
    SwitchOATHLED(DISABLE);
    SwitchSmartcardLED(DISABLE);
}

void StartBlinkingOATHLED(uint16_t times)
{
    blinkOATH.timesLeft = times;
}

void VerifyBlinkError(uint16_t times)
{
    blinkVerifyError.timesLeft = times;
}

void VerifyBlinkCorrect(uint16_t times)
{
    blinkVerifyCorrect.timesLeft = times;
}

void init_blinking(){
    Blink_init_all();
}

void reset_to_bootloader(void) {
    /* Boot loader magic number*/
    const uint32_t CMD_BOOT = 0x544F4F42UL;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    /* Write bootloader magic number to Backup registers*/
    BKP_WriteBackupRegister(BKP_DR1, (uint16_t) (CMD_BOOT & 0x0000FFFFUL));
    BKP_WriteBackupRegister(BKP_DR2, (uint16_t) ((CMD_BOOT & 0xFFFF0000UL) >> 16));

    NVIC_SystemReset();
}

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/

void exec_bootloader_if_wrong_hardware(void){
    const enum Hardware hardware = detect_hardware();
    const bool execute_bootloader = NK_HW_REV_ID != hardware;

    if (execute_bootloader) {
        reset_to_bootloader();
    }
}

void Set_System (void)
{
    /* RCC configuration */
    RCC_Config ();

    /* Enable and GPIOD clock */
    USB_Disconnect_Config ();

#if DISABLE_FW_PORT == 1
    DisableFirmwareDownloadPort ();
#endif

    init_blinking();
    EnableSmartcardLED ();
    EnableOATHLED ();

    /* Enable CRC calculator */
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_CRC, ENABLE);

    /* Enable Timer 2 */
    EnableTimer2 ();

#if ENABLE_BUTTON == 1
    /* Enable button */
    EnableButton ();
#endif

    /* MAL configuration */
    CCID_Init ();   // set CCID default values

    /* Enable SysTick Counter for time in firmware */
    SysTick_CounterCmd (SysTick_Counter_Enable);

}


/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/

void Set_USBClock (void)
{
    /* USBCLK = PLLCLK */
    RCC_USBCLKConfig (RCC_USBCLKSource_PLLCLK_1Div5);

    /* Enable USB clock */
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/

void Enter_LowPowerMode (void)
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

void Leave_LowPowerMode (void)
{
    DEVICE_INFO* pInfo = Device_Info;

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

void USB_Interrupts_Config (void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);

#ifdef USB_4BIT_SD_CARD_INTERFACE
    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);
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
        GPIO_SetBits (USB_DISCONNECT, USB_DISCONNECT_PIN);
    }
    else
    {
        GPIO_ResetBits (USB_DISCONNECT, USB_DISCONNECT_PIN);
    }
}

/*******************************************************************************

  HexToAscii

*******************************************************************************/

typedef struct {
  uint8_t left:4;
  uint8_t right:4;
} hexbyte;


char HexToAscii2 (uint8_t nHex, uint8_t byte_part)
{
  if (byte_part==1)
    nHex = ((hexbyte *) &nHex)->left;
  else
    nHex = ((hexbyte *) &nHex)->right;

  return HexToAscii(nHex);
}

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
* Description    : Create the serial number string descriptor using data got from
*                  the smartcard.
* Input          : None in params. Gets data from smartcard.
* Output         : None in params. Writes to CCID_StringSerial.
* Return         : None.
*******************************************************************************/

#ifdef DEVICE_NITROKEY_PRO
__IO uint32_t cardSerial = 0;
#endif

#include "string.h"

void Get_SerialNum (void)
{
#ifndef DEVICE_NITROKEY_PRO
  uint8_t buf[15] = { };
  const uint8_t serial_buf_size = sizeof(buf);
  const retCode results = getSerialNumber(buf, serial_buf_size);

  if (results.smartcard_ret != APDU_EOF) {
      *((uint32_t *) buf) = *((uint32_t *) &results);
      for (int i = 0; i < serial_buf_size; i++) {
          *((uint16_t *) CCID_StringSerial + i+1) = (uint8_t) HexToAscii2(buf[i/2], i%2);
      }
      return;
  }

  for (int i = 0; i < serial_buf_size; i++) {
      *((uint16_t *) CCID_StringSerial + i+1) = (uint8_t) buf[i];
  }
#endif //#ifndef DEVICE_NITROKEY_PRO

#ifdef DEVICE_NITROKEY_PRO
    uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

    uint8_t* SerialString;

    // Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8); // UNIQUE DEVICE ID
    // Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
    // Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

    // No device id
    // Device_Serial0 = 0;
    // Device_Serial1 = 0;
    // Device_Serial2 = 0;


    // device id from smart card

    getAID ();
    uint8_t b0 = getByteOfData (10);

    uint8_t b1 = getByteOfData (11);

    uint8_t b2 = getByteOfData (12);

    uint8_t b3 = getByteOfData (13);


    Device_Serial0 = b3 + (b2 << 8) + (b1 << 16) + (b0 << 24);

    cardSerial = Device_Serial0;

    Device_Serial1 = 0;
    Device_Serial2 = 0;


    // SerialString = MASS_StringSerial; // default value


    SerialString = CCID_StringSerial;

    // Create USB descriptor's serial number using smartcard's one and prefix it with zeroes
    // example: 000000000000000000005F11
    if (Device_Serial0 != 0)
    {
        SerialString[34] = HexToAscii ((uint8_t) ((Device_Serial0 & 0xF0000000) >> 28));
        SerialString[36] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x0F000000) >> 24));
        SerialString[38] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x00F00000) >> 20));
        SerialString[40] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x000F0000) >> 16));
        SerialString[42] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x0000F000) >> 12));
        SerialString[44] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x00000F00) >> 8));
        SerialString[46] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x000000F0) >> 4));
        SerialString[48] = HexToAscii ((uint8_t) ((Device_Serial0 & 0x0000000F) >> 0));

        SerialString[32] = HexToAscii ((uint8_t) ((Device_Serial1 & 0xF0000000) >> 28));
        SerialString[30] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x0F000000) >> 24));
        SerialString[28] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x00F00000) >> 20));
        SerialString[26] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x000F0000) >> 16));
        SerialString[24] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x0000F000) >> 12));
        SerialString[22] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x00000F00) >> 8));
        SerialString[20] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x000000F0) >> 4));
        SerialString[18] = HexToAscii ((uint8_t) ((Device_Serial1 & 0x0000000F) >> 0));

        SerialString[2]  = HexToAscii ((uint8_t) ((Device_Serial2 & 0xF0000000) >> 28));
        SerialString[4]  = HexToAscii ((uint8_t) ((Device_Serial2 & 0x0F000000) >> 24));
        SerialString[6]  = HexToAscii ((uint8_t) ((Device_Serial2 & 0x00F00000) >> 20));
        SerialString[8]  = HexToAscii ((uint8_t) ((Device_Serial2 & 0x000F0000) >> 16));
        SerialString[10] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x0000F000) >> 12));
        SerialString[12] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x00000F00) >> 8));
        SerialString[14] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x000000F0) >> 4));
        SerialString[16] = HexToAscii ((uint8_t) ((Device_Serial2 & 0x0000000F) >> 0));
    }
#endif
}

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void RCC_Config (void)
{
    /* Setup the microcontroller system. Initialize the Embedded Flash Interface, initialize the PLL and update the SystemFrequency variable. */
    SystemInit ();

}

enum Hardware g_current_hardware = HW_UNKNOWN;

enum Hardware detect_hardware(void) {
    /*
    * Check the hardware revision with the following:
    * 1. set B7 to input-pull up
    * 2. check if its high - low -> new hardware, high -> old hardware
    */

    if (g_current_hardware != HW_UNKNOWN) {
        return g_current_hardware;
    }

    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init (GPIOB, &GPIO_InitStructure);

    const uint8_t state = GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_7);
    if (state == 0) {
        g_current_hardware = HW_BGA_REV4;
    } else{
        g_current_hardware = HW_NON_BGA_REV3;
    }
    return g_current_hardware;
}

/*******************************************************************************
* Function Name  : MAL_Config
* Description    : MAL_layer configuration
* Input          : None.
* Return         : None.
*******************************************************************************/
void MAL_Config (void)
{
    // MAL_Init();

#ifdef USB_4BIT_SD_CARD_INTERFACE
    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_FSMC, ENABLE);
#endif
}

/*******************************************************************************
* Function Name  : USB_Disconnect_Config
* Description    : Disconnect pin configuration
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Disconnect_Config (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable USB_DISCONNECT GPIO clock */
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

    /* USB_DISCONNECT_PIN used as USB pull-up */
    GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_Init (USB_DISCONNECT, &GPIO_InitStructure);

    USB_Cable_Config (DISABLE);

}
