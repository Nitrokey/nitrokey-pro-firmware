/**
  @page SDIO_Example SDIO_Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    SDIO/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the SDIO Example.
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

This example provides a basic example of how to use the SDIO firmware library and
an associate driver to perform read/write operations on the SD Card memory (SD Card 
V1.0, V1.1, V2.0 and SDHC (High Capacity) protocol)that could be mounted on the 
STM3210E-EVAL board.
Below is a description of the different example steps:
  - Configure the SDIO according to the desired SDIO_CK clock frequency.
  - Reset the SD Card
  - Identify the SD Card
  - Initializes the SD Card
  - Get the SD Card Info
  - Select the SD Card
  - Enable the Wide Bus mode (4-bit data)
  - Erase the correponding blocks
  - Read the Erased blocks
  - Test if the corresponding Blocks are well erased: check if the EraseStatus 
    variable is equal to PASSED.
  - Set the Data Transfer Mode to DMA   
  - Write a single Block             
  - Read a single Block
  - Comapare the written Block and the read one: check if the TransferStatus1 
    variable is equal to PASSED.
  - Write multiple Blocks (2)             
  - Read multiple Blocks (2)
  - Comapare the written Blocks and the read one: check if the TransferStatus2 
    variable is equal to PASSED.  

@par Directory contents 

  - SDIO/stm32f10x_conf.h  Library Configuration file
  - SDIO/stm32f10x_it.c    Interrupt handlers
  - SDIO/stm32f10x_it.h    Header for stm32f10x_it.c
  - SDIO/main.c            Main program
  - stm32_eval_sdio_sd.c   SD Card Driver file
  - stm32_eval_sdio_sd.h   Header for stm32_eval_sdio_sd.c   
  
@par Hardware and Software environment 

  - This example runs only on STM32F10x High-Density and XL-Density Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (High-Density
    and XL-Density) evaluation board and can be easily tailored to any other 
    supported device and development board.

@note Make sure that the Jumper 17 (JP17) is closed and Jumper 20 (JP20) is open
      in STM3210E-EVAL 

@par How to use it ? 

In order to make the program work, you must do the following:
- Create a project and setup all project configuration
- Add the required Library files:
  - stm32f10x_gpio.c 
  - stm32f10x_rcc.c 
  - misc.c 
  - stm32f10x_sdio.c 
  - stm32f10x_dma.c
  - stm32f10x_usart.c
  - stm32f10x_exti.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
  - stm32_eval.c (under Utilities\STM32_EVAL)
  - stm32_eval_sdio_sd.c (under Utilities\STM32_EVAL\Common)
      
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
