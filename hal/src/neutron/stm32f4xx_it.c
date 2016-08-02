/**
 ******************************************************************************
 * @file    USB_Device/CDC_Standalone/Src/stm32f4xx_it.c
 * @author  MCD Application Team
 * @version V1.3.2
 * @date    13-November-2015
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "service_debug.h"
//#include "cmsis_os.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern ADC_HandleTypeDef ADC_HandleStruct;

extern I2S_HandleTypeDef I2sHandle;

//extern PCD_HandleTypeDef hpcd;

/* UART handler declared in "usbd_cdc_interface.c" file */
extern UART_HandleTypeDef UartHandle;

/* TIM handler declared in "usbd_cdc_interface.c" file */
extern TIM_HandleTypeDef TimHandle;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void HAL_USART1_Handler(UART_HandleTypeDef *huart) __attribute__ ((weak));
void Wiring_USART2_Interrupt_Handler(void) __attribute__ ((weak));
void WifiDrv_USART1_Interrupt_Handler(void);

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
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
void MemManage_Handler(void)
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
void BusFault_Handler(void)
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
void UsageFault_Handler(void)
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
/*void SVC_Handler(void)
  {
  }*/

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
/*void PendSV_Handler(void)
  {
  }*/
#if 0
/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    System1MsTick();
    /* Handle short and generic tasks for the device HAL on 1ms ticks */
    HAL_1Ms_Tick();
    HAL_SysTick_Handler();
    //HAL_System_Interrupt_Trigger(SysInterrupt_SysTick, NULL);
}
#endif
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
#if 0
/**
 * @brief  This function handles USB-On-The-Go FS global interrupt request.
 * @param  None
 * @retval None
 */
void OTG_FS_IRQHandler(void)
{
    HAL_PCD_IRQHandler(&hpcd);
}

/**
 * @brief  This function handles USARTx interrupt request.
 * @param  None
 * @retval None
 */
void USART1_IRQHandler(void)
{
    //HAL_USART1_Handler();
}

/**
 * @brief  This function handles USARTx interrupt request.
 * @param  None
 * @retval None
 */
void USART2_IRQHandler(void)
{
    Wiring_USART2_Interrupt_Handler();
    //HAL_UART_IRQHandler(&UartHandle);
}

#endif

/**
 * @brief  This function handles I2S IT Stream interrupt request.
 * @param  None
 * @retval None
 */
void DMA1_Stream3_IRQHandler(void)
{
    //HAL_DMA_IRQHandler(I2sHandle.hdmarx);
}

extern TIM_HandleTypeDef Timer2Handle;
extern TIM_HandleTypeDef Timer3Handle;
extern TIM_HandleTypeDef Timer4Handle;
extern TIM_HandleTypeDef Timer5Handle;
extern TIM_HandleTypeDef Timer9Handle;
extern TIM_HandleTypeDef Timer10Handle;
extern TIM_HandleTypeDef Timer11Handle;

void TIM1_BRK_TIM9_IRQHandler(void)
{
//    HAL_TIM_IRQHandler(&Timer9Handle);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
//    HAL_TIM_IRQHandler(&Timer10Handle);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
 //   HAL_TIM_IRQHandler(&Timer11Handle);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
