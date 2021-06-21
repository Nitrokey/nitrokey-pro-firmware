/*
 * Author: Copyright (C) Rudolf Boeddeker                   Date: 2010-01-13
 *                                              STMicroelectronics              Date:   04/27/2009
 *                                               MCD Application Team           Version V3.0.1
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


/* Define to prevent recursive inclusion ------------------------------------- */
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------ */
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------ */
/* Exported constants -------------------------------------------------------- */
/* Uncomment the line corresponding to the STMicroelectronics evaluation board used to run the example */

#undef 	USE_STM3210E_EVAL
#define USE_STM3210B_EVAL   // RB used for USE_STM3210_KEIL_EVAL

#define USE_BOARD_STICK_V12 // Cheange CPU to V12 = R8 else Keil = RC !!!!

/******************************************************************************

	USB_4BIT_SD_CARD_INTERFACE

	activate the 4 Bit DMA SD card interface

******************************************************************************/
// FIXME check and remove if not needed (SD card communication?)
#define USB_4BIT_SD_CARD_INTERFACE  // only RC CPU !!!!


/* Define the STM32F10x hardware depending on the used evaluation board */
#ifdef USE_STM3210B_EVAL

#define USB_DISCONNECT                    GPIOA
#define USB_DISCONNECT_PIN                GPIO_Pin_15   // Use GPIO_Pin_10
                                                        // for older PCB
#define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOA


/* Smartcard Inteface GPIO pins */
//Table 50. Vector table for STM32F100xx device, Doc ID16188 Rev 5, 8.1.2 Interrupt and exception vectors
#define EXTI9_5_IRQChannel           ((unsigned char)0x17)  /* External Line [9:5] Ipterrupts */
#define EXTI15_10_IRQChannel           ((unsigned char)0x28)  /* EXTI Line[15:10] Ipterrupts */
#define USART1_IRQChannel            ((unsigned char)0x25)  /* USART1 global Interrupt */
#define USART3_IRQChannel            ((unsigned char)0x27)  /* USART3 global Interrupt */

// Old hardware pins
// SCCLK PA8
// SCSDA PB6
/* Disable JTAG to be able to use PB3 */
// SCRST PB3
// SCVCC PB4 PB5


// for the new hardware
// move from USART1 to USART3
// USART3 no remap, 7.3.5 USART alternate function remapping, Doc ID16188 Rev 5
// USART3_TX PB10
// USART3_RX PB11
// USART3_CK PB12

// correct DMA to use USART3, both DMA1 controller, channels:
// USART1_TX 4 USART1_RX 5 (old)
// USART3_TX 2 USART3_RX 3 (new)

// ports
// USART1 APB2 (old)
// USART3 APB1 (new)

// clocks
// USART1 CLK2 APB2
// USART3 CLK1 APB1

// USART3/APB1, GPIOB+GPIOD/APB2, data: PB10-12, power: PB4 PD2

#if BUILD_DEBUG == 1
#define DEBUG_BOOT_LEDS
#endif

// TODO
#define DISABLE_FW_PORT 0
#define ENABLE_BUTTON 0
#define NVIC_IRQ 0

// COMMON
#define SMARTCARD_PCLK1_DIV                 RCC_HCLK_Div2
#define SMARTCARD_PCLK2_DIV                 RCC_HCLK_Div1
#define BUTTON_PERIPH						RCC_APB2Periph_GPIOA
#define BUTTON_PIN_PORT						GPIOA
#define BUTTON_PIN						    GPIO_Pin_0
// for disabling download firmware pins, used parallel to usb
#define FIRMWARE_DL_PERIPH                  RCC_APB2Periph_GPIOC
#define FIRMWARE_DL_PIN_PORT                GPIOC
#define FIRMWARE_DL_PIN_1                   GPIO_Pin_4
#define FIRMWARE_DL_PIN_2                   GPIO_Pin_5


#define SMARTCARD_USART                     (detect_hardware()->usart.usart)
#define SMARTCARD_USART_ClockCmd            (detect_hardware()->usart.clock_cmd)
#define SMARTCARD_USART_Periph              (detect_hardware()->usart.usart_peripheral)
#define SMARTCARD_USART_AFIO                (detect_hardware()->usart.afio_peripheral)
#define SMARTCARD_USART_REMAP               (detect_hardware()->usart.remap_mapping)
#define SMARTCARD_USART_REMAP_VALUE         (detect_hardware()->usart.remap_mapping_value)

#define SMARTCARD_POWER_PORT                (detect_hardware()->pins.power_port_1.port)
#define SMARTCARD_POWER_PIN_1            	(detect_hardware()->pins.power_port_1.pin_number)
#define SMARTCARD_POWER_PORT_2              (detect_hardware()->pins.power_port_2.port)
#define SMARTCARD_POWER_PIN_2            	(detect_hardware()->pins.power_port_2.pin_number)
#define SMARTCARD_USART_Periph_POWER_1      (get_peripheral_for_port(SMARTCARD_POWER_PORT))
#define SMARTCARD_USART_Periph_POWER_2      (get_peripheral_for_port(SMARTCARD_POWER_PORT_2))

#define SMARTCARD_PCLK_STATUS_FREQ(RCC_CLOCK_STATUS_PTR)          (get_clock_for_map(RCC_CLOCK_STATUS_PTR, detect_hardware()->clock.map_clock))

#define SMARTCARD_SCCLK_PORT                (detect_hardware()->pins.sc_clk.port)
#define SMARTCARD_SCCLK_PIN                 (detect_hardware()->pins.sc_clk.pin_number)
#define SMARTCARD_SCCLK_MODE                (detect_hardware()->pins.sc_clk.mode)
#define SMARTCARD_SCSDA_PORT                (detect_hardware()->pins.sc_sda.port)
#define SMARTCARD_SCSDA_PIN                 (detect_hardware()->pins.sc_sda.pin_number)
#define SMARTCARD_SCSDA_MODE                (detect_hardware()->pins.sc_sda.mode)
#define SMARTCARD_SCRST_PORT                (detect_hardware()->pins.sc_rst.port)
#define SMARTCARD_SCRST_PIN                 (detect_hardware()->pins.sc_rst.pin_number)
#define SMARTCARD_SCRST_PERI                (get_peripheral_for_port(SMARTCARD_SCRST_PORT))
#define SMARTCARD_SCRST_MODE                (detect_hardware()->pins.sc_rst.mode)
#define SMARTCARD_PRESCALER                 (detect_hardware()->clock.prescaler_value)

#define SMARTCARD_USART_IRQChannel          (detect_hardware()->interrupts.usart)
#define SC_EXTI_IRQ                         (detect_hardware()->interrupts.exti)

// port for possible smartcard LED
#define SMARTCARD_LED_PERIPH				(get_peripheral_for_port(SMARTCARD_LED_PIN_PORT))
#define SMARTCARD_LED_PIN_PORT				(detect_hardware()->led.smartcard.port)
#define SMARTCARD_LED_PIN					(detect_hardware()->led.smartcard.pin_number)
#define OATH_LED_PERIPH						(get_peripheral_for_port(OATH_LED_PIN_PORT))
#define OATH_LED_PIN_PORT					(detect_hardware()->led.oath.port)
#define OATH_LED_PIN						(detect_hardware()->led.oath.pin_number)


#endif /* USE_STM3210B_EVAL */



/**** Definitions for global stickstate ****/

#define STICK_STATE_RAMDISK						0
#define STICK_STATE_SD_DISK						1
#define STICK_STATE_SMARTCARD					2
#define STICK_STATE_FLASHDISK					3
#define STICK_STATE_COMPOSITE					4
#define STICK_STATE_COMPOSITE_SC_SD		5

extern int nGlobalStickState;


/* Exported macro ------------------------------------------------------------ */
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/**
 *
 * Hardware rev4 (BGA):
 * use USART3 instead of USART1
 * move PB10 -> data pin
 * PB12 -> clock pin
 *
 * Set ALL of these to input
 * (verify default setting of the pins - current assumption: set to default on reset)
 *
 * Current state:
 * shorted to ground:
 * C15
 * B7
 *
 * shorted to power:
 * C0
 * C7
 * C8
 *
 * shorted to BOOT:
 * C5
 * B9
 *
 * connected to smart card data pin:
 * B13
 *
 * Check the hardware revision with the following:
 * 1. set B7 to input-pull up
 * 2. check if its high - low -> new hardware, high -> old hardware
 */

/**
* 5.1 Avoid floating unused pinDo not leave unused pin floating. Connect it either to ground or to supply on the PCB, or use PU / PD. Noise on non-connected input pin is a source of extra consumption by making the input buffer switch randomly.If the application is sensitive to ESD, prefer a connection to ground or define the pin as PP output and drive it to low.
 * AN4899GPIO hardware guideline DocID029601 Rev 123/31
 *
 * 6.1 Configure unused GPIO input as analog input GPIO always have an input channel, which can be either digital or analog.If it is not necessary to read the GPIO data, prefer the configuration as analog input. This saves the consumption of the input Schmitt trigger.
*/