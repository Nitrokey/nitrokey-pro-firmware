/*
 * Author: Copyright (C) Rudolf Boeddeker                   Date: 2010-01-13
 *                                              STMicroelectronics              Date:   04/27/2009
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

#include "setjmp.h"
#include "stm32f10x.h"
#include "platform_config.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "CCID_usb.h"
#include "smartcard.h"
#include "ramdisk.h"
#include "keyboard.h"
#include "AccessInterface.h"
#include "hotp.h"
#include "report_protocol.h"
#include "CCIDHID_usb_prop.h"
#include "string.h"
#include "CcidLocalAccess.h"
#include "HandleAesStorageKey.h"


int nGlobalStickState = STICK_STATE_SMARTCARD;

int nFlagSendSMCardInserted = TRUE;

uint64_t currentTime = 0;

__IO uint8_t device_status = STATUS_READY;


jmp_buf jmpRestartUSB;          // reentrypoint for USB device change

void sendHOTPCodeForSlot(uint8_t slot_number);

#ifdef COMPILE_TEST
/*******************************************************************************

  Test2	- For pin toggle

*******************************************************************************/

void test2(void) {
  static int i = 0;

  i++;
  if (i & 1) {
    GPIO_ResetBits(GPIOB, GPIO_Pin_1); // org
  } else {
    GPIO_SetBits(GPIOB, GPIO_Pin_1);   // org
  }
}

/*******************************************************************************

  Test1	- For pin toggle

*******************************************************************************/

void Test1(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  // enable port clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  // set pin modes
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
#endif

/*******************************************************************************

  main							R8 Stick - RC development board

*******************************************************************************/


int main(void) {

  USB_SetDeviceConfiguration(STICK_STATE_SMARTCARD);

  setjmp (jmpRestartUSB); // entrypoint for the changed USB device

  Set_System();
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  SysTick_Config(720000);    // set systemtick to 10 ms - for delay ()


  /* Setup before USB startup */

  check_backups();
  SmartCardInitInterface();

#ifdef DEBUG_BOOT_LEDS
  SwitchSmartcardLED(ENABLE);
  SwitchOATHLED(ENABLE);
#endif
    USB_Start();
#ifdef DEBUG_BOOT_LEDS
  SwitchSmartcardLED(DISABLE);
  SwitchOATHLED(DISABLE);
#endif

  StartupCheck_u8();

#ifdef DEBUG_BOOT_LEDS
  VerifyBlinkCorrect(9999);
#endif

  /* Endless loop after USB startup */
  while (1) {
    if (device_status == STATUS_RECEIVED_REPORT) {
      device_status = STATUS_BUSY;
      parse_report(HID_SetReport_Value_tmp, HID_GetReport_Value_tmp);
      device_status = STATUS_READY;
    }

    if (numLockClicked) {
      numLockClicked = 0;
      uint8_t slot_number = ((uint8_t *) SLOTS_PAGE1_ADDRESS + GLOBAL_CONFIG_OFFSET)[0];
      sendHOTPCodeForSlot(slot_number);
    }

    if (capsLockClicked) {
      capsLockClicked = 0;
      uint8_t slot_number = ((uint8_t *) SLOTS_PAGE1_ADDRESS + GLOBAL_CONFIG_OFFSET)[1];
      sendHOTPCodeForSlot(slot_number);
    }

    if (scrollLockClicked) {
      scrollLockClicked = 0;
      uint8_t slot_number = ((uint8_t *) SLOTS_PAGE1_ADDRESS + GLOBAL_CONFIG_OFFSET)[2];
      sendHOTPCodeForSlot(slot_number);
    }

    CCID_CheckUsbCommunication();
    if (TRUE == nFlagSendSMCardInserted) {
      CCID_SendCardDetect(); // Send card detect to host
      nFlagSendSMCardInserted = FALSE;
    }

  }
}

void sendHOTPCodeForSlot(uint8_t slot_number) {
  if (slot_number <= 2) {
        OTP_slot *const otp_slot = (OTP_slot *) get_HOTP_slot_offset(slot_number);
        bool programmed = otp_slot->type != 0xFF;

        if (programmed) {
          uint32_t code = get_code_from_hotp_slot(slot_number);
          uint8_t config = get_hotp_slot_config(slot_number);

          if (config & (1 << SLOT_CONFIG_TOKENID))
            sendString(otp_slot->token_id, sizeof(otp_slot->token_id)-1); //last byte is keyboard config

          if (config & (1 << SLOT_CONFIG_DIGITS))
            sendNumberN(code, 8);
          else
            sendNumberN(code, 6);

          if (config & (1 << SLOT_CONFIG_ENTER))
            sendEnter();
        }
      }
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed (uint8_t * file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif
