/**
  ******************************************************************************
  * @file    IWDG/main.c 
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
    
/** @addtogroup IWDG_Example
  * @{
  */ 
    
    /* Private typedef ----------------------------------------------------------- */ 
    /* Private define ------------------------------------------------------------ */ 
    /* Private macro ------------------------------------------------------------- */ 
    /* Private variables --------------------------------------------------------- */ 
    /* Private function prototypes ----------------------------------------------- */ 
void SysTick_Configuration (void);


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
    
        /* Configure SysTick to generate an interrupt each 250ms */ 
        SysTick_Configuration ();
    
        /* 1 bits for pre-emption priority and 3 bits for subpriority */ 
        NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
    
        /* Set Button EXTI Interrupt priority to 0 (highest) */ 
        NVIC_SetPriority (KEY_BUTTON_EXTI_IRQn, NVIC_EncodePriority (NVIC_GetPriorityGrouping (), 0, 0));
    
        /* Set SysTick interrupt vector Preemption Priority to 1 */ 
        NVIC_SetPriority (SysTick_IRQn, NVIC_EncodePriority (NVIC_GetPriorityGrouping (), 1, 0));
    
        /* Check if the system has resumed from IWDG reset */ 
        if (RCC_GetFlagStatus (RCC_FLAG_IWDGRST) != RESET)
        
    {
        
            /* IWDGRST flag set */ 
            /* Turn on LED1 */ 
            STM_EVAL_LEDOn (LED1);
        
            /* Clear reset flags */ 
            RCC_ClearFlag ();
    }
    
    else
        
    {
        
            /* IWDGRST flag is not set */ 
            /* Turn off LED1 */ 
            STM_EVAL_LEDOff (LED1);
    }
    
        /* IWDG timeout equal to 280 ms (the timeout may varies due to LSI frequency dispersion) */ 
        /* Enable write access to IWDG_PR and IWDG_RLR registers */ 
        IWDG_WriteAccessCmd (IWDG_WriteAccess_Enable);
    
        /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */ 
        IWDG_SetPrescaler (IWDG_Prescaler_32);
    
        /* Set counter reload value to 349 */ 
        IWDG_SetReload (349);
    
        /* Reload IWDG counter */ 
        IWDG_ReloadCounter ();
    
        /* Enable IWDG (the LSI oscillator will be enabled by hardware) */ 
        IWDG_Enable ();
    while (1)
        
    {
    }
}


/**
  * @brief  Configures SysTick to generate an interrupt each 250ms.
  * @param  None
  * @retval None
  */ 
void SysTick_Configuration (void) 
{
    
        /* SysTick interrupt each 250ms with counter clock equal to (HCLK/8) */ 
        if (SysTick_Config ((SystemCoreClock / 8) / 4))
        
    {
        
            /* Capture error */ 
            while (1);
    }
    
        /* Select HCLK/8 as SysTick clock source */ 
        SysTick_CLKSourceConfig (SysTick_CLKSource_HCLK_Div8);
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
