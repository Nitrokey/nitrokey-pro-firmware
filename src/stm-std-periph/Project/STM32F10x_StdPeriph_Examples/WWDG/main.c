/**
  ******************************************************************************
  * @file    WWDG/main.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main program body.
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
    
/** @addtogroup WWDG_Example
  * @{
  */ 
    
    /* Private typedef ----------------------------------------------------------- */ 
    /* Private define ------------------------------------------------------------ */ 
    /* Private macro ------------------------------------------------------------- */ 
    /* Private variables --------------------------------------------------------- */ 
    /* Private function prototypes ----------------------------------------------- */ 
void NVIC_Configuration (void);


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
        
        /* Initialize LED1 and Key Button mounted on STM3210X-EVAL board */ 
        STM_EVAL_LEDInit (LED1);
    STM_EVAL_LEDInit (LED2);
    STM_EVAL_PBInit (BUTTON_KEY, BUTTON_MODE_EXTI);
    
        /* Check if the system has resumed from WWDG reset */ 
        if (RCC_GetFlagStatus (RCC_FLAG_WWDGRST) != RESET)
        
    {
        
            /* WWDGRST flag set */ 
            /* Turn on LED1 */ 
            STM_EVAL_LEDOn (LED1);
        
            /* Clear reset flags */ 
            RCC_ClearFlag ();
    }
    
    else
        
    {
        
            /* WWDGRST flag is not set */ 
            /* Turn off LED1 */ 
            STM_EVAL_LEDOff (LED1);
    }
    
        /* NVIC configuration */ 
        NVIC_Configuration ();
    
        /* WWDG configuration */ 
        /* Enable WWDG clock */ 
        RCC_APB1PeriphClockCmd (RCC_APB1Periph_WWDG, ENABLE);
    
        /* On Value line devices, WWDG clock counter = (PCLK1 (24MHz)/4096)/8 = 732 Hz (~1366 æs) */ 
        /* On other devices, WWDG clock counter = (PCLK1(36MHz)/4096)/8 = 1099 Hz (~910 æs) */ 
        WWDG_SetPrescaler (WWDG_Prescaler_8);
    
        /* Set Window value to 65 */ 
        WWDG_SetWindowValue (65);
    
        /* On Value line devices, Enable WWDG and set counter value to 127, WWDG timeout = ~1366 æs * 64 = 87.42 ms */ 
        /* On other devices, Enable WWDG and set counter value to 127, WWDG timeout = ~910 æs * 64 = 58.25 ms */ 
        WWDG_Enable (127);
    
        /* Clear EWI flag */ 
        WWDG_ClearFlag ();
    
        /* Enable EW interrupt */ 
        WWDG_EnableIT ();
    while (1)
        
    {
    }
}


/**
  * @brief  Configures NVIC.
  * @param  None
  * @retval None
  */ 
void NVIC_Configuration (void) 
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
        /* 1 bits for pre-emption priority and 3 bits for subpriority */ 
        NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
    
        /* Set Button EXTI Interrupt priority to 0 (highest) */ 
        NVIC_SetPriority (KEY_BUTTON_EXTI_IRQn, NVIC_EncodePriority (NVIC_GetPriorityGrouping (), 0, 0));
    
        /* Set WWDG interrupt vector Preemption Priority to 1 */ 
        NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
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
