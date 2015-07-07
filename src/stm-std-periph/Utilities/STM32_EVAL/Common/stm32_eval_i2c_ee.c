/**
  ******************************************************************************
  * @file    stm32_eval_i2c_ee.c
  * @author  MCD Application Team
  * @version V4.2.0
  * @date    04/16/2010
  * @brief   This file provides a set of functions needed to manage the I2C M24CXX 
  *          EEPROM memory mounted on STM32xx-EVAL board (refer to stm32_eval.h
  *          to know about the boards supporting this memory). 
  *          It implements a high level communication layer for read and write 
  *          from/to this memory. The needed STM32 hardware resources (I2C and 
  *          GPIO) are defined in stm32xx_eval.h file, and the initialization is 
  *          performed in sEE_LowLevel_Init() function declared in stm32xx_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          sEE_LowLevel_Init() function. 
  *        
  *          @note In this driver, basic read and write functions (sEE_ReadBuffer() 
  *                and sEE_WritePage()) use the DMA to perform the data transfer 
  *                to/from EEPROM memory (except when number of requested data is
  *                equal to 1). Thus, after calling these two functions, user 
  *                application may perform other tasks while DMA is transferring
  *                data. The application should then monitor the variable holding 
  *                the number of data in order to determine when the transfer is
  *                completed (variable decremented to 0). Stopping transfer tasks
  *                are performed into DMA interrupt handlers (which are integrated
  *                into this driver).
  *            
  *     +-----------------------------------------------------------------+
  *     |                        Pin assignment                           |                 
  *     +---------------------------------------+-----------+-------------+
  *     |  STM32 I2C Pins                       |   sEE     |   Pin       |
  *     +---------------------------------------+-----------+-------------+
  *     | .                                     |   E0(GND) |    1  (0V)  |
  *     | .                                     |   E1(GND) |    2  (0V)  |
  *     | .                                     |   E2(GND) |    3  (0V)  |
  *     | .                                     |   E0(VSS) |    4  (0V)  |
  *     | sEE_I2C_SDA_PIN/ SDA                  |   SDA     |    5        |
  *     | sEE_I2C_SCL_PIN/ SCL                  |   SCL     |    6        |
  *     | .                                     |   /WC(VDD)|    7 (3.3V) |
  *     | .                                     |   VDD     |    8 (3.3V) |
  *     +---------------------------------------+-----------+-------------+  
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
    
/** @addtogroup Utilities
  * @{
  */ 
    
/** @addtogroup STM32_EVAL
  * @{
  */ 
    
/** @addtogroup Common
  * @{
  */ 
    
/** @addtogroup STM32_EVAL_I2C_EE
  * @brief      This file includes the I2C EEPROM driver of STM32-EVAL boards.
  * @{
  */ 
    
/** @defgroup STM32_EVAL_I2C_EE_Private_Types
  * @{
  */ 
/**
  * @}
  */ 
    
/** @defgroup STM32_EVAL_I2C_EE_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 
    
/** @defgroup STM32_EVAL_I2C_EE_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 
    
/** @defgroup STM32_EVAL_I2C_EE_Private_Variables
  * @{
  */ 
    __IO uint16_t sEEAddress = 0;
__IO uint32_t sEETimeout = sEE_TIMEOUT_MAX;
__IO uint16_t * sEEDataReadPointer;
__IO uint8_t * sEEDataWritePointer;
__IO uint8_t sEEDataNum;

/**
  * @}
  */ 
    
/** @defgroup STM32_EVAL_I2C_EE_Private_Function_Prototypes
  * @{
  */ 
/**
  * @}
  */ 
    
/** @defgroup STM32_EVAL_I2C_EE_Private_Functions
  * @{
  */ 
    
/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */ 
void sEE_DeInit (void) 
{
    sEE_LowLevel_DeInit ();
} 

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */ 
void sEE_Init (void) 
{
    I2C_InitTypeDef I2C_InitStructure;
    sEE_LowLevel_Init ();
    
        /* !< I2C configuration */ 
        /* sEE_I2C configuration */ 
        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
    
        /* sEE_I2C Peripheral Enable */ 
        I2C_Cmd (sEE_I2C, ENABLE);
    
        /* Apply sEE_I2C configuration after enabling it */ 
        I2C_Init (sEE_I2C, &I2C_InitStructure);
    
        /* Enable the sEE_I2C peripheral DMA requests */ 
        I2C_DMACmd (sEE_I2C, ENABLE);
    
#if defined (sEE_M24C64_32)
        /* !< Select the EEPROM address according to the state of E0, E1, E2 pins */ 
        sEEAddress = sEE_HW_ADDRESS;
    
#elif defined (sEE_M24C08)
        /* !< depending on the sEE Address selected in the i2c_ee.h file */ 
#ifdef sEE_Block0_ADDRESS
        /* !< Select the sEE Block0 to write on */ 
        sEEAddress = sEE_Block0_ADDRESS;
    
#endif /*  */
        
#ifdef sEE_Block1_ADDRESS
        /* !< Select the sEE Block1 to write on */ 
        sEEAddress = sEE_Block1_ADDRESS;
    
#endif /*  */
        
#ifdef sEE_Block2_ADDRESS
        /* !< Select the sEE Block2 to write on */ 
        sEEAddress = sEE_Block2_ADDRESS;
    
#endif /*  */
        
#ifdef sEE_Block3_ADDRESS
        /* !< Select the sEE Block3 to write on */ 
        sEEAddress = sEE_Block3_ADDRESS;
    
#endif /*  */
#endif  /*!< sEE_M24C64_32 */    
} 

/**
  * @brief  Writes one byte to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @retval None
  */ 
void sEE_WriteByte (uint8_t * pBuffer, uint16_t WriteAddr) 
{
    
        /* !< Send STRAT condition */ 
        I2C_GenerateSTART (sEE_I2C, ENABLE);
    
        /* !< Test on EV5 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
        
    {
    }
    
        /* !< Send EEPROM address for write */ 
        I2C_Send7bitAddress (sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
    
        /* !< Test on EV6 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        
    {
    }
    
#ifdef sEE_M24C08
        
        /* !< Send the EEPROM's internal address to write to : only one byte Address */ 
        I2C_SendData (sEE_I2C, WriteAddr);
    
#elif defined(sEE_M24C64_32)
        
        /* !< Send the EEPROM's internal address to write to : MSB of the address first */ 
        I2C_SendData (sEE_I2C, (uint8_t) ((WriteAddr & 0xFF00) >> 8));
    
        /* !< Test on EV8 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        
    {
    }
    
        /* !< Send the EEPROM's internal address to write to : LSB of the address */ 
        I2C_SendData (sEE_I2C, (uint8_t) (WriteAddr & 0x00FF));
    
#endif  /*!< sEE_M24C08 */  
        
        /* !< Test on EV8 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        
    {
    }
    
        /* !< Send the byte to be written */ 
        I2C_SendData (sEE_I2C, *pBuffer);
    
        /* !< Test on EV8 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        
    {
    }
    
        /* !< Send STOP condition */ 
        I2C_GenerateSTOP (sEE_I2C, ENABLE);
}


/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read from 
  *         the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to read from.
  * @param  NumByteToRead : pointer to the variable holding number of bytes to 
  *         read from the EEPROM. 
  * 
  *        @note The variable pointed by NumByteToRead is reset to 0 when all the 
  *              data are read from the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * @note When number of data to be read is higher than 1, this function just 
  *       configure the communication and enable the DMA channel to transfer data.
  *       Meanwhile, the user application may perform other tasks.
  *       When number of data to be read is 1, then the DMA is not used.
  * 
  * @retval None
  */ 
void sEE_ReadBuffer (uint8_t * pBuffer, uint16_t ReadAddr, uint16_t * NumByteToRead) 
{
    
        /* Set the pointer to the Number of data to be read. This pointer will be used  by the DMA Transfer Completer interrupt Handler in order to
           reset the  variable to 0. User should check on this variable in order to know if the  DMA transfer has been complete or not. */ 
        sEEDataReadPointer = NumByteToRead;
    
        /* !< While the bus is busy */ 
        while (I2C_GetFlagStatus (sEE_I2C, I2C_FLAG_BUSY))
        
    {
    }
    
        /* !< Send START condition */ 
        I2C_GenerateSTART (sEE_I2C, ENABLE);
    
        /* !< Test on EV5 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
        
    {
    }
    
        /* !< Send EEPROM address for write */ 
        I2C_Send7bitAddress (sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
    
        /* !< Test on EV6 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        
    {
    }
    
#ifdef sEE_M24C08  
        
        /* !< Send the EEPROM's internal address to read from: Only one byte address */ 
        I2C_SendData (sEE_I2C, ReadAddr);
    
#elif defined (sEE_M24C64_32)
        
        /* !< Send the EEPROM's internal address to read from: MSB of the address first */ 
        I2C_SendData (sEE_I2C, (uint8_t) ((ReadAddr & 0xFF00) >> 8));
    
        /* !< Test on EV8 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        
    {
    }
    
        /* !< Send the EEPROM's internal address to read from: LSB of the address */ 
        I2C_SendData (sEE_I2C, (uint8_t) (ReadAddr & 0x00FF));
    
#endif  /*!< sEE_M24C08 */
        
        /* !< Test on EV8 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        
    {
    }
    
        /* !< Send STRAT condition a second time */ 
        I2C_GenerateSTART (sEE_I2C, ENABLE);
    
        /* !< Test on EV5 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
        
    {
    }
    
        /* !< Send EEPROM address for read */ 
        I2C_Send7bitAddress (sEE_I2C, sEEAddress, I2C_Direction_Receiver);
    
        /* !< Test on EV6 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        
    {
    }
    
        /* If number of data to be read is 1, then DMA couldn't be used */ 
        if ((uint16_t) (*NumByteToRead) < 2)
        
    {
        
            /* !< Disable Acknowledgement */ 
            I2C_AcknowledgeConfig (sEE_I2C, DISABLE);
        
            /* !< Send STOP Condition */ 
            I2C_GenerateSTOP (sEE_I2C, ENABLE);
        
            /* !< Test on EV7 and clear it */ 
            while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
            
        {
        }
        
            /* !< Read a byte from the EEPROM */ 
            *pBuffer = I2C_ReceiveData (sEE_I2C);
        
            /* !< Decrement the read bytes counter */ 
            (uint16_t) (*NumByteToRead)--;
        
            /* !< Enable Acknowledgement to be ready for another reception */ 
            I2C_AcknowledgeConfig (sEE_I2C, ENABLE);
    }
    
        /* DMA could be used for number of data higher than 1 */ 
        else
        
    {
        
            /* Configure the DMA Rx Channel with the buffer address and the buffer size */ 
            sEE_LowLevel_DMAConfig ((uint32_t) pBuffer, (uint16_t) (*NumByteToRead), sEE_DIRECTION_RX);
        
            /* Inform the DMA that the next End Of Transfer Signal will be the last one */ 
            I2C_DMALastTransferCmd (sEE_I2C, ENABLE);
        
            /* Enable the DMA Rx Channel */ 
            DMA_Cmd (sEE_I2C_DMA_CHANNEL_RX, ENABLE);
    }
}


/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */ 
void sEE_WriteBuffer (uint8_t * pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite) 
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
    uint16_t Addr = 0;
    Addr = WriteAddr % sEE_PAGESIZE;
    count = sEE_PAGESIZE - Addr;
    NumOfPage = NumByteToWrite / sEE_PAGESIZE;
    NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
    
        /* !< If WriteAddr is sEE_PAGESIZE aligned */ 
        if (Addr == 0)
        
    {
        
            /* !< If NumByteToWrite < sEE_PAGESIZE */ 
            if (NumOfPage == 0)
            
        {
            
                /* Store the number of data to be written */ 
                sEEDataNum = NumOfSingle;
            
                /* Start writing data */ 
                sEE_WritePage (pBuffer, WriteAddr, (uint8_t *) (&sEEDataNum));
            
                /* Wait transfer through DMA to be complete */ 
                sEETimeout = sEE_TIMEOUT_MAX;
            while ((sEEDataNum > 0) && (sEETimeout-- > 0))
                
            {
            }
            sEE_WaitEepromStandbyState ();
        }
        
            /* !< If NumByteToWrite > sEE_PAGESIZE */ 
            else
            
        {
            while (NumOfPage--)
                
            {
                
                    /* Store the number of data to be written */ 
                    sEEDataNum = sEE_PAGESIZE;
                sEE_WritePage (pBuffer, WriteAddr, (uint8_t *) (&sEEDataNum));
                
                    /* Wait transfer through DMA to be complete */ 
                    sEETimeout = sEE_TIMEOUT_MAX;
                while ((sEEDataNum > 0) && (sEETimeout-- > 0))
                    
                {
                }
                sEE_WaitEepromStandbyState ();
                WriteAddr += sEE_PAGESIZE;
                pBuffer += sEE_PAGESIZE;
            }
            if (NumOfSingle != 0)
                
            {
                
                    /* Store the number of data to be written */ 
                    sEEDataNum = NumOfSingle;
                sEE_WritePage (pBuffer, WriteAddr, (uint8_t *) (&sEEDataNum));
                
                    /* Wait transfer through DMA to be complete */ 
                    sEETimeout = sEE_TIMEOUT_MAX;
                while ((sEEDataNum > 0) && (sEETimeout-- > 0))
                    
                {
                }
                sEE_WaitEepromStandbyState ();
            }
        }
    }
    
        /* !< If WriteAddr is not sEE_PAGESIZE aligned */ 
        else
        
    {
        
            /* !< If NumByteToWrite < sEE_PAGESIZE */ 
            if (NumOfPage == 0)
            
        {
            
                /* !< If the number of data to be written is more than the remaining space  in the current page: */ 
                if (NumByteToWrite > count)
                
            {
                
                    /* Store the number of data to be written */ 
                    sEEDataNum = count;
                
                    /* !< Write the data conained in same page */ 
                    sEE_WritePage (pBuffer, WriteAddr, (uint8_t *) (&sEEDataNum));
                
                    /* Wait transfer through DMA to be complete */ 
                    sEETimeout = sEE_TIMEOUT_MAX;
                while ((sEEDataNum > 0) && (sEETimeout-- > 0))
                    
                {
                }
                sEE_WaitEepromStandbyState ();
                
                    /* Store the number of data to be written */ 
                    sEEDataNum = (NumByteToWrite - count);
                
                    /* !< Write the remaining data in the following page */ 
                    sEE_WritePage ((uint8_t *) (pBuffer + count), (WriteAddr + count), (uint8_t *) (&sEEDataNum));
                
                    /* Wait transfer through DMA to be complete */ 
                    sEETimeout = sEE_TIMEOUT_MAX;
                while ((sEEDataNum > 0) && (sEETimeout-- > 0))
                    
                {
                }
                sEE_WaitEepromStandbyState ();
            }
            
            else
                
            {
                
                    /* Store the number of data to be written */ 
                    sEEDataNum = NumOfSingle;
                sEE_WritePage (pBuffer, WriteAddr, (uint8_t *) (&sEEDataNum));
                
                    /* Wait transfer through DMA to be complete */ 
                    sEETimeout = sEE_TIMEOUT_MAX;
                while ((sEEDataNum > 0) && (sEETimeout-- > 0))
                    
                {
                }
                sEE_WaitEepromStandbyState ();
            }
        }
        
            /* !< If NumByteToWrite > sEE_PAGESIZE */ 
            else
            
        {
            NumByteToWrite -= count;
            NumOfPage = NumByteToWrite / sEE_PAGESIZE;
            NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
            if (count != 0)
                
            {
                
                    /* Store the number of data to be written */ 
                    sEEDataNum = count;
                sEE_WritePage (pBuffer, WriteAddr, (uint8_t *) (&sEEDataNum));
                
                    /* Wait transfer through DMA to be complete */ 
                    sEETimeout = sEE_TIMEOUT_MAX;
                while ((sEEDataNum > 0) && (sEETimeout-- > 0))
                    
                {
                }
                sEE_WaitEepromStandbyState ();
                WriteAddr += count;
                pBuffer += count;
            }
            while (NumOfPage--)
                
            {
                
                    /* Store the number of data to be written */ 
                    sEEDataNum = sEE_PAGESIZE;
                sEE_WritePage (pBuffer, WriteAddr, (uint8_t *) (&sEEDataNum));
                
                    /* Wait transfer through DMA to be complete */ 
                    sEETimeout = sEE_TIMEOUT_MAX;
                while ((sEEDataNum > 0) && (sEETimeout-- > 0))
                    
                {
                }
                sEE_WaitEepromStandbyState ();
                WriteAddr += sEE_PAGESIZE;
                pBuffer += sEE_PAGESIZE;
            }
            if (NumOfSingle != 0)
                
            {
                
                    /* Store the number of data to be written */ 
                    sEEDataNum = NumOfSingle;
                sEE_WritePage (pBuffer, WriteAddr, (uint8_t *) (&sEEDataNum));
                
                    /* Wait transfer through DMA to be complete */ 
                    sEETimeout = sEE_TIMEOUT_MAX;
                while ((sEEDataNum > 0) && (sEETimeout-- > 0))
                    
                {
                }
                sEE_WaitEepromStandbyState ();
            }
        }
    }
}


/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  pBuffer : pointer to the buffer containing the data to be written to 
  *         the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : pointer to the variable holding number of bytes to 
  *         written to the EEPROM. 
  * 
  *        @note The variable pointed by NumByteToWrite is reset to 0 when all the 
  *              data are read from the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * @note When number of data to be written is higher than 1, this function just 
  *       configure the communication and enable the DMA channel to transfer data.
  *       Meanwhile, the user application may perform other tasks.
  *       When number of data to be written is 1, then the DMA is not used.
  * 
  * @retval None
  */ 
void sEE_WritePage (uint8_t * pBuffer, uint16_t WriteAddr, uint8_t * NumByteToWrite) 
{
    
        /* Set the pointer to the Number of data to be written. This pointer will be used  by the DMA Transfer Completer interrupt Handler in order
           to reset the  variable to 0. User should check on this variable in order to know if the  DMA transfer has been complete or not. */ 
        sEEDataWritePointer = NumByteToWrite;
    
        /* !< While the bus is busy */ 
        while (I2C_GetFlagStatus (sEE_I2C, I2C_FLAG_BUSY))
        
    {
    }
    
        /* !< Send START condition */ 
        I2C_GenerateSTART (sEE_I2C, ENABLE);
    
        /* !< Test on EV5 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
        
    {
    }
    
        /* !< Send EEPROM address for write */ 
        I2C_Send7bitAddress (sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
    
        /* !< Test on EV6 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        
    {
    }
    
#ifdef sEE_M24C08
        
        /* !< Send the EEPROM's internal address to write to : only one byte Address */ 
        I2C_SendData (sEE_I2C, WriteAddr);
    
#elif defined(sEE_M24C64_32)
        
        /* !< Send the EEPROM's internal address to write to : MSB of the address first */ 
        I2C_SendData (sEE_I2C, (uint8_t) ((WriteAddr & 0xFF00) >> 8));
    
        /* !< Test on EV8 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        
    {
    }
    
        /* !< Send the EEPROM's internal address to write to : LSB of the address */ 
        I2C_SendData (sEE_I2C, (uint8_t) (WriteAddr & 0x00FF));
    
#endif  /*!< sEE_M24C08 */  
        
        /* !< Test on EV8 and clear it */ 
        while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        
    {
    }
    
        /* If number of data to be written is 1, then DMA couldn't be used */ 
        if ((uint16_t) (*NumByteToWrite) < 2)
        
    {
        
            /* !< Send the byte to be written */ 
            I2C_SendData (sEE_I2C, *pBuffer);
        
            /* !< Test on EV8 and clear it */ 
            while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
            
        {
        }
        
            /* !< Send STOP condition */ 
            I2C_GenerateSTOP (sEE_I2C, ENABLE);
        (uint8_t) (*NumByteToWrite)--;
    }
    
        /* DMA could be used for number of data higher than 1 */ 
        else
        
    {
        
            /* Configure the DMA Tx Channel with the buffer address and the buffer size */ 
            sEE_LowLevel_DMAConfig ((uint32_t) pBuffer, (uint8_t) (*NumByteToWrite), sEE_DIRECTION_TX);
        
            /* Enable the DMA Tx Channel */ 
            DMA_Cmd (sEE_I2C_DMA_CHANNEL_TX, ENABLE);
    }
}


/**
  * @brief  Wait for EEPROM Standby state
  * @param  None
  * @retval None
  */ 
void sEE_WaitEepromStandbyState (void) 
{
    __IO uint16_t SR1_Tmp = 0;
    
    do
        
    {
        
            /* !< Send START condition */ 
            I2C_GenerateSTART (sEE_I2C, ENABLE);
        
            /* !< Read sEE SR1 register to clear pending flags */ 
            SR1_Tmp = I2C_ReadRegister (sEE_I2C, I2C_Register_SR1);
        
            /* !< Send EEPROM address for write */ 
            I2C_Send7bitAddress (sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
    } while (!(I2C_ReadRegister (sEE_I2C, I2C_Register_SR1) & 0x0002));
    
        /* !< Clear AF flag */ 
        I2C_ClearFlag (sEE_I2C, I2C_FLAG_AF);
    
        /* !< STOP condition */ 
        I2C_GenerateSTOP (sEE_I2C, ENABLE);
}


/**
  * @}
  */ 
    
/**
  * @brief  This function handles the DMA Tx Channel interrupt Handler.
  * @param  None
  * @retval None
  */ 
void sEE_I2C_DMA_TX_IRQHandler (void) 
{
    
        /* Check if the DMA transfer is complete */ 
        if (DMA_GetFlagStatus (sEE_I2C_DMA_FLAG_TX_TC) != RESET)
        
    {
        
            /* Disable the DMA Tx Channel and Clear all its Flags */ 
            DMA_Cmd (sEE_I2C_DMA_CHANNEL_TX, DISABLE);
        DMA_ClearFlag (sEE_I2C_DMA_FLAG_TX_GL);
        
            /* !< Wait till all data have been physically transferred on the bus */ 
            while (!I2C_CheckEvent (sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
            
        {
        }
        
            /* !< Send STOP condition */ 
            I2C_GenerateSTOP (sEE_I2C, ENABLE);
        
            /* Reset the variable holding the number of data to be written */ 
            *sEEDataWritePointer = 0;
    }
}


/**
  * @brief  This function handles the DMA Rx Channel interrupt Handler.
  * @param  None
  * @retval None
  */ 
void sEE_I2C_DMA_RX_IRQHandler (void) 
{
    
        /* Check if the DMA transfer is complete */ 
        if (DMA_GetFlagStatus (sEE_I2C_DMA_FLAG_RX_TC) != RESET)
        
    {
        
            /* !< Send STOP Condition */ 
            I2C_GenerateSTOP (sEE_I2C, ENABLE);
        
            /* Disable the DMA Rx Channel and Clear all its Flags */ 
            DMA_Cmd (sEE_I2C_DMA_CHANNEL_RX, DISABLE);
        DMA_ClearFlag (sEE_I2C_DMA_FLAG_RX_GL);
        
            /* Reset the variable holding the number of data to be read */ 
            *sEEDataReadPointer = 0;
    }
}


/**
  * @}
  */ 
    
/**
  * @}
  */ 
    
/**
  * @}
  */ 
    
/**
  * @}
  */ 
    
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/ 
