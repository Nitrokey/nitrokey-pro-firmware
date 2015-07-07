/**
  @page IWDG_Example IWDG_Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    IWDG/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the IWDG Example.
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

This example shows how to reload at regulate period the IWDG counter using the
SysTick interrupt. The IWDG timeout is set to 280 ms (the timeout may vary due
to LSI frequency dispersion).

SysTick is configured to generate an interrupt every 250 ms. In the SysTick interrupt
service routine (ISR), the IWDG counter is reloaded to prevent an IWDG reset, and
LED2 is toggled.
An EXTI is connected to a specific GPIO pin and configured to generate an interrupt
on its falling edge.
In the NVIC, this EXTI line corresspondant interrupt vector is enabled with a 
priority equal to 0, and the SysTick interrupt vector priority is set to 1 
(EXTI IT > SysTick IT).

The EXTI Line is used to simulate a software failure: when the EXTI Line event is
triggered (by pressing the Key push-button on the board), the corresponding interrupt
is served. In the ISR, the LED2turns off and the EXTI line pending bit is 
not cleared. So the CPU executes the EXTI line ISR indefinitely and the SysTick 
ISR is never entered (IWDG counter not reloaded).
As a result, when the IWDG counter reaches 00h, the IWDG generates a reset.
If the IWDG reset is generated, LED1 is turned on after the system resumes operation.

If the EXTI Line event does not occur, the IWDG counter is indefinitely reloaded in
the SysTick ISR, which prevents any IWDG reset.

In this example the system clock is set to 24 MHz on Value line devices and to
72MHz on other devices.

@par Directory contents 

  - IWDG/stm32f10x_conf.h     Library Configuration file
  - IWDG/stm32f10x_it.c       Interrupt handlers
  - IWDG/stm32f10x_it.h       Header for stm32f10x_it.c
  - IWDG/main.c               Main program

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
  - stm32f10x_iwdg.c 
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
