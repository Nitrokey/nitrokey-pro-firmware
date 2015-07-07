/*
 * Author: Copyright (C) STMicroelectronics
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

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
uint16_t SaveRState;

uint16_t SaveTState;

/* Extern variables ---------------------------------------------------------- */
extern void (*pEpInt_IN[7]) (void); /* Handles IN interrupts */

extern void (*pEpInt_OUT[7]) (void);    /* Handles OUT interrupts */

/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/*******************************************************************************
* Function Name  : CTR_LP.
* Description    : Low priority Endpoint Correct Transfer interrupt's service
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_LP (void)
{
    uint32_t wEPVal = 0;

    /* stay in loop while pending ints */
    while (((wIstr = _GetISTR ()) & ISTR_CTR) != 0)
    {
        _SetISTR ((uint16_t) CLR_CTR);  /* clear CTR flag */
        /* extract highest priority endpoint number */
        EPindex = (uint8_t) (wIstr & ISTR_EP_ID);
        if (EPindex == 0)
        {
            /* Decode and service control endpoint interrupt */
            /* calling related service routine */
            /* (Setup0_Process, In0_Process, Out0_Process) */

            /* save RX & TX status */
            /* and set both to NAK */
            SaveRState = _GetEPRxStatus (ENDP0);
            SaveTState = _GetEPTxStatus (ENDP0);
            _SetEPRxStatus (ENDP0, EP_RX_NAK);
            _SetEPTxStatus (ENDP0, EP_TX_NAK);


            /* DIR bit = origin of the interrupt */

            if ((wIstr & ISTR_DIR) == 0)
            {
                /* DIR = 0 */

                /* DIR = 0 => IN int */
                /* DIR = 0 implies that (EP_CTR_TX = 1) always */


                _ClearEP_CTR_TX (ENDP0);
                In0_Process ();

                /* before terminate set Tx & Rx status */
                _SetEPRxStatus (ENDP0, SaveRState);
                _SetEPTxStatus (ENDP0, SaveTState);
                return;
            }
            else
            {
                /* DIR = 1 */

                /* DIR = 1 & CTR_RX => SETUP or OUT int */
                /* DIR = 1 & (CTR_TX | CTR_RX) => 2 int pending */

                wEPVal = _GetENDPOINT (ENDP0);
                if ((wEPVal & EP_CTR_TX) != 0)
                {
                    _ClearEP_CTR_TX (ENDP0);
                    In0_Process ();
                    /* before terminate set Tx & Rx status */
                    _SetEPRxStatus (ENDP0, SaveRState);
                    _SetEPTxStatus (ENDP0, SaveTState);
                    return;
                }
                else if ((wEPVal & EP_SETUP) != 0)
                {
                    _ClearEP_CTR_RX (ENDP0);    /* SETUP bit kept frozen while CTR_RX = 1 */
                    Setup0_Process ();
                    /* before terminate set Tx & Rx status */
                    _SetEPRxStatus (ENDP0, SaveRState);
                    _SetEPTxStatus (ENDP0, SaveTState);
                    return;
                }

                else if ((wEPVal & EP_CTR_RX) != 0)
                {
                    _ClearEP_CTR_RX (ENDP0);
                    Out0_Process ();
                    /* before terminate set Tx & Rx status */
                    _SetEPRxStatus (ENDP0, SaveRState);
                    _SetEPTxStatus (ENDP0, SaveTState);
                    return;
                }
            }
        }   /* if(EPindex == 0) */
        else
        {
            /* Decode and service non control endpoints interrupt */

            /* process related endpoint register */
            wEPVal = _GetENDPOINT (EPindex);
            if ((wEPVal & EP_CTR_RX) != 0)
            {
                /* clear int flag */
                _ClearEP_CTR_RX (EPindex);

                /* call OUT service function */
                (*pEpInt_OUT[EPindex - 1]) ();

            }   /* if((wEPVal & EP_CTR_RX) */

            if ((wEPVal & EP_CTR_TX) != 0)
            {
                /* clear int flag */
                _ClearEP_CTR_TX (EPindex);

                /* call IN service function */
                (*pEpInt_IN[EPindex - 1]) ();
            }   /* if((wEPVal & EP_CTR_TX) != 0) */

        }   /* if(EPindex == 0) else */

    }   /* while(...) */
}

/*******************************************************************************
* Function Name  : CTR_HP.
* Description    : High Priority Endpoint Correct Transfer interrupt's service
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_HP (void)
{
    uint32_t wEPVal = 0;

    while (((wIstr = _GetISTR ()) & ISTR_CTR) != 0)
    {
        _SetISTR ((uint16_t) CLR_CTR);  /* clear CTR flag */
        /* extract highest priority endpoint number */
        EPindex = (uint8_t) (wIstr & ISTR_EP_ID);
        /* process related endpoint register */
        wEPVal = _GetENDPOINT (EPindex);
        if ((wEPVal & EP_CTR_RX) != 0)
        {
            /* clear int flag */
            _ClearEP_CTR_RX (EPindex);

            /* call OUT service function */
            (*pEpInt_OUT[EPindex - 1]) ();

        }   /* if((wEPVal & EP_CTR_RX) */
        else if ((wEPVal & EP_CTR_TX) != 0)
        {
            /* clear int flag */
            _ClearEP_CTR_TX (EPindex);

            /* call IN service function */
            (*pEpInt_IN[EPindex - 1]) ();


        }   /* if((wEPVal & EP_CTR_TX) != 0) */

    }   /* while(...) */
}
