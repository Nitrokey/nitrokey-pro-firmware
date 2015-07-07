/**
  @page SPI_DMA SPI_DMA
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    SPI/DMA/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the SPI DMA Example.
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

This example provides a description of how to set a communication between the two
SPIs in simplex mode and performs a transfer from SPI_MASTER in polling mode to the
SPI_SLAVE in DMA receive mode. 

SPI_MASTER and SPI_SLAVE can be SPI1 and SPI2 or SPI3 and SPI2, depending on the
STMicroelectronics EVAL board you are using.

Both SPIs are configured with 8bit data frame and a 18Mbit/s communication speed.
SPI_MASTER is configured in bidirectional mode as transmitter only, while SPI_SLAVE
is configured in bidirectional mode but as receiver only. Both master and slave NSS
pins are managed by hardware.
A dedicated DMA channel is configured for SPI_SLAVE Rx request to store received
data in SPI_SLAVE_Buffer_Rx. 

SPI_MASTER starts by transferring the first data, once this data is received by the
SPI_SLAVE the RxNE request will trigger the DMA to transfer this data and store it
into SPI_SLAVE_Buffer_Rx. The same action is done for the rest of the buffer.

Once the transfer is completed a comparison is done and TransferStatus gives the
data transfer status where it is PASSED if transmitted and received data are the
same otherwise it is FAILED.


@par Directory contents 

  - SPI/DMA/platform_config.h Evaluation board specific configuration file
  - SPI/DMA/stm32f10x_conf.h  Library Configuration file
  - SPI/DMA/stm32f10x_it.c    Interrupt handlers
  - SPI/DMA/stm32f10x_it.h    Interrupt handlers header file
  - SPI/DMA/main.c            Main program


@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL (Medium-Density
    Value line), STM3210C-EVAL (Connectivity line), STM3210E-EVAL (High-Density and
    XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards and can be easily
    tailored to any other supported device and development board.
    This example can't be tested with STMicroelectronics STM32100B-EVAL (STM32F10x 
    Medium-Density Value line) evaluation boards
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in SPI/DMA/platform_config.h file.  

  - STM3210C-EVAL Set-up 
    - Connect SPI2 NSS pin (PB.12) to SPI3 NSS pin	(PA.04)
    - Connect SPI2 SCK pin (PB.13) to SPI3 SCK pin (PC.10)
    - Connect SPI2 MISO pin (PB.14) to SPI3 MOSI pin (PC.12)
    @note In this case SPI3 pins are remapped by software.

  - STM3210E-EVAL Set-up 
    - Connect SPI2 NSS pin (PB.12) to SPI1 NSS pin (PA.04)
    - Connect SPI2 SCK pin (PB.13) to SPI1 SCK pin (PA.05)
    - Connect SPI2 MISO pin (PB.14) to SPI1 MOSI pin (PA.07)
    @note The jumper 14 (USB Disconnect) must be set in position 1<->2 in order
          to not interfer with SPI2 MISO pin PB14.

  - STM3210B-EVAL Set-up 
    - Connect SPI2 NSS pin (PB.12) to SPI1 NSS pin (PA.04)
    - Connect SPI2 SCK pin (PB.13) to SPI1 SCK pin (PA.05)
    - Connect SPI2 MISO pin (PB.14) to SPI1 MOSI pin (PA.07)

    
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
- Edit SPI/DMA/platform_config.h file to select the evaluation board you will use.
  
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
