/**
  ******************************************************************************
  * @file    I2C/Interrupt/main.h 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Header file for main.c.
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
#ifndef __MAIN_H
#define __MAIN_H
    
    /* Includes ------------------------------------------------------------------ */ 
#include "stm32f10x.h"
    
    /* Exported types ------------------------------------------------------------ */ 
    /* Exported constants -------------------------------------------------------- */ 
#define Tx1BufferSize           4 
#define Tx2BufferSize           4 
#define I2C2_SLAVE_ADDRESS7     0x30
#define Transmitter             0x00
#define Receiver                0x01
    
    /* Exported macro ------------------------------------------------------------ */ 
    /* Exported functions ------------------------------------------------------- */ 
    
#endif  /* __MAIN_H*/
    
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/ 
    
