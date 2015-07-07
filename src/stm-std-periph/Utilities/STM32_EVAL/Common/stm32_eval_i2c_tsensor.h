/**
  ******************************************************************************
  * @file    stm32_eval_i2c_tsensor.h
  * @author  MCD Application Team
  * @version V4.2.0
  * @date    04/16/2010
  * @brief   This file contains all the functions prototypes for the 
  *          stm32_eval_i2c_tsensor firmware driver.
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
    
    /* Define to prevent recursive inclusion ------------------------------------- */ 
#ifndef __STM32_EVAL_I2C_TSENSOR_H
#define __STM32_EVAL_I2C_TSENSOR_H
    
#ifdef __cplusplus
extern "C"
{
    
#endif                          /*  */
    
        /* Includes ------------------------------------------------------------------ */ 
#include "stm32_eval.h"
    
/** @addtogroup Utilities
  * @{
  */ 
    
/** @addtogroup STM32_EVAL
  * @{
  */ 
    
/** @addtogroup Common
  * @{
  */ 
    
/** @addtogroup STM32_EVAL_I2C_TSENSOR
  * @{
  */ 
    
/** @defgroup STM32_EVAL_I2C_TSENSOR_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 
    
/** @defgroup STM32_EVAL_I2C_TSENSOR_Exported_Constants
  * @{
  */ 
    
/**
  * @brief  Block Size
  */ 
#define LM75_REG_TEMP       0x00  /*!< Temperature Register of LM75 */
#define LM75_REG_CONF       0x01  /*!< Configuration Register of LM75 */
#define LM75_REG_THYS       0x02  /*!< Temperature Register of LM75 */
#define LM75_REG_TOS        0x03  /*!< Over-temp Shutdown threshold Register of LM75 */
#define I2C_TIMEOUT         ((uint32_t)0x3FFFF) /*!< I2C Time out */
#define LM75_ADDR           0x90   /*!< LM75 address */
#define LM75_I2C_SPEED      100000 /*!< I2C Speed */
    
/**
  * @}
  */ 
    
/** @defgroup STM32_EVAL_I2C_TSENSOR_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 
    
/** @defgroup STM32_EVAL_I2C_TSENSOR_Exported_Functions
  * @{
  */ 
    void LM75_DeInit (void);
     void LM75_Init (void);
     ErrorStatus LM75_GetStatus (void);
     uint16_t LM75_ReadTemp (void);
     uint16_t LM75_ReadReg (uint8_t RegName);
     void LM75_WriteReg (uint8_t RegName, uint16_t RegValue);
     uint8_t LM75_ReadConfReg (void);
     void LM75_WriteConfReg (uint8_t RegValue);
     void LM75_ShutDown (FunctionalState NewState);
    
#ifdef __cplusplus
} 
#endif                          /*  */
 
#endif  /* __STM32_EVAL_I2C_TSENSOR_H */
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
 
/**
  * @}
  */ 
 
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/ 
