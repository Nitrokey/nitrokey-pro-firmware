/**
  @page USART_MultiProcessor USART_MultiProcessor
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    USART/MultiProcessor/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the USART Multi Processor Example.
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

This example provides a description of how to use the USART in multi-processor mode.
USARTy and USARTz can be USART1 and USART2 or USART2 and USART3 respectively, 
depending on the STMicroelectronics EVAL board you are using.

First, the USARTy and USARTz address are set to 0x1 and 0x2. The USARTy send 
continusouly the character 0x33 to the USARTz. The USARTz toggle LED1, LED2, LED3 
and LED4 pins while receiving 0x33.

When a falling edge is applied on BUTTON_KEY EXTI line, an interrupt is generated
and in the EXTI9_5_IRQHandler routine, the USARTz is entered in mute mode and still
in this mode (no LED toggling) until a rising edge is applied on BUTTON_WAKEUP 
EXTI Line 0.
In this interrupt routine the USARTy send the character of address mark (0x102)
to wakeup USARTz. The LED restart toggling.

USARTy and USARTz configured as follow:
  - BaudRate = 9600 baud  
  - Word Length = 9 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled

@par Directory contents 

  - USART/MultiProcessor/platform_config.h    Evaluation board specific configuration file
  - USART/MultiProcessor/stm32f10x_conf.h     Library Configuration file
  - USART/MultiProcessor/stm32f10x_it.h       Interrupt handlers header file
  - USART/MultiProcessor/stm32f10x_it.c       Interrupt handlers
  - USART/MultiProcessor/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL (Medium-Density
    Value line), STM3210C-EVAL (Connectivity line), STM3210E-EVAL (High-Density and
    XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards and can be easily
    tailored to any other supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in USART/MultiProcessor/platform_config.h or stm32_eval.h file.

  - STM32100B-EVAL Set-up  
    - Connect a null-modem female/female RS232 cable between CN9 and CN10.
      @note In this case USART2 Tx and Rx pins are remapped by software on 
            PD.05 and PD.06 respectively. 
    - Use Key push-button connected to pin PB.09 (EXTI Line9)
    - Use Wakeup push-button connected to pin PA.00 (EXTI Line0)
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PC.06, PC.07, 
      PC.08 and PC.09 pins

  - STM3210C-EVAL Set-up 
    - Connect USART2 Tx pin (PD.05) to USART3 Rx pin (PC.11)
    - Connect USART2 Rx pin (PD.06) to USART3 Tx pin	(PC.10)
    - Use Key push-button connected to pin PB.09 (EXTI Line9)
    - Use Wakeup push-button connected to pin PA.00 (EXTI Line0) 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins   
      @note In this case USART3 Tx and Rx pins are remapped by software.
            Make sure that jumpers JP19 and JP18 are open.
            Make sure that the Jumper 14 (JP14) is in position 2<-->3. 
    
  - STM3210E-EVAL Set-up 
    - Connect a null-modem female/female RS232 cable between CN12 and CN8.
    - Use Key push-button connected to pin PG.08 (EXTI Line8)
    - Use Wakeup push-button connected to pin PA.00 (EXTI Line0)
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PF.06, PF0.7, PF.08
      and PF.09 pins
      @note Make sure that the Jumper 4 (JP4) is in position 1<-->2. 

  - STM3210B-EVAL Set-up  
    - Connect a null-modem female/female RS232 cable between CN5 and CN6.
      @note In this case USART2 Tx and Rx pins are remapped by software on 
            PD.05 and PD.06 respectively. 
    - Use Key push-button connected to pin PB.09 (EXTI Line9)
    - Use Wakeup push-button connected to pin PA.00 (EXTI Line0)
    - Use LED1, LED2, LED3 and LED4 leds connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins             

   
@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_exti.c
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c 
  - stm32f10x_usart.c 
  - stm32f10x_i2c.c
  - stm32f10x_spi.c
  - misc.c 
  - stm32_eval.c       (under Utilities\STM32_EVAL)  
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
      
- Edit stm32f10x.h file to select the device you are working on.
- Edit USART/MultiProcessor/platform_config.h file to select the evaluation board you will use.
- or edit stm32_eval.h file to select the evaluation board you will use.
  
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
