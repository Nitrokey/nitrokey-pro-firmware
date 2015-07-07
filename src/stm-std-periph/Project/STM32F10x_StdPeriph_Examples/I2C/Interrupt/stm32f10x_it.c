/**
  ******************************************************************************
  * @file    I2C/Interrupt/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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
#include "stm32f10x_it.h"
#include "main.h"
    
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */ 
    
/** @addtogroup I2C_Interrupt
  * @{
  */ 
    
    /* Private typedef ----------------------------------------------------------- */ 
    /* Private define ------------------------------------------------------------ */ 
    /* Private macro ------------------------------------------------------------- */ 
    /* Private variables --------------------------------------------------------- */ 
extern __IO uint8_t Tx1_Idx, Rx1_Idx, Tx2_Idx, Rx2_Idx, Direction;

extern uint8_t I2C1_Buffer_Tx[], I2C1_Buffer_Rx[];

extern uint8_t I2C2_Buffer_Tx[], I2C2_Buffer_Rx[];


    /* Private function prototypes ----------------------------------------------- */ 
    /* Private functions --------------------------------------------------------- */ 
    
/******************************************************************************/ 
    /* Cortex-M3 Processor Exceptions Handlers */ 
/******************************************************************************/ 
    
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */ 
void NMI_Handler (void) 
{
} 

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */ 
void HardFault_Handler (void) 
{
    
        /* Go to infinite loop when Hard Fault exception occurs */ 
        while (1)
        
    {
    }
}


/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */ 
void MemManage_Handler (void) 
{
    
        /* Go to infinite loop when Memory Manage exception occurs */ 
        while (1)
        
    {
    }
}


/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */ 
void BusFault_Handler (void) 
{
    
        /* Go to infinite loop when Bus Fault exception occurs */ 
        while (1)
        
    {
    }
}


/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */ 
void UsageFault_Handler (void) 
{
    
        /* Go to infinite loop when Usage Fault exception occurs */ 
        while (1)
        
    {
    }
}


/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */ 
void SVC_Handler (void) 
{
} 

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */ 
void DebugMon_Handler (void) 
{
} 

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */ 
void PendSV_Handler (void) 
{
} 

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */ 
void SysTick_Handler (void) 
{
} 

/******************************************************************************/ 
    /* STM32F10x Peripherals Interrupt Handlers */ 
/******************************************************************************/ 
    
/**
  * @brief  This function handles I2C1 Event interrupt request.
  * @param  None
  * @retval None
  */ 
void I2C1_EV_IRQHandler (void) 
{
    switch (I2C_GetLastEvent (I2C1))
        
    {
        case I2C_EVENT_MASTER_MODE_SELECT:    /* EV5 */
            if (Direction == Transmitter)
                
            {
                
                    /* Master Transmitter ---------------------------------------------- */ 
                    /* Send slave Address for write */ 
                    I2C_Send7bitAddress (I2C1, I2C2_SLAVE_ADDRESS7, I2C_Direction_Transmitter);
            }
            
            else
                
            {
                
                    /* Master Receiver ------------------------------------------------- */ 
                    /* Send slave Address for read */ 
                    I2C_Send7bitAddress (I2C1, I2C2_SLAVE_ADDRESS7, I2C_Direction_Receiver);
            }
            break;
            
                /* Master Transmitter -------------------------------------------------- */ 
                /* Test on I2C1 EV6 and first EV8 and clear them */ 
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:
            
                /* Send the first data */ 
                I2C_SendData (I2C1, I2C1_Buffer_Tx[Tx1_Idx++]);
            break;
            
                /* Test on I2C1 EV8 and clear it */ 
        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:   /* Without BTF, EV8 */
            if (Tx1_Idx < (Tx1BufferSize))
                
            {
                
                    /* Transmit I2C1 data */ 
                    I2C_SendData (I2C1, I2C1_Buffer_Tx[Tx1_Idx++]);
            }
            
            else
                
            {
                I2C_TransmitPEC (I2C1, ENABLE);
                I2C_ITConfig (I2C1, I2C_IT_BUF, DISABLE);
            }
            break;
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:  /* With BTF EV8-2 */
            I2C_ITConfig (I2C1, I2C_IT_BUF, ENABLE);
            
                /* I2C1 Re-START Condition */ 
                I2C_GenerateSTART (I2C1, ENABLE);
            break;
            
                /* Master Receiver ------------------------------------------------------- */ 
        case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
            if (Tx2BufferSize == 1)
                
            {
                
                    /* Disable I2C1 acknowledgement */ 
                    I2C_AcknowledgeConfig (I2C1, DISABLE);
                
                    /* Send I2C1 STOP Condition */ 
                    I2C_GenerateSTOP (I2C1, ENABLE);
            }
            break;
            
                /* Test on I2C1 EV7 and clear it */ 
        case I2C_EVENT_MASTER_BYTE_RECEIVED:
            
                /* Store I2C1 received data */ 
                I2C1_Buffer_Rx[Rx1_Idx++] = I2C_ReceiveData (I2C1);
            
                /* Disable ACK and send I2C1 STOP condition before receiving the last data */ 
                if (Rx1_Idx == (Tx2BufferSize - 1))
                
            {
                
                    /* Disable I2C1 acknowledgement */ 
                    I2C_AcknowledgeConfig (I2C1, DISABLE);
                
                    /* Send I2C1 STOP Condition */ 
                    I2C_GenerateSTOP (I2C1, ENABLE);
            }
            break;
        default:
            break;
    }
}


/**
  * @brief  This function handles I2C2 Event interrupt request.
  * @param  None
  * @retval None
  */ 
void I2C2_EV_IRQHandler (void) 
{
    switch (I2C_GetLastEvent (I2C2))
        
    {
            
                /* Slave Transmitter --------------------------------------------------- */ 
        case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:  /* EV1 */
            
                /* Transmit I2C2 data */ 
                I2C_SendData (I2C2, I2C2_Buffer_Tx[Tx2_Idx++]);
            break;
        case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:   /* EV3 */
            
                /* Transmit I2C2 data */ 
                I2C_SendData (I2C2, I2C2_Buffer_Tx[Tx2_Idx++]);
            break;
            
                /* Slave Receiver ------------------------------------------------------ */ 
        case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED: /* EV1 */
            break;
        case I2C_EVENT_SLAVE_BYTE_RECEIVED:  /* EV2 */
            
                /* Store I2C2 received data */ 
                I2C2_Buffer_Rx[Rx2_Idx++] = I2C_ReceiveData (I2C2);
            if (Rx2_Idx == Tx2BufferSize)
                
            {
                I2C_TransmitPEC (I2C2, ENABLE);
                Direction = Receiver;
            }
            break;
        case I2C_EVENT_SLAVE_STOP_DETECTED:  /* EV4 */
            
                /* Clear I2C2 STOPF flag: read of I2C_SR1 followed by a write on I2C_CR1 */ 
                (void) (I2C_GetITStatus (I2C2, I2C_IT_STOPF));
            I2C_Cmd (I2C2, ENABLE);
            break;
        default:
            break;
    }
}


/**
  * @brief  This function handles I2C2 Error interrupt request.
  * @param  None
  * @retval None
  */ 
void I2C2_ER_IRQHandler (void) 
{
    
        /* Check on I2C2 AF flag and clear it */ 
        if (I2C_GetITStatus (I2C2, I2C_IT_AF))
        
    {
        I2C_ClearITPendingBit (I2C2, I2C_IT_AF);
    }
}


/******************************************************************************/ 
    /* STM32F10x Peripherals Interrupt Handlers */ 
    /* Add here the Interrupt Handler for the used peripheral(s) (PPP), for the */ 
    /* available peripheral interrupt handler's name please refer to the startup */ 
    /* file (startup_stm32f10x_xx.s).  */ 
/******************************************************************************/ 
    
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */ 
    /* void PPP_IRQHandler(void) { } */ 
    
/**
  * @}
  */ 
    
/**
  * @}
  */ 
    
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/ 
