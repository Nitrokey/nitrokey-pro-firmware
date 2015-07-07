/**
  ******************************************************************************
  * @file    I2C/EEPROM/main.c 
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
#include "stm32_eval_i2c_ee.h"
    
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */ 
    
/** @addtogroup I2C_EEPROM
  * @{
  */ 
    
    /* Private typedef ----------------------------------------------------------- */ 
    typedef enum
{ FAILED = 0, PASSED = !FAILED } TestStatus;


    /* Private define ------------------------------------------------------------ */ 
#define sEE_WRITE_ADDRESS1        0x50
#define sEE_READ_ADDRESS1         0x50
#define BUFFER_SIZE1             (countof(Tx1_Buffer)-1)
#define BUFFER_SIZE2             (countof(Tx2_Buffer)-1)
#define sEE_WRITE_ADDRESS2       (sEE_WRITE_ADDRESS1 + BUFFER_SIZE1)
#define sEE_READ_ADDRESS2        (sEE_READ_ADDRESS1 + BUFFER_SIZE1)
    
    /* Private macro ------------------------------------------------------------- */ 
#define countof(a) (sizeof(a) / sizeof(*(a)))
    
    /* Private variables --------------------------------------------------------- */ 
    uint8_t Tx1_Buffer[] = "/* STM32F10x I2C Firmware ";
uint8_t Tx2_Buffer[] = "Library Example */";
uint8_t Rx1_Buffer[BUFFER_SIZE1], Rx2_Buffer[BUFFER_SIZE2];
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

volatile uint16_t NumDataRead = 0;


    /* Private functions --------------------------------------------------------- */ 
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
        
        /* Initialize the I2C EEPROM driver ---------------------------------------- */ 
        sEE_Init ();
    
        /* First write in the memory followed by a read of the written data -------- */ 
        /* Write on I2C EEPROM from sEE_WRITE_ADDRESS1 */ 
        sEE_WriteBuffer (Tx1_Buffer, sEE_WRITE_ADDRESS1, BUFFER_SIZE1);
    
        /* Set the Number of data to be read */ 
        NumDataRead = BUFFER_SIZE1;
    
        /* Read from I2C EEPROM from sEE_READ_ADDRESS1 */ 
        sEE_ReadBuffer (Rx1_Buffer, sEE_READ_ADDRESS1, (uint16_t *) (&NumDataRead));
    
        /* Starting from this point, if the requested number of data is higher than 1,  then only the DMA is managing the data transfer. Meanwhile,
           CPU is free to  perform other tasks:  // Add your code here:  //... //...  For simplicity reasons, this example is just waiting
           till the end of the  transfer. */ 
        
        /* Wait till DMA transfer is compelete (Tranfer complete interrupt handler  resets the variable holding the number of data to be read) */ 
        while (NumDataRead > 0)
        
    {
    }
    
        /* Check if the data written to the memory is read correctly */ 
        TransferStatus1 = Buffercmp (Tx1_Buffer, Rx1_Buffer, BUFFER_SIZE1);
    
        /* TransferStatus1 = PASSED, if the transmitted and received data  to/from the EEPROM are the same */ 
        /* TransferStatus1 = FAILED, if the transmitted and received data  to/from the EEPROM are different */ 
        
        /* Wait for EEPROM standby state */ 
        sEE_WaitEepromStandbyState ();
    
        /* Second write in the memory followed by a read of the written data ------- */ 
        /* Write on I2C EEPROM from sEE_WRITE_ADDRESS2 */ 
        sEE_WriteBuffer (Tx2_Buffer, sEE_WRITE_ADDRESS2, BUFFER_SIZE2);
    
        /* Set the Number of data to be read */ 
        NumDataRead = BUFFER_SIZE2;
    
        /* Read from I2C EEPROM from sEE_READ_ADDRESS2 */ 
        sEE_ReadBuffer (Rx2_Buffer, sEE_READ_ADDRESS2, (uint16_t *) (&NumDataRead));
    
        /* Starting from this point, if the requested number of data is higher than 1,  then only the DMA is managing the data transfer. Meanwhile,
           CPU is free to  perform other tasks:  // Add your code here:  //... //...  For simplicity reasons, this example is just waiting
           till the end of the  transfer. */ 
        
        /* Wait till DMA transfer is compelete (Tranfer complete interrupt handler  resets the variable holding the number of data to be read) */ 
        while (NumDataRead > 0)
        
    {
    }
    
        /* Check if the data written to the memory is read correctly */ 
        TransferStatus2 = Buffercmp (Tx2_Buffer, Rx2_Buffer, BUFFER_SIZE2);
    
        /* TransferStatus2 = PASSED, if the transmitted and received data  to/from the EEPROM are the same */ 
        /* TransferStatus2 = FAILED, if the transmitted and received data  to/from the EEPROM are different */ 
        
        /* Free all used resources */ 
        sEE_DeInit ();
    while (1)
        
    {
    }
}


/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
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
