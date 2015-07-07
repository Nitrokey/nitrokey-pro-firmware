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
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "sdcard.h"
#include "hw_config.h"
#include "platform_config.h"
#include "hotp.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
vu32 TimingDelay = 0;

vu32 TimeCounter = 100;

/* Private function prototypes ----------------------------------------------- */
/* Private functions --------------------------------------------------------- */

/******************************************************************************/
/* Cortex-M3 Processor Exceptions Handlers */
/******************************************************************************/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler (void)
{
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler (void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler (void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler (void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler (void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler (void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler (void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler (void)
{
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler (void)
{
    /* Decrement the TimingDelay variable */
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
    if (TimeCounter != 0x00)
    {
        TimeCounter--;
    }
    else
    {
        TimeCounter = 100;
        current_time++;
    }
}

/******************************************************************************/
/* STM32F10x Peripherals Interrupt Handlers */
/******************************************************************************/

/*******************************************************************************
* Function Name  : USB_HP_CAN1_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN1_TX_IRQHandler (void)
{
    CTR_HP ();
}

/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler (void)
{
    USB_Istr ();
}

/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler (void)
{
    /* Process All SDIO Interrupt Sources */
    // SD_ProcessIRQSrc();

}

/******************************************************************************/
/* STM32F10x Peripherals Interrupt Handlers */
/* Add here the Interrupt Handler for the used peripheral(s) (PPP), for the */
/* available peripheral interrupt handler's name please refer to the startup */
/* file (startup_stm32f10x_xx.s).  */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/* void PPP_IRQHandler(void) { } */

// =============================================================================
// TIM2 Interrupt Handler
// =============================================================================


void TIM2_IRQHandler (void)
{
    if (TIM2->SR & TIM_SR_UIF)  // if UIF flag is set
    {
        TIM2->SR &= ~TIM_SR_UIF;    // clear UIF flag
        currentTime++;

        if (blinkOATHLEDTimes > 0)
        {
            if (blinkOATHLEDTimes & 1)
            {   // time to turn off the led
                if (currentTime >= (lastOATHBlinkTime + LED_ON_INTERVAL))
                {
                    SwitchOATHLED (DISABLE);
                    lastOATHBlinkTime = currentTime;
                    blinkOATHLEDTimes--;
                }

            }
            else
            {
                if (currentTime >= (lastOATHBlinkTime + LED_OFF_INTERVAL))
                {
                    SwitchOATHLED (ENABLE);
                    lastOATHBlinkTime = currentTime;
                    blinkOATHLEDTimes--;
                }

            }

        }
    }

}
