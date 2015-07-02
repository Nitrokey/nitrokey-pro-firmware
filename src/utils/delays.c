/*
 * Author: Copyright (C) Rudolf Boeddeker  Date: 2012-08-18
 *
 * This file is part of Nitrokey 2
 *
 * Nitrokey 2  is free software: you can redistribute it and/or modify
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

/*
 * tools.c
 *
 *  Created on: 24.06.2010
 *      Author: RB
 */


/*******************************************************************************

 Local defines

*******************************************************************************/

#define TOOL_TEST_PIN     AVR32_PIN_PX10    // = UART2 - RX


// Delay1Ms_Counting

// #define TOOLS_DELAY_1MS_COUNT 910 // at 12 MHz
#define TOOLS_DELAY_1MS_COUNT   (4200)  // At 60 MHz


/*******************************************************************************

 Global declarations

*******************************************************************************/

/*******************************************************************************

 External declarations

*******************************************************************************/


/*******************************************************************************

 Local declarations

*******************************************************************************/

/*******************************************************************************

  Delay1Ms_Counting

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void Delay1Ms_Counting (void)
{
    register int i;

    for (i = 0; i < TOOLS_DELAY_1MS_COUNT; i++)
    {
    }
}


/*******************************************************************************

  Delay1Ms

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void Delay1Ms (void)
{
    Delay1Ms_Counting ();
}

/*******************************************************************************

  DelayMs

  Reviews
  Date      Reviewer        Info
  16.08.13  RB              First review

*******************************************************************************/

void DelayMs (int nMs)
{
    int i;

    for (i = 0; i < nMs; i++)
    {
        Delay1Ms ();
    }
}
