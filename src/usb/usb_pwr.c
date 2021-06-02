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
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_pwr.h"
#include "hw_config.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
__IO uint32_t bDeviceState = UNCONNECTED;   /* USB device status */

__IO bool fSuspendEnabled = TRUE;   /* true when suspend is possible */

struct
{
    __IO RESUME_STATE eState;
    __IO uint8_t bESOFcnt;
}
ResumeS;

/* Extern variables ---------------------------------------------------------- */
/* Private function prototypes ----------------------------------------------- */
/* Extern function prototypes ------------------------------------------------ */
/* Private functions --------------------------------------------------------- */

/*******************************************************************************
* Function Name  : PowerOn
* Description    :
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
RESULT PowerOn (void)
{
    uint16_t wRegVal;

  /*** cable plugged-in ? ***/
    /* while(!CablePluggedIn()); */
    USB_Cable_Config (ENABLE);

  /*** CNTR_PWDN = 0 ***/
    wRegVal = CNTR_FRES;
    _SetCNTR (wRegVal);

  /*** CNTR_FRES = 0 ***/
    wInterrupt_Mask = 0;
    _SetCNTR (wInterrupt_Mask);
  /*** Clear pending interrupts ***/
    _SetISTR (0);
  /*** Set interrupt mask ***/
    wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;
    _SetCNTR (wInterrupt_Mask);

    return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : PowerOff
* Description    : handles switch-off conditions
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
RESULT PowerOff ()
{
    /* disable all ints and force USB reset */
    _SetCNTR (CNTR_FRES);
    /* clear interrupt status register */
    _SetISTR (0);
    /* Disable the Pull-Up */
    USB_Cable_Config (DISABLE);
    /* switch-off device */
    _SetCNTR (CNTR_FRES + CNTR_PDWN);
    /* sw variables reset */
    /* ... */

    return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Suspend
* Description    : sets suspend mode operating conditions
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
void Suspend (void)
{
    uint16_t wCNTR;

    /* suspend preparation */
    /* ... */

    /* macrocell enters suspend mode */
    wCNTR = _GetCNTR ();
    wCNTR |= CNTR_FSUSP;
    _SetCNTR (wCNTR);

    /* ------------------ ONLY WITH BUS-POWERED DEVICES ---------------------- */
    /* power reduction */
    /* ... on connected devices */


    /* force low-power mode in the macrocell */
    wCNTR = _GetCNTR ();
    wCNTR |= CNTR_LPMODE;
    _SetCNTR (wCNTR);

    /* switch-off the clocks */
    /* ... */
    Enter_LowPowerMode ();

}

/*******************************************************************************
* Function Name  : Resume_Init
* Description    : Handles wake-up restoring normal operations
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
void Resume_Init (void)
{
    uint16_t wCNTR;

    /* ------------------ ONLY WITH BUS-POWERED DEVICES ---------------------- */
    /* restart the clocks */
    /* ...  */

    /* CNTR_LPMODE = 0 */
    wCNTR = _GetCNTR ();
    wCNTR &= (~CNTR_LPMODE);
    _SetCNTR (wCNTR);

    /* restore full power */
    /* ... on connected devices */
    Leave_LowPowerMode ();

    /* reset FSUSP bit */
    _SetCNTR (IMR_MSK);

    /* reverse suspend preparation */
    /* ... */

}

/*******************************************************************************
* Function Name  : Resume
* Description    : This is the state machine handling resume operations and
*                 timing sequence. The control is based on the Resume structure
*                 variables and on the ESOF interrupt calling this subroutine
*                 without changing machine state.
* Input          : a state machine value (RESUME_STATE)
*                  RESUME_ESOF doesn't change ResumeS.eState allowing
*                  decrementing of the ESOF counter in different states.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Resume (RESUME_STATE eResumeSetVal)
{
uint16_t wCNTR;

    if (eResumeSetVal != RESUME_ESOF)
        ResumeS.eState = eResumeSetVal;

    switch (ResumeS.eState)
    {
        case RESUME_EXTERNAL:
            Resume_Init ();
            ResumeS.eState = RESUME_OFF;
            break;
        case RESUME_INTERNAL:
            Resume_Init ();
            ResumeS.eState = RESUME_START;
            break;
        case RESUME_LATER:
            ResumeS.bESOFcnt = 2;
            ResumeS.eState = RESUME_WAIT;
            break;
        case RESUME_WAIT:
            ResumeS.bESOFcnt--;
            if (ResumeS.bESOFcnt == 0)
                ResumeS.eState = RESUME_START;
            break;
        case RESUME_START:
            wCNTR = _GetCNTR ();
            wCNTR |= CNTR_RESUME;
            _SetCNTR (wCNTR);
            ResumeS.eState = RESUME_ON;
            ResumeS.bESOFcnt = 10;
            break;
        case RESUME_ON:
            ResumeS.bESOFcnt--;
            if (ResumeS.bESOFcnt == 0)
            {
                wCNTR = _GetCNTR ();
                wCNTR &= (~CNTR_RESUME);
                _SetCNTR (wCNTR);
                ResumeS.eState = RESUME_OFF;
            }
            break;
        case RESUME_OFF:
        case RESUME_ESOF:
        default:
            ResumeS.eState = RESUME_OFF;
            break;
    }
}

/*******************************************************************************

 USB_Start

*******************************************************************************/

void USB_Start (void)
{
    Set_USBClock ();

    USB_Interrupts_Config ();

    USB_Init ();
#ifndef DISABLE_USB_WAIT
    while (bDeviceState != CONFIGURED)
    {
    }
#endif


}
