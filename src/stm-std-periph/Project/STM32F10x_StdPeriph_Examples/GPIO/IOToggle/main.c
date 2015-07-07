/**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
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
    
/** @addtogroup GPIO_IOToggle
  * @{
  */ 
    
    /* Private typedef ----------------------------------------------------------- */ 
    /* Private define ------------------------------------------------------------ */ 
    /* Private macro ------------------------------------------------------------- */ 
    /* Private variables --------------------------------------------------------- */ 
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Private function prototypes ----------------------------------------------- */ 
void Delay (__IO uint32_t nCount);


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
        
        /* Configure all unused GPIO port pins in Analog Input mode (floating input trigger OFF), this will reduce the power consumption and
           increase the device immunity against EMI/EMC ************************************************ */ 
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                                RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init (GPIOA, &GPIO_InitStructure);
    GPIO_Init (GPIOB, &GPIO_InitStructure);
    GPIO_Init (GPIOC, &GPIO_InitStructure);
    GPIO_Init (GPIOD, &GPIO_InitStructure);
    GPIO_Init (GPIOE, &GPIO_InitStructure);
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                              RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, DISABLE);
    
#ifdef USE_STM3210E_EVAL
        RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);
    GPIO_Init (GPIOF, &GPIO_InitStructure);
    GPIO_Init (GPIOG, &GPIO_InitStructure);
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, DISABLE);
    
#endif  /* USE_STM3210E_EVAL */
        
        /* Initialize Leds mounted on STM3210X-EVAL board */ 
        STM_EVAL_LEDInit (LED1);
    STM_EVAL_LEDInit (LED2);
    STM_EVAL_LEDInit (LED3);
    STM_EVAL_LEDInit (LED4);
    while (1)
        
    {
        
            /* Turn on LD1 */ 
            STM_EVAL_LEDOn (LED1);
        
            /* Insert delay */ 
            Delay (0xAFFFF);
        
            /* Turn on LD2 and LD3 */ 
            STM_EVAL_LEDOn (LED2);
        STM_EVAL_LEDOn (LED3);
        
            /* Turn off LD1 */ 
            STM_EVAL_LEDOff (LED1);
        
            /* Insert delay */ 
            Delay (0xAFFFF);
        
            /* Turn on LD4 */ 
            STM_EVAL_LEDOn (LED4);
        
            /* Turn off LD2 and LD3 */ 
            STM_EVAL_LEDOff (LED2);
        STM_EVAL_LEDOff (LED3);
        
            /* Insert delay */ 
            Delay (0xAFFFF);
        
            /* Turn off LD4 */ 
            STM_EVAL_LEDOff (LED4);
    }
}


/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */ 
void Delay (__IO uint32_t nCount) 
{
    for (; nCount != 0; nCount--);
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
