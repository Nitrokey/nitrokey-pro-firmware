/**
  @page ADC_AnalogWatchdog ADC_AnalogWatchdog
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    ADC/AnalogWatchdog/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the ADC analog watchdog example.
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

This example describes how to use the ADC analog watchdog to guard continuously  
an ADC channel.
The ADC1 is configured to convert continuously ADC channel14. The analog watchdog
is configured and enabled to guard a single regular channel.
Each time the channel14 converted value exceeds programmed analog watchdog high 
threshold (value 0x0B00) or goes down analog watchdog low threshold (value 0x0300)
an AWD interrupt is generated and the output pin connected to LED1 is toggled. The
LED will bright as long as the AWD interrupt is generated which means that the 
converted value of regular ADC channel14 is outside the range limited by high and 
low analog watchdog thresholds.
The ADC1 clock is set to 12 MHz on Value line devices and to 14 MHz on other 
devices.

@par Directory contents 

  - ADC/AnalogWatchdog/stm32f10x_conf.h     Library Configuration file
  - ADC/AnalogWatchdog/stm32f10x_it.c       Interrupt handlers
  - ADC/AnalogWatchdog/stm32f10x_it.h       Interrupt handlers header file
  - ADC/AnalogWatchdog/system_stm32f10x.c   STM32F10x system source file
  - ADC/AnalogWatchdog/main.c               Main program

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
    - Use LED1 led connected to PC.06 pin
    - Connect a variable power supply 0-3.3V to ADC Channel14 mapped on pin PC.04
      (potentiometer RV2)

  - STM3210C-EVAL Set-up 
    - Use LED1 led connected to PD.07 pin
    - Connect a variable power supply 0-3.3V to ADC Channel14 mapped on pin PC.04
      (potentiometer RV1)   
            
  - STM3210E-EVAL Set-up 
    - Use LD1 led connected to PF.06 pin
    - Connect a variable power supply 0-3.3V to ADC Channel14 mapped on pin PC.04
      (potentiometer RV1)     

 - STM3210B-EVAL  
    - Use LD1 led connected to PC.06 pin
    - Connect a variable power supply 0-3.3V to ADC Channel14 mapped on pin PC.04
      (potentiometer RV1) 

@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_adc.c 
  - stm32f10x_gpio.c
  - stm32f10x_exti.c
  - stm32f10x_rcc.c
  - stm32f10x_usart.c 
  - stm32f10x_i2c.c
  - stm32f10x_spi.c 
  - misc.c
  - stm32_eval.c (under Utilities\STM32_EVAL)

- Edit stm32f10x.h file to select the device you are working on.
- Use STM32F10x system source file "system_stm32f10x.c" provided with this example.
  
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
