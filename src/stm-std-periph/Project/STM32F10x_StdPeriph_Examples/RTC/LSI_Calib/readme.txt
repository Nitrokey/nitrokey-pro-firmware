
/**
  @page RTC_LSI_Calib RTC_LSI_Calib
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    RTC/LSI_Calib/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the RTC LSI_Calib Example.
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

This example demonstrates and explains how to use the LSI clock source auto
calibration to get a precise RTC clock. 
As an application example, it demonstrates how to configure the TIM5 timer
internally connected to LSI clock output, in order to adjust the RTC prescaler. 

The Low Speed External (LSI) clock is used as RTC clock source.
After reset, the RTC prescaler is set with the default value (40000). 
The inaccuracy of the LSI clock causes the RTC Second signal to be inaccurate. This
signal is output on the Tamper pin (PC.13) and can be measured by on oscilloscope
or a frequencymeter.

The program waits until Key Push button is pressed to begin the auto calibration procedure:
 - Configure the TIM5 to remap internally the TIM5 Channel 4 Input Capture to the
   LSI clock output.
 - Enable the TIM5 Input Capture interrupt: after one cycle of LSI clock, the
   period value is stored in a variable and compared to the HCLK clock to get
   its real value.
 - The RTC prescaler is adjusted with this LSI frequency value so that the RTC
   Second value become more accurate.
 - When calibration is done a led connected to PF.07 is turned ON to indicate the
    end of this operation. At this moment, you can  monitor the Second signal on
    an oscilloscope to measure its accuracy again.

The RTC Second signal can be monitored either on Tamper pin or on LED1 which is 
toggled into the RTC Second interrupt service routine.
    
 
@par Directory contents 

  - RTC/LSI_Calib/stm32f10x_conf.h     Library Configuration file
  - RTC/LSI_Calib/stm32f10x_it.c       Interrupt handlers
  - RTC/LSI_Calib/stm32f10x_it.h       Header for stm32f10x_it.c
  - RTC/LSI_Calib/main.h               Main header file
  - RTC/LSI_Calib/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density and XL-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (High-Density
    and XL-Density) and STM3210C-EVAL (Coneectivity Line) evaluation boards 
    and can be easily tailored to any other supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file (under Utilities\STM32_EVAL)

  - STM3210C-EVAL Set-up 
    - Use LD1 and LD2 leds connected respectively to PD.07 and PD.13 pins
    - Use the Key push-button connected to PB.09 pin

  - STM3210E-EVAL Set-up 
    - Use LD1 and LD2 leds connected respectively to PF.06 and PF.07 pins
    - Use the Key push button connected to PG.08 pin

@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c 
  - stm32f10x_rtc.c 
  - stm32f10x_tim.c 
  - stm32f10x_bkp.c 
  - stm32f10x_pwr.c 
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
