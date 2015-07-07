/**
  ******************************************************************************
  * @file    CAN/LoopBack/main.c 
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
#include "stm32_eval.h"
    
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */ 
    
/** @addtogroup CAN_LoopBack
  * @{
  */ 
    
    /* Private typedef ----------------------------------------------------------- */ 
    typedef enum
{ FAILED = 0, PASSED = !FAILED } TestStatus;


    /* Private define ------------------------------------------------------------ */ 
    /* Private macro ------------------------------------------------------------- */ 
    /* Private variables --------------------------------------------------------- */ 
    __IO uint32_t ret = 0;  /* for return of the interrupt handling */
volatile TestStatus TestRx;


    /* Private function prototypes ----------------------------------------------- */ 
void NVIC_Configuration (void);

TestStatus CAN_Polling (void);

TestStatus CAN_Interrupt (void);


    /* Private functions --------------------------------------------------------- */ 
    
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */ 
int main (void) 
{
    
        /* !< At this stage the microcontroller clock setting is already configured,  this is done through SystemInit() function which is called
           from startup file (startup_stm32f10x_xx.s) before to branch to application main. To reconfigure the default setting of SystemInit()
           function, refer to system_stm32f10x.c file */ 
        
        /* CAN1 Periph clock enable */ 
        RCC_APB1PeriphClockCmd (RCC_APB1Periph_CAN1, ENABLE);
    
        /* NVIC Configuration */ 
        NVIC_Configuration ();
    
        /* Configures LED 1..4 */ 
        STM_EVAL_LEDInit (LED1);
    STM_EVAL_LEDInit (LED2);
    STM_EVAL_LEDInit (LED3);
    STM_EVAL_LEDInit (LED4);
    
        /* Turns selected LED Off */ 
        STM_EVAL_LEDOff (LED1);
    STM_EVAL_LEDOff (LED2);
    STM_EVAL_LEDOff (LED3);
    STM_EVAL_LEDOff (LED4);
    
        /* CAN transmit at 100Kb/s and receive by polling in loopback mode */ 
        TestRx = CAN_Polling ();
    if (TestRx == FAILED)
        
    {
        
            /* Turn on led LD3 */ 
            STM_EVAL_LEDOn (LED3);
    }
    
    else
        
    {
        
            /* Turn on led LD1 */ 
            STM_EVAL_LEDOn (LED1);
    }
    
        /* CAN transmit at 500Kb/s and receive by interrupt in loopback mode */ 
        TestRx = CAN_Interrupt ();
    if (TestRx == FAILED)
        
    {
        
            /* Turn on led LD4 */ 
            STM_EVAL_LEDOn (LED4);
    }
    
    else
        
    {
        
            /* Turn on led LD2 */ 
            STM_EVAL_LEDOn (LED2);
    }
    
        /* Infinite loop */ 
        while (1)
        
    {
    }
}


/**
  * @brief  Configures the CAN, transmit and receive by polling
  * @param  None
  * @retval PASSED if the reception is well done, FAILED in other case
  */ 
    TestStatus CAN_Polling (void) 
{
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    CanTxMsg TxMessage;
    CanRxMsg RxMessage;
    uint32_t i = 0;
    uint8_t TransmitMailbox = 0;
    
        /* CAN register init */ 
        CAN_DeInit (CAN1);
    CAN_StructInit (&CAN_InitStructure);
    
        /* CAN cell init */ 
        CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
    CAN_InitStructure.CAN_Prescaler = 5;
    CAN_Init (CAN1, &CAN_InitStructure);
    
        /* CAN filter init */ 
        CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit (&CAN_FilterInitStructure);
    
        /* transmit */ 
        TxMessage.StdId = 0x11;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = 2;
    TxMessage.Data[0] = 0xCA;
    TxMessage.Data[1] = 0xFE;
    TransmitMailbox = CAN_Transmit (CAN1, &TxMessage);
    i = 0;
    while ((CAN_TransmitStatus (CAN1, TransmitMailbox) != CANTXOK) && (i != 0xFF))
        
    {
        i++;
    }
    i = 0;
    while ((CAN_MessagePending (CAN1, CAN_FIFO0) < 1) && (i != 0xFF))
        
    {
        i++;
    }
    
        /* receive */ 
        RxMessage.StdId = 0x00;
    RxMessage.IDE = CAN_ID_STD;
    RxMessage.DLC = 0;
    RxMessage.Data[0] = 0x00;
    RxMessage.Data[1] = 0x00;
    CAN_Receive (CAN1, CAN_FIFO0, &RxMessage);
    if (RxMessage.StdId != 0x11)
        
    {
        return FAILED;
    }
    if (RxMessage.IDE != CAN_ID_STD)
        
    {
        return FAILED;
    }
    if (RxMessage.DLC != 2)
        
    {
        return FAILED;
    }
    if ((RxMessage.Data[0] << 8 | RxMessage.Data[1]) != 0xCAFE)
        
    {
        return FAILED;
    }
    return PASSED;    /* Test Passed */
}


/**
  * @brief  Configures the CAN, transmit and receive using interrupt.
  * @param  None
  * @retval PASSED if the reception is well done, FAILED in other case
  */ 
    TestStatus CAN_Interrupt (void) 
{
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    CanTxMsg TxMessage;
    uint32_t i = 0;
    
        /* CAN register init */ 
        CAN_DeInit (CAN1);
    CAN_StructInit (&CAN_InitStructure);
    
        /* CAN cell init */ 
        CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
    CAN_InitStructure.CAN_Prescaler = 1;
    CAN_Init (CAN1, &CAN_InitStructure);
    
        /* CAN filter init */ 
        CAN_FilterInitStructure.CAN_FilterNumber = 1;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit (&CAN_FilterInitStructure);
    
        /* CAN FIFO0 message pending interrupt enable */ 
        CAN_ITConfig (CAN1, CAN_IT_FMP0, ENABLE);
    
        /* transmit 1 message */ 
        TxMessage.StdId = 0x00;
    TxMessage.ExtId = 0x1234;
    TxMessage.IDE = CAN_ID_EXT;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 2;
    TxMessage.Data[0] = 0xDE;
    TxMessage.Data[1] = 0xCA;
    CAN_Transmit (CAN1, &TxMessage);
    
        /* initialize the value that will be returned */ 
        ret = 0xFF;
    
        /* receive message with interrupt handling */ 
        i = 0;
    while ((ret == 0xFF) && (i < 0xFFF))
        
    {
        i++;
    }
    if (i == 0xFFF)
        
    {
        ret = 0;
    }
    
        /* disable interrupt handling */ 
        CAN_ITConfig (CAN1, CAN_IT_FMP0, DISABLE);
    return (TestStatus) ret;
}


/**
  * @brief  Configures the NVIC and Vector Table base address.
  * @param  None
  * @retval None
  */ 
void NVIC_Configuration (void) 
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
        /* Enable CAN1 RX0 interrupt IRQ channel */ 
#ifndef STM32F10X_CL
        NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    
#else /*  */
        NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    
#endif  /* STM32F10X_CL*/
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);
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
