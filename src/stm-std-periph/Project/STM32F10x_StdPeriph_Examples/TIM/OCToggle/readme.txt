/**
  @page TIM_OCToggle TIM_OCToggle
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    TIM/OCToggle/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the TIM OCToggle example.
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

This example shows how to configure the TIM3 peripheral to generate four different 
signals with four different frequencies.

The TIM3CLK frequency is set to SystemCoreClock / 2 (Hz), and we want to get TIM3
counter clock at 12 MHz so the Prescaler is computed as following:
   - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
and Connectivity line devices and to 24 MHz for Low-Density Value line and
Medium-Density Value line devices

The TIM3 CCR1 register value is equal to 32768: 
CC1 update rate = TIM3 counter clock / CCR1_Val = 366.2 Hz,
so the TIM3 Channel 1 generates a periodic signal with a frequency equal to 183.1 Hz.

The TIM3 CCR2 register is equal to 16384:
CC2 update rate = TIM3 counter clock / CCR2_Val = 732.4 Hz
so the TIM3 channel 2 generates a periodic signal with a frequency equal to 366.3 Hz.

The TIM3 CCR3 register is equal to 8192:
CC3 update rate = TIM3 counter clock / CCR3_Val = 1464.8 Hz
so the TIM3 channel 3 generates a periodic signal with a frequency equal to 732.4 Hz.

The TIM3 CCR4 register is equal to 4096:
CC4 update rate = TIM3 counter clock / CCR4_Val =  2929.6 Hz
so the TIM3 channel 4 generates a periodic signal with a frequency equal to 1464.8 Hz.

@par Directory contents 

  - TIM/OCToggle/stm32f10x_conf.h  Library Configuration file
  - TIM/OCToggle/stm32f10x_it.c    Interrupt handlers
  - TIM/OCToggle/stm32f10x_it.h    Interrupt handlers header file
  - TIM/OCToggle/main.c            Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL (Medium-Density
    Value line), STM3210C-EVAL (Connectivity line), STM3210E-EVAL (High-Density and
    XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards and can be easily
    tailored to any other supported device and development board.
    

  - STM3210C-EVAL Set-up 
    - Connect the TIM1 pins(TIM3 full remapped pins) to an oscilloscope to monitor 
      the different waveforms:
       - PC.06 (TIM3_CH1)
       - PC.07 (TIM3_CH2)
       - PC.08 (TIM3_CH3)
       - PC.09 (TIM3_CH4)

  - STM32100B-EVAL, STM3210E-EVAL and STM3210B-EVAL Set-up 
    - Connect the following pins to an oscilloscope to monitor the different 
      waveforms:
       - PA.06 (TIM3_CH1)
       - PA.07 (TIM3_CH2)
       - PB.00 (TIM3_CH3)
       - PB.01 (TIM3_CH4) 
  
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
