/**
  @page TIM_TimeBase TIM_TimeBase
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    TIM/TimeBase/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the TIM Time Base example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Example Description 

This example shows how to configure the TIM peripheral in Output Compare Timing 
mode with the corresponding Interrupt requests for each channel in order to generate
4 different time bases.

The TIM2CLK frequency is set to SystemCoreClock / 2 (Hz), to get TIM2 counter 
clock at 6 MHz so the Prescaler is computed as following:
   - Prescaler = (TIM2CLK / TIM2 counter clock) - 1
SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
and Connectivity line devices and to 24 MHz for Low-Density Value line and
Medium-Density Value line devices

The TIM2 CC1 register value is equal to 40961, 
CC1 update rate = TIM2 counter clock / CCR1_Val = 146.48 Hz,
so the TIM2 Channel 1 generates an interrupt each 6.8ms

The TIM2 CC2 register is equal to 27309, 
CC2 update rate = TIM2 counter clock / CCR2_Val = 219.7 Hz
so the TIM2 Channel 2 generates an interrupt each 4.55ms

The TIM2 CC3 register is equal to 13654, 
CC3 update rate = TIM2 counter clock / CCR3_Val = 439.4Hz
so the TIM2 Channel 3 generates an interrupt each 2.27ms

The TIM2 CC4 register is equal to 6826, 
CC4 update rate = TIM2 counter clock / CCR4_Val =  878.9 Hz
so the TIM2 Channel 4 generates an interrupt each 1.13ms.

When the counter value reaches the Output compare registers values, the Output 
Compare interrupts are generated and, in the handler routine, 4 pins(PC.06, PC.07,
PC.08 and  PC.09) are toggled with the following frequencies: 

- PC.06: 73.24Hz (CC1)
- PC.07: 109.8Hz (CC2)
- PC.08: 219.7Hz (CC3) 
- PC.09: 439.4Hz (CC4)

@par Directory contents 

  - TIM/TimeBase/stm32f10x_conf.h  Library Configuration file
  - TIM/TimeBase/stm32f10x_it.c    Interrupt handlers
  - TIM/TimeBase/stm32f10x_it.h    Interrupt handlers header file
  - TIM/TimeBase/main.c            Main program 

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL (Medium-Density
    Value line), STM3210C-EVAL (Connectivity line), STM3210E-EVAL (High-Density and
    XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards and can be easily
    tailored to any other supported device and development board.

  - STM32100B-EVAL, STM3210E-EVAL, STM3210B-EVAL and STM32100B-EVAL Set-up 
    - Connect an oscilloscope on PC.06, PC.07, PC.08 and  PC.09 to show the 
      different Time Base signals.  
  
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_gpio.c  
  - stm32f10x_rcc.c 
  - stm32f10x_tim.c
  - misc.c 
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
  
- Edit stm32f10x.h file to select the device you are working on.
  
@b Tip: You can tailor the provided project template to run this example, for 
        more details please refer to "stm32f10x_stdperiph_lib_um.chm" user 
        manual; select "Peripheral Examples" then follow the instructions 
        provided in "How to proceed" section.   
- Link all compiled files and load your image into target memory
- Run the example

@note
 - Low-density Value line devices are STM32F100xx microcontrollers where the 
   Flash memory density ranges between 16 and 32 Kbytes.
 - Low-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 16 and 32 Kbytes.
 - Medium-density Value line devices are STM32F100xx microcontrollers where
   the Flash memory density ranges between 64 and 128 Kbytes.  
 - Medium-density devices are STM32F101xx, STM32F102xx and STM32F103xx 
   microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.
 - XL-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 512 and 1024 Kbytes.
 - Connectivity line devices are STM32F105xx and STM32F107xx microcontrollers.
   
 * <h3><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h3>
 */
