/*
 * Author: Copyright (C) Rudolf Boeddeker                   Date: 2010-01-13
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


// #define SD_NO_USB_TEST // Test SD Interface

/*******************************************************************************

  SD_Test

	Test der SD Schnittstelle ohne USB Initialsierung

*******************************************************************************/
#ifdef SD_NO_USB_TEST

#include "sdcard.h"
extern SD_CardInfo SDCardInfo;

extern void RCC_Config (void);

extern SDIO_InitTypeDef SDIO_InitStructure;

extern void GPIO_Configuration (void);

#define BLOCK_SIZE 0x200
char cTestBuffer[BLOCK_SIZE];


void SetupLED (void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOC clock */
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure PC6..PC9 as outputs push-pull, max speed 50 MHz */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init (GPIOC, &GPIO_InitStructure);
}

#define LED_On(led)     GPIOC->ODR |=  (led << 4)
#define LED_Off(led)    GPIOC->ODR &= ~(led << 4)


void SD_Test (void)
{
    uint32_t i;

    uint32_t i1;

    uint32_t Status;

    uint32_t nRunCount = 0;

    uint32_t nErrorCountRead = 0;

    uint32_t nErrorCountWrite = 0;

    uint32_t nErrorCountByte = 0;

    SD_Error errorstatus = SD_OK;

    Set_System ();

    RCC_Config ();

    GPIO_Configuration ();

    /* Reset SD interface */
    SDIO_DeInit ();

    for (i = 0; i < 5000; i++);

    /* Enable the SDIO AHB Clock */
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_SDIO, ENABLE);

    /* Enable the DMA2 Clock */
    RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA2, ENABLE);

    /* */
    errorstatus = SD_PowerON ();

    /* */
    Status = SD_Init ();

    if (SD_OK != Status)
    {
        Status = SD_Init ();
    }

    if (SD_OK != Status)
    {
        Status = SD_Init ();
    }

    Status = SD_GetCardInfo (&SDCardInfo);
    Status = SD_SelectDeselect ((uint32_t) (SDCardInfo.RCA << 16));
    Status = SD_EnableWideBusOperation (SDIO_BusWide_4b);
    Status = SD_SetDeviceMode (SD_DMA_MODE);

    // for (i=0;i<500;i++);

    for (i = 0; i < BLOCK_SIZE; i++)
    {
        cTestBuffer[i] = 255;   // (char)i;
    }

    // SetupLED();
    while (1)
    {
        nRunCount++;
        /*
           LED_On (1); for (i=0;i<100;i++); LED_Off(1); */
        for (i = 0; i < BLOCK_SIZE; i++)
        {
            cTestBuffer[i] = (char) nRunCount;
        }
        /*
           //(nRunCount%10000)*2+1 */
        if (SD_OK != SD_WriteBlock (0, (uint32_t *) cTestBuffer, BLOCK_SIZE))
        {
            nErrorCountWrite++;

            // LED_On (1);
            // for (i=0;i<10;i++);
            // LED_Off(1);

        }
        for (i = 0; i < 500; i++);
         /**/ /**/ for (i = 0; i < BLOCK_SIZE; i++)
        {
            cTestBuffer[i] = 0;
        }

        /* (nRunCount%10000)*2+0 */

        // if (SD_OK !=
        // SD_ReadMultiBlocks(0,(uinuffer,BLOCK_Suffer,BLOCK_SIZE,4))
        if (SD_OK != SD_ReadBlock (0, (uint32_t *) cTestBuffer, BLOCK_SIZE))
        {
            nErrorCountRead++;
            /*
               LED_On (1); for (i=0;i<10;i++); LED_Off(1); */
        }

        for (i = 0; i < 500; i++);
         /**/ for (i = 0; i < BLOCK_SIZE; i++)
        {
            if (nRunCount != cTestBuffer[i])
            {
                nErrorCountByte++;
            }
        }

        if (100 == nRunCount)
        {
            nRunCount = 0;
            nErrorCountRead = 0;
            nErrorCountWrite = 0;
            nErrorCountByte = 0;
            Status++;
        }


    }

}

/*******************************************************************************

  SDIO_DataLineTest

	Toggels the 4 data pin of the SD card

*******************************************************************************/

void SDIO_DataLineTest (void)
{
    unsigned int n;

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable SDIO port clock */
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure SDIO dtat pin8, pin9, pin10 and pin11 as output push-pull */
    GPIO_InitStructure.GPIO_Pin =
        GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init (GPIOC, &GPIO_InitStructure);

    n = 0;
    while (1)
    {
        n++;
        if (n & 1)
            GPIO_SetBits (GPIOC, GPIO_Pin_8);
        else
            GPIO_ResetBits (GPIOC, GPIO_Pin_8);
        if (n & 2)
            GPIO_SetBits (GPIOC, GPIO_Pin_9);
        else
            GPIO_ResetBits (GPIOC, GPIO_Pin_9);
        if (n & 4)
            GPIO_SetBits (GPIOC, GPIO_Pin_10);
        else
            GPIO_ResetBits (GPIOC, GPIO_Pin_10);
        if (n & 8)
            GPIO_SetBits (GPIOC, GPIO_Pin_11);
        else
            GPIO_ResetBits (GPIOC, GPIO_Pin_11);

    }
}

#endif /* SD_NO_USB_TEST */
