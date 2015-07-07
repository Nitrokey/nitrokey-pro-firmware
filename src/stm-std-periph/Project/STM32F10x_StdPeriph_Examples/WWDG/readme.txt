/**
  @page WWDG_Example WWDG_Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    WWDG/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the WWDG Example.
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

This example shows how to update at regulate period the WWDG counter using the
Early Wakeup interrupt (EWI).

The WWDG timeout is set to 87.42ms on Value line devices and to 58.25 ms on other
devices, the refresh window is set to 65 and the EWI is enabled. When the WWDG 
counter reaches 64, the EWI is generated. In the WWDG ISR, the counter is 
refreshed to prevent a WWDG reset and LED2 is toggled.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the falling edge of the signal.
In the NVIC (nested vectored interrupt controller), the EXTI Line interrupt
vector is enabled with a priority equal to 0 and the WWDG interrupt vector is
enabled with a priority equal to 1 (EXTI IT > WWDG IT).

The EXTI Line is used to simulate a software failure: once the EXTI Line event 
occurs, by pressing the Key push-button, the corresponding interrupt is served.
In the ISR, the LED2 turns off and the EXTI Line pending bit is not cleared.
So the CPU executes the EXTI Line ISR indefinitely and the WWDG ISR is never
executed (the WWDG counter is not updated).
As a result, when the WWDG counter falls to 63, the WWDG reset occurs.
If the WWDG reset is generated, after the system resumes from reset, LED1 turns on.

If the EXTI Line event does not occur, the WWDG counter is indefinitely refreshed
in the WWDG ISR, and there is no WWDG reset.

In this example the system clock is set to 24 MHz on Value line devices and to 
72 MHz on other devices.


@par Directory contents
 
  - WWDG/stm32f10x_conf.h     Library Configuration file
  - WWDG/stm32f10x_it.c       Interrupt handlers
  - WWDG/stm32f10x_it.h       Header for stm32f10x_it.c
  - WWDG/main.c               Main program


@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL (Medium-Density
    Value line), STM3210C-EVAL (Connectivity line), STM3210E-EVAL (High-Density and
    XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards and can be easily
    tailored to any other supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM32100B-EVAL Set-up  
    - Use LD1 and LD2 leds connected respectively to PC.06 and PC.07 pins
    - Use the KEY push button connected to PB.09 pin (EXTI Line9).
    
  - STM3210C-EVAL Set-up 
    - Use LD1 and LD2 connected respectively to PD.07 and PD.13 pins
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    
  - STM3210E-EVAL Set-up 
    - Use LD1 and LD2 leds connected respectively to PF.06 and PF.07 pins
    - Use the KEY push button connected to PG.08 pin (EXTI Line8).

  - STM3210B-EVAL Set-up  
    - Use LD1 and LD2 leds connected respectively to PC.06 and PC.07 pins
    - Use the KEY push button connected to PB.09 pin (EXTI Line9).

     
@par How to use it ?
 
In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files: 
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c 
  - misc.c
  - stm32f10x_usart.c
  - stm32f10x_wwdg.c 
  - stm32f10x_exti.c  
  - stm32f10x_i2c.c
  - stm32f10x_spi.c 
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
  - stm32_eval.c (under Utilities\STM32_EVAL)

- Edit stm32f10x.h file to select the device you are working on.
- Edit stm32_eval.h file to select the evaluation board you will use.
  
@b Tip: You can tailor the provided project template to run this example, for 
        more details please refer to "stm32f10x_stdperiph_lib_um.chm" user 
        manual; select "Peripheral Examples" then follow the instructions 
        provided in "How to proceed" section.   
- Link all compiled files and load your image into target memory
- Run the example in standalone mode (without debugger connection)

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
