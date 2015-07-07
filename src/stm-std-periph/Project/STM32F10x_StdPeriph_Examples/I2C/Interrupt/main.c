/**
  ******************************************************************************
  * @file    I2C/Interrupt/main.c 
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
#include "main.h"
    
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */ 
    
/** @addtogroup I2C_Interrupt
  * @{
  */ 
    
    /* Private typedef ----------------------------------------------------------- */ 
    typedef enum
{ FAILED = 0, PASSED = !FAILED } TestStatus;


    /* Private define ------------------------------------------------------------ */ 
#define I2C1_SLAVE_ADDRESS7     0x30
#define ClockSpeed              200000
    
    /* Private macro ------------------------------------------------------------- */ 
    /* Private variables --------------------------------------------------------- */ 
    I2C_InitTypeDef I2C_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
uint8_t I2C1_Buffer_Tx[Tx1BufferSize] =
{
1, 2, 3, 4}, I2C2_Buffer_Tx[Tx2BufferSize] =

{
5, 6, 7, 8};

uint8_t I2C1_Buffer_Rx[Tx2BufferSize], I2C2_Buffer_Rx[Tx1BufferSize + 1];
__IO uint8_t Tx1_Idx = 0, Rx1_Idx = 0, Tx2_Idx = 0, Rx2_Idx = 0;
__IO uint8_t Direction = Transmitter;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;


    /* Private functions --------------------------------------------------------- */ 
void RCC_Configuration (void);

void GPIO_Configuration (void);

void NVIC_Configuration (void);

TestStatus Buffercmp (uint8_t * pBuffer1, uint8_t * pBuffer2, uint16_t BufferLength);

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
    
        /* NVIC configuration ------------------------------------------------------ */ 
        NVIC_Configuration ();
    
        /* GPIO configuration ------------------------------------------------------ */ 
        GPIO_Configuration ();
    
        /* Enable I2C1 and I2C2 ---------------------------------------------------- */ 
        I2C_Cmd (I2C1, ENABLE);
    I2C_Cmd (I2C2, ENABLE);
    
        /* I2C1 configuration ------------------------------------------------------ */ 
        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
    I2C_Init (I2C1, &I2C_InitStructure);
    
        /* I2C2 configuration ------------------------------------------------------ */ 
        I2C_InitStructure.I2C_OwnAddress1 = I2C2_SLAVE_ADDRESS7;
    I2C_Init (I2C2, &I2C_InitStructure);
    I2C_CalculatePEC (I2C1, ENABLE);
    I2C_CalculatePEC (I2C2, ENABLE);
    
        /* Enable I2C1 event and buffer interrupts */ 
        I2C_ITConfig (I2C1, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
    
        /* Enable I2C1 event and buffer interrupts */ 
        I2C_ITConfig (I2C2, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
    
  /*----- Transmission Phase -------------------------------------------------*/ 
        /* Set data direction to transmitter */ 
        Direction = Transmitter;
    
        /* Send I2C1 START condition */ 
        I2C_GenerateSTART (I2C1, ENABLE);
    
        /* Wait until all data and the PEC value are received */ 
        /* I2C2_Buffer_Rx buffer will contain the data plus the PEC value */ 
        while (Rx2_Idx < Tx1BufferSize)
        
    {
    }
    
        /* Check the corectness of the I2C1 transmitted data */ 
        TransferStatus1 = Buffercmp (I2C1_Buffer_Tx, I2C2_Buffer_Rx, Tx1BufferSize);
    
        /* TransferStatus1 = PASSED, if the transmitted and received data are equal */ 
        /* TransferStatus1 = FAILED, if the transmitted and received data  are different */ 
        
  /*----- Reception Phase --------------------------------------------------*/ 
        /* Re-configure and enable I2C1 event interrupt to have the higher priority */ 
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);
    
        /* Wait until end of Slave transmission */ 
        while (Rx1_Idx < Tx2BufferSize)
        
    {
    }
    
        /* Check the corectness of the I2C1 received data */ 
        TransferStatus2 = Buffercmp (I2C2_Buffer_Tx, I2C1_Buffer_Rx, Tx2BufferSize);
    
        /* TransferStatus2 = PASSED, if the transmitted and received data are equal */ 
        /* TransferStatus2 = FAILED, if the transmitted and received data  are different */ 
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
        /* Enable I2C1 and I2C2 clock */ 
        RCC_APB1PeriphClockCmd (RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2, ENABLE);
    
        /* Enable GPIOB clock */ 
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB, ENABLE);
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
    GPIO_Init (GPIOB, &GPIO_InitStructure);
} 

/**
  * @brief  Configures NVIC and Vector Table base location.
  * @param  None
  * @retval None
  */ 
void NVIC_Configuration (void) 
{
    NVIC_PriorityGroupConfig (NVIC_PriorityGroup_0);
    
        /* Configure and enable I2C1 event interrupt ------------------------------- */ 
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);
    
        /* Configure and enable I2C2 event interrupt ------------------------------- */ 
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init (&NVIC_InitStructure);
    
        /* Configure and enable I2C2 error interrupt ------------------------------- */ 
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init (&NVIC_InitStructure);
} 

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *   FAILED: pBuffer1 differs from pBuffer2
  */ 
    TestStatus Buffercmp (uint8_t * pBuffer1, uint8_t * pBuffer2, uint16_t BufferLength) 
{
    while (BufferLength--)
        
    {
        if (*pBuffer1 != *pBuffer2)
            
        {
            return FAILED;
        }
        pBuffer1++;
        pBuffer2++;
    }
    return PASSED;
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
