/**
  @page PWR_STOP PWR_STOP
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    PWR/STOP/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the PWR STOP Example.
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

This example shows how to enter the system to STOP mode and wake-up using EXTI
Line interrupts. The EXTI Line sources are PB.09/PG.08 and RTC Alarm.

The EXTI line9/8 is configured to generate interrupt on falling edge.
The EXTI line17(RTC Alarm) is configured to generate interrupt on rising edge and
the RTC time base is set to 1 second using the external low speed oscillator(LSE).

The system clock is set to 24 MHz on Value line devices and to 72 MHz on other 
devices using the external high speed oscillator(HSE).

The system enters and exits STOP mode as following:
After 2 second from system start-up, the RTC is configured to generate an Alarm
event in 3 second then the system enters STOP mode. To wake-up from STOP mode you
have to apply a rising edge on EXTI line9/8, otherwise the  RTC Alarm will wake-up
the system within 3 second. After exit from STOP the system clock is reconfigured
to its previous state (as HSE and PLL are disabled in STOP mode).
Then after a delay the system will enter again in STOP mode and exit in the way
described above. This behavior is repeated in an infinite loop.

Three leds LED1, LED2 and LED3 are used to monitor the system state as following:
 - LED1 on: system in RUN mode
 - LED1 off: system in STOP mode
 - LED2 is toggled if EXTI Line9/8 is used to exit from STOP 
 - LED3 is toggled if EXTI line17(RTC Alarm) is used to exit from STOP 


@par Directory contents 

  - PWR/STOP/stm32f10x_conf.h     Library Configuration file
  - PWR/STOP/stm32f10x_it.c       Interrupt handlers
  - PWR/STOP/stm32f10x_it.h       Header for stm32f10x_it.c
  - PWR/STOP/main.c               Main program


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
    - Use LED1, LED2 and LED3 leds connected respectively to PC.06, PC.07 and PC.08 pins
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).
    
  - STM3210C-EVAL Set-up 
    - Use LED1, LED2 and LED3 leds connected respectively to PD.07, PD.13 and PF.03 pins
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).

  - STM3210E-EVAL Set-up 
    - Use LED1, LED2 and LED3 leds connected respectively to PF.06, PF0.7 and PF.08 pins
    - Use the Key push-button connected to pin PG.08 (EXTI Line8).

  - STM3210B-EVAL Set-up  
    - Use LED1, LED2 and LED3 leds connected respectively to PC.06, PC.07 and PC.08 pins
    - Use the Key push-button connected to pin PB.09 (EXTI Line9).

@note For power consumption measurement in STOP mode you have to: 
- Modify the example to configure all unused GPIO port pins in Analog Input mode
  (floating input trigger OFF). Refer to GPIO\IOToggle example for more details. 
- Replace jumper JP9 in the STM3210B-EVAL board, JP12 in the STM3210E-EVAL, 
  JP23 (position 1-2) in the STM3210C-EVAL board or JP8 (position 1-2) in the 
  STM32100B-EVAL board by an ampermeter.

         
@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_rcc.c 
  - stm32f10x_gpio.c 
  - stm32f10x_rtc.c   
  - stm32f10x_pwr.c   
  - stm32f10x_bkp.c 
  - stm32f10x_exti.c 
  - misc.c 
  - stm32f10x_usart.c
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
