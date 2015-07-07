/**
  @page DAC_OneChannel_NoiseWave DAC_OneChannel_NoiseWave
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    DAC/OneChannel_NoiseWave/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the DAC one channel noise wave example.
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

This example describes how to use one DAC channel to generate a signal with noise
waves on DAC Channel1 output.

DAC channel1 conversion are configured to be triggered by software with noise wave 
generation.12bit left data alignement is selected since we choose to acces DAC_DHR12L1
register. Bits 0 to 8 are masked for the Linear feedback shift register. 
DAC channel1 is then enabled. DAC Channel1 DHR12L1 register is configured to have
an output voltage of VREF/2.

Software triggers are generated continuously in an infinite loop, and on each
trigger the DAC channel1 start the conversion and calculate the noise value to
apply on the DAC channel1 output.

The output signal with noise waves can be visualized by connecting PA.04 pin to
an oscilloscope.

@par Directory contents 

  - DAC/OneChannel_NoiseWave/stm32f10x_conf.h     Library Configuration file
  - DAC/OneChannel_NoiseWave/stm32f10x_it.c       Interrupt handlers
  - DAC/OneChannel_NoiseWave/stm32f10x_it.h       Header for stm32f10x_it.c
  - DAC/OneChannel_NoiseWave/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, XL-Density,
    Medium-Density Value line and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL 
    (Medium-Density Value line), STM3210C-EVAL (Connectivity line) and 
    STM3210E-EVAL (High-Density and XL-Density) evaluation boards and can be 
    easily tailored to any other supported device and development board.   
    
  - STM32100B-EVAL Set-up  
    - Connect PA.04 pin to an oscilloscope
    @note JP2 should be open
      
  - STM3210C-EVAL Set-up 
    - Connect PA.04 pin to an oscilloscope
    
  - STM3210E-EVAL Set-up 
    - Connect PA.04 pin to an oscilloscope
    
@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_dac.c  
  - stm32f10x_gpio.c
  - stm32f10x_rcc.c
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
