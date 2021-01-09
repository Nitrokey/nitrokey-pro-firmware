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
#include "usb_lib.h"
#include "CCIDHID_usb_conf.h"
#include "CCIDHID_usb_desc.h"
#include "usb_pwr.h"
#include "usb_bot.h"
#include "hw_config.h"
#include "CCIDHID_usb_prop.h"
#include "CCIDHID_usb.h"
#include "string.h"

#include "sha1.h"
#include "hmac-sha1.h"
#include "hotp.h"
#include "keyboard.h"
#include "report_protocol.h"

#define KEY_STORE_ADDRESS 0x801FC00

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
#define HID_INTERFACE_NO    0   // interface number
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
uint32_t ProtocolValue;

uint8_t Request = 0;            // completed request at status stage

uint8_t RequestType = 0;

extern int nFlagSendSMCardInserted;

static uint32_t Max_Lun = 0;    // should not used

uint8_t HID_SetReport_Value[KEYBOARD_FEATURE_COUNT];

uint8_t HID_GetReport_Value[KEYBOARD_FEATURE_COUNT];

uint8_t HID_SetReport_Value_tmp[KEYBOARD_FEATURE_COUNT];

uint8_t HID_GetReport_Value_tmp[KEYBOARD_FEATURE_COUNT];

uint8_t message[KEYBOARD_FEATURE_COUNT];

DEVICE_INFO CCID_Device_Info;

DEVICE CCID_Device_Table = {
    CCID_EP_NUM,
    1
};


DEVICE_PROP CCID_Device_Property = {
    USB_CCID_init,
    USB_CCID_Reset,
    USB_CCID_Status_In,
    USB_CCID_Status_Out,
    USB_CCID_Data_Setup,
    USB_CCID_NoData_Setup,
    USB_CCID_Get_Interface_Setting,
    USB_CCID_GetDeviceDescriptor,   //
    USB_CCID_GetConfigDescriptor,
    USB_CCID_GetStringDescriptor,
    0,
    0x40    /* MAX PACKET SIZE */
};


USER_STANDARD_REQUESTS CCID_User_Standard_Requests = {
    USB_CCID_Storage_GetConfiguration,
    USB_CCID_Storage_SetConfiguration,
    USB_CCID_Storage_GetInterface,
    USB_CCID_Storage_SetInterface,
    USB_CCID_Storage_GetStatus,
    USB_CCID_Storage_ClearFeature,
    USB_CCID_Storage_SetEndPointFeature,
    USB_CCID_Storage_SetDeviceFeature,
    USB_CCID_Storage_SetDeviceAddress
};

ONE_DESCRIPTOR CCID_Device_Descriptor = {
    (uint8_t *) CCID_DeviceDescriptor,
    CCID_SIZ_DEVICE_DESC
};

ONE_DESCRIPTOR CCID_Config_Descriptor = {
    (uint8_t *) CCID_ConfigDescriptor,
    CCID_SIZ_CONFIG_DESC
};

ONE_DESCRIPTOR Keyboard_Report_Descriptor = {
    (uint8_t *) Keyboard_ReportDescriptor,
    KEYBOARD_SIZ_REPORT_DESC
};

ONE_DESCRIPTOR Keyboard_Hid_Descriptor = {
    (uint8_t *) CCID_ConfigDescriptor + KEYBOARD_OFF_HID_DESC,
    KEYBOARD_SIZ_HID_DESC
};

ONE_DESCRIPTOR CCID_String_Descriptor[5] = {
    {(uint8_t *) CCID_StringLangID, CCID_SIZ_STRING_LANGID},
    {(uint8_t *) CCID_StringVendor, CCID_SIZ_STRING_VENDOR},
    {(uint8_t *) CCID_StringProduct, CCID_SIZ_STRING_PRODUCT},
    {(uint8_t *) CCID_StringSerial, CCID_SIZ_STRING_SERIAL},
    {(uint8_t *) CCID_StringInterface, CCID_SIZ_STRING_INTERFACE},
};


/* Extern variables ---------------------------------------------------------- */
extern unsigned char Bot_State;

extern Bulk_Only_CBW CBW;

/* Private function prototypes ----------------------------------------------- */
/* Extern function prototypes ------------------------------------------------ */
/* Private functions --------------------------------------------------------- */
/*******************************************************************************
* Function Name  : CCID_init
* Description    : CCID init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_CCID_init (void)
{

    /* Update the serial number string descriptor with the data from the unique ID */
    Get_SerialNum ();

    Get_Name();

    pInformation->Current_Configuration = 0;

    /* Connect the device */
    PowerOn ();


    /* USB interrupts initialization */
    /* clear pending interrupts */
    _SetISTR (0);
    wInterrupt_Mask = IMR_MSK;
    /* set interrupts mask */
    _SetCNTR (wInterrupt_Mask);

    bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : CCID_Reset
* Description    : CCID Storage reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_CCID_Reset (void)
{
    /* Set the device as not configured */
    Device_Info->Current_Configuration = 0;
    pInformation->Current_Interface = 0;    /* the default Interface */
    /* Current Feature initialization */
    pInformation->Current_Feature = CCID_ConfigDescriptor[7];

    SetBTABLE (BTABLE_ADDRESS);

    /* Initialize Endpoint 0 */
    SetEPType (ENDP0, EP_CONTROL);

    SetEPTxStatus (ENDP0, EP_TX_NAK);
    SetEPRxAddr (ENDP0, CCID_ENDP0_RXADDR);
    SetEPRxCount (ENDP0, Device_Property->MaxPacketSize);
    SetEPTxAddr (ENDP0, CCID_ENDP0_TXADDR);
    Clear_Status_Out (ENDP0);
    SetEPRxValid (ENDP0);

    /* Initialize Endpoint 1 */
    SetEPType (ENDP1, EP_INTERRUPT);
    SetEPTxAddr (ENDP1, CCID_ENDP1_TXADDR);
    SetEPTxStatus (ENDP1, EP_TX_NAK);
    SetEPRxStatus (ENDP1, EP_RX_DIS);

    /* Initialize Endpoint 2 */
    SetEPType (ENDP2, EP_BULK);

    SetEPRxAddr (ENDP2, CCID_ENDP2_RXADDR);
    SetEPRxCount (ENDP2, Device_Property->MaxPacketSize);
    SetEPRxStatus (ENDP2, EP_RX_VALID);

    SetEPTxAddr (ENDP2, CCID_ENDP2_TXADDR);
    SetEPTxCount (ENDP2, Device_Property->MaxPacketSize);
    SetEPTxStatus (ENDP2, EP_TX_VALID);

    /* Initialize Endpoint 4 */
    SetEPType (ENDP4, EP_INTERRUPT);
    SetEPTxAddr (ENDP4, ENDP4_TXADDR);
    // SetEPTxCount(ENDP4, 8);
    SetEPRxStatus (ENDP4, EP_RX_DIS);
    SetEPTxStatus (ENDP4, EP_TX_NAK);

    /* */
    SetEPRxCount (ENDP0, Device_Property->MaxPacketSize);
    SetEPRxValid (ENDP0);

    /* Set the device to response on default address */
    SetDeviceAddress (0);

    bDeviceState = ATTACHED;

    CBW.dSignature = BOT_CBW_SIGNATURE;
    Bot_State = BOT_IDLE;

    nFlagSendSMCardInserted = TRUE; // card is always inserted
}

/*******************************************************************************
* Function Name  : CCID_Storage_SetConfiguration
* Description    : Handle the SetConfiguration request.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_CCID_Storage_SetConfiguration (void)
{
    if (pInformation->Current_Configuration != 0)
    {
        /* Device configured */
        bDeviceState = CONFIGURED;

        ClearDTOG_TX (ENDP1);
        ClearDTOG_RX (ENDP2);
        ClearDTOG_TX (ENDP2);
        // ClearDTOG_TX(ENDP3);
        ClearDTOG_TX (ENDP4);
        Bot_State = BOT_IDLE;   /* set the Bot state machine to the IDLE state */
    }
}


/*******************************************************************************
* Function Name  : CCID_Storage_ClearFeature
* Description    : Handle the ClearFeature request.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
#ifdef NOT_USED
void USB_CCID_Storage_ClearFeature (void)
{
    /* when the host send a CBW with invalid signature or invalid length the two Endpoints (IN & OUT) shall stall until receiving a CCID Storage
       Reset */
    if (CBW.dSignature != BOT_CBW_SIGNATURE)
        Bot_Abort (BOTH_DIR);
}
#endif
/*******************************************************************************
* Function Name  : CCID_Storage_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

void USB_CCID_Storage_SetDeviceAddress (void)
{
    bDeviceState = ADDRESSED;
}

/*******************************************************************************
* Function Name  : CCID_Status_In
* Description    : CCID Storage Status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

uint8_t lastLEDState = 0;

uint64_t lastNumLockChange = 0;

uint64_t lastCapsLockChange = 0;

uint64_t lastScrollLockChange = 0;

// extern uint64_t currentTime;
volatile uint8_t numLockClicked = 0;

volatile uint8_t capsLockClicked = 0;

volatile uint8_t scrollLockClicked = 0;

void USB_CCID_Status_In (void)
{
    if (Request == SET_REPORT)  // SET_REPORT completion
    {
        Request = 0;

        if (RequestType == HID_OUTPUT)
        {

    uint8_t LEDState = HID_SetReport_Value[0];

            if ((lastLEDState & NUM_LOCK_LED) != (LEDState & NUM_LOCK_LED))
            {   // num lock changed
                if ((currentTime - lastNumLockChange) < DOUBLE_CLICK_TIME)
                {
                    numLockClicked = 1;
                }
                lastNumLockChange = currentTime;
            }

            if ((lastLEDState & CAPS_LOCK_LED) != (LEDState & CAPS_LOCK_LED))
            {   // caps lock changed
                if ((currentTime - lastCapsLockChange) < DOUBLE_CLICK_TIME)
                {
                    capsLockClicked = 1;
                }
                lastCapsLockChange = currentTime;
            }

            if ((lastLEDState & SCROLL_LOCK_LED) != (LEDState & SCROLL_LOCK_LED))
            {   // numlock changed
                if ((currentTime - lastScrollLockChange) < DOUBLE_CLICK_TIME)
                {
                    scrollLockClicked = 1;
                }
                lastScrollLockChange = currentTime;
            }

            lastLEDState = LEDState;
        }
        else if (RequestType == HID_FEATURE)
        {
            // SwitchSmartcardLED(ENABLE);

            if (device_status == STATUS_READY)
            {
                device_status = STATUS_RECEIVED_REPORT;
                memcpy (HID_SetReport_Value_tmp, HID_SetReport_Value, KEYBOARD_FEATURE_COUNT);
            }
            // parse_report(HID_SetReport_Value,HID_GetReport_Value_tmp);
            // HID_GetReport_Value_tmp[0]=0xdd;
        }
        RequestType = 0;
    }

    return;
}

/*******************************************************************************
* Function Name  : CCID_Status_Out
* Description    : CCID Storage Status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_CCID_Status_Out (void)
{
    return;
}

/*******************************************************************************
* Function Name  : CCID_Data_Setup.
* Description    : Handle the data class specific requests..
* Input          : RequestNo.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT USB_CCID_Data_Setup (uint8_t RequestNo)
{
uint8_t* (*CopyRoutine) (uint16_t);

    CopyRoutine = NULL;
    if ((RequestNo == GET_DESCRIPTOR) && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT)) && (pInformation->USBwIndex0 == 0))

    {

        if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
        {
            CopyRoutine = Keyboard_GetReportDescriptor;
        }
        else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
        {
            CopyRoutine = Keyboard_GetHIDDescriptor;
        }

    }   /* End of GET_DESCRIPTOR */

  /*** GET_PROTOCOL ***/
    /* else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) && RequestNo == GET_PROTOCOL) { CopyRoutine = Keyboard_GetProtocolValue; } */
  /*** GET_PROTOCOL, GET_REPORT, SET_REPORT ***/
    else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)))
    {
        switch (RequestNo)
        {
            case GET_PROTOCOL:
                CopyRoutine = Keyboard_GetProtocolValue;
                break;
            case GET_REPORT:
                switch (pInformation->USBwValue1)   // report type
                {
                    case HID_FEATURE:
                        CopyRoutine = Keyboard_GetReport_Feature;
                        break;
                    case HID_INPUT:
                        break;
                }
                break;
            case SET_REPORT:
                switch (pInformation->USBwValue1)   // report type
                {
                    case HID_FEATURE:
                        CopyRoutine = Keyboard_SetReport_Feature;
                        Request = SET_REPORT;
                        RequestType = HID_FEATURE;
                        break;
                    case HID_OUTPUT:
                        CopyRoutine = Keyboard_SetReport_Output;
                        Request = SET_REPORT;
                        RequestType = HID_OUTPUT;
                        break;
                }
                break;
            default:
                break;
        }
    }

    if (CopyRoutine == NULL)
    {
        return USB_UNSUPPORT;
    }

    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    pInformation->Ctrl_Info.Usb_wOffset = 0;
    (*CopyRoutine) (0);
    return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CCID_NoData_Setup.
* Description    : Handle the no data class specific requests.
* Input          : RequestNo.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT USB_CCID_NoData_Setup (uint8_t RequestNo)
{
    if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) && (RequestNo == SET_PROTOCOL))
    {
        return Keyboard_SetProtocol ();
    }

    else
    {
        return USB_UNSUPPORT;
    }


    // return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : CCID_Get_Interface_Setting
* Description    : Test the interface and the alternate setting according to the
*                  supported one.
* Input          : uint8_t Interface, uint8_t AlternateSetting.
* Output         : None.
* Return         : RESULT.
*******************************************************************************/
RESULT USB_CCID_Get_Interface_Setting (uint8_t Interface, uint8_t AlternateSetting)
{
    if (AlternateSetting > 0)
    {
        return USB_UNSUPPORT;   /* in this application we don't have AlternateSetting */
    }
    else if (Interface > 1)
    {
        return USB_UNSUPPORT;   /* in this application we have only 1 interfaces */
    }
    return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CCID_GetDeviceDescriptor
* Description    : Get the device descriptor.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t* USB_CCID_GetDeviceDescriptor (uint16_t Length)
{
    return Standard_GetDescriptorData (Length, Device_Descriptor);
}

/*******************************************************************************
* Function Name  : CCID_GetConfigDescriptor
* Description    : Get the configuration descriptor.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t* USB_CCID_GetConfigDescriptor (uint16_t Length)
{
    return Standard_GetDescriptorData (Length, Config_Descriptor);
}

/*******************************************************************************
* Function Name  : CCID_GetStringDescriptor
* Description    : Get the string descriptors according to the needed index.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint8_t* USB_CCID_GetStringDescriptor (uint16_t Length)
{
uint8_t wValue0 = pInformation->USBwValue0;

    if (wValue0 > 5)
    {
        return NULL;
    }
    else
    {
        return Standard_GetDescriptorData (Length, String_Descriptor[wValue0]);
    }
}

/*******************************************************************************
* Function Name  : Get_Max_Lun
* Description    : Handle the Get Max Lun request.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/

uint8_t* USB_CCID_Get_Max_Lun (uint16_t Length)
{
    if (Length == 0)
    {
        pInformation->Ctrl_Info.Usb_wLength = 1;    // should not used
        // LUN_DATA_LENGTH;
        return 0;
    }
    else
    {
        return ((uint8_t *) (&Max_Lun));
    }
}


/*******************************************************************************
* Function Name  : Keyboard_SetProtocol
* Description    : Keyboard Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT Keyboard_SetProtocol (void)
{
uint8_t wValue0 = pInformation->USBwValue0;

    ProtocolValue = wValue0;
    return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Keyboard_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protocol value.
*******************************************************************************/
uint8_t* Keyboard_GetProtocolValue (uint16_t Length)
{
    if (Length == 0)
    {
        pInformation->Ctrl_Info.Usb_wLength = 1;
        return NULL;
    }
    else
    {
        return (uint8_t *) (&ProtocolValue);
    }
}

/*******************************************************************************
* Function Name  : Keyboard_GetReportDescriptor.
* Description    : Gets the HID report descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t* Keyboard_GetReportDescriptor (uint16_t Length)
{
    return Standard_GetDescriptorData (Length, &Keyboard_Report_Descriptor);
}

/*******************************************************************************
* Function Name  : Keyboard_GetHIDDescriptor.
* Description    : Gets the HID descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t* Keyboard_GetHIDDescriptor (uint16_t Length)
{
    return Standard_GetDescriptorData (Length, &Keyboard_Hid_Descriptor);
}






uint8_t* Keyboard_GetReport_Feature (uint16_t Length)
{

    if (Length == 0)
    {
        pInformation->Ctrl_Info.Usb_wLength = KEYBOARD_FEATURE_COUNT;
        return NULL;
    }
    else
    {

        // HID_GetReport_Value[0] = pInformation->USBwValue0; // report ID

        // HID_GetReport_Value[0] = 0xDE;
        // HID_GetReport_Value[1] = 0xAD;
        // HID_GetReport_Value[2] = 0xBE;
        // HID_GetReport_Value[3] = 0xEF;
        // HID_GetReport_Value[63] = 0xFF;

        memcpy (HID_GetReport_Value, HID_GetReport_Value_tmp, KEYBOARD_FEATURE_COUNT);
        // memcpy(HID_GetReport_Value,HID_SetReport_Value,KEYBOARD_FEATURE_COUNT);
        HID_GetReport_Value[0] = device_status;




        return HID_GetReport_Value;
    }
}





uint8_t* Keyboard_SetReport_Feature (uint16_t Length)
{

    if (Length == 0)
    {
        pInformation->Ctrl_Info.Usb_wLength = KEYBOARD_FEATURE_COUNT;
        return NULL;
    }
    else
    {
        return HID_SetReport_Value;
    }

}

uint8_t* Keyboard_SetReport_Output (uint16_t Length)
{

    if (Length == 0)
    {
        pInformation->Ctrl_Info.Usb_wLength = 1;
        return NULL;
    }
    else
    {
        return HID_SetReport_Value;
    }

}
