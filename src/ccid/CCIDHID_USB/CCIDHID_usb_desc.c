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
#include "CCIDHID_usb_desc.h"

#define WBVAL(x) (x&0xff),((x>>8)&0xff)

const uint8_t CCID_DeviceDescriptor[CCID_SIZ_DEVICE_DESC] = {
    0x12,           // bLength           Descriptor size (18 bytes)
    0x01,           // bDescriptorType   Descriptor type (device)
    WBVAL (0x0110), // bcdUSB            USB version 1.1
    0x00,           // bDeviceClass      Device class (specified at interface)
    0x00,           // bDeviceSubClass
    0x00,           // bDeviceProtocol
    0x40,           // bMaxPacketSize0   Maximum packet size (64)

    WBVAL (GLOBAL_VID), // idVendor      VID
    WBVAL (GLOBAL_PID), // idProduct     PID
    // WBVAL(0x0800), // Test VID for MS CCID driver idVendor
    // WBVAL(0x0006), // Test PID for MS CCID driver idProduct
    // WBVAL(0x08e6), // 0x08e6 = idVendor germalto - for testing
    // WBVAL(0x3437), // 0x3437 = idProduct germalto usb-sl reader - for

    WBVAL (0x0100), // bcdDevice version 1.00

    0x01,  // iManufacturer       Index of string Manufacturer
    0x02,  // iProduct            Index of string descriptor of product
    0x03,  // iSerialNumber       Index of serial number
    0x01   // bNumConfigurations  Number of configurations
};

/*****************************************************************************/

#define USB_CONFIG_BUS_POWERED                 0x80
#define USB_CONFIG_REMOTE_WAKEUP               0x20

const uint8_t CCID_ConfigDescriptor[CCID_SIZ_CONFIG_DESC] = {

    // Configuration descriptor

    0x09,     // bLength              Descriptor size (9 bytes)
    0x02,     // bDescriptorType      Descriptor type (configuration)
    WBVAL (CCID_SIZ_CONFIG_DESC),
              // wTotalLength         Length of this + subordinate descriptors
    0x02,     // bNumInterfaces       Number of interfaces
    0x01,     // bConfigurationValue  Index of this configuration
    0x00,     // iConfiguration       Configuration string (none)
    0x80,     // bmAttributes         Bus powered
    100 / 2,  // bMaxPower            100 mA

    // Interface 0 descriptor (Generic HID)

    0x09, // bLength               Descriptor size (9 bytes)
    0x04, // bDescriptorType       Descriptor type (interface)
    0x00, // bInterfaceNumber      Interface Number
    0x00, // bAlternateSetting     Alternate Setting Number (none)
    0x01, // bNumEndpoints         Number of endpoints
    0x03, // bInterfaceClass       Interface class (HID)
    0x00, // bInterfaceSubClass    Interface subclass
    0x00, // nInterfaceProtocol    Interface protocol
    0x00, // iInterface            Interface string index (none)

    // Generic HID class descriptor

    0x09,       // bLength           Descriptor size (9 bytes)
    0x21,       // bDescriptorType   Descriptor type (HID)
    0x10, 0x01, // bcdHID            HID release number (1.1)
    0x00,       // bCountryCode      Hardware target country (not supported)
    0x01,       // bNumDescriptors   Number class descriptors
    0x22,       // bDescriptorType   Class descriptor type (report)
    WBVAL (KEYBOARD_SIZ_REPORT_DESC),
                // wItemLength       Report descriptor size

    // Interrupt IN endpoint 4 descriptor

    0x07,       // bLength           Descriptor size (7 bytes)
    0x05,       // bDescriptorType   Descriptor type (endpoint)
    0x84,       // bEndpointAddress  Endpoint 4 IN
    0x03,       // bmAttributes      Transfer type (interrupt)
    0x08, 0x00, // wMaxPacketSize    Maximum packet size (8 bytes)
    0x0A,       // bInterval

    // Interface 1 descriptor (Smart Card Reader)

    0x09,   // bLength               Descriptor size (9 bytes)
    0x04,   // bDescriptorType       Descriptor type (interface)
    0x01,   // bInterfaceNumber      Interface Number
    0x00,   // bAlternateSetting     Alternate Setting Number (none)
    0x03,   // bNumEndpoints         Number of endpoints
    0x0B,   // bInterfaceClass       Interface class (CCID)
    0x00,   // bInterfaceSubClass    Interface subclass
    0x00,   // nInterfaceProtocol    Interface protocol
    0x00,   // iInterface            Interface string index (none)

    // CCID class descriptor

    0x36,       // bLength          CCID Descriptor size
    0x21,       // bDescriptorType  HID To be updated with CCID specific number
    0x10, 0x01, // bcdHID           CCID Class Spec release number (1.10)

    0x00,       // bMaxSlotIndex
    0x02,       // bVoltageSupport  3v

    0x02, 0x00, 0x00, 0x00, // dwProtocols              supports T=1
    0x10, 0x0E, 0x00, 0x00, // dwDefaultClock           3,6 Mhz (0x00000E10)
    0x10, 0x0E, 0x00, 0x00, // dwMaximumClock           3,6 Mhz (0x00000E10)
    0x00,                   // bNumClockSupported       only default clock

    0xCD, 0x25, 0x00, 0x00, // dwDataRate               9677 bps (0x000025CD)
    0xA1, 0xC5, 0x01, 0x00, // dwMaxDataRate            116129 bps (0x0001C5A1)
    0x00,                   // bNumDataRatesSupported   no manual setting

    0xFE, 0x00, 0x00, 0x00, // dwMaxIFSD                254
    0x00, 0x00, 0x00, 0x00, // dwSynchProtocols
    0x00, 0x00, 0x00, 0x00, // dwMechanical             no special characteristics

    // 0x3E,0x00,0x02,0x00, // instand pc reset on XP :-)
    0xBA, 0x04, 0x01, 0x00, // 000104BAh
    // 00000002h Automatic parameter configuration based on ATR data
    // 00000008h Automatic ICC voltage selection
    // 00000010h Automatic ICC clock frequency change
    // 00000020h Automatic baud rate change
    // 00000080h Automatic PPS
    // 00000400h Automatic IFSD exchange
    // 00010000h TPDU level exchanges with CCID

    0x0F, 0x01, 0x00, 0x00, // dwMaxCCIDMessageLength  Maximun block size + header (261 + 10)

    0x00,       // bClassGetResponse
    0x00,       // bClassEnvelope
    0x00, 0x00, // wLcdLayout
    0x00,       // bPINSupport        no PIN verif and modif
    0x01,       // bMaxCCIDBusySlots

    // Interrupt OUT endpoint 1 descriptor (SCR)

    0x07,       // bLength           Descriptor size (7 bytes)
    0x05,       // bDescriptorType   Descriptor type (endpoint)
    0x81,       // bEndpointAddress  Endpoint 1 IN
    0x03,       // bmAttributes      Transfer type (interrupt)
    0x40, 0x00, // wMaxPacketSize    Maximum packet size (TODO 0x08, zur sicherheit 0x40)
    0x18,       // bInterval         Polling interval (24 ms = 0x18)

    // Bulk OUT endpoint 1 descriptor (SCR)

    0x07,       // bLength           Descriptor size (7 bytes)
    0x05,       // bDescriptorType   Descriptor type (endpoint)
    0x02,       // bEndpointAddress  Endpoint 2 OUT
    0x02,       // bmAttributes      Transfer type (bulk)
    0x40, 0x00, // wMaxPacketSize    Maximum packet size (64 bytes)
    0x00,       // bInterval         Polling interval (ignored)

    // Bulk IN endpoint 2 descriptor (SCR)

    0x07,       // bLength           Descriptor size (7 bytes)
    0x05,       // bDescriptorType   Descriptor type (endpoint)
    0x82,       // bEndpointAddress  Endpoint 2 IN
    0x02,       // bmAttributes      Transfer type (bulk)
    0x40, 0x00, // wMaxPacketSize    Maximum packet size (64 bytes)
    0x00,       // bInterval         Polling interval (ignored)
};


const uint8_t Keyboard_ReportDescriptor[KEYBOARD_SIZ_REPORT_DESC] = {
    0x06, 0x00, 0xff,    // USAGE_PAGE (Vendor-Defined)
    0x09, 0x01,          // USAGE (Vendor)
    0xa1, 0x01,          // COLLECTION (Application)

        0x09, 0x01,         //   USAGE (Input Report Data) !!! 0x0920
        0x15, 0x00,         //   LOGICAL_MINIMUM (0)
        0x26, 0xff, 0x00,   //   LOGICAL_MAXIMUM (255)
        0x75, 0x08,         //   REPORT_SIZE (8)
        0x95, 0x40,         //   REPORT_COUNT (64)

        0x81, 0x02,         //   INPUT (Data,Var,Abs)
        0x09, 0x02,         //   USAGE(Output Report Data) !!! 0x0921
        0x15, 0x00,         //   LOGICAL_MINIMUM (0)
        0x26, 0xff, 0x00,   //   LOGICAL_MAXIMUM (255)
        0x75, 0x08,         //   REPORT_SIZE (8)
        0x95, 0x40,         //   REPORT_COUNT (64)

        0x91, 0x02,         //   OUTPUT (Data,Var,Abs)
        0x09, 0x02,         //   USAGE (Keyboard ErrorUndefine)
        0x75, 0x08,         //   REPORT_SIZE (8)
        0x95, KEYBOARD_FEATURE_COUNT,   // REPORT_COUNT (64)
        0xB1, 0x02,         //   FEATURE (Data,Var,Abs) */

    0xc0,               // END_COLLECTION
};

/*****************************************************************************/

const uint8_t CCID_StringLangID[CCID_SIZ_STRING_LANGID] = {
    CCID_SIZ_STRING_LANGID,
    0x03,
    0x09,
    0x04
};  /* LangID = 0x0409: U.S. English */


const uint8_t CCID_StringVendor[CCID_SIZ_STRING_VENDOR] = {
    CCID_SIZ_STRING_VENDOR, /* Size of manufaturer string */
    0x03,   /* bDescriptorType = String descriptor */
    /* Manufacturer: "Nitrokey" */
    'N', 0, 'i', 0, 't', 0, 'r', 0, 'o', 0, 'k', 0, 'e', 0, 'y', 0
};


const uint8_t CCID_StringProduct[CCID_SIZ_STRING_PRODUCT] = {
    CCID_SIZ_STRING_PRODUCT,
    0x03,
    /* Product name: "Nitrokey Pro" */
    'N', 0, 'i', 0, 't', 0, 'r', 0, 'o', 0, 'k', 0, 'e', 0, 'y', 0, ' ', 0,
    'P', 0, 'r', 0, 'o', 0
};


uint8_t CCID_StringSerial[CCID_SIZ_STRING_SERIAL] = {
    CCID_SIZ_STRING_SERIAL,
    0x03,
    /* Serial number " " - (3 * 8 Byte defined in sourcecode)* 2 + 2 = 50 byte */
    ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0,
    ' ', 0,
    ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0,
    ' ', 0,
    ' ', 0, ' ', 0, ' ', 0, ' ', 0
};


const uint8_t CCID_StringInterface[CCID_SIZ_STRING_INTERFACE] = {
    CCID_SIZ_STRING_INTERFACE,
    0x03,
    /* Interface 0: "CS SmCd" */
    'C', 0, 'S', 0, ' ', 0, 'S', 0, 'm', 0, 'C', 0, 'd', 0
};
