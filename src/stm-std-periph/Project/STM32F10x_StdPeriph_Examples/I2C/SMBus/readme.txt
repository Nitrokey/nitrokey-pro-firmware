/**
  @page I2C_SMBus I2C_SMBus
  
  @verbatim
  ******************** (C) COPYRIGHT 2010 STMicroelectronics *******************
  * @file    I2C/SMBus/readme.txt 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Description of the I2C SMBus mode example.
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

This example provides a description of how to send an ARP command from I2C1 to
I2C2 in SMBus mode.

After configuring the I2C1 and I2C2 as SMBus Host and Device, respectively, both
I2Cs are enabled. The PEC calculation is enabled for both I2Cs.
The ARP capability is enabled for the slave I2C2. Following the start condition
generation, the master I2C1 sends the SMBus default header and I2C2 responds by
setting its SMBDEFAULT flag. The master I2C1 then issues the "Prepare to ARP"
command to the slave I2C2. PEC transfer is then enabled for both I2Cs, and the
PEC value received on I2C2 is stored into the PEC_Value variable.

A correct transmission leads to obtaining the following variable values:
         - Status = 0x01 (the flag SMBDEFAULT has been set)
         - ReceivedCommand = 0x01 (the Command value has been correctly received)
         
The communication clock speed is set to 20KHz.

@par Directory contents 

  - I2C/SMBus/stm32f10x_conf.h  Library Configuration file
  - I2C/SMBus/stm32f10x_it.c    Interrupt handlers
  - I2C/SMBus/stm32f10x_it.h    Interrupt handlers header file
  - I2C/SMBus/main.c            Main program

@par Hardware and Software environment 

  - This example runs on STM32F10x Connectivity line, High-Density, Medium-Density, 
    XL-Density, Medium-Density Value line, Low-Density and Low-Density Value line Devices.
  
  - This example has been tested with STMicroelectronics STM3210E-EVAL (High-Density
    and XL-Density) and STM3210B-EVAL (Medium-Density) evaluation boards 
    and can be easily tailored to any other supported device and development 
    board.
    This example can't be tested with STMicroelectronics STM3210C-EVAL (STM32F10x 
    Connectivity-Line) evaluation board since the I2C2 pins (PB10 and PB11) are 
    already used by Ethernet PHY module.
    This example can't be tested with STMicroelectronics STM32100B-EVAL (STM32F10x 
    Medium-Density Value line) evaluation boards since the I2C2 pins (PB10 and PB11) 
    are already used by HDMI-CEC module.       

  - STM3210E-EVAL Set-up 
    - Connect I2C1 SCL pin (PB.06) to I2C2 SCL pin (PB.10)
    - Connect I2C1 SDA pin (PB.07) to I2C2 SDA pin	(PB.11)
    - Check that a pull-up resistor is connected on one I2C SDA pin
    - Check that a pull-up resistor is connected on one I2C SCL pin

  - STM3210B-EVAL Set-up 
    - Connect I2C1 SCL pin (PB.06) to I2C2 SCL pin (PB.10)
    - Connect I2C1 SDA pin (PB.07) to I2C2 SDA pin	(PB.11)
    - Check that a pull-up resistor is connected on one I2C SDA pin
    - Check that a pull-up resistor is connected on one I2C SCL pin    

@note The pull-up resitors are already implemented on the STM3210B-EVAL and
      STM3210E-EVAL evaluation boards.      

@par How to use it ? 

In order to make the program work, you must do the following :
- Create a project and setup all project configuration
- Add the required Library files :
  - stm32f10x_gpio.c
  - stm32f10x_i2c.c 
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
