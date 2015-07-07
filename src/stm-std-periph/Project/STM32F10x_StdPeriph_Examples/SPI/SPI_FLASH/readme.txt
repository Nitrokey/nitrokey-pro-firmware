/**
  @page SPI_FLASH SPI_FLASH
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    SPI/SPI_FLASH/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the SPI SPI_FLASH Example.
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

This example provides a basic example of how to use the SPI firmware library
and an associate SPI FLASH driver to communicate with an M25P64 or M25P128 FLASH.

The first step consist in reading the SPI Flash ID. A comparison between the ID 
read from SPI flash and the expected one is done and LED1 is turned on in case
of success otherwise LED2 is turned on.  

Using this driver the program performs an erase of the sector to be accessed, a 
write of a Tx_Buffer, defined in the main.c file, to the memory followed by a read
of the written data. Then data read from the memory stored in the Rx_Buffer are
compared with the expected values of the Tx_Buffer. The result of this comparison
is stored in the "TransferStatus1" variable.

A second erase of the same sector is done at the end, and a test is done to be
sure that all the data written there are erased further to the sector erase. All
the data location are read and checked with 0xFF value. The result of this test
is stored in "TransferStatus2" variable which is FAILED in case of error.

The SPI1 is configured as Master with an 8-bit data size. The SPI1 baudrate 
is set to 18 Mbit/s (for Value line devices the baudrate is set to 12 Mbit/s).
The FLASH_WriteAddress and the FLASH_ReadAddress where the program start the write 
and the read operations are defined in the main.c file. 


@par Directory contents 

  - SPI/SPI_FLASH/stm32f10x_conf.h     Library Configuration file
  - SPI/SPI_FLASH/stm32f10x_it.c       Interrupt handlers
  - SPI/SPI_FLASH/stm32f10x_it.h       Header for stm32f10x_it.c
  - SPI/SPI_FLASH/main.c               Main program
  - stm32_eval_spi_flash.c             SPI FLASH driver
  - stm32_eval_spi_flash.h             Header for stm32_eval_spi_flash.c   

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM32100B-EVAL 
    (Medium-Density Value line), STM3210E-EVAL (High-Density and XL-Density) 
    and STM3210B-EVAL (Medium-Density) evaluation boards and can be 
    easily tailored to any other supported device and development board.
    This example can't be tested with STM3210C-EVAL (Connectivity-Line)
    evaluation board (no SPI FLASH available).
    To select the STMicroelectronics evaluation board used to run the example, 
    uncomment the corresponding line in stm32_eval.h file.
  
  - STM32100B-EVAL Set-up  
    - Use LED1 and LED2 connected respectively to PC.06 and PC.07 pins
    - M25P128 FLASH is already available on this board.
    
  - STM3210E-EVAL Set-up 
    - Use LED1 and LED2 connected respectively to PF.06 and PF.07 pins
    - M25P64 FLASH is already available on this board.
    @note The jumper 14 (USB Disconnect) must be set in position 1<->2 in order
          to not interfer with SPI2 MISO pin PB14.
      
  - STM3210B-EVAL Set-up  
    - Use LED1 and LED2 connected respectively to PC.06 and PC.07 pins
    - M25P64 FLASH is already available on this board.
 
  - Other platform Set-up
    - Use STM3210B-EVAL hardware configuration defines. 
    - Connect LED1 and LED2 respectively to PD.07 and PD.13 pins
    - Connect both SPI1 and SPI FLASH pins as following:
      - Connect SPI1_NSS (PA.04) pin to SPI Flash chip select (pin1)
      - Connect SPI1_SCLK (PA.05) pin to SPI Flash serial clock (pin6)
      - Connect SPI1_MISO (PA.06) pin to SPI Flash serial data output (pin2)
      - Connect SPI1_MOSI (PA.07) pin to SPI Flash serial data input (pin5)
      - Connect SPI Flash Write Protect (pin3) to Vdd
      - Connect SPI Flash Hold (pin7) to Vdd
      - Connect SPI Flash Vcc (pin8) to Vdd
      - Connect SPI Flash Vss (pin4) to Vss


@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_exti.c
  - stm32f10x_gpio.c  
  - stm32f10x_rcc.c 
  - stm32f10x_i2c.c
  - stm32f10x_spi.c
  - stm32f10x_usart.c
  - misc.c
  - system_stm32f10x.c (under Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x)
  - stm32_eval.c (under Utilities\STM32_EVAL)
  - stm32_eval_spi_flash.c (under Utilities\STM32_EVAL\Common)

- Edit stm32f10x.h file to select the device you are working on.
- Edit stm32_eval.h file to select the evaluation board you will use
  
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
