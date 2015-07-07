/**
  @page CEC_Example CEC_Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    CEC/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the CEC example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Description 

This example provides a basic communication between two HDMI-CEC devices using 
interrupts. 
The first and second CEC device send TransmitBuffer to the CEC other device. 
The data received by the first and second CEC device  are stored respectively in 
ReceiveBuffer. The data transfer is managed in CEC_IRQHandler in 
stm32f10x_it.c file.

The example illustrates the use of the CEC communication between two devices 
(2 x STM32100B-EVAL boards).
Each device can send a frame to the other device by pressing the user key 
button on the EVAL board. 

- Edit CEC/stm32f10x_conf.h to select the the corresponding device (CEC_DEVICE1,
CEC_DEVICE2).
 
@par Directory contents 

  - CEC/stm32f10x_conf.h   Library Configuration file
  - CEC/stm32f10x_it.h     Interrupt handlers header file
  - CEC/stm32f10x_it.c     Interrupt handlers
  - CEC/main.c             Main program
           
@par Hardware and Software environment

  - This example runs on STM32F10x Medium-Density Value line and Low-Density 
    Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL 
    (STM32F10x Medium-Density Value line) evaluation board and can be easily 
    tailored to any other supported device and development board.
    
  - Connect the boards by using one of the two following alternatives:
  - A HDMI Cables between all boards HDMI-CEC connectors (CN15 or CN16)
  - Use a simple wire between all devices CEC Lines (PB.08), in this case don't 
    forget to connect all boards grounds together.
   
@note
 - You can also use more than two CEC devices as much as you want by changing 
   only the Device address and selecting the corresponding followers.
 - You can use also the STM3210B-EVAL with the UM0685 associated firmware as a
   CEC device. This configuration is available only when use a simple wire connected
   between STM3210B-EVAL PA.00 and STM32100B-EVAL PB.08 pins. Don't forget to
   add a 27KOhm pull-up resistor on the STM3210B-EVAL PA.00 and to to connect 
   all boards grounds together.

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_cec.c 
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c 
  - stm32f10x_exti.c 
  - misc.c
  - stm32f10x_usart.c
  - stm32f10x_i2c.c
  - stm32f10x_spi.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
  - stm32_eval.c (under Utilities\STM32_EVAL)
    
- Edit stm32f10x.h file to select the device you are working on.
  
@b Tip: You can tailor the provided project template to run this example, for 
        more details please refer to "stm32f10x_stdperiph_lib_um.chm" user 
        manual; select "Peripheral Examples" then follow the instructions 
        provided in "How to proceed" section.   
- Link all compiled files and load your image into target memory
- Load the project image of First_Device (choose CEC_DEVICE1 when compiling) to 
  the first board.
- Load the project image of Second_Device (choose CEC_DEVICE2 when compiling) to 
  the second board.
- Run the examples      

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

 * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
 */
