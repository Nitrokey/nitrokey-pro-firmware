/**
  ******************************************************************************
  * @file    I2C/SMBus/main.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */  
    
    /* Includes ------------------------------------------------------------------ */ 
#include "stm32f10x.h"
    
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */ 
    
/** @addtogroup I2C_SMBus
  * @{
  */ 
    
    /* Private typedef ----------------------------------------------------------- */ 
    /* Private define ------------------------------------------------------------ */ 
#define I2C1_SLAVE_ADDRESS7    0x10
#define I2C2_SLAVE_ADDRESS7    0x30
#define SMBusDefaultHeader     0xC2
#define Command                0x01
#define ClockSpeed             20000
    
    /* Private macro ------------------------------------------------------------- */ 
    /* Private variables --------------------------------------------------------- */ 
    I2C_InitTypeDef I2C_InitStructure;
__IO uint8_t ReceivedCommand = 0, PECValue = 0;
volatile FlagStatus Status = RESET;


    /* Private functions --------------------------------------------------------- */ 
void RCC_Configuration (void);

void GPIO_Configuration (void);


/**
  * @brief   Main program
  * @param  None
  * @retval None
  */ 
int main (void) 
{
    
        /* !< At this stage the microcontroller clock setting is already configured,  this is done through SystemInit() function which is called
           from startup file (startup_stm32f10x_xx.s) before to branch to application main. To reconfigure the default setting of SystemInit()
           function, refer to system_stm32f10x.c file */ 
        
        /* System clocks configuration --------------------------------------------- */ 
        RCC_Configuration ();
    
        /* GPIO configuration ------------------------------------------------------ */ 
        GPIO_Configuration ();
    
        /* Enable I2C1 and I2C2 ---------------------------------------------------- */ 
        I2C_Cmd (I2C1, ENABLE);
    I2C_Cmd (I2C2, ENABLE);
    
        /* I2C1 configuration: SMBus Host ------------------------------------------ */ 
        I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
    I2C_Init (I2C1, &I2C_InitStructure);
    
        /* I2C2 configuration: SMBus Device ---------------------------------------- */ 
        I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusDevice;
    I2C_InitStructure.I2C_OwnAddress1 = I2C2_SLAVE_ADDRESS7;
    I2C_Init (I2C2, &I2C_InitStructure);
    
        /* Enable I2C2 ARP */ 
        I2C_ARPCmd (I2C2, ENABLE);
    
        /* Enable I2C1 and I2C2 PEC Transmission */ 
        I2C_CalculatePEC (I2C1, ENABLE);
    I2C_CalculatePEC (I2C2, ENABLE);
    
  /*----- Transmission Phase -----*/ 
        /* Send I2C1 START condition */ 
        I2C_GenerateSTART (I2C1, ENABLE);
    
        /* Test on I2C1 EV5 and clear it */ 
        while (!I2C_CheckEvent (I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
        /* Send Slave address */ 
        I2C_Send7bitAddress (I2C1, SMBusDefaultHeader, I2C_Direction_Transmitter);
    
        /* Test on I2C1 EV6 and clear it */ 
        while (!I2C_CheckEvent (I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
        /* Get I2C2 SMBDEFAULT flag status */ 
        Status = I2C_GetFlagStatus (I2C2, I2C_FLAG_SMBDEFAULT);
    
        /* Send Command */ 
        I2C_SendData (I2C1, Command);
    
        /* Clear ADDR flag: read operation to I2C_SR1 register followed by a read  operation to I2C_SR2 register */ 
        (void) (I2C_GetFlagStatus (I2C2, I2C_FLAG_ADDR));
    (void) (I2C_GetFlagStatus (I2C2, I2C_FLAG_BUSY));
    
        /* Wait for I2C2 received data */ 
        while (!I2C_GetFlagStatus (I2C2, I2C_FLAG_RXNE));
    
        /* Store received data on I2C2 */ 
        ReceivedCommand = I2C_ReceiveData (I2C2);
    
        /* Test on I2C1 EV8 and clear it */ 
        while (!I2C_CheckEvent (I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
        /* Enable Transfer PEC next for I2C1 and I2C2 */ 
        I2C_TransmitPEC (I2C1, ENABLE);
    I2C_TransmitPEC (I2C2, ENABLE);
    
        /* Wait for I2C2 received data */ 
        while (!I2C_GetFlagStatus (I2C2, I2C_FLAG_RXNE));
    
        /* Store received PEC on I2C2 */ 
        PECValue = I2C_ReceiveData (I2C2);
    
        /* Test on I2C1 EV8 and clear it */ 
        while (!I2C_CheckEvent (I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
        /* Send I2C1 STOP Condition */ 
        I2C_GenerateSTOP (I2C1, ENABLE);
    
        /* Test on I2C2 EV4 and clear it */ 
        while (!I2C_CheckEvent (I2C2, I2C_EVENT_SLAVE_STOP_DETECTED));
    
        /* Clear I2C2 STOPF flag: read operation to I2C_SR1 followed by a  write operation to I2C_CR1 */ 
        (void) (I2C_GetFlagStatus (I2C2, I2C_FLAG_STOPF));
    I2C_Cmd (I2C2, ENABLE);
    while (1)
        
    {
    }
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */ 
void RCC_Configuration (void) 
{
    
        /* Enable peripheral clocks ------------------------------------------------ */ 
        /* GPIOB Periph clock enable */ 
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
    
        /* I2C1 and I2C2 Periph clock enable */ 
        RCC_APB1PeriphClockCmd (RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2, ENABLE);
} 

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */ 
void GPIO_Configuration (void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
        /* Configure I2C1 pins: SCL and SDA ---------------------------------------- */ 
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init (GPIOB, &GPIO_InitStructure);
    
        /* Configure I2C2 pins: SCL and SDA ---------------------------------------- */ 
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init (GPIOB, &GPIO_InitStructure);
} 

#ifdef  USE_FULL_ASSERT
    
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */ 
void assert_failed (uint8_t * file, uint32_t line) 
{
    
        /* User can add his own implementation to report the file name and line number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
           file, line) */ 
        
        /* Infinite loop */ 
        while (1)
        
    {
    }
}


#endif /*  */
    
/**
  * @}
  */ 
    
/**
  * @}
  */ 
    
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/ 
