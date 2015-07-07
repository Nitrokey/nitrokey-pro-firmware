/**
  @page CAN_DualCAN CAN_DualCAN
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    CAN/DualCAN/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the CAN DualCAN example.
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

This example shows how to configure the CAN1 and CAN2 peripherals to send and 
receive CAN frames in normal mode. The sent frames are used to control Leds by 
pressing  KEY or Tamper push buttons.
	
	The CAN1 and CAN2 are configured as follow:
    - Bit Rate   = 1 Mbit/s  
    - CAN Clock  = external clock (HSE)
		- ID Filter  = All identifiers are allowed
		- RTR = Data
		- DLC = 1 byte
		- Data: Led number that should be turned ON

@par Directory contents 

  - CAN/Normal/stm32f10x_conf.h     Library Configuration file
  - CAN/Normal/stm32f10x_it.c       Interrupt handlers
  - CAN/Normal/stm32f10x_it.h       Interrupt handlers header file
  - CAN/Normal/main.c               Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line Devices.
  
  - This example has been tested with STMicroelectronics STM3210C-EVAL (STM32F10x 
    Connectivity line) evaluation board and can be easily tailored to any other 
    supported device and development board.
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file  

  - STM3210C-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PD.07, PD.13, PF.03
      and PD.04 pins
    - Use Key Push Button connected to PB9
    - Use Tamper Push Button connected to PC13    
    - Connect a female/female CAN cable between at the  CAN connectors 
      (CN4 and CN3 on STM3210C-EVAL boards)
    - Connector 1 DB9_PIN2 to Connector 2 DB9_PIN2  (CAN_L)
    - Connector 1 DB9_PIN5 to Connector 2 DB9_PIN5  ( GND )
	  - Connector 1 DB9_PIN7 to Connector 2 DB9_PIN7  (CAN_H) 
		  @note JP6 or JP5 must be fitted.
       	       
@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_can.c 
  - stm32f10x_exti.c  
  - stm32f10x_gpio.c   
  - stm32f10x_rcc.c 
  - stm32f10x_usart.c
  - stm32f10x_i2c.c
  - stm32f10x_spi.c
  - misc.c
  - stm32_eval.c       (under Utilities\STM32_EVAL)
  - stm3210c_eval_lcd.c  (under Utilities\STM32_EVAL\STM3210C_EVAL)    
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)   
         
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
