/*
 * Author: Copyright (C) Rudolf Boeddeker                   Date: 2010-01-13
 *                                              STMicroelectronics
 *                                              MCD Application Team            Date:   07/11/2008
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
#include "stm32f10x_flash.h"
#include "stm32f10x_systick.h"
#include "stm32f10x_usart.h"
#include "platform_config.h"
#include "smartcard.h"
#include "CCID_Global.h"
#include "CCID_usb.h"
#include "hw_config.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */
/* Global variables definition and initialization ---------------------------- */
SC_ATR SC_A2R;

u8 SC_ATR_Table[40];

u8 SC_ATR_Length = 0;

static vu8 SCData = 0;
static u32 F_Table[16] = { 372, 372, 558, 744, 1116, 1488, 1860, 0, 0, 512, 768, 1024, 1536, 2048,
    0, 0
};
static u32 D_Table[16] = { 0, 1, 2, 4, 8, 16, 32, 0, 12, 20, 0, 0, 0, 0, 0, 0 };

/* Private function prototypes ----------------------------------------------- */
/* Transport Layer ----------------------------------------------------------- */
/*--------------APDU-----------*/
static void SC_SendData (SC_ADPU_Commands * SC_ADPU, SC_ADPU_Responce * SC_ResponceStatus);

/*------------ ATR ------------*/
static void SC_AnswerReq (SC_State * SCState, u8 * card, u8 length);    /* Ask ATR */
static u8 SC_decode_Answer2reset (u8 * card);   /* Decode ATR */

/* Physical Port Layer ------------------------------------------------------- */
void SC_Init (void);

static void SC_DeInit (void);

static ErrorStatus USART_ByteReceive (u8 * Data, u32 TimeOut);

/* Private functions --------------------------------------------------------- */

uc8 MasterRoot[2] = { 0x3F, 0x00 };
uc8 GSMDir[2] = { 0x7F, 0x20 };
uc8 ICCID[2] = { 0x2F, 0xE2 };
uc8 IMSI[2] = { 0x6F, 0x07 };
uc8 CHV1[8] = { '0', '0', '0', '0', '0', '0', '0', '0' };

/* APDU Transport Structures */
// SC_ADPU_Commands SC_ADPU;
SC_ADPU_Responce SC_Responce;

extern vu32 TimingDelay;

static volatile ErrorStatus HSEStartUpStatus = SUCCESS;

// vu32 CardInserted = 0;
u32 CHV1Status = 0;             // i = 0;
vu8 ICCID_Content[10] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
vu8 IMSI_Content[9] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


// SC_State SCState = SC_POWER_OFF;



/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void RCC_Configuration (void)
{
    /* RCC system reset(for debug purpose) */
    RCC_DeInit ();

    /* Enable HSE */
    RCC_HSEConfig (RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp ();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd (FLASH_PrefetchBuffer_Enable);  // RB
        // essarsary
        // ??

        /* Flash 2 wait state */
        FLASH_SetLatency (FLASH_Latency_2); // RB necessary ??

        /* HCLK = SYSCLK */
        RCC_HCLKConfig (RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config (RCC_HCLK_Div1);

        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config (RCC_HCLK_Div2);

        /* PLLCLK = 8MHz * 9 = 72 MHz */
        RCC_PLLConfig (RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);   // RB
        // _PLLMLLMul_9);

        /* Enable PLL */
        RCC_PLLCmd (ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus (RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource () != 0x08)
        {
        }
    }
    /* Enable GPIO_LED, GPIO SC OFF and AFIO clocks */
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO, ENABLE);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void NVIC_Configuration (void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable (NVIC_VectTab_RAM, 0x0);
#else /* VECT_TAB_FLASH */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable (NVIC_VectTab_FLASH, 0x0);
#endif

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);

    /* Clear the SC_EXTI_IRQ Pending Bit */
    // NVIC_ClearIRQChannelPendingBit(SC_EXTI_IRQ);

    NVIC_InitStructure.NVIC_IRQChannel = SC_EXTI_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);

    /* Enable the USART Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = SMARTCARD_USART_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_Init (&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration_Smartcard
* Description    : configure the poweron port for the smartcard
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

void GPIO_Configuration_Smartcard (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable SMARTCARD_POWER_PORT clock */
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE); //
    // already done

    /* Configure SMARTCARD_POWER_PINs as output push-pull */
    GPIO_InitStructure.GPIO_Pin = SMARTCARD_POWER_PIN_1;
    if (SMARTCARD_POWER_PORT == SMARTCARD_POWER_PORT_2) {
        GPIO_InitStructure.GPIO_Pin = SMARTCARD_POWER_PIN_1 | SMARTCARD_POWER_PIN_2;
    }
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init (SMARTCARD_POWER_PORT, &GPIO_InitStructure);

    if (SMARTCARD_POWER_PORT != SMARTCARD_POWER_PORT_2) {
        /* Configure SMARTCARD_POWER_PINs as output push-pull */
        GPIO_InitStructure.GPIO_Pin = SMARTCARD_POWER_PIN_2;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(SMARTCARD_POWER_PORT_2, &GPIO_InitStructure);
    }

}

/*******************************************************************************

	SmartcardOn

*******************************************************************************/

void SmartcardOn (void)
{
    GPIO_SetBits (SMARTCARD_POWER_PORT, SMARTCARD_POWER_PIN_1);
    GPIO_SetBits (SMARTCARD_POWER_PORT_2, SMARTCARD_POWER_PIN_2);
}

/*******************************************************************************

	SmartcardOff

*******************************************************************************/

void SmartcardOff (void)
{
    GPIO_ResetBits (SMARTCARD_POWER_PORT, SMARTCARD_POWER_PIN_1);
    GPIO_ResetBits (SMARTCARD_POWER_PORT_2, SMARTCARD_POWER_PIN_2);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : None
*******************************************************************************/
void Delay (u32 nCount)
{
    TimingDelay = nCount;

    /* Enable the SysTick Counter */
    // SysTick_CounterCmd(SysTick_Counter_Enable);

    while (TimingDelay != 0)
    {

        CCID_CheckUsbCommunication ();

    }

    /* Disable the SysTick Counter */
    // SysTick_CounterCmd(SysTick_Counter_Disable);

    /* Clear the SysTick Counter */
    // SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*******************************************************************************
* Function Name  : Delay_noUSBCheck
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_noUSBCheck (u32 nCount)
{
    TimingDelay = nCount;

    /* Enable the SysTick Counter */
    // SysTick_CounterCmd(SysTick_Counter_Enable);

    while (TimingDelay != 0)
    {
    }

    /* Disable the SysTick Counter */
    // SysTick_CounterCmd(SysTick_Counter_Disable);

    /* Clear the SysTick Counter */
    // SysTick_CounterCmd(SysTick_Counter_Clear);
}




/*******************************************************************************
* Function Name  : SC_Handler
* Description    : Handles all Smartcard states and serves to send and receive all
*                  communication data between Smartcard and reader.
* Input          : - SCState: pointer to an SC_State enumeration that will contain
*                    the Smartcard state.
*                  - SC_ADPU: pointer to an SC_ADPU_Commands structure that will be
*                    initialized.
*                  - SC_Response: pointer to a SC_ADPU_Responce structure which will
*                    be initialized.
* Output         : None
* Return         : None
*******************************************************************************/
void SC_Handler (SC_State * SCState, SC_ADPU_Commands * SC_ADPU, SC_ADPU_Responce * SC_Response)
{
u32 i = 0;

    switch (*SCState)
    {
        case SC_POWER_ON:
            if (SC_ADPU->Header.INS == SC_GET_A2R)
            {
                /* Smartcard intialization ------------------------------------------ */
                SC_Init ();
                SC_PowerCmd (ENABLE);

                /* Reset Data from SC buffer ----------------------------------------- */
                for (i = 0; i < 40; i++)
                {
                    SC_ATR_Table[i] = 0;
                }

                /* Reset SC_A2R Structure -------------------------------------------- */
                SC_A2R.TS = 0;
                SC_A2R.T0 = 0;
                for (i = 0; i < SETUP_LENGTH; i++)
                {
                    SC_A2R.T[i] = 0;
                }
                for (i = 0; i < HIST_LENGTH; i++)
                {
                    SC_A2R.H[i] = 0;
                }
                SC_A2R.Tlength = 0;
                SC_A2R.Hlength = 0;

                /* Next State -------------------------------------------------------- */
                *SCState = SC_RESET_LOW;
            }
            break;

        case SC_RESET_LOW:
            if (SC_ADPU->Header.INS == SC_GET_A2R)
            {
                while (((*SCState) != SC_POWER_OFF) && ((*SCState) != SC_ACTIVE))
                {
                    SC_AnswerReq (SCState, &SC_ATR_Table[0], 40);   /* Check for answer to eeset */
                }
            }
            break;

        case SC_ACTIVE:
            if (SC_ADPU->Header.INS == SC_GET_A2R)
            {
                if (SC_decode_Answer2reset (&SC_ATR_Table[0]) == T0_PROTOCOL)
                {
                    (*SCState) = SC_ACTIVE_ON_T0;
                }
                else
                {
                    // (*SCState) = SC_POWER_OFF;
                    (*SCState) = SC_ACTIVE_ON_T0;
                }
            }
            break;

        case SC_ACTIVE_ON_T0:
            SC_SendData (SC_ADPU, SC_Response);
            break;

        case SC_POWER_OFF:
            SC_DeInit ();   /* Disable Smartcard interface */
            break;

        default:
            (*SCState) = SC_POWER_OFF;
    }
}

/*******************************************************************************
* Function Name  : SC_PowerCmd
* Description    : Enables or disables the power to the Smartcard.
* Input          : NewState: new state of the Smartcard power supply.
*                  This parameter can be: ENABLE or DISABLE.
* Output         : None
* Return         : None
*******************************************************************************/
void SC_PowerCmd (FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        GPIO_SetBits (SMARTCARD_POWER_PORT, SMARTCARD_POWER_PIN_1);
        GPIO_SetBits (SMARTCARD_POWER_PORT_2, SMARTCARD_POWER_PIN_2);
        USART_SmartCardCmd (SMARTCARD_USART, ENABLE);
    }
    else
    {
        USART_SmartCardCmd (SMARTCARD_USART, DISABLE);   // card is always on
        GPIO_ResetBits (SMARTCARD_POWER_PORT, SMARTCARD_POWER_PIN_1);
        GPIO_ResetBits (SMARTCARD_POWER_PORT_2, SMARTCARD_POWER_PIN_2);
    }
}

/*******************************************************************************
* Function Name  : SC_Reset
* Description    : Sets or clears the Smartcard reset pin.
* Input          : - ResetState: this parameter specifies the state of the Smartcard
*                    reset pin.
*                    BitVal must be one of the BitAction enum values:
*                       - Bit_RESET: to clear the port pin.
*                       - Bit_SET: to set the port pin.
* Output         : None
* Return         : None
*******************************************************************************/
void SC_Reset (BitAction ResetState)
{
    GPIO_WriteBit (SMARTCARD_SCRST_PORT, SMARTCARD_SCRST_PIN, ResetState);
}

/*******************************************************************************
* Function Name  : SC_ParityErrorHandler
* Description    : Resends the byte that failed to be received (by the Smartcard)
*                  correctly.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SC_ParityErrorHandler (void)
{
    USART_SendData (SMARTCARD_USART, SCData);
    while (USART_GetFlagStatus (SMARTCARD_USART, USART_FLAG_TC) == RESET)
    {
    }
}


void SC_SetHwParams (u8 cBaudrateIndex, u8 cConversion, u8 Guardtime, u8 Waitingtime)
{
RCC_ClocksTypeDef RCC_ClocksStatus;

u32 workingbaudrate = 0;

u32 apbclock = 0;

USART_InitTypeDef USART_InitStructure;

    /* Reconfigure the USART Baud Rate ------------------------------------------- */
    RCC_GetClocksFreq (&RCC_ClocksStatus);

    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
    apbclock /= ((SMARTCARD_USART->GTPR & (u16) 0x00FF) * 2);

    workingbaudrate = apbclock * D_Table[(cBaudrateIndex & (u8) 0x0F)];
    workingbaudrate /= F_Table[((cBaudrateIndex >> 4) & (u8) 0x0F)];

    USART_InitStructure.USART_BaudRate = workingbaudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init (SMARTCARD_USART, &USART_InitStructure);

    /* USART Guard Time set to 16 Bit */
    USART_SetGuardTime (SMARTCARD_USART, 12 + Guardtime);


}

/*******************************************************************************
* Function Name  : SC_PTSConfig
* Description    : Configures the IO speed (BaudRate) communication.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SC_PTSConfig (void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;

    u32 workingbaudrate = 0, apbclock = 0;

    u8 locData = 0, PTSConfirmStatus = 1;

    USART_InitTypeDef USART_InitStructure;

    USART_ClockInitTypeDef USART_ClockInitStructure;

    /* Reconfigure the USART Baud Rate ------------------------------------------- */
    RCC_GetClocksFreq (&RCC_ClocksStatus);

    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
    apbclock /= ((SMARTCARD_USART->GTPR & (u16) 0x00FF) * 2);
    /* Enable the DMA Receive (Set DMAR bit only) to enable interrupt generation in case of a framing error FE */
    USART_DMACmd (SMARTCARD_USART, USART_DMAReq_Rx, ENABLE);

    // SC_A2R.T0 = 0x11; // for slow serial testing
    // SC_A2R.T[0] = 0x11;

    if ((SC_A2R.T0 & (u8) 0x10) == 0x10)
    {
        if (SC_A2R.T[0] != 0x11)
        {
            /* Send PTSS */
            SCData = 0xFF;
            USART_SendData (SMARTCARD_USART, SCData);
            while (USART_GetFlagStatus (SMARTCARD_USART, USART_FLAG_TC) == RESET)
            {
            }

            if ((USART_ByteReceive (&locData, SC_Receive_Timeout)) == SUCCESS)
            {
                if (locData != 0xFF)
                {
                    PTSConfirmStatus = 0x40;
                }
            }

            /* Send PTS0 */
            SCData = 0x11;
            USART_SendData (SMARTCARD_USART, SCData);
            while (USART_GetFlagStatus (SMARTCARD_USART, USART_FLAG_TC) == RESET)
            {
            }

            if ((USART_ByteReceive (&locData, SC_Receive_Timeout)) == SUCCESS)
            {
                if (locData != 0x11)
                {
                    PTSConfirmStatus = 0x30;
                }
            }
            /* Send PTS1 */
            SCData = SC_A2R.T[0];   // 0x13
            USART_SendData (SMARTCARD_USART, SCData);
            while (USART_GetFlagStatus (SMARTCARD_USART, USART_FLAG_TC) == RESET)
            {
            }

            if ((USART_ByteReceive (&locData, SC_Receive_Timeout)) == SUCCESS)
            {
                if (locData != SC_A2R.T[0])
                {
                    PTSConfirmStatus = 0x20;
                }
            }

            /* Send PCK */

            SCData = (u8) 0xFF ^ (u8) 0x11 ^ (u8) SC_A2R.T[0];
            USART_SendData (SMARTCARD_USART, SCData);
            while (USART_GetFlagStatus (SMARTCARD_USART, USART_FLAG_TC) == RESET)
            {
            }

            if ((USART_ByteReceive (&locData, SC_Receive_Timeout)) == SUCCESS)
            {
                if (locData != ((u8) 0xFF ^ (u8) 0x11 ^ (u8) SC_A2R.T[0]))
                {
                    PTSConfirmStatus = 0x10;
                }
            }

            // GET*************

            if ((USART_ByteReceive (&locData, SC_Receive_Timeout)) == SUCCESS)
            {
                if (locData != 0xFF)
                {
                    PTSConfirmStatus = 0x02;
                }
            }
            else
            {
                PTSConfirmStatus = 0x03;
            }

            if ((USART_ByteReceive (&locData, SC_Receive_Timeout)) == SUCCESS)
            {
                if (locData != 0x11)
                {
                    PTSConfirmStatus = 0x04;
                }
            }
            else
            {
                PTSConfirmStatus = 0x05;
            }

            if ((USART_ByteReceive (&locData, SC_Receive_Timeout)) == SUCCESS)
            {
                if (locData != SC_A2R.T[0])
                {
                    PTSConfirmStatus = 0x06;
                }
            }
            else
            {
                PTSConfirmStatus = 0x07;
            }

            if ((USART_ByteReceive (&locData, SC_Receive_Timeout)) == SUCCESS)
            {
                if (locData != ((u8) 0xFF ^ (u8) 0x11 ^ (u8) SC_A2R.T[0]))
                {
                    PTSConfirmStatus = 0x08;
                }
            }
            else
            {
                PTSConfirmStatus = 0x09;
            }

            // GET************* END

            USART_DMACmd (SMARTCARD_USART, USART_DMAReq_Rx, DISABLE);

            /* PTS Confirm */
            if (PTSConfirmStatus == 0x01)
            {
                workingbaudrate = apbclock * D_Table[(SC_A2R.T[0] & (u8) 0x0F)];
                workingbaudrate /= F_Table[((SC_A2R.T[0] >> 4) & (u8) 0x0F)];

                USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
                USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
                USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
                USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
                USART_ClockInit (SMARTCARD_USART, &USART_ClockInitStructure);

                USART_InitStructure.USART_BaudRate = workingbaudrate;
                USART_InitStructure.USART_WordLength = USART_WordLength_9b;
                USART_InitStructure.USART_StopBits = USART_StopBits_1;
                USART_InitStructure.USART_Parity = USART_Parity_Even;
                USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
                USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
                USART_Init (SMARTCARD_USART, &USART_InitStructure);
            }
        }
    }
}

/*******************************************************************************

	CheckForRecByte

*******************************************************************************/

static s16 CheckForRecByte (void)
{
    u8 locData;

    if ((USART_ByteReceive (&locData, 1)) == SUCCESS)
    {
        return ((s16) locData);
    }

    return (-1);

}

/*******************************************************************************

	SendDatabyte

*******************************************************************************/

static s16 SendDatabyte (vu8 uData)
{
s32 uRec;

u32 Counter = 0;


    USART_SendData (SMARTCARD_USART, uData);

    while (USART_GetFlagStatus (SMARTCARD_USART, USART_FLAG_TC) == RESET)
    {
        Counter++;
        if (10000L < Counter)
        {
            Counter++;
            return (-2);
        }
    }

    uRec = CheckForRecByte ();

    if (uRec != uData)
    {
        return (uRec);
    }

    return (-1);
}

/*******************************************************************************

	CheckForSCStatus

    Wait Procedure byte from card:
    1 - ACK
    2 - NULL
    3 - SW1; SW2

*******************************************************************************/

#define SC_GET_STATUS					0
#define SC_GET_ACK_STATUS			1
#define SC_GET_NO_STATUS 			2
#define SC_GET_WRONG_STATUS		3

static s8 CheckForSCStatus (SC_ADPU_Commands * SC_ADPU, SC_ADPU_Responce * SC_ResponceStatus)
{
u8 locData = 0;

    if ((USART_ByteReceive (&locData, SC_Receive_Timeout)) != SUCCESS)
    {
        return (SC_GET_NO_STATUS);
    }

    if (((locData & (u8) 0xF0) != 0x60) && ((locData & (u8) 0xF0) != 0x90))
    {
        return (SC_GET_WRONG_STATUS);
    }

    if (((locData & (u8) 0xFE) == (((u8) ~ (SC_ADPU->Header.INS)) & (u8) 0xFE)) || ((locData & (u8) 0xFE) == (SC_ADPU->Header.INS & (u8) 0xFE)))
    {
        SC_ResponceStatus->Data[0] = locData;   /* ACK received */
        return (SC_GET_ACK_STATUS);

    }

    /* SW1 received */
    SC_ResponceStatus->SW1 = locData;
    if ((USART_ByteReceive (&locData, SC_Receive_Timeout)) != SUCCESS)
    {
        return (SC_GET_WRONG_STATUS);
    }

    /* SW2 received */
    SC_ResponceStatus->SW2 = locData;
    return (SC_GET_STATUS);
}

/*******************************************************************************
* Function Name  : SC_SendData
* Description    : Manages the Smartcard transport layer: send APDU commands and
*                  receives the APDU responce.
* Input          : - SC_ADPU: pointer to a SC_ADPU_Commands structure which
*                    will be initialized.
*                  - SC_Response: pointer to a SC_ADPU_Responce structure which
*                    will be initialized.
* Output         : None
* Return         : None
*******************************************************************************/
static void SC_SendData (SC_ADPU_Commands * SC_ADPU, SC_ADPU_Responce * SC_ResponceStatus)
{
u32 i = 0;

u8 locData = 0;

    /* Reset responce buffer --------------------------------------------------- */
    for (i = 0; i < LCmax; i++)
    {
        SC_ResponceStatus->Data[i] = 0;
    }

    SC_ResponceStatus->SW1 = 0;
    SC_ResponceStatus->SW2 = 0;

    /* Enable the DMA Receive (Set DMAR bit only) to enable interrupt generation in case of a framing error FE */
    USART_DMACmd (SMARTCARD_USART, USART_DMAReq_Rx, ENABLE);

    /* Send header ------------------------------------------------------------- */
    SendDatabyte (SC_ADPU->Header.CLA);
    SendDatabyte (SC_ADPU->Header.INS);
    SendDatabyte (SC_ADPU->Header.P1);
    SendDatabyte (SC_ADPU->Header.P2);

    /* Send body length to/from SC --------------------------------------------- */
    if (SC_ADPU->Body.LC)
    {
        SendDatabyte (SC_ADPU->Body.LC);
    }
    else if (SC_ADPU->Body.LE)
    {
        SendDatabyte (SC_ADPU->Body.LE);
    }


    /* Flush the USART1 DR */
    (void) USART_ReceiveData (SMARTCARD_USART);

    if (SC_GET_NO_STATUS != CheckForSCStatus (SC_ADPU, SC_ResponceStatus))
    {
        return;
    }

    /* If no status bytes received --------------------------------------------- */
    if (SC_ResponceStatus->SW1 == 0x00)
    {
        /* Send body data to SC-------------------------------------------------- */
        if (SC_ADPU->Body.LC)
        {
            for (i = 0; i < SC_ADPU->Body.LC; i++)
            {
                SendDatabyte (SC_ADPU->Body.Data[i]);
            }
            /* Flush the USART1 DR */
            (void) USART_ReceiveData (SMARTCARD_USART);

            /* Disable the DMA Receive (Reset DMAR bit only) */
            USART_DMACmd (SMARTCARD_USART, USART_DMAReq_Rx, DISABLE);

        }

        /* Or receive body data from SC ------------------------------------------ */
        else if (SC_ADPU->Body.LE)
        {
            for (i = 0; i < SC_ADPU->Body.LE; i++)
            {
                if (USART_ByteReceive (&locData, SC_Receive_Timeout) == SUCCESS)
                {
                    SC_ResponceStatus->Data[i] = locData;
                }
            }
        }

        /* Wait SW1 -------------------------------------------------------------- */
        i = 0;
        while (i < 10)
        {
            if (USART_ByteReceive (&locData, SC_Receive_Timeout) == SUCCESS)
            {
                SC_ResponceStatus->SW1 = locData;
                i = 11;
            }
            else
            {
                i++;
            }
        }
        /* Wait SW2 ------------------------------------------------------------ */
        i = 0;
        while (i < 10)
        {
            if (USART_ByteReceive (&locData, SC_Receive_Timeout) == SUCCESS)
            {
                SC_ResponceStatus->SW2 = locData;
                i = 11;
            }
            else
            {
                i++;
            }
        }
    }
}

/*******************************************************************************
* Function Name  : SC_AnswerReq
* Description    : Requests the reset answer from card.
* Input          : - SCState: pointer to an SC_State enumeration that will contain
*                    the Smartcard state.
*                  - card: pointer to a buffer which will contain the card ATR.
*                  - length: maximum ATR length
* Output         : None
* Return         : None
*******************************************************************************/
static void SC_AnswerReq (SC_State * SCstate, u8 * card, u8 length)
{
u8 Data = 0;

u32 i = 0;

u8* card_local;

    SC_ATR_Length = 0;

    card_local = card;

    switch (*SCstate)
    {
        case SC_RESET_LOW:
            /* Check responce with reset low --------------------------------------- */
            for (i = 0; i < length; i++)
            {
                if ((USART_ByteReceive (&Data, SC_Receive_Timeout)) == SUCCESS)
                {
                    card[i] = Data;
                    SC_ATR_Length++;
                }
            }
            if (card[0])
            {
                (*SCstate) = SC_ACTIVE;
                SC_Reset (Bit_SET);
            }
            else
            {
                (*SCstate) = SC_RESET_HIGH;
            }
            break;

        case SC_RESET_HIGH:
            /* Check responce with reset high -------------------------------------- */
            SC_Reset (Bit_SET); /* Reset High */

            while (length--)
            {
                if ((USART_ByteReceive (&Data, SC_Receive_Timeout)) == SUCCESS)
                {
                    *card_local++ = Data;   /* Receive data for timeout = SC_Receive_Timeout */
                    SC_ATR_Length++;
                }
            }
            if (card[0])
            {
                (*SCstate) = SC_ACTIVE;
            }
            else
            {
                (*SCstate) = SC_POWER_OFF;
            }
            break;

        case SC_ACTIVE:
            break;

        case SC_POWER_OFF:
            /* Close Connection if no answer received ------------------------------ */
            // SC_Reset(Bit_SET); /* Reset high - a bit is used as level
            // shifter from 3.3 to 5 V */
            SC_Reset (Bit_RESET);
            SC_PowerCmd (DISABLE);
            break;

        default:
            (*SCstate) = SC_RESET_LOW;
    }
}

/*******************************************************************************
* Function Name  : SC_decode_Answer2reset
* Description    : Decodes the Answer to reset received from card.
* Input          : - Card: pointer to the buffer containing the card ATR.
* Output         : None
* Return         : None
*******************************************************************************/
static u8 SC_decode_Answer2reset (u8 * card)
{
u32 i = 0, flag = 0, buf = 0, protocol = 0;

    SC_A2R.ATR_ReciveLength = SC_ATR_Length;

    SC_A2R.TS = card[0];    /* Initial character */
    SC_A2R.T0 = card[1];    /* Format character */

    SC_A2R.Hlength = SC_A2R.T0 & (u8) 0x0F;

    if ((SC_A2R.T0 & (u8) 0x80) == 0x80)
    {
        flag = 1;
    }

    for (i = 0; i < 4; i++)
    {
        SC_A2R.Tlength = SC_A2R.Tlength + (((SC_A2R.T0 & (u8) 0xF0) >> (4 + i)) & (u8) 0x1);
    }

    for (i = 0; i < SC_A2R.Tlength; i++)
    {
        SC_A2R.T[i] = card[i + 2];
    }

    protocol = SC_A2R.T[SC_A2R.Tlength - 1] & (u8) 0x0F;

    while (flag)
    {
        if ((SC_A2R.T[SC_A2R.Tlength - 1] & (u8) 0x80) == 0x80)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }

        buf = SC_A2R.Tlength;
        SC_A2R.Tlength = 0;

        for (i = 0; i < 4; i++)
        {
            SC_A2R.Tlength = SC_A2R.Tlength + (((SC_A2R.T[buf - 1] & (u8) 0xF0) >> (4 + i)) & (u8) 0x1);
        }

        for (i = 0; i < SC_A2R.Tlength; i++)
        {
            SC_A2R.T[buf + i] = card[i + 2 + buf];
        }
        SC_A2R.Tlength += (u8) buf;
    }

    for (i = 0; i < SC_A2R.Hlength; i++)
    {
        SC_A2R.H[i] = card[i + 2 + SC_A2R.Tlength];
    }

    /* Check for checksum byte in ATR */

    if (SC_A2R.ATR_ReciveLength != 2 + SC_A2R.Tlength + SC_A2R.Hlength)
    {
        SC_A2R.H[SC_A2R.Hlength] = card[2 + SC_A2R.Tlength + SC_A2R.Hlength];
        SC_A2R.Hlength++;
        SC_A2R.CheckSumPresent = TRUE;
    }

    return (u8) protocol;
}

#include "platform_config.h"
void hard_delay(int);

/*******************************************************************************
* Function Name  : SC_Init
* Description    : Initializes all peripheral used for Smartcard interface.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SC_Init (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStructure;
    static int nStartFlag = TRUE;

    /* Enable GPIO_3_5V, GPIORESET and GPIO_CMDVCC clocks */
    RCC_APB2PeriphClockCmd (SMARTCARD_USART_Periph_POWER_1, ENABLE);
    RCC_APB2PeriphClockCmd (SMARTCARD_USART_Periph_POWER_2, ENABLE);

    /* Enable USART clock */
    RCC_APB2PeriphClockCmd (SMARTCARD_USART_Periph, ENABLE);
    RCC_APB2PeriphClockCmd (SMARTCARD_USART_AFIO, ENABLE);
    GPIO_PinRemapConfig (SMARTCARD_USART_AFIO_MAPR_REMAP, ENABLE);


    if (TRUE == nStartFlag)
    {
        // SCCLK
        /* Configure USART CK as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = SMARTCARD_SCCLK_PIN;
        GPIO_InitStructure.GPIO_Mode = SMARTCARD_SCCLK_MODE;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init (SMARTCARD_SCCLK_PORT, &GPIO_InitStructure);

        // SCSDA
        /* Configure USART Tx as alternate function open-drain */
        GPIO_InitStructure.GPIO_Pin = SMARTCARD_SCSDA_PIN;
        GPIO_InitStructure.GPIO_Mode = SMARTCARD_SCSDA_MODE;
        GPIO_Init (SMARTCARD_SCSDA_PORT, &GPIO_InitStructure);

        // TODO check if this is optional
        /* Disable JTAG to be able to use PB3 */
        GPIO_PinRemapConfig (GPIO_Remap_SWJ_NoJTRST, ENABLE);
        GPIO_PinRemapConfig (GPIO_Remap_SWJ_JTAGDisable, ENABLE);

        /* Configure Smartcard Reset */
        // SCRST
        GPIO_InitStructure.GPIO_Pin = SMARTCARD_SCRST_PIN;
        GPIO_InitStructure.GPIO_Mode = SMARTCARD_SCRST_MODE;
        GPIO_Init (SMARTCARD_SCRST_PORT, &GPIO_InitStructure);

        /* Configure Smartcard CMDVCC */
        GPIO_Configuration_Smartcard ();
        nStartFlag = FALSE;
    }
    /* USART1 configuration ------------------------------------------------------ */
    /* USART1 configured as follow: - Word Length = 9 Bits - 0.5 Stop Bit - Even parity - BaudRate = 9677 baud - Hardware flow control disabled (RTS
       and CTS signals) - Tx and Rx enabled - USART Clock enabled */

    /* USART Clock set to 3.6 MHz (PCLK1 (36 MHZ) / 10) */
    USART_SetPrescaler (SMARTCARD_USART, 0x0a);  // RB0x05

    /* USART Guard Time set to 16 Bit */
    USART_SetGuardTime (SMARTCARD_USART, 1); // RB 16

    USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
    USART_ClockInit (SMARTCARD_USART, &USART_ClockInitStructure);


    USART_InitStructure.USART_BaudRate = 9677;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init (SMARTCARD_USART, &USART_InitStructure);

    /* Enable the USART1 Parity Error Interrupt */
    USART_ITConfig (SMARTCARD_USART, USART_IT_PE, ENABLE);

    /* Enable the USART1 Framing Error Interrupt */
    USART_ITConfig (SMARTCARD_USART, USART_IT_ERR, ENABLE);

    /* Enable USART1 */
    USART_Cmd (SMARTCARD_USART, ENABLE);

    /* Enable the NACK Transmission */
    USART_SmartCardNACKCmd (SMARTCARD_USART, ENABLE);

    /* Enable the Smartcard Interface */
    USART_SmartCardCmd (SMARTCARD_USART, ENABLE);

    /* Set RSTIN HIGH */
    // SC_Reset(Bit_SET); org
    SC_Reset (Bit_RESET);

    /* Disable CMDVCC */
    SC_PowerCmd (DISABLE);

    // Hard wait
    // FIXME check if it is really required, as it was optimized out in Os and still working anyway
    hard_delay(7);
}

void hard_delay(const int times){
    int j;
    for (j = 0; j < times; j++){
        unsigned int i;
        for (i = 0; i < 50000; i++){
            __asm__ __volatile__("");
        }
    };
}

/*******************************************************************************
* Function Name  : SC_DeInit
* Description    : Deinitializes all ressources used by the Smartcard interface.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void SC_DeInit (void)
{
    SC_Reset (Bit_RESET);

    /* Disable CMDVCC */
    SC_PowerCmd (DISABLE);
    /* Delay (5); */
    // Hard wait
    hard_delay(6);

    /* Deinitializes the USART */
    USART_DeInit (SMARTCARD_USART);

    /* Deinitializes the GPIO_RESET */
    // GPIO_DeInit(GPIO_RESET);

    /* Disable GPIO_3_5V, GPIO_RESET and GPIO_CMDVCC clocks */
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_RESET, DISABLE);

    /* Disable USART clock */
    RCC_APB2PeriphClockCmd (SMARTCARD_USART_Periph, DISABLE);
    /*
       Delay (5); */

    // Hard wait
    hard_delay(6);
}




/*******************************************************************************
* Function Name  : USART_ByteReceive
* Description    : Receives a new data while the time out not elapsed.
* Input          : None
* Output         : None
* Return         : An ErrorStatus enumuration value:
*                         - SUCCESS: New data has been received
*                         - ERROR: time out was elapsed and no further data is
*                                  received
*******************************************************************************/

static ErrorStatus USART_ByteReceive (u8 * Data, u32 TimeOut)
{
u32 Counter = 0;

    while ((USART_GetFlagStatus (SMARTCARD_USART, USART_FLAG_RXNE) == RESET) && (Counter != TimeOut))
    {
        Counter++;
    }

    if (Counter != TimeOut)
    {
        *Data = (u8) USART_ReceiveData (SMARTCARD_USART);
        return SUCCESS;
    }
    else
    {
        return ERROR;
    }
}

/*******************************************************************************

	WaitForATR

	return		TRUE		Get ATR
						FALSE		No ATR detected

*******************************************************************************/

int WaitForATR (void)
{
    SC_State SCState = SC_POWER_OFF;

    SC_ADPU_Commands SC_ADPU;

    SC_A2R.cATR_Valid = FALSE;  // invalidate last ATR data

    SCState = SC_POWER_ON;

    SC_ADPU.Header.CLA = 0x00;
    SC_ADPU.Header.INS = SC_GET_A2R;
    SC_ADPU.Header.P1 = 0x00;
    SC_ADPU.Header.P2 = 0x00;
    SC_ADPU.Body.LC = 0x00;

    while (SC_ACTIVE_ON_T0 != SCState)
    {
        SC_Handler (&SCState, &SC_ADPU, &SC_Responce);

        if (SC_POWER_OFF == SCState)
        {
            SC_DeInit ();   /* Disable Smartcard interface */
            return (FALSE);
        }
    }

    SC_A2R.cATR_Valid = TRUE;   // get ATR data

    CCID_SetCardState (TRUE);

    return (TRUE);
}


/*******************************************************************************

	CRD_SendCommand

*******************************************************************************/

int CRD_SendCommand (unsigned char* pTransmitBuffer, unsigned int nCommandSize, unsigned int nExpectedAnswerSize, unsigned int* nReceivedAnswerSize)
{
    int i;

#ifdef GERMALTO_CARD
    int i1;
#endif
    int nDelayTime;

    int nStatus;

    int nRecData;

    unsigned char cSendData;

    nStatus = SC_GET_STATUS;

    /* Test for baudrate set */
    if (4 == nCommandSize)
    {
        if ((0xff == pTransmitBuffer[0]) && (0x11 == pTransmitBuffer[1]))
        {
            *nReceivedAnswerSize = 4;
            return (SC_GET_STATUS);
        }
    }

    SwitchSmartcardLED (ENABLE);



    for (i = 0; i < nCommandSize; i++)
    {
        cSendData = pTransmitBuffer[i];
        nRecData = SendDatabyte (cSendData);

        if (-1 != nRecData)
        {
            nRecData = 33;
            SwitchSmartcardLED (DISABLE);
            return (nRecData);
        }
        // when INS 0x20 is send, after 4 byte the germalto card (only?) send
        // a ACK byte
#ifdef GERMALTO_CARD
        if ((4 == i) && (0x20 == pTransmitBuffer[1]))
        {
            for (i1 = 0; i1 < 10000; i1++)
            {
            }
        }
#endif
    }

    /* Flush the USART1 DR */
    (void) USART_ReceiveData (SMARTCARD_USART);

    for (i = 0; i < ICC_MESSAGE_BUFFER_MAX_LENGTH - USB_MESSAGE_HEADER_SIZE; i++)
    {
        pTransmitBuffer[i] = 0xa5;
    }

    /* Get answer */
    //
    for (i = 0; i < ICC_MESSAGE_BUFFER_MAX_LENGTH - USB_MESSAGE_HEADER_SIZE; i++)   // max
        // buffer
        // size
        // (had
        // to
        // be
        // checked)
    {
        nDelayTime = SC_Receive_Timeout;
        if (0 == i)
        {
            nDelayTime = SC_Receive_Timeout * 10000L;   // Long long wait for
            // first byte, allow
            // card to work
        }

        if ((USART_ByteReceive (&pTransmitBuffer[i], nDelayTime)) != SUCCESS)
        {
            break;
        }

        switch (i)
        {
            case 0:
                // ACK byte ?
                if (((pTransmitBuffer[i] & (u8) 0xFE) ==
                     (((u8) ~ (pTransmitBuffer[1])) & (u8) 0xFE)) || ((pTransmitBuffer[i] & (u8) 0xFE) == (pTransmitBuffer[1] & (u8) 0xFE)))
                {
                    break;
                }
                // Get Error state
                if (((pTransmitBuffer[i] & (u8) 0xF0) == 0x60)) // ||
                    // TransansmitBuffer[i]
                    // &
                    // ()0xF0F0)
                    // == 0x90))
                {
                    nStatus = SC_GET_WRONG_STATUS;
                }
                break;
        }

        if ((1 == i) && (SC_GET_WRONG_STATUS == nStatus))   // Get Error code
        {
            *nReceivedAnswerSize = 2;
            SwitchSmartcardLED (DISABLE);
            return (nStatus);
        }
    }
#ifdef GERMALTO_CARD
    if (FALSE == uFlagCode)
    {
        *nReceivedAnswerSize = i - 1;
        for (i1 = 1; i1 < i; i1++)
        {
            pTransmitBuffer[i1 - 1] = pTransmitBuffer[i1];
        }
    }
    else
#endif /* GERMALTO_CARD */
    {
        *nReceivedAnswerSize = i;
    }

    SwitchSmartcardLED (DISABLE);

    if (0 == *nReceivedAnswerSize)
    {
        *nReceivedAnswerSize = 2;
        return (SC_GET_NO_STATUS);
    }

    return (SC_GET_STATUS);
}

/*******************************************************************************

  InvalidateATR

*******************************************************************************/

void InvalidateATR (void)
{
    SC_A2R.cATR_Valid = FALSE;  // invalidate ATR data
}

/*******************************************************************************

  initSCHardware

*******************************************************************************/

void initSCHardware (void)
{
    SC_A2R.cATR_Valid = FALSE;  // invalidate ATR data

    /* System Clocks Configuration */
    RCC_Configuration ();

    /* NVIC configuration */
    NVIC_Configuration ();

    /* SysTick configuration */
    SysTick_Config (720000);

    /* Configure the GPIO ports */
    GPIO_Configuration_Smartcard ();

    /* Configure the EXTI Controller */
    // EXTI_Configuration();

    SmartcardOn ();
}

/*******************************************************************************

	scTest

*******************************************************************************/
#ifdef USED_FOR_TESTING
int scTest (void)
{
    unsigned char pTransmitBuffer[40];

    // unsigned int nCommandSize;
    // unsigned int nExpectedAnswerSize;
    // unsigned int nReceivedAnswerSize;
    int i;

    char szText_Baud_0[4] = { 0xFF, 0x11, 0x13, 0xFD };
    char szText_Baud_1[4] = { 0xFF, 0x11, 0x18, 0xF6 };

    char szText_6f_0[5] = { 0x00, 0xC1, 0x01, 0xFD, 0x3D };
    char szTest_6f_1[15] = { 0x00, 0x00, 0x0B, 0x00, 0xA4, 0x04, 0x00, 0x06,
        0xD2, 0x76, 0x00, 0x01, 0x24, 0x01, 0x2D
    };
    initSCHardware ();


    while (FALSE == WaitForATR ())
    {
        Delay_noUSBCheck (100);
    }

    CCID_SendCardDetect ();

    /* Apply the Procedure Type Selection (PTS) */
    SC_PTSConfig ();

    /* Inserts delay(400ms) for Smartcard clock resynchronisation */
    // Delay_noUSBCheck(40);


    for (i = 0; i < 10000; i++);
    // Delay_noUSBCheck(1);

    // memcpy (pTransmitBuffer,szText_6f_0,5);
    // CRD_SendCommand (pTransmitBuffer,5,2,&nReceivedAnswerSize);

    // memcpy (pTransmitBuffer,szTest_6f_1,15);
    // CRD_SendCommand (pTransmitBuffer,15,2,&nReceivedAnswerSize);

    Delay_noUSBCheck (10);

    while (1)
    {
    }

    /*
       { unsigned char pTransmitBuffer[40]; unsigned int nCommandSize; unsigned int nExpectedAnswerSize; unsigned int nReceivedAnswerSize;

       pTransmitBuffer[0] = 0xFF; pTransmitBuffer[1] = 0x11; pTransmitBuffer[2] = 0x13; pTransmitBuffer[3] = 0xFD; CRD_SendCommand
       (pTransmitBuffer,4,2,&nReceivedAnswerSize);

       Delay_noUSBCheck(5); } */


}

#endif /* USED_FOR_TESTING */

/*******************************************************************************

  RestartSmartcard

  Restart ths smartcard and set the communication parameter

*******************************************************************************/

char RestartSmartcard (void)
{
    int n;

    InvalidateATR ();

    n = 0;
    while (FALSE == WaitForATR ())
    {
        n++;
        if (0 < n)
        {
            return (FALSE);
        }
    }
    SC_PTSConfig ();

    // Delay_noUSBCheck (40);

    return (TRUE);
}

/*******************************************************************************

  SmartCardInitInterface

*******************************************************************************/

void SmartCardInitInterface (void)
{
    initSCHardware ();


    while (FALSE == WaitForATR ())
    {
        Delay_noUSBCheck (1);
    }

    /* Apply the Procedure Type Selection (PTS) */
    SC_PTSConfig ();

    /* Inserts delay(400ms) for Smartcard clock resynchronisation */
    Delay_noUSBCheck (40);

    /* Smartcard is ready to work */
    SwitchSmartcardLED (DISABLE);

}
