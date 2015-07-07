/**
  @page DMA_SPI_RAM DMA_SPI_RAM
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    DMA/SPI_RAM/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the DMA SPI example.
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

This example provides a description of how to use four DMA channels to transfer
a data buffer from memory to SPI_SLAVE through SPI_MASTER and a second data buffer
from memory to SPI_MASTER through SPI_SLAVE in full-duplex mode.
SPI_MASTER and SPI_SLAVE can be SPI1 and SPI2 or SPI3 and SPI2, depending on the
STMicroelectronics EVAL board you are using.

For each SPI the NSS pin is configured by software (thus NSS pin is free for GPIO use)
and DMA Tx/Rx requests are enabled. 

In this example both transmission and reception are managed through DMA and the
received data are stored into buffers declared in the SRAM. The DMA channels 
involved in this transfer depend on the used SPIs (for more details please refer
to platform_config.h file).

A polling on all Transfer complete flags are done for all used DMA channels to
check the end of all DMA channels transfers. The last received data on SPI_MASTER
and SPI_SLAVE are the CRC values sent by each SPI to the other.
The transmitted and received buffers are compared to check that all data have
been correctly transferred.


@par Directory contents 

  - DMA/SPI_RAM/platform_config.h Evaluation board specific configuration file
  - DMA/SPI_RAM/stm32f10x_conf.h  Library Configuration file
  - DMA/SPI_RAM/stm32f10x_it.c    Interrupt handlers
  - DMA/SPI_RAM/stm32f10x_it.h    Interrupt handlers header file
  - DMA/SPI_RAM/main.c            Main program


@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL (Medium-Density
    Value line), STM3210C-EVAL (Connectivity line), STM3210E-EVAL (High-Density and
    XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards and can be easily
    tailored to any other supported device and development board.

  - STM32100B-EVAL Set-up  
    - Connect SPI1 SCK pin  (PA.05) to SPI2 SCK  pin (PB.13)
    - Connect SPI1 MISO pin (PA.06) to SPI2 MISO pin (PB.14)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MOSI pin (PB.15)
      
  - STM3210C-EVAL Set-up 
    - Connect SPI3 SCK pin (PC.10) to SPI2 SCK pin	(PB.13)
    - Connect SPI3 MISO pin (PC.11) to SPI2 MISO pin (PB.14)
    - Connect SPI3 MOSI pin (PC.12) to SPI2 MOSI pin (PB.15)
    @note In this case SPI3 pins are remapped by software.

  - STM3210E-EVAL Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin	(PB.13)
    - Connect SPI1 MISO pin (PA.06) to SPI2 MISO pin (PB.14)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MOSI pin (PB.15)
    @note The jumper 14 (USB Disconnect) must be set in position 1<->2 in order
          to not interfer with SPI2 MISO pin PB14.
      
  - STM3210B-EVAL Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin	(PB.13)
    - Connect SPI1 MISO pin (PA.06) to SPI2 MISO pin (PB.14)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MOSI pin (PB.15)    


@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_dma.c 
  - stm32f10x_gpio.c   
  - stm32f10x_rcc.c 
  - stm32f10x_spi.c 
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
   
- Edit stm32f10x.h file to select the device you are working on.
- Edit DMA/SPI_RAM/platform_config.h file to select the evaluation board you will use.
  
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
