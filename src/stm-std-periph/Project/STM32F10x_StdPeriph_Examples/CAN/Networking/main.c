/**
  ******************************************************************************
  * @file    CAN/Networking/main.c 
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
#include "platform_config.h"
    
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */ 
    
/** @addtogroup CAN_Networking
  * @{
  */ 
    
    /* Private typedef ----------------------------------------------------------- */ 
    /* Private define ------------------------------------------------------------ */ 
    /* Private define ------------------------------------------------------------ */ 
#define Key_Pressed   0x01
#define Key_NoPressed 0x00
    
    /* Private macro ------------------------------------------------------------- */ 
    /* Private variables --------------------------------------------------------- */ 
    CAN_InitTypeDef CAN_InitStructure;
CAN_FilterInitTypeDef CAN_FilterInitStructure;
RCC_ClocksTypeDef RCC_Clocks;
CanTxMsg TxMessage;
uint8_t Key_Pressed_Number = 0x0;

    /* Private function prototypes ----------------------------------------------- */ 
void RCC_Configuration (void);

void GPIO_Configuration (void);

void NVIC_Configuration (void);

void CAN_Config (void);

void LED_Display (uint8_t Ledstatus);

void Init_RxMes (CanRxMsg * RxMessage);

void Delay (void);


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
        
        /* System clocks configuration --------------------------------------------- */ 
        RCC_Configuration ();
    
        /* GPIO configuration ------------------------------------------------------ */ 
        GPIO_Configuration ();
    
        /* NVIC configuration ------------------------------------------------------ */ 
        NVIC_Configuration ();
    
        /* Configures LED 1..4 */ 
        STM_EVAL_LEDInit (LED1);
    STM_EVAL_LEDInit (LED2);
    STM_EVAL_LEDInit (LED3);
    STM_EVAL_LEDInit (LED4);
    RCC_GetClocksFreq (&RCC_Clocks);
    
        /* Configure Push buttion key */ 
        STM_EVAL_PBInit (BUTTON_KEY, BUTTON_MODE_GPIO);
    
        /* CAN configuration */ 
        CAN_Config ();
    CAN_ITConfig (CAN1, CAN_IT_FMP0, ENABLE);
    
        /* turn off all leds */ 
        STM_EVAL_LEDOff (LED1);
    STM_EVAL_LEDOff (LED2);
    STM_EVAL_LEDOff (LED3);
    STM_EVAL_LEDOff (LED4);
    
        /* Infinite loop */ 
        while (1)
        
    {
        while (STM_EVAL_PBGetState (BUTTON_KEY) == Key_Pressed)
            
        {
            if (Key_Pressed_Number == 0x4)
                
            {
                Key_Pressed_Number = 0x00;
            }
            
            else
                
            {
                LED_Display (++Key_Pressed_Number);
                TxMessage.Data[0] = Key_Pressed_Number;
                CAN_Transmit (CAN1, &TxMessage);
                Delay ();
                while (STM_EVAL_PBGetState (BUTTON_KEY) != Key_NoPressed)
                    
                {
                }
            }
        }
    }
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */ 
void RCC_Configuration (void) 
{
    
        /* GPIO clock enable */ 
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIO_CAN, ENABLE);
    
        /* CAN1 Periph clock enable */ 
        RCC_APB1PeriphClockCmd (RCC_APB1Periph_CAN1, ENABLE);
} 

/**
  * @brief  Configures the GPIO.
  * @param  None
  * @retval None
  */ 
void GPIO_Configuration (void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
        /* Configure CAN pin: RX */ 
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init (GPIO_CAN, &GPIO_InitStructure);
    
        /* Configure CAN pin: TX */ 
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_TX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init (GPIO_CAN, &GPIO_InitStructure);
    GPIO_PinRemapConfig (GPIO_Remap_CAN, ENABLE);
} 

/**
  * @brief  Configures the NVIC for CAN.
  * @param  None
  * @retval None
  */ 
void NVIC_Configuration (void) 
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig (NVIC_PriorityGroup_0);
    
#ifndef STM32F10X_CL
        NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    
#else /*  */
        NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    
#endif  /* STM32F10X_CL*/
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init (&NVIC_InitStructure);
} 

/**
  * @brief  Turn ON/OFF the dedicate led
  * @param  Ledstatus: Led number from 0 to 3 
  * @retval None
  */ 
void LED_Display (uint8_t Ledstatus) 
{
    
        /* turn off all leds */ 
        STM_EVAL_LEDOff (LED1);
    STM_EVAL_LEDOff (LED2);
    STM_EVAL_LEDOff (LED3);
    STM_EVAL_LEDOff (LED4);
    switch (Ledstatus)
        
    {
        case (1):
            STM_EVAL_LEDOn (LED1);
            break;
        case (2):
            STM_EVAL_LEDOn (LED2);
            break;
        case (3):
            STM_EVAL_LEDOn (LED3);
            break;
        case (4):
            STM_EVAL_LEDOn (LED4);
            break;
        default:
            break;
    }
}


/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */ 
void CAN_Config (void) 
{
    
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
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
    CAN_InitStructure.CAN_Prescaler = 4;
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
        TxMessage.StdId = 0x321;
    TxMessage.ExtId = 0x01;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = 1;
} 

/**
  * @brief  Delay
  * @param  None
  * @retval None
  */ 
void Delay () 
{
    uint16_t nTime = 0x0000;
    for (nTime = 0; nTime < 0xFFFF; nTime++);
}


/**
  * @brief  Initializes a Rx Message.
  * @param  CanRxMsg *RxMessage
  * @retval None
  */ 
void Init_RxMes (CanRxMsg * RxMessage) 
{
    uint8_t i = 0;
    RxMessage->StdId = 0x00;
    RxMessage->ExtId = 0x00;
    RxMessage->IDE = CAN_ID_STD;
    RxMessage->DLC = 0;
    RxMessage->FMI = 0;
    for (i = 0; i < 8; i++)
        RxMessage->Data[i] = 0x00;
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
