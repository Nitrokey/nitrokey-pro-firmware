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
#include "CCIDHID_usb_desc.h"

#define WBVAL(x) (x&0xff),((x>>8)&0xff)

const uint8_t CCID_DeviceDescriptor[CCID_SIZ_DEVICE_DESC] =
  {
    0x12,   				/* DeviceDescriptor bLength  */
    0x01,   				/* bDescriptorType */
  	WBVAL(0x0110), 	/* bcdUSB, version 1.10 */
    0x00,   				/* bDeviceClass : each interface define the device class */
    0x00,   				/* bDeviceSubClass */
    0x00,   				/* bDeviceProtocol */
    0x40,   				/* bMaxPacketSize0 0x08 - 0x40 = 64 */

  	WBVAL(0x20A0),  /* VID for CCID driver idVendor */
  	WBVAL(0x4108),  /* PID for CCID driver idProduct */
//  	WBVAL(0x0800),  /* Test VID for MS CCID driver idVendor */
//  	WBVAL(0x0006),  /* Test PID for MS CCID driver idProduct */
//  	WBVAL(0x08e6),  /* 0x08e6 = idVendor  germalto - for testing */
//  	WBVAL(0x3437),  /* 0x3437 = idProduct germalto usb-sl reader  - for testing */

  	WBVAL(0x0100), 	/* bcdDevice version 1.00   */

    1,              /* index of string Manufacturer  */
    2,              /* index of string descriptor of product*/
    3,  //3, RB            /* index of serial number */
    0x01    				/*bNumConfigurations */
  };

/*****************************************************************************/

#define USB_CONFIG_BUS_POWERED                 0x80
#define USB_CONFIG_REMOTE_WAKEUP               0x20

const uint8_t CCID_ConfigDescriptor[CCID_SIZ_CONFIG_DESC] = {
/* Configuration 1 */
  0x09,       												/* bLength ConfigDescriptor */
  0x02, 															/* bDescriptorType */
  CCID_SIZ_CONFIG_DESC,
  0x00,
  0x02,                              /* bNumInterfaces */
  0x01,                              /* bConfigurationValue */
  0x00,                              /* iConfiguration CCID = 6 ???? */
  USB_CONFIG_BUS_POWERED,            /* bmAttributes */
  100/2,          									 /* MaxPower 200 mA */
  
	// Interface 1 descriptor (Interface 1 = HID Keyboard)
  0x09,           										/* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,   		/* bDescriptorType */
  0x00,                              	/* bInterfaceNumber */
  0x00,                              	/* bAlternateSetting */
  0x01,                              	/* bNumEndpoints = 1 */
    0x03,         /*bInterfaceClass: HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
      /******************** Descriptor of Keyboard HID ********************/
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x10,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    KEYBOARD_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,

// Endpoint 1 descriptor (Interrupt in SCR)
  0x07,            									/* bLength */
	0x05,		// bDescriptorType: Endpoint descriptor type
	0x84,		// bEndpointAddress: Endpoint  1 IN
	0x03,		// bmAttributes: Interrupt endpoint
	0x08,		// wMaxPacketSize(LSB): 8 char max (0x0008)
	0x00,		// wMaxPacketSize (MSB)
	0x0A,
/* Terminator */
//  0                                  /* bLength */


// Interface 0 descriptor (Interface 0 = Smart Card Reader)
  0x09,           										/* bLength */
  0x04,     													/* bDescriptorType */
  0x01,                              	/* bInterfaceNumber */
  0x00,                              	/* bAlternateSetting */
  0x03,                              	/* bNumEndpoints = 3 */
  0x0B,             									/* bInterfaceClass = CCID */
  0x00,                 						 	/* bInterfaceSubClass */
  0x00,            									 	/* bInterfaceProtocol */
  0x00,                              	/* 0x04,  = 0 ??? iInterface Index of string descriptor (befor 0x64)*/

// CCID class descriptor
	0x36,															// bLength: CCID Descriptor size
	0x21,															// bDescriptorType: HID To be updated with CCID specific number
	0x10,															// bcdHID(LSB): CCID Class Spec release number (1.10)
	0x01,															// bcdHID(MSB)

	0x00,															// bMaxSlotIndex
	0x02,															// bVoltageSupport: 3v

	0x02,0x00,0x00,0x00,							// dwProtocols: supports T=1
	0x10,0x0E,0x00,0x00,							// dwDefaultClock: 3,6 Mhz (0x00000E10)
	0x10,0x0E,0x00,0x00,							// dwMaximumClock: 3,6 Mhz (0x00000E10)
	0x00,															// bNumClockSupported => only default clock

	0xCD,0x25,0x00,0x00,							// dwDataRate:      9677 bps (0x000025CD)
	0xA1,0xC5,0x01,0x00,							// dwMaxDataRate: 116129 bps (0x0001C5A1)
	0x00,															// bNumDataRatesSupported => no manual setting

	0xFE,0x00,0x00,0x00,   						/* dwMaxIFSD: 254 */
	0x00,0x00,0x00,0x00,  						/* dwSynchProtocols  */
	0x00,0x00,0x00,0x00,  						/* dwMechanical: no special characteristics */

//	0x3E,0x00,0x02,0x00,							// instand pc reset on XP :-)
	0xBA,0x04,0x01,0x00,							// 000104BAh
																		// 00000002h Automatic parameter configuration based on ATR data
																		// 00000008h Automatic ICC voltage selection
																		// 00000010h Automatic ICC clock frequency change
																		// 00000020h Automatic baud rate change
																		// 00000080h Automatic PPS
																		// 00000400h Automatic IFSD exchange
																		// 00010000h TPDU level exchanges with CCID

//	0x24,0x00,0x00,0x00,   						/* dwMaxCCIDMessageLength : Maximun block size + header*/
	0x0F,0x01,0x00,0x00,   						/* dwMaxCCIDMessageLength : Maximun block size + header*/
							   	  								/* 261 + 10   */

	0x00,															/* bClassGetResponse*/
	0x00,															/* bClassEnvelope */
	0x00,0x00,												/* wLcdLayout */
	0x00,															/* bPINSupport : no PIN verif and modif  */
	0x01,															/* bMaxCCIDBusySlots	*/

// Endpoint 4 descriptor (Interrupt in SCR)
  0x07,            									/* bLength */
	0x05,															// bDescriptorType: Endpoint descriptor type
	0x81,															// bEndpointAddress: Endpoint  4 IN
	0x03,															// bmAttributes: Interrupt endpoint
	0x40,		// 0x08,	zur sicherheit 0x40  // wMaxPacketSize(LSB): 8 char max (0x0008)
	0x00,															// wMaxPacketSize (MSB)
	0x18,       											// bInterval: Polling Interval (24 ms = 0x18)


// Endpoint 2 descriptor (Bulk out SCR)
  0x07,            									/* bLength */
	0x05,															// bDescriptorType: Endpoint descriptor type
	0x02,															// bEndpointAddress: Endpoint  2 OUT
	0x02,															// bmAttributes: Bulk endpoint
	0x40,															// wMaxPacketSize(LSB): 64 char max (0x0040)
	0x00,															// wMaxPacketSize (MSB)
	0x00,       											// bInterval: ignored
	
// Endpoint 3 descriptor (Bulk in SCR)
  0x07,            									/* bLength */
	0x05,															// bDescriptorType: Endpoint descriptor type
	0x82,															// RB to avoid doublebuffering ?	0x82,// bEndpointAddress: Endpoint  2 IN
	0x02,															// bmAttributes: Bulk endpoint
	0x40,															// wMaxPacketSize(LSB): 64 char max (0x0040)
	0x00,															// wMaxPacketSize (MSB)
	0x00,       											// bInterval: ignored


};


const uint8_t Keyboard_ReportDescriptor[KEYBOARD_SIZ_REPORT_DESC] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
//	0x85, 0x01,					   // REPORT_ID (1)	
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs) 
	0x09, 0x03,						//	USAGE (Keyboard ErrorUndefine)
	0x75, 0x08,						//  REPORT_SIZE (8)
	0x95, KEYBOARD_FEATURE_COUNT,						//	REPORT_COUNT (64)
	0xB1, 0x02,					    //	FEATURE (Data,Var,Abs)
	
    0xc0,                           // END_COLLECTION

}; 

/*****************************************************************************/

const uint8_t CCID_StringLangID[CCID_SIZ_STRING_LANGID] =
  {
    CCID_SIZ_STRING_LANGID,
    0x03,
    0x09,
    0x04
  };      /* LangID = 0x0409: U.S. English */


const uint8_t CCID_StringVendor[CCID_SIZ_STRING_VENDOR] =
  {
    CCID_SIZ_STRING_VENDOR, 		/* Size of manufaturer string */
    0x03,           						/* bDescriptorType = String descriptor */
    /* Manufacturer: "Crypto Stick" */
    'C', 0, 'r', 0, 'y', 0, 'p', 0, 't', 0, 'o', 0, ' ', 0, 'S', 0,
    't', 0, 'i', 0, 'c', 0, 'k', 0

  };


const uint8_t CCID_StringProduct[CCID_SIZ_STRING_PRODUCT] =
  {
    CCID_SIZ_STRING_PRODUCT,
    0x03,
    /* Product name: "Crypto Stick v1.4" */
    'C', 0, 'r', 0, 'y', 0, 'p', 0, 't', 0, 'o', 0, ' ', 0, 'S', 0,
    't', 0, 'i', 0, 'c', 0, 'k', 0, ' ', 0, 'v', 0, '1', 0, '.', 0,
	  '4', 0

  };


uint8_t CCID_StringSerial[CCID_SIZ_STRING_SERIAL] =
{
    CCID_SIZ_STRING_SERIAL,
    0x03,
/* Serial number  "            " - (3 * 8 Byte defined in sourcecode)* 2 + 2 = 50 byte  */
    ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0,
    ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0,
    ' ', 0, ' ', 0, ' ', 0, ' ', 0
};


const uint8_t CCID_StringInterface[CCID_SIZ_STRING_INTERFACE] =
  {
    CCID_SIZ_STRING_INTERFACE,
    0x03,
    /* Interface 0: "CS SmCd" */
    'C', 0, 'S', 0, ' ', 0, 'S', 0, 'm', 0, 'C', 0, 'd', 0
  };


